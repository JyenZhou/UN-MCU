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
        JHAL_CANBaudRate125K=0,
        JHAL_CANBaudRate250K,
        JHAL_CANBaudRate500K,
        JHAL_CANBaudRate1M,
    } JHAL_CANBaudRate;

    typedef struct {
        bool isOpen:1;
        bool isRxFinsh:1;


    } __JHAL_canOtherInfo;



    typedef struct
    {
        u32 id;
        u8 *data;
        u16 length;
//  trueΪ��չ֡ falseΪ����֡
        bool isExtendFrame:1;
//trueΪԶ��֡
        bool isRemoteFrame:1;
    } JHAL_CANBox;

    /*    canBaudRat:������
        filterRxIDList��ֻ����ָ������֡ID���������ݵ�����
        filterRxIDNumber�� ����֡�ĳ���*/
    typedef struct {
        u8 id:3;
        JHAL_CANBaudRate baudRate;
        u32  *filterRxIDList;
        u8 filterRxIDNumber;
        JHAL_CANBox rxDataBuff;

        __JHAL_canOtherInfo __info;
    } JHAL_CAN;

    /*CAN���� ֧�ֶ�֡ ע��  JHAL_CANBox��dataʵ����Ҫ����������һ��
    ʹ��CAN0
    length��Ҫ���͵����ݳ��� ����8ʱ�Զ���֡����

    sendOneFrameCallback���������1֡���ݺ�Ļص�  ��֡ʱ�����ڸú����������ʱ/ι���Ȳ���  ����Ҫ�ص�ʱ��NULL


    void   canSendOneFrameCallback()
    {
    delayMsAdnFeedDog(5);
    }
    */
    bool JHAL_canSendDatas(JHAL_CAN *can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback );


    /* ʹ��Ĭ��������Ҫע��ʹ�õ�����CAN�������Ƿ���Ҫ��ӳ�� ��ʼ������ true ��ʼ��ʹ��  false����ʼ����ʹ��

    */
    bool JHAL_canOpen( JHAL_CAN *can);
    bool  JHAL_canClose(JHAL_CAN *can);
    /*���ж��н���һ֡�󻺴�����ֱ���û����ú����� ��Ҫ�������û�������ѯ
     * ����ֵtrueֵ��Ч  falseֵ��Ч
      canBox  ͨ��ָ�����������ֵ
    ע�⣺1.�û�����ʱ��ûʹ��ʼ�ղ������������ֵ 2.����������֡ ����Ӧ�ߵĳ��� ��Ҫ���дӽ����жϴ���*/
    u16 JHAL_canRxFinsh(JHAL_CAN *can);



#ifdef CplusPlus
}
#endif

#endif







