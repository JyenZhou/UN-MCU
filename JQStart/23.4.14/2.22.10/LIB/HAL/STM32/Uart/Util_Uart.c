/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*

CUBEMX ����
UART
1.baud Rate ����������
2.NVIC USARTX global interupt����ȫ���ж�
3.Rank1



#include "Hall_bip_includes.h"
��ӿ����жϴ����� ����

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
    utilUart_RxIdleCallback(&huart1);
}


ע�� �����շ�����ͬʱ���У���Ҫ��   ����ͨ����ʼ���պͽ�����ɱ�־λ������  ���ǵ��жϽ���Ҳ�ǽ�������  �Ͱ��ⲽ�ɵ����˴���

*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#ifdef UTIL_UART

#include "communication.h"
#include "Util_Uart.h"
//��ʱ��
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
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


    if(huart->Instance == UART7)	// �ж����¶�
    {
        tempModuleReceiveHandlr();
        return;
    }

    if(huart->Instance == USART6)	// �ж���epc
    {
        epcModuleReceiveHandlr();
        return;
    }


#ifdef UTIL_UART1_EN
    // if(huart == &huart1)
    // ��������һ��������
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
                                    //ƽ��
                                }


}

/**
  * ��������:���ڿ����жϴ���1
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����

  ��ӿ����жϴ������� �� USART1_IRQHandler(void) (�жϴ�����)
     utilUart_RxIdleCallback(&huartx); ���Ҹú������жϴ���ĺ��� ��Ϊ���ֽڽ��մ���ʱ�ڴ��ڽ�������

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
        //�ж��Ƿ��ǿ����ж�  ��Ϊ����Ƿ��ڴ����ж��е� �����ж�ʱҲ�����
        return;
    }
    //��������жϱ�־�������һֱ���Ͻ����жϣ�
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    //������崮��1�Ĳ���������
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
//ƴ���õĻ�����{}����ɾ
                                }

}





/*�������ú���*/
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
//ƴ�ӵĲ���ɾ��
                                }



}


/*-----------------------------------------------------------------------*/

/*  ���ڵĽ���CRC����

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

/*  ��ͨ����ģʽ�ķ���*/

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

/*  ���ڵĽ���CRC����

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
//У��ʧ�����·������
        utilUart_Receive_IT(utilUartHandler);



    }




}

/*-----------------------------------------------------------------------*/

/*  ���ڵĽ��մ���   */

