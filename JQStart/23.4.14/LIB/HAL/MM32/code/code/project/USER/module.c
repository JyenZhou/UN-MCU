#include "module.h"
#include "StandardSensorModule.h"

#define	M_MAX(A,B) (A)>(B)?(A):(B)
#define	M_INT(A) 	 (A)>(0)?(A):(0)

void SetSSParaFromEE(void)
{
	uint8_t temp_buf[8];

	memset(temp_buf,0,sizeof(temp_buf));
	
/*未删除代码，用来参考*/
//	temp_buf[0] = 0x55;
//	temp_buf[1] = sys_info.sensor_zero_res_min >> 8;
//	temp_buf[2] = sys_info.sensor_zero_res_min & 0x0F;
//	
//	if(sys_info.run_timer > SEVEN_DAY)
//	{
//		temp_buf[3] = 0x55;
//		temp_buf[4] = sys_info.sensor_zero_res >> 8;
//		temp_buf[5] = sys_info.sensor_zero_res & 0x0F;
//	}
//	else
//	{
//		temp_buf[3] = 0x00;
//		temp_buf[4] = 0x00;
//		temp_buf[5] = 0x00;
//	}
	EEPROM_Write(temp_buf,8);
}

void GetSSParaFormEE(void)
{
	uint8_t temp_buf[8];
	
	memset(temp_buf,0,sizeof(temp_buf));
	
	EEPROM_Read(temp_buf,8);
	
/*未删除代码，用来参考*/
//	if(temp_buf[0] != 0x55)
//	{
//		sys_info.sensor_zero_res_min = 0;
//	}
//	else
//	{
//		sys_info.sensor_zero_res_min  = temp_buf[1] << 8;
//		sys_info.sensor_zero_res_min |= temp_buf[2];
//	}
//	if(temp_buf[3] != 0x55)
//	{
//		sys_info.sensor_zero_res = 0;
//	}
//	else
//	{
//		sys_info.sensor_zero_res  = temp_buf[4] << 8;
//		sys_info.sensor_zero_res |= temp_buf[5];
//	}
}

//无需做除零处理 未焊接或者传感器失效会得到明显的异常值
uint32_t  vol_res_task(void){
	float coef_temp = 1;
_SS
  while(UERROR){
		sys_info.vref_vol = 1.50;  //设置参考电压为1.5V
		if(vc_v.adc_fil_val){	//实测除0后结果为0，系统能正常工作，此处增加判断，杜绝隐患。 20211209
			sys_info.input_vol  =  sys_info.vref_vol*4095.0f/vc_v.adc_fil_val;	//由参考电压推算出输入电压，这里用vc_v的电压1.5V代替参考电压参与计算
		}
		else{
			sys_info.input_vol  =  0;
		}
			
		coef_temp = sys_info.input_vol/SYSTEM_VOLTAGE_VALUE;
		sys_info.vol_3v3_adc=(float)vc_v.adc_fil_val*(3.3/1.5);	//利用1.5伏来推算3.3所对应的ADC值		
		rs_gas1.adc_fil_val = rs_gas1.adc_fil_val * coef_temp;
		vc_v.adc_fil_val = vc_v.adc_fil_val* coef_temp;
		sys_info.sensor_res_gas1 = MATCHED_RES / rs_gas1.adc_fil_val  - MATCHED_RES;	
		sys_info.heat_vol = sys_info.measuring_vol = 1.50;	//设置加热电压
		
//	float temp;
//	
//	temp = sys_info.measuring_vol/sys_info.input_vol;		//因为单片机ADC量程是0~5V，但是传感器的电压范围是0~1.8V 这里的系数为量程转换用
//	
//	sys_info.sensor_res_gas1 = MATCHED_RES/(rs_gas1.adc_fil_val/(4095.0f*temp))-MATCHED_RES;		//得到传感器的电阻值，单位欧姆
		
//		sys_info.sensor_res_gas2 = MATCHED_RES/(rs_gas2.adc_fil_val/(4095.0f*temp))-MATCHED_RES;		//得到传感器的电阻值，单位欧姆

																																								
		sys_info.ntc_rt1_div_res = (NTC_MATCHED_RES*ntc_rt1.adc_fil_val)/(sys_info.vol_3v3_adc-ntc_rt1.adc_fil_val);	//NTC电阻，单位欧姆
		
		sys_info.ntc_rt2_div_res = (NTC_MATCHED_RES*ntc_rt2.adc_fil_val)/(sys_info.vol_3v3_adc-ntc_rt2.adc_fil_val);	//NTC电阻，单位欧姆
		
		sys_info.ntc_rt1_temp = -22.55804688*log(sys_info.ntc_rt1_div_res/1000.0f)+75.55857494;				//NTC电阻值通过拟合曲线得到温度值，单位是摄氏度
		
		sys_info.ntc_rt2_temp = -22.55804688*log(sys_info.ntc_rt2_div_res/1000.0f)+75.55857494;				//NTC电阻值通过拟合曲线得到温度值，单位是摄氏度
		sys_info.sensor_vc_adc = vc_v.adc_fil_val; //测量电压所对应的ADC
		sys_info.sensor_vl_adc = rs_gas1.adc_fil_val; //匹配电阻两端电压对应的ADC
		if(vc_v.adc_fil_val > rs_gas1.adc_fil_val)
		{
			sys_info.sensor_vs_adc = vc_v.adc_fil_val - rs_gas1.adc_fil_val; //传感器两端电压对应的ADC值
		}
		sys_info.sensor_vs_float=(float)sys_info.sensor_vs_adc* sys_info.input_vol / 4095.0f/coef_temp;
		WaitX(100);	
		
  }
_EE
}



uint32_t  get_degree_task(void){
_SS
  while(UERROR){
/*未删除代码，用来参考*/
//		sys_info.dac_output_vol = (sys_info.degree_num*0.0305f)+0.13f;
		
		WaitX(1000);
  }
_EE
}


uint32_t receive_data_task(void){
_SS
  while(UERROR){
		if(recv_timeout < 4)//延时5->4 20211209
		{
			recv_timeout++;
		}
		else if(recv_timeout == 4) //延时5->4 20211209
		{
			memcpy(recv_byte,recv_byte_temp,sizeof(recv_byte_temp));
			memset(recv_byte_temp,0,sizeof(recv_byte_temp));
			recv_count = 0;
			recv_timeout++;
			lincProcessHaierDat(recv_byte);
			memset(recv_byte,0,sizeof(recv_byte));
			
		}
		else
		{
			//NULL
		}
		
		/* 此处做接收处理，在下一次串口数据过来之前，不会清空recv_byte
		if(recv_byte[0] == 0xA5)	//未删除代码，用来参考
		{

		}
		*/
		WaitX(1);
  }
_EE
}

