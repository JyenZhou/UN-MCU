/*******-------------------------------------------------------------------------------------------
--获取到的VS,VC值经过处理输出degree值，包含了调理策略
*/

#include "StandardSensorModule.h"
#include "module.h"
/*硬件版本*/
#define HVER_H       5 
#define HVER_L       6
#define HVER_YEARS_H 20
#define HVER_YEARS_L 20
#define HVER_MONTH   5
#define HVER_DAY     5
/*软件版本*/
#define SVER_H       11 
#define SVER_L       28
#define SVER_YEARS_H 20
#define SVER_YEARS_L 22
#define SVER_MONTH   05
#define SVER_DAY     16

SensorModule sm;
volatile uint8_t uart_send_lock_flag;
Haier_bus_dat_T haier_bus_dat;
//base_line_t vs0_baseline_value_t;//记录基准值

//extern uint8_t HAIER_BUF[HAIER_DATA_MAX];


//volatile uint8_t not_get_bus_dat_flag;

volatile uint16_t sensor_vs_low_cnt=0;//传感器电压极低计数
volatile uint8_t sensor_break_flag=0;//传感器损坏标志
volatile uint8_t warmingup_ok_flag=0;//单片机上电30分钟预热ok
volatile uint8_t auto_degree_start_fag=0;//开启自动等级标志
volatile uint8_t auto_mode_start_cnt=0;
volatile uint32_t sensor_vs_constant_time=0;

volatile uint8_t insert=0;
volatile float ntc_max,ntc_min;
volatile uint32_t vs_no_change_cnt=0;
volatile uint8_t delay_10=0;
volatile uint8_t haier_cmd;
volatile uint8_t demarcate[10]={0XA5,0XA5,10,0XB4,0,0xB0,0,0,0,0};
volatile uint8_t version_number[43]={0XA5,0XA5,43,0XB4,0,0XA1,0X31,0X30,0X30,0X30,0X36,0X34,0X30,0X30,
0X32,0X37,0X35,0X30,0X5F,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,HVER_H,HVER_L,SVER_H,SVER_L,0,0,
HVER_YEARS_H,HVER_YEARS_L,SVER_YEARS_H,SVER_YEARS_L,HVER_MONTH,SVER_MONTH,HVER_DAY,SVER_DAY,0
	};//29.30:硬件版本号；31.32：软件版本号33.34：预留；35.36：硬件年份；37.38软件年份；
	  //39硬件月份；40软件月份；41硬件日；42软件日；43CRC
//硬件版本5.6 20200505，软件版本11.28 20211206	
volatile uint8_t calibration_flag=0;
	volatile uint8_t led_key = BUS_CTR_LED_OFF;	
volatile uint8_t open_the_door=CLOSE_DOOR_FLAG;//冰箱是否开门
volatile  	float coef;
volatile uint8_t O3_open_flag=O3_CLOSE_CMD;//负离子开启标志
	
volatile uint16_t vs_avg_120= 0;//120s内VS 20230625
volatile uint32_t VS_120_accumulative=0;//120S的VS均值20230625
volatile uint16_t VS_Accumulative_cnt=0;//VS累加数20230625
volatile  unsigned char show_flag=0;//演示模式的标志
#if 1
  const uint8_t crc8_table[256] = {
  0X00, 0X65, 0XCA, 0XAF, 0XF1, 0X94, 0X3B, 0X5E, 0X87, 0XE2, 0X4D, 0X28, 0X76, 0X13, 0XBC, 0XD9,//16
  0X6B, 0X0E, 0XA1, 0XC4, 0X9A, 0XFF, 0X50, 0X35, 0XEC, 0X89, 0X26, 0X43, 0X1D, 0X78, 0XD7, 0XB2,//32
  0XD6, 0XB3, 0X1C, 0X79, 0X27, 0X42, 0XED, 0X88, 0X51, 0X34, 0X9B, 0XFE, 0XA0, 0XC5, 0X6A, 0X0F,//48
  0XBD, 0XD8, 0X77, 0X12, 0X4C, 0X29, 0X86, 0XE3, 0X3A, 0X5F, 0XF0, 0X95, 0XCB, 0XAE, 0X01, 0X64,//64
  0XC9, 0XAC, 0X03, 0X66, 0X38, 0X5D, 0XF2, 0X97, 0X4E, 0X2B, 0X84, 0XE1, 0XBF, 0XDA, 0X75, 0X10,//80
  0XA2, 0XC7, 0X68, 0X0D, 0X53, 0X36, 0X99, 0XFC, 0X25, 0X40, 0XEF, 0X8A, 0XD4, 0XB1, 0X1E, 0X7B,//96
  0X1F, 0X7A, 0XD5, 0XB0, 0XEE, 0X8B, 0X24, 0X41, 0X98, 0XFD, 0X52, 0X37, 0X69, 0X0C, 0XA3, 0XC6,//112
  0X74, 0X11, 0XBE, 0XDB, 0X85, 0XE0, 0X4F, 0X2A, 0XF3, 0X96, 0X39, 0X5C, 0X02, 0X67, 0XC8, 0XAD,//128
  0XF7, 0X92, 0X3D, 0X58, 0X06, 0X63, 0XCC, 0XA9, 0X70, 0X15, 0XBA, 0XDF, 0X81, 0XE4, 0X4B, 0X2E,
  0X9C, 0XF9, 0X56, 0X33, 0X6D, 0X08, 0XA7, 0XC2, 0X1B, 0X7E, 0XD1, 0XB4, 0XEA, 0X8F, 0X20, 0X45,
  0X21, 0X44, 0XEB, 0X8E, 0XD0, 0XB5, 0X1A, 0X7F, 0XA6, 0XC3, 0X6C, 0X09, 0X57, 0X32, 0X9D, 0XF8,
  0X4A, 0X2F, 0X80, 0XE5, 0XBB, 0XDE, 0X71, 0X14, 0XCD, 0XA8, 0X07, 0X62, 0X3C, 0X59, 0XF6, 0X93,
  0X3E, 0X5B, 0XF4, 0X91, 0XCF, 0XAA, 0X05, 0X60, 0XB9, 0XDC, 0X73, 0X16, 0X48, 0X2D, 0X82, 0XE7,
  0X55, 0X30, 0X9F, 0XFA, 0XA4, 0XC1, 0X6E, 0X0B, 0XD2, 0XB7, 0X18, 0X7D, 0X23, 0X46, 0XE9, 0X8C,
  0XE8, 0X8D, 0X22, 0X47, 0X19, 0X7C, 0XD3, 0XB6, 0X6F, 0X0A, 0XA5, 0XC0, 0X9E, 0XFB, 0X54, 0X31,
  0X83, 0XE6, 0X49, 0X2C, 0X72, 0X17, 0XB8, 0XDD, 0X04, 0X61, 0XCE, 0XAB, 0XF5, 0X90, 0X3F, 0X5A
  };
