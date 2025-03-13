#include "rc522.h"
#include "stdio.h"
#include "tim.h"
#include "sg90.h"

// SPI2 引脚定义
// SPI2_SCK      -> PB13
// SPI2_MISO     -> PB14
// SPI2_MOSI     -> PB15
// RC522_RST(CE) -> PB9
// RC522_NSS(SDA)-> PB8
// RC522_IRQ     -> (未使用)

uint8_t UID[5], Temp[4];          
uint8_t RF_Buffer[18];
uint8_t Password_Buffer[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Mifare One 默认密码

/**
 * @brief  RC522 初始化
 * @note   初始化 SPI 通信、复位 RC522 并开启天线
 * @retval 无
 */
void RC522_Init(void)
{
    RC522_SDA_LOW;
    HAL_SPI_Transmit(&hspi2, (uint8_t *)0xaa, sizeof((uint8_t *)0xaa), 0xFF); // 发送初始化命令	
    RC522_SDA_HIGH;

    HAL_Delay(50);

    PcdReset(); // 复位 RC522
    HAL_Delay(10);
    PcdReset(); // 再次复位 RC522
    HAL_Delay(10);
    
    PcdAntennaOff(); // 关闭天线
    HAL_Delay(10);	 
    PcdAntennaOn(); // 打开天线
    
    printf("RFID-MFRC522 init succeed\r\nstart to detect card!\r\n");  // 打印初始化完成信息
	
//		HAL_TIM_Base_Start_IT(&htim1);
}

/**
 * @brief  延时函数 (纳秒级)
 * @param  ns: 延迟的纳秒数
 * @retval 无
 */
void delay_ns(uint32_t ns)
{
    uint32_t i;
    for(i = 0; i < ns; i++)
    {
        __nop();
        __nop();
        __nop();
    }
}

/**
 * @brief  SPI 发送和接收数据
 * @param  TxData: 发送的数据
 * @retval 接收到的数据
 */
char RC522_ReadWriteByte(uint8_t TxData)
{					 	
    while (SPI_CHECK_FLAG(SPI2->SR, ((uint16_t)0x0002)) == RESET) 
    {
        ; // 等待发送缓冲区空
    }			  

    SPI2->DR = TxData; // 发送数据
    
    while (SPI_CHECK_FLAG(SPI2->SR, ((uint16_t)0x0001)) == RESET) 
    {
        ; // 等待接收完成
    }	  						    

    return SPI2->DR; // 返回接收到的数据				    
}
 
//******************************************************************/
// 函数名: ReadRawRC
// 功  能: 读取 RC522 指定寄存器的值
// 参  数: 
//        Address[IN]: 寄存器地址
// 返  回: 读取到的寄存器值
//******************************************************************/
unsigned char ReadRawRC(unsigned char Address)
{
    uint8_t ucAddr;
    uint8_t ucResult = 0;

    RC522_SDA_LOW; // 选中 RC522
    HAL_Delay(10);
    
    ucAddr = ((Address << 1) & 0x7E) | 0x80; // 计算读取地址（最高位设为 1 表示读取）
    RC522_ReadWriteByte(ucAddr); // 发送读取命令
    HAL_Delay(10);
    
    ucResult = RC522_ReadWriteByte(0); // 读取数据
    HAL_Delay(10);
    
    RC522_SDA_HIGH; // 取消选中 RC522
    return ucResult;
}

//******************************************************************/
// 函数名: WriteRawRC
// 功  能: 向 RC522 指定寄存器写入数据
// 参  数: 
//        Address[IN]: 寄存器地址
//        value[IN]  : 要写入的值
// 返  回: 无
//******************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{
    uint8_t ucAddr;

    RC522_SDA_LOW; // 选中 RC522
    HAL_Delay(10);

    ucAddr = ((Address << 1) & 0x7E); // 计算写入地址（最高位设为 0 表示写入）
    RC522_ReadWriteByte(ucAddr); // 发送写入命令
    HAL_Delay(10);

    RC522_ReadWriteByte(value); // 发送数据
    HAL_Delay(10);

    RC522_SDA_HIGH; // 取消选中 RC522
}

 
//******************************************************************/
// 函数名: SetBitMask
// 功  能: 设置 RC522 指定寄存器的位
// 参  数: 
//        reg[IN]: 寄存器地址
//        mask[IN]: 需要设置的位掩码
//******************************************************************/
void SetBitMask(unsigned char reg, unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg) | mask;
    WriteRawRC(reg, tmp | mask);  // 设置指定位
}

