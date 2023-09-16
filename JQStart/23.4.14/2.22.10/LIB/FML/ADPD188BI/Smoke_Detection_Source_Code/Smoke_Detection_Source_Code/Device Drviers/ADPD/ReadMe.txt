ReadMe.txt
==========

ADPDDrv Version: 2.0

                                   ROM            RAM
ADPDDrv  (Release Build)           2,207 Bytes    83 Bytes

Release Notes
-------------
Version 0.9, Sept 24, 2014                                                  
  Change FIFO filled threshold                                              
Version 1.0, Oct 01, 2014                                                   
  Add efuse function                                                        
Version 1.1, Oct 29, 2014                                                   
  Add timestamp output                                                      
Version 1.2, Nov 21, 2014                                                   
  Created MCU_HAL_I2C_TxRx() and use it here                                
  Added set to Idle mode in Open/close driver                               
  Enable to read FIFO from ISR                                              
Version 1.3, Jan 28, 2015                                                   
  Fixed bug with time stamp and FIFO watermark                              
Version 1.4, Feb 28, 2015                                                   
  Added device reset and FIFO overflow indicator in debug mode              	
Version 1.5, Apr 30, 2015                                                   *
  Added support for the R4 in summation mode and channel 1 only             *
Version 1.6, Apr 30, 2015                                                   *
  Added support for the Digital Intergration mode.                          *
  Added flush soft buffer when FIFO is flushed.                             *
Version 1.7, Jun 17, 2015                                                   *
  Renamed MCU_HAL_I2C_TxRx() to ADPD_I2C_TxRx()                             *
  Renamed MCU_HAL_I2C_Transmit()() to ADPD_I2C_Transmit()                   *
  Removed GPIO_Pin argument from AdpdISR
Version 2.0, Oct 12, 2015                                                   *
  Simplified the driver code by moving the ring buffer to the application   *
  New functions and API added                                               *


HAL Functions
-------------  

-----> Typedefs used <-----
typedef enum
{
  ADI_HAL_OK       = 0x00,
  ADI_HAL_ERROR    = 0x01,
  ADI_HAL_BUSY     = 0x02,
  ADI_HAL_TIMEOUT  = 0x03
} ADI_HAL_STATUS_t;

-----> void GPIO_IRQ_ADPD_Disable(void) <-----
* Disable the interrupt line supporting the ADPD device

-----> void GPIO_IRQ_ADPD_Enable(void) <-----
* Enable the interrupt line supporting the ADPD device

-----> void MCU_HAL_Delay(uint32_t delay) <-----
* Implement a millisecond delay, specified by the value "delay".

-----> uint32_t MCU_HAL_GetTick() <-----
* returns the current clock tick in milliseconds.

-----> ADPD_I2C_Transmit(uint8_t *pData, uint16_t Size) <-----
* Transmits the buffer pointed to by "pData", where the size is specified by "Size". 

-----> ADPD_I2C_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize) <-----
* Transmits the address pointed to by "pTxData" buffer. 
* Receives the data from ADPD in buffer pointed to by "pRxData", where the size is specified by "RxSize". 

Usage
-----

#include "AdpdDrv.h"

static void LoadDefaultConfig(uint32_t *cfg);
static void DriverBringUp(uint8_t nSlotA, uint8_t nSlotB);

static uint32_t ganDcfg[] = {
    0x004b2695,  // Data sampling clock 32KHz frequency adjust
    0x004d4272,  // Internal timing clock 32MHz frequency adjust
    0x00020005,
    0x00060000,
    0x0012000A,
    0x00140447, //0x00140449
    0x00150333,
    0x00181F00,
    0x00191F00,
    0x001a1F00,
    0x001b1F00,
    0x001e1F00,
    0x001f1F00,
    0x00201F00,
    0x00211F00,
    0x00223030,
    0x00233035, //-> 3x, adjusted slew rate //0x00233035 // * 0x00233035
    0x0024303C, //-> 3x, adjusted slew rate //0x0024303A // * 0x0024303A
    0x002502DF,
    0x00340300,
    0x00300330,
    0x00310113, // LED pulses
    0x00421C36, // Integrator register
    0x0043ADA5,
    0x003924D4,
    0x003b24D4,
    0x00350330,
    0x00360813, //0x00360813 // LED pulses
    0x00441C35, // 35 -> 100K (writst) //0x00441C36 -> 50K (finger), // Integrator register 0x1c37 -> 25K
    0x0045ADA5,
    0x004e0040,
    0xFFFFFFFF,
};

main() {

    AdpdDrvDataReadyCallback(AdpdFifoCallBack);
    AdpdRxBufferInit();
    AdpdDrvSoftReset();
    AdpdDrvOpenDriver();
    LoadDefaultConfig(ganDcfg); // Loading Default configuration parameters
    VerifyDefaultConfig(ganDcfg);  // Reading Default configuration parameters
    /* Write standard value of clock registers */
    AdpdDrvRegWrite(0x004B, 0x2695);
    AdpdDrvRegWrite(0x004D, 0x4272);
    
    DriverBringUp(ADPDDrv_4CH_16, ADPDDrv_4CH_16);
    
    AdpdDrvCloseDriver();
}

