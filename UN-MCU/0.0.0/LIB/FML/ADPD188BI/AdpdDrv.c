/**
    ****************************************************************************
    * @addtogroup Device_Drivers Device Drivers
    * @{
    * @file     AdpdDrv.c
    * @author   ADI
    * @version  V2.0
    * @date     20-October-2015
    * @brief    Reference design device driver to access ADI ADPD chip.
    ****************************************************************************
    * @attention
    ****************************************************************************
*/
/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2015 Analog Devices Inc.                                      *
* All rights reserved.                                                        *
*                                                                             *
* This source code is intended for the recipient only under the guidelines of *
* the non-disclosure agreement with Analog Devices Inc.                       *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
*                                                                             *
* This software is intended for use with the ADPD and derivative parts        *
* only                                                                        *
*                                                                             *
******************************************************************************/

/******************************************************************************
* Version 0.9, Sept 24, 2014                                                  *
*   Change FIFO filled threshold                                              *
* Version 1.0, Oct 01, 2014                                                   *
*   Add efuse function                                                        *
* Version 1.1, Oct 29, 2014                                                   *
*   Add timestamp output                                                      *
* Version 1.2, Nov 21, 2014                                                   *
*   Created MCU_HAL_I2C_TxRx() and use it here                                *
*   Added set to Idle mode in Open/close driver                               *
*   Enable to read FIFO from ISR                                              *
* Version 1.3, Jan 28, 2015                                                   *
*   Fixed bug with time stamp and FIFO watermark                              *
* Version 1.4, Feb 28, 2015                                                   *
*   Added device reset and FIFO overflow indicator in debug mode              *
* Version 1.4.1, Apr 2, 2015                                                  *
*   Fixed bug with timestamp when the watermark is greater than 1             *
* Version 1.5, Apr 30, 2015                                                   *
*   Added support for the R4 in summation mode and channel 1 only             *
* Version 1.6, Apr 30, 2015                                                   *
*   Added support for the Digital Intergration mode.                          *
*   Added flush soft buffer when FIFO is flushed.                             *
* Version 1.7, Jun 17, 2015                                                   *
*   Renamed MCU_HAL_I2C_TxRx() to ADPD_I2C_TxRx()                             *
*   Renamed MCU_HAL_I2C_Transmit()() to ADPD_I2C_Transmit()                   *
*   Removed GPIO_Pin argument from AdpdISR                                    *
* Version 2.0, Oct 12, 2015                                                   *
*   Simplified the driver code by moving the ring buffer to the application   *
*   New functions and API added                                               *
******************************************************************************/

/* ------------------------- Includes -------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "AdpdDrv.h"
#include "jhal.h"






/* ------------------------- Defines  --------------------------------------- */

#define INTERRUPT_ENABLE
#define CLR_HFIFO_SAMPLE
#define DEVICE_ID_516           0x0516

#ifdef INTERRUPT_ENABLE
#define INT_ENA ((0x1) << 2)
#define INT_DRV ((0x0) << 1)
#define INT_POL ((0x1) << 0)
#endif


/* ------------------------- Public Function Prototypes -------------------- */
uint32_t* AdpdDrvGetDebugInfo(void);
void AdpdDrvSetIsrReadDataFromFIFO(uint8_t nEnableSet);
static void (*gpfnADPDCallBack)(void);
extern void AdpdDeviceReset(void);

/* ------------------------- Private variables ----------------------------- */
static uint8_t gnAdpdDataSetSize = 0;
static volatile  uint8_t gnIRQHappen;
static ADPDDrv_Operation_Slot_t gnSlotMode_A = (ADPDDrv_Operation_Slot_t) 0;
static ADPDDrv_Operation_Slot_t gnSlotMode_B = (ADPDDrv_Operation_Slot_t) 0;
static uint32_t gnAccessCnt[5];
static uint8_t gnFifoLevel;
static uint8_t gnAdpdDataSetSize_A, gnAdpdDataSetSize_B, gnChannelSize_A, gnChannelSize_B, gnBytesPerSample_A,gnBytesPerSample_B;
static uint16_t gnDeviceID;
static const uint8_t gaGetSlotSize[7] = {0x00,0x02,0x04,0x00,0x08,0x00,0x10};
static const uint8_t gaGetSlotMode[7] = {ADPDDrv_SLOT_OFF,ADPDDrv_SUM_16,ADPDDrv_SUM_32,ADPDDrv_SLOT_OFF,ADPDDrv_4CH_16,ADPDDrv_SLOT_OFF,ADPDDrv_4CH_32};
#ifndef NDEBUG
static uint32_t gnOverFlowCnt = 0;
#endif
/* ------------------------- Private Function Prototypes ------------------- */
static void Init(void);
static int16_t SetAdpdIdleMode(void);
static int16_t SetInterruptControl(void);
static int16_t AdpdDrvSetInterrupt(uint16_t nIntMask);
static int16_t GetSlotMode(uint8_t nSlot, uint16_t *pRegValue);
static int16_t SetRegister0x11(uint16_t nReg0x11Value);


