/*
 *****************************************************************************
 * @file:    adi_w25q32.c
 * @brief:   Driver for the W25Q32 SPI-flash.
 * @details: The W25Q32 is 32 Mb flash memory device that is interfaced SPI.

 * @note:    If the user wishes to write to a region of SPI-flash, they must 
 *           erase it before hand. The driver does not ensure that a region 
 *           has been erased before writing to it.
 *
 * @note:    The user is responsible for pinmuxing the SPI bus.
 *
 * @note:    The SPI bus used to interface with the W25Q32 is not available to
 *           the application until the driver is closed.
 *-----------------------------------------------------------------------------
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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/


#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): the basic types of char, int, short, long, float, and double should not be used
*   Necessary for stdbool.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm152 (rule 17.4) array indexing shall only be applied to objects defined as an array type
*   Need to use the pointer as an array in the write function for copying user buffer
*/
#pragma diag_suppress=Pm011,Pm073,Pm143,Pm152
#endif /* __ICCARM__ */


/** @addtogroup W25Q32_Driver W25Q32 SPI-Flash Driver
 *  @{
 *
 * @brief SPI-Flash driver for the Winbond W25Q32.
 *
 * @details The driver provides high-level memory access functions that abstract
 * the underlying communication protocol between the processor and the W25Q32.
 *
 * @note The application must include "drivers/spi-flash/w25q32/adi_w25q32.h" to 
 *       use this driver. The application must include src/drivers/spi/adi_spi.c
 *       in order to avoid linker errors.
 */


/*! \cond PRIVATE */


/* Includes */
#include <string.h>
#include <stdint.h>
#include "adi_w25q32_def.h"
#include <drivers/spi/adi_spi.h>
#include <drivers/spi-flash/w25q32/adi_w25q32.h>


/* Prototypes */
static ADI_W25Q32_RESULT busyWait(ADI_W25Q32_HANDLE hDevice);
static ADI_W25Q32_RESULT command (ADI_W25Q32_HANDLE hDevice, uint8_t nCommand, uint8_t * rxBuffer, uint32_t rxBytes);


/*! \endcond */


/*********************************************************************************
                                    API IMPLEMENTATIONS 
*********************************************************************************/


/*!
 * @brief       Initialize Driver
 *
 * @details     Initializes the SPI driver for communication with the W25Q32
 *
 * @param [in]  pDevMemory : Memory allocated for driver, must be ADI_W25Q32_MEMORY_SIZE bytes
 *
 * @param [in]  nMemorySize : Amount of memory allocated for driver 
 *
 * @param [out] phDevice : Pointer to the handle the application will use to reference the driver instance
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_INSUFFICIENT_MEMORY: nMemorySize is too small.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *                  - #ADI_W25Q32_FAILED_BAD_PART_INFO: Did not read the expected part info, check SPI connection.
 *
 * @note        This function must be called before any other functions are called
 *
 */
ADI_W25Q32_RESULT adi_w25q32_Open(void *pDevMemory, uint32_t nMemorySize, ADI_W25Q32_HANDLE * phDevice)
{
    ADI_W25Q32_HANDLE hDevice;
    ADI_SPI_RESULT   eSpiResult;
    uint8_t          rxBuffer[3u];

#ifdef ADI_DEBUG
    if (nMemorySize != sizeof(struct __ADI_W25Q32_DATA_TYPE))
    {
        return ADI_W25Q32_FAILED_INSUFFICIENT_MEMORY;
    }
#endif

    /* Clear the user provided memory */
    memset(pDevMemory, 0, nMemorySize); 

    /* Map the user provided memory to the driver data structure */
    hDevice = pDevMemory;
  
    eSpiResult = adi_spi_Open(ADI_W25Q32_CFG_SPI_DEVICE_NUM, hDevice->aDeviceMemory, ADI_SPI_MEMORY_SIZE, &(hDevice->hSpiDevice));
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    
    eSpiResult = adi_spi_SetChipSelect(hDevice->hSpiDevice, ADI_W25Q32_CFG_SPI_CS_NUM);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);

    /* Check the device and manf id */
    command(hDevice, ADI_W25Q32_CMD_ID, rxBuffer, 3u);
    RETURN_ERROR(rxBuffer[0u], ADI_W25Q32_MANF_ID,   ADI_W25Q32_FAILED_BAD_PART_INFO);
    RETURN_ERROR(rxBuffer[1u], ADI_W25Q32_DEVI_ID_H, ADI_W25Q32_FAILED_BAD_PART_INFO);
    RETURN_ERROR(rxBuffer[2u], ADI_W25Q32_DEVI_ID_L, ADI_W25Q32_FAILED_BAD_PART_INFO);

    /* Make the callers handle map to the device data structure */
    *phDevice = hDevice;

    return ADI_W25Q32_SUCCESS;
}


