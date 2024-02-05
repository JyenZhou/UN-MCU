/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-13 15:20:49
 * @Disclaimer: 
 * 	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * 	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * 	* TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * 	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * 	* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved. 
 */
#include "tc04xx_gpio.h"


/**
** \brief GPIO  GPIO_DeInit

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_DeInit(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->INT_MSK |= pin_mask;
    }
    else
    {
        GPIOA->INT_MSK |= pin_mask;
    }
   *(__IO uint32_t *)PINMUX_ADDR(gpio) = 0x0;
}

/**
  * @brief   GPIO_Config
  *
  * @param  [u16]  gpio:    GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
  *                         GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
  *                         GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
  * 
  * @param  [u8]   mode:    GPIO_INPUT_MODE,
                            GPIO_OUTPUT_MODE
  * @return   None
  *
  * @details
  */
void GPIO_Config(uint16_t gpio, uint8_t mode)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if (mode == (uint8_t)GPIO_OUTPUT_MODE)
    {
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->OUTEN_SET = pin_mask;
        }
        else
        {
            GPIOA->OUTEN_SET = pin_mask;
        }
    }
    else
    {
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->OUTEN_CLR = pin_mask;
        }
        else
        {
            GPIOA->OUTEN_CLR = pin_mask;
        }
    }
}
/**
** \brief GPIO  GPIO_Init
**
** \param [u8]  GPIO_Init_T

*  @param [16]   gpio:              GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                                   GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                                   GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30

** \param [u8]  GPIO_Mode:          GPIO_INPUT_MODE,
                                    GPIO_OUTPUT_MODE,

** \param [u8]  GPIO_Ext_Mode:      GPIO_PIN_OPEN_SOURCE,
                                    GPIO_PIN_OPEN_DRAIN,
                                    GPIO_PIN_PUSH_PULL,
                                    GPIO_PIN_HIGH_IMPEDANCE,

** \param [u8]  pull_mode:          GPIO_PULL_UP,
                                    GPIO_PULL_DOWN,
*  @return   None
*
*  @details

  */
void GPIO_Init(GPIO_INIT_T *pInit_t)
{
    if(pInit_t != NULL)
    {
        APINMUX_CFG_FIELD_T *pincfg = (APINMUX_CFG_FIELD_T *)PINMUX_ADDR(pInit_t->gpio);

        GPIO_Config(pInit_t->gpio, pInit_t->mode);
        switch (pInit_t->ext_mode)
        {
        case (uint8_t)GPIO_PIN_OPEN_SOURCE:
            pincfg->OS = 1;
            pincfg->OD  = 0;
            break;
        case (uint8_t)GPIO_PIN_OPEN_DRAIN:
            pincfg->OS = 0;
            pincfg->OD  = 1;
            break;
        case (uint8_t)GPIO_PIN_PUSH_PULL:
            pincfg->OS = 0;
            pincfg->OD  = 0;
            break;
        default:
            //error
            break;
        }

        switch (pInit_t->pull_mode)
        {
        case (uint8_t)GPIO_PULL_UP:
            pincfg->SW_PULL_SEL = 1;
            pincfg->SW_PU       = 1;
            pincfg->SW_PD       = 0;
            break;
        case (uint8_t)GPIO_PULL_DOWN:
            pincfg->SW_PULL_SEL = 1;
            pincfg->SW_PU       = 0;
            pincfg->SW_PD       = 1;
            break;
        case (uint8_t)GPIO_PULL_NONE:
            pincfg->SW_PULL_SEL = 0;
            pincfg->SW_PU       = 0;
            pincfg->SW_PD       = 0;
            break;
        default:
            //error
            break;
        }
    }
}



/**
  * @brief   GPIO_ITConfig
  *
  * @param  [16]  gpio:       GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
  *                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
  *                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
  *
  * @param  [u8]  mode:       GPIO_IT_FALLING_EDGE
                              GPIO_IT_RAISING_EDGE
                              GPIO_IT_LOW_LEVEL
                              GPIO_IT_HIGH_LEVEL
  *
  * @details
  */
