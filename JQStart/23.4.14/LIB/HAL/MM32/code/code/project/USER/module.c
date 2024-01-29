#include "module.h"
#include "StandardSensorModule.h"

#define	M_MAX(A,B) (A)>(B)?(A):(B)
#define	M_INT(A) 	 (A)>(0)?(A):(0)

void SetSSParaFromEE(void)
{
	uint8_t temp_buf[8];

	memset(temp_buf,0,sizeof(temp_buf));
	
/*δɾ�����룬�����ο�*/
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
	
/*δɾ�����룬�����ο�*/
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

//���������㴦�� δ���ӻ��ߴ�����ʧЧ��õ����Ե��쳣ֵ
uint32_t  vol_res_task(void){
	float coef_temp = 1;
_SS
  while(UERROR){
		sys_info.vref_vol = 1.50;  //���òο���ѹΪ1.5V
		if(vc_v.adc_fil_val){	//ʵ���0����Ϊ0��ϵͳ�������������˴������жϣ��ž������� 20211209
			sys_info.input_vol  =  sys_info.vref_vol*4095.0f/vc_v.adc_fil_val;	//�ɲο���ѹ����������ѹ��������vc_v�ĵ�ѹ1.5V����ο���ѹ�������
		}
		else{
			sys_info.input_vol  =  0;
		}
			
		coef_temp = sys_info.input_vol/SYSTEM_VOLTAGE_VALUE;
		sys_info.vol_3v3_adc=(float)vc_v.adc_fil_val*(3.3/1.5);	//����1.5��������3.3����Ӧ��ADCֵ		
		rs_gas1.adc_fil_val = rs_gas1.adc_fil_val * coef_temp;
		vc_v.adc_fil_val = vc_v.adc_fil_val* coef_temp;
		sys_info.sensor_res_gas1 = MATCHED_RES / rs_gas1.adc_fil_val  - MATCHED_RES;	
		sys_info.heat_vol = sys_info.measuring_vol = 1.50;	//���ü��ȵ�ѹ
		
//	float temp;
//	
//	temp = sys_info.measuring_vol/sys_info.input_vol;		//��Ϊ��Ƭ��ADC������0~5V�����Ǵ������ĵ�ѹ��Χ��0~1.8V �����ϵ��Ϊ����ת����
//	
//	sys_info.sensor_res_gas1 = MATCHED_RES/(rs_gas1.adc_fil_val/(4095.0f*temp))-MATCHED_RES;		//�õ��������ĵ���ֵ����λŷķ
		
//		sys_info.sensor_res_gas2 = MATCHED_RES/(rs_gas2.adc_fil_val/(4095.0f*temp))-MATCHED_RES;		//�õ��������ĵ���ֵ����λŷķ

																																								
		sys_info.ntc_rt1_div_res = (NTC_MATCHED_RES*ntc_rt1.adc_fil_val)/(sys_info.vol_3v3_adc-ntc_rt1.adc_fil_val);	//NTC���裬��λŷķ
		
		sys_info.ntc_rt2_div_res = (NTC_MATCHED_RES*ntc_rt2.adc_fil_val)/(sys_info.vol_3v3_adc-ntc_rt2.adc_fil_val);	//NTC���裬��λŷķ
		
		sys_info.ntc_rt1_temp = -22.55804688*log(sys_info.ntc_rt1_div_res/1000.0f)+75.55857494;				//NTC����ֵͨ��������ߵõ��¶�ֵ����λ�����϶�
		
		sys_info.ntc_rt2_temp = -22.55804688*log(sys_info.ntc_rt2_div_res/1000.0f)+75.55857494;				//NTC����ֵͨ��������ߵõ��¶�ֵ����λ�����϶�
		sys_info.sensor_vc_adc = vc_v.adc_fil_val; //������ѹ����Ӧ��ADC
		sys_info.sensor_vl_adc = rs_gas1.adc_fil_val; //ƥ��������˵�ѹ��Ӧ��ADC
		if(vc_v.adc_fil_val > rs_gas1.adc_fil_val)
		{
			sys_info.sensor_vs_adc = vc_v.adc_fil_val - rs_gas1.adc_fil_val; //���������˵�ѹ��Ӧ��ADCֵ
		}
		sys_info.sensor_vs_float=(float)sys_info.sensor_vs_adc* sys_info.input_vol / 4095.0f/coef_temp;
		WaitX(100);	
		
  }
_EE
}



uint32_t  get_degree_task(void){
_SS
  while(UERROR){
/*δɾ�����룬�����ο�*/
//		sys_info.dac_output_vol = (sys_info.degree_num*0.0305f)+0.13f;
		
		WaitX(1000);
  }
_EE
}


uint32_t receive_data_task(void){
_SS
  while(UERROR){
		if(recv_timeout < 4)//��ʱ5->4 20211209
		{
			recv_timeout++;
		}
		else if(recv_timeout == 4) //��ʱ5->4 20211209
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
		
		/* �˴������մ���������һ�δ������ݹ���֮ǰ���������recv_byte
		if(recv_byte[0] == 0xA5)	//δɾ�����룬�����ο�
		{

		}
		*/
		WaitX(1);
  }
_EE
}
