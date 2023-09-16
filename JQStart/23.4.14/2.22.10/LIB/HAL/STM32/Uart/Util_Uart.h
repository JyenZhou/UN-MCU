#ifdef UTIL_UART


/*使能指定串口
***/
#define UTIL_UART1_EN
//#define UTIL_UART2_EN
//#define UTIL_UART3_EN
//#define UTIL_UART4_EN
//#define UTIL_UART5_EN
#define UTIL_UART6_EN
#define UTIL_UART7_EN
//#define UTIL_UART8_EN


/****************************/


#ifndef _UTIL_UART_H_
#define _UTIL_UART_H_
#include "Hall_includes.h"



#ifdef UTIL_UART1_EN
#define UTIL_UART1_RXBUFF_MAXSIZE 70
#endif

#ifdef UTIL_UART2_EN
#define UTIL_UART2_RXBUFF_MAXSIZE 30
#endif

#ifdef UTIL_UART3_EN
#define UTIL_UART3_RXBUFF_MAXSIZE 7
#endif
#ifdef UTIL_UART4_EN
#define UTIL_UART4_RXBUFF_MAXSIZE  75
#endif

#ifdef UTIL_UART5_EN
#define UTIL_UART5_RXBUFF_MAXSIZE 100
#endif
#ifdef UTIL_UART6_EN
#define UTIL_UART6_RXBUFF_MAXSIZE 35
#endif

#ifdef UTIL_UART7_EN
#define UTIL_UART7_RXBUFF_MAXSIZE 1
#endif
#ifdef UTIL_UART8_EN
#define UTIL_UART8_RXBUFF_MAXSIZE 35
#endif
typedef enum
{
//定长模式（N字节中断 (包头校验时+单字节)）
    UtilUart_ReceiveMode_FixedLength=0x01,
//不定长空闲接收中断（单字节中断接收存在接收缓冲区，通过空闲时去取值）
    UtilUart_ReceiveMode_IDLE=0x02,
//DMA定长模式（N字节中断）
    UtilUart_ReceiveMode_DMA_FixedLength=0x03,
} UtilUart_ReceiveMode_E;


typedef enum
{
//只有 UtilUart_NONE  （包头和CRC模式是一起的,只有NONE时候不校验包头）
    UtilUart_CRC_NONE,
    UtilUart_CRC_XorBCC,
   UtilUart_CRC_CRC16_Modbus,

} UtilUart_CRCMode_E;


typedef struct
{
//接收开始的标志
    bool receivStart;

//包头校验标志位当 校验失败该标志置为true  同时多字节中断改成单字节 直到一包完整后切回来
    bool     lastCrcCheckOK ;
//当前接收到的长度记数
    uint16 receiveDataCount;
//单个字节接收时缓冲区
    uint8_t receiveDataPartial;

//数据接收
    uint8_t *receiveData;

//是否使能接收了
    bool receiveITEN;

} UtilUart_Handler_T_Private;
typedef struct
{

//绑定的串口
//id给debug用的
    uint8_t id;
    UART_HandleTypeDef  *huart;

//接收长度 定长模式下生效
    uint16 receiveLength;

//（包头和CRC模式是一起的,只有NONE时候不校验包头）
    uint8   rxHeadCode ;
//接收模式
    UtilUart_ReceiveMode_E receiveMode;
//接收CRC模式
    UtilUart_CRCMode_E receiveCRCMode;
//发送时CRC模式
    UtilUart_CRCMode_E sendCRCMode;

//接收完成标志位只读
    volatile    bool   receiveFinished;

    UtilUart_Handler_T_Private privateParameter;


} UtilUart_Handler_T;




#ifdef UTIL_UART1_EN
extern uint8   utilUart_1_RxBuff[];
extern UtilUart_Handler_T  utilUart_1_Handle;
extern UART_HandleTypeDef  huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
#endif


#ifdef UTIL_UART2_EN
extern uint8   utilUart_2_RxBuff[];
extern UtilUart_Handler_T  utilUart_2_Handle;
extern UART_HandleTypeDef  huart2

#endif
#ifdef UTIL_UART3_EN
extern uint8   utilUart_3_RxBuff[];
extern UtilUart_Handler_T  utilUart_3_Handle;
extern UART_HandleTypeDef  huart3;

#endif

#ifdef UTIL_UART4_EN
extern uint8   utilUart_4_RxBuff[];
extern UtilUart_Handler_T utilUart_4_Handle;
extern UART_HandleTypeDef  huart4;
#endif



#ifdef UTIL_UART5_EN
extern uint8   utilUart_5_RxBuff[];
extern UtilUart_Handler_T utilUart_5_Handle;
extern UART_HandleTypeDef  huart5
#endif

#ifdef UTIL_UART6_EN
extern UtilUart_Handler_T utilUart_6_Handle;
extern UART_HandleTypeDef  huart6;
extern uint8   utilUart_6_RxBuff[];
#endif



#ifdef UTIL_UART7_EN
extern uint8   utilUart_7_RxBuff[];
extern UtilUart_Handler_T utilUart_7_Handle;
extern UART_HandleTypeDef  huart7;
#endif


#ifdef UTIL_UART8_EN
extern uint8   utilUart_8_RxBuff[];
extern UtilUart_Handler_T utilUart_8_Handle;
extern UART_HandleTypeDef  huart8;
#endif



//初始化 由系统自动调用
void utilUart_Init(void);

//使用过程中动态的修改配置参数需要 先关闭接收，修改参数，调用utilUart_Configuration，再打开接收）
void  utilUart_Configuration(UtilUart_Handler_T *utilUartHandler  );

//使能空闲中断时 放在串口中断里面的
void utilUart_RxIdleCallback(UART_HandleTypeDef *huart);
//串口普通发送函数
void utilUart_sendDat(UtilUart_Handler_T *utilUartHandler, uint8 *dat,uint16 size);

//使能接收
void utilUart_Receive_IT (UtilUart_Handler_T *utilUart_Handle);
//使能接收
void utilUart_AbortReceive_IT (UtilUart_Handler_T *utilUart_Handle);

#endif

#endif









