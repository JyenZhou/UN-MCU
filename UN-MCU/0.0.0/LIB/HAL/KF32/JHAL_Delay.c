#include "../JHAL_Delay.h"

 void  JHAL_delayInit()
 {
	 systick_delay_init();
 }

 void  JHAL_delayMs(u32 ms)
 {
	 systick_delay_ms(ms);
 }



 void  JHAL_delayUs(u32 us)
 {
	 systick_delay_us(us);
 }






