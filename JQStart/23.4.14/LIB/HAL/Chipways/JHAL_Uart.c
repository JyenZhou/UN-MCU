#include "../JHAL_Uart.h"
#include "xl_uart.h"

//这个单片机只有三个串口
#define UART_NUM  3


//返回的是id 与UARTX对应
u8 __JHAL_juart2Id(JHAL_UART uart)
{
    if(uart==JHAL_UART0||JHAL_UART1==uart)
    {
        return 1;

    } else {

//不存在或未实现
        while(true);
    }

}

UART_Type * __JHAL_juart2uart(JHAL_UART uart)
{
    if(uart==JHAL_UART0||JHAL_UART1==uart)
    {
        return UART1;

    } else {

//不存在或未实现
        while(true);
    }

}

UART_Type* __JHAL_id2uart(u8 id)
{
    if(id==0)
    {
        return  UART0;
    } else if(id==1)
    {
        return UART1;
    } else
    {
        return UART2;
    }

}
/*读清*/
void __JHAL_uartReadBuffClean(UART_Type *uart)
{
    UART_GetLineStatus(uart,UART_LSRBreakIntr);
    UART_GetLineStatus(uart,UART_LSRDataReady);
    while(UART_GetStatus(uart,UART_Busy))
    {
        UART_ReceiveData(uart);
    }

    while(UART_GetStatus(uart,UART_ReceiveFIFONotEmpty))
    {
        UART_ReceiveData(uart);
    }

}


/**
  * @brief  中断配置
  * @retval None
  */
static void __Uart_Interrupt_Init(UART_Type * uartx)
{

    UART_EnableInterruptCmd(uartx, UART_RDataAvailableIntEN, DISABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    if(uartx == UART0)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART0_IRQn;
    }
    else if(uartx == UART1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    }
    else if(uartx == UART2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}





static JHAL_UART_RXConfig  *__rXConfig[UART_NUM];

/**
 * JHAL_Uart0->UART1_PS_PTC7_PTC6
 *
 *
 *
 * */


bool JHAL_uartInit(JHAL_UART juart,JHAL_UARTConfig *config)
{
    UART_Type *  __uartX=__JHAL_juart2uart(juart);
    UART_InitTypeDef UART_InitStructure;
    if(juart == JHAL_UART0)
    {
        SIM_SCGC_Cmd(SIM_SCGC_UART1,ENABLE);
        SIM_PINSEL_UART1(UART1_PS_PTC7_PTC6);
    } else   if(juart == JHAL_UART1)
    {
        SIM_SCGC_Cmd(SIM_SCGC_UART1,ENABLE);
        SIM_PINSEL_UART1(UART1_PS_PTF3_PTF2);
    }
    else {
//不存在或未实现
        while(true);

    }



    UART_InitStructure.UART_SourceClk= Get_PeripheralClock();
    UART_InitStructure.UART_BaudRate = config->baudRate;
    UART_InitStructure.UART_DataLength = UART_DataLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;

    UART_Init(__uartX, &UART_InitStructure);
    __Uart_Interrupt_Init(__uartX);

    __rXConfig[__JHAL_juart2Id(juart)]=&(config->rxConfig);


    UART_EnableCmd(__uartX, ENABLE);

    return true;
}






/*串口接收再使能*/

static void __JHAL_uartReceiveIT( u8 uartID, UART_Type *uart)
{


    if  ( __rXConfig[uartID]->__rxInfo.end||! __rXConfig[uartID]->__rxInfo.itEN)
    {
        return;
    }
    if( __rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

        __JHAL_uartReadBuffClean(uart);
        UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN, ENABLE);

    }



}

//使能接收
void JHAL_uartEnableReceiveIT (JHAL_UART uart)
{   u8 uartID=__JHAL_juart2Id(uart);
    __rXConfig[uartID]->__rxInfo.itEN=true;
    __JHAL_uartReceiveIT(uartID,__JHAL_juart2uart(uart));

}
//接收中断失能
void __JHAL_uartAbortReceiveIT (u8 id)
{
    __rXConfig[id]->__rxInfo.itEN=false;
    UART_Type* uart=__JHAL_id2uart(id);
    UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN, DISABLE);
    UART_EnableCmd(uart, DISABLE);
}

