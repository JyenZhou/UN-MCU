/***********************************************************************************************************************
    @file     hal_spi.c
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
              MM32 FIRMWARE LIBRARY.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#define _HAL_SPI_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_spi.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup SPI_HAL
  * @{
  */

/** @addtogroup SPI_Exported_Functions
  * @{
  */

/**
  * @brief  Deinitializes the spi peripheral registers to their
  *         default reset values.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @retval None.
  */
void SPI_DeInit(SPI_TypeDef* spi)
{
    if(SPI1 == spi) {
        RCC_APB1PeriphResetCmd(RCC_APB1RSTR_SPI1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1RSTR_SPI1, DISABLE);
    }
}

/**
  * @brief  Initializes the spi peripheral according to the specified
  *         parameters in the init_struct .
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  init_struct: pointer to a SPI_InitTypeDef structure
  *         that contains the configuration information for the
  *         specified SPI peripheral.
  * @retval None.
  */
void SPI_Init(SPI_TypeDef* spi, SPI_InitTypeDef* init_struct)
{
    WRITE_REG(spi->RDNR, 0);
    if (init_struct->SPI_DataSize == SPI_DataSize_32b) {
        SET_BIT(spi->GCR, SPI_GCR_DWSEL);
    }
    else {
        CLEAR_BIT(spi->GCR, SPI_GCR_DWSEL);
    }
    MODIFY_REG(spi->GCR, SPI_GCR_NSS, init_struct->SPI_NSS);
    MODIFY_REG(spi->GCR, SPI_GCR_MODE, init_struct->SPI_Mode);
    MODIFY_REG(spi->CCR, SPI_CCR_LSBFE, init_struct->SPI_FirstBit);
    MODIFY_REG(spi->CCR, SPI_CCR_CPOL, init_struct->SPI_CPOL);
    MODIFY_REG(spi->CCR, SPI_CCR_CPHA, init_struct->SPI_CPHA);

    SET_BIT(spi->CCR, SPI_CCR_SPILEN);

    MODIFY_REG(spi->BRR, BRR_Mask, init_struct->SPI_BaudRatePrescaler);

    if (init_struct->SPI_DataWidth >= 32) {
        MODIFY_REG(spi->ECR, ECR_Mask, 0);
    }
    else {
        MODIFY_REG(spi->ECR, ECR_Mask, init_struct->SPI_DataWidth);
    }
}

/**
  * @brief  Fills each init_struct member with its default value.
  * @param  init_struct: pointer to a SPI_InitTypeDef structure
  *         which will be initialized.
  * @retval None.
  */
void SPI_StructInit(SPI_InitTypeDef* init_struct)
{
    init_struct->SPI_Mode              = SPI_Mode_Slave;
    init_struct->SPI_DataSize          = SPI_DataSize_8b;
    init_struct->SPI_DataWidth         = 8;
    init_struct->SPI_CPOL              = SPI_CPOL_Low;
    init_struct->SPI_CPHA              = SPI_CPHA_1Edge;
    init_struct->SPI_NSS               = SPI_NSS_Soft;
    init_struct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    init_struct->SPI_FirstBit          = SPI_FirstBit_MSB;
}

/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  state: new state of the spi peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void SPI_Cmd(SPI_TypeDef* spi, FunctionalState state)
{
    (state) ? SET_BIT(spi->GCR, SPI_GCR_SPIEN) : CLEAR_BIT(spi->GCR, SPI_GCR_SPIEN);
}

