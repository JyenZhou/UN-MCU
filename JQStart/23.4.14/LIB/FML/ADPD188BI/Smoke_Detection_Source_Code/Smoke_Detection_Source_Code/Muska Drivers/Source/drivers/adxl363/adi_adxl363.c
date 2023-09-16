/*!
 *****************************************************************************
 * @file:    adi_adxl363.c
 * @brief:   Driver source for ADXL363 accelerometer 
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

/*! \addtogroup ADXL363_Driver ADXL363 Driver
 *  @{
 * @brief ADXL363 Accelerometer Driver (ADXL363) 
 * @details This driver manages the ADXL363 accelerometer.  The accelerometer is 
 *          off-chip and requires some hardware knowledge to configure the
 *          driver correctly for use.  GPIO connections are required if the 
 *          ADXL363-generated interrupts are required.  Additionally the connected 
 *          SPI device number and chip select must be known.  
 * @note This driver requires the SPI driver. The application must include the SPI driver sources to avoid link errors.
 * @note This driver requires the DMA driver. The application must include the DMA driver sources to avoid link errors.
 * @note This driver requires the GPIO driver. The application must include the GPIO driver sources to avoid link errors.
 * @note The application must include drivers/adxl363/adi_adxl363.h to use this driver
 */
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include <drivers/adxl363/adi_adxl363.h>
#include <drivers/spi/adi_spi.h>
#include "adi_adxl363_def.h"

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
* Pm073 (rule 14.7): a function should have a single point of exit.
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function.
*   Multiple returns are used for error handling.
*/
#pragma diag_suppress=Pm140,Pm073,Pm143
#endif /* __ICCARM__ */

#ifdef VERIFY_REGISTERS
/* Local function to query the state of the ADXL363 */
uint32_t DumpRegisters(ADI_ADXL363_DEVICE * pDevice);
#endif /* VERIFY_REGISTERS */

/* Check all the static defines - essential for use with this driver. */
static  ADI_ADXL363_RESULT Init_adxl363(ADI_ADXL363_DEVICE * pDevice);

/* ADXL363 instance information */
static ADI_ADXL363_DEVICE    gADXL363Dev[] =
{
    {
      ADI_ADXL363_STATE_NOT_OPENED,
      NULL
    }
    /* Add more instances here */
};

/*
 * function pointers for accessing the device
 */
#ifdef ADI_DEBUG
/* Function to validate the given device handle */
static ADI_ADXL363_RESULT ValidateHandle(
    ADI_ADXL363_DEVICE      *pDevice
);

#ifdef VERIFY_REGISTERS
#include <stdio.h>
uint32_t DumpRegisters(ADI_ADXL363_DEVICE * pDevice)
{
    uint8_t RegValue;

    ADXL363_REG_READ(ADI_ADXL363_ADI_DEVID, &RegValue)       printf("ADI_ADXL363_ADI_DEVID          = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_MEMS_DEVID, &RegValue)      printf("ADI_ADXL363_MEMS_DEVID         = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DEVID, &RegValue)           printf("ADI_ADXL363_DEVID              = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_REVID, &RegValue)           printf("ADI_ADXL363_REVID              = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_XDATA, &RegValue)           printf("ADI_ADXL363_XDATA              = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_YDATA, &RegValue)           printf("ADI_ADXL363_YDATA              = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_ZDATA, &RegValue)           printf("ADI_ADXL363_ZDATA              = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_STATUS, &RegValue)          printf("ADI_ADXL363_STATUS             = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_FIFO_ENTRIES_L, &RegValue)  printf("ADI_ADXL363_FIFO_ENTRIES_L     = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_FIFO_ENTRIES_H, &RegValue)  printf("ADI_ADXL363_FIFO_ENTRIES_H     = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAX_L, &RegValue)         printf("ADI_ADXL363_DATAX_L            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAX_H, &RegValue)         printf("ADI_ADXL363_DATAX_H            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAY_L, &RegValue)         printf("ADI_ADXL363_DATAY_L            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAY_H, &RegValue)         printf("ADI_ADXL363_DATAY_H            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAZ_L, &RegValue)         printf("ADI_ADXL363_DATAZ_L            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAZ_H, &RegValue)         printf("ADI_ADXL363_DATAZ_H            = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_TEMP_DATA_L, &RegValue)     printf("ADI_ADXL363_TEMP_DATA_L        = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_TEMP_DATA_H, &RegValue)     printf("ADI_ADXL363_TEMP_DATA_H        = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAADC_L, &RegValue)       printf("ADI_ADXL363_DATAADC_L          = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_DATAADC_H, &RegValue)       printf("ADI_ADXL363_DATAADC_H          = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_SRESET, &RegValue)          printf("ADI_ADXL363_SRESET             = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_THRESH_ACT_L, &RegValue)    printf("ADI_ADXL363_THRESH_ACT_L       = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_THRESH_ACT_H, &RegValue)    printf("ADI_ADXL363_THRESH_ACT_H       = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_TIME_ACT, &RegValue)        printf("ADI_ADXL363_TIME_ACT           = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_THRESH_INACT_L, &RegValue)  printf("ADI_ADXL363_THRESH_INACT_L     = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_THRESH_INACT_H, &RegValue)  printf("ADI_ADXL363_THRESH_INACT_H     = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_TIME_INACT_L, &RegValue)    printf("ADI_ADXL363_TIME_INACT_L       = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_TIME_INACT_H, &RegValue)    printf("ADI_ADXL363_TIME_INACT_H       = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &RegValue)   printf("ADI_ADXL363_ACT_INACT_CTL      = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_FIFO_CTL, &RegValue)        printf("ADI_ADXL363_FIFO_CTL           = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_FIFO_SAMPLES, &RegValue)    printf("ADI_ADXL363_FIFO_SAMPLES       = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_INT1_MAP, &RegValue)        printf("ADI_ADXL363_INT1_MAP           = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_INT2_MAP, &RegValue)        printf("ADI_ADXL363_INT2_MAP           = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_FILTER_CTL, &RegValue)      printf("ADI_ADXL363_FILTER_CTL         = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_POWER_CTL, &RegValue)       printf("ADI_ADXL363_POWER_CTL          = 0x%02x\n", RegValue);
    ADXL363_REG_READ(ADI_ADXL363_SELF_TEST, &RegValue)       printf("ADI_ADXL363_SELF_TEST          = 0x%02x\n", RegValue);

    return ADI_ADXL363_SUCCESS;
}
#endif

#endif /* ADI_DEBUG */
/*! \endcond */

/**
 * @brief       Opens the ADXL363 driver.
 *
 * @details     This opens the ADXL363 driver and upon success returns the handle
 *              to the driver. All the other APIs accept this handle as the parameter
 *              which determines the instance of the driver.
 *
 *              The ADXL363 uses SPI for communicating with the host processor.
 *
 *              After the ADXL363 driver is opened, the ADXL363 will be in standby
 *              mode. Once the required configuration is done, the API
 *              adi_adxl363_Enable should be called to enable the measurement.
 *
 *
 * @param [in]  pDevMemory          Memory required for ADXL363 device operation.
 *
 * @param [in]  nMemorySize         The size of the memory passed in bytes.
 *
 * @param [out] phDevice            Pointer to a location where the device handle
 *                                  is written.
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                      If successfully opened the device.
 *
 *  - #ADI_ADXL363_NULL_POINTER             [D] If one of the given pointers are
 *                                              pointing to NULL.
 *
 *  - #ADI_ADXL363_INSUFFICIENT_MEMORY      [D] If the given memory is not sufficient
 *                                              to operate the device.
 *
 *  - #ADI_ADXL363_DEVICE_ALREADY_OPENED    [D] If the given device is already opened.
 *
 *
 * @sa          adi_adxl363_Close()
 * @sa          adi_adxl363_Enable()
 */