//接收中断失能
void JHAL_uartAbortReceiveIT (JHAL_UART uart)
{
    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(uart));
}





/*-----------------------------------------------------------------------*/

/*  串口的接收处理   */

/*-----------------------------------------------------------------------*/
void  __JHAL_receiveHandler(u8 uartID, UART_Type *uart)
{


    if(__rXConfig[uartID]->__rxInfo. end|| __rXConfig[uartID]->__rxInfo.dataCount>=__rXConfig[uartID]->length)
//当接收要超过缓冲了也当做接收完成处理
    {
        __rXConfig[uartID]->__rxInfo.end=true;
        return;
    }
    __rXConfig[uartID]->__rxInfo.byteData = UART_ReceiveData(uart);
    if(__rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

//如果上一次处于包头校验失败的状态在使能的时候切换的是单个中断

        if((! __rXConfig[uartID]->__rxInfo.start)&&( __rXConfig[uartID]->__rxInfo.byteData== __rXConfig[uartID]->headCode))
        {
            __rXConfig[uartID]->__rxInfo.start = true;
            __rXConfig[uartID]->__rxInfo.dataCount=0;
        }
        if(  __rXConfig[uartID]->__rxInfo.start)
        {
            __rXConfig[uartID]->dataBuff[  __rXConfig[uartID]->__rxInfo.dataCount++] =  __rXConfig[uartID]->__rxInfo.byteData;

        }
        if(  __rXConfig[uartID]->__rxInfo.dataCount==  __rXConfig[uartID]->length)
        {
            if(JHAL_crcCheak( __rXConfig[uartID]->crcMode, __rXConfig[uartID]->dataBuff, __rXConfig[uartID]->length))
            {
                __rXConfig[uartID]->__rxInfo.end=true;

            }
        } else {
            __JHAL_uartReceiveIT(uartID,uart);
        }
    }
}



bool  JHAL_uartDeInit(JHAL_UART juart)
{

    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(juart));
    UART_Type *uart=__JHAL_juart2uart(juart);
    UART_DeInit(uart);
    UART_EnableCmd(uart, DISABLE);

    return true;
}





__weak bool JHAL_uartRxITCallback(u8 uartID)
{

    return true;
}
void __JHAL_uartITCallback4Lin(u8 id);
//lin模式判断
static bool isAnalogLinMode[UART_NUM]= {false};
/**
	* @brief  中断函数
	*/
void UART1_IRQHandler(void)
{
    u8 intFlags = UART_GetInterruptStatus(UART1);
    if(isAnalogLinMode[1]) {
        __JHAL_uartITCallback4Lin(1);
    } else {
        if((intFlags & 0x0F) == UART_IID_RxDataAvailable) {
            //被重定义后是否继续执行默认串口接收处理
            if(JHAL_uartRxITCallback(1)) {

                __JHAL_receiveHandler(1,UART1);
            }


        }
    }

}


  u16 JHAL_uartRxFinsh(JHAL_UART uart){
	
		
		return 0;
		
	}

	
	  bool JHAL_uartSendDatas(JHAL_UART uart,u8* data,u16  length,JHAL_CRC_Mode mode){
			
			
			return false;
		}

#ifdef JHAL_Uart_Printf


#if defined(__GNUC__)
#ifndef TRACE
/**
  * @brief  重定向printf()的输入
  */
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    if(fd > 2)
        return -1;
    while(i < len)
    {
        while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_Uart_Printf),UART_LSRTHREmpty));
        UART_SendData(Debug_Port, (uint8_t) *ptr++);
        i++;
    }
    return i;
}
#endif
#elif defined(__ARMCC_VERSION) || defined(__CWCC__) || defined(__IAR_SYSTEMS_ICC__)
/**
  * @brief  重定向printf()的输出
  */
int fputc(int ch, FILE *f)
{
    while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_Uart_Printf),UART_LSRTHREmpty));
    UART_SendData(__JHAL_juart2uart(JHAL_Uart_Printf), (uint8_t) ch);
    return ch;
}
/**
  * @brief  重定向printf()的输入
  */
int fgetc(FILE *f)
{
    return (int)UART_ReceiveData(__JHAL_juart2uart(JHAL_Uart_Printf));
}

#else
#error Not supported compiler type
#endif

#endif


