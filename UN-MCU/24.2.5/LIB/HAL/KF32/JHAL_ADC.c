#include "../JHAL_ADC.h"

 //BandGap最大、小AD值
const int SYS_BandGapAd_Min = 1750, SYS_BandGapAd_Max = 1950;
/**  基准电压，单位：MV */
const float ADC_BANDGAP_VOLTAGE	=	1.5f  ;
void JHAL_adchannelGPIOCModeInit(u8 channel)
{
#if defined(KF32A151MQV)



#elif defined (KF32A140INP)

  switch(channel)
  {
  case ADC_CHANNEL_7:
  GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_7,GPIO_MODE_AN);
  break;
  case ADC_CHANNEL_8:
   GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_8,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_9:
    GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_9,GPIO_MODE_AN);
    break;
  case ADC_CHANNEL_13:
   GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_13,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_36:
   GPIO_Write_Mode_Bits(GPIOE_SFR,GPIO_PIN_MASK_15,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_37:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_0,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_38:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_1,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_39:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_2,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_40:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_3,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_41:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_5,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_42:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_6,GPIO_MODE_AN);
   break;
   //53是内部基准通道
  case ADC_CHANNEL_53:
	  break;
  default:
    //不存在的ADC
    while(1);

  }


#else
  需要新增相关型号引脚映射
#endif



}



/*MSP相关初始化*/
void JHAL_adcMSPInit(u8* channel,u8 number)
{
for(u8 i=0;i<number;i++)
  {
	JHAL_adchannelGPIOCModeInit(channel[i]);
  }



}

/*例程中的延时方法--仅用于ADC初始化*/
void JHAL_adcInitDelay(volatile uint32_t cnt) {
        while (cnt--)
                ;
}


void  _JHAL_adcSingleModeInit(u8* channel,u8 number)
{



        ADC_InitTypeDef adcStruct;

        ADC_Reset (ADC0_SFR); //复位ADC外设并打开ADC的时钟

        /* 初始化 ADC时钟源选择 */
        adcStruct.m_Clock = ADC_HFCLK;
        /* 初始化 ADC时钟分频 */
        adcStruct.m_ClockDiv = ADC_CLK_DIV_32; //选择32分频
        /* 初始化 ADC扫描模式使能 */
        adcStruct.m_ScanMode = FALSE;  //不使用扫描模式，一次触发只转换第一个通道
        /* 初始化 ADC连续转换模式 */
        adcStruct.m_ContinuousMode = ADC_SINGLE_MODE; //不使用连续转换功能，只对设置的通道采样一遍
        /* 初始化 ADC转换结果输出格式 */
        adcStruct.m_DataAlign = ADC_DATAALIGN_RIGHT; //转换的结果右对齐
        /* 初始化 ADC常规通道外部触发转换模式使能 */
        adcStruct.m_ExternalTrig_EN = FALSE; //常规通道失能外部条件触发ADC转换，
        /* 初始化 ADC常规通道外部触发事件 */
        adcStruct.m_ExternalTrig = ADC_EXTERNALTRIG_T1TRGO; //常规通道外部触发信号源配置为T1TRGO
        /* 初始化 ADC高优先级通道外部触发转换模式使能 */
        adcStruct.m_HPExternalTrig_EN = FALSE; //高优先级通道失能外部条件触发ADC转换，
        /* 初始化 高优先级通道外部触发事件 */
        adcStruct.m_HPExternalTrig = ADC_HPEXTERNALTRIG_CCP1_CH1; //高优先级通道触发信号源配置为CCP1的通道1
        /* 参考电压选择，取值为宏“ADC参考电压选择”中的一个。 */
        adcStruct.m_VoltageRef=ADC_REF_AVDD; //转换的参考电压源配置为VDDA
        /* 初始化 ADC常规通道扫描长度 */
        adcStruct.m_NumOfConv = 1; //扫描的长度配置为1个通道
        /* 初始化 ADC高优先级通道扫描长度 */
        adcStruct.m_NumOfHPConv = 1; //高优先级通道的扫描长度
        ADC_Configuration (ADC0_SFR, &adcStruct);

        ADC_Cmd (ADC0_SFR, TRUE); //ADC使能

        JHAL_adcInitDelay(0xFFF);  //ADC使能后需要一段短暂的时间，等待电路充电完毕后，才能去采样，否者可能存在较大的采样误差

        JHAL_adcMSPInit(channel,number);

}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 单通道单次采集模式初始化
 * @输入参数:
isEN  true初始化  false反初始化
channel 对应通道的数组
number  要初始化通道的个数
 * @返 回 值: 无
 * @备注:
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/
void  JHAL_adcSingleModeInit(bool  isEN,u8* channel,u8 number)
{


 static bool sEN= {false};
 if(sEN!=isEN)
 {
     sEN=isEN;

     if(isEN)
     {
    		//参考电压配置
    	    PM_VREF_Software_Enable(TRUE);
    		PM_VREF_SELECT(PM_VREF_VOLTAGE_1P5V);
    	    ADC_Reference_Voltage_Config(ADC0_SFR,ADC_REF_INTERNAL);
    	 _JHAL_adcSingleModeInit( channel, number);
     } else {

        //TODO  ADC_Reset中有提到复位时钟 理论可以关闭对应时钟
     }
 }

}







//单通道采集
u32  JHAL_adcAqcSingle(u8 channel)

{
  //原型参考 ADC_Regular_Channel_Config (ADC0_SFR, ADC_CHANNEL_2, 0x01); //ADC0常规通道1的采样源设置为ADC_CHANNEL_2
  ADC_Regular_Channel_Config (ADC0_SFR, channel, 0x01); //ADC0常规通道1的采样源设置为ADC_CHANNEL_1
  ADC_Software_Start_Conv(ADC0_SFR);              //软件触发转换
  while(!ADC_Get_INT_Flag(ADC0_SFR,ADC_INT_EOC)); //等待转换结束，
  return ADC_Get_Conversion_Value(ADC0_SFR); //读取ADC数据寄存器,同时硬件会自动清零EOCIF

}

