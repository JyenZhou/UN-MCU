#include "../JHAL_Uart.h"
#if (  defined HAL_UART_MODULE_ENABLED ||defined HAL_USART_MODULE_ENABLED)



/**




 //使用空闲中断 (JHAL_UART_ReceiveITMode_IT_DMA_IDLE)  还需要这样操作
//1.cubemx中->DMA Seetings->add =rx     // 宽度等于byte mode=normal 如果用Circular模式还需要手动停止还会会导致粘包
//2. nvic 全局中断开启



波特率配置无效
一般配置方法
extern UART_HandleTypeDef huart1;


*/




//将定义的加入记数

#ifdef USART1
#define __USART1_DEFINED 1
#else
#define __USART1_DEFINED 0
#endif

#ifdef USART2
#define __USART2_DEFINED 1
#else
#define __USART2_DEFINED 0
#endif

#ifdef USART3
#define __USART3_DEFINED 1
#else
#define __USART3_DEFINED 0
#endif

#ifdef USART4
#define __USART4_DEFINED 1
#else
#define __USART4_DEFINED 0
#endif

#ifdef USART5
#define __USART5_DEFINED 1
#else
#define __USART5_DEFINED 0
#endif

#ifdef USART6
#define __USART6_DEFINED 1
#else
#define __USART6_DEFINED 0
#endif

#ifdef USART7
#define __USART7_DEFINED 1
#else
#define __USART7_DEFINED 0
#endif

#ifdef USART8
#define __USART8_DEFINED 1
#else
#define __USART8_DEFINED 0
#endif


#define __USART_COUNT COUNT_MACROS_CONST_VALUE(  __USART1_DEFINED, __USART2_DEFINED, __USART3_DEFINED, __USART4_DEFINED,  __USART5_DEFINED, __USART6_DEFINED, __USART7_DEFINED, __USART8_DEFINED)


 
#define __JHAL_Uart_Exist

static JHAL_UART*  __uartConfig[__USART_COUNT];

static u8 __uartIsInit[__USART_COUNT];






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

        JHAL_error("__JHAL_dev2uartInstance");
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

        JHAL_error("__JHAL_uartInstance2dev");
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
    {   //Cumbex 会自动打开 所以打开时候先关上再开就能兼容了
        __uartIsInit[id]=   config->__info.isOpen =true;

        JHAL_uartClose(config);

    }
    if(!	config->__info.isOpen ) {


        uart->Init.BaudRate = config->baudRate;

        if (HAL_UART_Init(uart) != HAL_OK)
        {
            JHAL_error("JHAL_uartOpen");
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

//串口接收失能
void JHAL_uartAbortReceiveIT (JHAL_UART *juart )
{




    __uartConfig[juart->id]->rxConfig.__info.itEN=false;

    __JHAL_uartAbortReceiveIT(juart->id);



}





void  __JHAL_uartEnableReceiveIT( u8 id )
{


    UART_HandleTypeDef *uart= (UART_HandleTypeDef * ) (  __uartConfig[id]->dev);




    if(  __uartConfig[id]->rxConfig.receiveMode==JHAL_UART_ReceiveMode_IT_FixedLength)
    {
        if(  __uartConfig[id]->rxConfig.__info.crcOK ||__uartConfig[id]->rxConfig .crcMode==JHAL_CRC_Mode_NONE ) {
            HAL_UART_Receive  (uart,__uartConfig[id]->rxConfig.dataBuff,__uartConfig[id]->rxConfig.length,100);
        } else {
            HAL_UART_Receive (uart,  &__uartConfig[id]->rxConfig.__info.byteData, 1,100);
        }

    } else    if(__uartConfig[id]->rxConfig.receiveMode== JHAL_UART_ReceiveITMode_IT_DMA_IDLE)
    {

        HAL_StatusTypeDef  st=  HAL_UARTEx_ReceiveToIdle_DMA(uart, __uartConfig[id]->rxConfig.dataBuff,__uartConfig[id]->rxConfig.length); //开启串口空闲中断DMA接收数据
        while(st!=HAL_OK)
        {
            __JHAL_uartAbortReceiveIT(id);
            st=  HAL_UARTEx_ReceiveToIdle_DMA(uart, __uartConfig[id]->rxConfig.dataBuff,__uartConfig[id]->rxConfig.length); //开启串口空闲中断DMA接收数据

        }


    }
}


/*串口接收使能*/
void  JHAL_uartEnableReceiveIT( JHAL_UART *uart  )
{


    __uartConfig[uart->id]->rxConfig.__info.start = false;
    __uartConfig[uart->id]->rxConfig.__info.dataCount=0;
    __uartConfig[uart->id]->rxConfig.__info.end=false;
    __uartConfig[uart->id]->rxConfig.__info.itEN=true;



    __JHAL_uartEnableReceiveIT(uart->id);
}



/**
  * 函数功能:串口空闲中断处理1


 */


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    u8 dev= __JHAL_uartInstance2dev (huart->  Instance );




    __uartConfig[dev]->rxConfig.__info.itEN=false;
    __uartConfig[dev]->rxConfig.__info.end=true;
    __uartConfig[dev]->rxConfig.__info.dataCount = Size;



}




/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    u8 dev= __JHAL_uartInstance2dev (huart->  Instance );


    if(!JHAL_uartRxITCallback(  dev))
    {
        return;
    }
    if( __uartConfig[  dev]->rxConfig.crcMode==JHAL_CRC_Mode_NONE ||__uartConfig[  dev]->rxConfig.__info. end)
// 接收完成可能存在异常继续中断
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
                ////如果上一次处于包头校验失败的状态在使能的时候切换的是单个中断
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

    while(HAL_UART_Transmit(uart,data,length,0xffff)!=HAL_OK )
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




 




extern JHAL_UART *   __JHAL_uartGetPrintfJUart(void);
/******重定向pr**BEGIN************/
int fputc (int ch,FILE * f)
{
    JHAL_uartSendDatas(__JHAL_uartGetPrintfJUart(),(uint8_t *)&ch,1,JHAL_CRC_Mode_NONE);
    return ch;
}

//接收的一般用不上 放在这
int fgetc (FILE * f)
{
    uint8_t ch=0;
    HAL_UART_Receive(__JHAL_uartGetPrintfJUart()->dev,&ch,1,0xffff);
    return ch;
}

/**********重定向pr*****END********/

#endif