void GPIO_ITConfig(uint16_t gpio, uint8_t mode)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    switch (mode)
    {
    case (uint8_t)GPIO_IT_FALLING_EDGE:
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->INT_POL_SEL &= ~pin_mask;
            GPIOB->INT_TYP_SEL |= pin_mask;
        }
        else
        {
            GPIOA->INT_POL_SEL &= ~pin_mask;
            GPIOA->INT_TYP_SEL |= pin_mask;
        }

        break;
    case (uint8_t)GPIO_IT_RAISING_EDGE:
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->INT_POL_SEL |= pin_mask;
            GPIOB->INT_TYP_SEL |= pin_mask;
        }
        else
        {
            GPIOA->INT_POL_SEL |= pin_mask;
            GPIOA->INT_TYP_SEL |= pin_mask;
        }
        break;
    case (uint8_t)GPIO_IT_LOW_LEVEL:
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->INT_POL_SEL &= ~pin_mask;
            GPIOB->INT_TYP_SEL |= pin_mask;
        }
        else
        {
            GPIOA->INT_POL_SEL &= ~pin_mask;
            GPIOA->INT_TYP_SEL |= pin_mask;
        }
        break;
    case (uint8_t)GPIO_IT_HIGH_LEVEL:
        if ((gpio & 0xFF00U) != 0x00)
        {
            GPIOB->INT_POL_SEL |= pin_mask;
            GPIOB->INT_TYP_SEL &= ~pin_mask;
        }
        else
        {
            GPIOA->INT_POL_SEL |= pin_mask;
            GPIOA->INT_TYP_SEL &= ~pin_mask;
        }
        break;
    default:
        //error
        break;
    }
}
/**
** \brief GPIO  GPIO_IT_Enable

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_IT_Enable(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->INT_MSK &= ~pin_mask;
    }
    else
    {
        GPIOA->INT_MSK &= ~pin_mask;
    }
}
/**
** \brief GPIO  GPIO_IT_Disable

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_IT_Disable(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->INT_MSK |= pin_mask;
    }
    else
    {
        GPIOA->INT_MSK |= pin_mask;
    }
}

/**
  * @brief    GPIO_ExtModeConfig
  *
  * @param  [u16]  gpio:              GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
  *                                   GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
  *                                   GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30

  ** \param [u8]  GPIO_Ext_Mode:      GPIO_PIN_OPEN_SOURCE   ,
                                      GPIO_PIN_OPEN_DRAIN,
                                      GPIO_PIN_PUSH_PULL,
                                      GPIO_PIN_HIGH_IMPEDANCE,

  ** \param [u8]  pull_mode:          GPIO_PULL_UP,
                                      GPIO_PULL_DOWN,

  * @return   None
  *
  * @details
  */
