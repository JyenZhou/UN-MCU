/*
 *****************************************************************************
 * @file:    adi_w25q32.h
 * @brief:   Header file for adi_w25q32.c
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


#ifndef ADI_W25Q32_H
#define ADI_W25Q32_H


#include <stdint.h>
#include <stdbool.h>


/** @addtogroup W25Q32_Driver W25Q32 SPI-Flash Driver
 *  @{
 */


/*! W25Q32 driver memory size */
#define ADI_W25Q32_MEMORY_SIZE (308u)


/*! W25Q32 driver handle */
typedef struct __ADI_W25Q32_DATA_TYPE*  ADI_W25Q32_HANDLE;


/*!
 *****************************************************************************
 *  \enum ADI_W25Q32_RESULT
 *   Enumeration for result code returned from the W25Q32 driver functions.
 *   The return value of all W25Q32 APIs returning #ADI_W25Q32_RESULT should always 
 *   be tested at the application level for success or failure.
 *****************************************************************************/
typedef enum
{
  /*! Successful function call */
  ADI_W25Q32_SUCCESS,
  /*! SPI driver returned an error */
  ADI_W25Q32_FAILED_SPI_ERROR,        
  /*! Did not read expected part info on initialization */
  ADI_W25Q32_FAILED_BAD_PART_INFO,
  /*! User supplied a bad address */
  ADI_W25Q32_FAILED_BAD_ADDRESS,      
  /*! User supplied a bad buffer size */
  ADI_W25Q32_FAILED_BAD_BUFFER_SIZE,
  /*! User did not provide enough memory for the driver */
  ADI_W25Q32_FAILED_INSUFFICIENT_MEMORY,
} ADI_W25Q32_RESULT;


/************************ USER CONFIG *************************/


/*! SPI bus connected from the processor to the W25Q32 */
 #define ADI_W25Q32_CFG_SPI_DEVICE_NUM  (2u)

 /*! Chip select connected from the processor to the W25Q32 */
 #define ADI_W25Q32_CFG_SPI_CS_NUM      (ADI_SPI_CS0)


/************************ DEVICE INFO *************************/


/*! W25Q32 Page Count */
#define ADI_W25Q32_NUM_PAGES            (0x4000u)

/*! W25Q32 Sector Size */
#define ADI_W25Q32_PAGE_SIZE            (0x100u)

/*! W25Q32 Sector Size */
#define ADI_W25Q32_SECT_SIZE            (0x1000u)


/************************ PUBLIC APIs *************************/


ADI_W25Q32_RESULT adi_w25q32_Open       (void *pDevMemory, uint32_t nMemorySize, ADI_W25Q32_HANDLE * phDevice);
ADI_W25Q32_RESULT adi_w25q32_Close      (ADI_W25Q32_HANDLE hDevice);
ADI_W25Q32_RESULT adi_w25q32_Write      (ADI_W25Q32_HANDLE hDevice, uint8_t * buf, uint32_t bufSize, uint32_t address);
ADI_W25Q32_RESULT adi_w25q32_Read       (ADI_W25Q32_HANDLE hDevice, uint8_t * buf, uint32_t bufSize, uint32_t address);
ADI_W25Q32_RESULT adi_w25q32_SectorErase(ADI_W25Q32_HANDLE hDevice, uint32_t address);
ADI_W25Q32_RESULT adi_w25q32_ChipErase  (ADI_W25Q32_HANDLE hDevice);


/*! @} */


#endif /* ADI_W25Q32_H */
