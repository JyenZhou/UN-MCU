/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADC默认通用HAL驱动
  使用ADC0 若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_ADC__H__
#define  __JHAL_ADC__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        JHAL_ADC0=0,
        JHAL_ADC_Number
    } JHAL_ADC;


    typedef struct
    {
        uint32_t maxAD;
        uint32_t minAD;
        uint32_t middleAD;
    } JHAL_ADCValue;
    typedef struct
    {
        float maxVoltage;
        float minVoltage;
        float middleVoltage;
        JHAL_ADCValue adcValue;
    } JHAL_ADCInfo;

//映射的基准源
    typedef enum {
        JHAL_ADC_ReferVoltage_BandGap,
        JHAL_ADC_ReferVoltage_VDD,
        JHAL_ADC_ReferVoltage_Vref,
    }
    JHAL_ADC_ReferSoure;
    /*ADC触发源*/
    typedef enum {
        JHAL_ADCTriggerSoure_Software=0,
        JHAL_ADC_ReferVoltage_PWM,
    } JHAL_ADCTriggerSoure;


    typedef struct
    {   //电压系数计算
        float calculationCoefficient;
    } __JHAL_ADCOtherInfo;
#ifdef  	XL6600A402L6
    // ADC_CHANNEL_TEMPSENSOR;    /*!< ADC输入通道内部温度传感器 FF*/
#define JHAL_ADC_CHANNEL_TEMPSENSOR 0x16
    //ADC_CHANNEL_BANDGAP ;  /*!< ADC采样基准电压 */
#define JHAL_ADC_CHANNEL_BANDGAP       0x17
//ADC_CHANNEL_VSS  ;    /*!< ADC输入通道VSS */
#define JHAL_ADC_CHANNEL_VSS           0x1a
//     		ADC_CHANNEL_VREFH   ;    /*!< ADC输入通道Vrefh */
#define JHAL_ADC_CHANNEL_VREFH  0x1d
//		ADC_CHANNEL_VREFL;    /*!< ADC输入通道Vrefl */
#define JHAL_ADC_CHANNEL_VREFL      0x1e
#define JHAL_ADC_CHANNEL_VREF      JHAL_ADC_CHANNEL_VREFH
#endif



    typedef struct
    {
//是否是多通道模式
        bool isMultichannelMode;
//多通道模式下AD接收缓冲区
        JHAL_ADCInfo* adcInfosBuff;

//adc通道数组中的值0-0xff 带隙基准及温度等常见内部通道用 JHAL_ADC_CHANNEL_XXX 表示
        u8* channels;
        //通道数量
        u8 channelsNumber;
        JHAL_ADC_ReferSoure vref;
//基准源满量程电压值
        float vrefVoltageValue;
        JHAL_ADCTriggerSoure triggerSoure ;
//这里是泛型传进来的与JHAL外设枚举一致 使用软件时无须配置该值
        u8 triggerSoureValue;

        __JHAL_ADCOtherInfo __otherInfo;
    } JHAL_ADCConfig;



    /*单通道单次采集模式初始化
    isEN  true初始化  false反初始化
    channel 对应通道的数组
    length  要初始化通道的个数
    JHAL_ADC_ReferVoltage 使用的参考电压
    vrefValue 参考电压的值
    */
    bool  JHAL_adcInit(JHAL_ADC adc, JHAL_ADCConfig *config);
//单通道单次采集  返回值ADC码值
    JHAL_ADCInfo  JHAL_adcAqcSingle(JHAL_ADC adc,u8 channel);
//判断多通道转化是否完成 完成则接收到缓冲区并开启下次转化  实际接收数据在初始化的 dataBuff
    bool  JHAL_adcAqcMultiple (JHAL_ADC adc);

    bool  JHAL_adcDeInit(JHAL_ADC adc);

#ifdef CplusPlus
}
#endif

#endif








