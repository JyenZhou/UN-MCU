#include "../JHAL_Uart.h"
#if (  defined HAL_UART_MODULE_ENABLED ||defined HAL_USART_MODULE_ENABLED)
/**
 //ʹ�ÿ����ж� (JHAL_UART_ReceiveITMode_IT_DMA_IDLE)  ����Ҫ��������
//1.cubemx��->DMA Seetings->add =rx     //������Ҫ��ʱmode=  ��mode���� Circular �൱�ڵ��������жϣ�   ��Ȼ�������൱�ڿ��к�����ʱ���ж�  ��������ճ������˵�����ڶ������Ǵ����ֵô���
//2. nvic ȫ���жϿ���






*/




#define __UART_Number  3





static JHAL_UART*  __uartConfig[__UART_Number];

static u8 __uartIsInit[__UART_Number];






USART_TypeDef* __JHAL_dev2uartInstance(u8 dev)
{
    if(dev==0)
    {
        return USART1;
    } else if(dev==1)
    {

        return USART2;

    }

    else {

      JHAL_Fault_Handler("__JHAL_dev2uartInstance");
			return USART1;

    }
}


u8 __JHAL_uartInstance2dev (USART_TypeDef*  instance )
{

    if(instance==USART1)
    {

        return 0;

    }


    else  if(instance==USART2)
    {

        return 1;

    }

    else {

     JHAL_Fault_Handler("__JHAL_uartInstance2dev");
			return 0;

    }
}





bool JHAL_uartOpen( JHAL_UART *config)
{
    UART_HandleTypeDef *uart=   (UART_HandleTypeDef * ) (config->dev) ;


    u8 id=__JHAL_uartInstance2dev(uart->Instance);
    config->id=id;


    __uartConfig[id]=  config  ;


    if(!__uartIsInit[id])
    {   //Cumbex ���Զ��� ���Դ�ʱ���ȹ����ٿ�
        __uartIsInit[id]=   config->__info.isOpen =true;

        JHAL_uartClose(config);

    }
    if(!	config->__info.isOpen ) {





        uart->Init.BaudRate = config->baudRate;

        if (HAL_UART_Init(uart) != HAL_OK)
        {
            Error_Handler();
        }




        JHAL_uartEnableReceiveIT (config );


        config->__info.isOpen=true;

        return config->__info.isOpen=true;
    }

    return false;

}


