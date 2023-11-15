#ifndef __JHAL_LIN__H__
#define __JHAL_LIN__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#include "JHAL_Uart.h"


    typedef enum
    {
        JHAL_LIN_FrameClass_CLASSIC=0x01,
        JHAL_LIN_FrameClass_ENHANCED=0x02
    } JHAL_LIN_FrameClass;


    typedef struct
    {

        u8 id ;
//�Ƿ�ֻ��֡ͷ ����ʱ�ж� ������Ч
        bool isOnlyHead:1;
//  JHAL_LIN_FrameClass �������ݹ���ʱ�������У�� ֱ�ӻ��ϼ���  ע��Ϊ�˷�������ÿ���ϵ����ͨһ��Э���Ͳ���֧�ֶ�Э��
        //����ͷģʽ�޷��Զ�����
        u8 frameclass;
        u8 data[8];
//���ݳ���<=8
        u8 length;
    } JHAL_LinBox;

    typedef struct
    {

//data ��Ч  length��֡ͷģʽ����Ч   JHAL_LIN_FrameClass�������У��ʱֱ�ӻ���
        JHAL_LinBox *boxs;
//����Ϊ0��������ID  ��������ֻ���ܸ�ID
        u8 number;
    }
    JHAL_LinFilter;

    typedef void         (*JHAL_LinCallback)(bool isMasterMode,JHAL_LinBox box );


//Lin�Ĵ��䲽��
    typedef enum
    {
        __JHAL_LIN_Start=0,
        __JHAL_LIN_Sys,
        __JHAL_LIN_Head,
        __JHAL_LIN_PID,
        __JHAL_LIN_Data,
        __JHAL_LIN_Check,
        __JHAL_LIN_Finish,
    } __JHAL_LIN_Step;

    typedef struct
    {
        JHAL_LinBox *box;
//�Ƿ�ʹ��
        volatile    bool enable:1;
//����
        u8 count;
//��ǰ����Ĳ���
//����ģʽ
        bool isMasterMode;
        volatile   __JHAL_LIN_Step step;
    }  __JHAL_Lin_RxTxInfo;

    typedef struct
    {
        JHAL_LinFilter filter;
//��֡��������жϻص��� */
        JHAL_LinCallback itCallback;


    } JHAL_LIN_RXConfig;


    typedef struct {
        JHAL_UART uart ;

    } __JHAL_LIN_therInfo;


    typedef struct {
        u8 dev:3;
        uint32_t baudRate;
        JHAL_LIN_RXConfig rxConfig;

        __JHAL_Lin_RxTxInfo __linRxInfo;
        __JHAL_Lin_RxTxInfo __linTxInfo;
        JHAL_LinBox __txBox;
        __JHAL_LIN_therInfo __info;


    } JHAL_LIN;



    /*��֡�жϷ��ͣ� ����false������һ�黹δ�����ꣻ  masterMode������ģʽ����:  ��������ͷģʽ���յ������ڻص��� */
    bool JHAL_linSendData4IT(JHAL_LIN *lin,bool isMasterMode,JHAL_LinBox box );
//��ȡ�жϷ���״̬ true�����ѿ���
    bool JHAL_linGetStatus4SendData4IT(JHAL_LIN *lin);


    bool JHAL_linOpen(JHAL_LIN *lin);

    bool  JHAL_linClose(JHAL_LIN *lin);

//�����жϵ�֡ʹ�� ������Ϣ��JHAL_linInit��config��ָ��
    void JHAL_linEnableReceiveIT (JHAL_LIN *lin);
//����ʧ�ܽ���
    void  JHAL_linAbortReceiveIT (JHAL_LIN *lin);




#ifdef CplusPlus
}
#endif

#endif