#ifndef NOTUSED
int16_t AdpdDrvOpenDriver() {
    int16_t nRetCode = ADPDDrv_SUCCESS;
#ifndef NDEBUG
    gnOverFlowCnt = 0;
#endif
    nRetCode = AdpdDrvRegRead(0x8, &gnDeviceID);
    nRetCode |= SetAdpdIdleMode();
    Init();
    nRetCode |= AdpdDrvSetInterrupt(FIFO_INT_EN);
    nRetCode |= SetInterruptControl();
    return nRetCode;
}

int16_t AdpdDrvCloseDriver(void) {
#ifndef NDEBUG
#endif
    return SetAdpdIdleMode();
}

#endif

int16_t AdpdDrvRegWrite(uint16_t nAddr, uint16_t nRegValue) {

    uint8_t pTxData[3];

    pTxData[0] = (uint8_t)nAddr;
    pTxData[1] = (uint8_t)(nRegValue >> 8);
    pTxData[2] = (uint8_t)(nRegValue);


#ifdef ADPD_SPI
    pTxData[0] = (pTxData[0] << 1 ) | ADPD_SPI_WRITE ;
    if (ADPD_SPI_Transmit(pTxData, 3)!= ADI_HAL_OK) {
        return ADPDDrv_ERROR;
    }
#else

    if (!JHAL_iicTransmit(ADPD_IIC_ID,(uint8_t *) pTxData, 3)) {
        return ADPDDrv_ERROR;
    }

#endif

    if(nAddr == REG_OP_MODE_CFG) {
        SetRegister0x11(nRegValue);
    }


    return ADPDDrv_SUCCESS;
}

int16_t AdpdDrvRegRead(uint16_t nAddr, uint16_t *pnData) {
    uint8_t nRegAddr, anRxData[2];
    nRegAddr = (uint8_t)nAddr;

#ifdef ADPD_SPI
    uint8_t pTxData[1];
    pTxData[0] = ( nRegAddr << 1) | ADPD_SPI_READ;

    if (ADPD_SPI_Receive(pTxData, anRxData, 1, 2)!= ADI_HAL_OK) {
        return ADPDDrv_ERROR;
    }
#else


    if (!JHAL_iicMemRead(ADPD_IIC_ID,nRegAddr,false, (uint8_t *) anRxData, 2) ) {
        return ADPDDrv_ERROR;
    }
#endif

    *pnData = (anRxData[0] << 8) + anRxData[1];
    return ADPDDrv_SUCCESS;
}


int16_t AdpdDrvSetOperationMode(uint8_t nOpMode) {
    uint8_t nLoopCnt = 0;
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nTemp;
    if (gnDeviceID < DEVICE_ID_516) {
        nRetCode = AdpdDrvRegWrite(REG_FIFO_CLK, FIFO_CLK_EN);
    }
    if (nOpMode == ADPDDrv_MODE_IDLE) {
        nRetCode = SetAdpdIdleMode();
    } else if (nOpMode == ADPDDrv_MODE_PAUSE) {
        nRetCode = AdpdDrvRegWrite(REG_OP_MODE, OP_PAUSE_MODE);
    } else if (nOpMode == ADPDDrv_MODE_SAMPLE) {
        nRetCode = AdpdDrvRegWrite(REG_OP_MODE, OP_PAUSE_MODE);

        if (gnDeviceID < DEVICE_ID_516) {
            nRetCode |= AdpdDrvRegWrite(REG_FIFO_CLK, FIFO_CLK_EN);
        }
#ifdef CLR_HFIFO_SAMPLE
        nRetCode |= AdpdDrvRegRead(REG_DATA_BUFFER, &nTemp);
        nRetCode |= AdpdDrvRegWrite(REG_INT_STATUS, FIFO_CLR | IRQ_CLR_ALL);
#endif
        nRetCode |= AdpdDrvRegWrite(REG_OP_MODE, OP_RUN_MODE);
    } else {
        nRetCode = ADPDDrv_ERROR;
    }
    if (gnDeviceID < DEVICE_ID_516) {

        nRetCode |= AdpdDrvRegWrite(REG_TEST_PD, 0x0242);
    }
    for (nLoopCnt = 0; nLoopCnt < 5; nLoopCnt++) {
        gnAccessCnt[nLoopCnt] = 0;
    }
    return nRetCode;
}


