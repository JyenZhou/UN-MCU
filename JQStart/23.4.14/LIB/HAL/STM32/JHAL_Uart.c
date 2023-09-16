#include "../JHAL_Uart.h"
/**
 //使用空闲中断 (JHAL_UART_ReceiveITMode_IT_DMA_IDLE)  还需要这样操作   
//1.cubemx中->DMA Seetings->add =rx-> mode= Circular
//2. nvic 全局中断开启 

   
  3. 在 USART1_IRQHandler(void)  中断处理函数中添加如下函数
	#include "JHAL.h"
void JHAL_uartRxIdleHandler(JHAL_UART juart);
     JHAL_uartRxIdleHandler(&huartx); 
  
 

*/

 
static JHAL_UART_RXConfig  *__rXConfig[JHAL_UART_Number];


extern UART_HandleTypeDef  huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

 
JHAL_UART __JHAL_uart2juart(UART_HandleTypeDef* uart)
{
  
    if(uart==&huart2)
    {
        return JHAL_UART2;

    } else {

//不存在或未实现
        while(true);
	
    }
 

}
UART_HandleTypeDef* __JHAL_juart2uart(JHAL_UART juart)
{
  
    if(juart==JHAL_UART2)
    {
        return &huart2;

    } 
		 
else {

//不存在或未实现
        while(true);
 
    }
}
 

USART_TypeDef* __JHAL_juart2uartInstance(JHAL_UART juart)
{
  
    if(juart==JHAL_UART2)
    {
        return USART2;

    } 
		 
else {

//不存在或未实现
        while(true);
	 
    }
}
 
 DMA_HandleTypeDef* __JHAL_juart2hdma_uart_rx(JHAL_UART juart)
{
 
   if(juart==JHAL_UART2)
    {
        return &hdma_usart2_rx;

    } 
	 
	else {

//不存在或未实现
        while(true);
	 
    }

}
 
 
 

bool JHAL_uartInit(JHAL_UART juart,JHAL_UARTConfig *config)
{
	JHAL_uartDeInit(juart);
	
	UART_HandleTypeDef *uart= __JHAL_juart2uart(juart);
	
	 huart2.Instance = __JHAL_juart2uartInstance(juart);
  huart2.Init.BaudRate = config->baudRate;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
   __rXConfig[juart]=&config->rxConfig;
		
		 JHAL_uartEnableReceiveIT (juart);
 

    return true;
}




/**
 * JHAL_Uart0->UART1_PS_PTC7_PTC6
 *
 *
 *
 * */

