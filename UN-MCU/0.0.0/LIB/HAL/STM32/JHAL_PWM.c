/**
  ****************************JHAL_PWM.c******************************************
  * @brief

CUBEMX 配置
 PWM 输出
TIMER
1.Clock Source->internal clock  时钟源选择
2->channel选中需要的通道输出 PWM Generation CHX  （无pwm的说明该定时器不支持换一个）
//参数-Count Setting
3.Prescaler 预分频器值0-65535 72M晶振用7199对应100us记数一次
4.Counter Period   计数器周期  与上面一起配合使用999次正好是 1s中断一次 (有自己做的工具)
//	上面两个就可以算到频率了
5. auto-reload preload (自动重装载 使能)



中断：仅PWM输出 中断不需要勾选

//可选配置
//极性

 PWM 捕获输入      缺点：1 周期超出自动重装载数无法捕获  （需要定时器计数频率配合  适用于已知频率范围的的 最好是输入配置和输出一样频率的才准）  缺点2：这里始终以__captureCount为0为起点 需要有一个完整的周期低-高-低才能检测 而高-低-高 不行需要后面补充个低（连续信号是满足的，并且方便手动重载）

 Timers
    

	  1. 通道上面不直接选择用cubemx内部集成的(时钟什么全不用手动配了) 手动配置的比较麻烦  通道4下面有个Combined Channels 可以选择通道1还是2   3和4用不了 实际上使用pwm捕获模式这里只能一个定时器用一个通道 计数器共用的  ps:手动写了个可以支持任意单通道的但是精度比较差 应该是中断内部修改和清除记数不同步导致的

		2.Prescaler  分频后频率尽量输入频率一致 或更高这样分辨精度越高   例如50HZ  也就是20ms周期的  配置31后65535次 为29ms能满足    但是比分频71精度要高一倍  这个可以使用我的做的工具算自动重装载设置为65535后调整分频 满足时间下越小越好
		//Counter Period 和自动重装载都不用配置    不使能最大值溢出是0 使能后内部设置自动重装载值是最大值0xFFFF  配合上面的一起   （内部使用了高低脉宽分别计数采 所以两个都能算到65535）

    3.中断TIMx capture compare interrupt 
		
    4.抗干扰  如果需要的话可以去GPIO->TIM中将IO下拉

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


JHAL_PWM* __JHAL_pwmDevArray[__TIM_COUNT];


u16 __JHAL_jpwmChannel2pwmChannel(u8 channel)
{
    if(channel==1)
    {
        return  TIM_CHANNEL_1;
    } else 	if(channel==2)
    {
        return  TIM_CHANNEL_2;
    } else 	if(channel==3)
    {
        return  TIM_CHANNEL_3;
    } else 	if(channel==4)
    {
        return  TIM_CHANNEL_4;
    } else 	if(channel==JHAL_PWM_ChannelAll)
    {
        return  TIM_CHANNEL_ALL;
    } else
    {
        JHAL_error("__JHAL_jpwmChannel2pwmChannel");
        return 0;
    }



}


//注意初始值 initialValue 要和Counter Period一致 以cubemx为准
//JHAL_PWM_Frequency 参数是无效的  以cubemx为准
bool JHAL_pwmOpen(JHAL_PWM *pwm )
{

    pwm->id=__JHAL_timerInstance2Id(((TIM_HandleTypeDef *)(pwm->dev))->Instance);


    __JHAL_pwmDevArray[pwm->id]=pwm;


    if(pwm->isInputMode)
    {
        __HAL_TIM_SetAutoreload((TIM_HandleTypeDef *)(pwm->dev), 0xFFFF);
        //STM32CUBEMX  6.9.2使用软件包 FW_F1 V1.8.5使用TIM_CHANNEL_ALL  pwm中断使能无论是输入还是输出都不能控制全部通道
        //HAL_TIM_IC_Start_IT((TIM_HandleTypeDef *)(pwm->dev),TIM_CHANNEL_ALL);
        HAL_TIM_IC_Start_IT((TIM_HandleTypeDef *)(pwm->dev),TIM_CHANNEL_1);
        HAL_TIM_IC_Start_IT((TIM_HandleTypeDef *)(pwm->dev),TIM_CHANNEL_2);

    } else
    {
        for(u8 channelIndex=0; channelIndex<pwm->channelsNumber; channelIndex++)
        {

            u16 channel=__JHAL_jpwmChannel2pwmChannel(pwm->channels[channelIndex].channel);



            __HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),channel,pwm->channels[channelIndex].initialValue);
            HAL_TIM_PWM_Start_IT((TIM_HandleTypeDef *)(pwm->dev),channel);

        }


    }
    return true;
}


//通道传进来0-4、 0操作所有通道
void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value)

{

    if(channelIndex>=pwm->channelsNumber||value>pwm->channels[channelIndex].maxValue)
    {
        JHAL_error("JHAL_pwmSetValue");
    }
    //设置占空比值
    __HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),__JHAL_jpwmChannel2pwmChannel(pwm->channels[channelIndex].channel),value);




}


float  __HAL_timerGetInternalClockFrequency(TIM_HandleTypeDef *  htim  ) {

    // 计算定时器的频率
    return  (float)HAL_RCC_GetSysClockFreq() / ( htim->Init.Prescaler + 1)/(__HAL_TIM_GetClockDivision(htim)+1);
}





float  JHAL_pwmGetCapturePulseWidthSecond(	JHAL_PWM*  pwm, bool isHighPulseWidth)
{

    u32 pulseWidthValue=isHighPulseWidth?	pwm->__inputInfo. captureValue1-pwm-> __inputInfo.captureValue2:pwm-> __inputInfo.captureValue2;
    return 	  (float)pulseWidthValue/	 __HAL_timerGetInternalClockFrequency((TIM_HandleTypeDef *)(pwm->dev));


}

float  JHAL_pwmGetCaptureDutyCycle(	JHAL_PWM*  pwm, bool isHighLevel)
{

    float  	highPulseWidth =pwm->__inputInfo. captureValue1- pwm->__inputInfo. captureValue2 ;
    return  isHighLevel? highPulseWidth/pwm->__inputInfo. captureValue1 : (float) pwm->__inputInfo. captureValue2/ pwm->__inputInfo. captureValue1;

}


float  JHAL_pwmGetCaptureFrequency(	JHAL_PWM*  pwm)
{


    return	1.0f/(JHAL_pwmGetCapturePulseWidthSecond(pwm,true)+JHAL_pwmGetCapturePulseWidthSecond(pwm,false));

}



u8  __JHAL_timActiveChannel2timChannel(HAL_TIM_ActiveChannel  activeChannel )
{

    if (activeChannel == HAL_TIM_ACTIVE_CHANNEL_1) {
        return TIM_CHANNEL_1;
    } else if (activeChannel == HAL_TIM_ACTIVE_CHANNEL_2) {
        return TIM_CHANNEL_2;
    } else if (activeChannel == HAL_TIM_ACTIVE_CHANNEL_3) {
        return TIM_CHANNEL_3;
    } else {
        return TIM_CHANNEL_4;
    }
}

void  __JHAL_pwmUpdateCaptureValue(TIM_HandleTypeDef *htim)
{
    JHAL_PWM*  pwm= __JHAL_pwmDevArray[ __JHAL_timerInstance2Id(htim->Instance)];
    u32  capturedValue =	 HAL_TIM_ReadCapturedValue((TIM_HandleTypeDef *)(pwm->dev), __JHAL_timActiveChannel2timChannel( htim->Channel) )+1;


    if( htim->Channel  ==HAL_TIM_ACTIVE_CHANNEL_1 )
    {

        if(htim->Instance->CCMR1 & TIM_CCMR1_CC1S_0)
        {
            //当使用Combined CH1时 默认这个总脉宽
            pwm->__inputInfo.captureValue1  =capturedValue ;
        } else {
            //当使用Combined CH2时  默认这个低电平脉宽
            pwm->__inputInfo.captureValue2 =capturedValue;
        }

    } else

    {

        if(htim->Instance->CCMR1 & TIM_CCMR1_CC1S_1)
        {
            //当使用Combined CH2时 默认这个总脉宽
            pwm->__inputInfo.captureValue1  =capturedValue ;
        } else {
            //当使用Combined CH1时 默认这个是高脉宽
            pwm->__inputInfo.captureValue2 = pwm->__inputInfo.captureValue1-capturedValue;
        }
    }



}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

    __JHAL_pwmUpdateCaptureValue(htim);

}


#endif

