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
                ����Լ���װ��Ƭ������Ȱ���ģ������.h
    						��ifdef������Ϊ�˵͵��Ż��ȼ��в���ӹ�������ͷ�ļ�
    						                    *
      *                                                                 *
       *                                                              *
    *********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/


    /*ͨ����������API*/
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
    /*BEGIN------������STM32-----BEGIN*/
#ifdef USE_HAL_DRIVER





//���Լ���
#include "BISP/BIP/STM32/Delay/Util_delay.h"

#include "BISP/Library/My/NumberConverter/NumberConverter.h"

#include "BISP/Library/ThirdParty/Cjson/cjson.h"
#include "BISP/Library/ThirdParty/Malloc/malloc.h"
#include  "BISP/Library/My/Kalman/kalman.h"
#include "BISP/Library/My/CRC/MyCRC.h"

//�ҵ��������е�
#include "BISP/Library/My/InternetOfThings/stateanalysis.h"

#include "BISP/BIP/STM32/Flash/Util_Flash.h"







    /*���ڰ�����*/
#if ((defined HAL_USART_MODULE_ENABLED ) || (defined HAL_UART_MODULE_ENABLED))
#include "BISP/BIP/STM32/Uart/Util_Uart.h"
#endif


    /*ADC������*/
#if defined HAL_ADC_MODULE_ENABLED
#include "BISP/BIP/STM32/ADC/Util_ADC.h"
#endif
#if defined HAL_CAN_MODULE_ENABLED
#include "BISP/BIP/STM32/CAN/Util_ADC.h"
#endif
#include "BISP/BIP/Util_ADC.h"

    /*ʹ�ܿ��Ź�*/
#ifdef HAL_WWDG_MODULE_ENABLED
#include "BISP/BIP/STM32/IWWDGUtil/Util_IWWDG.h"
#endif


#if defined HAL_TIM_MODULE_ENABLED
#include "BISP/BIP/STM32/Timer/Util_Timer.h"
#endif




#endif


    /*END------������STM32-----END*/




    /*BEGIN------������HC32-----BEGIN*/



    /*ADC����ʹ��*/
#ifdef HAL_ADC_UTIL_ENABLED

#include "BISP/BIP/HC32/ADCUtil/Util_adc.h"

#endif


    /*ADC����ʹ��*/
#ifdef HAL_FLASH_UTIL_ENABLED
#include "BISP/BIP/HC32/FlashUtil/Util_flash.h"
#endif

    /*Flash����ʹ��*/
#ifdef HAL_UART_UTIL_ENABLED

#include "BISP/BIP/HC32/UartUtil/Util_uart.h"
#endif


    /*END------������HC32-----END*/







#ifdef CplusPlus
}
#endif

#endif


