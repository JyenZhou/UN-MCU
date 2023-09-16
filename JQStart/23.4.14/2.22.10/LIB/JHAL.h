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
    /*BEGIN------适用于STM32-----BEGIN*/
#ifdef USE_HAL_DRIVER





//我自己的
#include "BISP/BIP/STM32/Delay/Util_delay.h"

#include "BISP/Library/My/NumberConverter/NumberConverter.h"

#include "BISP/Library/ThirdParty/Cjson/cjson.h"
#include "BISP/Library/ThirdParty/Malloc/malloc.h"
#include  "BISP/Library/My/Kalman/kalman.h"
#include "BISP/Library/My/CRC/MyCRC.h"

//我的物联网中的
#include "BISP/Library/My/InternetOfThings/stateanalysis.h"

#include "BISP/BIP/STM32/Flash/Util_Flash.h"







    /*串口帮助类*/
#if ((defined HAL_USART_MODULE_ENABLED ) || (defined HAL_UART_MODULE_ENABLED))
#include "BISP/BIP/STM32/Uart/Util_Uart.h"
#endif


    /*ADC帮助类*/
#if defined HAL_ADC_MODULE_ENABLED
#include "BISP/BIP/STM32/ADC/Util_ADC.h"
#endif
#if defined HAL_CAN_MODULE_ENABLED
#include "BISP/BIP/STM32/CAN/Util_ADC.h"
#endif
#include "BISP/BIP/Util_ADC.h"

    /*使能看门狗*/
#ifdef HAL_WWDG_MODULE_ENABLED
#include "BISP/BIP/STM32/IWWDGUtil/Util_IWWDG.h"
#endif


#if defined HAL_TIM_MODULE_ENABLED
#include "BISP/BIP/STM32/Timer/Util_Timer.h"
#endif




#endif


    /*END------适用于STM32-----END*/




    /*BEGIN------适用于HC32-----BEGIN*/



    /*ADC工具使能*/
#ifdef HAL_ADC_UTIL_ENABLED

#include "BISP/BIP/HC32/ADCUtil/Util_adc.h"

#endif


    /*ADC工具使能*/
#ifdef HAL_FLASH_UTIL_ENABLED
#include "BISP/BIP/HC32/FlashUtil/Util_flash.h"
#endif

    /*Flash工具使能*/
#ifdef HAL_UART_UTIL_ENABLED

#include "BISP/BIP/HC32/UartUtil/Util_uart.h"
#endif


    /*END------适用于HC32-----END*/







#ifdef CplusPlus
}
#endif

#endif