//串口接收失能
void JHAL_uartAbortReceiveIT (JHAL_UART   juart)
{
	 __rXConfig[juart]->__rxInfo.itEN=false;
 UART_HandleTypeDef *uart=  __JHAL_juart2uart(juart);
	
    uint16_t i=0;
    while( HAL_UART_Abort_IT(uart)!= HAL_OK )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(uart);
        }
    }

    
        __HAL_UART_DISABLE_IT(uart,UART_IT_IDLE);
     
		
}





  void  __JHAL_uartEnableReceiveIT( JHAL_UART uartID  )
{
	
  UART_HandleTypeDef *uart=__JHAL_juart2uart(uartID);



    if( __rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {
if( __rXConfig[uartID]->__rxInfo.crcOK ||__rXConfig[uartID]->crcMode==JHAL_CRC_Mode_NONE ){
        HAL_UART_Receive  (&huart2,__rXConfig[uartID]->dataBuff,__rXConfig[uartID]->length,100);
}else{
	   HAL_UART_Receive (__JHAL_juart2uart(uartID),  &__rXConfig[uartID]->__rxInfo.byteData, 1,100);
}

    }else    if(__rXConfig[uartID]->receiveMode== JHAL_UART_ReceiveITMode_IT_DMA_IDLE)
		{
			__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
      HAL_UART_Receive_DMA(&huart2,__rXConfig[uartID]->dataBuff,__rXConfig[uartID]->length);
		} 



}


/*串口接收使能*/
  void  JHAL_uartEnableReceiveIT( JHAL_UART uartID  )
{
	
 
    __rXConfig[uartID]->__rxInfo.start = false;
            __rXConfig[uartID]->__rxInfo.dataCount=0;
		  __rXConfig[uartID]->__rxInfo.end=false;	
     __rXConfig[uartID]->__rxInfo.itEN=true;
	
	
	
	__JHAL_uartEnableReceiveIT(uartID);
}


 
/**
  * 函数功能:串口空闲中断处理1
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：

  添加空闲中断处理函数： 在 USART1_IRQHandler(void) (中断处理函数)
     JHAL_RxIdleCallback(&huartx); 

 void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
    JHAL_RxIdleCallback(&huart1);
}


 */
void JHAL_uartRxIdleHandler(JHAL_UART juart)
{
	 
	UART_HandleTypeDef *uart=__JHAL_juart2uart(juart);
	
		DMA_HandleTypeDef *hdmaUart=__JHAL_juart2hdma_uart_rx(juart);
	
    if(RESET!=__HAL_UART_GET_FLAG(uart,UART_FLAG_IDLE))
    {
        //判断是否是空闲中断  因为这个是放在串口中断中的 串口中断时也会进来
        return;
    }
    //清除空闲中断标志（否则会一直不断进入中断）
    __HAL_UART_CLEAR_IDLEFLAG(uart);

  
		
		 
    uint32_t temp;
     
    
        temp = uart->Instance->SR;  
        temp = uart->Instance->DR; 
        HAL_UART_DMAStop(uart); 
        temp  = hdmaUart->Instance->CNDTR;
		
          __rXConfig[juart]->__rxInfo.dataCount =  temp; 
		
		  
            if(JHAL_crcCheak( __rXConfig[juart]->crcMode, __rXConfig[juart]->dataBuff, __rXConfig[juart]->length))
            {
                __rXConfig[juart]->__rxInfo.end=true;

            } 
        
		
        __rXConfig[juart]->__rxInfo.end=true;	
   
       

 
}
 
/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

JHAL_UART uartID=    __JHAL_uart2juart( huart);

	if(!JHAL_uartRxITCallback(uartID))
	{
		return;
	}
    if( __rXConfig[uartID]->crcMode==JHAL_CRC_Mode_NONE ||__rXConfig[uartID]->__rxInfo. end)
// 接收完成可能存在异常继续中断
    {
     			 __rXConfig[uartID]->__rxInfo.dataCount= __rXConfig[uartID]->length;
				__rXConfig[uartID]->__rxInfo.end=true;
        
    }else
		 
	 
   
    if(__rXConfig[uartID]->receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {
 
					if( __rXConfig[uartID]->__rxInfo.crcOK   ){
				
						 __rXConfig[uartID]->__rxInfo.dataCount= __rXConfig[uartID]->length;
						
						
					}else{
						////如果上一次处于包头校验失败的状态在使能的时候切换的是单个中断
							 if (! __rXConfig[uartID]->__rxInfo.start)  
        {
						if( __rXConfig[uartID]->__rxInfo.byteData== __rXConfig[uartID]->headCode){
            __rXConfig[uartID]->__rxInfo.start = true;
            __rXConfig[uartID]->__rxInfo.dataCount=0;
					
							
						} 
									__JHAL_uartEnableReceiveIT(uartID);
						 
        }else
				
		 
					{
				
				 
            __rXConfig[uartID]->dataBuff[  __rXConfig[uartID]->__rxInfo.dataCount++] =  __rXConfig[uartID]->__rxInfo.byteData;
 
					} 
			}
				
        if(  __rXConfig[uartID]->__rxInfo.dataCount==  __rXConfig[uartID]->length)
        {
            if(JHAL_crcCheak( __rXConfig[uartID]->crcMode, __rXConfig[uartID]->dataBuff, __rXConfig[uartID]->length))
            {
                __rXConfig[uartID]->__rxInfo.end=true;

            }
        } else {
					  __rXConfig[uartID]->__rxInfo.start = false;
            __rXConfig[uartID]->__rxInfo.dataCount=0;
					 	__JHAL_uartEnableReceiveIT(uartID);
					
        }
    }

	
	 
}



bool  JHAL_uartDeInit(JHAL_UART juart)
{

    JHAL_uartAbortReceiveIT(juart);
     
HAL_UART_DeInit(__JHAL_juart2uart(juart));
    return true;
}





__weak bool JHAL_uartRxITCallback(u8 uartID)
{

    return true;
}
 
 bool JHAL_uartSendDatas(JHAL_UART juart,u8* data,u16  length,JHAL_CRC_Mode mode) 
{

    bool receiveITEN=    __rXConfig[juart]->__rxInfo.itEN=true;

    if(receiveITEN)
    {
        JHAL_uartAbortReceiveIT (juart);
    }
  

   JHAL_crcAutoWirte( mode,data, length);

			UART_HandleTypeDef *uart= __JHAL_juart2uart(juart);
	
    uint16_t i=0;
    while(uart->gState != HAL_UART_STATE_READY )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(uart);
        }
    }
    i=0;
    while(  HAL_UART_Transmit(uart,data,length,0xffff)!=HAL_OK )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(uart);
        }
    }
    if(receiveITEN)
    {
        __JHAL_uartEnableReceiveIT(juart);  
    }
		return true;

}



 u16 JHAL_uartRxFinsh(JHAL_UART uart){
	 if(   __rXConfig[uart]->__rxInfo.start){
		  return  __rXConfig[uart]->__rxInfo.dataCount;
	 }
	 return 0;
 }


 
#ifdef JHAL_Uart_Printf


 


 
/******重定向pr**BEGIN************/
int fputc (int ch,FILE * f)
{

    JHAL_uartSendDatas(JHAL_Uart_Printf,(uint8_t *)&ch,1,JHAL_CRC_Mode_NONE);
    return ch;
}

//接收的一般用不上 放在这
int fgetc (FILE * f)
{
    uint8_t ch=0;
    HAL_UART_Receive(__JHAL_juart2uart(JHAL_Uart_Printf),&ch,1,0xffff);
    return ch;
}

/**********重定向pr*****END********/
 
#endif



