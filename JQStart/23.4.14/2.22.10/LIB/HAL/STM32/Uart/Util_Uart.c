/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*

CUBEMX 配置
UART
1.baud Rate 波特率配置
2.NVIC USARTX global interupt开启全局中断
3.Rank1



#include "Hall_bip_includes.h"
添加空闲中断处理函数 例：

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
    utilUart_RxIdleCallback(&huart1);
}


注意 串口收发不能同时进行，需要错开   本来通过开始接收和接收完成标志位来处理  考虑到判断接收也是接收里面  就把这步由调用人处理

*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#ifdef UTIL_UART

#include "communication.h"
#include "Util_Uart.h"
//临时的
#include "temperature.h"
#include "epc.h"
#ifdef UTIL_UART1_EN

uint8   utilUart_1_RxBuff[UTIL_UART1_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_1_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART1_RXBUFF_MAXSIZE
};


#endif
#ifdef UTIL_UART2_EN
uint8   utilUart_2_RxBuff[UTIL_UART2_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_2_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART2_RXBUFF_MAXSIZE
};

#endif
#ifdef UTIL_UART3_EN
uint8   utilUart_3_RxBuff[UTIL_UART3_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_3_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART3_RXBUFF_MAXSIZE
};

#endif

#ifdef UTIL_UART4_EN
uint8   utilUart_4_RxBuff[UTIL_UART4_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_4_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART4_RXBUFF_MAXSIZE
};


#endif

#ifdef UTIL_UART5_EN
uint8   utilUart_5_RxBuff[UTIL_UART5_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_5_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART5_RXBUFF_MAXSIZE
};

#endif

#ifdef UTIL_UART6_EN
uint8   utilUart_6_RxBuff[UTIL_UART6_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_6_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART6_RXBUFF_MAXSIZE
};

#endif

#ifdef UTIL_UART7_EN
uint8   utilUart_7_RxBuff[UTIL_UART7_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_7_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART7_RXBUFF_MAXSIZE
};

#endif

#ifdef UTIL_UART8_EN
uint8   utilUart_8_RxBuff[UTIL_UART8_RXBUFF_MAXSIZE];
UtilUart_Handler_T utilUart_8_Handle= {
    .rxHeadCode=0xA5,
    .receiveCRCMode=UtilUart_CRC_CRC16_Modbus,
    .sendCRCMode=UtilUart_CRC_CRC16_Modbus,
    .receiveMode= UtilUart_ReceiveMode_FixedLength,
    .receiveLength= UTIL_UART8_RXBUFF_MAXSIZE

};

#endif

void utilUart_ReceiveEN( UtilUart_Handler_T *utilUart_Handle);
void utilUart_RxIdleHandler(UtilUart_Handler_T *utilUartHandler);
void  utilUart_ReceiveHandler(UtilUart_Handler_T *utilUartHandler);


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

    if (CoreDebug->DHCSR & 1)
    {
        // __breakpoint(0);
    }
    if(huart->ErrorCode&HAL_UART_ERROR_ORE)
    {
        if (CoreDebug->DHCSR & 1)
        {
            //  __breakpoint(0);
        }
        __HAL_UART_CLEAR_OREFLAG(huart);

    }
}
/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


    if(huart->Instance == UART7)	// 判断是温度
    {
        tempModuleReceiveHandlr();
        return;
    }

    if(huart->Instance == USART6)	// 判断是epc
    {
        epcModuleReceiveHandlr();
        return;
    }


#ifdef UTIL_UART1_EN
    // if(huart == &huart1)
    // 这两个是一样的作用
    if(huart->Instance==USART1)
    {

        utilUart_ReceiveHandler(&utilUart_1_Handle);
    } else
#endif



#ifdef UTIL_UART2_EN
        if(huart->Instance==USART2)
        {
            utilUart_ReceiveHandler(&utilUart_2_Handle);
        } else
#endif


