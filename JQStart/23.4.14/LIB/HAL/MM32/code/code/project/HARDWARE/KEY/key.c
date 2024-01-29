////////////////////////////////////////////////////////////////////////////////
/// @file    key.c
/// @author  AE TEAM
/// @brief   key intput.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _KEY_C_

// Files includes

#include "key.h"
#include "delay.h"
#include "led.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  Keyboard Scanning
/// @note   None.
/// @param  None.
/// @retval Keyboard value.
////////////////////////////////////////////////////////////////////////////////
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;
    if(mode)key_up = 1;
    if(key_up && (WK_UP == 0)) {
        DELAY_Ms(10);
        key_up = 0;
        if(WK_UP == 0)
            return WKUP_PRES;
    }
    else if(WK_UP == 1)
        key_up = 1;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize KEY GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful, can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = SW2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SW2_GPIO_Port, &GPIO_InitStructure);


}

/// @}


/// @}

/// @}

