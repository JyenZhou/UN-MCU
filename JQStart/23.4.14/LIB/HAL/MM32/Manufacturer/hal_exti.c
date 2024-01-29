/*
 *******************************************************************************
    @file     hal_exti.c
    @author   VV TEAM
    @brief    THIS FILE PROVIDES ALL THE EXTI FIRMWARE FUNCTIONS.
 *******************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from this
       software without specific prior written permission.

    THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
    CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
    TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
    CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
    HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
    CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.

    <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#define _HAL_EXTI_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_exti.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup EXTI_HAL
  * @{
  */

/** @addtogroup EXTI_Exported_Functions
  * @{
  */

/**
  * @brief  Deinitializes the EXTI peripheral registers to their default reset
  *         values.
  * @param  None.
  * @retval None.
  */

/**
  * @brief  Deinitializes the EXTI registers to their default reset values.
  * @param  None.
  * @retval None.
  * @note   MEM_MODE bits are not affected by APB reset.
  * @note   MEM_MODE bits took the value from the user option bytes.
  * @note   CFGR2 register is not affected by APB reset.
  * @note   CLABBB configuration bits are locked when set.
  * @note   To unlock the configuration, perform a system reset.
  */
void EXTI_DeInit(void)
{
    uint16_t i;
    /*!< Clear all */
    exEXTI_LineDisable(~0x00000000);

    /*!< rc_w1 */
    EXTI->PR = EXTI->PR;

    /*!< Set EXTI_CFGR1 register to reset value without affecting MEM_MODE bits */
    EXTI->CFGR &= EXTI_CFGR_MEMMODE;

    /*!< Set EXTICRx registers to reset value */
    for (i = 0; i < 4; i++) {
        EXTI->CR[i] = 0;
    }
}

/**
  * @brief  Configures the memory mapping at address 0x00000000.
  * @param  memory_remap: selects the memory remapping.
  *          This parameter can be one of the following values:
  *            @arg EXTI_MemoryRemap_Flash: Main Flash memory mapped at 0x00000000
  *            @arg EXTI_MemoryRemap_SystemMemory: System Flash memory mapped at 0x00000000
  *            @arg EXTI_MemoryRemap_SRAM: Embedded SRAM mapped at 0x00000000
  * @retval None.
  */
void EXTI_MemoryRemapConfig(uint32_t memory_remap)
{
    EXTI->CFGR = (EXTI->CFGR & ~EXTI_CFGR_MEMMODE) | memory_remap;
}

/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  port_source_gpio: selects the GPIO port to be used as source for EXTI lines .
  * @param  pin_source: specifies the EXTI line to be configured.
  * @note   This parameter can be pin_source where x can be:
  *         For MCU: (0..15) for GPIOA, GPIOB, (13..15) for GPIOC and (0..1, 6..7) for GPIOD.
  * @retval None.
  */
void EXTI_LineConfig(uint8_t port_source_gpio, uint8_t pin_source)
{
    EXTI->CR[pin_source >> 0x02] &= ~(0x0F << (0x04 * (pin_source & 0x03)));
    EXTI->CR[pin_source >> 0x02] |= ((port_source_gpio) << (0x04 * (pin_source & 0x03)));
}

/**
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the init_struct.
  * @param  init_struct: pointer to a EXTI_InitTypeDef structure that
  *         contains the configuration information for the EXTI peripheral.
  * @retval None.
  */
void EXTI_Init(EXTI_InitTypeDef* init_struct)
{
    if (init_struct->EXTI_LineCmd != DISABLE) {
        EXTI->IMR  &= ~init_struct->EXTI_Line;
        EXTI->EMR  &= ~init_struct->EXTI_Line;
        if (init_struct->EXTI_Mode == EXTI_Mode_Interrupt) {
            EXTI->IMR |= init_struct->EXTI_Line;
        }
        else {
            EXTI->EMR |= init_struct->EXTI_Line;
        }
        EXTI->RTSR &= ~init_struct->EXTI_Line;
        EXTI->FTSR &= ~init_struct->EXTI_Line;
        if (init_struct->EXTI_Trigger == EXTI_Trigger_Rising_Falling) {
            EXTI->RTSR |= init_struct->EXTI_Line;
            EXTI->FTSR |= init_struct->EXTI_Line;                               /*!< Rising and Faling    afio */
        }
        else if (init_struct->EXTI_Trigger == EXTI_Trigger_Rising) {
            EXTI->RTSR |= init_struct->EXTI_Line;
        }
        else {
            EXTI->FTSR |= init_struct->EXTI_Line;
        }
    }
    else {
        if (init_struct->EXTI_Mode == EXTI_Mode_Interrupt) {
            EXTI->IMR &= ~init_struct->EXTI_Line;
        }
        else {
            EXTI->EMR &= ~init_struct->EXTI_Line;
        }
    }
}

/**
  * @brief  Fills each init_struct member with its reset value.
  * @param  init_struct: pointer to a EXTI_InitTypeDef structure which will
  *         be initialized.
  * @retval None.
  */
void EXTI_StructInit(EXTI_InitTypeDef* init_struct)
{
    init_struct->EXTI_Line    = EXTI_LineNone;
    init_struct->EXTI_Mode    = EXTI_Mode_Interrupt;
    init_struct->EXTI_Trigger = EXTI_Trigger_Falling;
    init_struct->EXTI_LineCmd = DISABLE;
}

/**
  * @brief  Generates a Software interrupt on selected EXTI line.
  * @param  line: specifies the EXTI line on which the software interrupt
  *         will be generated.
  * @retval None.
  */
void EXTI_GenerateSWInterrupt(uint32_t line)
{
    EXTI->SWIER |= line;
}

/**
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  line: specifies the EXTI line flag to check.
  * @retval The new state of line (SET or RESET).
  */
FlagStatus EXTI_GetFlagStatus(uint32_t line)
{
    return (EXTI->PR & line) ? SET : RESET;
}

/**
  * @brief  Clears the EXTI's line pending flags.
  * @param  line: specifies the EXTI lines flags to clear.
  * @retval None.
  */
void EXTI_ClearFlag(uint32_t line)
{
    EXTI->PR = line;
}

/**
  * @brief  Checks whether the specified EXTI line is asserted or not.
  * @param  line: specifies the EXTI line to check.
  * @retval The new state of line (SET or RESET).
  */
ITStatus EXTI_GetITStatus(uint32_t line)
{
    return ((EXTI->PR & line) && (EXTI->IMR & line)) ? SET : RESET;
}

/**
  * @brief  Clears the EXTI's line pending bits.
  * @param  line: specifies the EXTI lines to clear.
  * @retval None.
  */
void EXTI_ClearITPendingBit(uint32_t line)
{
    EXTI->PR = line;
}

/**
  * @brief  EXTI Line Disable
  * @param  line: specifies the EXTI lines to clear.
  * @retval None.
  */
void exEXTI_LineDisable(uint32_t line)
{
    EXTI->IMR  &= ~line;
    EXTI->EMR  &= ~line;
    EXTI->RTSR &= ~line;
    EXTI->FTSR &= ~line;
}

/**
  * @brief  Clears the EXTI's line all pending bits.
  * @param  None.
  * @retval None.
  */
uint32_t exEXTI_GetAllFlagStatus(void)
{
    return EXTI->PR;
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

