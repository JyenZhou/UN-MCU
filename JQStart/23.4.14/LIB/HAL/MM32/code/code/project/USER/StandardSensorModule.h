#ifndef _StandardSensorModule
#define _StandardSensorModule


#include <stdlib.h>
#include "module.h"

#define  SOFT_VERSION       "HAIER_V1_MM20230713"

#define VS0_FOLLOW_CYCLE_CNT   15 //开机前15sVS0直接跟随
//auto模式的参数定义
#define ERROR_VS_MAX                    1.45f//对应VS为1.45伏，电压过大则材料端异常（断裂，开路等）
#define ERROR_VS_MIN                    0.012f    // 对应VS为0.012伏 20211201 

#define DEGREE_MAX                      99//等级最大值
#define DEGREE_MIN                      1//等级最小值

/* 20211201增加判断换算过的单片机VCC电压值范围的逻辑 手册模组工作电压范围为5±0.2v */
#define ERROR_VCC_MAX                   5.5f//对应VCC最大5.5v伏
#define ERROR_VCC_MIN                   4.2f//对应VCC最小4.2v伏 ，降低下限，防止通过电脑USB取电演示时进入故障模式 20211210
/*
**非演示模式的参数定义
*/
#define START_5MIN_CYCLE   		    	300//开机5分钟灵敏度放大 20220516
#define SENSOR_CNT_PER_CYCLE   			7200//2H 一个小周期
#define SENSOR_CNT_PER_RT      			600
#define VS0_CHANGE_UP_COEF   		    0.05f//基准往上更新调整系数，差值的百分比
#define VS0_CHANGE_DOWN_COEF   	        0.05f//基准往下更新调整系数，差值的百分比
#define QUICK_UP_VALUE                 	5//异味快速上升的数值，当前值与滤波值相比（累计1200个点）
#define WARMINGUP_TIME                  7//预热阶段的累积次数，默认10秒以内
/*
**出厂参数相关
*/
#define USE_RS_CHANGE                   1//使用电阻变化率，默认为电压
#define CUSTOMER_KEEP_RUN_CNT           604800//保持运行7天之后开始记录基准值

#define SENSOR_MIN_LINE_CNT    		   	3000//60分钟开始记录基准下限值
#define VS_MIN_PROTECT_COEF            	0.10f//基准下限保护
//#define DEGREE_GAIN_INDEX               3.4f//在一定区间内灵敏度的增益系数
#define DEGREE_GAIN_INDEX_1             2.3f//区间1内灵敏度的增益系数,85%~70%
#define DEGREE_GAIN_INDEX_2             2.6f//区间2内的灵敏度的增益系数70%~55%
#define RS_GATE_0                   	99//原始变化率百分比的门限值
#define RS_GATE_1                   	85
#define RS_GATE_2                   	65
#define RS_GATE_3                   	55

#define MAP_GATE_1                  	85//需要映射到的变化率百分比的门限值
#define MAP_GATE_2                  	30
#define MAP_GATE_3                  	1

#define realtime_reset_value   			5//异味状态切换的初始值
#define DEGREE_GAIN_INDEX           1.3f//灵敏度的增益系数 20220114
#define START_5MIN_DEGREE_GAIN_INDEX      1.5f//灵敏度的增益系数 202200516

/*
**商场演示模式相关参数定义
*/
#define AFTER_SHOW_CNT_CYCLE            600//演示模式之后更新VS0的周期点数，10min
#define SHOW_VS0_UPDATE_CYCLE           600//演示模式恢复之后更新VS0的周期，10min
#define VS0_FAST_UPDATE_CNT       		6//初始化阶段快速更新VS0 的次数
#define SHOW_MODE_KEEP                  30//演示模式的保持时间
#define AFTER_SHOW_VS0_CNTDOWN          150//约4分钟，演示之后的基准更新周期
#define AFTER_SHOW_CNTDOWN_MAX          2//演示结束之后快速更新基准的次数

#define SHOW_MODE_KEEP_MAX              120//模式下degree最大值保持的时间


#define SPOILED_CNT                     2//触发演示模式的连续计数


              
/*
**bus主板的相关命令下发
*/
#define CALIBRATION_VS0_MIN     		0x15
#define CALIBRATION_VS0    				  0x51
#define customer_change_VS0_MIN     0x55//增加客户端修改VS0_MIN的接口

#define BUS_CTR_LED_ON          		0xFA//bus主板控制led的状态
#define BUS_CTR_LED_OFF         		0xF0
#define O3_OPEN_CMD             		0xFA
#define O3_CLOSE_CMD            		0xF0
#define CLOSE_DOOR_FLAG         		0x00
#define OPEN_DOOR_FLAG          		0x01
#define O3_MODE_CNTDOWN         		1500//臭氧影响倒计时 1450->1500 20211206
#define BASELINE_STORE_FLAG   			0x55//flash中存储了基准的标志
#define ADC_MAX      					  4096

#define FRAME_HEAD    					0xa5
#define MY_DEVICE_ADDR   				0xB4//海尔分配的地址

#define HAIER_DATA_MAX  				10

#define UART0_RX_MAX  					10

/***********************************************

模块切换1.5V和1.8V需要注意的地方


***********************************************/

#define LDO_OUTPUT_VC                   1.5f//加热&测试的电压值，二代改成1.8V 8寸传感器需要用1.8v加热 20230625

#define VS_MIN                        (0.1f*4096/5.0f)//模拟前电压值区间 20230625
#define VS_MAX                        (1.4f*4096/5.0f)
#define VS_SHOW_MIN                   (0.75f*4096/5.0f)//模拟后电压值区间 20230625
#define VS_SHOW_MAX                   (0.95f*4096/5.0f)

