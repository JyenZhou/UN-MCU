#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];

/* Private function prototypes -----------------------------------------------*/

/**
*  @brief  GPIO Configuration.
*  @param  None
*  @retval None
*/
uint32_t GPIO_Configuration(void) {
#ifdef ADPD_SPI
  /* SCL=1, SDA=1, to select SPI communication to ADPD188*/
  adi_gpio_OutputEnable(ADI_GPIO_PORT0, ADI_GPIO_PIN_5|ADI_GPIO_PIN_4, true);
  adi_gpio_SetHigh(ADI_GPIO_PORT0, ADI_GPIO_PIN_5|ADI_GPIO_PIN_4);
  
#else
  /* CS=1, MOSI=0, CLK=0, to select I2C communication to ADPD188*/
  adi_gpio_OutputEnable(ADI_GPIO_PORT1, ADI_GPIO_PIN_6|ADI_GPIO_PIN_7|ADI_GPIO_PIN_9, true);
  adi_gpio_SetHigh(ADI_GPIO_PORT1, ADI_GPIO_PIN_9);
  adi_gpio_SetLow(ADI_GPIO_PORT1, ADI_GPIO_PIN_7);
  adi_gpio_SetLow(ADI_GPIO_PORT1, ADI_GPIO_PIN_6);
#endif
  
  /* enable LED power supply*/
  adi_gpio_OutputEnable(ADI_GPIO_PORT1, ADI_GPIO_PIN_5, true);
  adi_gpio_SetHigh(ADI_GPIO_PORT1, ADI_GPIO_PIN_5); 
  
  return 0;
}

/**
*  @brief  GPIO Initialization.
*  @param  None
*  @retval None
*/
void GPIO_Init(void) {
  /* Initialize GPIO service with number of callbacks required */
    if (adi_gpio_Init(
            gpioMemory,
            ADI_GPIO_MEMORY_SIZE)
             != ADI_GPIO_SUCCESS) {
        return;
    }
    GPIO_Configuration();
}

