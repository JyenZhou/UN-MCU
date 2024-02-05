/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADC默认通用HAL驱动
  使用ADC0 若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_ACMP__H__
#define  __JHAL_ACMP__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"
    void  JHAL_acmpOpen( void);
#ifdef CplusPlus
}
#endif

#endif