/*-----------------------------------------------------------------------*/
void  utilUart_ReceiveHandler(UtilUart_Handler_T *utilUartHandler)
{

    if( utilUartHandler-> receiveFinished|| utilUartHandler->privateParameter.receiveDataCount>=utilUartHandler->receiveLength)
//������Ҫ����������Ҳ����������ɴ���
    {
        utilUartHandler-> receiveFinished=true;
        return;
    }

    if( utilUartHandler->receiveMode==UtilUart_ReceiveMode_IDLE)
    {
        utilUartHandler->privateParameter.receiveData[ utilUartHandler->privateParameter.receiveDataCount++]=  utilUartHandler->privateParameter.receiveDataPartial;

    } else if(utilUartHandler->receiveMode==UtilUart_ReceiveMode_FixedLength)
    {
//�Ƿ��ѿ���У��
        if( utilUartHandler->receiveCRCMode!=UtilUart_CRC_NONE)
        {
//�����һ�δ��ڰ�ͷУ��ʧ�ܵ�״̬��ʹ�ܵ�ʱ���л����ǵ����ж�

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
//�����һ�δ���У��ɹ� �����Ƕ��ֽ��жϽ�� ֱ���ж�
                crcRxHandler(utilUartHandler) ;
            }
        }

//���û����У�� ֱ�ӳɹ�
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
/*���ڽ�����ʹ��
��������н����ֽ� ��������  �����ڶ��ֽڽ�����  �÷������ڲ�����*/
void utilUart_ReceiveEN( UtilUart_Handler_T *utilUart_Handle)
{
    if  ( utilUart_Handle->receiveFinished)
    {
//��������ʱ��ʹ�ܽ�����Ҫ�ǵ�Ƭ���ǵ��߳��½�������̫�ർ�·���ʱ���൱�ڷְ�   ������ɱ�־λ˵����û��������һ����������Ҳû��Ҫ�ٽ�����
        return;
    }
//DMA��������
    if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_DMA_FixedLength)
    {
        if( utilUart_Handle->privateParameter. lastCrcCheckOK)
        {

            HAL_UART_Receive_DMA(utilUart_Handle->huart,  utilUart_Handle->privateParameter.receiveData,  utilUart_Handle->receiveLength);
        } else
        {
            HAL_UART_Receive_DMA(utilUart_Handle->huart, &(utilUart_Handle->privateParameter.receiveDataPartial), 1);
        }
    }//���н���ģʽ����������
    else if( utilUart_Handle->receiveMode==UtilUart_ReceiveMode_IDLE)
    {

        __HAL_UART_ENABLE_IT(utilUart_Handle->huart,UART_IT_IDLE);
        HAL_UART_Receive_IT(utilUart_Handle->huart,& utilUart_Handle->privateParameter.receiveDataPartial,1);
        //����ʹ�ܺ�����رս��գ������´򿪣������жϱ�־�ᱻ��λ���ٹرս��գ������´򿪣������жϱ�־�ֻᱻ��λ����˷�������
        __HAL_UART_CLEAR_IDLEFLAG(utilUart_Handle->huart);

    } //��������ģʽ
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



/*���ڽ���ʹ��  ����ѽ������ݺͽ�����ɱ�־λ  һ���ֶ���������ʱ����������Ҫ�ٽ���ʱҪ����*/
void utilUart_Receive_IT (UtilUart_Handler_T *utilUart_Handle)
{
    utilUart_AbortReceive_IT(utilUart_Handle);
    utilUart_Handle->privateParameter.receiveITEN=true;
    utilUart_Handle->receiveFinished=false;
    utilUart_Handle->privateParameter.receiveDataCount=0;
    utilUart_Handle->privateParameter.receivStart = false;

    utilUart_ReceiveEN(  utilUart_Handle);
}
//���ڽ���ʧ��
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






/*********�����Ǵ�����ص�����****�������ж��������º���******************/
//��������STM32CubeMX���ɡ�
//��һ����ĿҪ�õ�����ͨѶ���쳣���ݻ�ʹ����ֱ�ӿ��������Ҳ���ָ���ֻ�������ϵ���ָܻ���
//�����ѯ����һֱ���ж�������ߣ�
//void USART3_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART3_IRQn 0 */

//  /* USER CODE END USART3_IRQn 0 */
//  HAL_UART_IRQHandler(&huart3);
//  /* USER CODE BEGIN USART3_IRQn 1 */

//  /* USER CODE END USART3_IRQn 1 */
//}

//���ϲ�ѯ���кܶ����Ƶ������������ORE���£����������������һֱ�岻��

//ԭ���ҵ��ˣ����ǲ�֪����ô�������������Ҳ�Ҳ������������ֻ���Լ������
//1���Ȳ��ֲᣬ�鿴�����־����Щ��Ҫ��ô����������ϴ�ͼƬ�����ֲ��ͼ�Ͳ�����
//2����ô����HAL���ļ�������־






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

/******�ض���pr**BEGIN************/
int fputc (int ch,FILE * f)
{

    utilUart_sendDat(&utilUart_1_Handle,(uint8_t *)&ch,1);
    return ch;
}

//���յ�һ���ò��� ������
int fgetc (FILE * f)
{
    uint8_t ch=0;
    HAL_UART_Receive(&huart1,&ch,1,0xffff);
    return ch;
}

/**********�ض���pr*****END********/
#endif
#endif




