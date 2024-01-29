#include "wwdg.h"

/*���Ź�timeoutʱ��Ϊ43.7 ms*/

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Wwdg_ON(uint8_t ucTcnt, uint8_t ucWcnt)
{
    // WWDG configuration
    // Enable WWDG clock
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_WWDG, ENABLE);

    //WWDG_Prescaler_x,x?1,2,4,8,WWDG counter clock = (PCLK1/4096)/x
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);

    // Enable WWDG and set counter value from 0x40  to 0x7F
    // Enable WWDG and set counter value to 127, WWDG timeout = ~683 us * 64 = 43.7 ms
    //In this case the refresh window is: ~683 * (127-80)= 32.1ms < refresh window < ~683 * 64 = 43.7ms
    WWDG_Enable(ucTcnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Wwdg_irq_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_WWDG, ENABLE);
    WWDG_SetWindowValue(0x70);
    WWDG_EnableIT();
    //Here is the configuration. The parameter value must be less than the
    //window value, otherwise the reset will occur immediately.
    WWDG_Enable(0x66);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WWDG_IRQHandler (void)
{
    if(WWDG_GetFlagStatus()) {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);
    }
}
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload)
{
    //Start the internal low-speed clock and wait for the clock to be ready
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		
    //Setting Clock Pre-division Frequency
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);		
		
    IWDG_SetPrescaler(IWDG_Prescaler);

    //Setting overload register values
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    //Loading and Enabling Counter
    IWDG_ReloadCounter();
    IWDG_Enable();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  WWDG test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
//void WWDGtest(void)
//{
//    //ENABLE WWDG
//    Wwdg_ON(0x7e, 0x7f);
//    while(1) {
//        //No reset program goes into the dead cycle, anyway, the system
//        //has been printing serial data.
//        WWDG_SetCounter(0x7e);
//    }
//}

uint32_t  wwdg_task(void){
_SS
  while(UERROR){
		
//		WWDG_SetCounter(0x7e);	
  		IWDG_ReloadCounter();
		WaitX(40); 
  }
_EE
}