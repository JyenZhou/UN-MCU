/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>

//#include <system.h>
//#include "Common.h"
#include <drivers/i2c/adi_i2c.h>
#include <services/pwr/adi_pwr.h>
#include "HwIfConfig.h"

/* Private typedef ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ADPD_I2C_ADDRESS                            0x64
#define ADUX_I2C_ADDRESS                            0x64
#define ADUX1050_I2C_ADDRESS                        0x2C
#define ADP5061_I2C_ADDRESS                             0x14

#define TWIDEVNUM     (0u)        /* TWI device number */
#define BITRATE       (200u)      /* kHz */
#define DUTYCYCLE     (50u)       /* percent */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ADI_I2C_HANDLE hDevice1;
//static ADI_I2C_HANDLE hDevice2;

uint8_t deviceMemory1[ADI_I2C_MEMORY_SIZE];
//uint8_t deviceMemory2[ADI_I2C_MEMORY_SIZE];
//uint8_t deviceMemory3[ADI_I2C_MEMORY_SIZE];

/* Private function prototypes -----------------------------------------------*/
static ADI_I2C_RESULT I2C1_Init();
ADI_I2C_RESULT I2C2_Init();
static ADI_I2C_RESULT I2C3_Init();

/* Device handle */
ADI_I2C_HANDLE hI2CDevice1;
ADI_I2C_HANDLE hI2CDevice2;

extern void HAL_GPIO_EXT_Callback(void *pCBParam, uint32_t Event, void *pArg);
extern ADI_ADPD_RESULT AdpdEnable(ADI_ADPD_HANDLE const hDevice, bool bEnable);


#ifdef _USING_RTOS_
ADI_OSAL_MUTEX_HANDLE I2C1TransferLock;
#endif //_USING_RTOS_

/* ************************************************************************* */
/**
    * @brief    I2C initialization. This function initiaises all the I2C controllers used.
    * @param    None
    * @retval   None
    */
ADI_I2C_RESULT I2C_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
     eResult = I2C1_Init();  // For communicating with ADPD
     // I2C2_Init();         // For communicating with ADP5258
     // I2C3_Init();         // For communicating with ADP5061
     return eResult;
}

/**
    * @brief    I2C1 init function
    * @param    None
    * @retval   None
    */
ADI_I2C_RESULT I2C1_Init(void) {
        /* open driver */
        if (adi_i2c_Open(TWIDEVNUM, &deviceMemory1, ADI_I2C_MEMORY_SIZE, &hDevice1))
            return ADI_I2C_FAILURE;

        /* check reset flow */
        if (adi_i2c_Reset(hDevice1))
            return ADI_I2C_FAILURE;

#if 1
        /* check set speed API */
        if (adi_i2c_SetBitRate(hDevice1, BITRATE))  // top-end
            return ADI_I2C_FAILURE;
#else
        /* check set speed API */
        if (adi_i2c_SetBitRate(hDevice1, 55000))  // bottom-end
            return ADI_I2C_FAILURE;
#endif

        /* test ADT7420 I2C temperature sensor (U6) on ADuCM4x50 EZ-Kit */

        /* set slave address for ADI ADT7420 */
        if (adi_i2c_SetSlaveAddress(hDevice1, ADUX1050_I2C_ADDRESS))
            return ADI_I2C_FAILURE;
#ifdef _USING_RTOS_
    if (adi_osal_MutexCreate(&I2C1TransferLock) != ADI_OSAL_SUCCESS) {
            return ADI_I2C_FAILURE;
    }
#endif //_USING_RTOS_
     return ADI_I2C_SUCCESS;
}

ADI_I2C_RESULT I2C1_De_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
     if ((eResult = adi_i2c_Close(hDevice1)) != ADI_I2C_SUCCESS) {
         return eResult;
     }
     return eResult;
}

/**
    * @brief    HAL function to write a byte and read specified number of bytes from ADUX1050
    * @param    8-bit data pointer
    * @param    8-bit data pointer
    * @param    16-bit size of the data to read from the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADUX1050_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
    
    /* HW Error result */
    uint32_t hwErrors;    
    /* I2C transaction structure */
    ADI_I2C_TRANSACTION xfr;    
    
    /* do a simple one-byte read of chip ID */