#endif  


//unsigned int status_hal = 0;

//uint8_t have_no_alloc_addr;//还没有收到bus分配的地址
//volatile uint8_t get_haier_dat;

extern volatile uint8_t uart_send_lock_flag;

uint16_t show_keep_cnt;

/* USER CODE BEGIN 4 */

#if 1
/*
**将发给海尔BUS总线的数据打包
*/
void packetHaierBusData(SensorModule* sm)
{
	float tmpdat_he;//处理输出数值 20230625
	uint8_t crc_result=0xff;//
	float degree;	
	float index=1.0f;
	unsigned int temp_vs;
	unsigned int temp_vs0;
	  if(sm->has_baselineMIN_flag)    //是否经过VS_min标定 20230625
		{
		  tmpdat_he = sm->VS_ADC_VOL;
			tmpdat_he /= sm->Ratio;
			if(tmpdat_he>=SEND_MAX)
			{
				tmpdat_he=SEND_MAX;
			}
		  temp_vs = (uint16_t)tmpdat_he / 256;      //实时电压
			temp_vs = (uint16_t)tmpdat_he % 256;

	    tmpdat_he = sm->SensorVS0;
			tmpdat_he /= sm->Ratio;
			if(tmpdat_he>=SEND_MAX)
			{
				tmpdat_he=SEND_MAX;
			}
			temp_vs0 = (uint16_t)tmpdat_he / 256;        //  需要上传的基准
			temp_vs0 = (uint16_t)tmpdat_he % 256;
			
			tmpdat_he = sm->VS0_MIN;
			tmpdat_he /= sm->Ratio;
			if(tmpdat_he>=SEND_MAX)
			{
				tmpdat_he=SEND_MAX;
			}
			haier_bus_dat.VS0_MIN_H = (uint16_t)tmpdat_he / 256;         //   VS0_MIN
			haier_bus_dat.VS0_MIN_H = (uint16_t)tmpdat_he % 256;
		}
		else{
//	unsigned int temp_vs_avg;
			temp_vs=(float)(sm->VS_ADC_VOL)/index;
			temp_vs0=(float)(sm->SensorVS0)/index;
			haier_bus_dat.VS0_MIN_H=sm->VS0_MIN / 256;
			haier_bus_dat.VS0_MIN_L=sm->VS0_MIN % 256;
		}
//	temp_vs_avg=(float)sm->SensorRealTimeAvgValue/index;

	if(uart_send_lock_flag){return;}
//	degree=mappingDegree(sm->degree_inv);
	degree=sm->degree_inv;// 0对应最洁净
	haier_bus_dat.degree=degree;
	
	haier_bus_dat.temp_signal=temp_vs/256;
	haier_bus_dat.temp_value=temp_vs%256;

	haier_bus_dat.vs1_high=temp_vs0/256;
	haier_bus_dat.vs1_low=temp_vs0%256;
	haier_bus_dat.reserved1=sm->cleanup_flag;

	crc_result=getCrcResult(&(haier_bus_dat.frame_length),15);
	haier_bus_dat.crc=crc_result;

}
//初始化存储到flash中的参数
void Flash_int(void)
{
		//uint16_t i;
		uint8_t baseline_value[8]= {0};
		baseline_value[0]=0xFF;
		baseline_value[1]=0xFF;
		baseline_value[2]=0xFF;
		baseline_value[3]=0xFF;
		baseline_value[4]=0xFF;
		baseline_value[5]=0xFF;
		baseline_value[6]=0xFF;
		baseline_value[7]=0xFF;
		EEPROM_Write(baseline_value, 8);
	
}

