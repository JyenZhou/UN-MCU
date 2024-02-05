#include "../JHAL_Delay.h"

 void  JHAL_DelayInit( )
{
}
 
 void  JHAL_DelayMs(u32 ms)
 {
	    mdelay(ms);
 }

 

 void  JHAL_DelayUs(u32 us)
 {
	    udelay(us);
 }










