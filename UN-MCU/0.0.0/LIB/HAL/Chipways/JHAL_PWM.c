/**
  ****************************JHAL_PWM.c******************************************
  * @brief     
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
#include "xl_ftm.h"
#include <xl_sim.h>




void __JHAL_jpwm2ftm(u8 id,FTM_Type **FTMx, u8 *chX)
{
    if( id==0)
    {
        *FTMx=FTM1;
        *chX=3;
    }
    if( id==1)
    {
        *FTMx=FTM0;
        *chX=1;
    }
    else {
//�����ڻ�δʵ��
        while(true);
    }




}

//PWM����AD�ɼ��Ľӿ�
u8 __JHAL_jpwm2adcTriggerSoureValue(u8 id)
{

    if( id==0 )
    {
        return ADC_HT_FTM0CH1MAP;
    } else {
//�����ڻ�δʵ��
        while(true);
    }

}



/**
  * ��ʼ��pwmģ���xͨ��
  * ����   ��
  * ����   ��
  */
void __JHAL_pwmOpen(JHAL_PWM  *config )
{
    FTM_Type * FTMx;
    u8 chX;
    __JHAL_jpwm2ftm(config ->id ,&FTMx,&chX);
    FTM_InitTypeDef FTM_InitStructure;
    if(config->frequency==JHAL_PWM_750HZ)
    {
        FTM_InitStructure.FTM_ClkSource = FTM_CLOCK_SYSTEMCLOCK;
        FTM_InitStructure.FTM_Prescaler = FTM_CLOCK_PS_DIV128;
    } else if(config->frequency==JHAL_PWM_4HZ)
    {
        FTM_InitStructure.FTM_ClkSource = FTM_CLOCK_FIXEDFREQCLOCK;
        FTM_InitStructure.FTM_Prescaler = FTM_CLOCK_PS_DIV128;
    } else {
        while(true);
    }
    if(FTMx == FTM0)
    {
        SIM_SCGC_Cmd(SIM_SCGC_FTM0,ENABLE);
        if( FTM_InitStructure.FTM_ClkSource == FTM_CLOCK_SYSTEMCLOCK) {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM0T, ENABLE);
        } else if(FTM_InitStructure.FTM_ClkSource ==FTM_CLOCK_FIXEDFREQCLOCK)
        {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM0F, ENABLE);
        }
        if(config->id==1)
        {
            SIM_PINSEL_FTM0CH1(FTM0CH1_PS_PTB3);
        }
    }
    else if(FTMx == FTM1)
    {
        SIM_SCGC_Cmd(SIM_SCGC_FTM1,ENABLE);

        if( FTM_InitStructure.FTM_ClkSource == FTM_CLOCK_SYSTEMCLOCK) {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM1T, ENABLE);

        } else if(FTM_InitStructure.FTM_ClkSource ==FTM_CLOCK_FIXEDFREQCLOCK)
        {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM1F, ENABLE);
        }
        if(config->id==0)
        {
            SIM_PINSEL_FTM1CH3(FTM1CH3_PS_PTE3);
        }
    }
    FTM_WriteProtectDisable(FTMx);
    FTM_CenterAlignedPWMInit(FTMx, chX, FTM_PWM_HIGHTRUEPULSE);
    FTM_ChannelIntCmd(FTMx,chX,DISABLE);
    FTM_SetChannelValue(FTMx,chX,   config->channels->initialValue);
    FTM_SetCountInitValue(FTMx,0);
    FTM_SetModValue(FTMx, config->channels->maxValue);
    FTM_OverflowITCmd(FTMx,DISABLE);
    FTM_Init(FTMx,&FTM_InitStructure);
    FTM_SetChnTriggerCmd(FTMx, chX,  ENABLE);
    FTM_CVSyncEnable(FTMx,chX,ENABLE);

}

 

/*
JHAL_PWM0 ->  PTE3->FTM1_CH3
JHAL_PWM1 ->  PTB3->FTM0_CH1
clockSource ʱ��Դ
#define FTM_CLOCK_SYSTEMCLOCK            		1            < System clock
#define FTM_CLOCK_FIXEDFREQCLOCK        		2            < Fixed Freq Clock
#define FTM_CLOCK_EXTERNALCLOCK          		3             < External Clock


prescaler Ԥ��Ƶϵ�� 0->FTM_CLOCK_PS_DIV1 ...
#define FTM_CLOCK_PS_DIV1                		0              < 1��Ƶ
#define FTM_CLOCK_PS_DIV2                		1              < 2��Ƶ
#define FTM_CLOCK_PS_DIV4                		2              < 4��Ƶ
#define FTM_CLOCK_PS_DIV8                		3              < 8��Ƶ
#define FTM_CLOCK_PS_DIV16               		4              < 16��Ƶ
#define FTM_CLOCK_PS_DIV32               		5              < 32��Ƶ
#define FTM_CLOCK_PS_DIV64               		6              < 64��Ƶ
#define FTM_CLOCK_PS_DIV128              		7              < 128��Ƶ
*/
bool JHAL_pwmOpen(JHAL_PWM *config   )
{
    if(! config-> __info.isOpen) {



//���ó�ʼֵ�������ֵ
        while(config->channels->initialValue>config->channels->maxValue);
        __JHAL_pwmOpen( config);
			  
			return  config-> __info.isOpen=true; 
    }
		return false;
}

bool  JHAL_pwmClose(JHAL_PWM * pwm) {
    if(pwm-> __info.isOpen) {
        FTM_Type *FTMx;
        u8 chX;
        __JHAL_jpwm2ftm(pwm->id,&FTMx,&chX);

        if(FTMx == FTM0)
        {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM0T, DISABLE);
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM0F, DISABLE);
            SIM_SCGC_Cmd(SIM_SCGC_FTM0,DISABLE);
        }
        else if(FTMx == FTM1)
        {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM1T, DISABLE);
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM1F, DISABLE);
            SIM_SCGC_Cmd(SIM_SCGC_FTM1,DISABLE);
        }
        else
        {
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM2T, DISABLE);
            SIM_SCGC1_Cmd(SIM_SCGC1_FTM2F, DISABLE);
            SIM_SCGC_Cmd(SIM_SCGC_FTM2,DISABLE);
        }
				
				 pwm-> __info.isOpen=false;
return true;
    }
	return false;


}



    void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value)
{
    FTM_Type *FTMx;
    u8 chX;
    __JHAL_jpwm2ftm(pwm->id,&FTMx,&chX);


    FTM_SetChannelValue(FTMx,chX,  value);


}


