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


#ifndef __JHAL_CAN__H__
#define __JHAL_CAN__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif


    typedef enum
    {
        JHAL_CAN0=0,
        JHAL_CAN_Number
    } JHAL_CAN;



    typedef enum
    {
        JHAL_CANBaudRate125K=0,
        JHAL_CANBaudRate250K,
        JHAL_CANBaudRate500K,
        JHAL_CANBaudRate1M,
    } JHAL_CANBaudRate;


    typedef struct
    {
        u16 id;
        u8 *data;
        u16 length;
//  trueΪ��չ֡ falseΪ����֡
        bool isExtendFrame;
//trueΪԶ��֡
        bool isRemoteFrame;
    } JHAL_CANBox;

    /*    canBaudRat:������
        filterRxIDList��ֻ����ָ������֡ID���������ݵ�����
        filterRxIDNumber�� ����֡�ĳ���*/
    typedef struct {
        JHAL_CANBaudRate canBaudRate;
        u32  *filterRxIDList;
        u8 filterRxIDNumber;
    } JHAL_CANConfig;

    /*CAN���� ֧�ֶ�֡ ע��  JHAL_CANBox��dataʵ����Ҫ����������һ��
    ʹ��CAN0
    length��Ҫ���͵����ݳ��� ����8ʱ�Զ���֡����

    sendOneFrameCallback���������1֡���ݺ�Ļص�  ��֡ʱ�����ڸú����������ʱ/ι���Ȳ���  ����Ҫ�ص�ʱ��NULL


    void   canSendOneFrameCallback()
    {
    delayMsAdnFeedDog(5);
    }
    */
    bool JHAL_canSendDatas(JHAL_CAN can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback );


    /* ʹ��Ĭ��������Ҫע��ʹ�õ�����CAN�������Ƿ���Ҫ��ӳ�� ��ʼ������ true ��ʼ��ʹ��  false����ʼ����ʹ��

    */
    void JHAL_canInit(JHAL_CAN can,JHAL_CANConfig *config);
    void  JHAL_canDeInit(JHAL_CAN can);
    /*���ж��н���һ֡�󻺴�����ֱ���û����ú����� ��Ҫ�������û�������ѯ
     * ����ֵtrueֵ��Ч  falseֵ��Ч
      canBox  ͨ��ָ�����������ֵ
    ע�⣺1.�û�����ʱ��ûʹ��ʼ�ղ������������ֵ 2.����������֡ ����Ӧ�ߵĳ��� ��Ҫ���дӽ����жϴ���*/
    bool JHAL_canGetRxBuff(JHAL_CAN can,JHAL_CANBox* canBox);


#ifdef CplusPlus
}
#endif

#endif