void GPIO_ExtModeConfig(uint16_t gpio, uint8_t ext_mode, uint8_t pull_mode)
{
    APINMUX_CFG_FIELD_T *pincfg = (APINMUX_CFG_FIELD_T *)PINMUX_ADDR(gpio);

    switch (ext_mode)
    {
    case (uint8_t)GPIO_PIN_OPEN_SOURCE:
        pincfg->OS = 1;
        pincfg->OD = 0;
        break;
    case (uint8_t)GPIO_PIN_OPEN_DRAIN:
        pincfg->OS = 0;
        pincfg->OD = 1;
        break;
    case (uint8_t)GPIO_PIN_PUSH_PULL:
        pincfg->OS = 0;
        pincfg->OD = 0;
        break;
    case (uint8_t)GPIO_PIN_HIGH_IMPEDANCE:
        break;
    default:
        //error
        break;
    }

    switch (pull_mode)
    {
    case (uint8_t)GPIO_PULL_UP:
        pincfg->SW_PULL_SEL = 1;
        pincfg->SW_PU = 1;
        pincfg->SW_PD = 0;
        break;
    case (uint8_t)GPIO_PULL_DOWN:
        pincfg->SW_PULL_SEL = 1;
        pincfg->SW_PU = 0;
        pincfg->SW_PD = 1;
        break;
    case (uint8_t)GPIO_PULL_NONE:
        pincfg->SW_PULL_SEL = 0;
        pincfg->SW_PU = 0;
        pincfg->SW_PD = 0;
        break;
    default:
        //error
        break;
    }
}
/**
** \brief GPIO  GPIO_SetBit

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_SetBit(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->DATAOUT |= pin_mask;
    }
    else
    {
        GPIOA->DATAOUT |= pin_mask;
    }

}
/**
** \brief GPIO  GPIO_ClrBit

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_ClrBit(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->DATAOUT &= ~pin_mask;
    }
    else
    {
        GPIOA->DATAOUT &= ~pin_mask;
    }
}

/**
** \brief GPIO  GPIO_GetBit

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

 */
uint8_t GPIO_GetBit(uint16_t gpio)
{
    uint8_t val = 0;
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        val = (uint8_t)(((GPIOB->DATAIN & pin_mask) != 0) ? 1 : 0);
    }
    else
    {
        val = (uint8_t)(((GPIOA->DATAIN & pin_mask) != 0) ? 1 : 0);
    }
    return val;
}
/**
** \brief GPIO  GPIO_ToggleBit

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_ToggleBit(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        if ((GPIOB->DATAOUT & pin_mask) != 0x00U)
        {
            GPIOB->DATAOUT  &= ~pin_mask;
        }
        else
        {
            GPIOB->DATAOUT |= pin_mask;
        }
    }
    else
    {
        if ((GPIOA->DATAOUT & pin_mask) != 0x00U)
        {
            GPIOA->DATAOUT  &= ~pin_mask;
        }
        else
        {
            GPIOA->DATAOUT |= pin_mask;
        }
    }
}
/**
** \brief GPIO  GPIO_GetFlag

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
uint8_t GPIO_GetFlag(uint16_t gpio)
{
    uint8_t ret = FALSE;
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        ret = ((GPIOB->INT_STATUS  & pin_mask) != 0) ? TRUE : FALSE;
    }
    else
    {
        ret = ((GPIOA->INT_STATUS  & pin_mask) != 0) ? TRUE : FALSE;
    }
    return ret;
}
/**
** \brief GPIO  GPIO_ClrFlag

*  @param  [u16]  gpio:     GPIOA_00,GPIOA_01,GPIOA_02,GPIOA_03,GPIOA_04,GPIOA_05,GPIOA_06,GPIOA_07,GPIOA_08,GPIOA_09,GPIOA_10,GPIOA_11,GPIOA_12,GPIOA_13
*                           GPIOB_00,GPIOB_01,GPIOB_02,GPIOB_03,GPIOB_04,GPIOB_05,GPIOB_06,GPIOB_07,GPIOB_08,GPIOB_09,GPIOB_10,GPIOB_11,GPIOB_12,GPIOB_13,GPIOB_14,
*                           GPIOB_15,GPIOB_16,GPIOB_17,GPIOB_18,GPIOB_19,GPIOB_20,GPIOB_21,GPIOB_22,GPIOB_23,GPIOB_24,GPIOB_25,GPIOB_26,GPIOB_27,GPIOB_28,GPIOB_29,GPIOB_30
*  @return   None
*
*  @details

  */
void GPIO_ClrFlag(uint16_t gpio)
{
    uint32_t pin_mask = PIN_MASK(gpio);

    if ((gpio & 0xFF00U) != 0x00)
    {
        GPIOB->INT_CLR = pin_mask;
    }
    else
    {
        GPIOA->INT_CLR = pin_mask;
    }
}
