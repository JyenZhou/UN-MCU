/* Includes -----------------------------------------------------------------*/
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include "system_ADuCM4050.h"
#include "HwIfConfig.h"
#include "AdpdDrv.h"
#include <services/gpio/adi_gpio.h>
#include <services/xint/adi_xint.h>
#include <drivers/spi/adi_spi.h>
#include <drivers/i2c/adi_i2c.h>


/* Private typedef ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* ADPD device instance to be tested */
#define ADPD_DEV_NUM                     0
#ifndef __EV_COG
/* GPIO port and pin (GPIO16)/Wake1 Pin to which interrupt pin1 of ADPD is connected */
#define ADPD_INT1_GPIO_PORT              ADI_GPIO_PORT1
#define ADPD_INT1_GPIO_PIN               ADI_GPIO_PIN_0
#else
/* GPIO port and pin (GPIO15)/Wake0 Pin to which interrupt pin1 of ADPD is connected */
#define ADPD_INT1_GPIO_PORT              ADI_GPIO_PORT0
#define ADPD_INT1_GPIO_PIN               ADI_GPIO_PIN_15
#endif
/* AD5940 device instance to be tested */
#define AD5940_DEV_NUM                     0

/* GPIO port and pin (GPIO13)/Wake2 Pin to which interrupt pin of AD5940 is connected */
#define AD5940_INT_GPIO_PORT              ADI_GPIO_PORT0
#define AD5940_INT_GPIO_PIN               ADI_GPIO_PIN_13

/* ADUX1050 device instance to be tested */
#define ADUX1050_DEV_NUM                     0

/* GPIO port and pin (GPIO13)/Wake2 Pin to which interrupt pin of ADUX1050 is connected */
#define ADUX1050_INT_GPIO_PORT              ADI_GPIO_PORT0
#define ADUX1050_INT_GPIO_PIN               ADI_GPIO_PIN_12

/* Accelerometer device instance to be tested */
#define ADXL362_DEV_NUM                     0

/* GPIO port and pin (GPIO15)/Wake 0 to which interrupt pin1 of ADXL362 is connected */
#define ADXL362_INT1_GPIO_PORT              ADI_GPIO_PORT0
#define ADXL362_INT1_GPIO_PIN               ADI_GPIO_PIN_15

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Memory for GPIO callbacks */
uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];
/* Memory for XINT callbacks */
uint8_t xintMemory[ADI_XINT_MEMORY_SIZE];

/* Memory required for ADPD driver */
uint8_t AdpdMem[ADI_ADPD_MEMORY_SIZE];
/* Memory required for AD5940 driver */
uint8_t Ad5940Mem[ADI_AD5940_MEMORY_SIZE];
/* Memory required for ADUX1050 driver */
uint8_t Adux1050Mem[ADI_ADUX1050_MEMORY_SIZE];
/* Memory required for accelerometer driver */
uint8_t AccelMem[ADI_ADXL362_MEMORY_SIZE];

ADI_ADPD_HANDLE              hADPD;
ADI_AD5940_HANDLE            hAD5940;
ADI_ADUX1050_HANDLE              hADUX1050;
ADI_ADXL362_HANDLE           hADXL362;


/* Flag which indicates whether an interrupt is detected */
static volatile bool gbIntDetected = false;

/* ADPD instance information */
static ADI_ADPD_DEVICE    gADPDDev[] =
{
    {
      ADI_ADPD_STATE_NOT_OPENED,
      NULL
    }

    /* Add more instances here */
};

#ifndef __EV_COG
/* AD5940 instance information */
static ADI_AD5940_DEVICE    gAD5940Dev[] =
{
    {
      ADI_AD5940_STATE_NOT_OPENED,
      NULL
    }

    /* Add more instances here */
};

/* ADUX1050 instance information */
static ADI_ADUX1050_DEVICE    gADUX1050Dev[] =
{
    {
      ADI_ADUX1050_STATE_NOT_OPENED,
      NULL
    }

    /* Add more instances here */
};

/* ADXL362 instance information */
static ADI_ADXL362_DEVICE    gADXL362Dev[] = {
    {
      ADI_ADXL362_STATE_NOT_OPENED,
      NULL
    }

    /* Add more instances here */
};

/* Opens and sets up the ADPD driver */

/* Opens and sets up the ADPD driver */
void SetupAd5940(void);
/* Opens and sets up the ADUX1050 driver */
void SetupAdux1050(void);
/* Opens and sets up the accelerometer driver */
void AdxlISR();
void Ad5940ISR();
void Adux1050ISR();
void SetupAdxl(void);
#endif
void SetupAdpd(void);
/* Private function prototypes -----------------------------------------------*/
void AdpdISR();
/* Opens and sets up the ADPD driver */
void SetupAdpd(void);

/* Sets up the GPIO for ADPD interrupt 1 and 2 */
ADI_ADPD_RESULT          SetupAdpdGPIO(
    ADI_ADPD_INFO        *pDevInfo,
    ADI_ADPD_GPIO_INFO   *pGPIOInfo,
    ADI_CALLBACK         pfCallback
);

void HAL_GPIO_EXT_Callback(void *pCBParam, uint32_t Event, void *pArg);
#ifndef __EV_COG
/* Sets up the GPIO for Accelerometer interrupt 1 and 2 */
ADI_ADXL362_RESULT         SetupAdxlGPIO(
    ADI_ADXL362_INFO       *pDevInfo,
    ADI_ADXL362_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK           pfCallback
);



/* Sets up the GPIO for AD5940 interrupt */
ADI_AD5940_RESULT          SetupAd5940GPIO(
    ADI_AD5940_INFO        *pDevInfo,
    ADI_AD5940_GPIO_INFO   *pGPIOInfo,
    ADI_CALLBACK         pfCallback
);

/* Sets up the GPIO for ADUX1050 interrupt */
ADI_ADUX1050_RESULT          SetupAdux1050GPIO(
    ADI_ADUX1050_INFO        *pDevInfo,
    ADI_ADUX1050_GPIO_INFO   *pGPIOInfo,
    ADI_CALLBACK         pfCallback
);

#endif
/* Callback function for interrupt pin 1 */
static void ADPDCallback1(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data

);
#ifndef __EV_COG
/* Callback function for AD5940 interrupt pin  */
static void AD5940Callback(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data

);

/* Callback function for ADUX1050 interrupt pin  */
static void ADUX1050Callback(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data

);

/* Callback function for interrupt pin 1 */
static void ADXL362Callback1(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data

);
#endif
#ifdef ADI_DEBUG
static ADI_ADPD_RESULT ValidateAdpdHandle (
    ADI_ADPD_DEVICE * pDevice
);
#ifndef __EV_COG
static ADI_ADUX1050_RESULT ValidateAdux1050Handle (
    ADI_ADUX1050_DEVICE * pDevice
);

static ADI_AD5940_RESULT ValidateAd5940Handle (
    ADI_AD5940_DEVICE * pDevice
);

static ADI_ADXL362_RESULT ValidateAdxlHandle (
    ADI_ADXL362_DEVICE * pDevice
);
#endif
#endif
/* ************************************************************************* */

void GPIO_Init(void) {
  /* Initialize GPIO service with number of callbacks required */
    if (adi_gpio_Init(
            gpioMemory,
            ADI_GPIO_MEMORY_SIZE)
             != ADI_GPIO_SUCCESS) {
        // printf("GPIO Init Failed \n");
    }
    adi_xint_Init(xintMemory, ADI_XINT_MEMORY_SIZE);
    SetupAdpd();
#ifndef __EV_COG
 HAL_GPIO_AD5940_Init();
    SetupAdxl();
    SetupAd5940();
    SetupAdux1050();
#endif
}

void GPIO_IRQ_ADPD_Disable(void) {
    adi_xint_DisableIRQ(ADI_XINT_EVENT_INT1);
}

void GPIO_IRQ_ADPD_Enable(void) {
    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT1, ADI_XINT_IRQ_FALLING_EDGE);
}
#ifndef __EV_COG
void GPIO_IRQ_AD5940_Disable(void) {
    adi_xint_DisableIRQ(ADI_XINT_EVENT_INT2);
}

void GPIO_IRQ_AD5940_Enable(void) {
    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT2, ADI_XINT_IRQ_FALLING_EDGE);
}

void GPIO_IRQ_ADUX1050_Disable(void) {
    adi_xint_DisableIRQ(ADI_XINT_EVENT_INT2);
}

void GPIO_IRQ_ADUX1050_Enable(void) {
    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT2, ADI_XINT_IRQ_FALLING_EDGE);
}


void GPIO_IRQ_ADXL362_Disable(void) {
    adi_xint_DisableIRQ(ADI_XINT_EVENT_INT0);
}

void GPIO_IRQ_ADXL362_Enable(void) {
    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT0, ADI_XINT_IRQ_FALLING_EDGE);
}
#endif
/* Sets up the GPIO for ADPD interrupt */
ADI_ADPD_RESULT SetupAdpdGPIO(
    ADI_ADPD_INFO       *pDevInfo,
    ADI_ADPD_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK            pfCallback
) {
     /* Setup the GPIO pin direction to input */
    if (adi_gpio_InputEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS) {
        return ADI_ADPD_GPIO_FAILED;
    }
#ifndef __EV_COG
     if(adi_xint_RegisterCallback(
            ADI_XINT_EVENT_INT1,
            pfCallback,
            pDevInfo) != ADI_XINT_SUCCESS) {
        return ADI_ADPD_GPIO_FAILED;
    }

    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT1, ADI_XINT_IRQ_FALLING_EDGE);
