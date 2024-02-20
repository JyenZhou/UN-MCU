#include "JFML.h"





#ifdef USE_HAL_DRIVER
#include  "HAL/STM32/stm32f1xx_hal_wwdg.c"
#endif





#include "HAL/JHAL_Timer.c"
#include "HAL/JHAL_Delay.c"
#include "HAL/JHAL_GPIO.c"
#include "HAL/JHAL_Uart.c"




OS_WEAK u32 JHAL_uidGetHigh()
{
    return 0;
}
OS_WEAK u32 JHAL_uidGetMiddle()
{
    return 0;
}
OS_WEAK u32 JHAL_uidGetLow()
{
    return 0;
}


 OS_WEAK void JHAL_systemReset()
 {
	 
 }

