/*!
 * @brief       Unitialize Driver
 *
 * @param [in]  hDevice : Device handle
 *
 * @details     Uninitializes the SPI driver that was used for communication with the W25Q32
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *
 */
ADI_W25Q32_RESULT adi_w25q32_Close(ADI_W25Q32_HANDLE hDevice)
{
    ADI_SPI_RESULT eSpiResult;
    
    eSpiResult = adi_spi_Close(hDevice->hSpiDevice);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    
    return ADI_W25Q32_SUCCESS;
}


/*!
 * @brief       Write Data
 *
 * @details     Prepares a transaction using the W25Q32 command + address protocol and sends it 
 *              over the SPI bus, no data is received
 *
 * @param [in]  hDevice : Device handle
 *
 * @param [in]  buf : Data to send (allocated by caller)
 *
 * @param [in]  bufSize : Number of valid bytes in buf (must be <= #ADI_W25Q32_PAGE_SIZE)
 * 
 * @param [in]  address : Address to write to (must be <= #ADI_W25Q32_PAGE_SIZE * #ADI_W25Q32_NUM_PAGES) 
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *                  - #ADI_W25Q32_FAILED_BAD_BUFFER_SIZE: Buffer size greater than one page.
 *                  - #ADI_W25Q32_FAILED_BAD_ADDRESS: Address greater than entire address space.
 *
 * @note        User must be erase [address : address + bufSize] before writing
 *
 * @note        Can only write up to one page at a time for driver memory constraints
 *
 */
ADI_W25Q32_RESULT adi_w25q32_Write(ADI_W25Q32_HANDLE hDevice, uint8_t * buf, uint32_t bufSize, uint32_t address)
{
    ADI_SPI_TRANSCEIVER transceive; 
    ADI_W25Q32_RESULT   eResult;
    ADI_SPI_RESULT      eSpiResult;
    uint32_t            nHwError;

#ifdef ADI_DEBUG
    /* Verify no buffer overflow */
    if (bufSize > ADI_W25Q32_PAGE_SIZE) 
    {
        return ADI_W25Q32_FAILED_BAD_BUFFER_SIZE;
    }

    /* Verify valid address */
    if (address > (ADI_W25Q32_PAGE_SIZE*ADI_W25Q32_NUM_PAGES))
    {
        return ADI_W25Q32_FAILED_BAD_ADDRESS;
    }
#endif
    
    /* Turn on write enable */
    eResult = command(hDevice, ADI_W25Q32_CMD_WENAB, NULL, 0u);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    
    transceive.TransmitterBytes = (uint16_t) (ADI_W25Q32_PROLOGUE + bufSize);
    transceive.ReceiverBytes    = 0u;
    transceive.nTxIncrement     = 1u;
    transceive.nRxIncrement     = 1u;
    transceive.bDMA             = true;
    transceive.bRD_CTL          = false;
    transceive.pTransmitter     = hDevice->aTransmit;
    transceive.pReceiver        = NULL;
    
    /* Fill TX buffer */
    hDevice->aTransmit[0u] = ADI_W25Q32_CMD_WRITE;
    hDevice->aTransmit[1u] = (uint8_t)((address & 0x00FF0000UL) >> 16u);
    hDevice->aTransmit[2u] = (uint8_t)((address & 0x0000FF00UL) >> 8);
    hDevice->aTransmit[3u] = (uint8_t) (address & 0x000000FFUL);
    for (uint32_t i = 0u; i < bufSize; i++) 
    {
        hDevice->aTransmit[ADI_W25Q32_PROLOGUE + i] = buf[i];
    }  
    
    eSpiResult = adi_spi_MasterSubmitBuffer(hDevice->hSpiDevice, &transceive);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    eSpiResult = adi_spi_GetBuffer(hDevice->hSpiDevice, &nHwError);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    
    /* Wait for write to complete */
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);

    return ADI_W25Q32_SUCCESS;
}