int16_t AdpdDrvSetSlot(uint8_t nSlotA, uint8_t nSlotB) {
    uint16_t nRegValue;
    uint8_t pTxData[3];
    if (AdpdDrvRegRead(REG_OP_MODE_CFG, &nRegValue) != ADPDDrv_SUCCESS) {
        return ADPDDrv_ERROR;
    }
    gnSlotMode_A = (ADPDDrv_Operation_Slot_t)nSlotA;
    gnSlotMode_B = (ADPDDrv_Operation_Slot_t)nSlotB;
    nRegValue &= SLOT_MASK;
    if (nSlotA == ADPDDrv_SLOT_OFF) {
        gnAdpdDataSetSize_A = 0;
        gnChannelSize_A = 0;
        gnBytesPerSample_A = 0;
    } else if (nSlotA == ADPDDrv_4CH_16) {
        nRegValue |= SLOT_A_MODE;
        gnAdpdDataSetSize_A = SLOT_A_DATA_SIZE;
        gnChannelSize_A = 4;
        gnBytesPerSample_A = 2;
    } else if (nSlotA == ADPDDrv_4CH_32) {
        nRegValue |= SLOT_A_MODE_32;
        gnAdpdDataSetSize_A = SLOT_B_DATA_SIZE_32;
        gnChannelSize_A = 4;
        gnBytesPerSample_A = 4;
    } else if (nSlotA == ADPDDrv_SUM_16) {
        nRegValue |= SLOT_A_MODE_SUM16;
        gnAdpdDataSetSize_A = SLOT_B_DATA_SIZE_SUM16;
        gnChannelSize_A = 1;
        gnBytesPerSample_A = 2;
    } else if (nSlotA == ADPDDrv_SUM_32) {
        nRegValue |= SLOT_A_MODE_R4_SUM;
        gnAdpdDataSetSize_A = SLOT_A_DATA_SIZE_R4_SUM;
        gnChannelSize_A = 1;
        gnBytesPerSample_A = 4;
    } else if (nSlotA == ADPDDrv_DIM1_16) {
        nRegValue |= SLOT_A_D1_16;
        gnAdpdDataSetSize_A = SLOT_A_DATA_SIZE_D1_16;
        gnChannelSize_A = 1;
        gnBytesPerSample_A = 2;
    } else if (nSlotA == ADPDDrv_DIM2_32) {
        nRegValue |= SLOT_A_D2_32;
        gnAdpdDataSetSize_A = SLOT_A_DATA_SIZE_D2_32;
        gnChannelSize_A = 2;
        gnBytesPerSample_A = 4;
    } else {
        return ADPDDrv_ERROR;
    }

    if (nSlotB == ADPDDrv_SLOT_OFF) {
        gnAdpdDataSetSize_B = 0;
        gnChannelSize_B = 0;
        gnBytesPerSample_B = 0;
    } else if (nSlotB == ADPDDrv_4CH_16) {
        nRegValue |= SLOT_B_MODE;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE;
        gnChannelSize_B = 4;
        gnBytesPerSample_B = 2;
    } else if (nSlotB == ADPDDrv_4CH_32) {
        nRegValue |= SLOT_B_MODE_32;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE_32;
        gnChannelSize_B = 4;
        gnBytesPerSample_B = 4;
    } else if (nSlotB == ADPDDrv_SUM_16) {
        nRegValue |= SLOT_B_MODE_SUM16;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE_SUM16;
        gnChannelSize_B = 1;
        gnBytesPerSample_B = 2;
    } else if (nSlotB == ADPDDrv_SUM_32) {
        nRegValue |= SLOT_B_MODE_R4_SUM;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE_R4_SUM;
        gnChannelSize_B = 1;
        gnBytesPerSample_B = 4;
    } else if (nSlotB == ADPDDrv_DIM1_16) {
        nRegValue |= SLOT_B_D1_16;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE_D1_16;
        gnChannelSize_B = 1;
        gnBytesPerSample_B = 2;
    } else if (nSlotB == ADPDDrv_DIM2_32) {
        nRegValue |= SLOT_B_D2_32;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE_D2_32;
        gnChannelSize_B = 2;
        gnBytesPerSample_B = 4;
    } else {
        return ADPDDrv_ERROR;
    }

    gnAdpdDataSetSize = gnAdpdDataSetSize_A + gnAdpdDataSetSize_B;

    pTxData[0] = REG_OP_MODE_CFG ;
    pTxData[1] = (uint8_t)(nRegValue >> 8);
    pTxData[2] = (uint8_t)(nRegValue);
#ifdef ADPD_SPI
    pTxData[0] = (pTxData[0] << 1 ) | ADPD_SPI_WRITE ;
    if (ADPD_SPI_Transmit(pTxData, 3)!= ADI_HAL_OK) {
        return ADPDDrv_ERROR;
    }
#else
    if (!JHAL_iicTransmit(ADPD_IIC_ID,(uint8_t *) pTxData, 3) ) {
        return ADPDDrv_ERROR;
    }
#endif

    return ADPDDrv_SUCCESS;
}


