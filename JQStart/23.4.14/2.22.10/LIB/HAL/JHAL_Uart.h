/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CAN默认通用HAL驱动
  使用CAN0 帧类型 默认数据帧非远程帧         若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_UART_H__
#define __JHAL_UART_H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif


    typedef enum
    {
        JHAL_UART0=0,
        JHAL_UART1,
        JHAL_UART_Number
    } JHAL_UART;







    typedef enum
    {
//定长模式（N字节中断 (包头校验时+单字节)）
        JHAL_UART_ReceiveMode_IT_FixedLength=0x01,
//不定长空闲接收中断（单字节中断接收存在接收缓冲区，通过空闲时去取值）
        JHAL_UART_ReceiveITMode_IT_IDLE=0x02,
//DMA定长模式（N字节中断）
        JHAL_UART_ReceiveITMode_IT_DMA_FixedLength=0x03,
    } JHAL_UART_ReceiveMode;


    typedef struct
    {
//接收开始的标志
        bool start;
//接收完成标志位
        bool   end;

//当前接收到的长度记数
        uint16 dataCount;
//单个字节接收时缓冲区
        uint8_t byteData;

//是否使能接收了
        bool itEN;



    } __JHAL_Uart_RxInfo;

    typedef struct
    {

//接收长度 不定长模式下为最大长度
        uint16 length;
//（包头和CRC模式是一起的,只有NONE时候不校验包头）
        uint8   headCode ;
//接收模式
        JHAL_UART_ReceiveMode receiveMode;
//接收CRC模式
        JHAL_CRC_Mode crcMode;
//数据接收缓冲区 需要全局的并且大于等于接收长度
        uint8_t *dataBuff;

        __JHAL_Uart_RxInfo __rxInfo;
    } JHAL_UART_RXConfig;


    typedef struct {
        uint32_t baudRate;
        JHAL_UART_RXConfig rxConfig;

    } JHAL_UARTConfig;



    bool JHAL_uartSendDatas(JHAL_UART uart,u8* data,u16  length,JHAL_CRC_Mode mode);
    /*config全局不可被释放*/
    bool JHAL_uartInit(JHAL_UART uart,JHAL_UARTConfig *config);
    bool  JHAL_uartDeInit(JHAL_UART uart);


    /*使能接收 当到串口接收的数据满足配置文件中的要求时视为本次接收完成  给 JHAL_uartRxFinsh 的返回值置true */
    void JHAL_uartEnableReceiveIT (JHAL_UART uart);

    void JHAL_uartAbortReceiveIT (JHAL_UART uart);

    /*返回值：实际接收长度 当返回值不为0时即完成本次接收 实际接收数据在初始化的 dataBuff 中*/
    u16 JHAL_uartRxFinsh(JHAL_UART uart);

//中断回调 返回false将视为取消配置中接收封装方法  JHAL_uartRxFinsh将无效  配置中*dataBuff 不会得到赋值
    bool JHAL_uartRxITCallback(u8 uartID);



#ifdef CplusPlus
}
#endif

#endif








