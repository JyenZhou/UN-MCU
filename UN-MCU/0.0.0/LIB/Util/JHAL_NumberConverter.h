#ifndef __ITOA_INTTOINT__H__
#define __ITOA_INTTOINT__H__
#include "JSYS.h"
#ifdef __cplusPlus
extern "C" {
#endif

/** ----------------------------JHAL_u32ToPowerExponent-----------------------------------
  * @描述： 将一个U32数转化成幂指数形式   设置系数范围最大值  设置固定底数 返回幂指数
  *
  * @参数：
**	  	 value: 要转化的值
**			 itemMaxValue: 系数的最大值
**			 base: 设置底数
**			 *item: [输出]  系数
**			 *powerExponent: [输出] 指数
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2022-11-22-------------------------------------- */
void JHAL_u32ToPowerExponent( u32 value, u32 itemMaxValue, u32 base,u32 *item,u32 *powerExponent);

#ifdef CplusPlus
}
#endif

#endif


