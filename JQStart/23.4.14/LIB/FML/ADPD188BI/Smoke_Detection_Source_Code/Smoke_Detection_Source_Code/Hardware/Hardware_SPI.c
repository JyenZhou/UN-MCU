#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SPI_DEV_NUM                          1

/* Private variables ---------------------------------------------------------*/
/* Device handle */
ADI_SPI_HANDLE hSPIDevice0;
ADI_SPI_HANDLE hSPIDevice1;
ADI_SPI_HANDLE hSPIDevice2;

/* Memory required for SPI driver */
static uint8_t SPIMem1[ADI_SPI_MEMORY_SIZE];
//static uint8_t SPIMem2[ADI_SPI_MEMORY_SIZE];
static ADI_SPI_TRANSCEIVER Transceive;

/* Private function prototypes -----------------------------------------------*/

/**
    * @brief    SPI init function
    * @param    None
    * @retval   None
    */
void SPI_Init(void) {
  /* Configure the Device */
  /* Open the SPI device. It opens in Master mode by default */
  if (adi_spi_Open(SPI_DEV_NUM, SPIMem1, ADI_SPI_MEMORY_SIZE, &hSPIDevice1) != ADI_SPI_SUCCESS) {
    // printf("Failed to open SPI service\n");
  }
  
  /* Set the chip select */
  if (adi_spi_SetChipSelect(hSPIDevice1, ADPD_CFG_SPI_DEV_CS) != ADI_SPI_SUCCESS) {
    // printf("Failed to set the chip select\n");
  }
  
  /* Set the SPI clock rate */
  if (adi_spi_SetBitrate(hSPIDevice1, ADPD_CFG_SPI_DEV_BITRATE) != ADI_SPI_SUCCESS) {
    // printf("Failed to set the SPI clock rate\n");
  }

  /* Set master mode */
  if (adi_spi_SetMasterMode(hSPIDevice1, true) != ADI_SPI_SUCCESS) {
    // printf("Failed to set the master mode\n");
  }
}

/**
    * @brief    Function to transmit data through SPI1
    * @param    8-bit data pointer
    * @param    16-bit size of the data to write
    * @param    Time out if the device doesn't respond
    * @retval   HAL error status
    */
ADI_SPI_RESULT HAL_SPI1_Transmit(uint8_t *pTxData, uint16_t Size)
{
  ADI_SPI_RESULT eResult = ADI_SPI_SUCCESS;

  if(adi_spi_SetContinuousMode(hSPIDevice1, true) != ADI_SPI_SUCCESS)
  {
      return ADI_SPI_FAILURE;
  }

  /* Prepare the buffers for submitting to Master */
  Transceive.pTransmitter = pTxData;
  Transceive.TransmitterBytes = Size;
  Transceive.nTxIncrement = 1;
  Transceive.pReceiver = NULL;
  Transceive.ReceiverBytes = 0;
  Transceive.nRxIncrement = 0;
  Transceive.bDMA                =   false;
  Transceive.bRD_CTL             =   false;

  if(adi_spi_MasterReadWrite(hSPIDevice1, &Transceive) != ADI_SPI_SUCCESS)
  {
      return ADI_SPI_FAILURE;
  }
  
  return  eResult;
}

/**
    * @brief    HAL function to transmit and recieve data through SPI2 in full duplex mode.
    * @param    8-bit data pointer of transmit buffer
    * @param    8-bit data pointer of recieve buffer
    * @param    16-bit size of the data. This is the total size. ie Read + Write size.
    * @param    Time out if the device doesn't respond
    * @retval   HAL error status
    */
ADI_SPI_RESULT HAL_SPI1_Receive(uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize, uint16_t RxSize)
{
  ADI_SPI_RESULT eResult = ADI_SPI_SUCCESS;
  if(adi_spi_SetContinuousMode(hSPIDevice1, true) != ADI_SPI_SUCCESS)
  {
       return ADI_SPI_FAILURE;
  }

  /* Prepare the buffers for submitting to Master */
  Transceive.TransmitterBytes = TxSize; // TODO, for FIFO, this should be 1
  Transceive.ReceiverBytes = RxSize;
  Transceive.nTxIncrement = 1;
  Transceive.pTransmitter = pTxData;
  Transceive.pReceiver = pRxData;
  Transceive.nRxIncrement = 1;
  Transceive.bDMA                =   false;
  Transceive.bRD_CTL             =   true;
  
  if(adi_spi_MasterReadWrite(hSPIDevice1, &Transceive) != ADI_SPI_SUCCESS)
  {
      return ADI_SPI_FAILURE;
  }
 
  return  eResult;
}

/**
    * @brief    ADPD SPI Transmit function
    * @param    None
    * @retval   None
    */
ADI_HAL_STATUS_t ADPD_SPI_Transmit(uint8_t *pTxData, uint16_t Size)
{
  ADI_SPI_RESULT eResult = ADI_SPI_SUCCESS;
    
  adi_spi_SetChipSelect(hSPIDevice1, ADPD_CFG_SPI_DEV_CS); // Select ADPD106/7 device on PS board

  eResult = HAL_SPI1_Transmit(pTxData, Size);

  return  (ADI_HAL_STATUS_t)eResult;
}

/**
    * @brief    ADPD SPI Transmit and Receive function
    * @param    None
    * @retval   None
    */
ADI_HAL_STATUS_t ADPD_SPI_Receive(uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize, uint16_t RxSize)
{
  ADI_SPI_RESULT eResult = ADI_SPI_SUCCESS;

  adi_spi_SetChipSelect(hSPIDevice1, ADPD_CFG_SPI_DEV_CS); // Select ADPD106/7 device on PS board

  eResult = HAL_SPI1_Receive(pTxData, pRxData, TxSize, RxSize);
  
  return  (ADI_HAL_STATUS_t)eResult;
}