/**
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:SPI1, SPI2.
  * @param  interrupt: specifies the SPI interrupt sources to be
  *         enabled or disabled.
  *         This parameter can be one of the following values:
  * @arg    SPI_IT_TXEPT: Transmitter empty interrupt
  * @arg    SPI_IT_RXFULL: RX FIFO full interrupt
  * @arg    SPI_IT_RXMATCH: Receive data match the RXDNR number interrupt
  * @arg    SPI_IT_RXOERR: Receive overrun error interrupt
  * @arg    SPI_IT_UNDERRUN: underrun interrupt
  * @arg    SPI_IT_RX: Receive data available interrupt
  * @arg    SPI_IT_TX: Transmit FIFO available interrupt
  * @param  state: new state of the specified spi interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void SPI_ITConfig(SPI_TypeDef* spi, uint8_t interrupt, FunctionalState state)
{
    if (state) {
        SET_BIT(spi->GCR, (uint32_t)SPI_GCR_IEN);
        SET_BIT(spi->IER, (uint32_t)interrupt);
    }
    else {
        CLEAR_BIT(spi->IER, interrupt);
        CLEAR_BIT(spi->GCR, SPI_GCR_IEN);
    }
}

/**
  * @brief  configure tn Fifo trigger level bit.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  fifo_trigger_value: specifies the Fifo trigger level
  *         This parameter can be any combination of the following values:
  *         SPI_TXTLF : SPI TX FIFO Trigger value set
  *         SPI_RXTLF : SPI RX FIFO Trigger value set
  * @param  state: new state of the selected SPI transfer request.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void SPI_FifoTrigger(SPI_TypeDef* spi, SPI_TLF_TypeDef fifo_trigger_value, FunctionalState state)
{
    (state) ? SET_BIT(spi->GCR, (uint32_t)fifo_trigger_value) : CLEAR_BIT(spi->GCR, (uint32_t)fifo_trigger_value);
}

/**
  * @brief  Transmits a Data through the spi peripheral.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  data : Data to be transmitted.
  * @retval None.
  */
void SPI_SendData(SPI_TypeDef* spi, uint32_t data)
{
    u16 templen;
    __asm volatile("cpsid i");

    WRITE_REG(spi->TDR, data);

    templen = READ_REG(spi->ECR);
    if(templen  == 0)
        templen = 32;
    if (templen > 8)
        WRITE_REG(spi->TDR, data >> 8);
    if (templen > 16)
        WRITE_REG(spi->TDR, data >> 16);
    if (templen > 24)
        WRITE_REG(spi->TDR, data >> 24);
    __asm volatile("cpsie i");
}


/**
  * @brief  Returns the most recent received data by the spi peripheral.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @retval The value of the received data.
  */
uint32_t SPI_ReceiveData(SPI_TypeDef* spi)
{
    uint32_t temp;
    uint8_t templen;
    __asm volatile("cpsid i");

    temp = READ_REG(spi->RDR);

    templen = READ_REG(spi->ECR);
    if(templen  == 0)
        templen = 32;
    if (templen > 8)
        temp |= (uint32_t)(READ_REG(spi->RDR) << 8);
    if (templen > 16)
        temp |= (uint32_t)(READ_REG(spi->RDR) << 16);
    if (templen > 24)
        temp |= (uint32_t)(READ_REG(spi->RDR) << 24);

    __asm volatile("cpsie i");

    return temp;
}

/**
  * @brief  Slave chip csn single by selected
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  state: new state of the selected SPI CS pin
  *         request.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void SPI_CSInternalSelected(SPI_TypeDef* spi, FunctionalState state)
{
    (state) ? CLEAR_BIT(spi->NSSR, SPI_NSSR_NSS) : SET_BIT(spi->NSSR, SPI_NSSR_NSS);  /*!< illogical */
}

/**
  * @brief  Configures the NSS pin control mode for the selected SPI.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  nss: specifies the SPI NSS internal state.
  *         This parameter can be one of the following values:
  * @arg    SPI_NSS_Soft: NSS pin control by software
  * @arg    SPI_NSS_Hard: NSS pin control by hardware
  * @retval None.
  */
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* spi, SPI_NSS_TypeDef nss)
{
    (nss != SPI_NSS_Soft) ? SET_BIT(spi->GCR, SPI_NSS_Hard) : CLEAR_BIT(spi->GCR, SPI_NSS_Hard);
}

/**
  * @brief  Configures the data size for the selected SPI.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  data_size: specifies the SPI data size.
  *         This parameter can be one of the following values:
  *         0 to 31, 0 = 32b, 1 = 1b, 2 = 2b
  * @arg    DataSize : 0 to 31
  * @retval None.
  * @retval None.
  */