/*
**海尔bus总线的参数初始化
*/
void haierBusDataInit(void)
{
	haier_bus_dat.frame_head1=FRAME_HEAD;
	haier_bus_dat.frame_head2=FRAME_HEAD;
	haier_bus_dat.frame_length=18;
	haier_bus_dat.device_addr=MY_DEVICE_ADDR;
	haier_bus_dat.cmd_bit=0x00;
	haier_bus_dat.function=0xA0;
	haier_bus_dat.degree=50;
	haier_bus_dat.error_flag=0x00;
	haier_bus_dat.temp_signal=0;
	haier_bus_dat.temp_value=0;
	haier_bus_dat.vs1_high=0;
	haier_bus_dat.vs1_low=0;
	haier_bus_dat.reserved1=0;
	haier_bus_dat.VS0_MIN_H=0;
	haier_bus_dat.VS0_MIN_L=0;
	haier_bus_dat.reserved4=0;
	haier_bus_dat.reserved5=0;
}
/*
**将传感器VS,VC,温度等参数传进运算的结构体
**
*/
void updateVsData(void)
{
    sm.VC_ADC_VOL=sys_info.sensor_vc_adc;//更新测量电压的ADC
    sm.VL_ADC_VOL=sys_info.sensor_vl_adc;//更新匹配电阻电压的ADC
    sm.VS_ADC_VOL=sys_info.sensor_vs_adc;//更新传感器两端电压的ADC
    sm.VS_vol_float=sys_info.sensor_vs_float;//更新传感器两端的电压值，float类型，单位：伏
    sm.NTC_TEMP=sys_info.ntc_rt1_temp;//更新温度值
	
    
}

uint8_t lincProcessHaierDat(uint8_t * pdat)
{
uint8_t fcs;
	if((pdat[0]!=FRAME_HEAD)||(pdat[1]!=FRAME_HEAD))return 1;
    fcs=getCrcResult((uint8_t*)&pdat[2],HAIER_DATA_MAX-3);
	haier_cmd=pdat[5];
	
	if(fcs==pdat[HAIER_DATA_MAX-1])
	{		
		switch(haier_cmd){
		case 0xA0:
			open_the_door=pdat[6];
			led_key=pdat[7];
			O3_open_flag=pdat[8];
			if(led_key==BUS_CTR_LED_ON)
			{
				led_sys.led_sta=LED_FLASH_1HZ;
			}
			else if(led_key==BUS_CTR_LED_OFF)
			{
				led_sys.led_sta=LED_OFF;
			}
//			haier_bus_dat.reserved1=pdat[6];
			haier_bus_dat.reserved4=pdat[6];
			haier_bus_dat.reserved5=pdat[8];
			haier_bus_dat.crc=getCrcResult((uint8_t*)&haier_bus_dat.frame_length,15);
			uartDataSend((uint8_t*)&haier_bus_dat,sizeof(haier_bus_dat));
			break;
		case 0xB0:

			calibration_flag=pdat[6];
//		led_key=pdat[7];
			demarcate[0]=FRAME_HEAD;
			demarcate[1]=FRAME_HEAD;
			demarcate[2]=10;
			demarcate[3]=MY_DEVICE_ADDR;
			demarcate[4]=0;
			demarcate[5]=0xB0;
			demarcate[6]=calibration_flag;//获取标定VS0信号
			demarcate[7]=0;
			demarcate[8]=0;
			demarcate[9]=getCrcResult((uint8_t*)&demarcate[2],7);
			uartDataSend((uint8_t*)&demarcate[0],10);
			break;
		case 0xA1:
			version_number[42]=getCrcResult((uint8_t*)&version_number[2],40);
			uartDataSend((uint8_t*)&version_number[0],43);
			break;
		default:
			break;
		}
		
	}
	return 0;
}


/*
**获取海尔的数据校验值
*/
uint8_t getCrcResult(uint8_t*pdat,uint16_t len)
{
uint16_t count;
uint8_t CRC_Result=0xff; 
	for(count = 0; count < len; count++)
	{	
		CRC_Result = crc8_table[CRC_Result ^ pdat[count]];
	}
	return CRC_Result;
}
#endif
void StandardSensorModuleInit(SensorModule* sm)
{
	memset(sm,0,sizeof(SensorModule));
    sm->SensorCntPerCycle = 1;
    sm->SensorCntPerRealTime = 1;
    sm->degree = DEGREE_MAX;
    sm->degree_inv = DEGREE_MIN;	
	haierBusDataInit();

}
/*
**数据初始化，计数周期，等级值等参数初始化
*/

void paramDataInit(void)
{
    StandardSensorModuleInit(&sm);
    GetVSBaselineFromEE(&sm);

}

//写入基准值 掉电可以保护
void SetVSBaselineToEE(SensorModule* sm)
{
//uint8_t i=0;
    uint8_t baseline_value[16]= {0};
	  uint32_t K=0;

    unsigned char flag = BASELINE_STORE_FLAG;

    if(1==sm->has_baselineMIN_flag) {
        baseline_value[0]=flag;
        baseline_value[1]=sm->VS0_MIN/256;
        baseline_value[2]=sm->VS0_MIN%256;
			  K=*(uint32_t*)&sm->Ratio; //存储输出VS补偿系数 20230625
			  baseline_value[6]=K;
        baseline_value[7]=K>>8;
        baseline_value[8]=K>>16;
			  baseline_value[9]=K>>24;
    }
    if(1==sm->has_baselineTEMP_flag) {
        baseline_value[3]=flag;
        baseline_value[4]=sm->SensorVS0/256;
        baseline_value[5]=sm->SensorVS0%256;
    }

    EEPROM_Write(baseline_value, 16);

}

