/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*   
CUBEMX 配置
TIMER
1.Clock Source->internal clock  时钟源选择
2.Prescaler 预分频器值0-65535 72M晶振用7199对应100us记数一次
3.Counter Period   计数器周期  与上面一起配合使用1000次正好是 1s中断一次
4.勾选所有中断
	5.将下面回调函数放到要用的地方
      void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
        {
				
        }
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#ifdef UTTIL_TIMER
#include "Util_timer.h"


/**

  * 函数功能: 基本定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void utilTimer_Init(void)
{
//使能定时器x和定时器x更新中断：TIM_IT_UPDATE 
#ifdef UTIL_TIMER1_EN

 HAL_TIM_Base_Start_IT(&htim1); 
#endif
   

}





/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 里面执行语句
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//}

#endif