bool SPI_DataSizeConfig(SPI_TypeDef* spi, uint8_t data_size)
{
    if (data_size > 32)
        return false;
    data_size &= 0x1F;
    WRITE_REG(spi->ECR, data_size);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void SPI_DataSizeTypeConfig(SPI_TypeDef* spi, SPI_DataSize_TypeDef SPI_DataSize)
{
    CLEAR_BIT(spi->GCR, (uint32_t)SPI_DataSize_32b);
    SET_BIT(spi->GCR, (uint32_t)SPI_DataSize);
}

/**
  * @brief  Selects the data transfer direction in bi-directional mode
  *         for the specified SPI.
  * @param spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param direction: specifies the data transfer direction in
  *         bi-directional mode.
  *         This parameter can be one of the following values:
  * @arg    SPI_Direction_Tx: Selects Tx transmission direction
  * @arg    SPI_Direction_Rx: Selects Rx receive direction
  * @arg    SPI_Disable_Tx: Selects Rx receive direction
  * @arg    SPI_Disable_Rx: Selects Rx receive direction
  * @retval None.
  */
void SPI_BiDirectionalLineConfig(SPI_TypeDef* spi, SPI_Direction_TypeDef direction)
{
    switch (direction) {
        case SPI_Direction_Rx:
            SET_BIT(spi->GCR, SPI_GCR_RXEN);
            break;
        case SPI_Direction_Tx:
            SET_BIT(spi->GCR, SPI_GCR_TXEN);
            break;
        case SPI_Disable_Rx:
            CLEAR_BIT(spi->GCR, SPI_GCR_RXEN);
            break;
        case SPI_Disable_Tx:
            CLEAR_BIT(spi->GCR, SPI_GCR_TXEN);
            break;
        default:
            break;
    }
}

/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  flag: specifies the SPI flag to check.
  *         This parameter can be one of the following values:
  * @arg    SPI_FLAG_RXAVL: Rx buffer has bytes flag
  * @arg    SPI_FLAG_TXEPT: Tx buffer and tx shifter empty flag
  * @arg    SPI_FLAG_TXFULL: Tx buffer full flag
  * @arg    SPI_FLAG_RXAVL_4BYTE: Receive available 4 byte data message flag.
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* spi, SPI_FLAG_TypeDef flag)
{
/*    uint8_t number; */
    return (spi->SR & flag) ? SET : RESET;
/*  if (spi->ECR == 8 || spi->ECR == 0)
      return (spi->SR & SPI_FLAG) ? SET : RESET;
  else {
      if ((spi->ECR > 0) && (spi->ECR <= 8))
          number = 1;
      else if ((spi->ECR) <= 16)
          number = 2;
      else if ((spi->ECR) <= 24)
          number = 3;
      else if (((spi->ECR) <= 31) || (spi->ECR == 0))
          number = 4;
      return (((spi->SR & 0xf00) >> 8) >= number) ? SET : RESET;
  } */
}

/**
  * @brief  Checks whether the specified SPI interrupt has occurred or not.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  interrupt: specifies the SPI interrupt source to check.
  *         This parameter can be one of the following values:
  * @arg    SPI_IT_TX: Tx buffer empty interrupt
  * @arg    SPI_IT_RX: Rx buffer  interrupt
  * @arg    SPI_IT_UNDERRUN: under Error interrupt in slave mode
  * @arg    SPI_IT_RXOVER: RX OVER Error interrupt
  * @arg    SPI_IT_RXMATCH: spectials rx data numbers  interrupt
  * @arg    SPI_IT_RXFULL: Rx buffer full interrupt
  * @arg    SPI_IT_TXEPT: Tx buffer  and tx shifter empty interrupt
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus SPI_GetITStatus(SPI_TypeDef* spi, SPI_IT_TypeDef interrupt)
{
    return (spi->ISR & interrupt) ? SET : RESET;
}

/**
  * @brief  Clears the spi  interrupt pending bit.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  interrupt: specifies the SPI interrupt pending bit to clear.
  * @arg    SPI_IT_TX: Tx buffer empty interrupt
  * @arg    SPI_IT_RX: Rx buffer  interrupt
  * @arg    SPI_IT_UNDERRUN: under Error interrupt in slave mode
  * @arg    SPI_IT_RXOVER: RX OVER Error interrupt
  * @arg    SPI_IT_RXMATCH: spectials rx data numbers  interrupt
  * @arg    SPI_IT_RXFULL: Rx buffer full interrupt
  * @arg    SPI_IT_TXEPT: Tx buffer  and tx shifter empty interrupt
  *         This function clears only ERR intetrrupt pending bit.
  * @retval None.
  */