#ifdef UTIL_UART3_EN
            if(huart == &huart3)
            {
                utilUart_ReceiveHandler(&utilUart_3_Handle);
            } else
#endif

#ifdef UTIL_UART4_EN
                if(huart == &huart4)
                {
                    utilUart_ReceiveHandler(&utilUart_4_Handle);
                } else
#endif
#ifdef UTIL_UART5_EN
                    if(huart == &huart5)
                    {
                        utilUart_ReceiveHandler(&utilUart_5_Handle);
                    } else
#endif
#ifdef UTIL_UART6_EN
                        if(huart == &huart6)
                        {
                            utilUart_ReceiveHandler(&utilUart_6_Handle);
                        } else
#endif
#ifdef UTIL_UART7_EN
                            if(huart == &huart7)
                            {
                                utilUart_ReceiveHandler(&utilUart_7_Handle);
                            } else
#endif
#ifdef UTIL_UART8_EN
                                if(huart == &huart8)
                                {
                                    utilUart_ReceiveHandler(&utilUart_8_Handle);
                                } else
#endif


                                {
                                    //平接
                                }


}

/**
  * 函数功能:串口空闲中断处理1
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：

  添加空闲中断处理函数： 在 USART1_IRQHandler(void) (中断处理函数)
     utilUart_RxIdleCallback(&huartx); 并且该函数在中断处理的后面 因为单字节接收处理时在串口接收里面

 void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
    utilUart_RxIdleCallback(&huart1);
}


 */
