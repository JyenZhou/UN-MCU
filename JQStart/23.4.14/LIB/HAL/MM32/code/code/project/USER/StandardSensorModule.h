#ifndef _StandardSensorModule
#define _StandardSensorModule


#include <stdlib.h>
#include "module.h"

#define  SOFT_VERSION       "HAIER_V1_MM20230713"

#define VS0_FOLLOW_CYCLE_CNT   15 //����ǰ15sVS0ֱ�Ӹ���
//autoģʽ�Ĳ�������
#define ERROR_VS_MAX                    1.45f//��ӦVSΪ1.45������ѹ��������϶��쳣�����ѣ���·�ȣ�
#define ERROR_VS_MIN                    0.012f    // ��ӦVSΪ0.012�� 20211201 

#define DEGREE_MAX                      99//�ȼ����ֵ
#define DEGREE_MIN                      1//�ȼ���Сֵ

/* 20211201�����жϻ�����ĵ�Ƭ��VCC��ѹֵ��Χ���߼� �ֲ�ģ�鹤����ѹ��ΧΪ5��0.2v */
#define ERROR_VCC_MAX                   5.5f//��ӦVCC���5.5v��
#define ERROR_VCC_MIN                   4.2f//��ӦVCC��С4.2v�� ���������ޣ���ֹͨ������USBȡ����ʾʱ�������ģʽ 20211210
/*
**����ʾģʽ�Ĳ�������
*/
#define START_5MIN_CYCLE   		    	300//����5���������ȷŴ� 20220516
#define SENSOR_CNT_PER_CYCLE   			7200//2H һ��С����
#define SENSOR_CNT_PER_RT      			600
#define VS0_CHANGE_UP_COEF   		    0.05f//��׼���ϸ��µ���ϵ������ֵ�İٷֱ�
#define VS0_CHANGE_DOWN_COEF   	        0.05f//��׼���¸��µ���ϵ������ֵ�İٷֱ�
#define QUICK_UP_VALUE                 	5//��ζ������������ֵ����ǰֵ���˲�ֵ��ȣ��ۼ�1200���㣩
#define WARMINGUP_TIME                  7//Ԥ�Ƚ׶ε��ۻ�������Ĭ��10������
/*
**�����������
*/
#define USE_RS_CHANGE                   1//ʹ�õ���仯�ʣ�Ĭ��Ϊ��ѹ
#define CUSTOMER_KEEP_RUN_CNT           604800//��������7��֮��ʼ��¼��׼ֵ

#define SENSOR_MIN_LINE_CNT    		   	3000//60���ӿ�ʼ��¼��׼����ֵ
#define VS_MIN_PROTECT_COEF            	0.10f//��׼���ޱ���
//#define DEGREE_GAIN_INDEX               3.4f//��һ�������������ȵ�����ϵ��
#define DEGREE_GAIN_INDEX_1             2.3f//����1�������ȵ�����ϵ��,85%~70%
#define DEGREE_GAIN_INDEX_2             2.6f//����2�ڵ������ȵ�����ϵ��70%~55%
#define RS_GATE_0                   	99//ԭʼ�仯�ʰٷֱȵ�����ֵ
#define RS_GATE_1                   	85
#define RS_GATE_2                   	65
#define RS_GATE_3                   	55

#define MAP_GATE_1                  	85//��Ҫӳ�䵽�ı仯�ʰٷֱȵ�����ֵ
#define MAP_GATE_2                  	30
#define MAP_GATE_3                  	1

#define realtime_reset_value   			5//��ζ״̬�л��ĳ�ʼֵ
#define DEGREE_GAIN_INDEX           1.3f//�����ȵ�����ϵ�� 20220114
#define START_5MIN_DEGREE_GAIN_INDEX      1.5f//�����ȵ�����ϵ�� 202200516

/*
**�̳���ʾģʽ��ز�������
*/
#define AFTER_SHOW_CNT_CYCLE            600//��ʾģʽ֮�����VS0�����ڵ�����10min
#define SHOW_VS0_UPDATE_CYCLE           600//��ʾģʽ�ָ�֮�����VS0�����ڣ�10min
#define VS0_FAST_UPDATE_CNT       		6//��ʼ���׶ο��ٸ���VS0 �Ĵ���
#define SHOW_MODE_KEEP                  30//��ʾģʽ�ı���ʱ��
#define AFTER_SHOW_VS0_CNTDOWN          150//Լ4���ӣ���ʾ֮��Ļ�׼��������
#define AFTER_SHOW_CNTDOWN_MAX          2//��ʾ����֮����ٸ��»�׼�Ĵ���

#define SHOW_MODE_KEEP_MAX              120//ģʽ��degree���ֵ���ֵ�ʱ��


#define SPOILED_CNT                     2//������ʾģʽ����������


              
/*
**bus�������������·�
*/
#define CALIBRATION_VS0_MIN     		0x15
#define CALIBRATION_VS0    				  0x51
#define customer_change_VS0_MIN     0x55//���ӿͻ����޸�VS0_MIN�Ľӿ�

#define BUS_CTR_LED_ON          		0xFA//bus�������led��״̬
#define BUS_CTR_LED_OFF         		0xF0
#define O3_OPEN_CMD             		0xFA
#define O3_CLOSE_CMD            		0xF0
#define CLOSE_DOOR_FLAG         		0x00
#define OPEN_DOOR_FLAG          		0x01
#define O3_MODE_CNTDOWN         		1500//����Ӱ�쵹��ʱ 1450->1500 20211206
#define BASELINE_STORE_FLAG   			0x55//flash�д洢�˻�׼�ı�־
#define ADC_MAX      					  4096