void SPI_ClearITPendingBit(SPI_TypeDef* spi, SPI_IT_TypeDef interrupt)
{
    SET_BIT(spi->ICR, interrupt);
}

/**
  * @brief  SPI Hole a count Received bytes in next receive process.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  number: specifies the SPI receive Number.
  *         This parament can be 1-65535.
  *         This function can use only in SPI master single receive mode.
  * @retval None.
  */
void SPI_RxBytes(SPI_TypeDef* spi, uint16_t number)
{
    WRITE_REG(spi->RDNR, number);
}

/**
  * @brief  slave mode tx data transmit phase adjust set.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  adjust_value: slave mode tx data transmit phase adjust enum.
  *         This parament can be :
  *         SPI_SlaveAdjust_FAST:  fast speed use
  *         SPI_SlaveAdjust_LOW:   low speed use
  *         This function can use only in SPI master single receive mode.
  * @retval None.
  */
void SPI_SlaveAdjust(SPI_TypeDef* spi, SPI_SlaveAdjust_TypeDef adjust_value)
{
    (adjust_value) ? SET_BIT(spi->CCR, SPI_CCR_RXEDGE) : CLEAR_BIT(spi->CCR, SPI_CCR_RXEDGE);
}

/**
  * @brief  Enables or disables all SPI interrupts.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  state: new state of all spi interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void exSPI_ITCmd(SPI_TypeDef* spi, FunctionalState state)
{
    (state) ? SET_BIT(spi->IER, (uint32_t)SPI_GCR_IEN) : CLEAR_BIT(spi->IER, (uint32_t)SPI_GCR_IEN);
}

/**
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  interrupt: specifies the SPI interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  * @arg    SPI_IT_TXEPT: Transmitter empty interrupt
  * @arg    SPI_IT_RXFULL: RX FIFO full interrupt
  * @arg    SPI_IT_RXMATCH: Receive data match the RXDNR number interrupt
  * @arg    SPI_IT_RXOERR: Receive overrun error interrupt
  * @arg    SPI_IT_UNDERRUN: underrun interrupt
  * @arg    SPI_IT_RX: Receive data available interrupt
  * @arg    SPI_IT_TX: Transmit FIFO available interrupt
  * @param  state: new state of the specified spi interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void exSPI_ITConfig(SPI_TypeDef* spi, SPI_IT_TypeDef interrupt, FunctionalState state)
{
    (state) ? SET_BIT(spi->IER, (uint32_t)interrupt) : CLEAR_BIT(spi->IER, (uint32_t)interrupt);
}

/**
  * @brief  Set or reset Slave chip csn signal output
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  state: new state of Slave chip csn signal output.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void exSPI_CSInternalSelected(SPI_TypeDef* spi, FunctionalState state)
{
    (state) ? CLEAR_BIT(spi->NSSR, SPI_NSSR_NSS) : SET_BIT(spi->NSSR, SPI_NSSR_NSS);  /*!< illogical */
}

/**
  * @brief  tx data and rx data phase adjust.
  * @param  spi: Select the SPI peripheral.
  *         This parameter can be one of the following values:
  *         SPI1, SPI2.
  * @param  adjust_value: choose adjust mode.
  *         This parament can be :
  *         SPI_DataEdgeAdjust_LOW,
  *         SPI_DataEdgeAdjust_FAST
  * @retval None.
  */
void exSPI_DataEdgeAdjust(SPI_TypeDef* spi, SPI_DataEdgeAdjust_TypeDef adjust_value)
{
    /* master mode */
    if (spi->GCR & SPI_GCR_MODE) {
        adjust_value ? SET_BIT(spi->CCR, SPI_CCR_RXEDGE) : CLEAR_BIT(spi->CCR, SPI_CCR_RXEDGE);
    }
    /* slave mode */
    else {
        adjust_value ? SET_BIT(spi->CCR, SPI_CCR_TXEDGE) : CLEAR_BIT(spi->CCR, SPI_CCR_TXEDGE);
    }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
