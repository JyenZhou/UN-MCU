/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		PWM默认通用HAL驱动

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
    /*JHAL_PWM：使用的PWM 映射的引脚等信息见c中  prescaler:预分频倍数 对应时钟见c中   InitialValue：初始值 maxValue：最大值
    注意:使用过程中设置的值不能大于初始化给的最大值
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








