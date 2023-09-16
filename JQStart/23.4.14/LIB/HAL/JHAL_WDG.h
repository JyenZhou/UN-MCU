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
#ifndef __JHAL_WDG__H__
#define  __JHAL_WDG__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        //全局看门狗
        JHAL_WDG_GWDG=0,
        //窗口看门狗
        JHAL_WDG_WWDG,
    } JHAL_WDGType;



    typedef struct
    {

//itTimeValue 中断时间值 0-1000
        u16 itTimeValue;

//窗口看门狗时 最小时间有效
        u16 itTimeMinValue;
        u16 itTimeMaxValue;
        JHAL_TimeUnits itTimeUnit;
    } JHAL_WDGConfig;


//JHAL_wdgInit((JHAL_WDGType){NULL},(JHAL_WDGConfig){NULL});
    bool  JHAL_wdgInit(JHAL_WDGType wdg, JHAL_WDGConfig config);
    bool  JHAL_wdgDeInit(JHAL_WDGType wdg);
    void  JHAL_wdgFeed(void);
    //复位前中断，若中断里面没有喂狗则复位 */
    void JHALwdgBeforeInterruptCallBack(JHAL_WDGType wdg);



#ifdef CplusPlus
}
#endif

#endif