void utilUart_RxIdleCallback(UART_HandleTypeDef *huart)
{
    if(!__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
    {
        //判断是否是空闲中断  因为这个是放在串口中断中的 串口中断时也会进来
        return;
    }
    //清除空闲中断标志（否则会一直不断进入中断）
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    //如果定义串口1的不定长接收
#ifdef UTIL_UART1_EN
    if( USART1 == huart->Instance)
    {
        utilUart_RxIdleHandler(&utilUart_1_Handle);

    } else
#endif
#ifdef UTIL_UART2_EN
        if( USART2 == huart->Instance)
        {
            utilUart_RxIdleHandler(&utilUart_2_Handle);

        } else
#endif
#ifdef UTIL_UART3_EN
            if( USART3 == huart->Instance)
            {
                utilUart_RxIdleHandler(&utilUart_3_Handle);

            } else
#endif


#ifdef UTIL_UART4_EN
                if(huart->Instance==UART4)
                {
                    utilUart_RxIdleHandler(&utilUart_4_Handle);

                } else
#endif
#ifdef UTIL_UART5_EN
                    if(huart->Instance==UART5)
                    {
                        utilUart_RxIdleHandler(&utilUart_5_Handle);

                    } else
#endif
#ifdef UTIL_UART6_EN
                        if(huart->Instance==USART6)
                        {
                            utilUart_RxIdleHandler(&utilUart_6_Handle);

                        } else
#endif
#ifdef UTIL_UART7_EN
                            if(huart->Instance==UART7)
                            {
                                utilUart_RxIdleHandler(&utilUart_7_Handle);

                            } else
#endif
#ifdef UTIL_UART8_EN
                                if(huart->Instance==UART8)
                                {
                                    utilUart_RxIdleHandler(&utilUart_8_Handle);

                                } else
#endif
                                {
//拼接用的花括号{}不能删
                                }

}





/*串口配置函数*/
void  utilUart_Configuration(UtilUart_Handler_T *utilUartHandler)
{

    utilUart_AbortReceive_IT(utilUartHandler);


#ifdef UTIL_UART1_EN
    if(utilUartHandler->id==0x01)
    {
        if(utilUartHandler->receiveLength>UTIL_UART1_RXBUFF_MAXSIZE )
        {
            hall_bisp_ErrorHandler(UtilUart_Iint_Error);
        }


    } else
#endif
#ifdef UTIL_UART2_EN
        if(utilUartHandler->id==0x02)
        {
            if(utilUartHandler->receiveLength>UTIL_UART2_RXBUFF_MAXSIZE )
            {
                hall_bisp_ErrorHandler(UtilUart_Iint_Error);
            }

        }
        else
#endif
#ifdef UTIL_UART3_EN
            if(utilUartHandler->id==0x03)
            {
                if(utilUartHandler->receiveLength>UTIL_UART3_RXBUFF_MAXSIZE )
                {
                    hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                }

            }
            else
#endif
#ifdef UTIL_UART4_EN
                if(utilUartHandler->id==0x04)
                {
                    if(utilUartHandler->receiveLength>UTIL_UART4_RXBUFF_MAXSIZE )
                    {
                        hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                    }



                }
                else
#endif
#ifdef UTIL_UART5_EN
                    if(utilUartHandler->id==0x05)
                    {
                        if(utilUartHandler->receiveLength>UTIL_UART5_RXBUFF_MAXSIZE )
                        {
                            hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                        }
                    }
                    else
#endif
#ifdef UTIL_UART6_EN
                        if(utilUartHandler->id==0x06)
                        {
                            if(utilUartHandler->receiveLength>UTIL_UART6_RXBUFF_MAXSIZE )
                            {
                                hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                            }

                        }
                        else
#endif
#ifdef UTIL_UART7_EN
                            if(utilUartHandler->id==0x07)
                            {
                                if(utilUartHandler->receiveLength>UTIL_UART7_RXBUFF_MAXSIZE )
                                {
                                    hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                                }
                            }
                            else
#endif
#ifdef UTIL_UART8_EN
                                if(utilUartHandler->id==0x08)
                                {
                                    if(utilUartHandler->receiveLength>UTIL_UART8_RXBUFF_MAXSIZE )
                                    {
                                        hall_bisp_ErrorHandler(UtilUart_Iint_Error);
                                    }
                                }
                                else
#endif
                                {
//拼接的不能删除
                                }



}


/*-----------------------------------------------------------------------*/

/*  串口的接收CRC处理

 */
/*-----------------------------------------------------------------------*/
static  void crcTxHandler(UtilUart_Handler_T *utilUartHandler,uint8* sendData,uint8 dataLength)
{



    if(utilUartHandler->sendCRCMode==UtilUart_CRC_CRC16_Modbus)
    {
        crc16_Modbus(sendData,dataLength);

    } else if(utilUartHandler->sendCRCMode==UtilUart_CRC_XorBCC) {
        crc8_XOR(sendData,dataLength);
    }


}

/*-----------------------------------------------------------------------*/

/*  普通串口模式的发送*/

/*-----------------------------------------------------------------------*/

void utilUart_sendDat(UtilUart_Handler_T *utilUartHandler, uint8 *data,uint16 size)
{

    bool receiveITEN=   utilUartHandler->privateParameter.receiveITEN;

    if(receiveITEN)
    {
        utilUart_AbortReceive_IT (utilUartHandler);
    }
    crcTxHandler(utilUartHandler,data,size);



    uint16_t i=0;
    while( utilUartHandler->huart->gState != HAL_UART_STATE_READY )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(utilUartHandler->huart);
        }
    }
    i=0;
    while(  HAL_UART_Transmit(utilUartHandler->huart,data,size,0xffff)!=HAL_OK )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(utilUartHandler->huart);
        }
    }
    if(receiveITEN)
    {
        utilUart_Receive_IT (utilUartHandler);
    }

}


/*-----------------------------------------------------------------------*/

/*  串口的接收CRC处理

 */
