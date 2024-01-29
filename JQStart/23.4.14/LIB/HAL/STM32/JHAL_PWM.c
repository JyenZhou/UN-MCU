/**
  ****************************JHAL_PWM.c******************************************
  * @brief      
 
CUBEMX 配置
TIMER
1.Clock Source->internal clock  时钟源选择
2.Channel选中需要的通道输出 PWM Generation CHX  （无pwm的说明该定时器不支持换一个）
//参数-Count Setting
3.Prescaler 预分频器值0-65535 72M晶振用7199对应100us记数一次
4.Counter Period   计数器周期  与上面一起配合使用1000次正好是 1s中断一次 (有自己做的工具)
//	上面两个就可以算到频率了
5. auto-reload preload (自动重装载 使能) 
备注：仅PWM 中断不需要勾选

//可选配置
//极性  
 
	
	
	
  *
    ******************************************************************************
  * @file     JHAL_PWM.c
  * @author   Jyen
  * @version  v1.0
  * @date     2023-09-18 
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2023,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
  #include "../JHAL_PWM.h"
 #ifdef HAL_TIM_MODULE_ENABLED
 
 
  //注意初始值 initialValue 要和Counter Period一致 以cubemx为准
	//JHAL_PWM_Frequency 参数是无效的  以cubemx为准
	  bool JHAL_pwmOpen(JHAL_PWM *pwm )
		{
		 
			for(u8 channelIndex=0;channelIndex<pwm->channelsNumber;channelIndex++)
			{
				//TIM_CHANNEL_ALL  实测cubemx 1.8.5在pwm中无任何输出 不知道是不是bug
				if(pwm->channels[channelIndex].channel==TIM_CHANNEL_1 || pwm->channels[channelIndex].channel==TIM_CHANNEL_2  || pwm->channels[channelIndex].channel==TIM_CHANNEL_3 || pwm->channels[channelIndex].channel==TIM_CHANNEL_4    )
				{
					 	__HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),pwm->channels[channelIndex].channel,pwm->channels[channelIndex].initialValue);
						HAL_TIM_PWM_Start_IT((TIM_HandleTypeDef *)(pwm->dev),pwm->channels[channelIndex].channel);
				}else{
					   JHAL_Fault_Handler("JHAL_pwmOpen");
					return false;
				}
					
			}
			return true;
		}
 

 //通道传进来0-4、 0操作所有通道
  void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value)
		
	{
 
	 if(channelIndex>=pwm->channelsNumber||value>pwm->channels[channelIndex].maxValue)
	 {
		  JHAL_Fault_Handler("JHAL_pwmSetValue");
	 }
	 //设置占空比值
	__HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),pwm->channels[channelIndex].channel,value);
	 
 
 

 }
#endif