/*!
 * @brief       Read Data 
 *
 * @details     Prepares a transaction using the W25Q32 command + address protocol and sends it 
 *              over the SPI bus, data is received directly into the user provided buffer
 *
 * @param [in]  hDevice : Device handle
 *
 * @param [in]  buf : Data to send (allocated by caller)
 *
 * @param [in]  bufSize : Number of bytes to read into buf
 * 
 * @param [in]  address : Address to write to (must be <= #ADI_W25Q32_PAGE_SIZE * #ADI_W25Q32_NUM_PAGES) 
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *                  - #ADI_W25Q32_FAILED_BAD_ADDRESS: Address greater than entire address space.
 *
 */
ADI_W25Q32_RESULT adi_w25q32_Read(ADI_W25Q32_HANDLE hDevice, uint8_t * buf, uint32_t bufSize, uint32_t address)
{
    ADI_SPI_TRANSCEIVER transceive; 
    ADI_SPI_RESULT      eSpiResult;
    uint32_t            nHwError;

#ifdef ADI_DEBUG
    /* Verify valid address */
    if (address > (ADI_W25Q32_PAGE_SIZE*ADI_W25Q32_NUM_PAGES))
    {
        return ADI_W25Q32_FAILED_BAD_ADDRESS;
    }    
#endif    
    
    transceive.TransmitterBytes = ADI_W25Q32_PROLOGUE;
    transceive.ReceiverBytes    = (uint16_t) bufSize;
    transceive.nTxIncrement     = 1u;
    transceive.nRxIncrement     = 1u;
    transceive.bDMA             = true;
    transceive.bRD_CTL          = true;
    transceive.pTransmitter     = hDevice->aTransmit;
    transceive.pReceiver        = buf;
    
    /* Fill TX buffer */
    hDevice->aTransmit[0u] = ADI_W25Q32_CMD_READ;
    hDevice->aTransmit[1u] = (uint8_t)((address & 0x00FF0000UL) >> 16u);
    hDevice->aTransmit[2u] = (uint8_t)((address & 0x0000FF00UL) >> 8);
    hDevice->aTransmit[3u] = (uint8_t) (address & 0x000000FFUL);
    for (uint32_t i = 0u; i < ADI_W25Q32_PAGE_SIZE; i++) 
    {
        hDevice->aTransmit[ADI_W25Q32_PROLOGUE + i] = 0u;
    }

    /* Clear RX buffer */
    memset(buf, 0, bufSize);

    /* Perform transfer */
    eSpiResult = adi_spi_MasterSubmitBuffer(hDevice->hSpiDevice, &transceive);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    eSpiResult = adi_spi_GetBuffer(hDevice->hSpiDevice, &nHwError);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
    
    return ADI_W25Q32_SUCCESS;
}


/*!
 * @brief       Erase Sector
 *
 * @details     Erases a sector of memory in the W25Q32
 *
 * @param [in]  hDevice : Device handle
 *
 * @param [in]  address : Address to erase (must be <= #ADI_W25Q32_PAGE_SIZE * #ADI_W25Q32_NUM_PAGES)
 *
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *                  - #ADI_W25Q32_FAILED_BAD_ADDRESS: Address greater than entire address space or unaligned.
 *
 * @note        Address must be sector aligned
 */