void AdpdDrvDataReadyCallback(void (*pfADPDDataReady)()) {
    gpfnADPDCallBack = pfADPDDataReady;
}


void AdpdISR() {
    if (gpfnADPDCallBack != NULL) {
        (*gpfnADPDCallBack)();
    }
    gnAccessCnt[0]++;
}

uint32_t* AdpdDrvGetDebugInfo() {
    return gnAccessCnt;
}

int16_t AdpdDrvSetParameter(AdpdCommandStruct eCommand, uint16_t nValue) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    if (eCommand == ADPD_WATERMARKING) {
        nValue = (nValue == 0) ? 1 : nValue;
        nRetCode = AdpdDrvRegWrite(REG_I2CS_CTL_MATCH,
                                   (((nValue * gnAdpdDataSetSize) - 1) << 7));
    } else {
        return ADPDDrv_ERROR;
    }
    return nRetCode;
}

int16_t AdpdDrvGetParameter(AdpdCommandStruct eCommand, uint16_t *pnValue) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nStatData;
    uint16_t nRegValue1, nRegValue2;

    if (eCommand == ADPD_WATERMARKING) {
        nRetCode |= AdpdDrvRegRead(REG_I2CS_CTL_MATCH, &nStatData);
        nStatData = nStatData & FIFO_THRESHOLD_MASK;
        nStatData = ((nStatData | 0x0080) >> 7);
        nRetCode |= GetSlotMode(SLOT_A,&nRegValue1);
        nRetCode |= GetSlotMode(SLOT_B,&nRegValue2);
        nRegValue1 = gaGetSlotSize[nRegValue1];
        nRegValue2 = gaGetSlotSize[nRegValue2];
        nRegValue1 = nRegValue1 + nRegValue2;

        *pnValue = (nStatData/nRegValue1) + 1;

    } else if (eCommand == ADPD_FIFOLEVEL) {
        nRetCode |= AdpdDrvRegRead(REG_INT_STATUS, &nStatData);
        gnFifoLevel = nStatData >> 8;
        *pnValue = gnFifoLevel;

    } else if (eCommand == ADPD_TIMEGAP) {
        nRetCode |= AdpdDrvRegRead(REG_SAMPLING_FREQ, &nRegValue1);
        nRetCode |= AdpdDrvRegRead(REG_DEC_MODE, &nRegValue2);
        nRegValue2 = (nRegValue2 & 0xF0) >> 4;
        nRegValue2 = 1 << nRegValue2;
        *pnValue = ((nRegValue1 * nRegValue2)>>3);

    } else if (eCommand == ADPD_DATASIZEA) {
        *pnValue = gnAdpdDataSetSize_A;

    } else if (eCommand == ADPD_DATASIZEB) {
        *pnValue = gnAdpdDataSetSize_B;

    } else if (eCommand == ADPD_SLOTMODEA) {
        *pnValue = gnSlotMode_A;

    } else if (eCommand == ADPD_SLOTMODEB) {
        *pnValue = gnSlotMode_B;

    } else {
        return ADPDDrv_ERROR;
    }

    return nRetCode;
}

static void Init(void) {
    if (gnSlotMode_A == 0) {
        gnSlotMode_A = ADPDDrv_4CH_16;
        gnAdpdDataSetSize_A = SLOT_A_DATA_SIZE;
    }
    if (gnSlotMode_B == 0) {
        gnSlotMode_B = ADPDDrv_4CH_16;
        gnAdpdDataSetSize_B = SLOT_B_DATA_SIZE;
    }
    gnAdpdDataSetSize = gnAdpdDataSetSize_A + gnAdpdDataSetSize_B;
}

