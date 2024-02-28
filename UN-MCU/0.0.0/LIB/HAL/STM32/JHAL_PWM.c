/**
  ****************************JHAL_PWM.c******************************************
  * @brief

CUBEMX ����
 PWM ���
TIMER
1.Clock Source->internal clock  ʱ��Դѡ��
2->channelѡ����Ҫ��ͨ����� PWM Generation CHX  ����pwm��˵���ö�ʱ����֧�ֻ�һ����
//����-Count Setting
3.Prescaler Ԥ��Ƶ��ֵ0-65535 72M������7199��Ӧ100us����һ��
4.Counter Period   ����������  ������һ�����ʹ��999�������� 1s�ж�һ�� (���Լ����Ĺ���)
//	���������Ϳ����㵽Ƶ����
5. auto-reload preload (�Զ���װ�� ʹ��)



�жϣ���PWM��� �жϲ���Ҫ��ѡ

//��ѡ����
//����

 PWM ��������      ȱ�㣺1 ���ڳ����Զ���װ�����޷�����  ����Ҫ��ʱ������Ƶ�����  ��������֪Ƶ�ʷ�Χ�ĵ� ������������ú����һ��Ƶ�ʵĲ�׼��  ȱ��2������ʼ����__captureCountΪ0Ϊ��� ��Ҫ��һ�����������ڵ�-��-�Ͳ��ܼ�� ����-��-�� ������Ҫ���油����ͣ������ź�������ģ����ҷ����ֶ����أ�

 Timers
    

	  1. ͨ�����治ֱ��ѡ����cubemx�ڲ����ɵ�(ʱ��ʲôȫ�����ֶ�����) �ֶ����õıȽ��鷳  ͨ��4�����и�Combined Channels ����ѡ��ͨ��1����2   3��4�ò��� ʵ����ʹ��pwm����ģʽ����ֻ��һ����ʱ����һ��ͨ�� ���������õ�  ps:�ֶ�д�˸�����֧�����ⵥͨ���ĵ��Ǿ��ȱȽϲ� Ӧ�����ж��ڲ��޸ĺ����������ͬ�����µ�

		2.Prescaler  ��Ƶ��Ƶ�ʾ�������Ƶ��һ�� ����������ֱ澫��Խ��   ����50HZ  Ҳ����20ms���ڵ�  ����31��65535�� Ϊ29ms������    ���Ǳȷ�Ƶ71����Ҫ��һ��  �������ʹ���ҵ����Ĺ������Զ���װ������Ϊ65535�������Ƶ ����ʱ����ԽСԽ��
		//Counter Period ���Զ���װ�ض���������    ��ʹ�����ֵ�����0 ʹ�ܺ��ڲ������Զ���װ��ֵ�����ֵ0xFFFF  ��������һ��   ���ڲ�ʹ���˸ߵ�����ֱ������ �������������㵽65535��

    3.�ж�TIMx capture compare interrupt 
		
    4.������  �����Ҫ�Ļ�����ȥGPIO->TIM�н�IO����

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


//ע���ʼֵ initialValue Ҫ��Counter Periodһ�� ��cubemxΪ׼
//JHAL_PWM_Frequency ��������Ч��  ��cubemxΪ׼
bool JHAL_pwmOpen(JHAL_PWM *pwm )
{

    pwm->id=__JHAL_timerInstance2Id(((TIM_HandleTypeDef *)(pwm->dev))->Instance);


    __JHAL_pwmDevArray[pwm->id]=pwm;


    if(pwm->isInputMode)
    {
        __HAL_TIM_SetAutoreload((TIM_HandleTypeDef *)(pwm->dev), 0xFFFF);
        //STM32CUBEMX  6.9.2ʹ������� FW_F1 V1.8.5ʹ��TIM_CHANNEL_ALL  pwm�ж�ʹ�����������뻹����������ܿ���ȫ��ͨ��
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


//ͨ��������0-4�� 0��������ͨ��
void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value)

{

    if(channelIndex>=pwm->channelsNumber||value>pwm->channels[channelIndex].maxValue)
    {
        JHAL_error("JHAL_pwmSetValue");
    }
    //����ռ�ձ�ֵ
    __HAL_TIM_SET_COMPARE((TIM_HandleTypeDef *)(pwm->dev),__JHAL_jpwmChannel2pwmChannel(pwm->channels[channelIndex].channel),value);




}


float  __HAL_timerGetInternalClockFrequency(TIM_HandleTypeDef *  htim  ) {

    // ���㶨ʱ����Ƶ��
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
            //��ʹ��Combined CH1ʱ Ĭ�����������
            pwm->__inputInfo.captureValue1  =capturedValue ;
        } else {
            //��ʹ��Combined CH2ʱ  Ĭ������͵�ƽ����
            pwm->__inputInfo.captureValue2 =capturedValue;
        }

    } else

    {

        if(htim->Instance->CCMR1 & TIM_CCMR1_CC1S_1)
        {
            //��ʹ��Combined CH2ʱ Ĭ�����������
            pwm->__inputInfo.captureValue1  =capturedValue ;
        } else {
            //��ʹ��Combined CH1ʱ Ĭ������Ǹ�����
            pwm->__inputInfo.captureValue2 = pwm->__inputInfo.captureValue1-capturedValue;
        }
    }



}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

    __JHAL_pwmUpdateCaptureValue(htim);

}


#endif

