#include "rc522.h"
#include "stdio.h"
#include "tim.h"
#include "sg90.h"
#include "usart.h"

#define DEBUG_LOG 0
#if DEBUG_LOG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

uint8_t UID[5], Temp[4];          
uint8_t RF_Buffer[18];
uint8_t Password_Buffer[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 默认密码

void RC522_Init(void)
{
    RC522_SDA_LOW;
    HAL_SPI_Transmit(&hspi2, (uint8_t *)"\xAA", 1, 0xFF);	
    RC522_SDA_HIGH;

    HAL_Delay(20); // 改短延时

    PcdReset(); 
    HAL_Delay(2);

    PcdAntennaOff(); 
    HAL_Delay(2);	 
    PcdAntennaOn(); 
    
    printf("RC522 init succeed\r\nstart to detect card!\r\n");
}

void delay_ns(uint32_t ns)
{
    for(uint32_t i = 0; i < ns; i++)
    {
        __nop(); __nop(); __nop();
    }
}

char RC522_ReadWriteByte(uint8_t TxData)
{					 	
    while ((SPI2->SR & SPI_SR_TXE) == RESET);
    SPI2->DR = TxData;
    while ((SPI2->SR & SPI_SR_RXNE) == RESET);
    return SPI2->DR;				    
}

unsigned char ReadRawRC(unsigned char Address)
{
    uint8_t ucAddr = ((Address << 1) & 0x7E) | 0x80;
    uint8_t ucResult;

    RC522_SDA_LOW;
    HAL_Delay(2);
    RC522_ReadWriteByte(ucAddr);
    HAL_Delay(2);
    ucResult = RC522_ReadWriteByte(0);
    HAL_Delay(2);
    RC522_SDA_HIGH;

    return ucResult;
}

void WriteRawRC(unsigned char Address, unsigned char value)
{
    uint8_t ucAddr = ((Address << 1) & 0x7E);

    RC522_SDA_LOW;
    HAL_Delay(2);
    RC522_ReadWriteByte(ucAddr);
    HAL_Delay(2);
    RC522_ReadWriteByte(value);
    HAL_Delay(2);
    RC522_SDA_HIGH;
}

void SetBitMask(unsigned char reg, unsigned char mask)  
{
    char tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp | mask); 
}

void ClearBitMask(unsigned char reg, unsigned char mask)  
{
    char tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & (~mask));
}

char PcdReset()
{
    RC522_RST_HIGH;
    delay_ns(10);
    RC522_RST_LOW;
    delay_ns(100);
    RC522_RST_HIGH;
    delay_ns(10);

    WriteRawRC(CommandReg, PCD_RESETPHASE);
    delay_ns(100);

    WriteRawRC(ModeReg, 0x3D);
    WriteRawRC(TReloadRegL, 30);
    WriteRawRC(TReloadRegH, 0);
    WriteRawRC(TModeReg, 0x8D);
    WriteRawRC(TPrescalerReg, 0x3E);
    WriteRawRC(TxASKReg, 0x40);

    return MI_OK;
}

char MF522PcdConfigISOType(unsigned char type)
{
    if (type == 'A')
    {
        ClearBitMask(Status2Reg, 0x08);	
        WriteRawRC(ModeReg, 0x3D);
        WriteRawRC(RxSelReg, 0x86);
        WriteRawRC(RFCfgReg, 0x7F);
        WriteRawRC(TReloadRegL, 30);
        WriteRawRC(TReloadRegH, 0);
        WriteRawRC(TModeReg, 0x8D);
        WriteRawRC(TPrescalerReg, 0x3E);
        delay_ns(1000);
        PcdAntennaOn();
    } else {
        return 0xFE;
    }
    return MI_OK;
}

void PcdAntennaOn()
{
    if (!(ReadRawRC(TxControlReg) & 0x03))
        SetBitMask(TxControlReg, 0x03);
}

void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}

