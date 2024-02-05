/*
 * @FilePath     : tc04xx_debug.h
 * @Description  : 
 * @Author       : Liu_zhen
 * @Date         : 2023-09-11
 * @LastEditTime : 2023-09-11
 * Copyright 2023, All Rights Reserved. 
 */

#ifndef __TC04XX_DEBUG_H__
#define __TC04XX_DEBUG_H__

#include "tc04xx.h"


/*********************************************************************/
#define TC_DEBUG_EN         0           //debug开关

#define DEBUG_PIN           GPIOB_28    //debug输出引脚

#if TC_DEBUG_EN
#define TC_Printf( format, ... )	printf( format, ##__VA_ARGS__ )
#else
#define TC_Printf( format, ... )	
#endif

/*********************************************************************/
void UART1_SetFclkSource(uint8_t clk_src, uint16_t clk_div);
void Debug_Init(uint32_t baud);
/*********************************************************************/
#endif

