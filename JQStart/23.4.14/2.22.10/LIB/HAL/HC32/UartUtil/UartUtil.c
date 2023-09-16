#include "uartUtil.h"

#define     T1_PORT                 (3)
#define     T1_PIN                  (3)


//已接收到的长度记录
uint16 volatile Uart1_ReceivingCount=0;
//接收标志位  接收完成后 置1  也就是把这个标志位放在while里面  当他为1的时候就可以执行解包RecData了
unsigned char volatile RecEnd_Uart1=0;
//接收开始标志位
unsigned char volatile RecStart_Uart1=0;
//串口发送状态  主要用来处理 采用了中断模式时 我的串口发送时虽然关闭了接收 但是这时候还在接收中 会导致处理完把串口又打开了这样导致不一致
unsigned char volatile Sendingstatus_Uart1=0;


//我需要的不定长接收缓冲区 发送过来的别超出这个接收区域大小，这个在接收时候会有一个判断
unsigned char   ReceiveData_Uart1[Uart1_ReceivingLength]= {0};

void Uart1_RxIntCallback(void)
{
//Uart_SendData(UARTCH1,M0P_UART1->SBUF);
    if(RecEnd_Uart1==1)
        return;
    uint8 aRxBuffer_UART1=M0P_UART1->SBUF;
    if(aRxBuffer_UART1==0xFF&&RecStart_Uart1==0) //'FF',start recevie package
    {   Uart1_ReceivingCount = 0;
        RecStart_Uart1 = 1;
        ReceiveData_Uart1[Uart1_ReceivingCount++] = aRxBuffer_UART1;
    }
    else  if(RecStart_Uart1) {

        ReceiveData_Uart1[Uart1_ReceivingCount++] = aRxBuffer_UART1;
        if(Uart1_ReceivingCount==Uart1_ReceivingLength)
        {
            //标志接收完成
            RecEnd_Uart1 = 1;
            Uart1_ReceivingCount = 0;
            RecStart_Uart1 = 0;


        }


    }



}





void ErrIntCallback(void)
{

}


void uartUtilinit()
{

    uint16_t timer=0;
    uint32_t pclk=0;

    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;


    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);


//    Gpio_InitIO(T1_PORT,T1_PIN,GpioDirIn);
//    Gpio_InitIO(0,3,GpioDirOut);
//    Gpio_SetIO(0,3,1);

    Gpio_InitIOExt(0,1,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    Gpio_InitIOExt(0,2,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
//Gpio_InitIOExt(3,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
//    Gpio_InitIOExt(3,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE);

    //通道端口配置
//    Gpio_SetFunc_UART1TX_P35();
//    Gpio_SetFunc_UART1RX_P36();
    Gpio_SetFunc_UART1_RXD_P02()  ;
    Gpio_SetFunc_UART1_TXD_P01();

    //外设时钟使能
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//模式0/2可以不使能
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = Uart1_RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//测试项，更改此处来转换4种模式测试


    stcMulti.enMulti_mode = UartNormal;//测试项，更改此处来转换多主机模式，mode2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//双倍波特率功能
    stcBaud.u32Baud = 9600;//更新波特率位置
    stcBaud.u8Mode = UartMode3; //计算波特率需要模式参数
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//调用basetimer1设置函数产生波特率
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH1, &stcConfig);
    Uart_EnableIrq(UARTCH1,UartRxIrq);
    Uart_ClrStatus(UARTCH1,UartRxFull);
    Uart_EnableFunc(UARTCH1,UartRx);
// Uart_SetMMDOrCk(UARTCH1,Addr);
}


void Uart1_Output(uint8_t u8Data)
{
    M0P_UART1->SCON_f.REN = 0;
    M0P_UART1->SBUF = u8Data;

    while (TRUE != M0P_UART1->ISR_f.TI)
    {
        ;
    }
    M0P_UART1->ICR_f.TICLR = 0;
}


int fputc (int ch,FILE * f)
{
#if 0
    if (((uint8_t)ch) == '\n')
    {
        Uart1_Output('\r');
    }
    Uart1_Output(ch);
#else
    Uart_SendData(UARTCH1,(uint8_t )ch);
#endif

    return ch;
}

void uart1SendData(uint8 * data,uint16 length)
{
    for(uint16 i=0; i<length; i++ )
    {
        Uart_SendData(UARTCH1,data[i]);
    }

}
//
//int fgetc (FILE * f)
//{
//    uint8_t ch=0;
//  //  HAL_UART_Receive(&huart1,&ch,1,0xffff);
//    return ch;
//}