//获取存储的基准值
void GetVSBaselineFromEE(SensorModule* sm)
{
    uint8_t baseline_value[16]= {0};
     uint32_t K=0;
    EEPROM_Read(baseline_value,16);
//    dat1=baseline_value[0];

//	if(BASELINE_STORE_FLAG==baseline_value[3]){
//		sm->SensorVS0=baseline_value[4]*256+baseline_value[5];
//		sm->VS0_MIN=baseline_value[1]*256+baseline_value[2];
//		sm->has_baselineTEMP_flag=1;
//		sm->SensorVS0TempFloat=sm->SensorVS0;
//		sm->SensorRealTimeAvgValueFloat=sm->SensorVS0;
//		sm->cycle_2H_flag=1;
//	  	sm->has_baselineMIN_flag=1;
//		sm->SensorCntPerRealTime = realtime_reset_value;
//		sm->customer_run_cnt=CUSTOMER_KEEP_RUN_CNT;
//	}else 
	if(BASELINE_STORE_FLAG==baseline_value[0]) 
	{
        sm->has_baselineMIN_flag=1;
        sm->VS0_MIN=baseline_value[1]*256+baseline_value[2];
        sm->SensorVS0=sm->VS0_MIN;
        sm->SensorVS0TempFloat=sm->VS0_MIN;
		    sm->SensorVS0=sm->VS0_MIN;
				K=baseline_value[6]|baseline_value[7]<<8|baseline_value[8]<<16|baseline_value[9]<<24; //获取输出VS补偿系数 20230625
	    	sm->Ratio=*(float*)&K;
				sm->SensorVS0TempFloat=sm->VS0_MIN;
				
    }
	
}


void uartDataSend(uint8_t* dat,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
		Send_Data_To_UART0(dat[i]);
	}
}

void packetHmnstUartData(SensorModule* sm)
{
  uint8_t temp_degree;		
	float tmpdat;//处理输出数值 20230625
	sm->SERIAL_DATA[0] = 0xff;
//	temp_degree=mappingDegree(sm->degree_inv);
	temp_degree=sm->degree_inv;
	sm->SERIAL_DATA[1] = temp_degree; //映射后的等级
	 if(sm->has_baselineMIN_flag)//VS_min是否经过标定 20230625
		{
		  tmpdat = sm->VS_ADC_VOL;
			tmpdat /= sm->Ratio;
			if(tmpdat>=SEND_MAX)
			{
				tmpdat=SEND_MAX;
			}
		  sm->SERIAL_DATA[2] = (uint16_t)tmpdat / 256;      //实时电压
			sm->SERIAL_DATA[3] = (uint16_t)tmpdat % 256;

	    tmpdat = sm->SensorVS0;
			tmpdat /= sm->Ratio;
			if(tmpdat>=SEND_MAX)
			{
				tmpdat=SEND_MAX;
			}
			sm->SERIAL_DATA[4] = (uint16_t)tmpdat / 256;        //  需要上传的基准
			sm->SERIAL_DATA[5] = (uint16_t)tmpdat % 256;
			
			tmpdat = sm->VS0_MIN;
			tmpdat /= sm->Ratio;
			if(tmpdat>=SEND_MAX)
			{
				tmpdat=SEND_MAX;
			}
			sm->SERIAL_DATA[6] = (uint16_t)tmpdat / 256;         //   VS0_MIN
			sm->SERIAL_DATA[7] = (uint16_t)tmpdat % 256;
		}
		else
		{
			sm->SERIAL_DATA[2] = sm->VS_ADC_VOL / 256;      //实时电压  
			sm->SERIAL_DATA[3] = sm->VS_ADC_VOL % 256;
			sm->SERIAL_DATA[4] = sm->SensorVS0 / 256;        //  需要上传的基准
			sm->SERIAL_DATA[5] = sm->SensorVS0 % 256;
			sm->SERIAL_DATA[6] = sm->VS0_MIN / 256;         //   VS0_MIN
			sm->SERIAL_DATA[7] = sm->VS0_MIN % 256;
		}
	sm->SERIAL_DATA[8] = sm->NTC_TEMP > 0 ? 1:0;
	sm->SERIAL_DATA[9] = sm->NTC_TEMP > 0 ? sm->NTC_TEMP:0-sm->NTC_TEMP;//温度值
//	sm->SERIAL_DATA[10] = sm->have_after_o3_mode_flag;  //测试O3延时时长用 ，可保留 20211207
	sm->SERIAL_DATA[11] = (uint16_t)sm->SensorVS0TempFloat/256;//2h累积均值
    sm->SERIAL_DATA[12]=(uint16_t)sm->SensorVS0TempFloat%256;
		sm->SERIAL_DATA[13]=  sm->Ratio*100;  //输出VS比值系数 20230625
	sm->SERIAL_DATA[14]=(uint16_t)sm->NTC_temp_MAX;
//    sm->SERIAL_DATA[14] =sm->cleanup_flag;
	sm->SERIAL_DATA[15] = dataXorCRC(sm->SERIAL_DATA,15);  //CRC

}
/*
**将等级按照海尔规定优化映射
**71-99 不做映射，
**1-49不做映射
**50-70映射成49
*/