//    prologueData[0]     = 0x0b;  /* address of ID register */
    xfr.pPrologue       = pTxData;
    xfr.nPrologueSize   = 1;
    xfr.pData           = pRxData;
    xfr.nDataSize       = RxSize;
    xfr.bReadNotWrite   = true;
    xfr.bRepeatStart    = true;
    
    
    eResult = adi_i2c_SetSlaveAddress(hDevice1, ADUX1050_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_ReadWrite(hDevice1, &xfr, &hwErrors))) {
        return (ADI_HAL_STATUS_t)eResult;
    }
    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to transmit data through ADUX1050 I2C
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write to the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADUX1050_I2C_Transmit(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

    /* HW Error result */
    uint32_t hwErrors;        
    /* I2C transaction structure */
    ADI_I2C_TRANSACTION xfr;    
    
    /* do a simple one-byte read of chip ID */
//    prologueData[0]     = 0x0b;  /* address of ID register */
    xfr.pPrologue       = &pData[0];
    xfr.nPrologueSize   = 1;
    xfr.pData           = &pData[1];
    xfr.nDataSize       = 2;
    xfr.bReadNotWrite   = false;
    xfr.bRepeatStart    = true;
    eResult = adi_i2c_SetSlaveAddress(hDevice1, ADUX1050_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_ReadWrite(hDevice1, &xfr, &hwErrors))) {
        return (ADI_HAL_STATUS_t)eResult;
    }

    return (ADI_HAL_STATUS_t)eResult;
}


#if 0
// #ifdef _USING_RTOS_
//    INT8U perror;
//    OSMutexDel(I2C1TransferLock, 0, &perror);
// #endif //_USING_RTOS_

/**
    * @brief    I2C2 init function
    * @param    None
    * @retval   None
    */
ADI_I2C_RESULT I2C2_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

     eResult = adi_i2c_Open(TWIDEVNUM,
                            ADI_I2C_MASTER,
                            &deviceMemory2[0],
                            ADI_I2C_MEMORY_SIZE,
                            &hDevice2);
     // DEBUG_RESULT("adi_i2c_Open failed\n",eResult,ADI_I2C_SUCCESS);

     eResult = adi_i2c_SetBitRate(hDevice2, 100u);
     // DEBUG_RESULT("adi_i2c_SetBitRate failed\n",eResult,ADI_I2C_SUCCESS);

     eResult = adi_i2c_SetDutyCycle(hDevice2, DUTYCYCLE);
     // DEBUG_RESULT("adi_i2c_SetDutyCycle failed\n",eResult,ADI_I2C_SUCCESS);

     /* Set hardware address width */
     eResult = adi_i2c_SetHWAddressWidth(hDevice2, ADI_I2C_HWADDR_WIDTH_7_BITS);
     // DEBUG_RESULT("adi_i2c_SetHWAddressWidth failed\n",eResult,ADI_I2C_SUCCESS);

     /* set ADT7420 slave address */
     eResult = adi_i2c_SetHardwareAddress(hDevice2, ADP5061_I2C_ADDRESS);
     // DEBUG_RESULT("adi_i2c_SetHardwareAddress failed\n",eResult,ADI_I2C_SUCCESS);
     return eResult;
}

ADI_I2C_RESULT I2C2_De_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
     eResult = adi_i2c_Close(hDevice2);
     return eResult;
}

#if 0
/**
    * @brief    I2C3 init function
    * @param    None
    * @retval   None
    */
ADI_I2C_RESULT I2C3_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

     eResult = adi_i2c_Open(TWIDEVNUM+2,
                            ADI_I2C_MASTER,
                            &deviceMemory3[0],
                            ADI_I2C_MEMORY_SIZE,
                            &hDevice3);
     // DEBUG_RESULT("adi_i2c_Open failed\n",eResult,ADI_I2C_SUCCESS);

     eResult = adi_i2c_SetBitRate(hDevice3, BITRATE);
     // DEBUG_RESULT("adi_i2c_SetBitRate failed\n",eResult,ADI_I2C_SUCCESS);

     eResult = adi_i2c_SetDutyCycle(hDevice3, DUTYCYCLE);
     // DEBUG_RESULT("adi_i2c_SetDutyCycle failed\n",eResult,ADI_I2C_SUCCESS);

     /* Set hardware address width */
     eResult = adi_i2c_SetHWAddressWidth(hDevice3, ADI_I2C_HWADDR_WIDTH_7_BITS);
     // DEBUG_RESULT("adi_i2c_SetHWAddressWidth failed\n",eResult,ADI_I2C_SUCCESS);

     /* set ADT7420 slave address */
     eResult = adi_i2c_SetHardwareAddress(hDevice3, ADUX1050_I2C_ADDRESS);
     // DEBUG_RESULT("adi_i2c_SetHardwareAddress failed\n",eResult,ADI_I2C_SUCCESS);
     return eResult;
}
#endif