//#define RL_PER_VS                       (1.18f*4096/5.0f)    // 选择匹配电阻使用 20230625
#define RL_PER_VS_MAX                   (1.35f*4096/5.0f)    // VS映射条件 20230625
#define RL_PER_VS_MIN                   (0.3f*4096/5.0f)    // VS映射条件 20230625


#define SEND_MAX                       (1.45f*4096/5.0f)//模拟输出最大电压值 20230625

//#define ERROR_VDD_MAX                   5.55f // 对应VS为1.78伏 20230625
//#define ERROR_VDD_MIN                   4.45f    // 对应VS为0.008伏 20230625

//typedef enum 
//{
//empty=0,
//R_6=1, 
//R_33=2
//}RL_E;


typedef struct 
{
	
	unsigned int VC_ADC_VOL;
	unsigned int VL_ADC_VOL;
	unsigned int VS_ADC_VOL;
	unsigned int VNTC_ADC_VOL;
	
  /* mac.zhu-2021/10/09-增加判断加热端和测量端电压值范围的逻辑 */
  float 				VH_VOL;//加热端电压对应的电压值
 

	float NTC_TEMP;
	float NTC_temp_MAX;
	float sys_VDD;
	float NTC_Baseline;
	float VS_vol_float;//传感器两端的电压值，单位：伏
    unsigned char cycle_2H_flag;//已经有了一个小周期
	float   SensorVS0TempFloat;	
	unsigned int  SensorCntPerCycle; //传感器每一个周期的计数，范围：1-7200 初始化为1
    unsigned long customer_run_cnt;//客户端持续运行的次数
	unsigned int  VS0_MIN ;//基准下限
	unsigned int  SensorVS0;

	unsigned int  SensorCntPerRealTime;
	unsigned int  SensorRealTimeAvgValue;
	float   SensorRealTimeAvgValueFloat;
	
	unsigned char degree;
	unsigned char degree_inv;

	unsigned char has_filter_reset;
	unsigned char has_baselineMIN_flag;//基准下限标志
	unsigned char has_baselineTEMP_flag;//临时基准标志
	unsigned char has_baseline_real_flag;//基准标志
	
	unsigned char cleanup_flag;//回到清新的标志
	unsigned char has_after_show_flag;//经过一个演示模式
	unsigned char after_show_cntdown;//经过演示之后VS0快速更新的次数
	unsigned char heat_stop_flag;//暂停加热的标志
	unsigned char heat_stop_cntdown;//暂停加热倒计时
	unsigned char O3_MODE_PROTERCT;//臭氧影响期间屏蔽VS0更新
	unsigned char have_after_o3_mode_flag;//每经过一次臭氧开启都会置位
	unsigned char SERIAL_DATA[17];
	//演示模式之后的vs累加
	unsigned int  after_show_vs_avg_cnt;//演示之后VS低于VS0的累积均值的次数

	unsigned char after_show_vs_big_cnt;//演示之后VS高于于VS0的累积均值的次数

	unsigned char after_show_vs_reback_flag;//演示之后VS开始恢复并高于VS0的标志
	float  show_vs_avg_float;//演示之后VS的累积均值，最大累积点600
	unsigned int  BANDGAP_UID_VOL;//uid
	float        bandgap_vol;//基准值
	unsigned int BANDGAP_ADC_VOL;//ADC基准值
	unsigned char VS0_follow_cnt; //开机15sVS0跟随
 	unsigned int Start_5min_cnt; //开机5分钟等级放大计时器 20220516
	  float Ratio;                 //系数值，需要存储  20230625
}SensorModule;

typedef struct{
  uint8_t frame_head1;//帧头
  uint8_t frame_head2;//帧头
  uint8_t frame_length;//帧长
  uint8_t device_addr;//地址
  uint8_t cmd_bit;//类型
  uint8_t function;//功能
  uint8_t degree;//等级
  uint8_t error_flag;//传感器情况
  uint8_t temp_signal;//VS高位
  uint8_t temp_value;//VS高位
  uint8_t reserved1;//预留
  uint8_t vs1_high;//VS0高位
  uint8_t vs1_low;//VS0高位
  uint8_t VS0_MIN_H;//VS0_MIN高位
  uint8_t VS0_MIN_L;//VS0_MIN低位
  uint8_t reserved4;
  uint8_t reserved5;
  uint8_t crc;
}Haier_bus_dat_T;//定义传输给BUS总线的数据

uint8_t lincProcessHaierDat(uint8_t * pdat);

void haierBusDataInit(void);

uint8_t getCrcResult(uint8_t*pdat,uint16_t size);
void clearReceive_UART0(void);

void packetHaierBusData(SensorModule* sm);


void StandardSensorModuleInit(SensorModule* sm);

uint8_t dataXorCRC(uint8_t * sd,uint8_t length);
void uartDataSend(uint8_t* dat,uint8_t length);
void GetVSBaselineFromEE(SensorModule* sm);
void SetVSBaselineToEE(SensorModule* sm);

void packetHmnstUartData(SensorModule* sm);


void SensorRealTimeDataHandle(SensorModule* sm);
void update_VS0(SensorModule* sm);

void getRS_Degree(SensorModule* sm);
void sensorAutoMode(SensorModule* sm);

void show_time_vs0_update(SensorModule* sm);

void after_show_VS_filter(SensorModule* sm);
void processBusCmd(void);
void Flash_int(void);
void processO3Mode(SensorModule* sm);
uint8_t mappingDegree(uint8_t degree);
void updateVsData(void);
uint32_t send_data_task(void);
uint32_t  calculationTask(void) ;
void paramDataInit(void);


#endif
