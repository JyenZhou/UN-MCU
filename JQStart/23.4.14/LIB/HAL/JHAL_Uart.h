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
//����ģʽ��N�ֽ��ж� (��ͷУ��ʱ+���ֽ�)��
        JHAL_UART_ReceiveMode_IT_FixedLength=0x01,
//�����������ж� DMA����
        JHAL_UART_ReceiveITMode_IT_DMA_IDLE=0x02,



    } JHAL_UART_ReceiveMode;


    typedef struct
    {
//���տ�ʼ�ı�־
        bool start:1;
//������ɱ�־λ
        bool   end:1;


        bool crcOK:1;
//�Ƿ�ʹ�ܽ�����
        bool itEN:1;

        //��ǰ���յ��ĳ��ȼ���
        uint16 dataCount;
//�����ֽڽ���ʱ������
        uint8_t byteData;



    } __JHAL_Uart_RxOtherInfo;





    typedef struct
    {
        bool isOpen:1;



    } __JHAL_Uart__OtherInfo;

    typedef struct
    {

        //���ݽ��ջ����� ��Ҫȫ�ֵĲ��Ҵ��ڵ��ڽ��ճ���
        uint8_t *dataBuff ;
//���ճ��� ������ģʽ��Ϊ��󳤶� ����dataBuff����
        uint16 length;
        //����CRCģʽ
        JHAL_CRC_Mode crcMode;
//����ͷ��CRCģʽ��һ���,ֻ��NONEʱ��У���ͷ��
        uint8   headCode ;
//����ģʽ
        JHAL_UART_ReceiveMode receiveMode;



        __JHAL_Uart_RxOtherInfo __info;


    } JHAL_UART_RXConfig;


    typedef struct {
        u8 id:3;
        u32 baudRate;
        JHAL_UART_RXConfig rxConfig;
        __JHAL_Uart__OtherInfo __info;
        // �������������õ� ����stm32ʹ��cubemx���ú�ֱ�ӽ��Ǳ߶���ı��������� ����Ҫ�ڲ����¸�ֵ��
//			����ڲ�ʹ�����������һ��Ҫ�ȸ�ֵ
        void*  dev;

    } JHAL_UART ;



    bool JHAL_uartSendDatas(JHAL_UART *uart,u8* data,u16  length,JHAL_CRC_Mode mode);
    /*configȫ�ֲ��ɱ��ͷ�  */
    bool JHAL_uartOpen ( JHAL_UART  *uart);
    bool  JHAL_uartClose(JHAL_UART *uart);


    /*ʹ�ܽ��� �������ڽ��յ��������������ļ��е�Ҫ��ʱ��Ϊ���ν������  �� JHAL_uartRxFinsh �ķ���ֵ��true */
    void JHAL_uartEnableReceiveIT ( JHAL_UART *uart);

    void JHAL_uartAbortReceiveIT (JHAL_UART *uart);

    /*����ֵ��ʵ�ʽ��ճ��� ������ֵ��Ϊ0ʱ����ɱ��ν��� ʵ�ʽ��������ڳ�ʼ���� dataBuff ��*/
    u16 JHAL_uartRxFinsh(JHAL_UART *uart);

//�жϻص� ����false����Ϊȡ�������н��շ�װ����  JHAL_uartRxFinsh����Ч  ������*dataBuff ����õ���ֵ
    //�жϻص�  ע�������Ӧ����uartʵ�� id ���Ǵ���ȥ��dev  ��Ϊ����dev����ʵ������һ��uart ֻ�����Ų�һ��
    bool JHAL_uartRxITCallback(u8 realityId);



#ifdef CplusPlus
}
#endif

#endif








