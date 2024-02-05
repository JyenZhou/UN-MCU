/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*   
CUBEMX 配置
TIMER
1.Clock Source->internal clock  时钟源选择
//参数-Count Setting
2.Prescaler 预分频器值0-65535 72M晶振用7199对应100us记数一次
3.Counter Period   计数器周期  与上面一起配合使用1000次正好是 1s中断一次 (有自己做的工具)
4. auto-reload preload (自动重装载 使能)
//中断
5.勾选中断 update interrupt  
 
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

 
#include "../JHAL_Timer.h"

#ifdef HAL_TIM_MODULE_ENABLED


TIM_TypeDef* __JHAL_dev2timerInstance(u8 dev)
{
    
	if(false)
	{
		
	}
	#ifdef TIM1
	 else if(dev==0)
    {
        return TIM1;
    } 
		#endif
		
		#ifdef TIM2
		else if(dev==1)
    {

        return TIM2 ;

    }
	#endif

			#ifdef TIM3
		else if(dev==2)
    {

        return TIM3 ;

    } 
		#endif
			#ifdef TIM4
		else if(dev==3)
    {

        return TIM4 ;

    }
		#endif
		
#ifdef TIM5
		else if(dev==4)
    {

        return TIM5 ;

    } 
#endif
		#ifdef TIM6
		else if(dev==5)
    {

        return TIM6;

    } 
		#endif
		#ifdef TIM5
		else if(dev==6)
    {
        return TIM7 ;
    }
#endif
		
    else {

//不存在或未实现
       
      JHAL_error("__JHAL_dev2timerInstance");
  
    }
	  return NULL;
}
 
u8 __JHAL_timerInstance2dev (TIM_TypeDef*  instance )
{
	if(false)
	{ 
	}
	#ifdef TIM1
	
    if(instance==TIM1)
    {
        return 0;
    }
#endif
	#ifdef TIM2
    else  if(instance==TIM2)
    {
        return 1;
    }  
		#endif
			#ifdef TIM3
		else  if(instance==TIM3)
    {
        return 2;
    }  
		#endif
		#ifdef TIM4
		else  if(instance==TIM4)
    {
        return 3;
    } 
		#endif
	#ifdef TIM5
		else  if(instance==TIM5)
    {
        return 4;
    }
		#endif
		#ifdef TIM6
		else  if(instance==TIM6)
    {
        return 5;
    } 
		#endif
	#ifdef TIM7
		else  if(instance==TIM7)
    {
        return 6;
    }
#endif
    else {

   JHAL_error("__JHAL_timerInstance2dev");
  

    }
		 return NULL;
}


 
 
 bool JHAL_timerOpen(JHAL_Timer *timer ) 
{
	HAL_TIM_Base_Start_IT( (TIM_HandleTypeDef *)(timer->dev));
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
	 JHAL_timerInterruptCallback(__JHAL_timerInstance2dev(htim->Instance));
 }

 #endif