uint8_t mappingDegree(uint8_t degree)
{
  //float temp;
  uint8_t degree_out;
  if((degree>=71)||(degree<=49)){//71-99以及1-49之间不做映射
  	degree_out=degree;
  }else {//50~70之间输出49
    degree_out=49;
  }
  //合法区间保护
  if(degree_out>99){
	  degree_out=99;
  }
  if(degree_out<1){
	  degree_out=1;
  }
  return degree_out;
}

uint8_t dataXorCRC(uint8_t * sd,uint8_t length)
{
  uint8_t xordata,i;
  xordata = sd[0];
  for(i=1;i<length;i++)
  {
     xordata ^=sd[i];
  }
  return xordata;
}

void show_time_vs0_update(SensorModule* sm)
{
	sm->SensorRealTimeAvgValueFloat=sm->VS_ADC_VOL;
	sm->SensorCntPerRealTime = 1;
	sm->show_vs_avg_float=sm->VS_ADC_VOL;

//	sm->SensorCntPerCycle=SHOW_VS0_UPDATE_CYCLE +1;
	sm->has_after_show_flag=1;
	sm->after_show_cntdown=VS0_FAST_UPDATE_CNT;
	sm->after_show_vs_avg_cnt = 1;//演示模式之后开始累积vs均值

}



