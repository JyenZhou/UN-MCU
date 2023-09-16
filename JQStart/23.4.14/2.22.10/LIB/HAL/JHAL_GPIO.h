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
        /*!< 输入模式*/
        JHAL_IO_IN=0,
        //中断输入下降沿触发 Falling Edge Trigger
        JHAL_IO_IN_EXTI_FET,
        //Rising Edge Trigger
        JHAL_IO_IN_EXTI_RET,
        /*!< 推挽输出模式*/
        JHAL_IO_PP,
        /*!开漏输出模式 上拉/下拉*/
        JHAL_IO_OD,
        JHAL_IO_OD_UP,
        JHAL_IO_OD_DOWM,

    } JHAL_IO_MODE;



//读取IO状态  true高电平 false低电平
    bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin);
//设置IO状态  true高阻态/电平 false低电平
    void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level);
//模式设置
    void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,JHAL_IO_MODE mode);
//IO翻转  输出模式下生效
    void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin);

    void JHAL_gpioInterruptCallback(void);
    void JHAL_gpio4IRKeyInterruptCallback(void);


#ifdef CplusPlus
}
#endif

#endif