#else
  if(adi_xint_RegisterCallback(
            ADI_XINT_EVENT_INT0,
            pfCallback,
            pDevInfo) != ADI_XINT_SUCCESS) {
        return ADI_ADPD_GPIO_FAILED;
    }

    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT0, ADI_XINT_IRQ_FALLING_EDGE);
#endif
    return ADI_ADPD_SUCCESS;
}

/**
 * @brief       Enable or disable the ADPD measurement.
 *
 * @details     By default the device is opened in standby mode. After the
 *              required configuration is done, this API can be used to start
 *              the measurement.
 *
 * @param [in]  hDevice             Handle to ADPD device to be enabled.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_ADPD_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADPD_INVALID_HANDLE       [D] If the given ADPD device handle is
 *                                          invalid.
 *
 *  - #ADI_ADPD_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADPD_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADPD_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADPD_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADPD_RESULT AdpdEnable(
    ADI_ADPD_HANDLE  const       hDevice,
    bool                            bEnable
) {
    /* Pointer to the ADPD Device data */
    ADI_ADPD_DEVICE      *pDevice = (ADI_ADPD_DEVICE *) hDevice;

    /* Pointer to device instance information */
    ADI_ADPD_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* ADPD result code */

    /* Check if measurement is already enabled when trying to enable it */
    if ((bEnable == true) && (pDevice->eState == ADI_ADPD_STATE_MEASURING)) {
        return ADI_ADPD_INVALID_OPERATION;
    }

    /* Check if measurement already disabled when trying to disable it */
    if ((bEnable == false) && (pDevice->eState == ADI_ADPD_STATE_STANDBY)) {
        return ADI_ADPD_INVALID_OPERATION;
    }

    /* Check if any device state validation is required before enabling */

#endif /* ADI_DEBUG */

    /* Get the pointer to device instance information */
    pDevInfo = pDevice->pDevInfo;

    if (bEnable == true) {
        if (SetupAdpdGPIO(pDevInfo,
                     &pDevInfo->IntGPIOInfo[0],
                     (ADI_CALLBACK)ADPDCallback1)
           != ADI_ADPD_SUCCESS) {
            return ADI_ADPD_GPIO_FAILED;
        }

        /* Change the state to measuring */
        pDevice->eState     =   ADI_ADPD_STATE_MEASURING;
    } else {
        /* Change the state to standby */
        pDevice->eState     =   ADI_ADPD_STATE_STANDBY;
    }

    return ADI_ADPD_SUCCESS;
}
#ifndef __EV_COG
/* Sets up the GPIO for AD5940 interrupt */
ADI_AD5940_RESULT SetupAd5940GPIO(
    ADI_AD5940_INFO       *pDevInfo,
    ADI_AD5940_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK           pfCallback
) {
     /* Setup the GPIO pin direction to input */
    if (adi_gpio_InputEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS) {
        return ADI_AD5940_GPIO_FAILED;
    }

    if(adi_xint_RegisterCallback(
            ADI_XINT_EVENT_INT2,
            pfCallback,
            pDevInfo) != ADI_XINT_SUCCESS) {
        return ADI_AD5940_GPIO_FAILED;
    }

    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT2, ADI_XINT_IRQ_FALLING_EDGE);

    return ADI_AD5940_SUCCESS;
}

/**
 * @brief       Enable or disable the AD5940 measurement.
 *
 * @details     By default the device is opened in standby mode. After the
 *              required configuration is done, this API can be used to start
 *              the measurement.
 *
 * @param [in]  hDevice             Handle to AD5940 device to be enabled.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_AD5940_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_AD5940_INVALID_HANDLE       [D] If the given AD5940 device handle is
 *                                          invalid.
 *
 *  - #ADI_AD5940_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_AD5940_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_AD5940_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_AD5940_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_AD5940_RESULT Ad5940Enable(
    ADI_AD5940_HANDLE  const       hDevice,
    bool                            bEnable
) {
    /* Pointer to the AD5940 Device data */
    ADI_AD5940_DEVICE      *pDevice = (ADI_AD5940_DEVICE *) hDevice;

    /* Pointer to device instance information */
    ADI_AD5940_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* AD5940 result code */

    /* Check if measurement is already enabled when trying to enable it */
    if ((bEnable == true) && (pDevice->eState == ADI_AD5940_STATE_MEASURING)) {
        return ADI_AD5940_INVALID_OPERATION;
    }

    /* Check if measurement already disabled when trying to disable it */
    if ((bEnable == false) && (pDevice->eState == ADI_AD5940_STATE_STANDBY)) {
        return ADI_AD5940_INVALID_OPERATION;
    }

    /* Check if any device state validation is required before enabling */

#endif /* ADI_DEBUG */

    /* Get the pointer to device instance information */
    pDevInfo = pDevice->pDevInfo;

    if (bEnable == true) {
        if (SetupAd5940GPIO(pDevInfo,
                     &pDevInfo->IntGPIOInfo[0],
                     (ADI_CALLBACK)AD5940Callback)
           != ADI_AD5940_SUCCESS) {
            return ADI_AD5940_GPIO_FAILED;
        }

        /* Change the state to measuring */
        pDevice->eState     =   ADI_AD5940_STATE_MEASURING;
    } else {
        /* Change the state to standby */
        pDevice->eState     =   ADI_AD5940_STATE_STANDBY;
    }

    return ADI_AD5940_SUCCESS;
}

/**
 * @brief       Enable or disable the ADXL362 measurement.
 *
 * @details     By default the device is opened in standby mode. After the
 *              required configuration is done, this API can be used to start
 *              the measurement.
 *
 * @param [in]  hDevice             Handle to ADXL362 device to be enabled.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_ADXL362_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL362_INVALID_HANDLE       [D] If the given ADXL362 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL362_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL362_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADXL362_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL362_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL362_RESULT Adxl362Enable(

    ADI_ADXL362_HANDLE  const       hDevice,
    bool                            bEnable
) {
    /* Pointer to the ADXL362 Device data */
    ADI_ADXL362_DEVICE      *pDevice = (ADI_ADXL362_DEVICE *) hDevice;

    /* Pointer to device instance information */
    ADI_ADXL362_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* ADXL362 result code */

    /* Check if measurement is already enabled when trying to enable it */
    if ((bEnable == true) && (pDevice->eState == ADI_ADXL362_STATE_MEASURING)) {
        return ADI_ADXL362_INVALID_OPERATION;
    }

    /* Check if measurement already disabled when trying to disable it */
    if ((bEnable == false) && (pDevice->eState == ADI_ADXL362_STATE_STANDBY)) {
        return ADI_ADXL362_INVALID_OPERATION;
    }

    /* Check if any device state validation is required before enabling */

#endif /* ADI_DEBUG */

    /* Get the pointer to device instance information */
    pDevInfo = pDevice->pDevInfo;

    if (bEnable == true) {
        if (SetupAdxlGPIO(pDevInfo,
                     &pDevInfo->IntGPIOInfo[0],
                     (ADI_CALLBACK)ADXL362Callback1)
           != ADI_ADXL362_SUCCESS) {
            return ADI_ADXL362_GPIO_FAILED;
        }

        /* Change the state to measuring */
        pDevice->eState     =   ADI_ADXL362_STATE_MEASURING;
    } else {
        /* Change the state to standby */
        pDevice->eState     =   ADI_ADXL362_STATE_STANDBY;
    }
    return ADI_ADXL362_SUCCESS;
}

