/**
  ****************************JHAL_PWM.c******************************************
  * @brief      
 
CUBEMX ����
TIMER
1.Clock Source->internal clock  ʱ��Դѡ��
2.Channelѡ����Ҫ��ͨ����� PWM Generation CHX  ����pwm��˵���ö�ʱ����֧�ֻ�һ����
//����-Count Setting
3.Prescaler Ԥ��Ƶ��ֵ0-65535 72M������7199��Ӧ100us����һ��
4.Counter Period   ����������  ������һ�����ʹ��1000�������� 1s�ж�һ�� (���Լ����Ĺ���)
//	���������Ϳ����㵽Ƶ����
5. auto-reload preload (�Զ���װ�� ʹ��) 
��ע����PWM �жϲ���Ҫ��ѡ

//��ѡ����
//����  
 
	
	
	
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
 
 
  //ע���ʼֵ initialValue Ҫ��Counter Periodһ�� ��cubemxΪ׼
	//JHAL_PWM_Frequency ��������Ч��  ��cubemxΪ׼
	  bool JHAL_pwmOpen(JHAL_PWM *pwm )
		{
		 
			for(u8 channelIndex=0;channelIndex<pwm->channelsNumber;channelIndex++)
			{
				//TIM_CHANNEL_ALL  ʵ��cubemx 1.8.5��pwm�����κ���� ��֪���ǲ���bug
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
 

 //ͨ��������0-4�� 0��������ͨ��
  void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value)
		
	{
 
	 if(channelIndex>=pwm->channelsNumber||value>pwm->channels[channelIndex].maxValue)
	 {
		  JHAL_Fault_Handler("JHAL_pwmSetValue");
	 }
	 //����ռ�ձ�ֵ
	__HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),pwm->channels[channelIndex].channel,value);
	 
 
 

 }
#endif

