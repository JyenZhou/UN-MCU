#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ADPD_I2C_ADDRESS                            0x64

#define TWIDEVNUM     (0u)        /* TWI device number */
#define BITRATE       (100u)      /* kHz */
#define DUTYCYCLE     (50u)       /* percent */

/* Private variables ---------------------------------------------------------*/
/* Device handle */
static ADI_I2C_HANDLE hDevice1;
uint8_t deviceMemory1[ADI_I2C_MEMORY_SIZE];

/* Private function prototypes -----------------------------------------------*/

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

        /* check set speed API */
        if (adi_i2c_SetBitRate(hDevice1, BITRATE))  // top-end
            return ADI_I2C_FAILURE;


        /* test ADT7420 I2C temperature sensor (U6) on ADuCM4x50 EZ-Kit */

        /* set slave address for ADI ADT7420 */
        if (adi_i2c_SetSlaveAddress(hDevice1, ADPD_I2C_ADDRESS ))//ADUX1050_I2C_ADDRESS
            return ADI_I2C_FAILURE;

     return ADI_I2C_SUCCESS;
}

/**
    * @brief    I2C initialization. This function initiaises all the I2C controllers used.
    * @param    None
    * @retval   None
    */
ADI_I2C_RESULT I2C_Init(void) {
     ADI_I2C_RESULT eResult = ADI_I2C_SUCCESS;
     eResult = I2C1_Init();  // For communicating with ADPD

     return eResult;
}

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