ADI_ADXL363_RESULT adi_adxl363_Open(void                            *pDevMemory, 
                                    uint32_t                        nMemorySize, 
                                    ADI_ADXL363_HANDLE *            phDevice)
{
    /* Pointer to the ADXL363 device */
    ADI_ADXL363_DEVICE      *pDevice;
    /* Pointer to the ADXL363 instance information */
    ADI_ADXL363_INFO        *pDevInfo;
    /* Spi enumeration return */
    ADI_SPI_RESULT          eSpiResult; 
    /* This API return */
    ADI_ADXL363_RESULT      eResult = ADI_ADXL363_SUCCESS;
    
#ifdef ADI_DEBUG

    /* Check if the given memory is sufficient to operate at least in interrupt mode */
    if(nMemorySize < ADI_ADXL363_MEMORY_SIZE)
    {
        return ADI_ADXL363_INSUFFICIENT_MEMORY;
    }

    /* Check if the given pointer parameters are valid. */
    if((pDevMemory == NULL) || (phDevice == NULL))
    {
        return ADI_ADXL363_NULL_POINTER;
    }

    /* Check if the given device instance is already opened */
    if(gADXL363Dev[0].eState != ADI_ADXL363_STATE_NOT_OPENED)
    {
        return ADI_ADXL363_DEVICE_ALREADY_OPENED;
    }
    
    assert(ADI_ADXL363_MEMORY_SIZE == sizeof(ADI_ADXL363_INFO));
#endif /* ADI_DEBUG */

    pDevice = &gADXL363Dev[0];
 
    /* Fragment the given memory for various needs */
    pDevice->pDevInfo   =   (ADI_ADXL363_INFO * )pDevMemory;
    pDevInfo            =   pDevice->pDevInfo;

    /* Clear the Device Information structure */
    memset((void *)pDevice->pDevInfo, 0, sizeof(ADI_ADXL363_INFO));
    
    /* Open the SPI */
    eSpiResult = adi_spi_Open(ADXL363_CFG_SPI_DEV_NUM, 
                              pDevInfo->aDeviceMemory, 
                              ADI_SPI_MEMORY_SIZE, 
                              &(pDevInfo->hSPIDevice));
    
    if(eSpiResult == ADI_SPI_SUCCESS) {
      
      /* Set the chip select */
      eSpiResult = adi_spi_SetChipSelect(pDevInfo->hSPIDevice, 
                                         ADXL363_CFG_SPI_DEV_CS);
      
      if(eSpiResult == ADI_SPI_SUCCESS) {     
        /* Mark the device as opened */
        pDevice->eState =   ADI_ADXL363_STATE_OPENED;
        
        Init_adxl363(pDevice); 
        
        /* Mark the device as on Standby */
        pDevice->eState =   ADI_ADXL363_STATE_STANDBY;
        
        /* return the handle to the opened device*/
        *phDevice = pDevice;
      }
      else {
        /* Unable to set the chip select, so close it all out, and return a failure. */
        eSpiResult = adi_spi_Close(pDevInfo->hSPIDevice);
        eResult = ADI_ADXL363_SPI_DEV_OPEN_FAILED;
      }
    }
    else {
       eResult = ADI_ADXL363_SPI_DEV_OPEN_FAILED;
    }
 
    return eResult;
}


/**
 * @brief       Closes the ADXL363 driver.
 *
 * @details     This function stops the measurement before closing the device, this
 *              puts the ADXL363 into standby mode.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully closed the ADXL363
 *                                          device.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE    [D]    If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED [D]    if the given device is not yet opened.
 *
 *  - #ADI_ADXL363_GPIO_FAILED              Failed to unregister the callback
 *                                          with GPIO service.
 *
 * @sa          adi_adxl363_Open()
 */
