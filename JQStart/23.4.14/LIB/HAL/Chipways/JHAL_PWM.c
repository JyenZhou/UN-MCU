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




void __JHAL_jpwm2ftm(u8 dev,FTM_Type **FTMx, u8 *chX)
{
    if( dev==0)
    {
        *FTMx=FTM1;
        *chX=3;
    }
    if( dev==1)
    {
        *FTMx=FTM0;
        *chX=1;
    }
    else {
//不存在或未实现
        while(true);
    }




}

//PWM触发AD采集的接口
u8 __JHAL_jpwm2adcTriggerSoureValue(u8 dev)
{

    if( dev==0 )
    {
        return ADC_HT_FTM0CH1MAP;
    } else {
//不存在或未实现
        while(true);
    }

}



/**
  * 初始化pwm模块的x通道
  * 输入   无
  * 返回   无
  */
void __JHAL_pwmOpen(JHAL_PWM  *config )
{
    FTM_Type * FTMx;
    u8 chX;
    __JHAL_jpwm2ftm(config ->dev ,&FTMx,&chX);
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
        if(config->dev==1)
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
        if(config->dev==0)
        {
            SIM_PINSEL_FTM1CH3(FTM1CH3_PS_PTE3);
        }
    }
    FTM_WriteProtectDisable(FTMx);
    FTM_CenterAlignedPWMInit(FTMx, chX, FTM_PWM_HIGHTRUEPULSE);
    FTM_ChannelIntCmd(FTMx,chX,DISABLE);
    FTM_SetChannelValue(FTMx,chX,   config->initialValue);
    FTM_SetCountInitValue(FTMx,0);
    FTM_SetModValue(FTMx, config->maxValue);
    FTM_OverflowITCmd(FTMx,DISABLE);
    FTM_Init(FTMx,&FTM_InitStructure);
    FTM_SetChnTriggerCmd(FTMx, chX,  ENABLE);
    FTM_CVSyncEnable(FTMx,chX,ENABLE);

}

 

/*
JHAL_PWM0 ->  PTE3->FTM1_CH3
JHAL_PWM1 ->  PTB3->FTM0_CH1
clockSource 时钟源
#define FTM_CLOCK_SYSTEMCLOCK            		1            < System clock
#define FTM_CLOCK_FIXEDFREQCLOCK        		2            < Fixed Freq Clock
#define FTM_CLOCK_EXTERNALCLOCK          		3             < External Clock


prescaler 预分频系数 0->FTM_CLOCK_PS_DIV1 ...
#define FTM_CLOCK_PS_DIV1                		0              < 1分频
#define FTM_CLOCK_PS_DIV2                		1              < 2分频
#define FTM_CLOCK_PS_DIV4                		2              < 4分频
#define FTM_CLOCK_PS_DIV8                		3              < 8分频
#define FTM_CLOCK_PS_DIV16               		4              < 16分频
#define FTM_CLOCK_PS_DIV32               		5              < 32分频
#define FTM_CLOCK_PS_DIV64               		6              < 64分频
#define FTM_CLOCK_PS_DIV128              		7              < 128分频
*/
bool JHAL_pwmOpen(JHAL_PWM *config   )
{
    if(! config-> __info.isOpen) {



//设置初始值大于最大值
        while(config->initialValue>config->maxValue);
        __JHAL_pwmOpen( config);
			  
			return  config-> __info.isOpen=true; 
    }
		return false;
}

bool  JHAL_pwmClose(JHAL_PWM * pwm) {
    if(pwm-> __info.isOpen) {
        FTM_Type *FTMx;
        u8 chX;
        __JHAL_jpwm2ftm(pwm->dev,&FTMx,&chX);

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



void JHAL_pwmSetValue(JHAL_PWM *pwm,u16 value)
{
    FTM_Type *FTMx;
    u8 chX;
    __JHAL_jpwm2ftm(pwm->dev,&FTMx,&chX);


    FTM_SetChannelValue(FTMx,chX,  value);


}


