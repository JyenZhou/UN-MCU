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

OS_WEAK void     JHAL_timerInterruptCallback (u8 realityId)

{
  

}