/* Sets up the GPIO for Accelerometer interrupt 1 */
ADI_ADXL362_RESULT SetupAdxlGPIO(
    ADI_ADXL362_INFO       *pDevInfo,
    ADI_ADXL362_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK            pfCallback
) {
     /* Setup the GPIO pin direction to input */
    if (adi_gpio_InputEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS) {
        return ADI_ADXL362_GPIO_FAILED;
    }

    /* Setup the GPIO pin direction to input */
    /*if (adi_gpio_RegisterCallback(
            pGPIOInfo->eIRQType,
            pfCallback,
            pDevInfo) != ADI_GPIO_SUCCESS) {
        return ADI_ADXL362_GPIO_FAILED;
    }*/
    if(adi_xint_RegisterCallback(
            ADI_XINT_EVENT_INT0,
            pfCallback,
            pDevInfo) != ADI_XINT_SUCCESS) {
        return ADI_ADXL362_GPIO_FAILED;
    }

#ifdef WATCH_V2
    //adi_gpio_EnableExIRQ(XINT_EVT0_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
    adi_xint_EnableIRQ(ADI_XINT_EVENT_INT0, ADI_XINT_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT_EVT1_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //WATCH_V2
    return ADI_ADXL362_SUCCESS;
}

    
/* Sets up the GPIO for ADUX1050 interrupt */
ADI_ADUX1050_RESULT SetupAdux1050GPIO(
    ADI_ADUX1050_INFO       *pDevInfo,
    ADI_ADUX1050_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK            pfCallback
) {
     /* Setup the GPIO pin direction to input */
    if (adi_gpio_InputEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS) {
        return ADI_ADUX1050_GPIO_FAILED;
    }
     /* Enable the pull up for the ADUX interrupt input */
    if (adi_gpio_PullUpEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS) {
        return ADI_ADUX1050_GPIO_FAILED;
    }    
    
//    /* set Pin polarity as rising edge */
//    if(ADI_GPIO_SUCCESS != (eResult = adi_gpio_SetGroupInterruptPolarity(pGPIOInfo->ePort,pGPIOInfo->nPin)))
//    {
//        return ADI_ADUX1050_GPIO_FAILED;
//    }

    /* Enable pin interrupt on group interrupt A */
    if(ADI_GPIO_SUCCESS != (adi_gpio_SetGroupInterruptPins(pGPIOInfo->ePort,ADI_GPIO_INTA_IRQ,pGPIOInfo->nPin)))
    {
        return ADI_ADUX1050_GPIO_FAILED;
    }
    
//    adi_gpio_GetData(pGPIOInfo->ePort,pGPIOInfo->nPin, &gsgpioVal);
//    if(gsgpioVal)
//    {
//      HAL_Delay(100);
//      adi_gpio_GetData(pGPIOInfo->ePort,pGPIOInfo->nPin, &gsgpioVal);
//          if (gsgpioVal)
//              gsgpioFlag =1;  
//    }

    /* Setup the GPIO pin direction to input */
    if(adi_gpio_RegisterCallback(
            pGPIOInfo->eIRQType,
            pfCallback,
            pDevInfo) != ADI_GPIO_SUCCESS) {
        return ADI_ADUX1050_GPIO_FAILED;
    }

//    adi_xint_EnableIRQ(XINT_EVT2_IRQn, ADI_XINT_IRQ_FALLING_EDGE);

    return ADI_ADUX1050_SUCCESS;
}

/**
 * @brief       Enable or disable the ADUX1050 measurement.
 *
 * @details     By default the device is opened in standby mode. After the
 *              required configuration is done, this API can be used to start
 *              the measurement.
 *
 * @param [in]  hDevice             Handle to ADUX1050 device to be enabled.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_ADUX1050_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADUX1050_INVALID_HANDLE       [D] If the given ADUX1050 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADUX1050_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADUX1050_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADUX1050_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADUX1050_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADUX1050_RESULT Adux1050Enable(
    ADI_ADUX1050_HANDLE  const       hDevice,
    bool                            bEnable
) {
    /* Pointer to the ADUX1050 Device data */
    ADI_ADUX1050_DEVICE      *pDevice = (ADI_ADUX1050_DEVICE *) hDevice;

    /* Pointer to device instance information */
    ADI_ADUX1050_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* ADUX1050 result code */

    /* Check if measurement is already enabled when trying to enable it */
    if ((bEnable == true) && (pDevice->eState == ADI_ADUX1050_STATE_MEASURING)) {
        return ADI_ADUX1050_INVALID_OPERATION;
    }

    /* Check if measurement already disabled when trying to disable it */
    if ((bEnable == false) && (pDevice->eState == ADI_ADUX1050_STATE_STANDBY)) {
        return ADI_ADUX1050_INVALID_OPERATION;
    }

    /* Check if any device state validation is required before enabling */

#endif /* ADI_DEBUG */

    /* Get the pointer to device instance information */
    pDevInfo = pDevice->pDevInfo;

    if (bEnable == true) {
        if (SetupAdux1050GPIO(pDevInfo,
                     &pDevInfo->IntGPIOInfo[0],
                     (ADI_CALLBACK)ADUX1050Callback)
           != ADI_ADUX1050_SUCCESS) {
            return ADI_ADUX1050_GPIO_FAILED;
        }

        /* Change the state to measuring */
        pDevice->eState     =   ADI_ADUX1050_STATE_MEASURING;
    } else {
        /* Change the state to standby */
        pDevice->eState     =   ADI_ADUX1050_STATE_STANDBY;
    }

    return ADI_ADUX1050_SUCCESS;
}


/**
 * @brief       Opens the ADUX1050 driver.
 *
 * @details     This opens the ADUX1050 driver and upon success returns the handle
 *              to the driver. All the other APIs accept this handle as the parameter
 *              which determines the instance of the driver.
 *
 *              The ADUX1050 uses I2C for communicating with the host processor.
 *
 *              After the ADUX1050 driver is opened, the part will be in standby
 *              mode. Once the required configuration is done, the API
 *              adi_adux1050_Enable should be called to enable the measurement.
 *

 * @param [in]  nDeviceNum          ADUX1050 device instance number. There can be
 *                                  more than one ADUX1050 devices on a board. This
 *                                  determines the instance that is to be opened.
 *
 * @param [in]  pDeviceMemory       Memory required for ADUX1050 device operation.
 *\n
 *
 * @param [in]  nMemSize            The size of the memory passed in bytes.
 *
 * @param [in]  pfCallback          Application supplied callback function, which
 *                                  will be called by the driver to notify interrupts
 *                                  from the accelerometer.
 * \n
 *                                  Upon getting the interrupt callback, application
 *                                  can use the API #adi_adxl362_GetIntStatus to know
 *                                  what caused the interrupts. It can be one or more
 *                                  of interrupts in the enumeration #ADI_ADUX1050_INT
 *                                  that could have caused the interrupt.
 *
 * @param [in]  pCBParam            Application supplied callback parameter, which
 *                                  will be passed back when the callback function
 *                                  is called.
 *
 * @param [out] hI2CDevice          Handle of I2C device which communicate with  device.
 *
 * @param [out] phDevice            Pointer to a location where the device handle
 *                                  is written.
 * @return      Status
 *
 *  - #ADI_ADUX1050_SUCCESS                      If successfully opened the device.
 *
 *  - #ADI_ADUX1050_INVALID_DEVICE_NUM       [D] If the given device number is outside
 *                                              the number of instances supported.
 *
 *  - #ADI_ADUX1050_NULL_POINTER             [D] If one of the given pointers are
 *                                              pointing to NULL.
 *
 *  - #ADI_ADUX1050_INSUFFICIENT_MEMORY      [D] If the given memory is not sufficient
 *                                              to operate the device.
 *
 *  - #ADI_ADUX1050_NULL_POINTER             [D] If one of the pointer parameter
 *                                              points to NULL.
 *
 *  - #ADI_ADUX1050_UNALIGNED_MEM_PTR        [D] If the given device memory pointer
 *                                              is not aligned to a word boundary.
 *
 *  - #ADI_ADUX1050_DEVICE_ALREADY_OPENED    [D] If the given device is already opened.
 *
 * @note        I2C mode is not supported.
 *
 * @sa          adi_adux1050_Close()
 * @sa          adi_adux1050_Enable()
 */
ADI_ADUX1050_RESULT Adux1050Open(
    uint32_t                        nDeviceNum,
    void * const                    pDeviceMemory,
    uint32_t                        nMemSize,
    ADI_CALLBACK                    pfCallback,
    ADI_ADUX1050_HANDLE * const      phDevice
) {
    /* Pointer to the ADUX1050 device */
    ADI_ADUX1050_DEVICE      *pDevice;
    /* Pointer to the ADUX1050 instance information */
    ADI_ADUX1050_INFO        *pDevInfo;
    /* Pointer for keeping track of application supplied memory usage */
    uint8_t                 *pAvailableMem = (uint8_t *) pDeviceMemory;

#ifdef ADI_DEBUG

    /* Check if the given device number is valid */
    if (nDeviceNum > ADUX1050_NUM_INSTANCES) {
        return ADI_ADUX1050_INVALID_DEVICE_NUM;
    }

    /* Check if the given memory is sufficient to operate at least in interrupt mode */
    if (nMemSize < ADI_ADUX1050_MEMORY_SIZE) {
        return ADI_ADUX1050_INSUFFICIENT_MEMORY;
    }

    /* Check if the given pointer parameters are valid. */
    if ((pDeviceMemory == NULL) || (phDevice == NULL) || (pfCallback == NULL)) {
        return ADI_ADUX1050_NULL_POINTER;
    }

    /* Check if the given pointer to the device memory is word aligned. */
    if (((uint32_t)pDeviceMemory & 0x3u) != 0u) {
        return ADI_ADUX1050_UNALIGNED_MEM_PTR;
    }

    /* Check if the given device instance is already opened */
    if (gADUX1050Dev[nDeviceNum].eState != ADI_ADUX1050_STATE_NOT_OPENED) {
        return ADI_ADUX1050_DEVICE_ALREADY_OPENED;
    }

    /* verify the memory size macros */
    assert(ADI_ADUX1050_MEMORY_SIZE == (sizeof(ADI_ADUX1050_INFO)));

#endif /* ADI_DEBUG */

    pDevice = &gADUX1050Dev[nDeviceNum];

    /*
     * Fragment the given memory for various needs
     */

    pDevice->pDevInfo   =   (ADI_ADUX1050_INFO *)pAvailableMem;
    pDevInfo            =   pDevice->pDevInfo;

    /* Clear the Device Information structure */
    memset((void *)pDevice->pDevInfo, 0, sizeof(ADI_ADUX1050_INFO));

    /* Increment the available memory pointer with amount of memory used */
    pAvailableMem       +=  (sizeof (ADI_ADUX1050_INFO));

    /* Save the given parameters */
    pDevInfo->pfCallback        =   pfCallback;
    /*
     *  Mark the device as opened
     */
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    pDevice->eState =   ADI_ADUX1050_STATE_OPENED;

    ADI_EXIT_CRITICAL_REGION();

    /* return the handle to the opened device*/
    *phDevice = pDevice;

    return ADI_ADUX1050_SUCCESS;
}


