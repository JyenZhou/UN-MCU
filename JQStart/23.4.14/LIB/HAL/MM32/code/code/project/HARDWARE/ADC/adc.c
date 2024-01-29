#include "adc.h"

/*
ADC文件说明：

1、ADCxSampleTimeConfig 设置采样时间，无需关注
2、Adc_Init初始化ADC
3、GetAllChannelValue ADC值获函数，以供adc_task调用，同时获取多通道ADC值
4、adc_task   adc处理滤波函数，用数组模拟一个循环链表，每次执行adc_task，都会循环替换掉链表内的一个采样值
	 每次执行该任务，都会剔除链表内的最大值和最小值后对链表内的数据做均值滤波。
	 该任务的调用频率为100HZ
	 
5、由于做的是均值滤波，未做初始化填充处理，所以adc_task运行的前50次（FIL_NUM定义的链表长度），数组会逐渐上升，即前500ms内的adc数据不可用（重要）
*/

adc_t rs_gas1,rs_gas2,ntc_rt1,ntc_rt2,vc_v;

static void ADCxSampleTimeConfig(ADC_TypeDef* ADCn, ADCSAM_TypeDef sampleTime)
{
    ADCn->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCn->CFGR |= sampleTime;
}

void Adc_Init(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA|RCC_AHBENR_GPIOB, ENABLE);
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin  =  NTC_RT1_ADC_Pin|NTC_RT2_ADC_Pin|VC_ADC_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
		GPIO_Init(NTC_VC_ADC_Port, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin  =  RS_GAS1_ADC_Pin|RS_GAS2_ADC_Pin;
		GPIO_Init(RS_ADC_Port, &GPIO_InitStructure);
	}
	{
		ADC_InitTypeDef  ADC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1ENR_ADC1, ENABLE);
		ADC_StructInit(&ADC_InitStructure);

    ADC_InitStructure.ADC_Resolution 	= ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE 		= ADC_PCLK2_PRESCARE_16;                    //ADC prescale factor
    ADC_InitStructure.ADC_Mode 				= ADC_Mode_Scan;		   											//Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_DataAlign 	= ADC_DataAlign_Right;                      //AD data right-justified
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);
		
		ADCxSampleTimeConfig(ADC1, ADC_Samctl_239_5);
    ADC_Cmd(ADC1, ENABLE);
		
		ADC_ANY_Cmd(ADC1,DISABLE);
		ADC_ANY_NUM_Config(ADC1, 5); 
		ADC_ANY_CH_Config	(ADC1, 0, SENSOR_GAS1);
		ADC_ANY_CH_Config	(ADC1, 1, SENSOR_GAS2);
		ADC_ANY_CH_Config	(ADC1, 2, NTC_RT1);
		ADC_ANY_CH_Config	(ADC1, 3, NTC_RT2);
		ADC_ANY_CH_Config	(ADC1, 4, VC_VOL);
		ADC_ANY_CH_Config	(ADC1, 5, VREF_VOL);
		
		ADC_ANY_Cmd(ADC1,ENABLE);
		
		ADC1->ADCFG |= 0x08;  //开启内部基准源
	}
}

void GetAllChannelValue()
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == 0);
    ADC_ClearFlag(ADC1, ADC_IT_EOC);
	
		rs_gas1.adc_reg_val = ADC1->ADDR0;
		rs_gas2.adc_reg_val = ADC1->ADDR1;
		ntc_rt1.adc_reg_val = ADC1->ADDR7;//使用RT1
		ntc_rt2.adc_reg_val = ADC1->ADDR6;
		vc_v.adc_reg_val		= ADC1->ADDR5;
}

