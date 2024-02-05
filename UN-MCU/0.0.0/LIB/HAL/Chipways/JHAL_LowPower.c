#include "../JHAL_LowPower.h"
#include "xl_pmc.h"
#include <xl_osc.h>
void  __JHAL_Enter_Stop(void)
{
    //close IO
//	GPIOA->PIDR = 0xFFFFFFFF;//0xFF00FFFF
//	GPIOC->PIDR = 0xFFFFFFFF;
//	GPIOB->PIDR |= 0xFFFCFFFF;

    //close wodg
    SIM->SCGC &=  ~SIM_SCGC_WDG_MASK;
    //FLASH sleep when enter stop mode
//	FLASH_SleepInStopCmd(ENABLE);
    //Disable LVD Detect
    PMC->SPMSC &= (~0x1C);//(~0x8);
    //FLL is bypass and IRC is system clock now

    //Config enter deepsleep or sleep
    NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP, ENABLE);

//	SIM->SCGC &= SIM_SCGC_RTC_MASK;
//	SIM->SCGC1 = 0;
    SIM->LP |= 0x55aa3008;
    ICS_FllLPModeStopEnableCmd(ENABLE);;
    ICS_ClocksSelect(ICS_CLOCKS_IRC);
    ICS_IRCSelect(ICS_IRC_37o5K);
    ICS_IREFStopEnableCmd(DISABLE);

    if((OSC->CR & OSC_CR_OSCEN_MASK) == OSC_CR_OSCEN_MASK)
    {
        OSC->CR &= ~OSC_CR_OSCEN_MASK;
        OSC->CR |= 0x16;
        OSC->CR |= OSC_CR_OSCEN_MASK;

        while((OSC->CR & OSC_CR_OSCINIT_MASK) != OSC_CR_OSCINIT_MASK);

        OSC->CR &= ~OSC_CR_OSCEN_MASK;
    }
    __asm("WFI");
    SystemSetFEE(8000000, ICS_FLLFACTOR_1920, ICS_RDIV_1or32,ICS_RDIV2_5);

}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: Stop模式 内存数据保留，基本外设开启

  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

void JHAL_lowPowerByStopEnter(void)
{
    __JHAL_Enter_Stop();
}