/**
 * @brief       Sets the processor GPIO pin to which the ADUX1050 interrupt pin is connected.
 *
 * @details     ADUX1050 has two interrupt pins (INT1 and INT2) which can be
 *              connected to the host processor for notifying the events.
 *              Depending upon the board design, the ADUX1050 interrupt pins can
 *              be connected to different GPIO pins on the host processor. This
 *              API can be used to specify the GPIO pin for which each of the
 *              interrupt is connected. This API should be called separately
 *              (can be twice) to specify the GPIO pin for each of the ADUX1050
 *              interrupt pins.
 *
 * @note        This function should not be called after data flow is enabled.
 *
 * @param [in]  hDevice         Handle to ADUX1050 device whose interrupt pin to be
 *                              configured.
 * @param [in]  eIrq            Indicate whether the interrupt need to be mapped to GROUP-A
                                OR to GROUP-B.
 * @param [in]  eIntPin         ADUX1050 interrupt pin to be configured.
 *
 * @param [in]  eGPIOPort       GPIO port of the host processor to which the
 *                              given ADUX1050 interrupt pin is connected.
 *
 * @param [in]  nGPIOPin        GPIO pin within the given port to which the
 *                              given ADUX1050 interrupt pin is connected.
 *
 * @return      Status
 *
 *  - #ADI_ADUX1050_SUCCESS                  If successfully mapped GPIO pin
 *                                          for interrupt pin.
 *
 *  - #ADI_ADUX1050_INVALID_HANDLE       [D] If the given ADUX1050 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADUX1050_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADUX1050_INVALID_OPERATION    [D] If trying to configure GPIO pin for
 *                                          an interrupt after measurement is enabled.
 *
 * @sa  adi_adxl362_SetIntMask()
 * @sa  adi_adxl362_GetIntMap()
 */
