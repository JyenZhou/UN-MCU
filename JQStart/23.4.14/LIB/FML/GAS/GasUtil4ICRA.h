#ifndef __GasUtil4ICRA_H__
#define __GasUtil4ICRA_H__


#include "JHAL.h"
//传感器个数
//#define GasUtil4ICRA_GasSensorNumber	2
//与alarmWindowTimeMs对应   例如1秒采集下5分钟 300个点即可  若RAM不足可适量减少但要保证传感器响应速度够 否则无法触发阈值  另需要满足初始化的存储
//#define GasUtil4ICRA_BuffSiZe 300


typedef enum
{

    //传感器瞬时变化过大
    GASA_SRC_SHARP_REACTION=0,
    //传感器长时间阻值过低 对应微泄漏
    GASA_SRC_RS_LOW,
//相当传感器故障  多传感器可以不报警切传感器
    GASA_SRC_RS_HIGH
}
//报警触发源
GASA_TriggerSource;



typedef enum
{
    //无效的参数
    GASA_INVALID_DIRECTION=0,
    //氧化性
    GASA_OXIDATION,
    //还原性
    GASA_REDUCTION

}//信号方向
GASA_TriggerDirection;

typedef struct
{


    //报警窗口单位时间  例如设置1分钟   必须是该时间内rs/r0超出阈值才报警  主要兼容低功耗下非等间隔采集造成时间过长导致误报现象  建议值5分钟  5*60*1000
    uint32_t alarmWindowTimeMs;
    //单位时间内报警阈值 注意是“变化了”不是“变化到” abs(rs/r0 -1) 建议值：HGS1000>=0.2  HGS1001>=0.4
    float alarm_s_s0_abs;

    //rs长期时间过低/高报警触发的时间阈值
    float alarm_timeout_limitRsTimeMs;
    //匹配电阻大小 输出时将与此时传入单位一致
    float rl;

} GasUtil4ICRA_Configuration;
typedef struct
{


    //rs的极限值
    float  rsShield_Min;
    float  rsShield_Max;
    //VC波动值  由 GasUtil4ICRA_updateVCInfo 传进去的差值
    float  vcUndulate;
    //选用最大值当做VC可以尽量避免出现负值
    float vc;
    //共模抑制变化率   这个是动态的     报警阈值相当于这个值 +用户传进来的alarm_s_s0_abs
    float simulatedCommonModeRejectoValue;
    //是否处于上电爬坡状态
    bool isSteepGrade;
} GasUtil4ICRA_InfoTypeDef;

//GasUtil4ICRA_SensorAlertCallback 报警回调   rete 变化率    triggeroure触发源   目标传感器索引
typedef void        (*GasUtil4ICRA_SensorAlertCallback)(float rete, GASA_TriggerDirection direction,GASA_TriggerSource triggeroure, u8 gasIndex);
typedef uint32        (*GasUtil4ICRA_GetSystemDayMs)(void);


//气体传感器初始参数设置  配置的个数，每个传感器对应一个   传感器检测到异味时触发该中断回调
void GasUtil4ICRA_Init(const GasUtil4ICRA_Configuration *parameters,GasUtil4ICRA_GetSystemDayMs getSystemDayMs, GasUtil4ICRA_SensorAlertCallback GasUtil4ICRA_SensorAlertCallback);
//更新VC信息  若采集的不是差分信号 可能会产生共模干扰  可以通过软件模拟共模抑制器
//传进来vc极值即可  考虑到存在各种工作模式会影响到这个值 这里单独拿出来  当电源负载有变化时可以重新校准
//设置传感器的波动该函数至少要调用一次 单位必须是V  simulatedCommonModeEnable是否开启共模干扰  开启后一定程度上降低变化率 纹波大时可启用  一般无需启用
void GasUtil4ICRA_updateVCInfo(float vcMax, float vcMin, int gasIndex,bool simulatedCommonModeEnable);
//报警检测 传值  报警后触发回调
void GasUtil4ICRA_PushData(float vl, u8 gasIndex);
//传感器每次上电的时候调用
void GasUtil4ICRA_gotPowerON(u8 gasIndex);
//获取自动生成的一些实时值/阈值信息
void GasUtil4ICRA_getUtilInfo( GasUtil4ICRA_InfoTypeDef** infos);

#endif