/*-----------------------------------------------------------------------*/
static  void crcRxHandler(UtilUart_Handler_T *utilUartHandler)
{


    bool crcOK=false;


    if(utilUartHandler->receiveCRCMode==UtilUart_CRC_CRC16_Modbus)
    {
        if(crc16_Modbus_Cheak(utilUartHandler->privateParameter.receiveData,utilUartHandler->receiveLength))
        {
            crcOK=true;
        }

    } else if(utilUartHandler->receiveCRCMode==UtilUart_CRC_XorBCC) {
        if(crc8_XOR_Cheak(utilUartHandler->privateParameter.receiveData,utilUartHandler->receiveLength))
        {
            crcOK=true;
        }
    }


    if(crcOK)
    {
        utilUartHandler->privateParameter.receiveDataCount= utilUartHandler->receiveLength;
        utilUartHandler->privateParameter.lastCrcCheckOK=true;
        utilUartHandler->receiveFinished=true;
    } else {

        utilUartHandler->privateParameter.lastCrcCheckOK=false;
//校验失败重新发起接收
        utilUart_Receive_IT(utilUartHandler);



    }




}

/*-----------------------------------------------------------------------*/

/*  串口的接收处理   */

/*-----------------------------------------------------------------------*/
void  utilUart_ReceiveHandler(UtilUart_Handler_T *utilUartHandler)
{

    if( utilUartHandler-> receiveFinished|| utilUartHandler->privateParameter.receiveDataCount>=utilUartHandler->receiveLength)
//当接收要超过缓冲了也当做接收完成处理
    {
        utilUartHandler-> receiveFinished=true;
        return;
    }

    if( utilUartHandler->receiveMode==UtilUart_ReceiveMode_IDLE)
    {
        utilUartHandler->privateParameter.receiveData[ utilUartHandler->privateParameter.receiveDataCount++]=  utilUartHandler->privateParameter.receiveDataPartial;

    } else if(utilUartHandler->receiveMode==UtilUart_ReceiveMode_FixedLength)
    {
//是否已开启校验
        if( utilUartHandler->receiveCRCMode!=UtilUart_CRC_NONE)
        {
//如果上一次处于包头校验失败的状态在使能的时候切换的是单个中断

            if(! utilUartHandler->privateParameter.lastCrcCheckOK)
            {
                if((! utilUartHandler->privateParameter.receivStart)&&( utilUartHandler->privateParameter.receiveDataPartial== utilUartHandler->rxHeadCode))
                {
                    utilUartHandler->privateParameter.receivStart = true;
                    utilUartHandler->privateParameter.receiveDataCount=0;
                }
                if( utilUartHandler->privateParameter.receivStart)
                {
                    utilUartHandler->privateParameter.receiveData[ utilUartHandler->privateParameter.receiveDataCount++] =  utilUartHandler->privateParameter.receiveDataPartial;
                }
                if( utilUartHandler->privateParameter.receiveDataCount== utilUartHandler->receiveLength)
                {

                    crcRxHandler(utilUartHandler);

                } else {
                    utilUart_ReceiveEN(utilUartHandler);
                }

            } else {
//如果上一次处于校验成功 这里是多字节中断结果 直接判断
                crcRxHandler(utilUartHandler) ;
            }
        }

//如果没有用校验 直接成功
        else
        {
            utilUartHandler->privateParameter.receiveDataCount=utilUartHandler->receiveLength;
            utilUartHandler->receiveFinished=true;
            return;
        }


    }
}