/**
    * @brief    HAL function to transmit data through ADPD I2C
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write to the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADPD_I2C_Transmit(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#elif ADUCM3029
    adi_gpio_DisableExIRQ(XINT_EVT2_IRQn);
#else
    adi_gpio_DisableExIRQ(XINT0_EVT2_IRQn);
#endif //_USING_RTOS_
    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADPD_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Write(hDevice1, pData, Size))) {
        // DEBUG_MESSAGE("adi_i2c_Write failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
    return (ADI_HAL_STATUS_t)eResult;
}

/**
* @brief    HAL function to transmit data through ADPD I2C
* @param    8-bit data pointer
* @param    16-bit size of the data to write to the hardware
* @retval   HAL error status
*/
ADI_HAL_STATUS_t ADPD_I2C_Transmit_UBLK(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
    void* pBuffer;
#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#elif ADUCM3029
    adi_gpio_DisableExIRQ(XINT_EVT2_IRQn);
#else
    adi_gpio_DisableExIRQ(XINT0_EVT2_IRQn);
#endif //_USING_RTOS_

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_SubmitTxBuffer(hDevice1, pData, Size, true))) {
        // DEBUG_MESSAGE("adi_i2c_SubmitTxBuffer failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, true))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_GetTxBuffer(hDevice1, &pBuffer))) {
        // DEBUG_MESSAGE("adi_i2c_GetTxBuffer failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, false))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to write a byte and read specified number of bytes from ADPD
    * @param    8-bit data pointer
    * @param    8-bit data pointer
    * @param    16-bit size of the data to read from the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADPD_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#elif ADUCM3029
    adi_gpio_DisableExIRQ(XINT_EVT2_IRQn);
#else
    adi_gpio_DisableExIRQ(XINT0_EVT2_IRQn);
#endif //_USING_RTOS_

    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADPD_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Write(hDevice1, pTxData, 1))) {
        // DEBUG_MESSAGE("adi_i2c_Write failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Read(hDevice1, pRxData, RxSize))) {
        // DEBUG_MESSAGE("adi_i2c_Write failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
    return (ADI_HAL_STATUS_t)eResult;
}
/**
* @brief    HAL function to write a byte and read specified number of bytes from ADPD
* @param    8-bit data pointer
* @param    8-bit data pointer
* @param    16-bit size of the data to read from the hardware
* @retval   HAL error status
*/
ADI_HAL_STATUS_t ADPD_I2C_TxRx_UBLK(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
    void* pBuffer;

#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#elif ADUCM3029
    adi_gpio_DisableExIRQ(XINT_EVT2_IRQn);
#else
    adi_gpio_DisableExIRQ(XINT0_EVT2_IRQn);
#endif //_USING_RTOS_

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_SubmitTxBuffer(hDevice1, pTxData, 1u, true))) {
        // DEBUG_MESSAGE("adi_i2c_SubmitTxBuffer failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    /* read register value */
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_SubmitRxBuffer(hDevice1, pRxData, RxSize, false))) {
        // DEBUG_MESSAGE("adi_i2c_SubmitRxBuffer failed\n");
#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, true))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_GetTxBuffer(hDevice1, &pBuffer))) {
        // DEBUG_MESSAGE("adi_i2c_GetTxBuffer failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_GetRxBuffer(hDevice1, &pBuffer))) {
        // DEBUG_MESSAGE("adi_i2c_GetRxBuffer failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, false))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
        adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
        return (ADI_HAL_STATUS_t)eResult;
    }

#ifdef _USING_RTOS_
    adi_osal_MutexPost(I2C1TransferLock);
#elif ADUCM3029
    adi_gpio_EnableExIRQ(XINT_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#else
    adi_gpio_EnableExIRQ(XINT0_EVT2_IRQn, ADI_GPIO_IRQ_FALLING_EDGE);
#endif //_USING_RTOS_
    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to transmit data through ADUX I2C
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write to the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADP5061_I2C_Transmit(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#endif //_USING_RTOS_
    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADP5061_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Write(hDevice1, pData, Size))) {
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#endif //_USING_RTOS_
        // DEBUG_MESSAGE("adi_i2c_Write failed\n");
        return ADI_HAL_ERROR;
    }
