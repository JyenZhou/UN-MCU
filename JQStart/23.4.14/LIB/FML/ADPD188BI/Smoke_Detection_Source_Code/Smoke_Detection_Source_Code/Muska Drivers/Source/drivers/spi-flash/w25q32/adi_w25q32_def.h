/*!
 *****************************************************************************
 * @file:    adi_w25q32.c
 * @brief:   W25Q32 driver definition
 *****************************************************************************
Copyright (c) 2010-2016 Analog Devices, Inc.

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


#ifndef ADI_W25Q32_DEF_H
#define ADI_W25Q32_DEF_H


/*! \cond PRIVATE */


#include <drivers/spi/adi_spi.h>
#include <drivers/spi-flash/w25q32/adi_w25q32.h>


/* W25Q32 status reigster bit mask */
#define ADI_W25Q32_STAT_BSY       (0x01u)


/* W25Q32 device identifiers */
#define ADI_W25Q32_MANF_ID        (0xEFu)
#define ADI_W25Q32_DEVI_ID_H      (0x40u)
#define ADI_W25Q32_DEVI_ID_L      (0x16u)


/* W25Q32 device commands */
#define ADI_W25Q32_CMD_ID         (0x9Fu)
#define ADI_W25Q32_CMD_ERASE      (0x20u)
#define ADI_W25Q32_CMD_ERASE_CHIP (0xC7u)
#define ADI_W25Q32_CMD_RDSR       (0x05u)
#define ADI_W25Q32_CMD_WENAB      (0x06u)
#define ADI_W25Q32_CMD_WRITE      (0x02u)
#define ADI_W25Q32_CMD_READ       (0x03u)


/* W25Q32 prologue size: 1 byte command + 3 bytes address */
#define ADI_W25Q32_PROLOGUE       (0x04u)


 /* Helper macro */
#define RETURN_ERROR(result, expected, value) \
do {\
    if ((expected) != (result))\
    {\
        return (value); \
    }\
} while (0)


typedef struct __ADI_W25Q32_DATA_TYPE
{
    ADI_SPI_HANDLE hSpiDevice;
    uint8_t        aDeviceMemory[ADI_SPI_MEMORY_SIZE];
    uint8_t        aTransmit    [ADI_W25Q32_PROLOGUE + ADI_W25Q32_PAGE_SIZE];

} ADI_W25Q32_DATA_TYPE;


/*! \endcond */


#endif /* ADI_W25Q32_DEF_H */
