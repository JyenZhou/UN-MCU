#ifndef __HARDWARE__H
#define __HARDWARE__H

#include <drivers/beep/adi_beep.h>
#include <drivers/pwr/adi_pwr.h>
#include <drivers/gpio/adi_gpio.h>
#include <drivers/i2c/adi_i2c.h>
#include <drivers/rtc/adi_rtc.h>
#include <drivers/spi/adi_spi.h>
#include <drivers/tmr/adi_tmr.h>
#include <drivers/uart/adi_uart.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  ADI_HAL_OK       = 0x00,
  ADI_HAL_ERROR    = 0x01,
  ADI_HAL_BUSY     = 0x02,
  ADI_HAL_TIMEOUT  = 0x03
} ADI_HAL_STATUS_t;

typedef enum {
  BAUD_19200 = 0x00,
  BAUD_57600,
  BAUD_115200,
  BAUD_230400,
  BAUD_460800,
  BAUD_921600,
  BAUD_1000000,
  BAUD_1500000,
  BAUD_2000000,
  BAUD_3000000,
} UART_BAUD_OPTIONS;

/* Private define ------------------------------------------------------------*/
#define DEFAULT_UART_BAUD       BAUD_460800
#define ADPD_CFG_SPI_DEV_CS     ADI_SPI_CS0
#define ADPD_CFG_SPI_DEV_BITRATE  1000000

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/*--------------UART--------------*/
void UART_Init(void);
int  uart_printf( const char * format, ... );

/*--------------CLOCK-------------*/
void Clock_Init(void);
uint32_t MCU_HAL_GetTick();
void MCU_HAL_Delay();

/*--------------I2C---------------*/
ADI_I2C_RESULT I2C_Init(void);
ADI_HAL_STATUS_t ADPD_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize);
ADI_HAL_STATUS_t ADPD_I2C_Transmit(uint8_t *pData, uint16_t Size);

/*--------------SPI---------------*/
void SPI_Init(void);
ADI_HAL_STATUS_t ADPD_SPI_Transmit(uint8_t *pTxData, uint16_t Size);
ADI_HAL_STATUS_t ADPD_SPI_Receive(uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize, uint16_t RxSize);

/*--------------UART--------------*/
void GPIO_Init(void);

/*--------------BEEP--------------*/
ADI_BEEP_RESULT BEEP_Init(void);
ADI_BEEP_RESULT BEEP_De_Init(void);
void beepCallback(void *pCBUnused, uint32_t Event, void *pvUnused);
ADI_BEEP_RESULT PlayOne(void);
ADI_BEEP_RESULT PlayAlarm(void);
ADI_BEEP_RESULT PlayTune(void);
#endif