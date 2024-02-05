/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-30 15:34:38
 * @Disclaimer:
 *  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 *  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 *  * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 *  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 *  * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved.
 */
#include "tc04xx_flash.h"
#include "system_tc04xx.h"

#define     FLASH_WCT2_DEFAULT_CFG          (0xB013DAB7U)

/**
  * @brief    FLASH init function
  *
  * @param    None
  *
  * @return   None
  *
  * @details
  */
void Flash_Init(void)
{
    FLASH->WCTC2 = FLASH_WCT2_DEFAULT_CFG;//enable the write and erase
}

/**
  * @brief    FLASH deinit function
  *
  * @param    None
  *
  * @return   None
  *
  * @details
  */
void Flash_Deinit(void)
{
    FLASH->WCTC2 = 0;
}

#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_Erase(uint8_t wIndex)
{
    if (wIndex > (FLASH_SIZE / FLASH_SECTOR_SIZE))
    {
        return;
    }
    interrupt_disable();
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0;
    FLASH->ECONF_F.CONFIG_SECTOR_INDEX = (wIndex & 0x7FU);
    FLASH->ECOMM_F.CMD_SECTOR_ERASE = 1;

    while (FLASH->MCR_F.STAT_ERASE_BUSY == 1)
    {

    }

    interrupt_enable();
}
#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_NVR_Erase(uint8_t wIndex)
{
    if (wIndex > (FLASH_NVR_SIZE / FLASH_SECTOR_SIZE))
    {
        return;
    }
    interrupt_disable();

    FLASH->ECONF_F.CONFIG_NVR_SELECT = 1;
    FLASH->ECONF_F.CONFIG_SECTOR_INDEX = (wIndex & 0x7FU);
    FLASH->ECOMM_F.CMD_SECTOR_ERASE = 1;

    while (FLASH->MCR_F.STAT_ERASE_BUSY == 1)
    {

    }
    interrupt_enable();
}

/**
 * @brief       Read 32-bit Data from specified address of flash
 *
 * @param[in]   u32Addr  Flash address include Main memory
 *
 * @return      The data of specified address
 *
 * @details     To read word data from Flash.
 *
 */
uint32_t Flash_Read(uint32_t u32Addr)
{
    uint32_t nReadData = 0;

    if (u32Addr > (FLASH_SIZE))
    {
        return 0;
    }
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x0;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint32_t *)(FLASH_ADDR + u32Addr);

    return nReadData;
}

uint16_t Flash_Read16(uint32_t u32Addr)
{
    uint16_t nReadData = 0;

    if (u32Addr > (FLASH_SIZE))
    {
        return 0;
    }
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x0;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint16_t *)(FLASH_ADDR + u32Addr);

    return nReadData;
}

/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  u32Addr  Flash address in Main memory
 * @param[in]  u32Data  32-bit Data to program
 *
 * @return     None
 *
 * @details    To program word data into Main memory
 *
 */
#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_Write(uint32_t u32Addr, uint32_t u32Data)
{
    if (u32Addr > (FLASH_SIZE))
    {
        return;
    }
    interrupt_disable();

    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x0;         /* Main memory (NVM) selected */
    *(volatile uint32_t *)(FLASH_ADDR + u32Addr) = u32Data;
    while (FLASH->MCR_F.STAT_PROGRAM_BUSY == 1)
    {

    }
    interrupt_enable();
}


#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_Write16(uint32_t u32Addr, uint16_t u16Data)
{
    if (u32Addr > (FLASH_SIZE))
    {
        return;
    }
    interrupt_disable();
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x0;        /* Main memory (NVM) selected */
    *(volatile uint16_t *)(FLASH_ADDR + u32Addr) = u16Data;
    while (FLASH->MCR_F.STAT_PROGRAM_BUSY == 1)
    {

    }
    interrupt_enable();
}

/**
 * @brief       Read 32-bit Data from specified address of flash
 *
 * @param[in]   u32Addr  Flash address include Main memory
 *
 * @return      The data of specified address
 *
 * @details     To read word data from Flash.
 *
 */
uint32_t Flash_NVR_Read(uint32_t u32Addr)
{
    uint32_t nReadData = 0;

    if (u32Addr > (FLASH_SIZE))
    {
        return 0;
    }
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x1;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint32_t *)(FLASH_NVR_ADDR + u32Addr);

    return nReadData;
}
uint16_t Flash_NVR_Read16(uint32_t u32Addr)
{
    uint16_t nReadData = 0;

    if (u32Addr > (FLASH_SIZE))
    {
        return 0;
    }
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x1;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint16_t *)(FLASH_NVR_ADDR + u32Addr);

    return nReadData;
}
/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  u32Addr  Flash address in Main memory
 * @param[in]  u32Data  32-bit Data to program
 *
 * @return     None
 *
 * @details    To program word data into Main memory
 *
 */
#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_NVR_Write(uint32_t u32Addr, uint32_t u32Data)
{
    if (u32Addr > (FLASH_NVR_SIZE))
    {
        return;
    }
    interrupt_disable();
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x1;         /* Main memory (NVM) selected */
    *(volatile uint32_t *)(FLASH_NVR_ADDR + u32Addr) = u32Data;
    while (FLASH->MCR_F.STAT_PROGRAM_BUSY == 1)
    {

    }
    interrupt_enable();
}