/**
      @internal
      @brief  Create a Ring Buffer to store received data.
      @param  None
      @retval None
*/
static void AdpdRxBufferInit(void) {
    uint8_t aLoopCount;
    AdpdWtBufferPtr = &AdpdRxBuff[0];
    AdpdRdBufferPtr = &AdpdRxBuff[0];
    for (aLoopCount = 0; aLoopCount < ADPD_RX_BUFFER_SIZE; aLoopCount++) {
        AdpdRxBuff[aLoopCount].Next = &AdpdRxBuff[aLoopCount + 1];
    }
    AdpdRxBuff[ADPD_RX_BUFFER_SIZE - 1].Next = &AdpdRxBuff[0];
}


/**
  * Load ADPD default configuration
  */
static void LoadDefaultConfig(uint32_t *cfg) {
    uint8_t regAddr, i;
    uint16_t regData;
    if (cfg == 0) {
        return;
    }
    // clear FIFO
    AdpdDrvRegWrite(0x10, 0);
    AdpdDrvRegWrite(0x5F, 1);
    AdpdDrvRegWrite(0x00, 0x80FF);
    AdpdDrvRegWrite(0x5F, 0);
    i = 0;
    while (1) {
        regAddr = (uint8_t)(cfg[i] >> 16);
        regData = (uint16_t)(cfg[i]);
        i++;
        if (regAddr == 0xFF) {
            break;
        }
        if (AdpdDrvRegWrite(regAddr, regData) != ADPDDrv_SUCCESS) {
            break;
        }
    }
}
/**
  * Verify the loaded ADPD default configuration
  */
void VerifyDefaultConfig(uint32_t *cfg) {
    uint16_t def_val;
    uint8_t  i;
    uint8_t  regAddr;
    uint16_t regData;
    if (cfg == 0) {
        return;
    }
    i = 0;
    regAddr = (uint8_t)(cfg[0] >> 16);
    def_val = (uint16_t)(cfg[0]);
    while (regAddr != 0xFF) {
        if (AdpdDrvRegRead(regAddr, &regData) != ADPDDrv_SUCCESS) {
            debug("DCFG: Read Error reg(%0.2x)\n", regAddr);
            return;
        } else if (regData != def_val) {
            debug("DCFG: Read mismatch reg(%0.2x) (%0.2x != %0.2x)\n",
                  regAddr, def_val, regData);
            return;
        }
        i++;
        regAddr = (uint8_t)(cfg[i] >> 16);
        def_val = (uint16_t)(cfg[i]);
    }
}

uint16_t AppReadAdpdDataBuffer(ADPDData_t *rxData, uint32_t *time) {
    uint8_t i;

    AdpdDrvGetParameter(ADPD_SLOTMODEA, &gnSlotModeA);
    AdpdDrvGetParameter(ADPD_SLOTMODEB, &gnSlotModeB);
    if (AdpdRdBufferPtr == AdpdWtBufferPtr) {
        /* Set gnAdpdDataReady to 1 if IRQ just happened else set it to 0 */
        gnAdpdDataReady = gnAdpdIRQReady;
        /* Return error as there is no data in soft FIFO */
        return ADPDDrv_ERROR;
    }
    if (gnSlotModeA & 0x4) {   // for 32-bit mode
        for (i = 0; i < gnDataSetSize_A; i += 4) {
            *rxData = (AdpdRdBufferPtr->DataValue[i] << 8) +
                      (AdpdRdBufferPtr->DataValue[i + 1]) +
                      (AdpdRdBufferPtr->DataValue[i + 2] << 24) +
                      (AdpdRdBufferPtr->DataValue[i + 3] << 16);
            rxData++;
        }
    } else {          // for 16-bit mode
        for (i = 0; i < (gnDataSetSize_A); i += 2) {
            *rxData = (AdpdRdBufferPtr->DataValue[i] << 8) +
                      AdpdRdBufferPtr->DataValue[i + 1];
            rxData++;
        }
    }
    if (gnSlotModeB & 0x4) {   // for 32-bit mode
        for (i = 0; i < (gnDataSetSize_B); i += 4) {
            *rxData = (AdpdRdBufferPtr->DataValue[i + gnDataSetSize_A] << 8) +
                      (AdpdRdBufferPtr->DataValue[i + gnDataSetSize_A + 1]) +
                      (AdpdRdBufferPtr->DataValue[i +
                      gnDataSetSize_A + 2] << 24) +
                      (AdpdRdBufferPtr->DataValue[i +
                      gnDataSetSize_A + 3] << 16);
            rxData++;
        }
    } else {          // for 16-bit mode
        for (i = 0; i < (gnDataSetSize_B); i += 2) {
            *rxData = (AdpdRdBufferPtr->DataValue[i + gnDataSetSize_A] << 8) +
                      AdpdRdBufferPtr->DataValue[i + gnDataSetSize_A + 1];
            rxData++;
        }
    }
    if (time != 0) {
        *time = AdpdRdBufferPtr->TimeStamp;
    }
    AdpdRdBufferPtr = AdpdRdBufferPtr->Next;
    gnAdpdIRQReady = 0;
    return ADPDDrv_SUCCESS;
}

