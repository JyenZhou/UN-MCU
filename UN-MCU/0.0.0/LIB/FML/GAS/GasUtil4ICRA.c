#include "GasUtil4ICRA.h"
/*传感器短时间变化报警  GasUtil4ICRA=GasUtil for Instantaneous Change Rate Alarm  */
//是欧姆为单位的电阻


//传感器个数
//#define GasUtil4ICRA_GasSensorNumber	2
//与alarmWindowTimeMs对应   例如1秒采集下5分钟 300个点即可  若RAM不足可适量减少但要保证传感器响应速度够 否则无法触发阈值  另需要满足初始化的存储
//#define GasUtil4ICRA_BuffSiZe 300
#if (GasUtil4ICRA_GasSensorNumber*GasUtil4ICRA_BuffSiZe)



static uint32 gas_lastAcqTime[GasUtil4ICRA_GasSensorNumber] =  {0};
static uint32_t rsShieldMIn_count[GasUtil4ICRA_GasSensorNumber] = {0};
static uint32_t rsShieldMax_count[GasUtil4ICRA_GasSensorNumber] = { 0 };
//指定长时间应该高于的阻值 否则报传感器故障
static GasUtil4ICRA_InfoTypeDef utilInfo[GasUtil4ICRA_GasSensorNumber]= {0};
//用户的配置参数
static GasUtil4ICRA_Configuration configurationParameters[GasUtil4ICRA_GasSensorNumber] = {0};
//报警回调
static GasUtil4ICRA_SensorAlertCallback alert;
static GasUtil4ICRA_GetSystemDayMs systemRunDayMS;
// 0.传感器初始化配置
void GasUtil4ICRA_Init(const GasUtil4ICRA_Configuration*parameters,GasUtil4ICRA_GetSystemDayMs getSystemDayMs,GasUtil4ICRA_SensorAlertCallback GasUtil4ICRA_SensorAlertCallback)
{
    alert = GasUtil4ICRA_SensorAlertCallback;
    systemRunDayMS  =getSystemDayMs;
    //防止用户定义局部指针变量后释放掉 这里深拷贝
    memcpy(configurationParameters, parameters, sizeof(configurationParameters));
    for (uint8_t i = 0; i < GasUtil4ICRA_GasSensorNumber; i++)
    {
        GasUtil4ICRA_gotPowerON(i);
        //暂时只有这个会存在未赋值被外面获取
        utilInfo[i].simulatedCommonModeRejectoValue=0;
    }
}
static bool __simulatedCommonModeEnable=false;
void GasUtil4ICRA_updateVCInfo(float vcMax, float vcMin, int gasIndex,bool simulatedCommonModeEnable)
{
//波动一般不会达到这个真正极值了  另外后面计算中也是按照极值计算的 正常时候波动是上下的不是单向的  除2是因为RL上面存在滤波
    utilInfo[gasIndex].vcUndulate = (vcMax - vcMin)/2;
    //选用最大值当做VC可以尽量避免出现负值
    utilInfo[gasIndex].vc = vcMax;
    //当除掉vl最大时候 即RS最小的有效值  该类传感器长期处于过大/小的值中是没办法检测的
    utilInfo[gasIndex].rsShield_Min  = utilInfo[gasIndex].vcUndulate *  configurationParameters[gasIndex].rl / ( utilInfo[gasIndex].vc - utilInfo[gasIndex].vcUndulate);
    utilInfo[gasIndex].rsShield_Max = ( utilInfo[gasIndex].vc - utilInfo[gasIndex].vcUndulate) * configurationParameters[gasIndex].rl   / utilInfo[gasIndex].vcUndulate;
    __simulatedCommonModeEnable=simulatedCommonModeEnable;
}


/*rs数据 及对应时间 缓存区；上次最后一个点位置；是否更新最后一个点位置*/
static float rsDataBuff[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
static uint32 timeMsBuff[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
//共模抑制器的值集合
static float simulatedCommonModeRejectorValueList[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
/*true则报警*/

// RS保护  true故障  false正常
bool rsShieldAlarmDetection(float *rs, int gasIndex)
{

//1Ω是为了方便浮点数的比较
    if (*rs <= (utilInfo[gasIndex].rsShield_Min+1))
    {

        //爬坡阶段最低阻值检测有预热时间  当爬坡标志结束说明传感器电阻上去了  此时若还到了下限值则立即触发报警
        if (!  utilInfo [gasIndex].isSteepGrade )
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_LOW, gasIndex);
            return true;
        }

        else if(rsShieldMIn_count[gasIndex]++ > configurationParameters[gasIndex].alarm_timeout_limitRsTimeMs)
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_LOW, gasIndex);
            return true;
        }
        //因为vc的波动这个已经是可能的极限值了 为了不计算有误 这里直接赋值该值
        *rs=utilInfo[gasIndex].rsShield_Min;
    }
    else
    {
        rsShieldMIn_count[gasIndex] = 0;
    }


    if (*rs >= (utilInfo[gasIndex].rsShield_Max-1))
    {
        if (!  utilInfo [gasIndex].isSteepGrade )
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_HIGH, gasIndex);
            return true;
        }
        else if(rsShieldMax_count[gasIndex]++ > configurationParameters[gasIndex].alarm_timeout_limitRsTimeMs)
        {
            alert(*rs, GASA_INVALID_DIRECTION,GASA_SRC_RS_HIGH, gasIndex);
            return true;

        }


        *rs=utilInfo[gasIndex].rsShield_Max;
    }
    else
    {
        rsShieldMax_count[gasIndex] = 0;
    }
    return false;

}

