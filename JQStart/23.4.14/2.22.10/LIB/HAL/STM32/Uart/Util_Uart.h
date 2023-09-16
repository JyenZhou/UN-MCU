#ifdef UTIL_UART


/*ʹ��ָ������
***/
#define UTIL_UART1_EN
//#define UTIL_UART2_EN
//#define UTIL_UART3_EN
//#define UTIL_UART4_EN
//#define UTIL_UART5_EN
#define UTIL_UART6_EN
#define UTIL_UART7_EN
//#define UTIL_UART8_EN


/****************************/


#ifndef _UTIL_UART_H_
#define _UTIL_UART_H_
#include "Hall_includes.h"



#ifdef UTIL_UART1_EN
#define UTIL_UART1_RXBUFF_MAXSIZE 70
#endif

#ifdef UTIL_UART2_EN
#define UTIL_UART2_RXBUFF_MAXSIZE 30
#endif

#ifdef UTIL_UART3_EN
#define UTIL_UART3_RXBUFF_MAXSIZE 7
#endif
#ifdef UTIL_UART4_EN
#define UTIL_UART4_RXBUFF_MAXSIZE  75
#endif

#ifdef UTIL_UART5_EN
#define UTIL_UART5_RXBUFF_MAXSIZE 100
#endif
#ifdef UTIL_UART6_EN
#define UTIL_UART6_RXBUFF_MAXSIZE 35
#endif

#ifdef UTIL_UART7_EN
#define UTIL_UART7_RXBUFF_MAXSIZE 1
#endif
#ifdef UTIL_UART8_EN
#define UTIL_UART8_RXBUFF_MAXSIZE 35
#endif
typedef enum
{
//����ģʽ��N�ֽ��ж� (��ͷУ��ʱ+���ֽ�)��
    UtilUart_ReceiveMode_FixedLength=0x01,
//���������н����жϣ����ֽ��жϽ��մ��ڽ��ջ�������ͨ������ʱȥȡֵ��
    UtilUart_ReceiveMode_IDLE=0x02,
//DMA����ģʽ��N�ֽ��жϣ�
    UtilUart_ReceiveMode_DMA_FixedLength=0x03,
} UtilUart_ReceiveMode_E;


typedef enum
{
//ֻ�� UtilUart_NONE  ����ͷ��CRCģʽ��һ���,ֻ��NONEʱ��У���ͷ��
    UtilUart_CRC_NONE,
    UtilUart_CRC_XorBCC,
   UtilUart_CRC_CRC16_Modbus,

} UtilUart_CRCMode_E;


typedef struct
{
//���տ�ʼ�ı�־
    bool receivStart;

//��ͷУ���־λ�� У��ʧ�ܸñ�־��Ϊtrue  ͬʱ���ֽ��жϸĳɵ��ֽ� ֱ��һ���������л���
    bool     lastCrcCheckOK ;
//��ǰ���յ��ĳ��ȼ���
    uint16 receiveDataCount;
//�����ֽڽ���ʱ������
    uint8_t receiveDataPartial;

//���ݽ���
    uint8_t *receiveData;

//�Ƿ�ʹ�ܽ�����
    bool receiveITEN;

} UtilUart_Handler_T_Private;
typedef struct
{

//�󶨵Ĵ���
//id��debug�õ�
    uint8_t id;
    UART_HandleTypeDef  *huart;

//���ճ��� ����ģʽ����Ч
    uint16 receiveLength;

//����ͷ��CRCģʽ��һ���,ֻ��NONEʱ��У���ͷ��
    uint8   rxHeadCode ;
//����ģʽ
    UtilUart_ReceiveMode_E receiveMode;
//����CRCģʽ
    UtilUart_CRCMode_E receiveCRCMode;
//����ʱCRCģʽ
    UtilUart_CRCMode_E sendCRCMode;

//������ɱ�־λֻ��
    volatile    bool   receiveFinished;

    UtilUart_Handler_T_Private privateParameter;


} UtilUart_Handler_T;




#ifdef UTIL_UART1_EN
extern uint8   utilUart_1_RxBuff[];
extern UtilUart_Handler_T  utilUart_1_Handle;
extern UART_HandleTypeDef  huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
#endif


#ifdef UTIL_UART2_EN
extern uint8   utilUart_2_RxBuff[];
extern UtilUart_Handler_T  utilUart_2_Handle;
extern UART_HandleTypeDef  huart2

#endif
#ifdef UTIL_UART3_EN
extern uint8   utilUart_3_RxBuff[];
extern UtilUart_Handler_T  utilUart_3_Handle;
extern UART_HandleTypeDef  huart3;

#endif

#ifdef UTIL_UART4_EN
extern uint8   utilUart_4_RxBuff[];
extern UtilUart_Handler_T utilUart_4_Handle;
extern UART_HandleTypeDef  huart4;
#endif



#ifdef UTIL_UART5_EN
extern uint8   utilUart_5_RxBuff[];
extern UtilUart_Handler_T utilUart_5_Handle;
extern UART_HandleTypeDef  huart5
#endif

#ifdef UTIL_UART6_EN
extern UtilUart_Handler_T utilUart_6_Handle;
extern UART_HandleTypeDef  huart6;
extern uint8   utilUart_6_RxBuff[];
#endif



#ifdef UTIL_UART7_EN
extern uint8   utilUart_7_RxBuff[];
extern UtilUart_Handler_T utilUart_7_Handle;
extern UART_HandleTypeDef  huart7;
#endif


#ifdef UTIL_UART8_EN
extern uint8   utilUart_8_RxBuff[];
extern UtilUart_Handler_T utilUart_8_Handle;
extern UART_HandleTypeDef  huart8;
#endif



//��ʼ�� ��ϵͳ�Զ�����
void utilUart_Init(void);

//ʹ�ù����ж�̬���޸����ò�����Ҫ �ȹرս��գ��޸Ĳ���������utilUart_Configuration���ٴ򿪽��գ�
void  utilUart_Configuration(UtilUart_Handler_T *utilUartHandler  );

//ʹ�ܿ����ж�ʱ ���ڴ����ж������
void utilUart_RxIdleCallback(UART_HandleTypeDef *huart);
//������ͨ���ͺ���
void utilUart_sendDat(UtilUart_Handler_T *utilUartHandler, uint8 *dat,uint16 size);

//ʹ�ܽ���
void utilUart_Receive_IT (UtilUart_Handler_T *utilUart_Handle);
//ʹ�ܽ���
void utilUart_AbortReceive_IT (UtilUart_Handler_T *utilUart_Handle);

#endif

#endif