ADI_ADUX1050_RESULT Adux1050ConfigIntPin(
    ADI_ADUX1050_HANDLE  const           hDevice,
    ADI_GPIO_IRQ                         eIrq,
    ADI_ADUX1050_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    uint16_t                            nGPIOPin
) {
    /* Pointer to the ADUX1050 Device data */
    ADI_ADUX1050_DEVICE      *pDevice = (ADI_ADUX1050_DEVICE *) hDevice;

    /* Pointer to GPIO pin and Interrupt pin mapping structure */
    ADI_ADUX1050_GPIO_INFO   *pIntGPIOInfo;

#ifdef ADI_DEBUG
    /* ADUX1050 result code */
    ADI_ADUX1050_RESULT      eResult;

    /* Validate the given device handle */
    if ((eResult = ValidateAdux1050Handle(pDevice)) != ADI_ADUX1050_SUCCESS) {
        return eResult;
    }

    /* Don't allow to configure GPIO when measurement is enabled. */
    if (pDevice->eState  ==  ADI_ADUX1050_STATE_MEASURING) {
        return ADI_ADUX1050_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Get the pointer to GPIO pin information structure for the given interrupt pin */
    pIntGPIOInfo    =   &pDevice->pDevInfo->IntGPIOInfo[(uint8_t)eIntPin];

    /* Save the given GPIO mapping */
    pIntGPIOInfo->ePort =   eGPIOPort;

    /* GPIO service expects the pins as mask. So store the mask which will used later for all the
     * GPIO API calls
     */
    pIntGPIOInfo->nPin  =  nGPIOPin;
    /* Select the IRQ type */
    pIntGPIOInfo->eIRQType = eIrq;

    return ADI_ADUX1050_SUCCESS;
}


/* Opens and sets up the ADUX1050 driver */
void SetupAdux1050(void) {
     /* Open the accelerometer driver */
        if (Adux1050Open (
                ADUX1050_DEV_NUM,
                Adux1050Mem,
                ADI_ADUX1050_MEMORY_SIZE,
                HAL_GPIO_EXT_Callback,
                &hADUX1050) != ADI_ADUX1050_SUCCESS) {
            // printf("Failed to open the accelerometer driver \n");
        }

        /* Set the GPIO pins connected to the host for
        accelerometer interrupts */
        if (Adux1050ConfigIntPin (
                hADUX1050,
                ADI_GPIO_INTA_IRQ,
                ADI_ADUX1050_INTPIN1,
                ADUX1050_INT_GPIO_PORT,
                ADUX1050_INT_GPIO_PIN) != ADI_ADUX1050_SUCCESS) {
            // printf("Failed to configure interrupt pin1 \n");
        }

        if (Adux1050Enable(hADUX1050, true) != ADI_ADUX1050_SUCCESS) {
        // printf("Failed to enable accelerometer \n");
    }
}        
#endif
/* Callback function for interrupt pin 1 */
static void ADPDCallback1(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data
) {
     /* Pointer to Device Instance data */
     ADI_ADPD_INFO        *pDevInfo = (ADI_ADPD_INFO *) pCBParam;

     /* Indicate that interrupt 1 occurred */
     pDevInfo->pfCallback(pDevInfo->pCBParam, (uint32_t)ADI_ADPD_EVENT_INT, NULL);

    return;
}
#ifndef __EV_COG
/* Callback function for AD5940 interrupt pin  */
static void AD5940Callback(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data
) {
     /* Pointer to Device Instance data */
     ADI_AD5940_INFO        *pDevInfo = (ADI_AD5940_INFO *) pCBParam;

     /* Indicate that interrupt 1 occurred */
     pDevInfo->pfCallback(NULL, (uint32_t)ADI_AD5940_EVENT_INT, NULL);

    return;
}

/* Callback function for ADUX1050 interrupt pin  */
static void ADUX1050Callback(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data
) {
     /* Pointer to Device Instance data */
     ADI_ADUX1050_INFO        *pDevInfo = (ADI_ADUX1050_INFO *) pCBParam;

     /* Indicate that interrupt 1 occurred */
     pDevInfo->pfCallback(NULL, (uint32_t)ADI_ADUX1050_EVENT_INT, NULL);

    return;
}


/* Callback function for interrupt pin 1 */
static void ADXL362Callback1(
    void*                   pCBParam,
    ADI_GPIO_PORT           ePort,
    uint32_t                Data

) {
     /* Pointer to Device Instance data */
     ADI_ADXL362_INFO        *pDevInfo = (ADI_ADXL362_INFO *) pCBParam;

     /* Indicate that interrupt 1 occurred */
     pDevInfo->pfCallback(pDevInfo->pCBParam, (uint32_t)ADI_ADXL362_EVENT_INT1, NULL);
    return;
}
#endif
/**
    * @brief    EXTI callback. This is called for all EXTI
    * @param    None
    * @retval   None
    */
void HAL_GPIO_EXT_Callback(void *pCBParam, uint32_t Event, void *pArg) {
  switch (Event) {
        /* Set the flag to indicate that an interrupt event is detected */
        case ADI_ADPD_EVENT_INT:
             AdpdISR();
             break;
	#ifndef __EV_COG
		  case ADI_ADXL362_EVENT_INT1:
             AdxlISR();
             break;
        case ADI_AD5940_EVENT_INT:
             Ad5940ISR();
             break;
        case ADI_ADUX1050_EVENT_INT:
             Adux1050ISR(); 
             //printf("ADUX1050 Event Occured \n");
             break;
	#endif
        default:
             // printf("Unknown event occurred \n");
             break;
    }
}

/**
 * @brief       Opens the adpd driver.
 *
 * @details     This opens the ADPD driver and upon success returns the handle
 *              to the driver. All the other APIs accept this handle as the parameter
 *              which determines the instance of the driver.
 *
 *              The ADPD uses I2C for communicating with the host processor.
 *
 *              After the ADPD driver is opened, the part will be in standby
 *              mode. Once the required configuration is done, the API
 *              adi_adxl362_Enable should be called to enable the measurement.
 *

 * @param [in]  nDeviceNum          ADPD device instance number. There can be
 *                                  more than one ADPD devices on a board. This
 *                                  determines the instance that is to be opened.
 *
 * @param [in]  pDeviceMemory       Memory required for ADPD device operation.
 *\n
 *
 * @param [in]  nMemSize            The size of the memory passed in bytes.
 *
 * @param [in]  pfCallback          Application supplied callback function, which
 *                                  will be called by the driver to notify interrupts
 *                                  from the accelerometer.
 * \n
 *                                  Upon getting the interrupt callback, application
 *                                  can use the API #adi_adxl362_GetIntStatus to know
 *                                  what caused the interrupts. It can be one or more
 *                                  of interrupts in the enumeration #ADI_ADPD_INT
 *                                  that could have caused the interrupt.
 *
 * @param [in]  pCBParam            Application supplied callback parameter, which
 *                                  will be passed back when the callback function
 *                                  is called.
 *
 * @param [out] hI2CDevice          Handle of I2C device which communicate with  device.
 *
 * @param [out] phDevice            Pointer to a location where the device handle
 *                                  is written.
 * @return      Status
 *
 *  - #ADI_ADPD_SUCCESS                      If successfully opened the device.
 *
 *  - #ADI_ADPD_INVALID_DEVICE_NUM       [D] If the given device number is outside
 *                                              the number of instances supported.
 *
 *  - #ADI_ADPD_NULL_POINTER             [D] If one of the given pointers are
 *                                              pointing to NULL.
 *
 *  - #ADI_ADPD_INSUFFICIENT_MEMORY      [D] If the given memory is not sufficient
 *                                              to operate the device.
 *
 *  - #ADI_ADPD_NULL_POINTER             [D] If one of the pointer parameter
 *                                              points to NULL.
 *
 *  - #ADI_ADPD_UNALIGNED_MEM_PTR        [D] If the given device memory pointer
 *                                              is not aligned to a word boundary.
 *
 *  - #ADI_ADPD_DEVICE_ALREADY_OPENED    [D] If the given device is already opened.
 *
 * @note        I2C mode is not supported.
 *
 * @sa          adi_adpd_Close()
 * @sa          adi_adpd_Enable()
 */
ADI_ADPD_RESULT AdpdOpen(
    uint32_t                        nDeviceNum,
    void * const                    pDeviceMemory,
    uint32_t                        nMemSize,
    ADI_CALLBACK                    pfCallback,
    ADI_ADPD_HANDLE * const      phDevice
) {
    /* Pointer to the ADPD device */
    ADI_ADPD_DEVICE      *pDevice;
    /* Pointer to the ADPD instance information */
    ADI_ADPD_INFO        *pDevInfo;
    /* Pointer for keeping track of application supplied memory usage */
    uint8_t                 *pAvailableMem = (uint8_t *) pDeviceMemory;
#ifdef ADI_DEBUG

    /* Check if the given device number is valid */
    if (nDeviceNum > ADPD_NUM_INSTANCES) {
        return ADI_ADPD_INVALID_DEVICE_NUM;
    }

    /* Check if the given memory is sufficient to operate at least in interrupt mode */
    if (nMemSize < ADI_ADPD_MEMORY_SIZE) {
        return ADI_ADPD_INSUFFICIENT_MEMORY;
    }

    /* Check if the given pointer parameters are valid. */
    if ((pDeviceMemory == NULL) || (phDevice == NULL) || (pfCallback == NULL)) {
        return ADI_ADPD_NULL_POINTER;
    }

    /* Check if the given pointer to the device memory is word aligned. */
    if (((uint32_t)pDeviceMemory & 0x3u) != 0u) {
        return ADI_ADPD_UNALIGNED_MEM_PTR;
    }

    /* Check if the given device instance is already opened */
    if (gADPDDev[nDeviceNum].eState != ADI_ADPD_STATE_NOT_OPENED) {
        return ADI_ADPD_DEVICE_ALREADY_OPENED;
    }

    /* verify the memory size macros */
    assert(ADI_ADPD_MEMORY_SIZE == (sizeof(ADI_ADPD_INFO)));

#endif /* ADI_DEBUG */

    pDevice = &gADPDDev[nDeviceNum];

    /*
     * Fragment the given memory for various needs
     */

    pDevice->pDevInfo   =   (ADI_ADPD_INFO *)pAvailableMem;
    pDevInfo            =   pDevice->pDevInfo;

    /* Clear the Device Information structure */
    memset((void *)pDevice->pDevInfo, 0, sizeof(ADI_ADPD_INFO));

    /* Increment the available memory pointer with amount of memory used */
    pAvailableMem       +=  (sizeof (ADI_ADPD_INFO));

    /* Save the given parameters */
    pDevInfo->pfCallback        =   pfCallback;
    /*
     *  Mark the device as opened
     */
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    pDevice->eState =   ADI_ADPD_STATE_OPENED;

    ADI_EXIT_CRITICAL_REGION();

    // Init_ADPD(pDevice);


    /* return the handle to the opened device*/
    *phDevice = pDevice;

    return ADI_ADPD_SUCCESS;
}


/**
 * @brief       Sets the processor GPIO pin to which the ADPD interrupt pin is connected.
 *
 * @details     ADPD has two interrupt pins (INT1 and INT2) which can be
 *              connected to the host processor for notifying the events.
 *              Depending upon the board design, the ADPD interrupt pins can
 *              be connected to different GPIO pins on the host processor. This
 *              API can be used to specify the GPIO pin for which each of the
 *              interrupt is connected. This API should be called separately
 *              (can be twice) to specify the GPIO pin for each of the ADPD
 *              interrupt pins.
 *
 * @note        This function should not be called after data flow is enabled.
 *
 * @param [in]  hDevice         Handle to ADPD device whose interrupt pin to be
 *                              configured.
 * @param [in]  eIrq            Indicate whether the interrupt need to be mapped to GROUP-A
                                OR to GROUP-B.
 * @param [in]  eIntPin         ADPD interrupt pin to be configured.
 *
 * @param [in]  eGPIOPort       GPIO port of the host processor to which the
 *                              given ADPD interrupt pin is connected.
 *
 * @param [in]  nGPIOPin        GPIO pin within the given port to which the
 *                              given ADPD interrupt pin is connected.
 *
 * @return      Status
 *
 *  - #ADI_ADPD_SUCCESS                  If successfully mapped GPIO pin
 *                                          for interrupt pin.
 *
 *  - #ADI_ADPD_INVALID_HANDLE       [D] If the given ADPD device handle
 *                                          is invalid.
 *
 *  - #ADI_ADPD_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADPD_INVALID_OPERATION    [D] If trying to configure GPIO pin for
 *                                          an interrupt after measurement is enabled.
 *
 * @sa  adi_adxl362_SetIntMask()
 * @sa  adi_adxl362_GetIntMap()
 */
ADI_ADPD_RESULT AdpdConfigIntPin(
    ADI_ADPD_HANDLE  const           hDevice,
    IRQn_Type                              eIrq,
    ADI_ADPD_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    uint16_t                            nGPIOPin
) {
    /* Pointer to the ADPD Device data */
    ADI_ADPD_DEVICE      *pDevice = (ADI_ADPD_DEVICE *) hDevice;

    /* Pointer to GPIO pin and Interrupt pin mapping structure */
    ADI_ADPD_GPIO_INFO   *pIntGPIOInfo;

#ifdef ADI_DEBUG
    /* ADPD result code */
    ADI_ADPD_RESULT      eResult;

    /* Validate the given device handle */
    if ((eResult = ValidateAdpdHandle(pDevice)) != ADI_ADPD_SUCCESS) {
        return eResult;
    }

    /* Don't allow to configure GPIO when measurement is enabled. */
    if (pDevice->eState  ==  ADI_ADPD_STATE_MEASURING) {
        return ADI_ADPD_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Get the pointer to GPIO pin information structure for the given interrupt pin */
    pIntGPIOInfo    =   &pDevice->pDevInfo->IntGPIOInfo[(uint8_t)eIntPin];

    /* Save the given GPIO mapping */
    pIntGPIOInfo->ePort =   eGPIOPort;

    /* GPIO service expects the pins as mask. So store the mask which will used later for all the
     * GPIO API calls
     */
    pIntGPIOInfo->nPin  =  nGPIOPin;
    /* Select the IRQ type */
    pIntGPIOInfo->eIRQType = eIrq;

    return ADI_ADPD_SUCCESS;
}

/* Opens and sets up the ADPD driver */
void SetupAdpd(void) {
     /* Open the accelerometer driver */
        if (AdpdOpen (
                ADPD_DEV_NUM,
                AdpdMem,
                ADI_ADPD_MEMORY_SIZE,
                HAL_GPIO_EXT_Callback,
                &hADPD) != ADI_ADPD_SUCCESS) {
            // printf("Failed to open the accelerometer driver \n");
        }

        /* Set the GPIO pins connected to the host for
        accelerometer interrupts */
        if (AdpdConfigIntPin (
                hADPD,
#ifdef WATCH_V2
#ifndef __EV_COG
                XINT_EVT1_IRQn,
#else
				XINT_EVT0_IRQn,
#endif
#else
                XINT_EVT2_IRQn,
#endif
                ADI_ADPD_INTPIN1,
                ADPD_INT1_GPIO_PORT,
                ADPD_INT1_GPIO_PIN) != ADI_ADPD_SUCCESS) {
            // printf("Failed to configure interrupt pin1 \n");
        }

        if (AdpdEnable(hADPD, true) != ADI_ADPD_SUCCESS) {
        // printf("Failed to enable accelerometer \n");
    }
}
#ifndef __EV_COG

/**
 * @brief       Opens the adxl362 driver.
 *
 * @details     This opens the ADXL362 driver and upon success returns the handle
 *              to the driver. All the other APIs accept this handle as the parameter
 *              which determines the instance of the driver.
 *
 *              The ADXL362 uses SPI for communicating with the host processor.
 *
 *              After the ADXL362 driver is opened, the part will be in standby
 *              mode. Once the required configuration is done, the API
 *              adi_adxl362_Enable should be called to enable the measurement.
 *

 * @param [in]  nDeviceNum          ADXL362 device instance number. There can be
 *                                  more than one ADXL362 devices on a board. This
 *                                  determines the instance that is to be opened.
 *
 * @param [in]  pDeviceMemory       Memory required for ADXL362 device operation.
 *\n
 *
 * @param [in]  nMemSize            The size of the memory passed in bytes.
 *
 * @param [in]  pfCallback          Application supplied callback function, which
 *                                  will be called by the driver to notify interrupts
 *                                  from the accelerometer.
 * \n
 *                                  Upon getting the interrupt callback, application
 *                                  can use the API #adi_adxl362_GetIntStatus to know
 *                                  what caused the interrupts. It can be one or more
 *                                  of interrupts in the enumeration #ADI_ADXL362_INT
 *                                  that could have caused the interrupt.
 *
 * @param [in]  pCBParam            Application supplied callback parameter, which
 *                                  will be passed back when the callback function
 *                                  is called.
 *
 * @param [out] hSPIDevice          Handle of SPI device which communicate with  device.
 *
 * @param [out] phDevice            Pointer to a location where the device handle
 *                                  is written.
 * @return      Status
 *
 *  - #ADI_ADXL362_SUCCESS                      If successfully opened the device.
 *
 *  - #ADI_ADXL362_INVALID_DEVICE_NUM       [D] If the given device number is outside
 *                                              the number of instances supported.
 *
 *  - #ADI_ADXL362_NULL_POINTER             [D] If one of the given pointers are
 *                                              pointing to NULL.
 *
 *  - #ADI_ADXL362_INSUFFICIENT_MEMORY      [D] If the given memory is not sufficient
 *                                              to operate the device.
 *
 *  - #ADI_ADXL362_NULL_POINTER             [D] If one of the pointer parameter
 *                                              points to NULL.
 *
 *  - #ADI_ADXL362_UNALIGNED_MEM_PTR        [D] If the given device memory pointer
 *                                              is not aligned to a word boundary.
 *
 *  - #ADI_ADXL362_DEVICE_ALREADY_OPENED    [D] If the given device is already opened.
 *
 * @note        SPI mode is not supported.
 *
 * @sa          adi_adxl362_Close()
 * @sa          adi_adxl362_Enable()
 */
ADI_ADXL362_RESULT Adxl362Open (

    uint32_t                        nDeviceNum,
    void * const                    pDeviceMemory,
    uint32_t                        nMemSize,
    ADI_CALLBACK                    pfCallback,
    ADI_ADXL362_HANDLE * const      phDevice
) {
    /* Pointer to the ADXL362 device */
    ADI_ADXL362_DEVICE      *pDevice;
    /* Pointer to the ADXL362 instance information */
    ADI_ADXL362_INFO        *pDevInfo;
    /* Pointer for keeping track of application supplied memory usage */
    uint8_t                 *pAvailableMem = (uint8_t *) pDeviceMemory;
#ifdef ADI_DEBUG

    /* Check if the given device number is valid */
    if (nDeviceNum > ADXL362_NUM_INSTANCES) {
        return ADI_ADXL362_INVALID_DEVICE_NUM;
    }

    /* Check if the given memory is sufficient to operate at least in interrupt mode */
    if (nMemSize < ADI_ADXL362_MEMORY_SIZE) {
        return ADI_ADXL362_INSUFFICIENT_MEMORY;
    }

    /* Check if the given pointer parameters are valid. */
    if ((pDeviceMemory == NULL) || (phDevice == NULL) || (pfCallback == NULL)) {
        return ADI_ADXL362_NULL_POINTER;
    }

    /* Check if the given pointer to the device memory is word aligned. */
    if (((uint32_t)pDeviceMemory & 0x3u) != 0u) {
        return ADI_ADXL362_UNALIGNED_MEM_PTR;
    }

    /* Check if the given device instance is already opened */
    if (gADXL362Dev[nDeviceNum].eState != ADI_ADXL362_STATE_NOT_OPENED) {
        return ADI_ADXL362_DEVICE_ALREADY_OPENED;
    }

    /* verify the memory size macros */
    assert(ADI_ADXL362_MEMORY_SIZE == (sizeof(ADI_ADXL362_INFO)));

#endif /* ADI_DEBUG */

    pDevice = &gADXL362Dev[nDeviceNum];

    /*
     * Fragment the given memory for various needs
     */

    pDevice->pDevInfo   =   (ADI_ADXL362_INFO * )pAvailableMem;
    pDevInfo            =   pDevice->pDevInfo;

    /* Clear the Device Information structure */
    memset((void *)pDevice->pDevInfo, 0, sizeof(ADI_ADXL362_INFO));

    /* Increment the available memory pointer with amount of memory used */
    pAvailableMem       +=  (sizeof (ADI_ADXL362_INFO));

    /* Save the given parameters */
    pDevInfo->pfCallback        =   pfCallback;
    /*
     *  Mark the device as opened
     */
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    pDevice->eState =   ADI_ADXL362_STATE_OPENED;

    ADI_EXIT_CRITICAL_REGION();

    //Init_adxl362(pDevice);


    /* return the handle to the opened device*/
    *phDevice = pDevice;

    return ADI_ADXL362_SUCCESS;
}

/**
 * @brief       Sets the processor GPIO pin to which the ADXL362 interrupt pin is connected.
 *
 * @details     ADXL362 has two interrupt pins (INT1 and INT2) which can be
 *              connected to the host processor for notifying the events.
 *              Depending upon the board design, the ADXL362 interrupt pins can
 *              be connected to different GPIO pins on the host processor. This
 *              API can be used to specify the GPIO pin for which each of the
 *              interrupt is connected. This API should be called separately
 *              (can be twice) to specify the GPIO pin for each of the ADXL362
 *              interrupt pins.
 *
 * @note        This function should not be called after data flow is enabled.
 *
 * @param [in]  hDevice         Handle to ADXL362 device whose interrupt pin to be
 *                              configured.
 * @param [in]  eIrq            Indicate whether the interrupt need to be mapped to GROUP-A
                                OR to GROUP-B.
 * @param [in]  eIntPin         ADXL362 interrupt pin to be configured.
 *
 * @param [in]  eGPIOPort       GPIO port of the host processor to which the
 *                              given ADXL362 interrupt pin is connected.
 *
 * @param [in]  nGPIOPin        GPIO pin within the given port to which the
 *                              given ADXL362 interrupt pin is connected.
 *
 * @return      Status
 *
 *  - #ADI_ADXL362_SUCCESS                  If successfully mapped GPIO pin
 *                                          for interrupt pin.
 *
 *  - #ADI_ADXL362_INVALID_HANDLE       [D] If the given ADXL362 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL362_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL362_INVALID_OPERATION    [D] If trying to configure GPIO pin for
 *                                          an interrupt after measurement is enabled.
 *
 * @sa  adi_adxl362_SetIntMask()
 * @sa  adi_adxl362_GetIntMap()
 */
ADI_ADXL362_RESULT Adxl362ConfigIntPin(
    ADI_ADXL362_HANDLE  const           hDevice,
    IRQn_Type                              eIrq,
    ADI_ADXL362_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    uint16_t                            nGPIOPin
) {
    /* Pointer to the ADXL362 Device data */
    ADI_ADXL362_DEVICE      *pDevice = (ADI_ADXL362_DEVICE *) hDevice;


    /* Pointer to GPIO pin and Interrupt pin mapping structure */
    ADI_ADXL362_GPIO_INFO   *pIntGPIOInfo;

#ifdef ADI_DEBUG
    /* ADXL362 result code */
    ADI_ADXL362_RESULT      eResult;

    /* Validate the given device handle */
    if ((eResult = ValidateAdxlHandle(pDevice)) != ADI_ADXL362_SUCCESS) {
        return eResult;
    }

    /* Don't allow to configure GPIO when measurement is enabled. */
    if (pDevice->eState  ==  ADI_ADXL362_STATE_MEASURING) {
        return ADI_ADXL362_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Get the pointer to GPIO pin information structure for the given interrupt pin */
    pIntGPIOInfo    =   &pDevice->pDevInfo->IntGPIOInfo[(uint8_t)eIntPin];

    /* Save the given GPIO mapping */
    pIntGPIOInfo->ePort =   eGPIOPort;

    /* GPIO service expects the pins as mask. So store the mask which will used later for all the
     * GPIO API calls
     */
    pIntGPIOInfo->nPin  =  nGPIOPin;
    /* Select the IRQ type */
    pIntGPIOInfo->eIRQType = eIrq;

    return ADI_ADXL362_SUCCESS;
}

/* Opens and sets up the accelerometer driver */
void SetupAdxl(void) {
     /* Open the accelerometer driver */
        if(Adxl362Open (
                ADXL362_DEV_NUM,
                AccelMem,
                ADI_ADXL362_MEMORY_SIZE,
                HAL_GPIO_EXT_Callback,
                &hADXL362) != ADI_ADXL362_SUCCESS) {
            // printf("Failed to open the accelerometer driver \n");
        }

        /* Set the GPIO pins connected to the host for accelerometer interrupts */
        if(Adxl362ConfigIntPin (
                hADXL362,
#ifdef WATCH_V2
                XINT_EVT0_IRQn,
#else
                XINT_EVT1_IRQn,
#endif // WATCH_V2
                ADI_ADXL362_INTPIN1,
                ADXL362_INT1_GPIO_PORT,
                ADXL362_INT1_GPIO_PIN) != ADI_ADXL362_SUCCESS) {
            // printf("Failed to configure interrupt pin1 \n");
        }

        if(Adxl362Enable(

            hADXL362,
            true) != ADI_ADXL362_SUCCESS) {
        // printf("Failed to enable accelerometer \n");
    }
}


/**
 * @brief       Opens the AD5940 driver.
 *
 * @details     This opens the AD5940 driver and upon success returns the handle
 *              to the driver. All the other APIs accept this handle as the parameter
 *              which determines the instance of the driver.
 *
 *              The AD5940 uses I2C for communicating with the host processor.
 *
 *              After the AD5940 driver is opened, the part will be in standby
 *              mode. Once the required configuration is done, the API
 *              adi_ad5940_Enable should be called to enable the measurement.
 *

 * @param [in]  nDeviceNum          AD5940 device instance number. There can be
 *                                  more than one AD5940 devices on a board. This
 *                                  determines the instance that is to be opened.
 *
 * @param [in]  pDeviceMemory       Memory required for AD5940 device operation.
 *\n
 *
 * @param [in]  nMemSize            The size of the memory passed in bytes.
 *
 * @param [in]  pfCallback          Application supplied callback function, which
 *                                  will be called by the driver to notify interrupts
 *                                  from the accelerometer.
 * \n
 *                                  Upon getting the interrupt callback, application
 *                                  can use the API #adi_adxl362_GetIntStatus to know
 *                                  what caused the interrupts. It can be one or more
 *                                  of interrupts in the enumeration #ADI_AD5940_INT
 *                                  that could have caused the interrupt.
 *
 * @param [in]  pCBParam            Application supplied callback parameter, which
 *                                  will be passed back when the callback function
 *                                  is called.
 *
 * @param [out] hI2CDevice          Handle of I2C device which communicate with  device.
 *
 * @param [out] phDevice            Pointer to a location where the device handle
 *                                  is written.
 * @return      Status
 *
 *  - #ADI_AD5940_SUCCESS                      If successfully opened the device.
 *
 *  - #ADI_AD5940_INVALID_DEVICE_NUM       [D] If the given device number is outside
 *                                              the number of instances supported.
 *
 *  - #ADI_AD5940_NULL_POINTER             [D] If one of the given pointers are
 *                                              pointing to NULL.
 *
 *  - #ADI_AD5940_INSUFFICIENT_MEMORY      [D] If the given memory is not sufficient
 *                                              to operate the device.
 *
 *  - #ADI_AD5940_NULL_POINTER             [D] If one of the pointer parameter
 *                                              points to NULL.
 *
 *  - #ADI_AD5940_UNALIGNED_MEM_PTR        [D] If the given device memory pointer
 *                                              is not aligned to a word boundary.
 *
 *  - #ADI_AD5940_DEVICE_ALREADY_OPENED    [D] If the given device is already opened.
 *
 * @note        I2C mode is not supported.
 *
 * @sa          adi_ad5940_Close()
 * @sa          adi_ad5940_Enable()
 */
ADI_AD5940_RESULT Ad5940Open(
    uint32_t                        nDeviceNum,
    void * const                    pDeviceMemory,
    uint32_t                        nMemSize,
    ADI_CALLBACK                    pfCallback,
    ADI_AD5940_HANDLE * const      phDevice
) {
    /* Pointer to the AD5940 device */
    ADI_AD5940_DEVICE      *pDevice;
    /* Pointer to the AD5940 instance information */
    ADI_AD5940_INFO        *pDevInfo;
    /* Pointer for keeping track of application supplied memory usage */
    uint8_t                 *pAvailableMem = (uint8_t *) pDeviceMemory;

#ifdef ADI_DEBUG

    /* Check if the given device number is valid */
    if (nDeviceNum > AD5940_NUM_INSTANCES) {
        return ADI_AD5940_INVALID_DEVICE_NUM;
    }

    /* Check if the given memory is sufficient to operate at least in interrupt mode */
    if (nMemSize < ADI_AD5940_MEMORY_SIZE) {
        return ADI_AD5940_INSUFFICIENT_MEMORY;
    }

    /* Check if the given pointer parameters are valid. */
    if ((pDeviceMemory == NULL) || (phDevice == NULL) || (pfCallback == NULL)) {
        return ADI_AD5940_NULL_POINTER;
    }

    /* Check if the given pointer to the device memory is word aligned. */
    if (((uint32_t)pDeviceMemory & 0x3u) != 0u) {
        return ADI_AD5940_UNALIGNED_MEM_PTR;
    }

    /* Check if the given device instance is already opened */
    if (gAD5940Dev[nDeviceNum].eState != ADI_AD5940_STATE_NOT_OPENED) {
        return ADI_AD5940_DEVICE_ALREADY_OPENED;
    }

    /* verify the memory size macros */
    assert(ADI_AD5940_MEMORY_SIZE == (sizeof(ADI_AD5940_INFO)));

#endif /* ADI_DEBUG */

    pDevice = &gAD5940Dev[nDeviceNum];

    /*
     * Fragment the given memory for various needs
     */

    pDevice->pDevInfo   =   (ADI_AD5940_INFO *)pAvailableMem;
    pDevInfo            =   pDevice->pDevInfo;

    /* Clear the Device Information structure */
    memset((void *)pDevice->pDevInfo, 0, sizeof(ADI_AD5940_INFO));

    /* Increment the available memory pointer with amount of memory used */
    pAvailableMem       +=  (sizeof (ADI_AD5940_INFO));

    /* Save the given parameters */
    pDevInfo->pfCallback        =   pfCallback;
    /*
     *  Mark the device as opened
     */
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    pDevice->eState =   ADI_AD5940_STATE_OPENED;

    ADI_EXIT_CRITICAL_REGION();

    /* return the handle to the opened device*/
    *phDevice = pDevice;

    return ADI_AD5940_SUCCESS;
}


/**
 * @brief       Sets the processor GPIO pin to which the AD5940 interrupt pin is connected.
 *
 * @details     AD5940 has two interrupt pins (INT1 and INT2) which can be
 *              connected to the host processor for notifying the events.
 *              Depending upon the board design, the AD5940 interrupt pins can
 *              be connected to different GPIO pins on the host processor. This
 *              API can be used to specify the GPIO pin for which each of the
 *              interrupt is connected. This API should be called separately
 *              (can be twice) to specify the GPIO pin for each of the AD5940
 *              interrupt pins.
 *
 * @note        This function should not be called after data flow is enabled.
 *
 * @param [in]  hDevice         Handle to AD5940 device whose interrupt pin to be
 *                              configured.
 * @param [in]  eIrq            Indicate whether the interrupt need to be mapped to GROUP-A
                                OR to GROUP-B.
 * @param [in]  eIntPin         AD5940 interrupt pin to be configured.
 *
 * @param [in]  eGPIOPort       GPIO port of the host processor to which the
 *                              given AD5940 interrupt pin is connected.
 *
 * @param [in]  nGPIOPin        GPIO pin within the given port to which the
 *                              given AD5940 interrupt pin is connected.
 *
 * @return      Status
 *
 *  - #ADI_AD5940_SUCCESS                  If successfully mapped GPIO pin
 *                                          for interrupt pin.
 *
 *  - #ADI_AD5940_INVALID_HANDLE       [D] If the given AD5940 device handle
 *                                          is invalid.
 *
 *  - #ADI_AD5940_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_AD5940_INVALID_OPERATION    [D] If trying to configure GPIO pin for
 *                                          an interrupt after measurement is enabled.
 *
 * @sa  adi_adxl362_SetIntMask()
 * @sa  adi_adxl362_GetIntMap()
 */
ADI_AD5940_RESULT Ad5940ConfigIntPin(
    ADI_AD5940_HANDLE  const           hDevice,
    IRQn_Type                              eIrq,
    ADI_AD5940_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    uint16_t                            nGPIOPin
) {
    /* Pointer to the AD5940 Device data */
    ADI_AD5940_DEVICE      *pDevice = (ADI_AD5940_DEVICE *) hDevice;

    /* Pointer to GPIO pin and Interrupt pin mapping structure */
    ADI_AD5940_GPIO_INFO   *pIntGPIOInfo;

#ifdef ADI_DEBUG
    /* AD5940 result code */
    ADI_AD5940_RESULT      eResult;

    /* Validate the given device handle */
    if ((eResult = ValidateAd5940Handle(pDevice)) != ADI_AD5940_SUCCESS) {
        return eResult;
    }

    /* Don't allow to configure GPIO when measurement is enabled. */
    if (pDevice->eState  ==  ADI_AD5940_STATE_MEASURING) {
        return ADI_AD5940_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Get the pointer to GPIO pin information structure for the given interrupt pin */
    pIntGPIOInfo    =   &pDevice->pDevInfo->IntGPIOInfo[(uint8_t)eIntPin];

    /* Save the given GPIO mapping */
    pIntGPIOInfo->ePort =   eGPIOPort;

    /* GPIO service expects the pins as mask. So store the mask which will used later for all the
     * GPIO API calls
     */
    pIntGPIOInfo->nPin  =  nGPIOPin;
    /* Select the IRQ type */
    pIntGPIOInfo->eIRQType = eIrq;

    return ADI_AD5940_SUCCESS;
}

/* Opens and sets up the AD5940 driver */
void SetupAd5940(void) {
     /* Open the accelerometer driver */
        if (Ad5940Open (
                AD5940_DEV_NUM,
                Ad5940Mem,
                ADI_AD5940_MEMORY_SIZE,
                HAL_GPIO_EXT_Callback,
                &hAD5940) != ADI_AD5940_SUCCESS) {
            // printf("Failed to open the accelerometer driver \n");
        }

        /* Set the GPIO pins connected to the host for
        accelerometer interrupts */
        if (Ad5940ConfigIntPin (
                hAD5940,
                XINT_EVT2_IRQn,
                ADI_AD5940_INTPIN1,
                AD5940_INT_GPIO_PORT,
                AD5940_INT_GPIO_PIN) != ADI_AD5940_SUCCESS) {
        }

        if (Ad5940Enable(hAD5940, true) != ADI_AD5940_SUCCESS) {
        // printf("Failed to enable AD5940 \n");
    }
}


/*==========================================================================
    Validation functions.
 *=========================================================================*/
#ifdef ADI_DEBUG

static ADI_ADXL362_RESULT ValidateAdxlHandle (
    ADI_ADXL362_DEVICE * pDevice
) {
    uint32_t    nDevIdx;

    for (nDevIdx = 0u; nDevIdx < ADXL362_NUM_INSTANCES; nDevIdx++) {
        /* Check if the given device pointer matches any of the  instances */
        if (pDevice == &gADXL362Dev[nDevIdx]) {
            /* The given pointer matches an instance, now check if it is opened */
            if (pDevice->eState == ADI_ADXL362_STATE_NOT_OPENED) {
                return ADI_ADXL362_DEVICE_NOT_OPENED;
            } else {
                return ADI_ADXL362_SUCCESS;
            }
        }
    }

    return ADI_ADXL362_INVALID_HANDLE;
}
#endif /* ADI_DEBUG */
#endif
/*==========================================================================
    Validation functions.
 *=========================================================================*/
#ifdef ADI_DEBUG

static ADI_ADPD_RESULT ValidateAdpdHandle(ADI_ADPD_DEVICE * pDevice) {
  uint32_t    nDevIdx;

  for (nDevIdx = 0u; nDevIdx < ADPD_NUM_INSTANCES; nDevIdx++) {
      /* Check if the given device pointer matches any of the  instances */
      if (pDevice == &gADPDDev[nDevIdx]) {
          /* The given pointer matches an instance, now check if it is opened */
          if (pDevice->eState == ADI_ADPD_STATE_NOT_OPENED) {
              return ADI_ADPD_DEVICE_NOT_OPENED;
          } else {
              return ADI_ADPD_SUCCESS;
          }
      }
  }

    return ADI_ADPD_INVALID_HANDLE;
}
#endif /* ADI_DEBUG */
#ifndef __EV_COG
/*==========================================================================
    Validation functions.
 *=========================================================================*/
#ifdef ADI_DEBUG

static ADI_AD5940_RESULT ValidateAd5940Handle(ADI_AD5940_DEVICE * pDevice) {
  uint32_t    nDevIdx;

  for (nDevIdx = 0u; nDevIdx < AD5940_NUM_INSTANCES; nDevIdx++) {
      /* Check if the given device pointer matches any of the  instances */
      if (pDevice == &gAD5940Dev[nDevIdx]) {
          /* The given pointer matches an instance, now check if it is opened */
          if (pDevice->eState == ADI_AD5940_STATE_NOT_OPENED) {
              return ADI_AD5940_DEVICE_NOT_OPENED;
          } else {
              return ADI_AD5940_SUCCESS;
          }
      }
  }

    return ADI_AD5940_INVALID_HANDLE;
}
#endif /* ADI_DEBUG */

/*==========================================================================
    Validation functions.
 *=========================================================================*/
#ifdef ADI_DEBUG

static ADI_ADUX1050_RESULT ValidateAdux1050Handle(ADI_ADUX1050_DEVICE * pDevice) {
  uint32_t    nDevIdx;

  for (nDevIdx = 0u; nDevIdx < ADUX1050_NUM_INSTANCES; nDevIdx++) {
      /* Check if the given device pointer matches any of the  instances */
      if (pDevice == &gADUX1050Dev[nDevIdx]) {
          /* The given pointer matches an instance, now check if it is opened */
          if (pDevice->eState == ADI_ADUX1050_STATE_NOT_OPENED) {
              return ADI_ADUX1050_DEVICE_NOT_OPENED;
          } else {
              return ADI_ADUX1050_SUCCESS;
          }
      }
  }

    return ADI_ADUX1050_INVALID_HANDLE;
}
#endif /* ADI_DEBUG */
#endif
/*  EDA  */

void HAL_GPIO_AD7689_2_5V_EN_Init(){
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,true);
  }

void HAL_GPIO_AD7689_2_5V_EN_DeInit(){
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,false);
  }

void HAL_GPIO_AD7689_2_5V_EN_High(){
  adi_gpio_SetHigh(ADI_GPIO_PORT0,ADI_GPIO_PIN_8);
  }

void HAL_GPIO_AD7689_2_5V_EN_Low(){
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_8);
  }


inline void HAL_GPIO_EDA_SQ_EX_High(){
  adi_gpio_SetHigh(ADI_GPIO_PORT0,ADI_GPIO_PIN_9);
  }

inline void HAL_GPIO_EDA_SQ_EX_Low(){
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_9);
  }