int16_t AdpdDrvReadFifoData(uint8_t *pnData, uint16_t nDataSetSize) {
    uint8_t nGetFifoData, nAddr;
#ifdef ADPD_SPI
    uint8_t pTxData[1];
#endif
#ifndef NDEBUG
    if (gnFifoLevel >= 128) {
        gnOverFlowCnt++;
    }
#endif
    if (gnDeviceID < DEVICE_ID_516) {
        if (gnFifoLevel >= nDataSetSize) {
            nGetFifoData = 1;

            AdpdDrvRegWrite(REG_TEST_PD, 0x343);
            AdpdDrvRegWrite(REG_TEST_PD, 0xF4F);
        } else {
            nGetFifoData = 0;
        }
    }
    if (gnFifoLevel >= nDataSetSize) {
        gnAccessCnt[2]++;
        nAddr = REG_DATA_BUFFER;

#ifdef ADPD_SPI
        pTxData[0] = (nAddr << 1) | ADPD_SPI_READ;
        if (ADPD_SPI_Receive(pTxData, pnData, 1, nDataSetSize)!= ADI_HAL_OK) {
            return ADPDDrv_ERROR;
        }
#else

        if (!JHAL_iicMemRead(ADPD_IIC_ID,false,nAddr, pnData,nDataSetSize)) {
            return ADPDDrv_ERROR;
        }
#endif
    }
    if (gnDeviceID < DEVICE_ID_516) {
        if (nGetFifoData == 1) {
            AdpdDrvRegWrite(REG_TEST_PD, 0x0242);
        }
    }
    return ADPDDrv_SUCCESS;
}

int16_t AdpdDrvEfuseRead(uint16_t *pReg_Values, uint8_t nSize) {
    uint8_t nRegLoopCount;
    uint16_t nReg_Value, clk32K, clkfifo;
    AdpdDrvRegRead(REG_OSC32K, &clk32K);
    AdpdDrvRegWrite(REG_OSC32K, clk32K | 0x80);
    JHAL_delayMs(10);
    AdpdDrvRegRead(REG_FIFO_CLK, &clkfifo);
    AdpdDrvRegWrite(REG_FIFO_CLK, clkfifo | 1);
    AdpdDrvRegWrite(REG_EFUSE_CTRL, 0x0003);
    AdpdDrvRegRead(REG_EFUSE_STATUS0, &nReg_Value);
    AdpdDrvRegRead(REG_EFUSE_STATUS0, &nReg_Value);
    nRegLoopCount = 0;
    if ((nReg_Value & 0x7) == 0x4) {
        for (; nRegLoopCount < nSize; nRegLoopCount++) {
            AdpdDrvRegRead(0x70 + nRegLoopCount, &pReg_Values[nRegLoopCount]);
        }
    }
    AdpdDrvRegWrite(REG_EFUSE_CTRL, 0x0);
    AdpdDrvRegWrite(REG_OSC32K, clk32K);
    AdpdDrvRegWrite(REG_FIFO_CLK, clkfifo);
    if (nRegLoopCount == 0) {
        return ADPDDrv_ERROR;
    }
    return ADPDDrv_SUCCESS;
}

int16_t AdpdDrvEfuseModuleTypeRead(uint16_t *moduletype) {
    return AdpdDrvEfuseRead(moduletype, 1);
}


uint8_t* AdpdDrvEfuseModuleNameRead() {
    uint16_t moduletype;
    AdpdDrvEfuseModuleTypeRead(&moduletype);
    switch (moduletype) {
    case 1:
        return ("153GGRI");
    case 2:
        return ("163URI");
    case 3:
        return ("163URIR2");
    case 4:
        return ("163URIR3");
    case 5:
        return ("174GGI");
    default:
        return ("153GGRI");
    }
}

