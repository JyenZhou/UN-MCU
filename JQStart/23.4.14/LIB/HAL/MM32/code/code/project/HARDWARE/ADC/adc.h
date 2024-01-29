#ifndef __ADCX_H
#define __ADCX_H

#include "main.h"

#define NTC_VC_ADC_Port GPIOA

#define NTC_RT1_ADC_Pin  	GPIO_Pin_15
#define NTC_RT2_ADC_Pin  	GPIO_Pin_7
#define VC_ADC_Pin    		GPIO_Pin_2

#define RS_ADC_Port  			GPIOB

#define RS_GAS1_ADC_Pin   GPIO_Pin_1
#define RS_GAS2_ADC_Pin   GPIO_Pin_0

#define SENSOR_GAS1				ADC_Channel_0
#define	SENSOR_GAS2				ADC_Channel_1
#define	NTC_RT1						ADC_Channel_6
#define	NTC_RT2						ADC_Channel_7
#define VC_VOL						ADC_Channel_5
#define VREF_VOL	     		ADC_Channel_8
#define FIL_NUM			50

typedef struct
{
	uint16_t adc_reg_val;				//寄存器的原始电压
	uint16_t adc_reg_max_val;		//每次采样后链表内的最大值
	uint16_t adc_reg_min_val;		//每次采样后链表内的最小值
	uint16_t adc_fil_val;				//滤波之后的电压（建议使用此电压）
	uint16_t adc_fil_buf[FIL_NUM]; //缓存数组（可理解为循环链表）
	uint32_t adc_fil_sum;					 //用来计算均值的数据累加值，无需关心
}adc_t;

extern adc_t rs_gas1,rs_gas2,ntc_rt1,ntc_rt2,vc_v;

void 			Adc_Init(void);
void 			GetAllChannelValue(void);
uint32_t  adc_task(void);

#endif