inline void HAL_GPIO_EDA_SQ_EX_Toggle(){
  adi_gpio_Toggle(ADI_GPIO_PORT0,ADI_GPIO_PIN_9);
  }

void HAL_GPIO_EDA_Init(){

  adi_gpio_OutputEnable(ADI_GPIO_PORT2,ADI_GPIO_PIN_0,true);
  adi_gpio_SetLow(ADI_GPIO_PORT2,ADI_GPIO_PIN_0);
  adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_13,true);
  adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_13);

}

void HAL_GPIO_EDA_CalInit(){

  /* Enable the GPIO to set the EDA/ECG sel pin */
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_2,true);
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_2);
  /* Enable the GPIO to set the  EDA_SQ_EX pin */
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_9,true);
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_9);

  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,true);
  adi_gpio_SetHigh(ADI_GPIO_PORT0,ADI_GPIO_PIN_8);

  // EDA CAL pin to high
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_12,true);
  adi_gpio_SetHigh(ADI_GPIO_PORT0,ADI_GPIO_PIN_12);

}

void HAL_GPIO_EDA_CalDeInit(){

  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,true);
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_8);

  // EDA CAL pin to low
  adi_gpio_OutputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_12,true);
  adi_gpio_SetLow(ADI_GPIO_PORT0,ADI_GPIO_PIN_12);

}

