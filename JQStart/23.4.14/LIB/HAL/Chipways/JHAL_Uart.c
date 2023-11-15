#include "../JHAL_Uart.h"
#include "xl_uart.h"
 #define __JHAL_UART_Exist
//�����Ƭ��ֻ����������
#define __UART_Number  3


//���ص���id ��UARTX��Ӧ
u8 __JHAL_juart2Id(u8 dev )
{
    if( dev==0||1== dev)
    {
        return 1;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

UART_Type * __JHAL_juart2uart(u8 dev)
{
    if( dev==0||1== dev)
    {
        return UART1;

    } else {

//�����ڻ�δʵ��
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
/*����*/
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
  * @brief  �ж�����
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





static JHAL_UART_RXConfig  *__rXConfig[__UART_Number];

/**
 * JHAL_Uart0->UART1_PS_PTC7_PTC6
 *
 *
 *
 * */


bool JHAL_uartOpen( JHAL_UART  *config)
{
		if(!	config->__info.isOpen ){
    UART_Type *  __uartX=__JHAL_juart2uart(config->dev);
    UART_InitTypeDef UART_InitStructure;
    if(config->dev == 0)
    {
        SIM_SCGC_Cmd(SIM_SCGC_UART1,ENABLE);
        SIM_PINSEL_UART1(UART1_PS_PTC7_PTC6);
    } else   if(config->dev == 1)
    {
        SIM_SCGC_Cmd(SIM_SCGC_UART1,ENABLE);
        SIM_PINSEL_UART1(UART1_PS_PTF3_PTF2);
    }
    else {
//�����ڻ�δʵ��
        while(true);

    }



    UART_InitStructure.UART_SourceClk= Get_PeripheralClock();
    UART_InitStructure.UART_BaudRate = config->baudRate;
    UART_InitStructure.UART_DataLength = UART_DataLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;

    UART_Init(__uartX, &UART_InitStructure);
    __Uart_Interrupt_Init(__uartX);

    __rXConfig[__JHAL_juart2Id(config->dev)]=&(config->rxConfig);


    UART_EnableCmd(__uartX, ENABLE);

		
		
    return config->__info.isOpen=true;
	}
	
		return false;
		
}






/*���ڽ�����ʹ��*/

static void __JHAL_uartReceiveIT( u8 uartID, UART_Type *uart)
{


    if  ( __rXConfig[uartID]->__info.end||! __rXConfig[uartID]->__info.itEN)
    {
        return;
    }
    if( __rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

        __JHAL_uartReadBuffClean(uart);
        UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN, ENABLE);

    }



}

//ʹ�ܽ���
void JHAL_uartEnableReceiveIT (JHAL_UART *uart)
{   u8 uartID=__JHAL_juart2Id(uart->dev);
    __rXConfig[uartID]->__info.itEN=true;
    __JHAL_uartReceiveIT(uartID,__JHAL_juart2uart(uart->dev));

}
//�����ж�ʧ��
void __JHAL_uartAbortReceiveIT (u8 id)
{
    __rXConfig[id]->__info.itEN=false;
    UART_Type* uart=__JHAL_id2uart(id);
    UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN, DISABLE);
    UART_EnableCmd(uart, DISABLE);
}

//�����ж�ʧ��
void JHAL_uartAbortReceiveIT (JHAL_UART *uart)
{
    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(uart->dev));
}





/*-----------------------------------------------------------------------*/

/*  ���ڵĽ��մ���   */

/*-----------------------------------------------------------------------*/
void  __JHAL_receiveHandler(u8 uartID, UART_Type *uart)
{


    if(__rXConfig[uartID]->__info. end|| __rXConfig[uartID]->__info.dataCount>=__rXConfig[uartID]->length)
//������Ҫ����������Ҳ����������ɴ���
    {
        __rXConfig[uartID]->__info.end=true;
        return;
    }
    __rXConfig[uartID]->__info.byteData = UART_ReceiveData(uart);
    if(__rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

//�����һ�δ��ڰ�ͷУ��ʧ�ܵ�״̬��ʹ�ܵ�ʱ���л����ǵ����ж�

        if((! __rXConfig[uartID]->__info.start)&&( __rXConfig[uartID]->__info.byteData== __rXConfig[uartID]->headCode))
        {
            __rXConfig[uartID]->__info.start = true;
            __rXConfig[uartID]->__info.dataCount=0;
        }
        if(  __rXConfig[uartID]->__info.start)
        {
            __rXConfig[uartID]->dataBuff[  __rXConfig[uartID]->__info.dataCount++] =  __rXConfig[uartID]->__info.byteData;

        }
        if(  __rXConfig[uartID]->__info.dataCount==  __rXConfig[uartID]->length)
        {
            if(JHAL_crcCheak( __rXConfig[uartID]->crcMode, __rXConfig[uartID]->dataBuff, __rXConfig[uartID]->length))
            {
                __rXConfig[uartID]->__info.end=true;

            }
        } else {
            __JHAL_uartReceiveIT(uartID,uart);
        }
    }
		
	 
}



bool  JHAL_uartClose(JHAL_UART *juart)
{
if(  juart->__info.isOpen){
	
    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(juart->dev));
    UART_Type *uart=__JHAL_juart2uart(juart->dev);
    UART_DeInit(uart);
    UART_EnableCmd(uart, DISABLE);
 juart->__info.isOpen =false;
    return true;
}
return false;
}





__weak bool JHAL_uartRxITCallback(u8 uartID)
{

    return true;
}
void __JHAL_uartITCallback4Lin(u8 id);
//linģʽ�ж�
static bool isAnalogLinMode[__UART_Number]= {false};
/**
	* @brief  �жϺ���
	*/
void UART1_IRQHandler(void)
{
    u8 intFlags = UART_GetInterruptStatus(UART1);
    if(isAnalogLinMode[1]) {
        __JHAL_uartITCallback4Lin(1);
    } else {
        if((intFlags & 0x0F) == UART_IID_RxDataAvailable) {
            //���ض�����Ƿ����ִ��Ĭ�ϴ��ڽ��մ���
            if(JHAL_uartRxITCallback(1)) {

                __JHAL_receiveHandler(1,UART1);
            }


        }
    }

}


  u16 JHAL_uartRxFinsh(JHAL_UART *uart){
	
		
		return 0;
		
	}

	
	  bool JHAL_uartSendDatas(JHAL_UART *uart,u8* data,u16  length,JHAL_CRC_Mode mode){
			
			
			return false;
		}

#ifdef JHAL_UartPrintfDev


#if defined(__GNUC__)
#ifndef TRACE
/**
  * @brief  �ض���printf()������
  */
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    if(fd > 2)
        return -1;
    while(i < len)
    {
        while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_UartPrintfDev),UART_LSRTHREmpty));
        UART_SendData(Debug_Port, (uint8_t) *ptr++);
        i++;
    }
    return i;
}
#endif
#elif defined(__ARMCC_VERSION) || defined(__CWCC__) || defined(__IAR_SYSTEMS_ICC__)
/**
  * @brief  �ض���printf()�����
  */
int fputc(int ch, FILE *f)
{
    while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_UartPrintfDev),UART_LSRTHREmpty));
    UART_SendData(__JHAL_juart2uart(JHAL_UartPrintfDev), (uint8_t) ch);
    return ch;
}
/**
  * @brief  �ض���printf()������
  */
int fgetc(FILE *f)
{
    return (int)UART_ReceiveData(__JHAL_juart2uart(JHAL_UartPrintfDev));
}

#else
#error Not supported compiler type
#endif

#endif


