#include "JFML.h"

#ifdef USE_HAL_DRIVER
#include  "HAL/STM32/stm32f1xx_hal_wwdg.c"
#endif










	OS_WEAK void     JHAL_delayOsUs (u32 ms) 

{
	JHAL_delayUs(ms);
}


	OS_WEAK void     JHAL_delayOsMs (u32 ms) 

{
	JHAL_delayMs(ms);
}


	OS_WEAK void     JHAL_delayOs (JHAL_TimeUnits timeUntis,u16 value) 

{
	JHAL_delayOs(timeUntis,value);
}
 
	OS_WEAK void     JHAL_timerInterruptCallBack (u8 realityId) 
	 
{
   


}



