ADI_ADXL363_RESULT adi_adxl363_Close (
    ADI_ADXL363_HANDLE         hDevice
)
{        
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* Pointer to device information structure */
    ADI_ADXL363_INFO        *pDevInfo;
    
#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

#endif /* ADI_DEBUG */

    /* Get the pointer to device information */
    pDevInfo    =   pDevice->pDevInfo;
    
    /* Change the state to standby */
    pDevice->eState = ADI_ADXL363_STATE_STANDBY;

    /* Reset the ADXL363 */
    ADXL363_REG_WRITE(ADI_ADXL363_SRESET, ADI_ADXL363_RESET_KEY)
      
    /* Unregister the interrupt handler for GPIO pins */
    /* If this GPIO line has been setup for use, then unregister the
       callback for it. */
    if(pDevInfo->IntGPIOInfo[0].eIRQGrp != 0u) {
      if(adi_gpio_RegisterCallback(pDevInfo->IntGPIOInfo[0].eIRQGrp,
                                   NULL,
                                   NULL) != ADI_GPIO_SUCCESS)
      {
        return ADI_ADXL363_GPIO_FAILED;
      }
    }

    /* If this GPIO line has been setup for use, then unregister the
       callback for it. */
    if(pDevInfo->IntGPIOInfo[0].eIRQGrp != 0u) {
      if(adi_gpio_RegisterCallback(pDevInfo->IntGPIOInfo[1].eIRQGrp,
                                   NULL,
                                   NULL) != ADI_GPIO_SUCCESS)
      {
        return ADI_ADXL363_GPIO_FAILED;
      }
    }

    if(ADI_SPI_SUCCESS != adi_spi_Close(pDevInfo->hSPIDevice)) {
        return ADI_ADXL363_SPI_DEV_FAILED;
    }
    
    /*
     *  Mark the device as closed
     */
    pDevice->eState =   ADI_ADXL363_STATE_NOT_OPENED;

    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Register a callback for the ADXL363.
 *
 * @details     This API registers a callback for the ADXL363.  Only one callback
 *              is registered, even if both Interrupt pins are used. 
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  pfCallback          Application supplied callback function which
 *                                  will be called by the driver to notify interrupts
 *                                  from the accelerometer.
 * \n
 *                                  Upon getting the interrupt callback, the application
 *                                  can use the API #adi_adxl363_GetIntStatus to know
 *                                  what caused the interrupt. There is only one callback 
 *                                  for the ADXL363 device, so the device must be queried
 *                                  to establish which event occurred.
 *
 * @param [in]  pCBParam            Application supplied callback parameter, which
 *                                  will be passed back when the callback function
 *                                  is called.
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully registered the callback.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE    [D]    If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED [D]    If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION [D]    If trying to register a callback once 
 *                                          the device is already enabled and measuring.
 *
 * @sa          adi_adxl363_Open()
 */
ADI_ADXL363_RESULT adi_adxl363_RegisterCallback(ADI_ADXL363_HANDLE              hDevice,
                                                ADI_CALLBACK                    pfCallback,
                                                void *                          pCBParam)
{
    ADI_INT_STATUS_ALLOC();
      
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;
    ADI_ADXL363_RESULT      eResult = ADI_ADXL363_SUCCESS;
    
#ifdef ADI_DEBUG
    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if measurement is enabled - do not reconfire while measuring */
    if(pDevice->eState == ADI_ADXL363_STATE_MEASURING)
    { 
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */
    
    ADI_ENTER_CRITICAL_REGION();
    /* Save the given parameters */
    pDevice->pDevInfo->pfCallback        =   pfCallback;
    pDevice->pDevInfo->pCBParam          =   pCBParam; 
    ADI_EXIT_CRITICAL_REGION();
    
    return eResult;
}
                                      
/**
 * @brief       Enable or disable the ADXL363 measurement.
 *
 * @details     By default the device is opened in standby mode. After the
 *              required configuration is done, this API can be used to start
 *              measuring.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_Enable(
    ADI_ADXL363_HANDLE              hDevice,
    bool                            bEnable
)
{  
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* POWER_CTL register contents */
    uint8_t                 PwrCtlData = 0u;

    /* Pointer to device instance information */
    ADI_ADXL363_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    /* Check if measurement is already enabled when trying to enable it */
    if((bEnable == true) && (pDevice->eState == ADI_ADXL363_STATE_MEASURING))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
    /* Check if measurement already disabled when trying to disable it */
    if((bEnable == false) && (pDevice->eState == ADI_ADXL363_STATE_STANDBY))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */
    
    /* Get the pointer to device instance information */
    pDevInfo = pDevice->pDevInfo;

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_POWER_CTL, &PwrCtlData)

    if(bEnable == true) {
        /* DATA_FORMAT register contents */
        uint8_t                 DataFormat;
        /* Edge sense type */
        ADI_GPIO_IRQ_TRIGGER_CONDITION          eSense;

        /* Read the contents of the Data format register */
        ADXL363_REG_READ(ADI_ADXL363_INT1_MAP, &DataFormat)

        /* Choose the GPIO sensing based on the interrupt polarity */
        eSense = ((DataFormat & BITM_ADXL363_INT_LOW) != 0u) ? \
                   ADI_GPIO_IRQ_FALLING_EDGE : ADI_GPIO_IRQ_RISING_EDGE ;

        /* If this GPIO line has been setup for use, then register the
           callback for it. */
        if(pDevInfo->IntGPIOInfo[0].eIRQGrp != 0u) {
          if(SetupGPIO(pDevInfo,
                       &pDevInfo->IntGPIOInfo[0],
                       pDevice->pDevInfo->pfCallback,
                       pDevice->pDevInfo->pCBParam,
                       eSense) != ADI_ADXL363_SUCCESS) {
                return ADI_ADXL363_GPIO_FAILED;
            }
        }
        
        /* If this GPIO line has been setup for use, then register the
           callback for it. */
        if(pDevInfo->IntGPIOInfo[1].eIRQGrp != 0u) {
            if(SetupGPIO(pDevInfo,
                       &pDevInfo->IntGPIOInfo[1],
                       pDevice->pDevInfo->pfCallback,
                       pDevice->pDevInfo->pCBParam,
                       eSense) != ADI_ADXL363_SUCCESS) {
                return ADI_ADXL363_GPIO_FAILED;
            }
        }
        
        /* Set the Measure bit to enable measurement */
        PwrCtlData |= (2u << BITP_ADXL363_MEASURE_EN);

        /* Change the state to measuring */
        pDevice->eState = ADI_ADXL363_STATE_MEASURING;
    }
    else {
        /* Clear the Measure bit to put the part into standby mode */
        PwrCtlData &=  ~(BITM_ADXL363_MEASURE_EN);

        /* Change the state to standby */
        pDevice->eState = ADI_ADXL363_STATE_STANDBY;
    }
    
    /* Set the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_POWER_CTL, PwrCtlData)
    
    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Reads the sign-extended, 12-bit,  accelerator output samples in 
 *              blocking mode.
 *
 * @details     This is a blocking read call to get back the accelerator output
 *              samples.  The samples read are the full 12-bit, sign-extended,
 *              samples.
 *
 *              This API should be called only once an event indicating that the
 *              data is ready to be read is notified. Otherwise there may be a
 *              chance of reading old data. Please follow the documentation of
 *              #ADI_ADXL363_EVENT to know which events can be used to determine 
 *              the availability of data.
 *
 *              The accelerator output is read in terms of triplets (data 
 *              corresponding to x, y, z axis). As each sample is 16bit width, the 
 *              size of the buffer should be multiple 6 bytes.
 *
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 * @param [in]  pBuffer             Pointer to the buffer where output samples
 *                                  are written.  The buffer should be 
 *                                  #ADI_ADXL363_DATA_SIZE bytes in length.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully read the data.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to read data when measurement
 *                                          is not enabled.
 *
 *  - #ADI_ADXL363_PENDING_IO           [D] If trying to perform a read when
 *                                          another input/output operation is
 *                                          in progress.
 *
 *  - #ADI_ADXL363_SPI_DEV_FAILED          If communication device related
 *                                          failure occurs.
 */
ADI_ADXL363_RESULT adi_adxl363_ReadData(
    ADI_ADXL363_HANDLE              hDevice,
    void *                          pBuffer
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* Pointer to device instance information */
    ADI_ADXL363_INFO        *pDevInfo;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;
    
    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if measurement is enabled when trying to read the data. */
    if(pDevice->eState != ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    pDevInfo = pDevice->pDevInfo;
    /* Perform device read */
    return SPI_Read (pDevInfo, ADI_ADXL363_DATAX_L, pBuffer, ADI_ADXL363_DATA_SIZE);
}

/**
 * @brief       Sets the output data rate.
 *
 * @details     The output data rates have fixed values, as defined by the
*               #ADI_ADXL363_DATA_RATE enumeration.  
 *
 * @note        The default data rate is 100 Hz.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eDataRate       Data rate to be set.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully set the data rate.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                           invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *
 * @sa adi_adxl363_SetPowerMode()
 *
 */
ADI_ADXL363_RESULT adi_adxl363_SetDataRate (

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ADXL363_DATA_RATE               eDataRate
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* BW_RATE register contents */
    uint8_t                 BWRateData;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;
    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    
    /* Check if measurement is enabled - do not reconfire while measuring */
    if(pDevice->eState == ADI_ADXL363_STATE_MEASURING)
    { 
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */

    /* Read the contents of BW_RATE register */
    ADXL363_REG_READ(ADI_ADXL363_FILTER_CTL, &BWRateData)

    /* Clear the existing data rate */
    BWRateData &=   ~(BITM_ADXL363_ODR);

    /* Set the given data rate */
    BWRateData |=   (uint8_t)eDataRate;

    /* Write back the contents of BW_RATE register */
    ADXL363_REG_WRITE(ADI_ADXL363_FILTER_CTL, BWRateData)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Configures activity detection.
 *
 * @details     Configures the threshold, time  and mode of activity detection.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  nThreshold      Acceleration above this threshold is considered
 *                              to be activity. The scale factor is 62.5 mg/LSB.
 *                              And the valid values are from 1 to 0xFF which
 *                              corresponds for 62.5 mg to 16g. A value of 0 may
 *                              result in undesirable behavior.
 *
 * @param [in]  nTime          Number of consecutive samples that must have at least
 *                             one axis greater than the activity threshold (set by nThreshold)
 *                             for an activity event to be detected.  Time = nTime/ODR where
 *                              ODR is output data rate ( Please see #adi_adxl363_SetDataRate)
 *
 * @param [in]  eMode          Mode of activity detection function.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully configured the activity
 *                                          detection.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_ACT_THRESH   [D] If the given activity threshold is
 *                                          outside the valid range.
 *
 * @sa  adi_adxl363_ConfigInactivity()
 */
ADI_ADXL363_RESULT adi_adxl363_ConfigActivity (

    ADI_ADXL363_HANDLE                  hDevice,
    uint16_t                            nThreshold,
    uint8_t                             nTime,
    ADI_ADXL363_DETECTION_MODE          eMode
)
{  
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;


#ifdef ADI_DEBUG

    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if the given activity detection threshold is valid */
    if((nThreshold < ADXL363_MIN_ACT_THRESH) || (nThreshold > ADXL363_MAX_ACT_THRESH ))
    {
        return ADI_ADXL363_INVALID_ACT_THRESH;
    }
    
    /* Check if measurement is enabled - do not reconfire while measuring */
    if(pDevice->eState == ADI_ADXL363_STATE_MEASURING)
    { 
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */
    uint8_t nTemp = (uint8_t)nThreshold;
    /* Read the contents of Activity & Inactivity control register */
    ADXL363_REG_WRITE(ADI_ADXL363_THRESH_ACT_L, nTemp)
    nTemp = (uint8_t)(nThreshold >> 8);
    ADXL363_REG_WRITE(ADI_ADXL363_THRESH_ACT_H, nTemp)

    ADXL363_REG_WRITE(ADI_ADXL363_TIME_ACT, nTime)
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &nTemp)
    nTemp = nTemp | (uint8_t)((uint8_t)eMode << BITP_ADXL363_ACT_REF);
    ADXL363_REG_WRITE(ADI_ADXL363_ACT_INACT_CTL, nTemp)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Enable  activity detection.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable  activity detection.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to reconfigure while already 
 *                                          measuring.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_EnableActivity(

    ADI_ADXL363_HANDLE              hDevice,
    bool                            bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* ADI_ADXL363_ACT_INACT_CTL register contents */
    uint8_t                 ActCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't change while measuring. */
    if((pDevice->eState == ADI_ADXL363_STATE_MEASURING))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &ActCtlData)

    if(bEnable == true)
    {

        ActCtlData |= BITM_ADXL363_ACT_EN;
    }
    else
    {
        /* Clear the Measure bit to put the part into standby mode */
        ActCtlData &=  ~(BITM_ADXL363_ACT_EN);
    }

    /* Get the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_ACT_INACT_CTL, ActCtlData)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Configures inactivity detection.
 *
 * @details     Configures the threshold, time, mode of operation.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  nThreshold      Acceleration below this threshold is considered
 *                              to be inactivity. The scale factor is 62.5 mg/LSB.
 *                              And the valid values are from 1 to 0xFF which
 *                              corresponds for 62.5 mg to 16g. A value of 0 may
 *                              result in undesirable behavior.
 *
 *
 * @param [in]  nTime           Number of consecu-tive samples that must have all axes
 *                              lower than the inactivity threshold (nThreshold)
 *                              for an inactivity event to be detected.
 *
 * @param [in]  eMode          Inactivity detection function operation mode
 * \n
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully configured the
 *                                          inactivity detection.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_INACT_THRESH [D] If the given inactivity threshold
 *                                          is outside the valid range.
 *
 *  - #ADI_ADXL363_INVALID_AXIS         [D] If the given participating axes
 *                                          mask is invalid.
 *
 * @sa  adi_adxl363_ConfigActivity()
 */
ADI_ADXL363_RESULT adi_adxl363_ConfigInactivity (

    ADI_ADXL363_HANDLE                  hDevice,
    uint16_t                             nThreshold,
    uint16_t                             nTime,
    ADI_ADXL363_DETECTION_MODE           eMode
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;


#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if the given inactivity detection threshold is valid */
    if(nThreshold < ADXL363_MIN_INACT_THRESH)
    {
        return ADI_ADXL363_INVALID_INACT_THRESH;
    }
    
    /* Don't change while measuring. */
    if((pDevice->eState == ADI_ADXL363_STATE_MEASURING))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */

    uint8_t nTemp = (uint8_t)nThreshold;
    /* Read the contents of Activity & Inactivity control register */
    ADXL363_REG_WRITE(ADI_ADXL363_THRESH_INACT_L, nTemp)
    nTemp = (uint8_t)(nThreshold >> 8);
    ADXL363_REG_WRITE(ADI_ADXL363_THRESH_INACT_H, nTemp)
    nTemp = (uint8_t)nThreshold;
    ADXL363_REG_WRITE(ADI_ADXL363_TIME_INACT_L, nTemp)
    nTemp = (uint8_t)(nThreshold >> 8);
    ADXL363_REG_WRITE(ADI_ADXL363_TIME_INACT_H, nTemp)

    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &nTemp)
    nTemp = nTemp | (uint8_t)((uint8_t)eMode << BITP_ADXL363_INACT_REF);
    ADXL363_REG_WRITE(ADI_ADXL363_ACT_INACT_CTL, nTemp)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Enable  Inactivity detection.
 *
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable Incactivity detection.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] Do not reconfigure while measuring.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_EnableInactivity(

    ADI_ADXL363_HANDLE              hDevice,
    bool                            bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* POWER_CTL register contents */
    uint8_t                 InactCtlData = 0u;
    
#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS) {
        return eResult;
    }

    /* Don't change while measuring. */
    if((pDevice->eState == ADI_ADXL363_STATE_MEASURING))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &InactCtlData)

    if(bEnable == true) {

        InactCtlData |= BITP_ADXL363_INACT_EN;
    }
    else {
        /* Clear the Measure bit to put the part into standby mode */
        InactCtlData &=  ~(BITP_ADXL363_INACT_EN);
    }

    /* Get the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_ACT_INACT_CTL, InactCtlData)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Enable the  Wakeup function.
 *
 * @details     Wake-up mode is ideal for simple detection of the presence or absence of motion at extremely low power consumption.
 *              Wake-up mode is useful particularly for implementation of a motion activated on/off switch, allowing the rest of
 *              the system to power down until activity is detected.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable wakeup mode.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_EnableWakeUp(

    ADI_ADXL363_HANDLE              hDevice,
    bool                            bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* POWER_CTL register contents */
    uint8_t                 PwrCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_POWER_CTL, &PwrCtlData)

    if(bEnable == true)
    {
        PwrCtlData |= BITM_ADXL363_WAKE_UP;
    }
    else
    {
        /* Clear the Measure bit to put the part into standby mode */
        PwrCtlData &=  ~(BITM_ADXL363_WAKE_UP);
    }

    /* Get the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_POWER_CTL, PwrCtlData)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief      To  Enable/disable Autosleep mode.
 *
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable measurement.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_EnableAutoSleep(
    ADI_ADXL363_HANDLE              hDevice,
    bool                            bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* POWER_CTL register contents */
    uint8_t                 PwrCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_POWER_CTL, &PwrCtlData)

    if(bEnable == true)
    {
        PwrCtlData |= BITM_ADXL363_AUTO_SLEEP;
    }
    else
    {
        /* Clear the Measure bit to put the part into standby mode */
        PwrCtlData &=  ~(BITM_ADXL363_AUTO_SLEEP);
    }

    /* Get the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_POWER_CTL, PwrCtlData)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Select the antialiasing filter.
 *
 * @details     The user can set this antialiasing filter to a bandwidth
 *              that is at1/2 the output data rate or 1/2 the output data rate.
 *              Setting the antialiasing filter pole to 1/4 of the output data
 *              rate provides less aggressive antialiasing filtering,
 *              but maximizes bandwidth and is adequate for most applications.
 *
 * @param [in]  hDevice       Handle to ADXL363 device.
 *
 * @param [in]  eBandWidth    Select the filter type to be used.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully applied the bandwidth filter.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to applied the bandwidth filter
 *                                          whilst the device is measuring.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 */
ADI_ADXL363_RESULT adi_adxl363_SetFilterBandWidth(
    ADI_ADXL363_HANDLE            hDevice,
    ADI_FILTER_BW  eBandWidth
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* FILTER_CTL register contents */
    uint8_t                 FiltCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    
    /* Only change when in standby. */
    if((pDevice->eState != ADI_ADXL363_STATE_STANDBY))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
    
#endif /* ADI_DEBUG */

    /* Get the current contents of filter control register */
    ADXL363_REG_READ(ADI_ADXL363_FILTER_CTL, &FiltCtlData)
    /* Mask-in the bandwith */
    FiltCtlData &=  ~(BITM_ADXL363_HALF_BW_EN);
    FiltCtlData |= eBandWidth;
    /* Write it back */
    ADXL363_REG_WRITE(ADI_ADXL363_FILTER_CTL, FiltCtlData)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Enable FIFO for storing the temperature data.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable inclusion of temperature data in the 
 *                                  FIFO.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_EnableTemperatureFIFO(

    ADI_ADXL363_HANDLE             hDevice,
    bool                            bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* FIFO_CTL register contents */
    uint8_t                 FifoCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Only change when in standby. */
    if((pDevice->eState != ADI_ADXL363_STATE_STANDBY))
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */
    
    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &FifoCtlData)

    if(bEnable == true) {
        FifoCtlData |= BITM_ADXL363_TEMP_FIFO_EN;
    }
    else {
        /* Clear the Measure bit to put the part into standby mode */
        FifoCtlData &=  ~(BITM_ADXL363_TEMP_FIFO_EN);
    }

    /* Get the current contents of Power control register */
    ADXL363_REG_WRITE(ADI_ADXL363_FIFO_CTL, FifoCtlData)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Set the range for measurement.
 *
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  eRange              The measurement range of the device.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to enable/disable
 *                                          the measurement when it is already
 *                                          enabled/disabled.
 *
 *  - #ADI_ADXL363_DEVICE_ACCESS_FAILED [D] If failed to read/write device registers.
 *
 *  - #ADI_ADXL363_GPIO_FAILED          [D] If failed to register callback with
 *                                          GPIO service.
 */
ADI_ADXL363_RESULT adi_adxl363_SetRange(

    ADI_ADXL363_HANDLE              hDevice,
    ADI_MEASUREMENT_RANGE           eRange
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    /* FILTER_CTL register contents */
    uint8_t                 FilterCtlData = 0u;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
   /* Check if measurement already disabled when trying to disable it */
    if(pDevice->eState == ADI_ADXL363_STATE_STANDBY)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

    /* Check if any device state validation is required before enabling */

#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_FILTER_CTL, &FilterCtlData)
    FilterCtlData &= ~(BITM_ADXL363_RANGE);
    FilterCtlData |= eRange;
    ADXL363_REG_WRITE(ADI_ADXL363_FILTER_CTL, FilterCtlData)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       To get the temperature data from the device.
 *
 * @param [in]  hDevice             Handle to ADXL363 device.
 *
 * @param [in]  nPdata              Pointer to a location where we need to write the temperature data.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully enabled/disabled the
 *                                          measurement.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle is
 *                                          invalid.
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer points to NULL.
 *
 */
ADI_ADXL363_RESULT adi_adxl363_GetTemperature(

    ADI_ADXL363_HANDLE              hDevice,
    uint16_t                        *nPdata
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;
    uint8_t ndata;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    /* Check if measurement already disabled when trying to disable it */
    if(pDevice->eState == ADI_ADXL363_STATE_STANDBY)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
    
    if(nPdata == NULL)
    {
       return ADI_ADXL363_NULL_POINTER;
    }
#endif /* ADI_DEBUG */

    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_TEMP_DATA_H, &ndata)
     *nPdata =  (uint16_t)((uint16_t)ndata << 8u);
    /* Get the current contents of power control register */
    ADXL363_REG_READ(ADI_ADXL363_TEMP_DATA_L, &ndata)
    *nPdata |= ndata;
    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Serializes activity and inactivity detection.
 *
 * @details     Helps to select the Link/Loop mode of operation when the 
 *              activity and inactivity are enabled. 
 *
 * @note        This mode only applies when both Inactivity and Activity are
 *              enabled.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eMode           Mode in which device need to be configured
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully linked activity and
 *                                          inactivity detection.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to reconfigure when
 *                                          measurement is enabled.
 *
 * @sa  adi_adxl363_ConfigInactivity()
 * @sa  adi_adxl363_ConfigActivity()
 */
ADI_ADXL363_RESULT adi_adxl363_ActInactMode (

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ACT_INACT_MODE                  eMode
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

   /* POWER_CTL register contents */
    uint8_t                 InActCtlData;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;
    
    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }
    
#endif /* ADI_DEBUG */

    /* Read the contents of Power control register */
    ADXL363_REG_READ(ADI_ADXL363_ACT_INACT_CTL, &InActCtlData)
    InActCtlData |= (eMode & BITM_ADXL363_LINK_LOOP);
    /* Write back the Power control register contents */
    ADXL363_REG_WRITE(ADI_ADXL363_ACT_INACT_CTL, InActCtlData)
    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Sets interrupt polarity.
 *
 * @details     Interrupt polarity can be set to active high or active low.
 *              By default interrupts are set to be active high.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eIntPin         Specify which interrupt pin to be configured.
 *
 * @param [in]  bActiveLow      Flag which indicates the polarity of the interrupt.
 * \n
 *                              'true'  -   Polarity set to active low.     \n
 *                              'false' -   Polarity set to active high.
 *
 * @note        This function should not be called after measurement is enabled.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully set the interrupt
 *                                          polarity.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to set interrupt polarity
 *                                          when measurement is enabled.
 *
 *  @sa adi_adxl363_Enable()
 */
ADI_ADXL363_RESULT adi_adxl363_SetIntPolarity (

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ADXL363_INTPIN                  eIntPin,
    bool                                bActiveLow
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;


    /* DATA_FORMAT register contents */
    uint8_t                 nIntPolarity;


#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to change the interrupt polarity when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Read the contents of the Data format register */
    ADXL363_REG_READ(ADI_ADXL363_INT1_MAP+eIntPin, &nIntPolarity)

    /* Set the given polarity */
    if(bActiveLow == true)
    {
        nIntPolarity |=  (BITM_ADXL363_INT_LOW);
    }
    else
    {
        nIntPolarity &=  ~(BITM_ADXL363_INT_LOW);
    }

    /* Write back the contents of the Data format register */
    ADXL363_REG_WRITE(ADI_ADXL363_INT1_MAP+eIntPin, nIntPolarity)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Reset the ADXL363 Device.
 *
 * @details     All register settings are cleared, and the sensor is
 *              placed in standby. Interrupt pins are configured to a
 *              high output impedance mode.
 *
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully set self test mode.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  @sa adi_adxl363_Enable()
 */
ADI_ADXL363_RESULT adi_adxl363_ResetDevice(
    ADI_ADXL363_HANDLE                 hDevice
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

   /* DATA_FORMAT register contents */
    uint8_t                 nTemp =ADI_ADXL363_RESET_KEY ;


#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to enable/disable self test when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Write back the contents of the Data format register */
    ADXL363_REG_WRITE(ADI_ADXL363_SRESET, nTemp)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Enables or disables self test mode.
 *
 * @note        This function should not be called after measurement is enabled.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  bEnable         Flag which indicates whether to enable or disable
 *                              self test mode.
 * \n
 *                              'true'  -   Enable self test.       \n
 *                              'false' -   disable self test.
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully set self test mode.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to enable or disable self
 *                                          test when measurement is enabled.
 *
 *  @sa adi_adxl363_Enable()
 */
ADI_ADXL363_RESULT adi_adxl363_EnableSelfTest (
    ADI_ADXL363_HANDLE                  hDevice,
    bool                                bEnable
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

   /* DATA_FORMAT register contents */
    uint8_t                 DataFormat;


#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to enable/disable self test when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Read the contents of the Data format register */
    ADXL363_REG_READ(ADI_ADXL363_SELF_TEST, &DataFormat)

    /* Enable or disable self test mode */
    if(bEnable == true)
    {
        DataFormat |=  (BITM_ADXL363_SELFTEST_EN);
    }
    else
    {
        DataFormat &=  ~(BITM_ADXL363_SELFTEST_EN);
    }

    /* Write back the contents of the Data format register */
    ADXL363_REG_READ(ADI_ADXL363_SELF_TEST, &DataFormat)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Configures the FIFO operation.
 *
 * @details     Configures FIFO mode and  number of samples.
 *
 * @note        This function should not be called after measurement is enabled.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eFIFOMode       The FIFO mode of operation.
 *
 * @param [in]  nSamples        The number of samples in the FIFO. The meaning of
 *                              samples varies depending upon the FIFO mode as
 *                              explained above.
 *\n
 *                              Valid values are from 0 to 511.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully configured FIFO.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to configure FIFO when
 *                                          measurement is enabled.
 *
 *  - #ADI_ADXL363_INVALID_FIFO_SAMPLES [D] If the given number of FIFO samples
 *                                          is not in the valid range.
 */
ADI_ADXL363_RESULT adi_adxl363_ConfigFIFO(

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ADXL363_FIFO_MODE               eFIFOMode,
    uint16_t                            nSamples
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

    uint8_t nTemp;
    /* FIFO_CTL register contents */
    uint8_t                 FIFOCtlData;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to configure FIFO when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

    /* check if the given number of FIFO samples are within the valid range. */
     if(nSamples > ADI_ADXL363_MAX_FIFO_SAMPLES)
    {
        return ADI_ADXL363_INVALID_FIFO_SAMPLES;
    }
#endif /* ADI_DEBUG */

    /* As all the fields of the FIFO control register are set it is not required
     * to do read-modify-write */
    ADXL363_REG_READ(ADI_ADXL363_FIFO_CTL, &FIFOCtlData)
    FIFOCtlData &= BITM_ADXL363_TEMP_FIFO_EN;
    FIFOCtlData |=    (uint8_t)eFIFOMode;
    nTemp = (uint8_t)((nSamples & FIFO_HALF_MARK_LIMIT) >> 8);
    FIFOCtlData |= (uint8_t)(nTemp <<  BITP_ADXL363_FIFO_HALF);
    /* Write to the FIFO control register */
    ADXL363_REG_WRITE(ADI_ADXL363_FIFO_CTL, FIFOCtlData)
    /* Write to the FIFO control register */
    FIFOCtlData = (uint8_t)nSamples;
    ADXL363_REG_WRITE(ADI_ADXL363_FIFO_SAMPLES, FIFOCtlData)

    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Set the interrupt map for the speicified interrupt pin. This also
 *              enables the specified interrupt.
 *
 *
 * @note        This function should not be called after device is enabled.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eIntPin         Specify which interrupt pin to be configured.
 *
 * @param [in]  nIntMap         Interrupt map to be set. The map can be generated
 *                              by logically orring the interrupts (required
 *                              to be mapped to interrupt 2) that
 *                              are enumerated by #ADI_ADXL363_INT.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully set the interrupt map.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to set interrupt map
 *                                          after measurement is enabled.
 *
 * @sa  adi_adxl363_SetIntMask()
 * @sa  adi_adxl363_GetIntMask()
 */
ADI_ADXL363_RESULT adi_adxl363_SetIntMap (

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ADXL363_INTPIN                  eIntPin,
    uint8_t                             nIntMap
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;


    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to set the interrupt map when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
    }

#endif /* ADI_DEBUG */

    /* Write to the interrupt map register */
    ADXL363_REG_WRITE(ADI_ADXL363_INT1_MAP, nIntMap)

    return ADI_ADXL363_SUCCESS;
}

/**
 * @brief       Gets the current interrupt mask for the specified interrupt pin.
 *
 * @details     This API can be used to retrieve the interrupt mask the driver
 *              has set, when application has to eanble/disable a particular interrupt.
 *              The mask is used to map requireed interrupts to specific output
 *              pins.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [in]  eIntPin         Indicate whether retrieve the interrupt mask of INT1 or INT2.
 *
 * @param [out] pnIntMask       Pointer to a location where the interrupt mask
 *                              is written.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully obtained the
 *                                          interrupt mask.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer points to NULL.
 *
 * @sa  adi_adxl363_SetIntMask()
 * @sa  adi_adxl363_GetIntMap()
 */
ADI_ADXL363_RESULT adi_adxl363_GetIntMap (

    ADI_ADXL363_HANDLE                  hDevice,
    ADI_ADXL363_INTPIN                  eIntPin,
    uint8_t *                           pnIntMask
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

 #ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    
    if(pnIntMask == NULL)
    {
      return ADI_ADXL363_NULL_POINTER;
    }
#endif /* ADI_DEBUG */

    /* Read the interrupt mask register */
    ADXL363_REG_READ(ADI_ADXL363_INT1_MAP+eIntPin, pnIntMask)

    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Gets the source(s) for generating an interrupt, also clears 
 *              the Activity and Inactivity interrupt sources.
 *
 * @details     This API can be used to know the interrupt source that caused
 *              a prior interrupt. This API is recommended to be called after
 *              an interrupt is notified to the application.  This API also 
 *              clears Activity and Inactivity interrupt sources, which is the 
 *              behavior of the hardware when reading the interrupt status register.
 *\n
 *              Based on the interrupt source, the application may have to obtain
 *              additional information regarding the interrupt. The applicable
 *              APIs for each interrupt source are documented in the enumeration
 *              #ADI_ADXL363_INT.
 *
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [out] pnIntSource     Pointer to a location where the interrupt source(s)
 *                              are written. A bit mask of interrupts enumerated
 *                              by ADI_ADXL363_INT is written into the given
 *                              location. There can be more than one interrupt
 *                              source that caused the interrupt. The
 *                              corresponding bit is set to 1 for all the
 *                              interrupt sources that caused the interrupt.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully obtained the
 *                                          interrupt source(s).
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer points to NULL.
 *
 * @sa  adi_adxl363_SetIntMask()
 * @sa  adi_adxl363_GetIntMap()
 */
ADI_ADXL363_RESULT adi_adxl363_GetIntStatus(
    ADI_ADXL363_HANDLE            hDevice,
    uint8_t *                     pnIntSource
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }
    
    if(pnIntSource == NULL)
    {
      return ADI_ADXL363_NULL_POINTER;
    }
#endif /* ADI_DEBUG */

    /* Read the interrupt source register */
    ADXL363_REG_READ(ADI_ADXL363_STATUS, pnIntSource)

    return ADI_ADXL363_SUCCESS;
}
/**
 * @brief       Sets the processor GPIO pin to which the ADXL363 interrupt pin is connected.
 *
 * @details     ADXL363 has two interrupt pins (INT1 and INT2) which can be
 *              connected to the host processor for notifying the events.
 *              Depending upon the board design, the ADXL363 interrupt pins can
 *              be connected to different GPIO pins on the host processor. This
 *              API can be used to specify the GPIO pin for which each of the
 *              interrupt is connected. This API should be called separately
 *              (can be twice) to specify the GPIO pin for each of the ADXL363
 *              interrupt pins.
 *
 * @note        This function should not be called after data flow is enabled.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 * @param [in]  eIrqGrp         Indicate whether the interrupt need to be mapped to GROUP-A
                                OR to GROUP-B.
 * @param [in]  eIntPin         ADXL363 interrupt pin to be configured.
 *
 * @param [in]  eGPIOPort       GPIO port of the host processor to which the
 *                              given ADXL363 interrupt pin is connected.
 *
 * @param [in]  nGPIOPin        GPIO pin within the given port to which the
 *                              given ADXL363 interrupt pin is connected.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully mapped GPIO pin
 *                                          for interrupt pin.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_INVALID_OPERATION    [D] If trying to configure GPIO pin for
 *                                          an interrupt after measurement is enabled.
 *
 * @sa  adi_adxl363_SetIntMask()
 * @sa  adi_adxl363_GetIntMap()
 */
ADI_ADXL363_RESULT adi_adxl363_ConfigIntPin (
    ADI_ADXL363_HANDLE                  hDevice,
    ADI_GPIO_IRQ                        eIrqGrp,
    ADI_ADXL363_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    ADI_GPIO_DATA                       nGPIOPin
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;


    /* Pointer to GPIO pin and Interrupt pin mapping structure */
    ADI_ADXL363_GPIO_INFO   *pIntGPIOInfo;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to configure GPIO when measurement is enabled. */
    if(pDevice->eState  ==  ADI_ADXL363_STATE_MEASURING)
    {
        return ADI_ADXL363_INVALID_OPERATION;
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
    pIntGPIOInfo->eIRQGrp = eIrqGrp;

    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Gets the number of entries held in the FIFO.
 *
 * @details     Gets the number of entries in the FIFO and also returns the
 *              trigger status. The trigger event is notified via the callback.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [out] pnEntries       Pointer to a location where the number of entries
 *                              in the FIFO are written.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully obtained the FIFO
 *                                          status.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer to points to NULL.
 *
 *
 * @sa  adi_adxl363_ConfigFIFO()
 */
ADI_ADXL363_RESULT adi_adxl363_GetEntriesInFIFO(

    ADI_ADXL363_HANDLE                  hDevice,
    uint16_t *                          pnEntries
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;


    /* FIFO_STATUS register contents */
    uint8_t                 nNumEntriesL;
    uint8_t                 nNumEntriesH;
    

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if the given pointers are valid */
    if(pnEntries == NULL)
    {
        return ADI_ADXL363_NULL_POINTER;
    }

#endif /* ADI_DEBUG */
    
    /* Read the contents of FIFO status register */
    ADXL363_REG_READ(ADI_ADXL363_FIFO_ENTRIES_H, &nNumEntriesH)
    ADXL363_REG_READ(ADI_ADXL363_FIFO_ENTRIES_L, &nNumEntriesL)
   /* Get the number of FIFO entries */
    *pnEntries  =  ((uint16_t)((uint16_t)nNumEntriesH & 0x03u) << 0x08u) + (uint16_t)(nNumEntriesL);
    return ADI_ADXL363_SUCCESS;
}


/**
 * @brief       Gets the FIFO data.
 *
 * @details     Gets the data held in the FIFO.
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [out] pData           Pointer to a location where the data
 *                              in the FIFO are to be written.  FIFO samples also
 *                              contain information on the data type -
 *                               bits[15:14] of any 16bit sample contains:
 *                              00 = x-axis data
 *                              01 = y-axis data
 *                              10 = z-axis data
 *                              11 = temperature data, if temperature is enabled
 *                                   with FIFO data collection.
 * 
 * @param [out] nSamples        Number of samples to read from FIFO.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully obtained the FIFO
 *                                          data.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer to data is NULL.
 * 
 *  - #ADI_ADXL363_INVALID_FIFO_SAMPLES [D] If the given number of FIFO samples
 *                                          is not in the valid range.
 *
 *
 * @sa  adi_adxl363_ConfigFIFO()
 * @sa  adi_adxl363_GetEntriesInFIFO()
 * @sa  adi_adxl363_EnableTemperatureFIFO()
 */
ADI_ADXL363_RESULT adi_adxl363_ReadFIFO(ADI_ADXL363_HANDLE                  hDevice,
                                        void                               *pData,
                                        uint16_t                            nSamples)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;

#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if the given pointers are valid */
    if(pData == NULL)
    {
        return ADI_ADXL363_NULL_POINTER;
    }
    
    if(nSamples > ADI_ADXL363_MAX_FIFO_SAMPLES)
    {
       return ADI_ADXL363_INVALID_FIFO_SAMPLES;
    }
    
    if(nSamples == 0u)
    {
       return ADI_ADXL363_INVALID_FIFO_SAMPLES;
    }
#endif /* ADI_DEBUG */

    /* Read the contents of FIFO status register */
    return SPI_FifoRead(pDevice->pDevInfo, pData, nSamples);
}

/**
 * @brief       Gets the contents of a register on the ADXL363 .
 *
 * @param [in]  hDevice         Handle to ADXL363 device.
 *
 * @param [out] nAddress        Specify the register to be read.
 *
 * @param [out] pData           Specify the address where data need to be written.
 *
 * @param [out] nSize           Specify the size of data to be that need to be read.
 *                              If more than 1 byte is requested, subsequent registers
 *                              will be read as part of the same transfer.
 *
 * @return      Status
 *
 *  - #ADI_ADXL363_SUCCESS                  If successfully obtained the FIFO
 *                                          status.
 *
 *  - #ADI_ADXL363_INVALID_HANDLE       [D] If the given ADXL363 device handle
 *                                          is invalid.
 *
 *  - #ADI_ADXL363_DEVICE_NOT_OPENED    [D] If the given device is not yet opened.
 *
 *  - #ADI_ADXL363_NULL_POINTER         [D] If the given pointer points to NULL.
 *
 *
 * @sa  adi_adxl363_ConfigFIFO()
 */
ADI_ADXL363_RESULT adi_adxl363_ReadRegister (
    ADI_ADXL363_HANDLE                 hDevice,
    uint32_t                            nAddress,
    uint8_t                            *pData,
    uint8_t                             nSize
)
{
    /* Pointer to the ADXL363 Device data */
    ADI_ADXL363_DEVICE      *pDevice = (ADI_ADXL363_DEVICE *) hDevice;
#ifdef ADI_DEBUG
    /* ADXL363 result code */
    ADI_ADXL363_RESULT      eResult;

    /* Validate the given device handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_ADXL363_SUCCESS)
    {
        return eResult;
    }

    /* Check if the given pointers are valid */
    if(pData == NULL)
    {
        return ADI_ADXL363_NULL_POINTER;
    }

#endif /* ADI_DEBUG */
    /* Read the contents of required register */
    ADXL363_REG_READ((uint8_t)nAddress, pData)
    return ADI_ADXL363_SUCCESS;
}

/* Sets up the GPIO for Accelerometer interrupt 1 and 2 */
static ADI_ADXL363_RESULT SetupGPIO (
    ADI_ADXL363_INFO       *pDevInfo,
    ADI_ADXL363_GPIO_INFO  *pGPIOInfo,
    ADI_CALLBACK            pfCallback,
    void *                  pCBParam,
    ADI_GPIO_IRQ_TRIGGER_CONDITION          eSense
)
{
     /* Setup the GPIO pin direction to input */
    if(adi_gpio_InputEnable(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin,
            true) != ADI_GPIO_SUCCESS)
    {
        return ADI_ADXL363_GPIO_FAILED;
    }

     /* Setup the GPIO pin direction to input */
    if(adi_gpio_SetGroupInterruptPins(
            pGPIOInfo->ePort,
            pGPIOInfo->eIRQGrp,
            pGPIOInfo->nPin) != ADI_GPIO_SUCCESS)
    {
        return ADI_ADXL363_GPIO_FAILED;
    }

    if(adi_gpio_SetGroupInterruptPolarity(
            pGPIOInfo->ePort,
            pGPIOInfo->nPin) != ADI_GPIO_SUCCESS)
    {
        return ADI_ADXL363_GPIO_FAILED;
    }

    /* Setup the GPIO pin direction to input */
    if(adi_gpio_RegisterCallback(
            pGPIOInfo->eIRQGrp,
            pfCallback,
            pCBParam) != ADI_GPIO_SUCCESS)
    {
        return ADI_ADXL363_GPIO_FAILED;
    }

    return ADI_ADXL363_SUCCESS;
}


/*! \cond PRIVATE */


/*==========================================================================
     Device register access functions.
 *=========================================================================*/

/* Function to read the FIFO data using SPI interface. */
static ADI_ADXL363_RESULT SPI_FifoRead(ADI_ADXL363_INFO     *pDevInfo,
                                       void                 *pBuffer,
                                       uint16_t              nSize)
{
    /* SPI transceiver instance */
    ADI_SPI_TRANSCEIVER Transceiver;
    uint8_t nTemp[2];
    nTemp[0] = 0x0Du;
    nTemp[1] = 0x00u;  /* Essentially unused */

    /* Initialize the transceiver */
    Transceiver.pReceiver           =   pBuffer;
    Transceiver.ReceiverBytes       =   nSize * sizeof(uint16_t);
    Transceiver.nRxIncrement        =   1u;
    Transceiver.pTransmitter        =   nTemp;
    Transceiver.TransmitterBytes    =   1u;
    Transceiver.nTxIncrement        =   1u;
    Transceiver.bDMA                =   true;
    Transceiver.bRD_CTL             =   true;
    
    /* Transmit the sequence */
    if(adi_spi_MasterReadWrite(pDevInfo->hSPIDevice, &Transceiver) != ADI_SPI_SUCCESS)
    {
        return ADI_ADXL363_SPI_DEV_FAILED;
    }

    return ADI_ADXL363_SUCCESS;
}


/* Function to read the data using SPI interface. */
static ADI_ADXL363_RESULT SPI_Read(ADI_ADXL363_INFO     *pDevInfo,
                                    uint8_t               RegAddr,
                                    uint8_t              *pBuffer,
                                    uint16_t              nSize)
{
    /* SPI transceiver instance */
    ADI_SPI_TRANSCEIVER Transceiver;
    uint8_t nTemp[2];
    nTemp[0] = 0xBu;
    nTemp[1] = RegAddr;

    /* Initialize the transceiver */
    Transceiver.pReceiver           =   pBuffer;
    Transceiver.ReceiverBytes       =   nSize;
    Transceiver.nRxIncrement        =   1u;
    Transceiver.pTransmitter        =   nTemp;
    Transceiver.TransmitterBytes    =   sizeof(nTemp);
    Transceiver.nTxIncrement        =   1u;
    Transceiver.bDMA                =   false;   
    Transceiver.bRD_CTL             =   true;
    
    /* Transmit the first sequence */
    if(adi_spi_MasterReadWrite(pDevInfo->hSPIDevice, &Transceiver) != ADI_SPI_SUCCESS)
    {
        return ADI_ADXL363_SPI_DEV_FAILED;
    }

    return ADI_ADXL363_SUCCESS;
}

/* Function to write the data using SPI interface. Assumes single register write */
static ADI_ADXL363_RESULT SPI_Write(ADI_ADXL363_INFO    *pDevInfo,
                                    uint8_t              RegAddr,
                                    uint8_t              Value)
{

    /* SPI transceiver instance */
    ADI_SPI_TRANSCEIVER Transceiver;
    uint8_t                  LocalBuf[3];
    
    LocalBuf[0] = 0x0Au;
    LocalBuf[1] = RegAddr;
    LocalBuf[2] = Value;
    /* Initialize the transceiver */
    Transceiver.pReceiver           =   NULL;
    Transceiver.ReceiverBytes       =   0u;
    Transceiver.nRxIncrement        =   0u;
    Transceiver.TransmitterBytes    =   3u;
    Transceiver.pTransmitter        =   LocalBuf;
    Transceiver.nTxIncrement        =   1u;
    Transceiver.bDMA                =   false;
    Transceiver.bRD_CTL             =   false;
    
    /* Transmit the first sequence */
    if(adi_spi_MasterReadWrite(pDevInfo->hSPIDevice, &Transceiver) != ADI_SPI_SUCCESS)
    {
        return ADI_ADXL363_SPI_DEV_FAILED;
    }

    return ADI_ADXL363_SUCCESS;
}


/*==========================================================================
    Validation functions.
 *=========================================================================*/
#ifdef ADI_DEBUG

static ADI_ADXL363_RESULT ValidateHandle (
    ADI_ADXL363_DEVICE * pDevice
)
{
    uint32_t    nDevIdx;

    for(nDevIdx = 0u; nDevIdx < ADXL363_NUM_INSTANCES; nDevIdx++)
    {
        /* Check if the given device pointer matches any of the  instances */
        if(pDevice == &gADXL363Dev[nDevIdx])
        {
            /* The given pointer matches an instance, now check if it is opened */
            if(pDevice->eState == ADI_ADXL363_STATE_NOT_OPENED)
            {
                return ADI_ADXL363_DEVICE_NOT_OPENED;
            }
            else
            {
                return ADI_ADXL363_SUCCESS;
            }
        }
    }

    return ADI_ADXL363_INVALID_HANDLE;
}
#endif /* ADI_DEBUG */



static  ADI_ADXL363_RESULT  Init_adxl363(ADI_ADXL363_DEVICE * pDevice)
{
  uint32_t i;
  
  ADI_ADXL363_DEVICE_CONFIG aInitData[] =
  {
    {ADI_ADXL363_SRESET,          (ADI_ADXL363_RESET_KEY)},   
    {ADI_ADXL363_THRESH_ACT_L,    (ADXL363_CFG_ACTIVITY_THRESHOLD & 0XFF)},
    {ADI_ADXL363_THRESH_ACT_H,    (ADXL363_CFG_ACTIVITY_THRESHOLD & 0X700)>>8},
    {ADI_ADXL363_TIME_ACT,        (ADXL363_CFG_ACTIVITY_TIME & 0XFF)},
    {ADI_ADXL363_THRESH_INACT_L,  (ADXL363_CFG_INACTIVITY_THRESHOLD & 0XFF)},
    {ADI_ADXL363_THRESH_INACT_H,  (ADXL363_CFG_INACTIVITY_THRESHOLD & 0X700)>>8},
    {ADI_ADXL363_TIME_INACT_L ,   (ADXL363_CFG_INACTIVITY_TIME & 0XFF)},
    {ADI_ADXL363_TIME_INACT_H,    (ADXL363_CFG_INACTIVITY_TIME & 0X700)>>8},
    
    {ADI_ADXL363_ACT_INACT_CTL,   (ADXL363_CFG_ENABLE_ACTIVITY    << BITP_ADXL363_ACT_EN    |
                                   ADXL363_CFG_ACTIVITY_MODE      << BITP_ADXL363_ACT_REF   |
                                   ADXL363_CFG_ENABLE_INACTIVITY  << BITP_ADXL363_INACT_EN  |
                                   ADXL363_CFG_INACTIVITY_MODE    << BITP_ADXL363_INACT_REF |
                                   ADXL363_CFG_LINK_LOOP_MODE     << BITP_ADXL363_LINK_LOOP )},
    
    {ADI_ADXL363_FIFO_CTL,        (ADXL363_CFG_FIFO_MODE    << BITP_ADXL363_FIFO_MODE    |
                                   ADXL363_CFG_ENABLE_TEMPERATURE_FIFO      << BITP_ADXL363_TEMP_FIFO_EN) },
    
    {ADI_ADXL363_FIFO_SAMPLES,    (ADXL363_CFG_FIFO_SIZE & 0X0FF)},
    {ADI_ADXL363_INT1_MAP,        (ADXL363_CFG_INT1_MAP & 0X0FF)}, 
    {ADI_ADXL363_INT2_MAP,        (ADXL363_CFG_INT2_MAP & 0X0FF)},
    {ADI_ADXL363_FILTER_CTL,      (ADXL363_CFG_OUTPUT_DATARATE << BITP_ADXL363_ODR   |
                                   ADXL363_CFG_FILTER_BW << BITP_ADXL363_HALF_BW_EN |
                                   ADXL363_CFG_MEASUREMENT_RANGE <<BITP_ADXL363_RANGE)},
    
    {ADI_ADXL363_POWER_CTL,       (ADXL363_CFG_ENABLE_MEASUREMENT <<  BITP_ADXL363_MEASURE_EN |
                                   ADXL363_CFG_ENABLE_AUTOSLEEP   <<  BITP_ADXL363_AUTO_SLEEP |
                                   ADXL363_CFG_ENABLE_WAKEUP_MODE <<  BITP_ADXL363_WAKE_UP    |
                                   ADXL363_CFG_NOISE_MODE         <<  BITP_ADXL363_LOW_NOISE )},
  };

  for(i=0u;i<sizeof(aInitData)/sizeof(ADI_ADXL363_DEVICE_CONFIG);i++)
  {
        ADXL363_REG_WRITE(aInitData[i].nAddress, aInitData[i].nData)
  }
  
  return(ADI_ADXL363_SUCCESS);
}


/*! \endcond */

/* @} */
