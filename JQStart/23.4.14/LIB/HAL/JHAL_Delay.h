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
#ifndef __JHAL_Delay__H__
#define  __JHAL_Delay__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef struct {
//主频
        u8 basicFrequency;

    } JHAL_Delay;






    //  JHAL_delayOpen((JHAL_Delay){NULL});
    void  JHAL_delayOpen(JHAL_Delay config);
    void  JHAL_delayMs(u32 ms);
    void  JHAL_delayUs(u32 us);
    void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value);

		//上面的延时是和通信相关的不可打断   准确度较高 使用时中断被屏蔽下也可正常使用    当osDelay满足这里条件时则可以同用
   
		//关于os delay 默认可以不实现  已使用弱定义调用上面硬延时
	  void  JHAL_delayOsMs (u32 ms); 
    void  JHAL_delayOsUs(u32 us);
    void  JHAL_delayOs(JHAL_TimeUnits timeUntis,u16 value);

 

#ifdef CplusPlus
}
#endif

#endif








