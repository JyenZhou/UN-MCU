/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CANĬ��ͨ��HAL����
  ʹ��CAN0 ֡���� Ĭ������֡��Զ��֡         �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_UART_H__
#define __JHAL_UART_H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif


    typedef enum
    {
        JHAL_UART0=0,
        JHAL_UART1,
        JHAL_UART_Number
    } JHAL_UART;







    typedef enum
    {
//����ģʽ��N�ֽ��ж� (��ͷУ��ʱ+���ֽ�)��
        JHAL_UART_ReceiveMode_IT_FixedLength=0x01,
//���������н����жϣ����ֽ��жϽ��մ��ڽ��ջ�������ͨ������ʱȥȡֵ��
        JHAL_UART_ReceiveITMode_IT_IDLE=0x02,
//DMA����ģʽ��N�ֽ��жϣ�
        JHAL_UART_ReceiveITMode_IT_DMA_FixedLength=0x03,
    } JHAL_UART_ReceiveMode;


    typedef struct
    {
//���տ�ʼ�ı�־
        bool start;
//������ɱ�־λ
        bool   end;

//��ǰ���յ��ĳ��ȼ���
        uint16 dataCount;
//�����ֽڽ���ʱ������
        uint8_t byteData;

//�Ƿ�ʹ�ܽ�����
        bool itEN;



    } __JHAL_Uart_RxInfo;

    typedef struct
    {

//���ճ��� ������ģʽ��Ϊ��󳤶�
        uint16 length;
//����ͷ��CRCģʽ��һ���,ֻ��NONEʱ��У���ͷ��
        uint8   headCode ;
//����ģʽ
        JHAL_UART_ReceiveMode receiveMode;
//����CRCģʽ
        JHAL_CRC_Mode crcMode;
//���ݽ��ջ����� ��Ҫȫ�ֵĲ��Ҵ��ڵ��ڽ��ճ���
        uint8_t *dataBuff;

        __JHAL_Uart_RxInfo __rxInfo;
    } JHAL_UART_RXConfig;


    typedef struct {
        uint32_t baudRate;
        JHAL_UART_RXConfig rxConfig;

    } JHAL_UARTConfig;



    bool JHAL_uartSendDatas(JHAL_UART uart,u8* data,u16  length,JHAL_CRC_Mode mode);
    /*configȫ�ֲ��ɱ��ͷ�*/
    bool JHAL_uartInit(JHAL_UART uart,JHAL_UARTConfig *config);
    bool  JHAL_uartDeInit(JHAL_UART uart);


    /*ʹ�ܽ��� �������ڽ��յ��������������ļ��е�Ҫ��ʱ��Ϊ���ν������  �� JHAL_uartRxFinsh �ķ���ֵ��true */
    void JHAL_uartEnableReceiveIT (JHAL_UART uart);

    void JHAL_uartAbortReceiveIT (JHAL_UART uart);

    /*����ֵ��ʵ�ʽ��ճ��� ������ֵ��Ϊ0ʱ����ɱ��ν��� ʵ�ʽ��������ڳ�ʼ���� dataBuff ��*/
    u16 JHAL_uartRxFinsh(JHAL_UART uart);

//�жϻص� ����false����Ϊȡ�������н��շ�װ����  JHAL_uartRxFinsh����Ч  ������*dataBuff ����õ���ֵ
    bool JHAL_uartRxITCallback(u8 uartID);



#ifdef CplusPlus
}
#endif

#endif