/**
      @internal
      @brief  Move data from i2c FIFO to this Ring Buffer. If WtPtr reach RdPtr,
              host's reading is too slow. Toggle LED to indicate this event.
      @param  tcv Timestamp of the oldest data in the FIFO
      @retval int16_t A 16-bit integer: 0 - success; < 0 - failure
*/
uint16_t AdpdRxBufferInsertData(uint32_t nTcv) {
    uint32_t nTimeStamp;
    if (AdpdWtBufferPtr->Next == AdpdRdBufferPtr) {
        return ADPDDrv_ERROR;
    }
    if (gnAdpdTimeIrqSet == 1 && gnAdpdTimeIrqSet1 == 0) {
        gnAdpdTimeIrqSet = 0;
        if (gnAdpdTimeCurVal > 0)
            nTimeStamp = nTcv;
        else
            nTimeStamp = 0 - nTcv;
    } else {
        nTimeStamp = gnAdpdTimePreVal + gnAdpdTimeGap;
    }
    gnAdpdTimePreVal = nTimeStamp;

    AdpdWtBufferPtr->TimeStamp = nTimeStamp;
    memset((void *)AdpdWtBufferPtr->DataValue,
           0,
           sizeof(AdpdWtBufferPtr->DataValue));
    memcpy(AdpdWtBufferPtr->DataValue,
           &anRegReadData[0],
           gnDataSetSize_A + gnDataSetSize_B);
    AdpdWtBufferPtr = AdpdWtBufferPtr->Next;
    //AppReadAdpdDataBuffer(anAlignReadData, &time_s);
    return ADPDDrv_SUCCESS;
}

/**
 * DriverBringUp(): Sets up the ADPD driver for reading data
 */
static void DriverBringUp(uint8_t nSlotA, uint8_t nSlotB) {

    uint8_t nLoopCount = 0;
    uint16_t nRetValue = 0, nAdpdFifoLevelSize = 0, nAdpdSetWaterMarkLevel = 4;
    uint32_t nTcv = 0, LoopCnt;
    uint16_t nAdpdDataSetSize;

    ADPDData_t value[8] = {0};
    gnAdpdDataReady   = 0;
    gnAdpdIRQHappen   = 0;
    gnDataSetSize_A   = 0;
    gnDataSetSize_B   = 0;
    AdpdWtBufferPtr = AdpdRdBufferPtr;

    AdpdDrvSetSlot(nSlotA, nSlotB);
    gnSlotModeA = (ADPDDrv_Operation_Slot_t)nSlotA;
    gnSlotModeB = (ADPDDrv_Operation_Slot_t)nSlotB;
    if (gnSlotModeA == ADPDDrv_4CH_16) {
        gnDataSetSize_A  = SLOT_A_DATA_SIZE;
    }
    if (gnSlotModeB == ADPDDrv_4CH_16) {
        gnDataSetSize_B  = SLOT_B_DATA_SIZE;
    }
    nAdpdDataSetSize = (gnDataSetSize_A + gnDataSetSize_B);
    AdpdDrvSetParameter(ADPD_WATERMARKING, nAdpdSetWaterMarkLevel);
    AdpdDrvSetOperationMode(ADPDDrv_MODE_SAMPLE);

    while (1) {
      if(gnAdpdDataReady)  {
          gnAdpdDataReady = 0;
          AdpdDrvGetParameter(ADPD_FIFOLEVEL, &nAdpdFifoLevelSize);

          while (nAdpdFifoLevelSize >= nAdpdDataSetSize) {
              nRetValue = AdpdDrvReadFifoData((uint8_t *)&anRegReadData[0],
                                                  nAdpdDataSetSize);
              if (nRetValue == ADPDDrv_SUCCESS) {
                  if (AdpdRxBufferInsertData(nTcv) != ADPDDrv_SUCCESS)
                      break;
                  nAdpdFifoLevelSize = nAdpdFifoLevelSize - nAdpdDataSetSize;
              }
          }
      }

      if (AppReadAdpdDataBuffer((ADPDData_t *)&value[0], 0) != ADPDDrv_SUCCESS)
      {
         for (LoopCnt = 0; LoopCnt < 8; LoopCnt++)
               debug("%u ", value[LoopCnt]);
         debug("\r\n");

         AdpdLibDelay(5); //Give the CPU a break
      }
    }
}
