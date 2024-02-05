/**
  ****************************GasUtil4ICRA2.c******************************************
  * @brief
  *传感器短时间变化报警  阵列版  输出信号最强的  GasUtil4ICRA2=GasUtil for Instantaneous Change Rate Alarm

#define GasUtil4ICRA2_GasSensorNumber	2 传感器个数 默认最大支持16个 需要更多要修改回调类型
#define GasUtil4ICRA2_BuffSiZe 300 与alarmWindowSec对应   例如1秒采集下5分钟 300个点即可
若RAM不足可适量减少但要保证传感器响应速度够 否则无法触发阈值  另需要满足初始化的存储

    ******************************************************************************
  * @file     GasUtil4ICRA2.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-11-24
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
#include "GasUtil4ICRA2.h"

#if !GasUtil4ICRA2_BuffSiZe
需要设置数据缓存大小
#endif


static uint32 gas_lastAcqTime =  0;
static uint32_t rsShieldMInTimerBuff[GasUtil4ICRA2_GasSensorNumber] = {0};
static uint32_t rsShieldMaxTimeBuff[GasUtil4ICRA2_GasSensorNumber] = { 0 };
//指定长时间应该高于的阻值 否则报传感器故障
static GasUtil4ICRA2_InfoTypeDef utilInfo= {0};
//用户的配置参数
static GasUtil4ICRA2_Configuration configurationParameters = {0};
//报警回调
static GasUtil4ICRA2_OutCallback outCallback;

// 0.传感器初始化配置
void GasUtil4ICRA2_Init( GasUtil4ICRA2_Configuration *parameters, GasUtil4ICRA2_OutCallback callback)
{
    outCallback = callback;

    configurationParameters=*parameters;
    GasUtil4ICRA2_gotPowerON();


}
static bool __simulatedCommonModeEnable=false;


/*rs数据 及对应时间 缓存区；上次最后一个点位置；是否更新最后一个点位置*/
static float rsDataBuff[GasUtil4ICRA2_GasSensorNumber][GasUtil4ICRA2_BuffSiZe] = {0};
static uint32 timeMsBuff[GasUtil4ICRA2_BuffSiZe] = {0};

/*true则报警*/

// RS保护  0正常 1过小 2过大
GASA_Exception rsShieldAlarmDetection(float *rs, int gasIndex, u32 intervalSec )
{


    if (*rs < (configurationParameters.rsMin[gasIndex]))
    {

        //爬坡阶段最低阻值检测有预热时间  当爬坡标志结束说明传感器电阻上去了  此时若还到了下限值则立即触发报警
        if (!  utilInfo .isSteepGrade )
        {

            return GASAE_RS_LOW;
        }

        else if((rsShieldMInTimerBuff[gasIndex]+=intervalSec) > configurationParameters.alarmLimitRsTimeoutSec)
        {

            return GASAE_RS_LOW;
        }
        //因为vc的波动这个已经是可能的极限值了 为了不计算有误 这里直接赋值该值
        *rs=configurationParameters.rsMin[gasIndex];
    }
    else
    {
        rsShieldMInTimerBuff[gasIndex] = GASAE_Normal;
    }


    if (*rs >configurationParameters.rsMax[gasIndex])
    {
        if (!  utilInfo .isSteepGrade )
        {
            return GASAE_RS_HIGH;
        }
        else if((rsShieldMaxTimeBuff[gasIndex]+=intervalSec) > configurationParameters.alarmLimitRsTimeoutSec)
        {

            return GASAE_RS_HIGH;

        }


        *rs=configurationParameters.rsMax[gasIndex];
    }
    else
    {
        rsShieldMaxTimeBuff[gasIndex] = GASAE_Normal;
    }
    return GASAE_Normal;

}



/*
 * 是否是上升趋势
leftStartIndex 左侧（最新）数据开始坐标
segmentsNumber 将数据分成3段后每段的大小  必须大于3   采样频率高这个值相对大点为好
*/

