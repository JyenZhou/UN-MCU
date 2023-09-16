/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		PWMĬ��ͨ��HAL����

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_PWM__H__
#define __JHAL_PWM__H__
#include "JUTIL.h"
#include "JHAL_GPIO.h"
#ifdef __CplusPlus
extern "C" {
#endif
    typedef enum {
        JHAL_Align_Center=0,
        JHAL_Align_Left,
        JHAL_Align_Right,
    } JHAL_Align;

    typedef enum {
        JHAL_PWM_750HZ=0,
        JHAL_PWM_4HZ,

    } JHAL_PWM_Frequency;

    typedef enum {
        JHAL_PWM0=0,
        JHAL_PWM1,
        JHAL_PWM_Number
    } JHAL_PWM;
    /*JHAL_PWM��ʹ�õ�PWM ӳ������ŵ���Ϣ��c��  prescaler:Ԥ��Ƶ���� ��Ӧʱ�Ӽ�c��   InitialValue����ʼֵ maxValue�����ֵ
    ע��:ʹ�ù��������õ�ֵ���ܴ��ڳ�ʼ���������ֵ
    */
    typedef struct {
        JHAL_PWM_Frequency frequency;
        u16 initialValue;
        u16 maxValue;
        JHAL_Align align;

    } JHAL_PWMConfig;



    void JHAL_pwmInit(JHAL_PWM pwm,JHAL_PWMConfig *config );
    void  JHAL_pwmDeInit(JHAL_PWM pwm);

    void JHAL_pwmSetValue(JHAL_PWM pwm,u16 value);

#ifdef CplusPlus
}
#endif

#endif








