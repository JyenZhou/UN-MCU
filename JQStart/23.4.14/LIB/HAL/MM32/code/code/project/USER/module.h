#ifndef __SSM_H
#define __SSM_H

#include "main.h"

//#define CMFA103J3500HANT 	1
//#define TIMER_MAX_VALUE		1000000		//100W��
//#define INIF_TIME					3	//��ʼ��ʱ��3��
//#define	MATCHED_RES				20000
//#define	SAMPLING_RES			0.1f
//#define	NTC_MATCHED_RES		10000
//#define	DAC_MAX_OUT				5.0f
//#define SS_W_MIN_CNT			3000//50���ӿ�ʼ��¼��׼����ֵ
//#define SS_W_MIN_COEF   	0.7f//��׼���ޱ���
//#define	SEVEN_DAY					604800//��������7��֮��ʼ��¼��׼ֵ
//#define	TWO_HOURS					7200//2H һ��С����
//#define UP_COEF   		    0.05f//��׼���ϸ��µ���ϵ������ֵ�İٷֱ�
//#define DOWN_COEF   	    0.05f//��׼���¸��µ���ϵ������ֵ�İٷֱ�
//#define FIRST_FIL_COEF		7200//һ���˲�ϵ�����ı��ֵ�ɸı��׼�����ٶ�
//#define HEAT_UP_TIME			10	//����������ʱ��
//#define SEND_BUFF_LEN			16	//�������ݰ�����
#define SYSTEM_VOLTAGE_VALUE		5.0f	//ϵͳ������ѹ 5v 20211123
#define CMFA103J3500HANT 	1				
#define TIMER_MAX_VALUE		1000000	//100W��
#define INIF_TIME					3				//��ʼ��ʱ��3��
#define	MATCHED_RES				20000		//������ƥ�������ֵ��ŷķ��
#define	SAMPLING_RES			1.0f		//����������ֵ��ŷķ��
#define	NTC_MATCHED_RES		10000		//NTC��ƥ�������ֵ��ŷķ��
#define	DAC_MAX_OUT				5.0f		//DAC��������ѹ
#define SS_W_MIN_CNT			3000		//50���ӿ�ʼ��¼��׼����ֵ
#define SS_W_MIN_COEF   	0.5f		//��׼���ޱ���
#define	SEVEN_DAY					604800	//��������7��֮��ʼ��¼��׼ֵ
#define	TWO_HOURS					7200		//2H һ��С����
#define UP_COEF   		    0.05f		//��׼���ϸ��µ���ϵ������ֵ�İٷֱ�
#define DOWN_COEF   	    0.05f		//��׼���¸��µ���ϵ������ֵ�İٷֱ�
#define FIRST_FIL_COEF		1000		//һ���˲�ϵ�����ı��ֵ�ɸı��׼�����ٶ�
#define HEAT_UP_TIME			8				//����������ʱ��
#define SEND_BUFF_LEN			16			//�������ݰ�����

#define	AVER_RES_COEF			0.99998f		//�������ƽ��ֵ��ʱ��ݼ�
#define	TRIGGER_COEF			0.9f		//������ֵ
#define	TRIGGER_DELAY			30			//������תΪ�����˲����ӳ�ʱ��
#define PROTECT_TEMP			50.0f		//�����¶�
#define	HEAT_RES_MAX			100			//������������
#define	HEAT_RES_MIN			40   		//��������С����

#define CHECK_DELAY				5				//�������ŵ�״̬���ӳ�����

typedef	enum {NORM_STA = 0, OZONE_STA,UART_STA}check_sta_t;

typedef struct{
	
	float 		vref_vol;				//��׼��ѹ ��Ҫ��ʼ������
	float 		input_vol;  		//��Ƭ����ѹ ��vref����õ���NTC ��͵�Ƭ��ʹ����ͬ��Դ��
	float 		heat_vol;				//���ȵ�ѹ ��vref����õ�
	float 		measuring_vol;	//������ѹ ��vref����õ�
	uint32_t 	sensor_res_gas1;//��������ѹ
	uint32_t 	sensor_res_gas2;//��������ѹ
	uint32_t 	heat_res;				//���ȵ���
	uint32_t 	ntc_rt1_div_res;//NTC���裨ŷķ��
	uint32_t 	ntc_rt2_div_res;//NTC���裨ŷķ��
	uint32_t    sensor_vl_adc;//������ƥ��������˵�ѹ��Ӧ��ADC
    uint32_t    sensor_vs_adc;//��������ѹ��Ӧ��ADC
    uint32_t    sensor_vc_adc;//������·��ѹ��Ӧ��ADC
	float		 	ntc_rt1_temp;					//NTC��õ��¶�
	float		 	ntc_rt2_temp;					//NTC��õ��¶�
	float       sensor_vs_float;//���������˵ĵ�ѹֵ����λ����
	uint32_t	sensor_zero_res;			//��׼��
	uint32_t	sensor_zero_res_temp;	//�����׼��
	uint32_t	sensor_zero_res_min;	//����ʱ���õ���ͻ�׼��
	
	uint8_t 	degree_num;						//����ȼ�ֵ
	float			degree_vol;						//����ȼ���ѹֵ
	uint32_t      vol_3v3_adc;//3.3������Ӧ��ADC
	uint64_t  run_timer;	//ϵͳ����ʱ�䣨�룩
	
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