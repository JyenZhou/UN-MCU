/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-13 14:51:09
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

#ifndef __SYSTEM_tc04xx_H__
#define __SYSTEM_tc04xx_H__


#include "base_types.h"

/******************************************************************************* 
* Definitions 
******************************************************************************/

extern uint32_t SystemCoreClock;


/******************************************************************************* 
* API 
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/


void interrupt_enable(void);
void interrupt_disable(void);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* __SYSTEM_tc04xx_H__ */