ADI_W25Q32_RESULT adi_w25q32_SectorErase(ADI_W25Q32_HANDLE hDevice, uint32_t address)
{
    ADI_SPI_TRANSCEIVER transceive; 
    ADI_W25Q32_RESULT   eResult;
    ADI_SPI_RESULT      eSpiResult;
    
#ifdef ADI_DEBUG
    /* Verify valid address */
    if (address > (ADI_W25Q32_PAGE_SIZE*ADI_W25Q32_NUM_PAGES))
    {
        return ADI_W25Q32_FAILED_BAD_ADDRESS;
    }    

    /* Verify alignment */
    if ((address % ADI_W25Q32_SECT_SIZE) != 0x0u)
    {
        return ADI_W25Q32_FAILED_BAD_ADDRESS;
    }
#endif    

    /* Turn on write enable */
    eResult = command(hDevice, ADI_W25Q32_CMD_WENAB, NULL, 0u);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    
    transceive.TransmitterBytes = ADI_W25Q32_PROLOGUE;
    transceive.ReceiverBytes    = 0u;
    transceive.nTxIncrement     = 1u;
    transceive.nRxIncrement     = 1u;
    transceive.bDMA             = false;
    transceive.bRD_CTL          = false;
    transceive.pTransmitter     = hDevice->aTransmit;
    transceive.pReceiver        = NULL;
    
    /* Fill TX buffer */
    hDevice->aTransmit[0u] = ADI_W25Q32_CMD_ERASE;
    hDevice->aTransmit[1u] = (uint8_t)((address & 0x00FF0000UL) >> 16u);
    hDevice->aTransmit[2u] = (uint8_t)((address & 0x0000FF00UL) >> 8);
    hDevice->aTransmit[3u] = (uint8_t) (address & 0x000000FFUL);
    for (uint32_t i = 0u; i < ADI_W25Q32_PAGE_SIZE; i++) 
    {
        hDevice->aTransmit[ADI_W25Q32_PROLOGUE + i] = 0u;
    }    
     
    /* Perform transfer */
    eSpiResult = adi_spi_MasterReadWrite(hDevice->hSpiDevice, &transceive);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);
      
    /* Wait for erase to complete */
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);

    return ADI_W25Q32_SUCCESS;
}


/*!
 * @brief       Erase Chip
 *
 * @details     Erase all memory on the W25Q32
 *
 * @param [in]  hDevice : Device handle
 *
 * @return      ADI_W25Q32_RESULT
 *                  - #ADI_W25Q32_SUCCESS: Function call completed successfully.
 *                  - #ADI_W25Q32_FAILED_SPI_ERROR: SPI driver reported an error.
 *
 * @note        This operation takes about 10-15 seconds to complete
 *
 */
ADI_W25Q32_RESULT adi_w25q32_ChipErase(ADI_W25Q32_HANDLE hDevice)
{
    ADI_W25Q32_RESULT eResult;
    
    /* Turn on write enable */
    eResult = command(hDevice, ADI_W25Q32_CMD_WENAB, NULL, 0u);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    
    /* Issue erase command */
    eResult = command(hDevice, ADI_W25Q32_CMD_ERASE_CHIP, NULL, 0u);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
      
    /* Wait for erase to complete */
    eResult = busyWait(hDevice);
    RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);

    return ADI_W25Q32_SUCCESS;
}


/*********************************************************************************
                                   PRIVATE FUNCTIONS
*********************************************************************************/


/*! \cond PRIVATE */


static ADI_W25Q32_RESULT command(ADI_W25Q32_HANDLE hDevice, uint8_t nCommand, uint8_t * rxBuffer, uint32_t rxBytes)
{
    ADI_SPI_TRANSCEIVER transceive;
    ADI_SPI_RESULT      eSpiResult;
    uint8_t             txBuffer;
    
    transceive.TransmitterBytes = 1u;
    transceive.ReceiverBytes    = (uint16_t) rxBytes;
    transceive.nTxIncrement     = 1u;
    transceive.nRxIncrement     = 1u;
    transceive.bDMA             = false;
    transceive.pTransmitter     = &txBuffer;
    transceive.pReceiver        = rxBuffer;
    if (rxBytes != 0u) 
    {
        transceive.bRD_CTL = true;
    } 
    else 
    {
        transceive.bRD_CTL = false;
    }

    /* Fill TX buffer */
    txBuffer = nCommand;
        
    /* Clear RX buffer */
    memset(rxBuffer, 0, rxBytes);

    /* Perform transfer */
    eSpiResult = adi_spi_MasterReadWrite(hDevice->hSpiDevice, &transceive);
    RETURN_ERROR(eSpiResult, ADI_SPI_SUCCESS, ADI_W25Q32_FAILED_SPI_ERROR);

    return ADI_W25Q32_SUCCESS;
}


static ADI_W25Q32_RESULT busyWait(ADI_W25Q32_HANDLE hDevice)
{
    ADI_W25Q32_RESULT eResult;
    uint8_t           rxBuffer;
    do 
    {
        /* Wait for the busy bit to clear */
        eResult = command(hDevice, ADI_W25Q32_CMD_RDSR, &rxBuffer, 1u);
        RETURN_ERROR(eResult, ADI_W25Q32_SUCCESS, eResult);
    } while((rxBuffer & ADI_W25Q32_STAT_BSY) == ADI_W25Q32_STAT_BSY);

    return ADI_W25Q32_SUCCESS;
}

/*! \endcond */

/*! @} */
