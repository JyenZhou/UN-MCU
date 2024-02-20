/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADC默认通用HAL驱动
  使用ADC0 若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_GPIO__H__
#define __JHAL_GPIO__H__

#ifdef __cplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        JHAL_IO_NONE=0,
        JHAL_IOA,
        JHAL_IOB,
        JHAL_IOC,
        JHAL_IOD,
        JHAL_IOE,
        JHAL_IOF,
        JHAL_IOG,
        JHAL_IOH,
        JHAL_IOI,
    } JHAL_IO_Port;

  typedef   struct
    {
        JHAL_IO_Port port;
        u8 pin;
    } JHAL_IO;

    typedef enum
    {
        JHAL_IOMODE_PullResistor_None=0x00000U,
        JHAL_IOMODE_PullResistor_Up=0x10000U,
        JHAL_IOMODE_PullResistor_Down=0x20000U,
        __JHAL_IOMODE_PullResistor_Mask=0xF0000U

    } JHAL_IOMODE_PullResistor;

    typedef enum
    {
        JHAL_IOMODE_IO_Input=0x0000U,
        JHAL_IOMODE_IO_OutputPushPull  =0x1000U,
        JHAL_IOMODE_IO_OutputDrain =0x2000U,
        __JHAL_IOMODE_IO_Mask=0xF000U
    } JHAL_IOMODE_IO;

    typedef enum
    {
        //不使能
        JHAL_IOMODE_EXTI_Trigger_High_None=0x000,
        //高电平、上升沿
        JHAL_IOMODE_EXTI_Trigger_High_Rising=0x100U,
        JHAL_IOMODE_EXTI_Trigger_Low_Falling=0x200U,
        JHAL_IOMODE_EXTI_Trigger_HightLow_Rising_Falling=0x300U,
        __JHAL_IOMODE_EXTI_Trigger_Mask=0xF00U
    } JHAL_IOMODE_EXTI_Trigger;

    typedef enum
    {
        //边沿触发
        JHAL_IOMODE_EXTI_TriggerMode_Edge=0x00U,
        //高低电平触发
        JHAL_IOMODE_EXTI_TriggerMode_Levle=0x10U,
        JHAL_IOMODE_EXTI_TriggerMode_All=0x20U,
        __JHAL_IOMODE_EXTI_TriggerMode_Mask=0xF0U
    } JHAL_IOMODE_EXTI_TriggerMode;


    typedef enum
    {
        JHAL_IOMODE_Speed_High=0U,
        JHAL_IOMODE_Speed_Low=0x01U,
        JHAL_IOMODE_Speed_Medium=0X02U,
        JHAL_IOMODE_Speed_VeryHigh=0x03U,
        __JHAL_IOMODE_Speed_Mask=0x0FU
    } JHAL_IOMODE_Speed;

//预设的下拉中断输入
#define JHAL_IOMODE_Preset_Exit_High_Rising  (JHAL_IOMODE_EXTI_Trigger_High_Rising|JHAL_IOMODE_PullResistor_Down)
//预设的上拉中断输入
#define JHAL_IOMODE_Preset_Exit_Low_Falling  (JHAL_IOMODE_EXTI_Trigger_Low_Falling|JHAL_IOMODE_PullResistor_Up)



//读取IO状态  true高电平 false低电平  pin从0开始
    bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin);
//设置IO状态  true高阻态/电平 false低电平
    void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level);
//通过MODE相关枚举|进来
    void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,u32 mode);

//IO翻转  输出模式下生效
    void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin);

    void JHAL_gpioInterruptCallback(void);
    void JHAL_gpio4IRKeyInterruptCallback(void);

//当使用自动生成工具时才会用的到
    JHAL_IO_Port JHAL_gpioPort2jport(void * port );
    u8  JHAL_gpioPin2jpin ( u16 pin );
		//适用于端口和pin定义在一起的那种
    void JHAL_gpioIo2jio(void * io,JHAL_IO_Port *port,u8 *pin);



 
  
 
	

#ifdef CplusPlus
}
#endif

#endif








