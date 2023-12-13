/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*   
CUBEMX 配置
TIMER
1.Clock Source->internal clock  时钟源选择
2.Prescaler 预分频器值0-65535 72M晶振用7199对应100us记数一次
3.Counter Period   计数器周期  与上面一起配合使用1000次正好是 1s中断一次 (有自己做的工具)
4.勾选中断 update interrupt  
 
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

 
#include "../JHAL_Timer.h"




TIM_TypeDef* __JHAL_dev2timerInstance(u8 dev)
{
    if(dev==0)
    {
        return TIM1;
    } else if(dev==1)
    {

        return TIM2 ;

    } else if(dev==2)
    {

        return TIM3 ;

    } else if(dev==3)
    {

        return TIM4 ;

    } else if(dev==4)
    {

        return TIM5 ;

    } else if(dev==5)
    {

        return TIM6;

    } else if(dev==6)
    {

        return TIM7 ;

    }

    else {

//不存在或未实现
        while(true);

    }
}
 
u8 __JHAL_timerInstance2dev (TIM_TypeDef*  instance )
{

    if(instance==TIM1)
    {

        return 0;

    }


    else  if(instance==TIM2)
    {

        return 1;

    }  else  if(instance==TIM3)
    {

        return 2;

    }  else  if(instance==TIM4)
    {

        return 3;

    }  else  if(instance==TIM5)
    {

        return 4;

    }  else  if(instance==TIM6)
    {

        return 5;

    }  else  if(instance==TIM7)
    {

        return 6;

    }

    else {

//不存在或未实现
        while(true);

    }
}


 
 
 bool JHAL_timerOpen(JHAL_Timer *timer ) 
{
	HAL_TIM_Base_Start_IT( (TIM_HandleTypeDef *)(timer->timer));
	return true;
} 




/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 里面执行语句
  */
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	 JHAL_timerInterruptCallBack(__JHAL_timerInstance2dev(htim->Instance));
 }

 
