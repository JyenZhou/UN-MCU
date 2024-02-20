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
#ifdef __cplusPlus
extern "C" {
#endif






    typedef enum
    {
//定长模式（N字节中断 (包头校验时+单字节)）
        JHAL_UART_ReceiveMode_IT_FixedLength=0x01,
//不定长空闲中断 DMA接收
        JHAL_UART_ReceiveITMode_IT_DMA_IDLE=0x02,



    } JHAL_UART_ReceiveMode;


    typedef struct
    {
//接收开始的标志
        bool start:1;
//接收完成标志位
        bool   end:1;


        bool crcOK:1;
//是否使能接收了
        bool itEN:1;

        //当前接收到的长度记数
        uint16 dataCount;
//单个字节接收时缓冲区
        uint8_t byteData;



    } __JHAL_Uart_RxOtherInfo;





    typedef struct
    {
        bool isOpen:1;



    } __JHAL_Uart__OtherInfo;

    typedef struct
    {

        //数据接收缓冲区 需要全局的并且大于等于接收长度
        uint8_t *dataBuff ;
//接收长度 不定长模式下为最大长度 就是dataBuff长度
        uint16 length;
        //接收CRC模式
        JHAL_CRC_Mode crcMode;
//（包头和CRC模式是一起的,只有NONE时候不校验包头）
        uint8   headCode ;
//接收模式
        JHAL_UART_ReceiveMode receiveMode;



        __JHAL_Uart_RxOtherInfo __info;


    } JHAL_UART_RXConfig;


    typedef struct {
        u8 id:3;
        u32 baudRate;
        JHAL_UART_RXConfig rxConfig;
        __JHAL_Uart__OtherInfo __info;
        // 给第三方适配用的 例如stm32使用cubemx配置后直接将那边定义的变量传进来 不需要内部重新赋值等
//			如果内部使用了这个变量一定要先赋值
        void*  dev;

    } JHAL_UART ;



    bool JHAL_uartSendDatas(JHAL_UART *uart,u8* data,u16  length,JHAL_CRC_Mode mode);
    /*config全局不可被释放  */
    bool JHAL_uartOpen ( JHAL_UART  *uart);
    bool  JHAL_uartClose(JHAL_UART *uart);


    /*使能接收 当到串口接收的数据满足配置文件中的要求时视为本次接收完成  给 JHAL_uartRxFinsh 的返回值置true */
    void JHAL_uartEnableReceiveIT ( JHAL_UART *uart);

    void JHAL_uartAbortReceiveIT (JHAL_UART *uart);

    /*返回值：实际接收长度 当返回值不为0时即完成本次接收 实际接收数据在初始化的 dataBuff 中*/
    u16 JHAL_uartRxFinsh(JHAL_UART *uart);

//中断回调 返回false将视为取消配置中接收封装方法  JHAL_uartRxFinsh将无效  配置中*dataBuff 不会得到赋值
    //中断回调  注意这里对应的是uart实例 id 不是传进去的dev  因为两个dev可能实际上是一个uart 只是引脚不一样
    bool JHAL_uartRxITCallback(u8 realityId);

//注册printf对应的串口  放在.c中而不是函数中 一般放在配置文件中
#define  JHAL_UartRegistPrintf(JUART)   JHAL_UART *   __JHAL_uartGetPrintfJUart(){ JHAL_UART *printf=JUART; return  printf;  }

#ifdef CplusPlus
}
#endif

#endif








