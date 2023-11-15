#ifndef _ads_1115
#define _ads_1115
//https://www.jianshu.com/p/e0b448995316
#include "JHAL.h"


typedef enum
{   //ADDR PIN ->GND
    ADS115_ADDRESS_GND=0x90,
    //ADDR PIN ->VDD
    ADS115_ADDRESS_VDD=0x92,
    //ADDR PIN ->SDA
    ADS115_ADDRESS_SDA=0x94,
    //ADDR PIN ->SCL
    ADS115_ADDRESS_SCL=0x96
} ADS1115_Addr;

typedef enum
{
    /******************************************************************************
    000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
    001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
    010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
    011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
    */
    ADS1115Channel_Diff_01       =           (0x0U << 4), // 差分输入0引脚和1引脚
    ADS1115Channel_Diff_03     =             (0x1U << 4),  // 差分输入0引脚和3引脚
    ADS1115Channel_Diff_13     =             (0x2U << 4),  // 差分输入1引脚和3引脚
    ADS1115Channel_Diff_23      =            (0x3U << 4),  // 差分输入2引脚和3引脚
    ADS1115Channel_SINGLE_0      =           (0x4U << 4),  //单端输入 0
    ADS1115Channel_SINGLE_1    =             (0x5U << 4),  //单端输入 1
    ADS1115Channel_SINGLE_2      =           (0x6U << 4),  //单端输入 2
    ADS1115Channel_SINGLE_3    =             (0x7U << 4)   //单端输入 3


} ADS1115Channel ; //通道选择 输入引脚选择和输入方式选择

typedef enum
{

    ADS1115_PGA_6        =     (0x0U << 1), // +/- 6.1144    精度为0.125 mV/LSB。
    ADS1115_PGA_4       =           (0x1U << 1), // +/- 4.096 精度为0.0625 mV/LSB。
    ADS1115_PGA_2           =           (0x2U << 1), // +/- 2.048 精度为0.03125 mV/LSB。
    ADS1115_PGA_1            =         (0x3U << 1), // +/- 1.024 精度为0.015625 mV/LSB。
    ADS1115_PGA_05          =         (0x4U << 1), // +/- 0.512 精度为0.0078125 mV/LSB。
    ADS1115_PGA_02             =          (0x5U << 1),  // +/- 0.256  精度为0.00390625 mV/LSB。

} ADS1115_PGA ; //量程选择



//ADS1115_Speed_SPS_8    每秒能采样8次 的每个采样周期的时间间隔（毫秒） = 1000毫秒 / 8 = 125毫秒
//ADS1115_Speed_SPS_860 每秒能采样800次0  每个采样周期的时间间隔（毫秒） = 1000毫秒 / 860 ≈ 1.1628毫秒

typedef enum
{

    ADS1115_Speed_SPS_8     =          0x0U,
    ADS1115_Speed_SPS_16,
    ADS1115_Speed_SPS_32,
    ADS1115_Speed_SPS_64,
    ADS1115_Speed_SPS_128,
    ADS1115_Speed_SPS_250,
    ADS1115_Speed_SPS_475,
    ADS1115_Speed_SPS_860
} ADS1115_Speed; //转换速率




typedef struct
{

//i2c.delayMultiple=10 是可以的

    JHAL_I2CSF i2c;


    ADS1115_PGA pga;
    ADS1115_Speed speed;

    //true=单次转换 false=连续转换
    bool isSingleConversion :1;

    //可选配置 转换完成低电平判断引脚  连续转换模式下无效
    JHAL_IO_Port alert_rdy_Port;
    u8 alert_rdy_Pin;
    //采样多次滤波配置
    JHAL_FilteredMode  filteredModel;
    //采样次数小于等于1时则只返回原数据
    u8 samplingCount4filter;

}  ADS1115;






bool  ads1115Open( ADS1115  *config);

//mdianNumbe 中位数个数
//true读取成功 false 读取失败
bool ads1115GetVoltage(ADS1115  *config,ADS1115Channel ch, JHAL_ADCInfo  *adcInfo  );



#endif
