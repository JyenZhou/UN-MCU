#ifndef __SSM_H
#define __SSM_H

#include "main.h"

//#define CMFA103J3500HANT 	1
//#define TIMER_MAX_VALUE		1000000		//100W秒
//#define INIF_TIME					3	//初始化时间3秒
//#define	MATCHED_RES				20000
//#define	SAMPLING_RES			0.1f
//#define	NTC_MATCHED_RES		10000
//#define	DAC_MAX_OUT				5.0f
//#define SS_W_MIN_CNT			3000//50分钟开始记录基准下限值
//#define SS_W_MIN_COEF   	0.7f//基准下限保护
//#define	SEVEN_DAY					604800//保持运行7天之后开始记录基准值
//#define	TWO_HOURS					7200//2H 一个小周期
//#define UP_COEF   		    0.05f//基准往上更新调整系数，差值的百分比
//#define DOWN_COEF   	    0.05f//基准往下更新调整系数，差值的百分比
//#define FIRST_FIL_COEF		7200//一阶滤波系数，改变此值可改变基准跟随速度
//#define HEAT_UP_TIME			10	//传感器加热时间
//#define SEND_BUFF_LEN			16	//发送数据包长度
#define SYSTEM_VOLTAGE_VALUE		5.0f	//系统工作电压 5v 20211123
#define CMFA103J3500HANT 	1				
#define TIMER_MAX_VALUE		1000000	//100W秒
#define INIF_TIME					3				//初始化时间3秒
#define	MATCHED_RES				20000		//测量端匹配电阻阻值（欧姆）
#define	SAMPLING_RES			1.0f		//采样电阻阻值（欧姆）
#define	NTC_MATCHED_RES		10000		//NTC的匹配电阻阻值（欧姆）
#define	DAC_MAX_OUT				5.0f		//DAC最大输出电压
#define SS_W_MIN_CNT			3000		//50分钟开始记录基准下限值
#define SS_W_MIN_COEF   	0.5f		//基准下限保护
#define	SEVEN_DAY					604800	//保持运行7天之后开始记录基准值
#define	TWO_HOURS					7200		//2H 一个小周期
#define UP_COEF   		    0.05f		//基准往上更新调整系数，差值的百分比
#define DOWN_COEF   	    0.05f		//基准往下更新调整系数，差值的百分比
#define FIRST_FIL_COEF		1000		//一阶滤波系数，改变此值可改变基准跟随速度
#define HEAT_UP_TIME			8				//传感器加热时间
#define SEND_BUFF_LEN			16			//发送数据包长度

#define	AVER_RES_COEF			0.99998f		//触发后的平均值随时间递减
#define	TRIGGER_COEF			0.9f		//触发阈值
#define	TRIGGER_DELAY			30			//触发后转为慢速滤波的延迟时间
#define PROTECT_TEMP			50.0f		//保护温度
#define	HEAT_RES_MAX			100			//加热器最大电阻
#define	HEAT_RES_MIN			40   		//加热器最小电阻

#define CHECK_DELAY				5				//检测输入脚的状态，延迟五秒

typedef	enum {NORM_STA = 0, OZONE_STA,UART_STA}check_sta_t;

typedef struct{
	
	float 		vref_vol;				//基准电压 需要初始化填入
	float 		input_vol;  		//单片机电压 由vref解算得到（NTC 需和单片机使用相同电源）
	float 		heat_vol;				//加热电压 由vref解算得到
	float 		measuring_vol;	//测量电压 由vref解算得到
	uint32_t 	sensor_res_gas1;//传感器电压
	uint32_t 	sensor_res_gas2;//传感器电压
	uint32_t 	heat_res;				//加热电阻
	uint32_t 	ntc_rt1_div_res;//NTC电阻（欧姆）
	uint32_t 	ntc_rt2_div_res;//NTC电阻（欧姆）
	uint32_t    sensor_vl_adc;//传感器匹配电阻两端电压对应的ADC
    uint32_t    sensor_vs_adc;//传感器电压对应的ADC
    uint32_t    sensor_vc_adc;//测量回路电压对应的ADC
	float		 	ntc_rt1_temp;					//NTC测得的温度
	float		 	ntc_rt2_temp;					//NTC测得的温度
	float       sensor_vs_float;//传感器两端的电压值，单位：伏
	uint32_t	sensor_zero_res;			//基准线
	uint32_t	sensor_zero_res_temp;	//计算基准线
	uint32_t	sensor_zero_res_min;	//出厂时设置的最低基准线
	
	uint8_t 	degree_num;						//输出等级值
	float			degree_vol;						//输出等级电压值
	uint32_t      vol_3v3_adc;//3.3伏所对应的ADC
	uint64_t  run_timer;	//系统运行时间（秒）
	
}sys_info_t;

extern  sys_info_t sys_info;

typedef struct{
	
	float 	aver_res;
	uint8_t	flash_value[8];

}debug_info_t;

extern  debug_info_t debug_info;

uint32_t  vol_res_task(void);

uint32_t  get_degree_task(void);
//uint32_t  send_data_task(void);
uint32_t 	receive_data_task(void);
void 			GetSSParaFormEE(void);
void 			SetSSParaFromEE(void);

#endif