void HAL_GPIO_BCM_Init(){

  adi_gpio_OutputEnable(ADI_GPIO_PORT2,ADI_GPIO_PIN_0,true);
  adi_gpio_SetLow(ADI_GPIO_PORT2,ADI_GPIO_PIN_0);
  adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_13,true);
  adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_13);

}

void HAL_GPIO_BCM_DeInit(){

}

void HAL_GPIO_ECG_Electrode_Select(uint8_t enable){

  if(enable){
    /* Enable the GPIO to set the ECG sel pin */
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_7,true); //switch selection for SPORT
    adi_gpio_SetHigh(ADI_GPIO_PORT1,ADI_GPIO_PIN_7);   
  }else{
    /* Enable the GPIO to set the ECG sel pin */
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_7,true); //switch selection for MONITOR
    adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_7);
  }
  
  adi_gpio_OutputEnable(ADI_GPIO_PORT2,ADI_GPIO_PIN_0,true); //Case electrode 1
  adi_gpio_SetHigh(ADI_GPIO_PORT2,ADI_GPIO_PIN_0);
  adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_13,true);//Case electrode 2
  adi_gpio_SetHigh(ADI_GPIO_PORT1,ADI_GPIO_PIN_13);

}

void HAL_GPIO_ECG_Sport_Enable(uint8_t enable){

  if(enable){
    adi_gpio_OutputEnable(ADI_GPIO_PORT3,ADI_GPIO_PIN_2,true);//Fast Restore
    adi_gpio_SetLow(ADI_GPIO_PORT3,ADI_GPIO_PIN_2);
    adi_gpio_OutputEnable(ADI_GPIO_PORT3,ADI_GPIO_PIN_0,true);//LOD mode
    adi_gpio_SetLow(ADI_GPIO_PORT3,ADI_GPIO_PIN_0);
    adi_gpio_OutputEnable(ADI_GPIO_PORT2,ADI_GPIO_PIN_2,true);//Shutdown
    adi_gpio_SetHigh(ADI_GPIO_PORT2,ADI_GPIO_PIN_2);
    adi_gpio_OutputEnable(ADI_GPIO_PORT3,ADI_GPIO_PIN_1,true);//RLD Shutdown
    adi_gpio_SetHigh(ADI_GPIO_PORT3,ADI_GPIO_PIN_1);
    adi_gpio_InputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,true);//Leads off detection
  }
  else
  {
    adi_gpio_OutputEnable(ADI_GPIO_PORT2,ADI_GPIO_PIN_2,true);//Shutdown
    adi_gpio_SetLow(ADI_GPIO_PORT2,ADI_GPIO_PORT2);
    adi_gpio_InputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,false);//Leads off detection
  }

}