/*
	传感器周期数据的预处理
*/
void update_VS0(SensorModule* sm)
{
	float temp_change;
  
	/*
	**主控下发臭氧信号后的传感器处理
	*/
//	debugO3Mode(sm);
	processO3Mode(sm);
	/*
	**主控下发指令标定VS0
	*/
 	if(calibration_flag==CALIBRATION_VS0_MIN){//重跑VS0_MIN,下发标定指令之后需重新跑下限值
		calibration_flag=0;
		sm->has_baselineTEMP_flag=0;
		sm->has_baselineMIN_flag=0;
		Flash_int();
		StandardSensorModuleInit(sm);//初始化
		
		sm->has_baselineTEMP_flag=0;
		sm->has_baselineMIN_flag=0;
	}
	else if(calibration_flag==CALIBRATION_VS0){//标定VS0
		calibration_flag=0;
		if((sm->SensorVS0==0)||(sm->VS0_MIN==0)){return; }
		sm->SensorVS0=sm->SensorVS0TempFloat;

		if(sm->SensorVS0>sm->VS0_MIN)
		{
			sm->SensorVS0 =sm->SensorVS0;
		}else{
		  sm->SensorVS0 =sm->VS0_MIN;
		}
		sm->has_baselineTEMP_flag=1;
		sm->has_baselineMIN_flag=1;
		SetVSBaselineToEE(sm);
	}else if(calibration_flag==customer_change_VS0_MIN){//标定VS0_MIN
			calibration_flag=0;
			sm->VS0_MIN=sm->SensorVS0TempFloat*VS_MIN_PROTECT_COEF;
			sm->has_baselineMIN_flag=1;
			SetVSBaselineToEE(sm);

	}
	
	if(sm->SensorCntPerCycle == SENSOR_CNT_PER_CYCLE + 1)//满足一个2H小周期
	{
			sm->SensorCntPerCycle = 1;
			sm->cycle_2H_flag=1;
			//更新大周期的均值缓存
			sm->NTC_temp_MAX=sm->NTC_TEMP;
			if(sm->SensorVS0<sm->SensorVS0TempFloat)
			{//基准往上更新
				temp_change=((float)sm->SensorVS0TempFloat-(float)sm->SensorVS0)*VS0_CHANGE_UP_COEF;
				sm->SensorVS0+=temp_change;
			}
			else if(sm->SensorVS0>sm->SensorVS0TempFloat)
			{
				temp_change=((float)sm->SensorVS0-(float)sm->SensorVS0TempFloat)*VS0_CHANGE_DOWN_COEF;
				sm->SensorVS0-=temp_change;
			}
			//更新基准值
			if(sm->SensorVS0<sm->VS0_MIN){
				sm->SensorVS0=sm->VS0_MIN;
			}
			if(sm->customer_run_cnt>=CUSTOMER_KEEP_RUN_CNT){//在客户家连续运行一周开始记录基准值
					sm->has_baselineTEMP_flag=1;
					SetVSBaselineToEE(sm);
			}	
 	}
//    if((O3_CLOSE_CMD==O3_open_flag)||(0==O3_open_flag))//臭氧开启时的VS不带入计算均值
	if(0==sm->O3_MODE_PROTERCT)
	{
		sm->SensorVS0TempFloat = ((float)sm->SensorVS0TempFloat/(float)sm->SensorCntPerCycle)\
				*(sm->SensorCntPerCycle - 1) + (float)sm->VS_ADC_VOL/(float)sm->SensorCntPerCycle;

	}
	
	if((sm->SensorCntPerCycle < SENSOR_CNT_PER_CYCLE)&&sm->cycle_2H_flag==0)//没有达到第一个小周期则保持向上跟随
	{
		if((sm->SensorCntPerCycle<SENSOR_CNT_PER_CYCLE)&&(0==sm->has_after_show_flag)){			
				if(sm->VS0_follow_cnt < VS0_FOLLOW_CYCLE_CNT) //VS0开机快速跟随 20211206
					{
						if(sm->SensorVS0 < sm->VS_ADC_VOL){
							sm->SensorVS0 = sm->VS_ADC_VOL;
							sm->SensorVS0TempFloat = sm->SensorVS0;							
						}
						sm->VS0_follow_cnt ++;
					}
					else{
						sm->SensorVS0=sm->SensorVS0TempFloat;
            if(sm->SensorVS0<sm->VS0_MIN) {
                sm->SensorVS0=sm->VS0_MIN;
            }
					}
		}
	}
	/*记录基准下限*/
	if(sm->SensorCntPerCycle == SENSOR_MIN_LINE_CNT){
		if(sm->has_baselineMIN_flag==0){
			if(sm->Ratio==0)//海尔一代有重跑VS_min指令 20230625
			{
				if(VS_Accumulative_cnt)//累计次数不能为0 20230625
				{
					vs_avg_120=VS_120_accumulative/VS_Accumulative_cnt;//120sVS均值 20230625
					VS_120_accumulative=0;
					VS_Accumulative_cnt=0;
					if((vs_avg_120<RL_PER_VS_MAX)&&(vs_avg_120>RL_PER_VS_MIN))//当传感器电压在1.5V——0.5V之间，将传感器电压模拟到0.85——1.05之间 20230625
					{
						sm->Ratio = (float)vs_avg_120/(((((float)vs_avg_120-VS_MIN)/(VS_MAX-VS_MIN))*(VS_SHOW_MAX-VS_SHOW_MIN))+VS_SHOW_MIN);//将VS电压范围从0.1~1.7v锁定在0.75~0.85v，计算VS与输出VS比值 20230625
						sm->Ratio = (uint8_t)((sm->Ratio+0.005)*100);//保留2位有效小数（四舍五入） 20230625
						sm->Ratio = sm->Ratio/100;     //20230625
					}
			  }
			}
			sm->has_baselineMIN_flag=1;
			sm->VS0_MIN=sm->SensorVS0TempFloat*VS_MIN_PROTECT_COEF;
			sm->SensorVS0=sm->VS0_MIN;
			SetVSBaselineToEE(sm);
			sm->SensorVS0=sm->SensorVS0TempFloat;
		}	
	}
	//更新2H周期内NTC温度的最大值
	if(sm->NTC_temp_MAX<sm->NTC_TEMP){
		sm->NTC_temp_MAX=sm->NTC_TEMP;
	}
	
	if(sm->has_baselineMIN_flag==0)//跑完VS0_MIN后就不计算了 20230625
	{
		if((sm->SensorCntPerCycle >= SENSOR_MIN_LINE_CNT-120)&&(sm->SensorCntPerCycle <SENSOR_MIN_LINE_CNT))
		{
			if(sm->VS_ADC_VOL)//无效VS0值不进行累加
			{
				VS_120_accumulative+=sm->VS_ADC_VOL;
				VS_Accumulative_cnt++;
			}
		}
		else{
			VS_120_accumulative=0;
			VS_Accumulative_cnt=0;
		}
	}
	sm->SensorCntPerCycle ++;
	sm->customer_run_cnt++;
}
/*
**
臭氧信号下发后的处理
*/
void processO3Mode(SensorModule* sm)
{
	static uint16_t o3_mode_cntdown=O3_MODE_CNTDOWN;//臭氧影响倒计时
	
	if(O3_open_flag==O3_OPEN_CMD){//臭氧开启
		sm->O3_MODE_PROTERCT=1;
		o3_mode_cntdown=O3_MODE_CNTDOWN;//臭氧信号开启，倒计时复位
		if(0==sm->have_after_o3_mode_flag){
			sm->have_after_o3_mode_flag=1;
		}
	}else if(O3_open_flag==O3_CLOSE_CMD){//臭氧关闭
		if(sm->have_after_o3_mode_flag==1){//是否经过一次臭氧环境
			o3_mode_cntdown--;
			if(o3_mode_cntdown<2){//臭氧影响倒计时清零，
				sm->O3_MODE_PROTERCT=0;//臭氧保护清零，VS0正常累积
				sm->have_after_o3_mode_flag=0;
			}
			if(sm->VS_ADC_VOL<sm->SensorVS0+10){//vs下降到VS0以下（保留0.01伏）
				sm->O3_MODE_PROTERCT=0;
				sm->have_after_o3_mode_flag=0;
			}
		}
	}
}
/*

	传感器实时数据的预处理

  【主要为了传感器小周期的滤波/30分钟的一次滤波】

*/
void SensorRealTimeDataHandle(SensorModule* sm)
{	
	if(sm->SensorCntPerRealTime == SENSOR_CNT_PER_RT + 1)
	{
		sm->SensorCntPerRealTime = realtime_reset_value;	   //可以有一定的初始计数，防止数据波动过大。
//		sm->SensorCntPerRealTime = SENSOR_CNT_PER_RT/2;
	}
	
	sm->SensorRealTimeAvgValueFloat = ((float)sm->SensorRealTimeAvgValueFloat/(float)sm->SensorCntPerRealTime) *(sm->SensorCntPerRealTime - 1) \
										+ (float)sm->VS_ADC_VOL/(float)sm->SensorCntPerRealTime;
	
	sm->SensorRealTimeAvgValue = sm->SensorRealTimeAvgValueFloat;
	
	sm->SensorCntPerRealTime ++;	
}