//模拟共模抑制器
void __GasUtil4ICRA_simulatedCommonModeRejector(float vl, float *rs, float *rsMin,u8 gasIndex)
{

    *rs = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
    if(!__simulatedCommonModeEnable)
    {
        return;
    }

    //防止共模干扰时vl>vc
    if (vl >utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate)
    {
        vl=utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate;
    } else if(vl<utilInfo[gasIndex].vcUndulate)
    {
        vl=utilInfo[gasIndex].vcUndulate;
    }


    *rs = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
    // VC的波动也会影响VL结果 所以加在这里
    vl +=  utilInfo[gasIndex].vcUndulate;
    if (vl >= utilInfo[gasIndex].vc)
    {
        vl =utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate;

    }
    *rsMin = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
}


/*
 * 是否是上升趋势
leftStartIndex 左侧（最新）数据开始坐标
segmentsNumber 将数据分成3段后每段的大小  必须大于3   采样频率高这个值相对大点为好
*/

bool __GasUtil4ICRA_isUptrend(uint8 leftStartIndex, uint8 segmentsNumber, float data[])
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
    //数组中左边的数据是最新的 //+0.001是为了防止低电阻下使用基线值导致平均值被影响从而提前满足条件 很快触发上电完成  如果最新的比较大那就是上升趋势 否则下降趋势完成预热
    if(argRegihtData<argLeftData+0.001f)
    {
        return true;
    } else {
        return false;
    }
}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
 *@ 函数功能或简介: 传感器数据处理
 * @输入参数:   无
 * @返 回 值: true 触发报警  false  不报警
 * @备注: 无
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/