char PcdComMF522(unsigned char Command, unsigned char *pInData, 
                 unsigned char InLenByte, unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn = 0x00, waitFor = 0x00, lastBits, n;
    unsigned int i;

    switch (Command)
    {
        case PCD_AUTHENT: irqEn = 0x12; waitFor = 0x10; break;
        case PCD_TRANSCEIVE: irqEn = 0x77; waitFor = 0x30; break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80);
    ClearBitMask(ComIrqReg, 0x80);
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);

    for (i = 0; i < InLenByte; i++)
        WriteRawRC(FIFODataReg, pInData[i]);

    WriteRawRC(CommandReg, Command);
    if (Command == PCD_TRANSCEIVE)
        SetBitMask(BitFramingReg, 0x80);

    i = 3000; // 原为 6000，提升响应速度
    do {
        n = ReadRawRC(ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));

    ClearBitMask(BitFramingReg, 0x80);

    if (i && !(ReadRawRC(ErrorReg) & 0x1B))
    {
        status = MI_OK;
        if (n & irqEn & 0x01) status = MI_NOTAGERR;
        if (Command == PCD_TRANSCEIVE)
        {
            n = ReadRawRC(FIFOLevelReg);
            lastBits = ReadRawRC(ControlReg) & 0x07;
            *pOutLenBit = lastBits ? (n - 1) * 8 + lastBits : n * 8;
            if (n == 0) n = 1;
            if (n > MAXRLEN) n = MAXRLEN;
            for (i = 0; i < n; i++) pOutData[i] = ReadRawRC(FIFODataReg);
        }
    }

    SetBitMask(ControlReg, 0x80);
    WriteRawRC(CommandReg, PCD_IDLE);

    return status;
}

char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
    char status;  
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
 
    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x07);
    SetBitMask(TxControlReg, 0x03);
 
    ucComMF522Buf[0] = req_code;
    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);
    
    if ((status == MI_OK) && (unLen == 0x10))
    {    
        pTagType[0] = ucComMF522Buf[0];
        pTagType[1] = ucComMF522Buf[1];
    }
    else status = MI_ERR;
    
    return status;
}

char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i, snr_check = 0;
    unsigned int unLen;
    unsigned char buf[MAXRLEN]; 
    
    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x00);
    ClearBitMask(CollReg, 0x80);

    buf[0] = PICC_ANTICOLL1;
    buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE, buf, 2, buf, &unLen);

    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            pSnr[i] = buf[i];
            snr_check ^= buf[i];
        }
        if (snr_check != buf[i]) status = MI_ERR;
    }

    SetBitMask(CollReg, 0x80);
    return status;
}

extern void send_response(uint8_t response, ParsedData data, UART_HandleTypeDef *huart);

void ReaderCard(void)
{
    char temp_value;
    uint32_t t_start = HAL_GetTick();

    static uint8_t last_uid[4] = {0};
    static uint32_t last_time = 0;
    uint32_t now = HAL_GetTick();

    if (PcdRequest(PICC_REQALL, Temp) == MI_OK)
    {
        if (PcdAnticoll(UID) == MI_OK)
        {
            // 如果UID与上次相同，且时间差小于1000ms，跳过处理（消抖）
            if (memcmp(UID, last_uid, 4) == 0 && (now - last_time < 1000))
            {
                return;
            }

            // 记录当前UID和时间
            memcpy(last_uid, UID, 4);
            last_time = now;

            printf("Card UID: %02X%02X%02X%02X\r\n", UID[0], UID[1], UID[2], UID[3]);
            temp_value = ((UID[0] >> 4) * 10 + (UID[0] & 0x0F));

            switch (temp_value)
            {
                case 77:
                    printf("Valid ID: %d\r\n", temp_value);
                    if (door_open())
                    {
                        ParsedData resp = { .device_id = 0x1, .switch_state = 1 };
                        send_response(0x1, resp, &huart2);
                    }
                    break;

                default:
                    printf("Invalid ID: %d\r\n", temp_value);
                    if (door_close())
                    {
                        ParsedData resp = { .device_id = 0x1, .switch_state = 0 };
                        send_response(0x1, resp, &huart2);
                    }
                    break;
            }
        }
    }

    uint32_t t_end = HAL_GetTick();
//    printf("刷卡耗时: %lu ms\r\n", (t_end - t_start));
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//    if(htim->Instance == TIM1)
//    {
//        ReaderCard();
//    }
}