/*
**演示模式退出之后的VS0累积均值。  
**
*/
void after_show_VS_filter(SensorModule* sm)
{
    if(sm->has_after_show_flag==0){
		sm->after_show_vs_big_cnt=0;
		sm->after_show_vs_reback_flag=0;
		return;
	}
	if(sm->after_show_vs_avg_cnt == AFTER_SHOW_CNT_CYCLE + 1)
	{
		sm->after_show_vs_avg_cnt = 1;	   //可以有一定的初始计数，防止数据波动过大。
	}	
	sm->show_vs_avg_float = ((float)sm->show_vs_avg_float/(float)sm->after_show_vs_avg_cnt) *(sm->after_show_vs_avg_cnt- 1) \
									+ (float)sm->VS_ADC_VOL/(float)sm->after_show_vs_avg_cnt;
    if(sm->VS_ADC_VOL>sm->SensorVS0){
		sm->after_show_vs_big_cnt++;
		if((sm->after_show_vs_big_cnt>3)&&(sm->after_show_vs_reback_flag==0)){
			sm->after_show_vs_reback_flag=1;//连续3次VS大于VS0则设置恢复标志
			sm->after_show_vs_avg_cnt=1;//重置计数并重新累积均值
		}
		
	}else{
		sm->after_show_vs_big_cnt=0;

	}
	sm->after_show_vs_avg_cnt ++;
	
}

/*

	传感器等级计算

*/


