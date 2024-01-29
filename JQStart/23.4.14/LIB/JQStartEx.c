#include "JFML.h"





#ifdef USE_HAL_DRIVER
#include  "HAL/STM32/stm32f1xx_hal_wwdg.c"
#endif







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






OS_WEAK     void  JHAL_delayOpen(JHAL_Delay config)

{

}

OS_WEAK     void   JHAL_delayUs (u32 us)

{

}

OS_WEAK     void   JHAL_delayMs (u32 ms)

{
    for(u8 i=0; i<ms; i++)
    {
        JHAL_delayUs(1000);
    }
}


OS_WEAK     void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,u32 mode)
{
}



OS_WEAK	    bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin) {
    return false;
}

OS_WEAK   void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)

{

}


OS_WEAK void JHAL_gpioInterruptCallback()
{

}

OS_WEAK void JHAL_gpio4IRKeyInterruptCallback()
{

}

 


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
 jsystemMs++;

}




















