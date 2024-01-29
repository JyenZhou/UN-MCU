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
        JHAL_PWM_750HZ=0,
        JHAL_PWM_4HZ,
    } JHAL_PWM_Frequency;

    //JHAL_DEV_INIT
    typedef struct {
        bool isOpen:1;

    } __JHAL_PWM_OtherInfo;

    typedef struct {
        //实际上的通道
        u8  channel;
        //初始时候占比值
        u16 initialValue;
        //一般就是定时器重装载值
        u16 maxValue;
        JHAL_Align align;

    } JHAL_PWM_Channel;


    /*JHAL_PWM：使用的PWM 映射的引脚等信息见c中  prescaler:预分频倍数 对应时钟见c中   InitialValue：初始值 maxValue：最大值
    注意:使用过程中设置的值不能大于初始化给的最大值
    */
    typedef struct {


        u8 id:3;
        u8 channelsNumber:4;
        JHAL_PWM_Channel *channels;
        //这里是的通道实际值

        JHAL_PWM_Frequency frequency;
        __JHAL_PWM_OtherInfo __info;
        void*  dev;

    } JHAL_PWM;



    bool JHAL_pwmOpen(JHAL_PWM *pwm );
    bool  JHAL_pwmClose(JHAL_PWM *pwm);
//这里传的是通道的索引，实际值在JHAL_PWM中定义
    void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value);

#ifdef CplusPlus
}
#endif

#endif