void __JHAL_uartAbortReceiveIT (u8 id )
{

    UART_HandleTypeDef *uart= (UART_HandleTypeDef * ) (  __uartConfig[id]->dev);



    if( __uartConfig[id]->rxConfig.receiveMode== JHAL_UART_ReceiveITMode_IT_DMA_IDLE)
    {
        HAL_UART_DMAStop(uart);
    }
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

/**
 * JHAL_Uart0->UART1_PS_PTC7_PTC6
 *
 *
 *
 * */

//���ڽ���ʧ��
void JHAL_uartAbortReceiveIT (JHAL_UART *juart )
{




    __uartConfig[juart->id]->rxConfig.__info.itEN=false;

    __JHAL_uartAbortReceiveIT(juart->id);



}





void  __JHAL_uartEnableReceiveIT( u8 dev )
{


    UART_HandleTypeDef *uart= (UART_HandleTypeDef * ) (  __uartConfig[dev]->dev);




    if(  __uartConfig[dev]->rxConfig.receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {
        if(  __uartConfig[dev]->rxConfig.__info.crcOK ||__uartConfig[dev]->rxConfig .crcMode==JHAL_CRC_Mode_NONE ) {
            HAL_UART_Receive  (uart,__uartConfig[dev]->rxConfig.dataBuff,__uartConfig[dev]->rxConfig.length,100);
        } else {
            HAL_UART_Receive (uart,  &__uartConfig[dev]->rxConfig.__info.byteData, 1,100);
        }

    } else    if(__uartConfig[dev]->rxConfig.receiveMode== JHAL_UART_ReceiveITMode_IT_DMA_IDLE)
    {
        // __HAL_UART_ENABLE_IT(uart, UART_IT_IDLE);
        // HAL_UART_Receive_DMA(uart, __uartConfig[  dev]->rxConfig.dataBuff, __uartConfig[  dev]->rxConfig.length);

        HAL_UARTEx_ReceiveToIdle_DMA(uart, __uartConfig[dev]->rxConfig.dataBuff,__uartConfig[dev]->rxConfig.length); //�������ڿ����ж�DMA��������
    }



}


/*���ڽ���ʹ��*/
void  JHAL_uartEnableReceiveIT( JHAL_UART *uart  )
{


    __uartConfig[uart->id]->rxConfig.__info.start = false;
    __uartConfig[uart->id]->rxConfig.__info.dataCount=0;
    __uartConfig[uart->id]->rxConfig.__info.end=false;
    __uartConfig[uart->id]->rxConfig.__info.itEN=true;



    __JHAL_uartEnableReceiveIT(uart->id);
}



/**
  * ��������:���ڿ����жϴ���1


 */


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    u8 dev= __JHAL_uartInstance2dev (huart->  Instance );




    __uartConfig[dev]->rxConfig.__info.itEN=false;
    __uartConfig[dev]->rxConfig.__info.end=true;
    __uartConfig[dev]->rxConfig.__info.dataCount = Size;



}




/**
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 dev= __JHAL_uartInstance2dev (huart->  Instance );


    if(!JHAL_uartRxITCallback(  dev))
    {
        return;
    }
    if( __uartConfig[  dev]->rxConfig.crcMode==JHAL_CRC_Mode_NONE ||__uartConfig[  dev]->rxConfig.__info. end)
// ������ɿ��ܴ����쳣�����ж�
    {
        __uartConfig[  dev]->rxConfig.__info.dataCount= __uartConfig[  dev]->rxConfig.length;
        __uartConfig[  dev]->rxConfig.__info.end=true;
        __uartConfig[  dev]->rxConfig.__info.itEN=false;

        __JHAL_uartAbortReceiveIT( dev);

    } else



        if( __uartConfig[  dev]->rxConfig.receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
        {

            if(  __uartConfig[  dev]->rxConfig.__info.crcOK   ) {

                __uartConfig[  dev]->rxConfig.__info.dataCount=  __uartConfig[  dev]->rxConfig.length;


            } else {
                ////�����һ�δ��ڰ�ͷУ��ʧ�ܵ�״̬��ʹ�ܵ�ʱ���л����ǵ����ж�
                if (!  __uartConfig[  dev]->rxConfig.__info.start)
                {
                    if(  __uartConfig[  dev]->rxConfig.__info.byteData==  __uartConfig[  dev]->rxConfig.headCode) {
                        __uartConfig[  dev]->rxConfig.__info.start = true;
                        __uartConfig[  dev]->rxConfig.__info.dataCount=0;


                    }
                    __JHAL_uartEnableReceiveIT(dev);

                } else


                {


                    __uartConfig[  dev]->rxConfig.dataBuff[   __uartConfig[  dev]->rxConfig.__info.dataCount++] =   __uartConfig[  dev]->rxConfig.__info.byteData;

                }
            }

            if(   __uartConfig[  dev]->rxConfig.__info.dataCount==   __uartConfig[  dev]->rxConfig.length)
            {
                if(JHAL_crcCheak(  __uartConfig[  dev]->rxConfig.crcMode,  __uartConfig[  dev]->rxConfig.dataBuff,  __uartConfig[  dev]->rxConfig.length))
                {
                    __uartConfig[  dev]->rxConfig.__info.end=true;
                    __uartConfig[  dev]->rxConfig.__info.itEN=false;

                    __JHAL_uartAbortReceiveIT(  dev);

                }
            } else {
                __uartConfig[  dev]->rxConfig.__info.start = false;
                __uartConfig[  dev]->rxConfig.__info.dataCount=0;
                __JHAL_uartEnableReceiveIT(dev);

            }
        }



}



bool  JHAL_uartClose(JHAL_UART *juart)
{

    if(  juart->__info.isOpen) {

        JHAL_uartAbortReceiveIT(juart);

        HAL_UART_DeInit( (UART_HandleTypeDef * ) (  __uartConfig[juart->id]->dev));


        juart->__info.isOpen =false;
        return true;
    }
    return false;
}




__weak bool JHAL_uartRxITCallback(u8 uartID)
{

    return true;
}

bool JHAL_uartSendDatas(JHAL_UART *juart,u8* data,u16  length,JHAL_CRC_Mode mode)
{


    bool receiveITEN=     __uartConfig[  juart->id]->rxConfig.__info.itEN ;

    if(receiveITEN)
    {
        JHAL_uartAbortReceiveIT (juart);
    }


    JHAL_crcAutoWirte( mode,data, length);

    UART_HandleTypeDef *uart= (UART_HandleTypeDef * ) (  __uartConfig[juart->id]->dev);

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
        JHAL_uartEnableReceiveIT(juart);
    }
    return true;

}



u16 JHAL_uartRxFinsh(JHAL_UART *juart) {

    if(    __uartConfig[  juart->id]->rxConfig.__info.end) {
        return __uartConfig[  juart->id]->rxConfig.__info.dataCount;
    }
    return 0;
}



#ifdef JHAL_Uart_Printf






/******�ض���pr**BEGIN************/
int fputc (int ch,FILE * f)
{

    JHAL_uartSendDatas(JHAL_Uart_Printf,(uint8_t *)&ch,1,JHAL_CRC_Mode_NONE);
    return ch;
}

//���յ�һ���ò��� ������
int fgetc (FILE * f)
{
    uint8_t ch=0;
    HAL_UART_Receive(__JHAL_juart2uart(JHAL_Uart_Printf),&ch,1,0xffff);
    return ch;
}

/**********�ض���pr*****END********/

#endif



#endif
