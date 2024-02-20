
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