#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#endif //_USING_RTOS_

    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to write a byte and read specified number of bytes from ADUX
    * @param    8-bit data pointer
    * @param    8-bit data pointer
    * @param    16-bit size of the data to read from the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADP5061_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
    void* pBuffer;
#ifdef _USING_RTOS_
    adi_osal_MutexPend(I2C1TransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#endif //_USING_RTOS_
    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADP5061_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_SubmitTxBuffer(hDevice1, pTxData, 1u, true))) {
        // DEBUG_MESSAGE("adi_i2c_SubmitTxBuffer failed\n");
        return ADI_HAL_ERROR;
    }

    /* read register value */
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_SubmitRxBuffer(hDevice1, pRxData, 1u, false))) {
        // DEBUG_MESSAGE("adi_i2c_SubmitRxBuffer failed\n");
        return ADI_HAL_ERROR;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, true))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
        return ADI_HAL_ERROR;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_GetTxBuffer(hDevice1, &pBuffer))) {
        // DEBUG_MESSAGE("adi_i2c_GetTxBuffer failed\n");
        return ADI_HAL_ERROR;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_GetRxBuffer(hDevice1, &pBuffer))) {
        // DEBUG_MESSAGE("adi_i2c_GetRxBuffer failed\n");
        return ADI_HAL_ERROR;
    }
    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Enable(hDevice1, false))) {
        // DEBUG_MESSAGE("adi_i2c_Enable failed\n");
        return ADI_HAL_ERROR;
    }

#ifdef _USING_RTOS_
        adi_osal_MutexPost(I2C1TransferLock);
#endif //_USING_RTOS_
    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to transmit data through ADUX1050 I2C
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write to the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADUX1050_I2C_Transmit(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADUX1050_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Write(hDevice1, pData, Size))) {
        return (ADI_HAL_STATUS_t)eResult;
    }

    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to write a byte and read specified number of bytes from ADUX1050
    * @param    8-bit data pointer
    * @param    8-bit data pointer
    * @param    16-bit size of the data to read from the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADUX1050_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

    eResult = adi_i2c_SetHardwareAddress(hDevice1, ADUX1050_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Write(hDevice1, pTxData, 1))) {
        return (ADI_HAL_STATUS_t)eResult;
    }

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_Read(hDevice1, pRxData, RxSize))) {
        return (ADI_HAL_STATUS_t)eResult;
    }
    return (ADI_HAL_STATUS_t)eResult;
}
#endif

/**
    * @brief    HAL function to write a byte and read specified number of bytes from ADUX1050
    * @param    8-bit data pointer
    * @param    8-bit data pointer
    * @param    16-bit size of the data to read from the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADPD_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
    /* HW Error result */
    uint32_t hwErrors;
    /* I2C transaction structure */
    ADI_I2C_TRANSACTION xfr;
    /* do a simple one-byte read of chip ID */
//    prologueData[0]     = 0x0b;  /* address of ID register */
    xfr.pPrologue       = pTxData;
    xfr.nPrologueSize   = 1;
    xfr.pData           = pRxData;
    xfr.nDataSize       = RxSize;
    xfr.bReadNotWrite   = true;
    xfr.bRepeatStart    = true;
    eResult = adi_i2c_SetSlaveAddress(hDevice1, ADPD_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_ReadWrite(hDevice1, &xfr, &hwErrors))) {
        return (ADI_HAL_STATUS_t)eResult;
    }
    return (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    HAL function to transmit data through ADUX1050 I2C
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write to the hardware
    * @retval   HAL error status
    */
ADI_HAL_STATUS_t ADPD_I2C_Transmit(uint8_t *pData, uint16_t Size) {
    ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;

    /* HW Error result */
    uint32_t hwErrors;
    /* I2C transaction structure */
    ADI_I2C_TRANSACTION xfr;
    /* do a simple one-byte read of chip ID */
//    prologueData[0]     = 0x0b;  /* address of ID register */
    xfr.pPrologue       = &pData[0];
    xfr.nPrologueSize   = 1;
    xfr.pData           = &pData[1];
    xfr.nDataSize       = 2;
    xfr.bReadNotWrite   = false;
    xfr.bRepeatStart    = true;
    eResult = adi_i2c_SetSlaveAddress(hDevice1, ADPD_I2C_ADDRESS);

    if (ADI_I2C_SUCCESS != (eResult = adi_i2c_ReadWrite(hDevice1, &xfr, &hwErrors))) {
        return (ADI_HAL_STATUS_t)eResult;
    }

    return (ADI_HAL_STATUS_t)eResult;
}

