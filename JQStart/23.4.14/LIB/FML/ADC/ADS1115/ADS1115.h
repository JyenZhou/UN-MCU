#ifndef _ads_1115
#define _ads_1115
//https://www.jianshu.com/p/e0b448995316
#include "JHAL.h"

 
typedef enum
{ //ADDR PIN ->GND
	ADS115_ADDRESS_GND=0x90,
	 //ADDR PIN ->VDD
	ADS115_ADDRESS_VDD=0x92,
	//ADDR PIN ->SDA
	ADS115_ADDRESS_SDA=0x94,
	//ADDR PIN ->SCL
	ADS115_ADDRESS_SCL=0x96
}ADS1115_Addr;
 
typedef enum
{
	 /******************************************************************************
000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
	*/
  ADS1115Channel_Diff_01       =           (0x0U << 4), // �������0���ź�1����
  ADS1115Channel_Diff_03     =             (0x1U << 4)  ,// �������0���ź�3����
  ADS1115Channel_Diff_13     =             (0x2U << 4),  // �������1���ź�3����
  ADS1115Channel_Diff_23      =            (0x3U << 4) , // �������2���ź�3����
  ADS1115Channel_INGLE_0      =           (0x4U << 4) , //�������� 0
  ADS1115Channel_SINGLE_1    =             (0x5U << 4) , //�������� 1
  ADS1115Channel_SINGLE_2      =           (0x6U << 4) , //�������� 2
  ADS1115Channel_SINGLE_3    =             (0x7U << 4)   //�������� 3

 
}ADS1115Channel ;//ͨ��ѡ�� ��������ѡ������뷽ʽѡ��

typedef enum
{

ADS1115_PGA_6        =     (0x0U << 1), // +/- 6.1144
ADS1115_PGA_4       =           (0x1U << 1), // +/- 4.096
ADS1115_PGA_2           =           (0x2U << 1), // +/- 2.048
ADS1115_PGA_1            =         (0x3U << 1), // +/- 1.024
ADS1115_PGA_05          =         (0x4U << 1), // +/- 0.512
ADS1115_PGA_02             =          (0x5U << 1) , // +/- 0.256
	
}ADS1115_PGA ;//����ѡ��



//ADS1115_Speed_SPS_8    ÿ���ܲ���8�� ��ÿ���������ڵ�ʱ���������룩 = 1000���� / 8 = 125����
//ADS1115_Speed_SPS_860 ÿ���ܲ���800��0  ÿ���������ڵ�ʱ���������룩 = 1000���� / 860 �� 1.1628����
//�����ٶ�Խ�죬ÿ����ADS1115���������ź��вɼ�������������ݡ���ˣ�SPSԽ�󣬲����ٶ�Խ�죬����Ҫ�����ʱ�����Դ�����������������ݡ�
typedef enum
{
 
  ADS1115_Speed_SPS_8            =         (0x0U << 5),
  ADS1115_Speed_SPS_16         =           (0x1U << 5), 
  ADS1115_Speed_SPS_32           =         (0x2U << 5),
  ADS1115_Speed_SPS_64           =         (0x3U << 5),
  ADS1115_Speed_SPS_128           =        (0x4U << 5),
  ADS1115_Speed_SPS_250            =       (0x5U << 5),
  ADS1115_Speed_SPS_475            =       (0x6U << 5),
  ADS1115_Speed_SPS_860            =       (0x7U << 5)
}ADS1115_Speed; //ת������



 
typedef struct
{
	
//�������ʱϵ����СֵӦ��Ϊ1 Ҫ�ǽ������ʿ����޸�����ֵ  ע��IO�з�תʱ��ļ��̫С���ܵ���ʱ��׼ ����0��ȫ��û����ʱ��������¸�ʱ�����������
  
   JHAL_IICConfig iicConfig;
     

 	ADS1115_PGA pga;
  ADS1115_Speed speed;
	//ת����ɵ͵�ƽ�ж�����  ����ת��ģʽ����Ч ��ʹ����ʱ  
	JHAL_IO_Port alert_rdy_Port;
	u8 alert_rdy_Pin;
	//true=����ת�� false=����ת��
	bool isSingleConversion ;
	//��������˲����� 
		JHAL_FilteredMode  filteredModel;
	//��������С�ڵ���1ʱ��ֻ����ԭ����
		 u8 samplingCount4filter;
}  ADS1115Config;


 
 

 
   bool  ads1115Init( ADS1115Config  *config);
 
//mdianNumbe ��λ������  
 
  JHAL_ADCInfo ads1115GetVoltage(ADS1115Config  *config ,  ADS1115Channel ch  );



#endif