static void utilUart_Receive_IT_RN(UART_HandleTypeDef *huart,uint8* rxData,uint16 size)
{
    HAL_UART_AbortReceive(huart);
    uint16_t i=0;
    while(HAL_UART_Receive_IT(huart,  rxData, size )!= HAL_OK )
        //  while(HAL_UART_Receive_IT(huart,  utilUart_1_RxBuff, size )!= HAL_OK )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            huart->RxState = HAL_UART_STATE_READY;
            __HAL_UNLOCK(huart);
        }
    }
}
/*串口接收再使能
不清除已有接收字节 继续接收  适用于多字节接收下  该方法仅内部调用*/
void utilUart_ReceiveEN( UtilUart_Handler_T *utilUart_Handle)
{
    if  ( utilUart_Handle->receiveFinished)
    {
//发送数据时候不使能接收主要是单片机是单线程怕接收数据太多导致发送时候相当于分包   接收完成标志位说明并没处理完上一组数据所以也没必要再接收了
        return;
    }
//DMA定长接收
    if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_DMA_FixedLength)
    {
        if( utilUart_Handle->privateParameter. lastCrcCheckOK)
        {

            HAL_UART_Receive_DMA(utilUart_Handle->huart,  utilUart_Handle->privateParameter.receiveData,  utilUart_Handle->receiveLength);
        } else
        {
            HAL_UART_Receive_DMA(utilUart_Handle->huart, &(utilUart_Handle->privateParameter.receiveDataPartial), 1);
        }
    }//空闲接收模式（不定长）
    else if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_IDLE)
    {

        __HAL_UART_ENABLE_IT(utilUart_Handle->huart,UART_IT_IDLE);
        HAL_UART_Receive_IT(utilUart_Handle->huart,& utilUart_Handle->privateParameter.receiveDataPartial,1);
        //串口使能后，如果关闭接收，再重新打开，空闲中断标志会被置位，再关闭接收，再重新打开，空闲中断标志又会被置位，如此反复。。
        __HAL_UART_CLEAR_IDLEFLAG(utilUart_Handle->huart);

    } //定长接收模式
    else if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_FixedLength)
    {
        if( utilUart_Handle->receiveCRCMode!=UtilUart_CRC_NONE)
        {
            if( utilUart_Handle->privateParameter.lastCrcCheckOK)
            {
                utilUart_Receive_IT_RN(utilUart_Handle->huart,  utilUart_Handle->privateParameter.receiveData,  utilUart_Handle->receiveLength);
            }
            else
            {
                utilUart_Receive_IT_RN(utilUart_Handle->huart,  & utilUart_Handle->privateParameter.receiveDataPartial, 1);
            }
        }
        else
        {
            utilUart_Receive_IT_RN(utilUart_Handle->huart,  utilUart_Handle->privateParameter.receiveData,  utilUart_Handle->receiveLength);
        }
    }



}



/*串口接收使能  清除已接收数据和接收完成标志位  一般手动开启接收时或接收完成需要再接收时要调用*/
void utilUart_Receive_IT (UtilUart_Handler_T *utilUart_Handle)
{
    utilUart_AbortReceive_IT(utilUart_Handle);
    utilUart_Handle->privateParameter.receiveITEN=true;
    utilUart_Handle->receiveFinished=false;
    utilUart_Handle->privateParameter.receiveDataCount=0;
    utilUart_Handle->privateParameter.receivStart = false;

    utilUart_ReceiveEN(  utilUart_Handle);
}
//串口接收失能
void utilUart_AbortReceive_IT (UtilUart_Handler_T *utilUart_Handle)
{
    utilUart_Handle->privateParameter.receiveITEN=false;
    uint16_t i=0;
    while( HAL_UART_Abort_IT(utilUart_Handle->huart)!= HAL_OK )
    {
        i++;
        HAL_Delay(1);
        if( i > 100 )
        {
            __HAL_UNLOCK(utilUart_Handle->huart);
        }
    }

    if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_IDLE)
    {
        __HAL_UART_DISABLE_IT(utilUart_Handle->huart,UART_IT_IDLE);
    }
}





void utilUart_RxIdleHandler(UtilUart_Handler_T *utilUartHandler)
{

    if( utilUartHandler->receiveMode==UtilUart_ReceiveMode_IDLE)
    {
        utilUartHandler->receiveFinished=true;
    }

    return;
}






/*********下面是错误处理回调函数****仅处理中断死在以下函数******************/
//程序框架用STM32CubeMX生成。
//有一个项目要用到串口通讯，异常数据会使串口直接卡死，而且不会恢复，只能重新上电才能恢复。
//仿真查询，会一直进中断死在这边：
//void USART3_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART3_IRQn 0 */

//  /* USER CODE END USART3_IRQn 0 */
//  HAL_UART_IRQHandler(&huart3);
//  /* USER CODE BEGIN USART3_IRQn 1 */

