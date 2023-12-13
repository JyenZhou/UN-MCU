/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADCĬ��ͨ��HAL����
  ʹ��ADC0 �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_GPIO__H__
#define __JHAL_GPIO__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        JHAL_IOA=0,
        JHAL_IOB,
        JHAL_IOC,
        JHAL_IOD,
        JHAL_IOE,
        JHAL_IOF,
        JHAL_IOG,
        JHAL_IOH,
        JHAL_IOI,
    } JHAL_IO_Port;

    typedef enum
    {
        /*!< ����ģʽ*/
        JHAL_IO_IN=0,
        //�ж������½��ش��� Falling Edge Trigger
        JHAL_IO_IN_EXTI_FET,
        //Rising Edge Trigger
        JHAL_IO_IN_EXTI_RET,
        /*!< �������ģʽ*/
        JHAL_IO_PP,
        /*!��©���ģʽ ����/����*/
        JHAL_IO_OD,
        JHAL_IO_OD_UP,
        JHAL_IO_OD_DOWM,

    } JHAL_IO_MODE;



//��ȡIO״̬  true�ߵ�ƽ false�͵�ƽ
    bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin);
//����IO״̬  true����̬/��ƽ false�͵�ƽ
    void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level);
//ģʽ����
    void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,JHAL_IO_MODE mode);
//IO��ת  ���ģʽ����Ч
    void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin);

    void JHAL_gpioInterruptCallback(void);
    void JHAL_gpio4IRKeyInterruptCallback(void);


#ifdef CplusPlus
}
#endif

#endif