//******************************************************************/
// 函数名: ClearBitMask
// 功  能: 清除 RC522 指定寄存器的位
// 参  数: 
//        reg[IN]: 寄存器地址
//        mask[IN]: 需要清除的位掩码
//******************************************************************/
void ClearBitMask(unsigned char reg, unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg) & (~mask);
    WriteRawRC(reg, tmp);  // 清除指定位
}

//******************************************************************/
// 函数名: PcdReset
// 功  能: 复位 RC522
// 返  回: 成功返回 MI_OK
//******************************************************************/
char PcdReset()
{
    RC522_RST_HIGH; // 复位引脚拉高
    delay_ns(10);   // 延迟 10ns
    RC522_RST_LOW;  // 复位引脚拉低
    delay_ns(100);  // 延迟 100ns
    RC522_RST_HIGH; // 复位引脚拉高
    delay_ns(10);   // 延迟 10ns

    WriteRawRC(CommandReg, PCD_RESETPHASE); // 发送复位命令
    delay_ns(100); // 延迟 100ns 让 RC522 复位完成

    WriteRawRC(ModeReg, 0x3D);      // 进入 Mifare 读卡模式，CRC 初始值 0x6363
    WriteRawRC(TReloadRegL, 30);    // 设置 16 位定时器低字节
    WriteRawRC(TReloadRegH, 0);     // 设置 16 位定时器高字节
    WriteRawRC(TModeReg, 0x8D);     // 启用定时器自动模式
    WriteRawRC(TPrescalerReg, 0x3E);// 设置定时器预分频器
    WriteRawRC(TxASKReg, 0x40);     // 设定 100% ASK 调制模式

    return MI_OK; // 复位成功
}

//******************************************************************/
// 函数名: MF522PcdConfigISOType
// 功  能: 配置 RC522 的 ISO14443 工作模式
// 参  数: 
//        type:  设定协议类型，'A' 代表 ISO14443_A
// 返回值: 成功返回 MI_OK，失败返回 0xFE
//******************************************************************/
char MF522PcdConfigISOType(unsigned char type)
{
    if (type == 'A')	// ISO14443_A 协议
    {
        ClearBitMask(Status2Reg, 0x08);	// 关闭加密模式
        
        WriteRawRC(ModeReg, 0x3D);	    // 设定 Mifare 读卡模式，CRC 初始值 0x6363
        WriteRawRC(RxSelReg, 0x86);	    // 设定接收器增益，提高信号接收能力
        WriteRawRC(RFCfgReg, 0x7F);	    // 设置增益最大 48dB
        
        WriteRawRC(TReloadRegL, 30);	// 设置超时时间低字节
        WriteRawRC(TReloadRegH, 0);	    // 设置超时时间高字节
        
        WriteRawRC(TModeReg, 0x8D);	    // 设定自动模式
        WriteRawRC(TPrescalerReg, 0x3E); // 设定定时器预分频
        
        delay_ns(1000); // 延迟 1 微秒，确保配置生效
        
        PcdAntennaOn(); // 打开天线
    }     
    else 
    {
        return 0xFE; // 失败，返回错误码
    }
    
    return MI_OK; 
}  

//******************************************************************/
// 函数名: PcdAntennaOn
// 功  能: 打开 RC522 天线
// 参  数: 无
// 返回值: 无
//******************************************************************/
void PcdAntennaOn()
{
    unsigned char i;
    WriteRawRC(TxASKReg, 0x40); // 设定 100%ASK 调制方式
    HAL_Delay(1); // 延迟 1ms，等待天线稳定
    
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03)) // 检查 TX1RF 和 TX2RF 是否打开
        SetBitMask(TxControlReg, 0x03); // 打开 TX1RF 和 TX2RF，使能天线
    
    i = ReadRawRC(TxASKReg); // 读取 TxASKReg 以确认设置
}

 
 
