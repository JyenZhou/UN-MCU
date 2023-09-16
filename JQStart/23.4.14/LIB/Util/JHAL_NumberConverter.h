#ifndef __ITOA_INTTOINT__H__
#define __ITOA_INTTOINT__H__
#include "JSYS.h"
#ifdef __CplusPlus
extern "C" {
#endif

/** ----------------------------JHAL_u32ToPowerExponent-----------------------------------
  * @������ ��һ��U32��ת������ָ����ʽ   ����ϵ����Χ���ֵ  ���ù̶����� ������ָ��
  *
  * @������
**	  	 value: Ҫת����ֵ
**			 itemMaxValue: ϵ�������ֵ
**			 base: ���õ���
**			 *item: [���]  ϵ��
**			 *powerExponent: [���] ָ��
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2022-11-22-------------------------------------- */
void JHAL_u32ToPowerExponent( u32 value, u32 itemMaxValue, u32 base,u32 *item,u32 *powerExponent);

#ifdef CplusPlus
}
#endif

#endif