int16_t AdpdDrvSetLedCurrent(uint8_t nLedCurrent, AdpdLedId nLedId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t nAdpdTxData;

    nReg = REG_LED1_DRV + (nLedId -1);

    if (!((nReg == REG_LED1_DRV) || (nReg == REG_LED2_DRV) || (nReg == REG_LED3_DRV))) {
        return ADPDDrv_ERROR;
    }
    nMask = LED_CURRENT_MASK;
    nBitPos = LED_CURRENT_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    nAdpdTxData = (nAdpdRxData & nMask) | (nLedCurrent << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}


int16_t AdpdDrvGetLedCurrent(uint8_t *pLedCurrent, AdpdLedId nLedId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_LED1_DRV + (nLedId -1);

    if (!((nReg == REG_LED1_DRV) || (nReg == REG_LED2_DRV) || (nReg == REG_LED3_DRV))) {
        return ADPDDrv_ERROR;
    }

    nMask = LED_CURRENT_MASK;
    nBitPos = LED_CURRENT_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    *pLedCurrent = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}

int16_t AdpdDrvSetLedTrimCurrent(uint8_t nLedTrim, AdpdLedId nLedId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t nAdpdTxData;

    nReg = REG_LED_TRIM;

    if (nLedId == LED_1) {
        nMask = LED_1_TRIM_MASK;
        nBitPos = LED_1_TRIM_BITPOS;

    } else if (nLedId == LED_2) {
        nMask = LED_2_TRIM_MASK;
        nBitPos = LED_2_TRIM_BITPOS;

    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    nAdpdTxData = (nAdpdRxData & nMask) | (nLedTrim << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetLedTrimCurrent(uint8_t *pLedTrim, AdpdLedId nLedId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_LED_TRIM;

    if (nLedId == LED_1) {
        nMask = LED_1_TRIM_MASK;
        nBitPos = LED_1_TRIM_BITPOS;

    } else if (nLedId == LED_2) {
        nMask = LED_2_TRIM_MASK;
        nBitPos = LED_2_TRIM_BITPOS;

    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    *pLedTrim = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}

int16_t AdpdDrvSetNumPulses(uint8_t nNumPulses, AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t nAdpdTxData;

    nReg = REG_PULSE_PERIOD_A + 5 * (nSlotId -1);

    if (!((nReg == REG_PULSE_PERIOD_A) || (nReg == REG_PULSE_PERIOD_B))) {
        return ADPDDrv_ERROR;
    }

    nMask = LED_NUMBER_PULSES_MASK;
    nBitPos = LED_NUMBER_PULSES_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    nAdpdTxData = (nAdpdRxData & nMask) | (nNumPulses << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetNumPulses(uint8_t *pNumPulses, AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_PULSE_PERIOD_A + 5 * (nSlotId -1);

    if (!((nReg == REG_PULSE_PERIOD_A) || (nReg == REG_PULSE_PERIOD_B))) {
        return ADPDDrv_ERROR;
    }

    nMask = LED_NUMBER_PULSES_MASK;
    nBitPos = LED_NUMBER_PULSES_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    *pNumPulses = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}

int16_t AdpdDrvSetAfeTiaGain(uint8_t nTiaGain, AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t  nAdpdTxData;

    nReg = REG_AFE_TRIM_A + 2 * (nSlotId - 1);

    if (!((nReg == REG_AFE_TRIM_A) || (nReg == REG_AFE_TRIM_B))) {
        return ADPDDrv_ERROR;
    }

    nMask = AFE_TIA_GAIN_MASK;
    nBitPos = AFE_TIA_GAIN_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    nAdpdTxData = (nAdpdRxData & nMask) | (nTiaGain << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetAfeTiaGain(uint8_t *pTiaGain, AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_AFE_TRIM_A + 2 * (nSlotId - 1);

    if (!((nReg == REG_AFE_TRIM_A) || (nReg == REG_AFE_TRIM_B))) {
        return ADPDDrv_ERROR;
    }

    nMask = AFE_TIA_GAIN_MASK;
    nBitPos = AFE_TIA_GAIN_BITPOS;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    *pTiaGain = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}


int16_t AdpdDrvSetSamplingFrequency(uint16_t nSamplingFreq) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint8_t nReg;
    uint16_t  nAdpdTxData;
    nReg = REG_SAMPLING_FREQ;

    nAdpdTxData = nSamplingFreq;

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetSamplingFrequency(uint16_t *pSamplingFreq) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint8_t nReg;
    uint16_t nAdpdRxData;

    nReg = REG_SAMPLING_FREQ;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    *pSamplingFreq = nAdpdRxData;

    return nRetCode;
}


int16_t AdpdDrvSetSlotLed(uint8_t nLedConnection, uint8_t nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t  nAdpdTxData;

    nReg = REG_PD_SELECT;

    if (nSlotId == SLOT_A) {
        nMask = SLOT_A_LED_CON_MASK;
        nBitPos = SLOT_A_LED_CON_BITPOS;

    } else if (nSlotId == SLOT_B) {
        nMask = SLOT_B_LED_CON_MASK;
        nBitPos = SLOT_B_LED_CON_BITPOS;

    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    nAdpdTxData = (nAdpdRxData & nMask) | (nLedConnection << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetSlotLed(uint8_t *pLedConnection, uint8_t nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_PD_SELECT;

    if (nSlotId == SLOT_A) {
        nMask = SLOT_A_LED_CON_MASK;
        nBitPos = SLOT_A_LED_CON_BITPOS;

    } else if (nSlotId == SLOT_B) {
        nMask = SLOT_B_LED_CON_MASK;
        nBitPos = SLOT_B_LED_CON_BITPOS;

    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    *pLedConnection = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}


int16_t AdpdDrvSetInternalAverage(uint8_t nAvgFactor, uint8_t nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;
    uint16_t  nAdpdTxData;

    nReg = REG_DEC_MODE;

    if (nSlotId == SLOT_A) {
        nMask = INT_AVG_SLOT_A_MASK;
        nBitPos = INT_AVG_SLOT_A_BITPOS;
    }  else if (nSlotId == SLOT_B) {
        nMask = INT_AVG_SLOT_B_MASK;
        nBitPos = INT_AVG_SLOT_B_BITPOS;
    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != 0)
        return ADPDDrv_ERROR;

    nAdpdTxData = (nAdpdRxData & nMask) | (nAvgFactor << nBitPos);

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}

int16_t AdpdDrvGetInternalAverage(uint8_t* pAvgFactor, uint8_t nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint8_t nBitPos;
    uint16_t nAdpdRxData;

    nReg = REG_DEC_MODE;

    if (nSlotId == SLOT_A) {
        nMask = INT_AVG_SLOT_A_MASK;
        nBitPos = INT_AVG_SLOT_A_BITPOS;
    }  else if (nSlotId == SLOT_B) {
        nMask = INT_AVG_SLOT_B_MASK;
        nBitPos = INT_AVG_SLOT_B_BITPOS;

    } else {
        return ADPDDrv_ERROR;
    }

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    *pAvgFactor = (nAdpdRxData & (~nMask)) >> nBitPos;

    return nRetCode;
}



int16_t AdpdDrvSetAdcOffset(uint16_t nAdcOffset, uint8_t nChannel,
                            AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint16_t  nAdpdTxData;

    nReg = REG_CH1_OFFSET_A + nChannel + 6 * (nSlotId - 1);

    if ((nReg < REG_CH1_OFFSET_A) || (nReg > REG_CH4_OFFSET_B)) {
        return ADPDDrv_ERROR;
    }

    nMask = ADC_MASK;
    nAdcOffset &= ~nMask;
    nAdpdTxData = nAdcOffset;

    if (AdpdDrvRegWrite(nReg, nAdpdTxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;

    return nRetCode;
}



int16_t AdpdDrvGetAdcOffset(uint16_t *pAdcOffset, uint8_t nChannel,
                            AdpdSlotId nSlotId) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nMask;
    uint8_t nReg;
    uint16_t nAdpdRxData;

    nReg = REG_CH1_OFFSET_A + 6 * (nSlotId -1);

    if ((nReg < REG_CH1_OFFSET_A) || (nReg > REG_CH4_OFFSET_B)) {
        return ADPDDrv_ERROR;
    }

    nReg = nReg + nChannel;

    nMask = 0x3FFF;

    if (AdpdDrvRegRead(nReg, &nAdpdRxData) != ADPDDrv_SUCCESS)
        return ADPDDrv_ERROR;
    *pAdcOffset = nAdpdRxData & nMask;

    return nRetCode;
}


int16_t AdpdDrvSoftReset(void) {
    AdpdDrvRegWrite(REG_SW_RESET, 0x1);
    return ADPDDrv_SUCCESS;
}


static int16_t AdpdDrvSetInterrupt(uint16_t nIntMask) {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    if ((nIntMask & 0x3E00) == 0) {
        nRetCode  = AdpdDrvRegWrite(REG_INT_MASK, nIntMask);
    }
    return nRetCode;
}


static int16_t SetInterruptControl() {
    int16_t nRetCode = ADPDDrv_SUCCESS;
    uint16_t nRegValue;
    nRetCode |= AdpdDrvRegRead(REG_PAD_IO_CTRL, &nRegValue);
    nRegValue = (nRegValue & 0xF8) | INT_ENA | INT_DRV | INT_POL;
    nRetCode |= AdpdDrvRegWrite(REG_PAD_IO_CTRL, nRegValue);  // Enable int
    return nRetCode;
}


static int16_t SetAdpdIdleMode() {
    int16_t nRetCode;

    nRetCode = AdpdDrvRegWrite(REG_OP_MODE, OP_PAUSE_MODE);

    if (gnDeviceID < DEVICE_ID_516) {
        nRetCode |= AdpdDrvRegWrite(REG_FIFO_CLK, FIFO_CLK_EN);
    }
#ifndef CLR_HFIFO_SAMPLE
    nRetCode |= AdpdDrvRegWrite(REG_INT_STATUS, FIFO_CLR | IRQ_CLR_ALL);
#endif

    nRetCode |= AdpdDrvRegWrite(REG_OP_MODE, OP_IDLE_MODE);

    if (gnDeviceID < DEVICE_ID_516) {
        nRetCode |= AdpdDrvRegWrite(REG_FIFO_CLK, FIFO_CLK_DIS);
    }
    return nRetCode;
}

static int16_t GetSlotMode(uint8_t nSlot, uint16_t *pRegValue)
{
    uint16_t nRegValue;
    if (AdpdDrvRegRead(REG_OP_MODE_CFG, &nRegValue) != ADPDDrv_SUCCESS) {
        return ADPDDrv_ERROR;
    }

    if(nSlot == SLOT_A)
        *pRegValue = ((nRegValue & SLOT_A_MASK) >> 2);
    else
        *pRegValue = ((nRegValue & SLOT_B_MASK) >> 6);

    return (ADPDDrv_SUCCESS);
}

int16_t AdpdGetSlotMode(uint8_t *slotModeA,uint8_t *slotModeB) {

    uint16_t nRegValue1;
    uint16_t nRegValue2;

    GetSlotMode(SLOT_A,&nRegValue1);
    GetSlotMode(SLOT_B,&nRegValue2);
    *slotModeA = gaGetSlotMode[nRegValue1];
    *slotModeB = gaGetSlotMode[nRegValue2];

    return 0;

}

int16_t AdpdFillDataToArray(uint32_t *pnaADPDData, uint8_t *FifoData) {

    uint8_t i;


    if(gnSlotMode_A != ADPDDrv_SLOT_OFF)
    {
        if (gnBytesPerSample_A == 4) {   // for 32-bit mode
            for (i = 0; i < gnChannelSize_A; i += 1) {
                (pnaADPDData[i]) = (FifoData[i*4] << 8) +
                                   (FifoData[i*4 + 1]) +
                                   (FifoData[i*4 + 2] << 24) +
                                   (FifoData[i*4 + 3] << 16);
            }
            for (i = gnChannelSize_A; i < 4; i += 1) {
                (pnaADPDData[i]) = 0;
            }
        } else {          // for 16-bit mode
            for (i = 0; i < gnChannelSize_A; i += 1) {
                (pnaADPDData[i]) = (FifoData[i*2] << 8) +
                                   FifoData[i*2 + 1];
            }
            for (i = gnChannelSize_A; i < 4; i += 1) {
                (pnaADPDData[i]) = 0;
            }
        }
    }
    else {
        for (i = 0; i < 4; i += 1) {
            (pnaADPDData[i]) = 0;
        }
    }

    if(gnSlotMode_B != ADPDDrv_SLOT_OFF)
    {
        if (gnBytesPerSample_B == 4) {   // for 32-bit mode
            for (i = 0; i < gnChannelSize_B; i += 1) {
                (pnaADPDData[i+4]) = (FifoData[i*4 + gnAdpdDataSetSize_A] << 8) +
                                     (FifoData[i*4 + gnAdpdDataSetSize_A + 1]) +
                                     (FifoData[i*4 + gnAdpdDataSetSize_A + 2] << 24) +
                                     (FifoData[i*4 + gnAdpdDataSetSize_A + 3] << 16);
            }
            for (i = gnChannelSize_B; i < 4; i += 1) {
                (pnaADPDData[i+4]) = 0;
            }
        } else {          // for 16-bit mode
            for (i = 0; i < gnChannelSize_B; i += 1) {
                (pnaADPDData[i+4]) = (FifoData[i*2 + gnAdpdDataSetSize_A] << 8) +
                                     FifoData[i*2 + gnAdpdDataSetSize_A + 1];
            }
            for (i = gnChannelSize_B; i < 4; i += 1) {
                (pnaADPDData[i+4]) = 0;
            }
        }
    }
    else {
        for (i = 0; i < 4; i += 1) {
            (pnaADPDData[i+4]) = 0;
        }
    }

    return 0;

}

static int16_t SetRegister0x11(uint16_t nRegValue) {

    uint8_t regSlotModeA;
    uint8_t regSlotModeB;

    regSlotModeA = ((nRegValue & SLOT_A_MASK) >> 2);
    regSlotModeB = ((nRegValue & SLOT_B_MASK) >> 6);
    return(AdpdDrvSetSlot(gaGetSlotMode[regSlotModeA],gaGetSlotMode[regSlotModeB]));

}

/**@}*/ /* end of group Adpd_Driver */
/**@}*/ /* end of group Device_Drivers */

