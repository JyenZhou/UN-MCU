/*!
 *****************************************************************************
 * @file:    adi_adxl363_def.h
 * @brief:   Driver  for acceleratometer ADXL363 
 *----------------------------------------------------------------------------
 *
Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/


#ifndef ADI_ADXL363_DEF_H
#define ADI_ADXL363_DEF_H

#define ADI_ADXL363_RESET_KEY           0x52u

/*! \cond PRIVATE */

/* Number of ADXL363 devices on the board */
#define ADXL363_NUM_INSTANCES       (sizeof(gADXL363Dev)/sizeof(ADI_ADXL363_DEVICE))


/* Minimum acceleration threshold for activity detection. */
#define ADXL363_MIN_ACT_THRESH      0x1u

   /* Maximum acceleration threshold for activity detection. */
#define ADXL363_MAX_ACT_THRESH      0x2048u

/* Minimum acceleration threshold for inactivity detection. */
#define ADXL363_MIN_INACT_THRESH    0x1u

/* Minimum acceleration threshold for Free Fall detection. */
#define ADXL363_MIN_FF_THRESH       0x1u

/* Minimum time that the acceleration to be less that threshold for Free Fall detection. */
#define ADXL363_MIN_FF_TIME         0x1u

/* Minimum samples that can set for a FIFO trigger. */
#define ADXL363_MIN_FIFO_SAMPLES    0x1u

/* Number of interrupt pins to the host processor */
#define ADXL363_NUM_INT_PIN         0x2u


/* Macros to generate read/write address from given address */
#ifdef ADI_DEBUG 
#define ADXL363_REG_READ(RegAddr, pRegData)      \
        if(SPI_Read (pDevice->pDevInfo, (RegAddr), (pRegData), 1u) != ADI_ADXL363_SUCCESS) \
        {\
            return ADI_ADXL363_DEVICE_ACCESS_FAILED;\
        }

#define ADXL363_REG_WRITE(RegAddr, pRegData)     \
        if(SPI_Write(pDevice->pDevInfo, (RegAddr), (pRegData)) != ADI_ADXL363_SUCCESS) \
        { \
            return ADI_ADXL363_DEVICE_ACCESS_FAILED;\
        }
#else

#define ADXL363_REG_READ(RegAddr, pRegData)     \
        SPI_Read (pDevice->pDevInfo, (RegAddr), (pRegData), 1u);

#define ADXL363_REG_WRITE(RegAddr, pRegData)     \
        SPI_Write(pDevice->pDevInfo, (RegAddr), (pRegData));

#endif /* ADI_DEBUG */
        
#define FIFO_HALF_MARK_LIMIT       0X100u
        
/* enumeration of different device or driver states */
typedef enum
{
    /* The device is not yet opened */
    ADI_ADXL363_STATE_NOT_OPENED,

    /* The device is opened */
    ADI_ADXL363_STATE_OPENED,

    /* The device is opened, but in standby mode */
    ADI_ADXL363_STATE_STANDBY,

    /* The device is in measuring state */
    ADI_ADXL363_STATE_MEASURING

} ADI_ADXL363_STATE;


/* Structure to hold the information regarding the SPI device configuration */
typedef struct adi_adxl363_SPI_Info
{
    /* SPI Device number to be used for communicating with ADXL363 */
    uint32_t                nDeviceNum;

    /* Chip select number to address ADXL363 */
    ADI_SPI_CHIP_SELECT     eChipSelect;

    /* SPI clock divide to get the required SPI clock rate */
    uint32_t                nClkDiv;

    /* SPI device handle */
    ADI_SPI_HANDLE          hSPIDevice;
  
}ADI_ADXL363_SPI_INFO;

/* Structure to hold mapping between ADXL363 interrupt pin and host processor
 * GPIO pin. */
typedef struct adi_adxl363_GPIO_Info
{
    /* GPIO interrupt group */
    ADI_GPIO_IRQ            eIRQGrp;
    
    /* GPIO port to which the interrupt pin is connected */
    ADI_GPIO_PORT           ePort;

    /* GPIO pin within the GPIO port */
    ADI_GPIO_DATA           nPin;

}ADI_ADXL363_GPIO_INFO;

/* Structure to hold the ADXL363 device related instance data. This structure
 * is defined using the memory passed by the application. */
typedef struct adi_adxl363_info
{
    /* Callback function pointer */
    ADI_CALLBACK            pfCallback;

    /* Chip select number to address ADXL363 */
    ADI_SPI_CHIP_SELECT     eChipSelect;

    /* SPI clock divide to get the required SPI clock rate */
    uint32_t                nClkDiv;

    /* Callback parameter */
    void *                  pCBParam;

    /* GPIO pin connection information for Interrupt pin 1 and 2 */
    ADI_ADXL363_GPIO_INFO   IntGPIOInfo[ADXL363_NUM_INT_PIN];

    /* SPI device handle */
    ADI_SPI_HANDLE          hSPIDevice;
    
    /* SPI device memory */
    uint8_t                  aDeviceMemory[ADI_SPI_MEMORY_SIZE];
} ADI_ADXL363_INFO;


/* ADXL363 device instance data */
typedef struct adi_adxl363_device
{
    /* State of the driver */
    ADI_ADXL363_STATE   eState;

    /* Pointer to the device instance information */
    ADI_ADXL363_INFO    *pDevInfo;

} ADI_ADXL363_DEVICE;

typedef struct adi_adxl363_device_config
{
    /* State of the driver */
    uint8_t nAddress;

    /* Pointer to the device instance information */
    uint8_t nData;

} ADI_ADXL363_DEVICE_CONFIG;

/***************************************************************************
  Local function declaration
 ***************************************************************************/

/* Spi call to read Fifo contents */
static ADI_ADXL363_RESULT SPI_FifoRead(
   ADI_ADXL363_INFO     *pDevInfo,
   void                 *pBuffer,
   uint16_t              nSize);

/* Function to read the data using SPI interface. */
static ADI_ADXL363_RESULT SPI_Read(
    ADI_ADXL363_INFO     *pDevInfo,
    uint8_t               RegAddr,
    uint8_t              *pBuffer,
    uint16_t              nSize
);

/* Function to write the data using SPI interface. Assumes single register write */
static ADI_ADXL363_RESULT SPI_Write(
    ADI_ADXL363_INFO    *pDevInfo,
    uint8_t              RegAddr,
    uint8_t              Value
);

/* Sets up the GPIO for Accelerometer interrupt 1 and 2 */
static ADI_ADXL363_RESULT SetupGPIO (
    ADI_ADXL363_INFO       *pDevInfo,
    ADI_ADXL363_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK            pfCallback,
    void *                  pCBParam,
    ADI_GPIO_IRQ_TRIGGER_CONDITION          eSense
);

/*! \endcond */
#endif /* ADI_ADXL363_REG_H */