//******************************************************************/
// 函数名: PcdAntennaOff
// 功  能: 关闭 RC522 天线
// 参  数: 无
// 返回值: 无
//******************************************************************/
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03); // 清除 TX1RFen 和 TX2RFen 位，关闭天线
}

 
//******************************************************************/
// 函数名: PcdComMF522
// 功  能: 通过 RC522 发送 ISO14443 命令并接收数据
// 参  数:
//        Command[IN]        : RC522 指令
//        pInData[IN]       : 需要发送到 RC522 的数据
//        InLenByte[IN]     : 发送数据的字节数
//        pOutData[OUT]     : 接收 RC522 返回的数据
//        *pOutLenBit[OUT]  : 接收数据的位数
// 返回值: 成功返回 MI_OK，失败返回错误码
//******************************************************************/
char PcdComMF522(unsigned char Command, unsigned char *pInData, 
                 unsigned char InLenByte, unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;

    // 根据不同的指令设置中断使能和等待标志
    switch (Command)
    {
        case PCD_AUTHENT:
            irqEn = 0x12;  // 认证指令的中断使能
            waitFor = 0x10; // 认证等待标志
            break;
        case PCD_TRANSCEIVE:
            irqEn = 0x77;  // 发送/接收指令的中断使能
            waitFor = 0x30; // 发送/接收等待标志
            break;
        default:
            break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80); // 使能 RC522 的中断
    ClearBitMask(ComIrqReg, 0x80);       // 清除所有中断标志
    WriteRawRC(CommandReg, PCD_IDLE);    // 让 RC522 进入空闲状态

    SetBitMask(FIFOLevelReg, 0x80); // 清空 FIFO 缓冲区

    // 将数据写入 FIFO
    for (i = 0; i < InLenByte; i++)
        WriteRawRC(FIFODataReg, pInData[i]);

    WriteRawRC(CommandReg, Command); // 发送指令

    if (Command == PCD_TRANSCEIVE)
        SetBitMask(BitFramingReg, 0x80); // 启动发送

    i = 6000; // 设置超时时间，M1 卡最大等待时间约 25ms

    // 等待指令执行完成
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i != 0) && !(n & 0x01) && !(n & waitFor));

    ClearBitMask(BitFramingReg, 0x80); // 关闭发送模式

    if (i != 0) // 判断是否超时
    {
        if (!(ReadRawRC(ErrorReg) & 0x1B)) // 检查是否有错误
        {
            status = MI_OK;

            if (n & irqEn & 0x01)
                status = MI_NOTAGERR; // 无卡错误

            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg); // 读取 FIFO 中的数据长度
                lastBits = ReadRawRC(ControlReg) & 0x07; // 读取最后的位数

                if (lastBits)
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                else
                    *pOutLenBit = n * 8;

                if (n == 0)
                    n = 1;

                if (n > MAXRLEN)
                    n = MAXRLEN;

                // 读取 FIFO 数据
                for (i = 0; i < n; i++)
                    pOutData[i] = ReadRawRC(FIFODataReg);
            }
        }
        else
        {
            status = MI_ERR;
        }
    }

    SetBitMask(ControlReg, 0x80); // 停止计时器
    WriteRawRC(CommandReg, PCD_IDLE); // 让 RC522 进入空闲模式

    return status;
}

 
//******************************************************************/
// 函数名: PcdRequest
// 功  能: 发送请求命令，检测是否有符合的卡片
// 参  数: 
//        req_code[IN]: 请求代码
//            0x52 = 全部寻卡，检测所有符合 ISO14443A 标准的卡片
//            0x26 = 唤醒模式，检测处于休眠状态的卡片
//        pTagType[OUT]: 识别到的卡片类型
//            0x4400 = Mifare_UltraLight
//            0x0400 = Mifare_One(S50)
//            0x0200 = Mifare_One(S70)
//            0x0800 = Mifare_Pro(X)
//            0x4403 = Mifare_DESFire
// 返回值: 成功返回 MI_OK，失败返回错误码
//******************************************************************/
char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
    char status;  
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
 
    ClearBitMask(Status2Reg, 0x08);  // 清除 MRCrypto1on 标志，关闭加密模式
    WriteRawRC(BitFramingReg, 0x07); // 设置起始发送位为 0，接收对齐为 0，FIFO 数据完整，最后 7 位 TX 发送
    SetBitMask(TxControlReg, 0x03);  // 开启 TX1RF 和 TX2RF，使读写器发出 13.56MHz 的射频信号
 
    ucComMF522Buf[0] = req_code; // 设置请求代码
 
    // 发送请求命令并接收响应
    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);
    
    if ((status == MI_OK) && (unLen == 0x10)) // 判断响应数据是否有效
    {    
        *pTagType     = ucComMF522Buf[0]; // 读取卡片类型
        *(pTagType+1) = ucComMF522Buf[1];
    }
    else
    {
        status = MI_ERR; // 失败返回错误码
    }
    
    return status;
}

 
//******************************************************************/
// 函数名: PcdAnticoll
// 功  能: 防冲突处理，获取卡片序列号
// 参  数: pSnr[OUT]：卡片序列号，长度为4字节
// 返回值: 成功返回 MI_OK，失败返回错误码
//******************************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i, snr_check = 0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ClearBitMask(Status2Reg, 0x08); // 清除 MRCrypto1on 标志，关闭加密模式
    WriteRawRC(BitFramingReg, 0x00); // 取消位寻址，设置为标准模式
    ClearBitMask(CollReg, 0x80); // 允许多个卡片同时存在，即关闭防冲突保护

    ucComMF522Buf[0] = PICC_ANTICOLL1; // 发送防冲突命令
    ucComMF522Buf[1] = 0x20; // 指定防冲突级别

    // 发送命令并接收数据
    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK)
    {
        // 读取卡片序列号
        for (i = 0; i < 4; i++)
        {   
            *(pSnr + i) = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i]; // 计算校验和
        }

        // 校验序列号
        if (snr_check != ucComMF522Buf[i])
        {   
            status = MI_ERR;    
        }
    }
    
    SetBitMask(CollReg, 0x80); // 重新启用防冲突保护，避免多个卡片干扰
    return status;
}

 
 
 
//==============================================================================
// 读取卡片信息
// 识别卡片ID
//==============================================================================
void ReaderCard(void)
{
	char temp_value;
    
	if(PcdRequest(PICC_REQALL,Temp)==MI_OK)	// 发送请求，检测是否有卡片
	{
		if(Temp[0]==0x04&&Temp[1]==0x00)  
				printf("\r\nMFOne-S50\r\n");  // 识别MFOne-S50卡
		else if(Temp[0]==0x02&&Temp[1]==0x00)
			printf("MFOne-S70");  // 识别MFOne-S70卡
		else if(Temp[0]==0x44&&Temp[1]==0x00)
			printf("MF-UltraLight");  // 识别MF-UltraLight卡
		else if(Temp[0]==0x08&&Temp[1]==0x00)
			printf("MF-Pro");  // 识别MF-Pro卡
		else if(Temp[0]==0x44&&Temp[1]==0x03)
			printf("MF Desire");  // 识别MF Desire卡
		else
			printf("Unknown");  // 未知卡片类型

		if(PcdAnticoll(UID)==MI_OK)	// 进行防冲突处理，获取卡片唯一ID
		{ 
			printf("Card Id is:");
			/* 输出卡片ID */
			printf("%d%d%d%d\r\n",UID[0],UID[1],UID[2],UID[3]); 		              

			// 解析UID的第一个字节并进行身份验证
			temp_value = ((UID[0]>>4)*10+(UID[0]&0x0f));
			switch(temp_value)
			{
				case 80 : 
					printf("valid id : %d, succeed!\r\n",temp_value);
					door_open();
				break;
				case 41 : 
					printf("valid id : %d, succeed!\r\n",temp_value);
					door_open();
				break;
				default : 
					printf("invalid id :%d，faild!\r\n",temp_value);
					door_close();
				break;
			}	                             
		}
  } 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
//		printf("entern nfc check irq\n");
//		ReaderCard();
	}
}