//  /* USER CODE END USART3_IRQn 1 */
//}

//网上查询了有很多类似的情况，错误由ORE导致，这个错误置起来后一直清不掉

//原因找到了，但是不知道怎么解决，但是网上也找不到解决方法，只能自己解决了
//1、先查手册，查看错误标志有哪些，要怎么清掉，不会上传图片所以手册截图就不发了
//2、怎么调用HAL库文件清错误标志






void utilUart_Init(void)
{
#ifdef UTIL_UART1_EN

    utilUart_1_Handle.huart=&huart1;
    utilUart_1_Handle.privateParameter.receiveData=utilUart_1_RxBuff;
    utilUart_1_Handle.id=0x01;
    utilUart_Configuration(&utilUart_1_Handle);
    utilUart_Receive_IT (&utilUart_1_Handle);

#endif
#ifdef UTIL_UART2_EN
    utilUart_2_Handle.huart=&huart2;
    utilUart_2_Handle.privateParameter.receiveData=utilUart_2_RxBuff;
    utilUart_2_Handle.id=0x02;
    utilUart_Configuration(&utilUart_2_Handle);
    utilUart_Receive_IT (&utilUart_2_Handle);
#endif


#ifdef UTIL_UART3_EN
    utilUart_3_Handle.huart=&huart3;
    utilUart_3_Handle.privateParameter.receiveData=utilUart_3_RxBuff;
    utilUart_3_Handle.id=0x03;
    utilUart_Receive_IT (&utilUart_3_Handle);

#endif

#ifdef UTIL_UART4_EN
    utilUart_4_Handle.huart=&huart4;
    utilUart_4_Handle.privateParameter.receiveData=utilUart_4_RxBuff;
    utilUart_4_Handle.id=0x04;
    utilUart_Configuration(&utilUart_4_Handle);
    utilUart_Receive_IT (&utilUart_4_Handle);

#endif

#ifdef UTIL_UART5_EN
    utilUart_5_Handle.huart=&huart5;
    utilUart_5_Handle.privateParameter.receiveData=utilUart_5_RxBuff;
    utilUart_5_Handle.id=0x05;
    utilUart_Configuration(&utilUart_5_Handle);
    utilUart_Receive_IT (&utilUart_5_Handle);

#endif

#ifdef UTIL_UART6_EN

    utilUart_6_Handle.huart=&huart6;
    utilUart_6_Handle.privateParameter.receiveData=utilUart_6_RxBuff;
    utilUart_6_Handle.id=0x06;
    utilUart_Configuration(&utilUart_6_Handle);
    utilUart_Receive_IT (&utilUart_6_Handle);

#endif

#ifdef UTIL_UART7_EN
    utilUart_7_Handle.huart=&huart7;
    utilUart_7_Handle.privateParameter.receiveData=utilUart_7_RxBuff;
    utilUart_7_Handle.id=0x07;
    utilUart_Configuration(&utilUart_7_Handle);
    utilUart_Receive_IT (&utilUart_7_Handle);

#endif

#ifdef UTIL_UART8_EN
    utilUart_8_Handle.huart=&huart8;
    utilUart_8_Handle.privateParameter.receiveData=utilUart_8_RxBuff;
    utilUart_8_Handle.id=0x08;
    utilUart_Configuration(&utilUart_8_Handle);
    utilUart_Receive_IT (&utilUart_8_Handle);
#endif




}












#ifdef UTIL_UART1_EN

/******重定向pr**BEGIN************/
int fputc (int ch,FILE * f)
{

    utilUart_sendDat(&utilUart_1_Handle,(uint8_t *)&ch,1);
    return ch;
}

//接收的一般用不上 放在这
int fgetc (FILE * f)
{
    uint8_t ch=0;
    HAL_UART_Receive(&huart1,&ch,1,0xffff);
    return ch;
}

/**********重定向pr*****END********/
#endif
#endif