#define FRAME_HEAD    					0xa5
#define MY_DEVICE_ADDR   				0xB4//��������ĵ�ַ

#define HAIER_DATA_MAX  				10

#define UART0_RX_MAX  					10

/***********************************************

ģ���л�1.5V��1.8V��Ҫע��ĵط�


***********************************************/

#define LDO_OUTPUT_VC                   1.5f//����&���Եĵ�ѹֵ�������ĳ�1.8V 8�紫������Ҫ��1.8v���� 20230625

#define VS_MIN                        (0.1f*4096/5.0f)//ģ��ǰ��ѹֵ���� 20230625
#define VS_MAX                        (1.4f*4096/5.0f)
#define VS_SHOW_MIN                   (0.75f*4096/5.0f)//ģ����ѹֵ���� 20230625
#define VS_SHOW_MAX                   (0.95f*4096/5.0f)

//#define RL_PER_VS                       (1.18f*4096/5.0f)    // ѡ��ƥ�����ʹ�� 20230625
#define RL_PER_VS_MAX                   (1.35f*4096/5.0f)    // VSӳ������ 20230625
#define RL_PER_VS_MIN                   (0.3f*4096/5.0f)    // VSӳ������ 20230625


#define SEND_MAX                       (1.45f*4096/5.0f)//ģ���������ѹֵ 20230625

//#define ERROR_VDD_MAX                   5.55f // ��ӦVSΪ1.78�� 20230625
//#define ERROR_VDD_MIN                   4.45f    // ��ӦVSΪ0.008�� 20230625

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
	
  /* mac.zhu-2021/10/09-�����жϼ��ȶ˺Ͳ����˵�ѹֵ��Χ���߼� */
  float 				VH_VOL;//���ȶ˵�ѹ��Ӧ�ĵ�ѹֵ
 

	float NTC_TEMP;
	float NTC_temp_MAX;
	float sys_VDD;
	float NTC_Baseline;
	float VS_vol_float;//���������˵ĵ�ѹֵ����λ����
    unsigned char cycle_2H_flag;//�Ѿ�����һ��С����
	float   SensorVS0TempFloat;	
	unsigned int  SensorCntPerCycle; //������ÿһ�����ڵļ�������Χ��1-7200 ��ʼ��Ϊ1
    unsigned long customer_run_cnt;//�ͻ��˳������еĴ���
	unsigned int  VS0_MIN ;//��׼����
	unsigned int  SensorVS0;

	unsigned int  SensorCntPerRealTime;
	unsigned int  SensorRealTimeAvgValue;
	float   SensorRealTimeAvgValueFloat;
	
	unsigned char degree;
	unsigned char degree_inv;

	unsigned char has_filter_reset;
	unsigned char has_baselineMIN_flag;//��׼���ޱ�־
	unsigned char has_baselineTEMP_flag;//��ʱ��׼��־
	unsigned char has_baseline_real_flag;//��׼��־
	
	unsigned char cleanup_flag;//�ص����µı�־
	unsigned char has_after_show_flag;//����һ����ʾģʽ
	unsigned char after_show_cntdown;//������ʾ֮��VS0���ٸ��µĴ���
	unsigned char heat_stop_flag;//��ͣ���ȵı�־
	unsigned char heat_stop_cntdown;//��ͣ���ȵ���ʱ
	unsigned char O3_MODE_PROTERCT;//����Ӱ���ڼ�����VS0����
	unsigned char have_after_o3_mode_flag;//ÿ����һ�γ�������������λ
	unsigned char SERIAL_DATA[17];
	//��ʾģʽ֮���vs�ۼ�
	unsigned int  after_show_vs_avg_cnt;//��ʾ֮��VS����VS0���ۻ���ֵ�Ĵ���

	unsigned char after_show_vs_big_cnt;//��ʾ֮��VS������VS0���ۻ���ֵ�Ĵ���

	unsigned char after_show_vs_reback_flag;//��ʾ֮��VS��ʼ�ָ�������VS0�ı�־
	float  show_vs_avg_float;//��ʾ֮��VS���ۻ���ֵ������ۻ���600
	unsigned int  BANDGAP_UID_VOL;//uid
	float        bandgap_vol;//��׼ֵ
	unsigned int BANDGAP_ADC_VOL;//ADC��׼ֵ
	unsigned char VS0_follow_cnt; //����15sVS0����
 	unsigned int Start_5min_cnt; //����5���ӵȼ��Ŵ��ʱ�� 20220516
	  float Ratio;                 //ϵ��ֵ����Ҫ�洢  20230625
}SensorModule;

typedef struct{
  uint8_t frame_head1;//֡ͷ
  uint8_t frame_head2;//֡ͷ
  uint8_t frame_length;//֡��
  uint8_t device_addr;//��ַ
  uint8_t cmd_bit;//����
  uint8_t function;//����
  uint8_t degree;//�ȼ�
  uint8_t error_flag;//���������
  uint8_t temp_signal;//VS��λ
  uint8_t temp_value;//VS��λ
  uint8_t reserved1;//Ԥ��
  uint8_t vs1_high;//VS0��λ
  uint8_t vs1_low;//VS0��λ
  uint8_t VS0_MIN_H;//VS0_MIN��λ
  uint8_t VS0_MIN_L;//VS0_MIN��λ
  uint8_t reserved4;
  uint8_t reserved5;
  uint8_t crc;
}Haier_bus_dat_T;//���崫���BUS���ߵ�����

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