#if defined   ( __ICCARM__   ) /* iar */
    #pragma location = "RAMUSERCODE"
#elif defined ( __CC_ARM )     /* keil */
    __attribute__((section("RAMCODE")))
#endif
void Flash_NVR_Write16(uint32_t u32Addr, uint16_t u16Data)
{
    if (u32Addr > (FLASH_NVR_SIZE))
    {
        return;
    }

    interrupt_disable();
    FLASH->ECONF_F.CONFIG_NVR_SELECT = 0x1;        /* Main memory (NVM) selected */
    *(volatile uint16_t *)(FLASH_NVR_ADDR + u32Addr) = u16Data;
    while (FLASH->MCR_F.STAT_PROGRAM_BUSY == 1)
    {

    }
    interrupt_enable();
}


/**
  * @brief    Flash_WriteByte
  *
  ** @param [u8]    addr: 0x0-FLASH_MAXSIZE
                    notice : the addr must satisfy in multiples of four

  ** \param [u8]    pBuf: write buf

  ** \param [u8]    Size: write buf size

  * @return   None
  *
  * @details
  */
void Flash_WriteByte(uint32_t addr, uint8_t *pBuf, uint16_t Size)
{
    uint32_t wr_data = 0;
    uint32_t idx = 0;
    uint32_t addrstep = addr;

    if (((addr % 4) != 0) || ((addr + Size) >  FLASH_SIZE) || (pBuf == NULL))
    {
        return;
    }

    while (idx < Size)
    {
        memcpy((void*)&wr_data, (void*)&pBuf[idx], 4);
        Flash_Write(addrstep, wr_data);
        addrstep = addrstep + 4;
        idx = idx + 4;
    }
}

/**
  * @brief    Flash_ReadByte
  *
  ** @param [u8]    addr: 0x0-FLASH_MAXSIZE
                    notice : the addr must satisfy in multiples of four

  ** \param [u8]    pBuf: read buf

  ** \param [u8]    Size: read buf size

  * @return   None
  *
  * @details
  */
void Flash_ReadByte(uint32_t addr, uint8_t *pBuf, uint16_t Size)
{
    uint32_t rd_data = 0;
    uint32_t idx = 0;
    uint32_t addrstep = addr;

    if (((addr % 4) != 0) || ((addr + Size) >  FLASH_SIZE) || (pBuf == NULL))
    {
        return;
    }

    while (idx < Size)
    {
        rd_data = Flash_Read(addrstep);
        if (Size - idx > sizeof(rd_data))
        {
            memcpy((void*)&pBuf[idx], (void*)&rd_data, sizeof(rd_data));
            idx = idx + sizeof(rd_data);
        }
        else
        {
            memcpy((void*)&pBuf[idx], (void*)&rd_data, Size - idx);
            idx = idx + (Size - idx);
        }
        addrstep = addrstep + 4;

    }
}

/**
  * @brief    Flash_NVR_ReadByte
  *
  ** @param [u8]    addr: 0x0-NVR_FLASH_MAXSIZE
                    notice : the addr must satisfy in multiples of four

  ** \param [u8]    pBuf: read buf

  ** \param [u8]    Size: read buf size

  * @return   None
  *
  * @details caution：the NVR sector 1 and sector 6 is used to store the chip information
  */
void Flash_NVR_ReadByte(uint32_t addr, uint8_t *pBuf, uint16_t Size)
{
    uint32_t rd_data = 0;
    uint32_t idx = 0;
    uint32_t addrstep = addr;

    if (((addr % 4) != 0) || ((addr + Size) >  FLASH_NVR_SIZE) || (pBuf == NULL))
    {
        return;
    }

    while (idx < Size)
    {
        rd_data = Flash_NVR_Read(addrstep);
        if (Size - idx > sizeof(rd_data))
        {
            memcpy((void*)&pBuf[idx], (void*)&rd_data, sizeof(rd_data));
            idx = idx + sizeof(rd_data);
        }
        else
        {
            memcpy((void*)&pBuf[idx], (void*)&rd_data, Size - idx);
            idx = idx + (Size - idx);
        }
        addrstep = addrstep + 4;

    }
}

/**
  * @brief    Flash_NVR_WriteByte
  *
  ** @param [u8]    addr: 0x0-NVR_FLASH_MAXSIZE
                    notice : the addr must satisfy in multiples of four

  ** \param [u8]    pBuf: write buf

  ** \param [u8]    Size: write buf size

  * @return   None
  *
  * @details caution：the NVR sector 1 and sector 6 is used to store the chip information
  */
void Flash_NVR_WriteByte(uint32_t addr, uint8_t *pBuf, uint16_t Size)
{
    uint32_t wr_data = 0;
    uint32_t idx = 0;
    uint32_t addrstep = addr;

    if (((addr % 4) != 0) || ((addr + Size) >  FLASH_NVR_SIZE) || (pBuf == NULL))
    {
        return;
    }

    while (idx < Size)
    {
        memcpy((void*)&wr_data, (void*)&pBuf[idx], 4);
        Flash_NVR_Write(addrstep, wr_data);
        addrstep = addrstep + 4;
        idx = idx + 4;
    }

}
