/**
  ****************************JHAL_Uart.c******************************************
  * @brief      JHAL_UART0->UART1  PC4 --> UART1-RxD PC3 --> UART1-TxD
  *
    ******************************************************************************
  * @file     JHAL_Uart.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-12-28 
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
  #include "../JHAL_Uart.h"
#include "asm32lx003_uart.h"
//�����Ƭ�����ڸ���
#define UART_NUM  2


//���ص���id ��UARTX��Ӧ
u8 __JHAL_juart2Id(JHAL_UART uart)
{
    if(uart==JHAL_UART0)
    {
        return 1;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

UART_TypeDef * __JHAL_juart2uart(JHAL_UART uart)
{
    if(uart==JHAL_UART0)
    {
        return UART1;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

UART_TypeDef* __JHAL_id2uart(u8 id)
{
    if(id==0)
    {
        return  UART0;
    } else {

//�����ڻ�δʵ��
        while(true);
    }

}
 




bool __JHAL_uartIsENCheak(JHAL_UART juart, bool isEN) {
	u8  id=__JHAL_juart2Id(juart);
	
    static bool sEN[UART_NUM]= {false};
    if(sEN[id]!=isEN)
    {
        sEN[id]=isEN;
        return true;
    }
    return  false;
}
static JHAL_UART_RXConfig  *__rXConfig[UART_NUM];

/*���ڽ�����ʹ��*/

 void __JHAL_uartReceiveIT( u8 uartID, UART_TypeDef *uart)
{


    if  ( __rXConfig[uartID]->__rxInfo.end||! __rXConfig[uartID]->__rxInfo.itEN)
    {
        return;
    }
    if( __rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

 
	UART_ITConfig(uart, UART_RIEN_EABLE, ENABLE);
				/* ʹ�ܴ��� */
	UART_Cmd(uart, UART_RXEN_EABLE, ENABLE);	  

    }



} 

//ʹ�ܽ���
void JHAL_uartEnableReceiveIT (JHAL_UART uart)
{   u8 uartID=__JHAL_juart2Id(uart);
    __rXConfig[uartID]->__rxInfo.itEN=true;
    __JHAL_uartReceiveIT(uartID,__JHAL_juart2uart(uart));

}

//�����ж�ʧ��
void __JHAL_uartAbortReceiveIT (u8 id)
{
    __rXConfig[id]->__rxInfo.itEN=false;
    UART_TypeDef* uart=__JHAL_id2uart(id);
	
	UART_ITConfig(uart, UART_RIEN_EABLE, DISABLE);
	UART_ITConfig(uart, UART_TIEN_EABLE, DISABLE);
	UART_Cmd(uart, UART_RXEN_EABLE, DISABLE);	 
}

//�����ж�ʧ��
void JHAL_uartAbortReceiveIT (JHAL_UART uart)
{
    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(uart));
}





 bool  JHAL_uartDeInit(JHAL_UART juart)
{
 	if(!__JHAL_uartIsENCheak(juart,false))
	{
		return false;
	}
    __JHAL_uartAbortReceiveIT(__JHAL_juart2Id(juart));
    UART_TypeDef *uart=__JHAL_juart2uart(juart);
		UART_Cmd(uart, UART_RXEN_EABLE, DISABLE);	
    UART_DeInit(uart);
 return true;
}


bool JHAL_uartInit(JHAL_UART juart,JHAL_UARTConfig *config)
{
	if(!__JHAL_uartIsENCheak(juart,true))
	{
		return false;
	}
	
  UART_TypeDef*  uartx=__JHAL_juart2uart(juart);
    
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	UART_InitTypeDef UART_InitStructure;
	
    if(juart == JHAL_UART0)
    {
				/* ��USART Rx��GPIO����Ϊ����ģʽ */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOC, &GPIO_InitStruct);   // 

	/* ��USART Tx��GPIO����Ϊ�������ģʽ */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOC, &GPIO_InitStruct);   //
	
	/* uart GPIO�ܽŹ���ѡ������ */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_UART1_RXD_PC4); // PC4 --> UART1-RxD
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_UART1_TXD_PC3); // PC3 --> UART1-TxD

			
    } 
    else {
//�����ڻ�δʵ��
        while(true);

    }

  UART_InitStructure.UART_BaudRate = config->baudRate; // ���ò�����ʱ��Ҫ����PCLKʱ���޸�asm32lx003.h�����63�����ң��е�Fpclk�궨��
	UART_InitStructure.UART_BaudRateDbaud_Enable = ENABLE;
	UART_InitStructure.UART_BaudRateTimer_Selected = DISABLE;
	UART_InitStructure.UART_Mode = UART_MODE1;
	UART_Init(uartx, TIM11, &UART_InitStructure);
   
		
		
			/* ����NVIC�ж����� */
      if(uartx == UART1)
    {
     	/* ����NVIC�ж����� */
    NVIC_SetPriority(UART1_IRQn, 0);
    NVIC_EnableIRQ(UART1_IRQn);
    }

    __rXConfig[__JHAL_juart2Id(juart)]=&(config->rxConfig);


   
	return true;

}












/*-----------------------------------------------------------------------*/

/*  ���ڵĽ��մ���   */

/*-----------------------------------------------------------------------*/
void  __JHAL_receiveHandler(u8 uartID, UART_TypeDef *uart)
{


    if(__rXConfig[uartID]->__rxInfo. end|| __rXConfig[uartID]->__rxInfo.dataCount>=__rXConfig[uartID]->length)
//������Ҫ����������Ҳ����������ɴ���
    {
        __rXConfig[uartID]->__rxInfo.end=true;
        return;
    }
    __rXConfig[uartID]->__rxInfo.byteData = UART_ReceiveData(uart);
    if(__rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {

//�����һ�δ��ڰ�ͷУ��ʧ�ܵ�״̬��ʹ�ܵ�ʱ���л����ǵ����ж�

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




/*����*/
void __JHAL_uartReadBuffClean(UART_TypeDef *uart)
{

        UART_ReceiveData(uart);
    

}



 
__weak bool JHAL_uartRxITCallback(u8 uartID)
{

    return true;
}
void __JHAL_uartITCallback4Lin(u8 id);
//linģʽ�ж�
static bool isAnalogLinMode[UART_NUM]= {false};
/**
	* @brief  �жϺ���
	*/
void UART1_IRQHandler(void)
{
	
    if(isAnalogLinMode[1]) {
        __JHAL_uartITCallback4Lin(1);
    } else {
			 if ((UART0->ISR & UART_ISR_RI) == UART_ISR_RI){
        //���ض�����Ƿ����ִ�д��ڴ���
        if(JHAL_uartRxITCallback(1)) {
             __JHAL_receiveHandler(1,UART1); 
        }
				         UART0->ICR |= UART_ICR_RI;
			}
    }

}



#ifdef JHAL_Uart_Printf


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
        while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_Uart_Printf),UART_LSRTHREmpty));
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
    while(!UART_GetLineStatus(__JHAL_juart2uart(JHAL_Uart_Printf),UART_LSRTHREmpty));
    UART_SendData(__JHAL_juart2uart(JHAL_Uart_Printf), (uint8_t) ch);
    return ch;
}
/**
  * @brief  �ض���printf()������
  */
int fgetc(FILE *f)
{
    return (int)UART_ReceiveData(__JHAL_juart2uart(JHAL_Uart_Printf));
}

#else
#error Not supported compiler type
#endif

#endif


