#ifndef __GasUtil4ICRA2_H__
#define __GasUtil4ICRA2_H__


#include "JHAL.h"
//传感器个数
//#define GasUtil4ICRA2_GasSensorNumber	2
//与alarmWindowTimeMs对应   例如1秒采集下5分钟 300个点即可  若RAM不足可适量减少但要保证传感器响应速度够 否则无法触发阈值  另需要满足初始化的存储
//#define GasUtil4ICRA2_BuffSiZe 300

typedef enum
{

    //正常
    GASAE_Normal=0,
    //传感器长时间阻值过低 对应微泄漏
    GASAE_RS_LOW,
//相当传感器故障  多传感器可以不报警切传感器
    GASAE_RS_HIGH
}
//异常
GASA_Exception;



typedef enum
{
    //氧化性
    GASA_OXIDATION,
    //还原性
    GASA_REDUCTION

}//响应
GASA_Type;

typedef struct
{

    //报警窗口有效时间  例如设置1分钟主要兼容低功耗下非等间隔采集造成时间过长导致误报现象


    uint32_t alarmWindowSec;
//基线跟随时间
    u32 baseLineSelfRecoverySec;
//窗口波动>rSmin_rsMax 时允许恢复    0-1 越小越容易恢复
    float baseLineSelfRecoveryaAllow4WindowsRate;
    //rs长期时间过低/高报警触发的时间阈值
    float alarmLimitRsTimeoutSec;
//rs的极限值
    float  rsMin[GasUtil4ICRA2_GasSensorNumber];
    float  rsMax[GasUtil4ICRA2_GasSensorNumber];

    //匹配电阻大小 输出时将与此时传入单位一致
    float rl[GasUtil4ICRA2_GasSensorNumber];
    float vc[GasUtil4ICRA2_GasSensorNumber];


} GasUtil4ICRA2_Configuration;
typedef struct
{
    //是否处于上电爬坡状态
    bool isSteepGrade;
    float baseLine[GasUtil4ICRA2_GasSensorNumber];
} GasUtil4ICRA2_InfoTypeDef;

//输出回调 JHAL_FloatInde4Set 变化率rs/r0
typedef void        (*GasUtil4ICRA2_OutCallback)(GASA_Exception ex, int8 exIndex,JHAL_FloatInde4Set *arraySet);



//气体传感器初始参数配置  配置参数必须全局/静态变量
void GasUtil4ICRA2_Init( GasUtil4ICRA2_Configuration *parameters, GasUtil4ICRA2_OutCallback callback);
//报警检测传值一同传入
void GasUtil4ICRA2_PushData(float *vl);
//传感器每次上电的时候调用
void GasUtil4ICRA2_gotPowerON(void);
//获取一些内部阈值信息 用于debug
void GasUtil4ICRA2_getUtilInfo( GasUtil4ICRA2_InfoTypeDef* infos);

#endif


