#ifndef __RC522_H
#define __RC522_H 		

#include "spi.h"
#include "main.h"
#include "usart.h"

//******************************************************************/
//                         RC522 引脚定义                          /
/******************************************************************/
#define RC522_SDA_HIGH  	HAL_GPIO_WritePin(GPIOB, RC522_SDA_Pin, GPIO_PIN_SET)  // 片选信号拉高
#define RC522_SDA_LOW  	    HAL_GPIO_WritePin(GPIOB, RC522_SDA_Pin, GPIO_PIN_RESET) // 片选信号拉低

#define RC522_RST_HIGH	    HAL_GPIO_WritePin(GPIOB, RC522_RST_Pin, GPIO_PIN_SET)  // 复位信号拉高
#define RC522_RST_LOW		HAL_GPIO_WritePin(GPIOB, RC522_RST_Pin, GPIO_PIN_RESET) // 复位信号拉低

//******************************************************************/
//                         RC522 FIFO 相关参数                     /
/******************************************************************/
#define DEF_FIFO_LENGTH    64  // FIFO 缓冲区大小 64 字节
#define MAXRLEN           18  // 读写缓冲区最大长度
#define MIN_STRENGTH     228  // 最小信号强度

//******************************************************************/
//                         RC522 命令定义                          /
/******************************************************************/
#define PCD_IDLE          0x00  // 空闲状态
#define PCD_AUTHENT       0x0E  // 验证密钥
#define PCD_RECEIVE       0x08  // 接收数据
#define PCD_TRANSMIT      0x04  // 发送数据
#define PCD_TRANSCEIVE    0x0C  // 发送并接收数据
#define PCD_RESETPHASE    0x0F  // 复位 RC522
#define PCD_CALCCRC       0x03  // 计算 CRC 校验值

//******************************************************************/
//                     Mifare One 卡片命令                         /
/******************************************************************/
#define PICC_REQIDL       0x26  // 轮询寻卡，检测未进入休眠状态的卡
#define PICC_REQALL       0x52  // 轮询寻卡，检测所有符合标准的卡
#define PICC_ANTICOLL1    0x93  // 防冲突命令 1
#define PICC_ANTICOLL2    0x95  // 防冲突命令 2
#define PICC_AUTHENT1A    0x60  // 使用 A 密钥进行认证
#define PICC_AUTHENT1B    0x61  // 使用 B 密钥进行认证
#define PICC_READ         0x30  // 读数据块
#define PICC_WRITE        0xA0  // 写数据块
#define PICC_DECREMENT    0xC0  // 扣款操作
#define PICC_INCREMENT    0xC1  // 充值操作
#define PICC_RESTORE      0xC2  // 将数据从卡片传输到内部寄存器
#define PICC_TRANSFER     0xB0  // 将内部寄存器的数据写入卡片
#define PICC_HALT         0x50  // 让卡片进入休眠状态

//******************************************************************/
//                      RC522 主要寄存器地址                       /
/******************************************************************/
// PAGE 0
#define CommandReg         0x01  // 启动和停止命令
#define ComIEnReg          0x02  // 中断使能寄存器
#define ComIrqReg          0x04  // 中断请求寄存器
#define ErrorReg           0x06  // 错误状态寄存器
#define Status1Reg         0x07  // 通信状态寄存器 1
#define Status2Reg         0x08  // 通信状态寄存器 2
#define FIFODataReg        0x09  // FIFO 缓冲区寄存器
#define FIFOLevelReg       0x0A  // FIFO 缓冲区水平寄存器
#define ControlReg         0x0C  // 控制寄存器
#define BitFramingReg      0x0D  // 位帧设置寄存器
#define CollReg            0x0E  // 冲突检测寄存器

// PAGE 1
#define ModeReg            0x11  // 模式寄存器
#define TxControlReg       0x14  // 天线控制寄存器
#define TxASKReg           0x15  // 100%ASK 调制控制
#define RxSelReg           0x17  // 接收选择寄存器
#define RFCfgReg           0x26  // 接收增益调节寄存器

// PAGE 2
#define CRCResultRegM      0x21  // CRC 计算结果高字节
#define CRCResultRegL      0x22  // CRC 计算结果低字节
#define TModeReg           0x2A  // 定时模式寄存器
#define TPrescalerReg      0x2B  // 定时器预分频寄存器
#define TReloadRegH        0x2C  // 定时器重装值高字节
#define TReloadRegL        0x2D  // 定时器重装值低字节

// PAGE 3
#define VersionReg         0x37  // 版本寄存器

//******************************************************************/
//                          RC522 返回状态码                      /
/******************************************************************/
#define MI_OK             0x00  // 操作成功
#define MI_NOTAGERR       0xFF  // 无卡错误
#define MI_ERR            0xFE  // 操作失败
#define MI_AUTHERR        0xFC  // 认证失败
#define MI_PARITYERR      0xFB  // 校验错误
#define MI_CODEERR        0xFA  // 编码错误
#define MI_WRITEERR       0xF1  // 写入失败
#define MI_READERR        0xEE  // 读取失败
#define MI_COLLERR        0xE8  // 冲突错误

//******************************************************************/
//                          RC522 相关函数声明                     /
/******************************************************************/
void RC522_Init(void);  // 初始化 RC522
char RC522_ReadWriteByte(uint8_t TxData); // SPI 读写函数
void delay_ns(uint32_t ns); // 纳秒级延时

char PcdReset(void);  // 复位 RC522
void PcdAntennaOn(void);  // 开启天线
void PcdAntennaOff(void); // 关闭天线
char PcdRequest(unsigned char req_code, unsigned char *pTagType); // 发送寻卡命令
char PcdAnticoll(unsigned char *pSnr); // 防冲突检测，获取卡片 UID
char PcdSelect(unsigned char *pSnr); // 选定卡片
char PcdAuthState(unsigned char auth_mode, unsigned char addr, unsigned char *pKey, unsigned char *pSnr); // 认证卡片
char PcdRead(unsigned char addr, unsigned char *pData); // 读取卡片数据
char PcdWrite(unsigned char addr, unsigned char *pData); // 向卡片写数据
char PcdValue(unsigned char dd_mode, unsigned char addr, unsigned char *pValue); // 进行加/减值操作
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr); // 备份存储数据
char MF522PcdConfigISOType(unsigned char type); // 设置 ISO14443 工作模式
char PcdComMF522(unsigned char Command, unsigned char *pInData, unsigned char InLenByte, unsigned char *pOutData, unsigned int *pOutLenBit); // 通过 RC522 发送指令

void WriteRawRC(unsigned char Address, unsigned char value); // 写寄存器
unsigned char ReadRawRC(unsigned char Address); // 读寄存器
void SetBitMask(unsigned char reg, unsigned char mask); // 设置寄存器位
void ClearBitMask(unsigned char reg, unsigned char mask); // 清除寄存器位

void ReaderCard(void); // 读卡函数

//******************************************************************/
//                          外部变量声明                           /
/******************************************************************/
extern unsigned char RF_Buffer[18]; 
extern unsigned char UID[5]; 
extern unsigned char Password_Buffer[6]; 

#endif // __RC522_H
