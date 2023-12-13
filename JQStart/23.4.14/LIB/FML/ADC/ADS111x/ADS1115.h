#ifndef _ads_1115
#define _ads_1115
 
#include "JHAL.h"


typedef enum
{   //ADDR PIN ->GND
    ADS115_ADDRESS_GND=0x90,
    //ADDR PIN ->VDD
    ADS115_ADDRESS_VDD=0x92,
    //ADDR PIN ->SDA
    ADS115_ADDRESS_SDA=0x94,
    //ADDR PIN ->SCL
    ADS115_ADDRESS_SCL=0x96
} ADS1115_Addr;

typedef enum
{
    /******************************************************************************
    000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
    001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
    010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
    011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
    */

    ADS1115Channel_Diff_01       =           (0x0U << 4), // �������0���ź�1����
    ADS1115Channel_Diff_03     =             (0x1U << 4),  // �������0���ź�3����
    ADS1115Channel_Diff_13     =             (0x2U << 4),  // �������1���ź�3����
    ADS1115Channel_Diff_23      =            (0x3U << 4),  // �������2���ź�3����
    ADS1115Channel_SINGLE_0      =           (0x4U << 4),  //�������� 0
    ADS1115Channel_SINGLE_1    =             (0x5U << 4),  //�������� 1
    ADS1115Channel_SINGLE_2      =           (0x6U << 4),  //�������� 2
    ADS1115Channel_SINGLE_3    =             (0x7U << 4)   //�������� 3


} ADS1115Channel ; //ͨ��ѡ�� ��������ѡ������뷽ʽѡ��

/*
FSR LSB SIZE
��6.144 V
(1) 187.5 ��V
��4.096 V
(1) 125 ��V
��2.048 V 62.5 ��V
��1.024 V 31.25 ��V
��0.512 V 15.625 ��V
��0.256 V 7.8125 ��
*/

typedef enum
{

    ADS1115_PGA_6        =     (0x0U << 1), // +/- 6.1144    ����Ϊ0.125 mV/LSB��
    ADS1115_PGA_4       =           (0x1U << 1), // +/- 4.096 ����Ϊ0.0625 mV/LSB��
    ADS1115_PGA_2           =           (0x2U << 1), // +/- 2.048 ����Ϊ0.03125 mV/LSB��
    ADS1115_PGA_1            =         (0x3U << 1), // +/- 1.024 ����Ϊ0.015625 mV/LSB��
    ADS1115_PGA_05          =         (0x4U << 1), // +/- 0.512 ����Ϊ0.0078125 mV/LSB��
    ADS1115_PGA_02             =          (0x5U << 1),  // +/- 0.256  ����Ϊ0.00390625 mV/LSB��

} ADS1115_PGA ; //����ѡ��







//ADS1115_Speed_SPS_8    ÿ���ܲ���8�� ��ÿ���������ڵ�ʱ���������룩 = 1000���� / 8 = 125����
//ADS1115_Speed_SPS_860 ÿ���ܲ���800��0  ÿ���������ڵ�ʱ���������룩 = 1000���� / 860 �� 1.1628����



typedef enum
{

    ADS1115_Speed_SPS_8     =          0x0U,
    ADS1115_Speed_SPS_16,
    ADS1115_Speed_SPS_32,
    ADS1115_Speed_SPS_64,
    ADS1115_Speed_SPS_128,
    ADS1115_Speed_SPS_250,
    ADS1115_Speed_SPS_475,
    ADS1115_Speed_SPS_860
} ADS1115_Speed; //ת������



typedef struct
{

//stm32 i2c.delayMultiple=10 �ǿ��Ե�

    JHAL_I2CSF i2c;


    ADS1115_PGA pga;
    ADS1115_Speed speed;

    //true=����ת�� false=����ת��
    bool isSingleConversion :1;

    //��������˲�����
    JHAL_FilteredMode  filteredModel;
    //��������С�ڵ���1ʱ��ֻ����ԭ����
    u8 samplingCount4filter;



}  ADS1115;






bool  ads1115Open( ADS1115  *config);

//mdianNumbe ��λ������
//true��ȡ�ɹ� false ��ȡʧ��
bool ads1115GetVoltage(ADS1115  *config,ADS1115Channel ch, JHAL_ADCInfo  *adcInfo  );



#endif