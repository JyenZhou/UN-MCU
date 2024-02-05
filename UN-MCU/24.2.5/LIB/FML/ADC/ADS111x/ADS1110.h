#ifndef _ads_1110
#define _ads_1110
//https://www.jianshu.com/p/e0b448995316
#include "JHAL.h"



//�����ַ�Ǹ���оƬ�ͺŹ̶���  ADS1110A0IDBVT  ��A0����ED0 �Դ�����A7����ED7
typedef enum
{   
    ADS1110_ADDRESS_ED0=0x90,
    ADS1110_ADDRESS_ED1,
	 ADS1110_ADDRESS_ED2,
	 ADS1110_ADDRESS_ED3,
	 ADS1110_ADDRESS_ED4,
	 ADS1110_ADDRESS_ED5,
	 ADS1110_ADDRESS_ED6,
	 ADS1110_ADDRESS_ED7,
 
	
} ADS1110_Addr;

 
//�����̵�ѹ 2.048/pga
typedef enum
{

    ADS1110_PGA1_2V048        =     0x0U  , // +/- 2.048    ����Ϊ0.125 mV/LSB��
    ADS1110_PGA2_1V024       =         0x1U , // +/- 1.024 ����Ϊ0.0625 mV/LSB��
    ADS1110_PGA4_0V512          =         0x2U  , // +/- 0.512 ����Ϊ0.0078125 mV/LSB��
    ADS1110_PGA8_8_0V256      =        0x3U  , // +/- 0.256  ����Ϊ0.00390625 mV/LSB��
 

} ADS1110_PGA ; //����ѡ��




//ADS1115_Speed_SPS_8    ÿ���ܲ���8�� ��ÿ���������ڵ�ʱ���������룩 = 1000���� / 8 = 125����
//ADS1115_Speed_SPS_860 ÿ���ܲ���800��0  ÿ���������ڵ�ʱ���������룩 = 1000���� / 860 �� 1.1628����
 
typedef enum
{

    ADS1110_Speed_SPS_240_12Bit     =          0x0U,
    ADS1110_Speed_SPS_60_14Bit,
    ADS1110_Speed_SPS_30_15Bit,
    ADS1110_Speed_SPS_15_16Bit,
 
} ADS1110_Speed; //ת������



typedef struct
{

//stm32 i2c.delayMultiple=10 �ǿ��Ե�

    JHAL_I2CSF i2c;


    ADS1110_PGA pga;
    ADS1110_Speed speed;

    //true=����ת�� false=����ת��
    bool isSingleConversion :1;

    //��������˲�����
    JHAL_FilteredMode  filteredModel;
    //��������С�ڵ���1ʱ��ֻ����ԭ����
    u8 samplingCount4filter;
 

}  ADS1110;






bool  ads1110Open( ADS1110  *config);

//mdianNumbe ��λ������
//true��ȡ�ɹ� false ��ȡʧ��
bool ads1110GetVoltage(ADS1110  *config , JHAL_ADCInfo  *adcInfo  );



#endif