void HAL_GPIO_ECG_Monitor_Enable(uint8_t enable){

  if(enable){
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_14,true);//Fast Restore
    adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_14);
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_8,true);//LOD mode
    adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_8);
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_6,true);//Shutdown
    adi_gpio_SetHigh(ADI_GPIO_PORT1,ADI_GPIO_PIN_6);
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_15,true);//RLD Shutdown
    adi_gpio_SetHigh(ADI_GPIO_PORT1,ADI_GPIO_PIN_15);
    adi_gpio_InputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_9,true);//Leads off detection
  }
  else
  {
    adi_gpio_OutputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_6,true);//Shutdown
    adi_gpio_SetLow(ADI_GPIO_PORT1,ADI_GPIO_PIN_6);
    adi_gpio_InputEnable(ADI_GPIO_PORT0,ADI_GPIO_PIN_9,false);//Leads off detection
  }

}

uint8_t HAL_GPIO_ECG_Sport_LOD_GetStatus(){

  uint16_t gpio_status;
  adi_gpio_GetData(ADI_GPIO_PORT0,ADI_GPIO_PIN_8,&gpio_status);//LOD status for SPORT
  if(gpio_status)
  {
    return 1;  // LOD = 1 --> Electrodes not touched
  }
  else
  {
    return 0;   // LOD = 0 --> Electrodes touched
  }

}

uint8_t HAL_GPIO_ECG_Monitor_LOD_GetStatus(){

  uint16_t gpio_status;
  adi_gpio_GetData(ADI_GPIO_PORT0,ADI_GPIO_PIN_9,&gpio_status);//LOD status for MONITOR
  if(gpio_status)
  {
    return 1;  // LOD = 1 --> Electrodes not touched
  }
  else
  {
    return 0;   // LOD = 0 --> Electrodes touched
  }

}

void HAL_GPIO_AD5940_Init(){

  adi_gpio_OutputEnable(ADI_GPIO_PORT2, ADI_GPIO_PIN_13, true);
}

void HAL_GPIO_AD5940_CS_Set(){

    adi_gpio_SetHigh(ADI_GPIO_PORT2, ADI_GPIO_PIN_13);
}

void HAL_GPIO_AD5940_CS_Clr(){

    adi_gpio_SetLow(ADI_GPIO_PORT2, ADI_GPIO_PIN_13);
}
