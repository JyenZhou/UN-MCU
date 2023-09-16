#ifndef _ads_1115
#define _ads_1115
//https://www.jianshu.com/p/e0b448995316
#include "JHAL.h"

 
typedef enum
{ //ADDR PIN ->GND
	ADS115_ADDRESS_GND=0x90,
	 //ADDR PIN ->VDD
	ADS115_ADDRESS_VDD=0x92,
	//ADDR PIN ->SDA
	ADS115_ADDRESS_SDA=0x94,
	//ADDR PIN ->SCL
	ADS115_ADDRESS_SCL=0x96
}ADS1115_Addr;
 
typedef enum
{
	 /******************************************************************************
000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
	*/
  ADS1115Channel_Diff_01       =           (0x0U << 4), // 差分输入0引脚和1引脚
  ADS1115Channel_Diff_03     =             (0x1U << 4)  ,// 差分输入0引脚和3引脚
  ADS1115Channel_Diff_13     =             (0x2U << 4),  // 差分输入1引脚和3引脚
  ADS1115Channel_Diff_23      =            (0x3U << 4) , // 差分输入2引脚和3引脚
  ADS1115Channel_INGLE_0      =           (0x4U << 4) , //单端输入 0
  ADS1115Channel_SINGLE_1    =             (0x5U << 4) , //单端输入 1
  ADS1115Channel_SINGLE_2      =           (0x6U << 4) , //单端输入 2
  ADS1115Channel_SINGLE_3    =             (0x7U << 4)   //单端输入 3

 
}ADS1115Channel ;//通道选择 输入引脚选择和输入方式选择

typedef enum
{

ADS1115_PGA_6        =     (0x0U << 1), // +/- 6.1144
ADS1115_PGA_4       =           (0x1U << 1), // +/- 4.096
ADS1115_PGA_2           =           (0x2U << 1), // +/- 2.048
ADS1115_PGA_1            =         (0x3U << 1), // +/- 1.024
ADS1115_PGA_05          =         (0x4U << 1), // +/- 0.512
ADS1115_PGA_02             =          (0x5U << 1) , // +/- 0.256
	
}ADS1115_PGA ;//量程选择



//ADS1115_Speed_SPS_8    每秒能采样8次 的每个采样周期的时间间隔（毫秒） = 1000毫秒 / 8 = 125毫秒
//ADS1115_Speed_SPS_860 每秒能采样800次0  每个采样周期的时间间隔（毫秒） = 1000毫秒 / 860 ≈ 1.1628毫秒
//采样速度越快，每秒内ADS1115将从输入信号中采集更多的样本数据。因此，SPS越大，采样速度越快，但需要更多的时间和资源来处理更多的样本数据。
typedef enum
{
 
  ADS1115_Speed_SPS_8            =         (0x0U << 5),
  ADS1115_Speed_SPS_16         =           (0x1U << 5), 
  ADS1115_Speed_SPS_32           =         (0x2U << 5),
  ADS1115_Speed_SPS_64           =         (0x3U << 5),
  ADS1115_Speed_SPS_128           =        (0x4U << 5),
  ADS1115_Speed_SPS_250            =       (0x5U << 5),
  ADS1115_Speed_SPS_475            =       (0x6U << 5),
  ADS1115_Speed_SPS_860            =       (0x7U << 5)
}ADS1115_Speed; //转换速率



 
typedef struct
{
	
//这个是延时系数最小值应该为1 要是降低速率可以修改这里值  注意IO有翻转时间的间隔太小可能导致时序不准 所以0则全程没有延时正常情况下该时序是有问题的
  
   JHAL_IICConfig iicConfig;
     

 	ADS1115_PGA pga;
  ADS1115_Speed speed;
	//转换完成低电平判断引脚  连续转换模式下无效 将使用延时  
	JHAL_IO_Port alert_rdy_Port;
	u8 alert_rdy_Pin;
	//true=单次转换 false=连续转换
	bool isSingleConversion ;
	//采样多次滤波配置 
		JHAL_FilteredMode  filteredModel;
	//采样次数小于等于1时则只返回原数据
		 u8 samplingCount4filter;
}  ADS1115Config;


 
 

 
   bool  ads1115Init( ADS1115Config  *config);
 
//mdianNumbe 中位数个数  
 
  JHAL_ADCInfo ads1115GetVoltage(ADS1115Config  *config ,  ADS1115Channel ch  );



#endif