uint32_t  adc_task(void){
static uint8_t fil_count = 0;
_SS
  while(UERROR){
		{
			GetAllChannelValue();
			rs_gas1.adc_fil_buf	[fil_count] = rs_gas1.adc_reg_val;
			rs_gas2.adc_fil_buf	[fil_count]	=	rs_gas2.adc_reg_val;
			ntc_rt1.adc_fil_buf	[fil_count]	=	ntc_rt1.adc_reg_val;
			ntc_rt2.adc_fil_buf	[fil_count]	= ntc_rt2.adc_reg_val;
			vc_v.adc_fil_buf		[fil_count]	=	vc_v.adc_reg_val;
			
			fil_count = (fil_count+1)%FIL_NUM;
			
			rs_gas1.adc_fil_sum 	= 0;
			rs_gas2.adc_fil_sum 	= 0;
			ntc_rt1.adc_fil_sum 	= 0;
			ntc_rt2.adc_fil_sum		= 0;
			vc_v.adc_fil_sum			=	0;
			
			rs_gas1.adc_reg_max_val = rs_gas1.adc_reg_min_val = rs_gas1.adc_fil_buf[0];
			rs_gas2.adc_reg_max_val = rs_gas2.adc_reg_min_val = rs_gas2.adc_fil_buf[0];
			ntc_rt1.adc_reg_max_val = ntc_rt1.adc_reg_min_val = ntc_rt1.adc_fil_buf[0];
			ntc_rt2.adc_reg_max_val = ntc_rt2.adc_reg_min_val = ntc_rt2.adc_fil_buf[0];
			vc_v.adc_reg_max_val	  = vc_v.adc_reg_min_val 		= vc_v.adc_fil_buf[0];
			
			for(uint8_t i=0;i<FIL_NUM;i++)
			{
				rs_gas1.adc_fil_sum	+= rs_gas1.adc_fil_buf[i];
				rs_gas2.adc_fil_sum += rs_gas2.adc_fil_buf[i];
				ntc_rt1.adc_fil_sum += ntc_rt1.adc_fil_buf[i];
				ntc_rt2.adc_fil_sum	+= ntc_rt2.adc_fil_buf[i];
				vc_v.adc_fil_sum		+= vc_v.adc_fil_buf[i];
				
				if(rs_gas1.adc_fil_buf[i] > rs_gas1.adc_reg_max_val)
				{
					rs_gas1.adc_reg_max_val = rs_gas1.adc_fil_buf[i];
				}
				else if(rs_gas1.adc_fil_buf[i] < rs_gas1.adc_reg_min_val)
				{
					rs_gas1.adc_reg_min_val = rs_gas1.adc_fil_buf[i];
				}
				else
				{
					//NULL
				}
				
				if(rs_gas2.adc_fil_buf[i] > rs_gas2.adc_reg_max_val)
				{
					rs_gas2.adc_reg_max_val = rs_gas2.adc_fil_buf[i];
				}
				else if(rs_gas2.adc_fil_buf[i] < rs_gas2.adc_reg_min_val)
				{
					rs_gas2.adc_reg_min_val = rs_gas2.adc_fil_buf[i];
				}
				else
				{
					//NULL
				}
				
				if(ntc_rt1.adc_fil_buf[i] > ntc_rt1.adc_reg_max_val)
				{
					ntc_rt1.adc_reg_max_val = ntc_rt1.adc_fil_buf[i];
				}
				else if(ntc_rt1.adc_fil_buf[i] < ntc_rt1.adc_reg_min_val)
				{
					ntc_rt1.adc_reg_min_val = ntc_rt1.adc_fil_buf[i];
				}
				else
				{
					//NULL
				}
				
				if(ntc_rt2.adc_fil_buf[i] > ntc_rt2.adc_reg_max_val)
				{
					ntc_rt2.adc_reg_max_val = ntc_rt2.adc_fil_buf[i];
				}
				else if(ntc_rt2.adc_fil_buf[i] < ntc_rt2.adc_reg_min_val)
				{
					ntc_rt2.adc_reg_min_val = ntc_rt2.adc_fil_buf[i];
				}
				else
				{
					//NULL
				}
				
				if(vc_v.adc_fil_buf[i] > vc_v.adc_reg_max_val)
				{
					vc_v.adc_reg_max_val = vc_v.adc_fil_buf[i];
				}
				else if(vc_v.adc_fil_buf[i] < vc_v.adc_reg_min_val)
				{
					vc_v.adc_reg_min_val = vc_v.adc_fil_buf[i];
				}
				else
				{
					//NULL
				}
			}
			rs_gas1.adc_fil_val = (rs_gas1.adc_fil_sum - rs_gas1.adc_reg_max_val - rs_gas1.adc_reg_min_val)/(FIL_NUM-2);
			rs_gas2.adc_fil_val	= (rs_gas2.adc_fil_sum - rs_gas2.adc_reg_max_val - rs_gas2.adc_reg_min_val)/(FIL_NUM-2);
			ntc_rt1.adc_fil_val = (ntc_rt1.adc_fil_sum - ntc_rt1.adc_reg_max_val - ntc_rt1.adc_reg_min_val)/(FIL_NUM-2);
			ntc_rt2.adc_fil_val = (ntc_rt2.adc_fil_sum - ntc_rt2.adc_reg_max_val - ntc_rt2.adc_reg_min_val)/(FIL_NUM-2);
			vc_v.adc_fil_val 		= (vc_v.adc_fil_sum - vc_v.adc_reg_max_val - vc_v.adc_reg_min_val)/(FIL_NUM-2);	
			WaitX(10); 
			
		}
  }
_EE
}