void getRS_Degree(SensorModule* sm)
{
//	float rs=0,rs_filter,rs0;
	float rs0;
	float rs_filter_temp1,rs_filter_temp2;
	float rs_temp1=0,rs_temp2=0;
//	static float temp_rs_change=99;
	float degree_r;
	float degree_filter_r;
//	float D1,D2;
//	static float D2_TEMP;
	static float D2_LAST=99;
	float degree_temp;
	static unsigned char show_mode_start_cnt;//满足触发5的次数
	static unsigned char spioled_flag;
//	rs = (float)(sm->VS_ADC_VOL);
//	rs_filter = (float)sm->SensorRealTimeAvgValueFloat;
	rs0 = (float)(sm->SensorVS0);
	if(0==rs0){return;}
	rs_temp1 = ((double)(sm->VS_ADC_VOL) / (double)(sm->VL_ADC_VOL));
	rs_temp2 = ((double)(sm->SensorVS0) / (double)(sm->VC_ADC_VOL - sm->SensorVS0));
	rs_filter_temp1 = ((double)(sm->SensorRealTimeAvgValue) / (double)(sm->VL_ADC_VOL));
	rs_filter_temp2 = ((double)(sm->SensorVS0) / (double)(sm->VC_ADC_VOL - sm->SensorVS0));

	degree_r = (rs_temp1 / rs_temp2)*100.0f;//电阻变化率
	degree_filter_r = (rs_filter_temp1 / rs_filter_temp2)*100.0f;

	if(degree_filter_r>99){
		degree_filter_r=99;
	}
	//判断是否触发演示模式
	if((degree_filter_r > degree_r + QUICK_UP_VALUE )&&(!sm->has_filter_reset))
	{
	     show_mode_start_cnt++;
		 if((show_mode_start_cnt>SPOILED_CNT)&&(spioled_flag==0)){
			 sm->has_filter_reset = 1;
			 spioled_flag=1;
		 } 
	}else{
		 show_mode_start_cnt=0;
		 spioled_flag=0;
	}
//    D1=degree_filter_r;
 //   D2= degree_r;
    if(sm->has_filter_reset==1){//触发演示模式
		
		if(show_keep_cnt++>SHOW_MODE_KEEP_MAX){
				sm->cleanup_flag=0;
				degree_temp = degree_r;
				show_time_vs0_update(sm);
				sm->has_filter_reset = 0;
				show_mode_start_cnt=0;
		 		spioled_flag=0;
				D2_LAST=99;					
		}
		else{//还在演示模式锁定期
			
			if(D2_LAST<degree_r){
				degree_temp = D2_LAST;
			}else
			{
				D2_LAST = degree_r;
				degree_temp = D2_LAST;
			}
			if(degree_temp>90){
				degree_temp=90;
			}
//			D2_TEMP=D2;
			sm->cleanup_flag=2;
		}
	}
	else{//没有触发演示模式
		  degree_temp = degree_filter_r;
		  show_keep_cnt=0;
	}
	if(degree_filter_r<degree_r && sm->has_filter_reset==0 ){//change by pillar.@20210304
		degree_temp = degree_r;
	}
//	if(temp_rs_change>1){
//		degree_temp=temp_rs_change--;//debug 用 99-1自动跑一遍,用于调试测试
//	}
	if(degree_temp > DEGREE_MAX)
	{
		degree_temp = DEGREE_MAX;
	}else if(degree_temp > RS_GATE_1)//85%至99%不做放大
	{
		degree_temp = degree_temp;
	}else if(degree_temp > RS_GATE_3)//85~55映射成85~1
	{
		degree_temp= 2.7899*degree_temp - 151.81;//拟合曲线的公式
	}else{//小于55%
		degree_temp  = 1 ;
	}
	sm->degree = degree_temp;
	

	sm->degree_inv = 100 - sm->degree;// TODO,改成1.5伏加热之后需要调整灵敏度倍乘的系数
	 if(sm->degree_inv > 70) { //等级70以上再做1.3倍放大 满足工厂开机30s测试等级需求 20220114
        sm->degree_inv=(float)sm->degree_inv*DEGREE_GAIN_INDEX;//对灵敏度做放大
    }
	if(++sm->Start_5min_cnt < START_5MIN_CYCLE)  //开机5分钟，灵敏度再做1.5倍放大20220516
	{
	   sm->degree_inv=(float)sm->degree_inv*START_5MIN_DEGREE_GAIN_INDEX;//对灵敏度做放大
	}
	else
	{
	  sm->Start_5min_cnt = START_5MIN_CYCLE;
	}
	if(sm->degree_inv>99){
		sm->degree_inv=99;
	}
	if(sm->degree_inv<1){
		sm->degree_inv=1;
	}
}
/*
**将获取的VS值带入计算，更新VS0，计算degree
*/
uint32_t  calculationTask(void) 
{
//	static uint8_t sensor_warmingup_time=0;//传感器运行的时间计数，用来滤除上电初始阶段的较低VS值 VS0开机跟随 未删除 参照 20211206
    _SS
    while(UERROR) {
        updateVsData();//更新VS,VL,VC,温度值等参数
        if(sys_info.run_timer > INIF_TIME) { //需要等待AD采集的链表中填满数据
            if(sm.VC_ADC_VOL <sm.VL_ADC_VOL )//极端情况会出现VC小于VL的情况
            {
                sm.degree_inv=15;
            }
            else
            {
//VS0开机跟随 未删除 参照 20211206
//                if(sm.has_baselineTEMP_flag==0){//之前没有存储过VS0
//					sensor_warmingup_time++;
//					if(sensor_warmingup_time<WARMINGUP_TIME){
//						if(sm.SensorVS0<sm.VS_ADC_VOL){
//							sm.SensorVS0=sm.VS_ADC_VOL;//预热阶段VS0始终取VS的最大值
//						}
//					}else{
//						sensor_warmingup_time=WARMINGUP_TIME+1;
//						if((sm.NTC_TEMP<51.0f)) { 
//							update_VS0(&sm);
//						}
//					}

//				}else{//EEP中已经存储了VS0值
//					if((sm.NTC_TEMP<51.0f)) { 
//						update_VS0(&sm);
//					}
//				}
              	if((sm.NTC_TEMP<51.0f)) { 
									update_VS0(&sm);
								}
                SensorRealTimeDataHandle(&sm);
                getRS_Degree(&sm);
                sensorAutoMode(&sm);
            }
			packetHmnstUartData(&sm);//打包普通串口数据
			packetHaierBusData(&sm);//打包海尔bus数据
        }
        //指示灯状态切换,非海尔BUS控制LED情况下,执行模组LED控制
				if(led_key != BUS_CTR_LED_ON){
					if(sm.has_baselineMIN_flag==0) { //正在跑vs0_min
							led_sys.led_sta=LED_FLASH_5HZ;
					}				
					else
					{
							led_sys.led_sta=LED_OFF;
					}
				}
		WaitX(1000); //默认1000毫秒
    }
    _EE
}
/*
**发送16字节的串口数据，供hmnst上位机显示用
*/
uint32_t send_data_task(void){
_SS
  while(UERROR){
		if(sys_info.run_timer > INIF_TIME)
		{
			for(uint8_t i=0;i<SEND_BUFF_LEN;i++)  //按照BUFF长度发送
			{
				Send_Data_To_UART0(sm.SERIAL_DATA[i]);
			}
		}
		WaitX(1000); 
  }
_EE
}

//判断传感器是否失效及进入自动模式：NTC触发等级变化
void sensorAutoMode(SensorModule* sm)
{
  /* 20211206 增加换算后的VCC电压范围判断，增加判断测量端电压值范围的逻辑 */
    if((sys_info.input_vol > ERROR_VCC_MAX)||(sys_info.input_vol < ERROR_VCC_MIN)||
			 (sm->VS_vol_float > ERROR_VS_MAX)  ||(sm->VS_vol_float < ERROR_VS_MIN))//传感器电压过高或过低，传感器测量电压修改之后需要同步修改 TODO
    {
		/* mac.zhu-2021/10/18-将sm->VL_ADC_VOL==0改为sm->VL_ADC_VOL<=2 */
        if(sm->degree_inv<5||(sm->VL_ADC_VOL<=2)) {
            sm->degree_inv=sm->NTC_TEMP+16-sm->NTC_temp_MAX;//优-良
            if(sm->degree_inv>20) { //阈值保护，确保报良
                sm->degree_inv=20;
            }
        }
        if(sm->degree_inv>50) { //阈值保护，确保报良
            sm->degree_inv=20;
        }
    }
}