bool __GasUtil4ICRA2_isUptrend(uint8 leftStartIndex, uint8 segmentsNumber, float data[])
{
//控制对比的每段数据个数  分为3段  第一段和第三段对比

    u8 regihtStartIndex=leftStartIndex+segmentsNumber*2;
    float  argLeftData=data[leftStartIndex];
    float  argRegihtData=data[regihtStartIndex];
    for(u8 i=1; i<segmentsNumber-1; i++)
    {
        argLeftData+=data[leftStartIndex+i];
        argLeftData/=2;

        argRegihtData+=data[i+regihtStartIndex];
        argRegihtData/=2;

    }
    //数组中左边的数据是最新的
    if(argRegihtData<argLeftData)
    {
        return true;
    } else {
        return false;
    }
}
//变化率不到1-n%不算拐点 防止电阻波动造成的假拐点
//#define Filtration_FlectionPoint 1
//数组拐点查找   从下标0开始找出最近的一个拐点  返回值0无拐点  用于判断当前是上升还是下降
u16   findInflectionPoint(u32* array,u16 maxIndex)
{
    for(u16 i=1; i<maxIndex-1; i++)
    {
        if(array[i]<array[-i])
        {
            if(array[i+1]>array[i])
            {
                return i;
            }
        } else
        {
            if(array[i+1]<array[i])
            {
                return i;
            }

        }

    }
    return 0;
}




/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
 *@ 函数功能或简介: 报警检测传值  阵列启用时必须一同传入 gasIndex无效
 * @输入参数:   无
 * @返 回 值:  无
 * @备注: 无
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/

//上电时候如果上升报警要排除稳定前的值  只有为0时队列中所有才有效
static u16 powerOnFilterCount=0;
static u16 powerOnFilterEnd=0;

