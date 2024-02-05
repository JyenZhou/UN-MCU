#ifndef _ads_1110
#define _ads_1110
//https://www.jianshu.com/p/e0b448995316
#include "JHAL.h"



//这个地址是根据芯片型号固定的  ADS1110A0IDBVT  中A0就是ED0 以此类推A7就是ED7
typedef enum
{   
    ADS1110_ADDRESS_ED0=0x90,
    ADS1110_ADDRESS_ED1,
	 ADS1110_ADDRESS_ED2,
	 ADS1110_ADDRESS_ED3,
	 ADS1110_ADDRESS_ED4,
	 ADS1110_ADDRESS_ED5,
	 ADS1110_ADDRESS_ED6,
	 ADS1110_ADDRESS_ED7,
 
	
} ADS1110_Addr;

 
//满量程电压 2.048/pga
typedef enum
{

    ADS1110_PGA1_2V048        =     0x0U  , // +/- 2.048    精度为0.125 mV/LSB。
    ADS1110_PGA2_1V024       =         0x1U , // +/- 1.024 精度为0.0625 mV/LSB。
    ADS1110_PGA4_0V512          =         0x2U  , // +/- 0.512 精度为0.0078125 mV/LSB。
    ADS1110_PGA8_8_0V256      =        0x3U  , // +/- 0.256  精度为0.00390625 mV/LSB。
 

} ADS1110_PGA ; //量程选择




//ADS1115_Speed_SPS_8    每秒能采样8次 的每个采样周期的时间间隔（毫秒） = 1000毫秒 / 8 = 125毫秒
//ADS1115_Speed_SPS_860 每秒能采样800次0  每个采样周期的时间间隔（毫秒） = 1000毫秒 / 860 ≈ 1.1628毫秒
 
typedef enum
{

    ADS1110_Speed_SPS_240_12Bit     =          0x0U,
    ADS1110_Speed_SPS_60_14Bit,
    ADS1110_Speed_SPS_30_15Bit,
    ADS1110_Speed_SPS_15_16Bit,
 
} ADS1110_Speed; //转换速率



typedef struct
{

//stm32 i2c.delayMultiple=10 是可以的

    JHAL_I2CSF i2c;


    ADS1110_PGA pga;
    ADS1110_Speed speed;

    //true=单次转换 false=连续转换
    bool isSingleConversion :1;

    //采样多次滤波配置
    JHAL_FilteredMode  filteredModel;
    //采样次数小于等于1时则只返回原数据
    u8 samplingCount4filter;
 

}  ADS1110;






bool  ads1110Open( ADS1110  *config);

//mdianNumbe 中位数个数
//true读取成功 false 读取失败
bool ads1110GetVoltage(ADS1110  *config , JHAL_ADCInfo  *adcInfo  );



#endif