//上电时候如果上升报警要排除稳定前的值  只有为0时队列中所有才有效
static u16 powerOnFilterCount=0;
static u16 powerOnFilterEnd=0;
void GasUtil4ICRA_PushData(float vl, u8 gasIndex)
{
    float rs = 0, rsMin = 0;
    __GasUtil4ICRA_simulatedCommonModeRejector(vl, &rs, &rsMin, gasIndex);
    if( rsShieldAlarmDetection(&rs, gasIndex))
    {
        return;
    }
    //当前系统运行的计时
    uint32 nowTime = systemRunDayMS();
    //点数的时间累计 最大、最小RS
    uint32 addUpPointTimeMS = 0;
    float maxRS = 0;
    float minRS = 0x7f7fffff ;




    //所有的缓存数据后移一位  下标0给最新的数据
    for (int i = GasUtil4ICRA_BuffSiZe - 1; i > 0; i--)
    {
        rsDataBuff[gasIndex][i] = rsDataBuff[gasIndex][i - 1];
        timeMsBuff[gasIndex][i] = timeMsBuff[gasIndex][i - 1];
        simulatedCommonModeRejectorValueList[gasIndex][i]=simulatedCommonModeRejectorValueList[gasIndex][i - 1];
    }

    //  上次时间为0说明首次进来  间隔时间应该为0 但是0在后面属于无效值 所以用1
    //若进来间隔时间<计时最小台阶也可能为0一次要避免这种风险，当外面大的执行一次超过间隔上限时就会连续进来（定时器中是独立计时置位的），例如获取系统信息时为了上位机好解析不和报警信息混着就加了延时
    //第二天的则需要加上Daytimesunset_MS再去减掉前一天的
    uint32 intervalMs = (gas_lastAcqTime[gasIndex] == 0 || nowTime == gas_lastAcqTime[gasIndex]) ? 1 : (nowTime > gas_lastAcqTime[gasIndex] ? nowTime - gas_lastAcqTime[gasIndex] : nowTime + Day2Msec - gas_lastAcqTime[gasIndex]);
    gas_lastAcqTime[gasIndex] = nowTime;
    //赋值
    timeMsBuff[gasIndex][0] = intervalMs;
    rsDataBuff[gasIndex][0] = rs;
    simulatedCommonModeRejectorValueList[gasIndex][0]=rsMin/rs;
    /****** 求最大值最小值的比值 *********/
    //有效点定义 主要方便给后面算斜率
    int effectivePoint;
//判断稳定趋势的标志位置
    u16 startEffectiveIndex=0;
//累计的数据达到目标时间 主要给首次上电找基准用
    bool alarmWindowTimeMsOver=false;


    u16 maxRsIndex=0;
    u16 minRsIndex=0;
    float simulatedCommonModeRejectoValue = 0;
    //遍历累计的点数
    for (uint16 i = 0; i < GasUtil4ICRA_BuffSiZe; i++)
    {
        effectivePoint = i;
        //累加时间
        addUpPointTimeMS += timeMsBuff[gasIndex][i];
        //等于0 说明该处开始后面都是没有被赋值的 没必要比较过去
        if (timeMsBuff[gasIndex][i] == 0)
        {
            break;
        }
        //当累计时间到了指定时间   没必要比较过去 退出更多的计算
        if ((addUpPointTimeMS >= configurationParameters[gasIndex].alarmWindowTimeMs)||(i==GasUtil4ICRA_BuffSiZe-1))
        {
            alarmWindowTimeMsOver=true;
            break;
        }
//上电状态已进行判断 动态阈值可以选取最灵敏状态
        simulatedCommonModeRejectoValue=max(simulatedCommonModeRejectoValue,simulatedCommonModeRejectorValueList[gasIndex][i]);

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

    utilInfo[gasIndex].simulatedCommonModeRejectoValue=1-simulatedCommonModeRejectoValue;

    float s_s0;

    for (uint16 i = 0; i < effectivePoint; i++)
    {

        if(rsDataBuff[gasIndex][i]<minRS)
        {

            if(!utilInfo [gasIndex].isSteepGrade)

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


            if(__simulatedCommonModeEnable)
            {
                s_s0 = 1-(minRS/maxRS)-utilInfo[gasIndex].simulatedCommonModeRejectoValue;
            } else {
                s_s0 = 1-(minRS/maxRS);
            }
            //报警判断处理放在这里面是为了方便判断报警源与最近一致
            if ( s_s0  > configurationParameters[gasIndex].alarm_s_s0_abs)
            {

                break;
            }

        }

    }



    if(utilInfo [gasIndex].isSteepGrade)
    {
//当有效数据能够分3段 并且整体时间=窗口期
        if(alarmWindowTimeMsOver)
        {
            //获取末端数的斜率用来判断是否稳定  (effectivePoint-startEffectiveIndex)%3 和(effectivePoint-startEffectiveIndex)/3配合 保证能够对比到有效数据的最后一个 防止最后一个上电时候电压过低在判断趋势时没带入报上电完成 但是rs/r0时候却带进去了 不一致导致误报
            if( !__GasUtil4ICRA_isUptrend(startEffectiveIndex+((effectivePoint-startEffectiveIndex)%3),(effectivePoint-startEffectiveIndex)/3, rsDataBuff[gasIndex]))
            {
                utilInfo [gasIndex].isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
                powerOnFilterCount=maxRsIndex;
            }
        }
    }

    if ( s_s0  > configurationParameters[gasIndex].alarm_s_s0_abs)
    {
//当最近一段数据平稳时 只是之前某段数据比较小时 只会当时报氧化性的
        if(maxRsIndex>minRsIndex)
        {
            if(utilInfo [gasIndex].isSteepGrade)
            {
                utilInfo [gasIndex].isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
//当前数据+下一个数据 所以是2
                powerOnFilterCount=maxRsIndex;
            }
            alert(s_s0, GASA_REDUCTION, GASA_SRC_SHARP_REACTION, gasIndex);
        } //氧化性响应的报警 爬坡时候不报
        else if (!utilInfo [gasIndex].isSteepGrade)
        {
            alert(s_s0, GASA_OXIDATION, GASA_SRC_SHARP_REACTION, gasIndex);
        }
    }


    if(0!=powerOnFilterEnd&&powerOnFilterCount!=GasUtil4ICRA_BuffSiZe)
    {
        powerOnFilterCount++;
    }
}

//清空报警检测器缓存数据 一般切换传感器时调用
void GasUtil4ICRA_gotPowerON(u8 gasIndex)
{
    for(u16 i=0; i<GasUtil4ICRA_BuffSiZe; i++)
    {
        timeMsBuff[gasIndex][i]=0;
        rsDataBuff[gasIndex][i]=0;
        simulatedCommonModeRejectorValueList[gasIndex][i]=0;
    }


    gas_lastAcqTime[gasIndex] = 0;
    utilInfo [gasIndex].isSteepGrade  = true;
    rsShieldMIn_count[gasIndex]=0;
    powerOnFilterEnd=0;
    powerOnFilterCount=0;

}

void GasUtil4ICRA_getUtilInfo( GasUtil4ICRA_InfoTypeDef** infos)
{
    *infos=utilInfo;
}

#endif


