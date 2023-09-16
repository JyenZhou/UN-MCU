#ifndef  __JHAL_INCLUDES__H__
#define   __JHAL_INCLUDES__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif
    /*****************************************************************************************************
       *                                                                *
     *                                                                   *
    *                                                                     *
                存放自己封装的片上外设等板载模块整合.h
    						用ifdef屏蔽是为了低的优化等级中不添加过多无用头文件
    						                    *
      *                                                                 *
       *                                                              *
    *********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/


    /*通用外设驱动API*/
#include  "HAL/JHAL_Delay.h"
#include  "HAL/JHAL_ADC.h"
#include  "HAL/JHAL_CAN.h"
#include  "HAL/JHAL_Flash.h"
#include  "HAL/JHAL_LowPower.h"
#include  "HAL/JHAL_RTC.h"
#include  "HAL/JHAL_Timer.h"
#include  "HAL/JHAL_GPIO.h"
#include  "HAL/JHAL_SoftwareIIC.h"
#include  "HAL/JHAL_PWM.h"
#include  "HAL/JHAL_BootLoader.h"
#include "HAL/JHAL_LIN.h"
#include "HAL/JHAL_Uart.h"
#include "HAL/JHAL_WDG.h"
 


 






#ifdef CplusPlus
}
#endif

#endif


