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
        JHAL_PWM_750HZ=0,
        JHAL_PWM_4HZ,

    } JHAL_PWM_Frequency;


    typedef struct {
        bool isOpen:1;

    } __JHAL_PWM_OtherInfo;

    /*JHAL_PWM��ʹ�õ�PWM ӳ������ŵ���Ϣ��c��  prescaler:Ԥ��Ƶ���� ��Ӧʱ�Ӽ�c��   InitialValue����ʼֵ maxValue�����ֵ
    ע��:ʹ�ù��������õ�ֵ���ܴ��ڳ�ʼ���������ֵ
    */
    typedef struct {
        u8 dev:3;
        JHAL_PWM_Frequency frequency;
        u16 initialValue;
        u16 maxValue;
        JHAL_Align align;
        __JHAL_PWM_OtherInfo __info;

    } JHAL_PWM;



    bool JHAL_pwmOpen(JHAL_PWM *pwm );
    bool  JHAL_pwmClose(JHAL_PWM *pwm);

    void JHAL_pwmSetValue(JHAL_PWM *pwm,u16 value);

#ifdef CplusPlus
}
#endif

#endif