static   JHAL_FloatInde4Set rs_r0[GasUtil4ICRA2_GasSensorNumber];
void GasUtil4ICRA2_PushData(float *vl)
{

    float rs;

    GASA_Exception exception=GASAE_Normal;
    int8 exIndex=-1;


    float rsShieldMaxRs=0;
    float rsShieldMinRs=0x7f7fffff;
    //当前系统运行的计时
    uint32 nowTime = jsystemMs;

    if(nowTime<=gas_lastAcqTime)
    {
//间隔不足一秒的不计算
        return;
    }
    uint32 intervalSec = nowTime-gas_lastAcqTime;
    gas_lastAcqTime = nowTime;



    for(u8 gasIndex=0; gasIndex<GasUtil4ICRA2_GasSensorNumber; gasIndex++) {

        rs = ((configurationParameters.vc[gasIndex] - vl[gasIndex]) * configurationParameters.rl[gasIndex]) / vl[gasIndex];

        exception =rsShieldAlarmDetection(&(rs), gasIndex,intervalSec);
        if(exception==GASAE_RS_LOW)
        {
            if(rsShieldMinRs>rs)
            {
                exIndex=gasIndex;
                rsShieldMinRs=rs;

            }


        } else if(exception==GASAE_RS_HIGH)
        {

            if(rsShieldMaxRs<rs)
            {
                exIndex=gasIndex;
                rsShieldMinRs=rs;

            }
        }



        //点数的时间累计 最大、最小RS
        uint32 addUpPointTimeMS = 0;
        float maxRS = 0;
        float minRS = 0x7f7fffff ;




        //所有的缓存数据后移一位  下标0给最新的数据
        for (int i = GasUtil4ICRA2_BuffSiZe - 1; i > 0; i--)
        {
            rsDataBuff[gasIndex][i] = rsDataBuff[gasIndex][i - 1];
            timeMsBuff[i] = timeMsBuff[i - 1];
        }

        //赋值
        timeMsBuff[0] = intervalSec;
        rsDataBuff[gasIndex][0] = rs;
        /****** 求最大值最小值的比值 *********/
        //有效点定义 主要方便给后面算斜率
        int effectivePoint;
//判断稳定趋势的标志位置
        u16 startEffectiveIndex=0;
//累计的数据达到目标时间 主要给首次上电找基准用
        bool alarmWindowSecOver=false;

        u16 maxRsIndex=0;
        u16 minRsIndex=0;

        //遍历累计的点数
        for (uint16 i = 0; i < GasUtil4ICRA2_BuffSiZe; i++)
        {
            effectivePoint = i;
            //累加时间
            addUpPointTimeMS += timeMsBuff[i];
            //等于0 说明该处开始后面都是没有被赋值的 没必要比较过去
            if (timeMsBuff[i] == 0)
            {
                break;
            }
            //当累计时间到了指定时间   没必要比较过去 退出更多的计算
            if ((addUpPointTimeMS >= configurationParameters.alarmWindowSec)||(i==GasUtil4ICRA2_BuffSiZe-1))
            {
                alarmWindowSecOver=true;
                break;
            }


            if(rsDataBuff[gasIndex][i]>maxRS)
            {
                maxRsIndex=i;
                maxRS=rsDataBuff[gasIndex][i];
            }
        }
        if(effectivePoint==0)
        {
//只有通过手动切换传感器时时间瞬间变大超过预置5分钟所有没有有效点 会导致0/1 变化率爆表
            return;
        }



        for (uint16 i = 0; i < effectivePoint; i++)
        {

            if(rsDataBuff[gasIndex][i]<minRS)
            {

                if(!utilInfo.isSteepGrade)

                {
                    if(i<powerOnFilterCount) {
                        minRsIndex=i;
                        minRS=rsDataBuff[gasIndex][i];
                    }
                } else {
//上电稳定前的minrs必须要比max后采集到
                    if(maxRsIndex>=i) {
                        minRS=rsDataBuff[gasIndex][i];
                        minRsIndex=i;
                    }
                }
            }
        }

        if(utilInfo.isSteepGrade)
        {
//当有效数据能够分3段 并且整体时间=窗口期
            if(alarmWindowSecOver)
            {
                //获取末端数的斜率用来判断是否稳定  (effectivePoint-startEffectiveIndex)%3 和(effectivePoint-startEffectiveIndex)/3配合 保证能够对比到有效数据的最后一个 防止最后一个上电时候电压过低在判断趋势时没带入报上电完成 但是rs/r0时候却带进去了 不一致导致误报
                if( !__GasUtil4ICRA2_isUptrend(startEffectiveIndex+((effectivePoint-startEffectiveIndex)%3),(effectivePoint-startEffectiveIndex)/3, rsDataBuff[gasIndex]))
                {
                    utilInfo.isSteepGrade=false;
                    powerOnFilterEnd=effectivePoint;
                    powerOnFilterCount=maxRsIndex;
                }
            }
        }
//当最近一段数据平稳时 预热完成
        if(utilInfo.isSteepGrade&&maxRsIndex>minRsIndex)
        {   //变化率达到10%判断为预热完成
            if(minRS/maxRS<0.9f)
            {
                utilInfo.isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
                powerOnFilterCount=maxRsIndex;
            }

        }


//基线的处理
//预热时基线是最大RS
        if(utilInfo.isSteepGrade)
        {
            utilInfo.baseLine[gasIndex]=maxRS;
            rs_r0[gasIndex].value=0.999f;
        } else
        {
//如果当前值和窗口内最大值/最小值 变化小于x% 基线允许自恢复
            if((minRS/maxRS)> configurationParameters.baseLineSelfRecoveryaAllow4WindowsRate )
            {
                float differenceRsValue=((float)intervalSec /configurationParameters.baseLineSelfRecoverySec*fabs(utilInfo.baseLine[gasIndex]-rs));

                if(utilInfo.baseLine[gasIndex]>rs)
                {
                    utilInfo.baseLine[gasIndex]-=differenceRsValue;
                } else
                {
                    utilInfo.baseLine[gasIndex]+=differenceRsValue;
                }

            }

            rs_r0[gasIndex].value=rs/utilInfo.baseLine[gasIndex];

        }
        rs_r0[gasIndex].index=gasIndex;
    }



//将变化率
    JHAL_sortFloatIndexSetAsc(rs_r0,GasUtil4ICRA2_GasSensorNumber);

    outCallback( exception,exIndex,rs_r0);


//上电有效值记数
    if(0!=powerOnFilterEnd&&powerOnFilterCount!=GasUtil4ICRA2_BuffSiZe)
    {
        powerOnFilterCount++;
    }

}


//清空报警检测器缓存数据 一般切换传感器时调用
void GasUtil4ICRA2_gotPowerON()
{
    for(u16 gasIndex=0; gasIndex<GasUtil4ICRA2_GasSensorNumber; gasIndex++)
    {
        for(u16 j=0; j<GasUtil4ICRA2_BuffSiZe; j++)
        {
            timeMsBuff[j]=0;
            rsDataBuff[gasIndex][j]=0;

        }

        gas_lastAcqTime = 0;
        rsShieldMInTimerBuff[gasIndex]=0;
    }

    utilInfo.isSteepGrade  = true;
    powerOnFilterEnd=0;
    powerOnFilterCount=0;

}

void GasUtil4ICRA2_getUtilInfo( GasUtil4ICRA2_InfoTypeDef* infos)
{
    *infos=utilInfo;
}


