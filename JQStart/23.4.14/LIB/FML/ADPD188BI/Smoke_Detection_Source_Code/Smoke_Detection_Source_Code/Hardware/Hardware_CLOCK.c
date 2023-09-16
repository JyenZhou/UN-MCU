#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief       Initialize the system clock
 * @param[in]   none
 * @return      none 
 */  
void Clock_Init(void)
{   
  adi_pwr_Init();  

  adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1u);  // HCLK divider

  adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1u);  //PCLK divider
  
  /* Enable HFXTAL */
  ADI_CLOCK_SOURCE_STATUS clk_status; 
  adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_HFXTAL, true);
  adi_pwr_GetClockStatus(ADI_CLOCK_SOURCE_HFXTAL, &clk_status);
  while (clk_status !=  ADI_CLOCK_SOURCE_ENABLED_STABLE) 
  {
    adi_pwr_GetClockStatus(ADI_CLOCK_SOURCE_HFXTAL, &clk_status);
  } 
  
  adi_pwr_SetRootClockMux(ADI_CLOCK_MUX_ROOT_HFXTAL); // Set Root Clock HFXTAL based
}

void MCU_HAL_Delay(uint32_t n)
{
  uint32_t i;
  for(i=0;i<n*100;i++);
}