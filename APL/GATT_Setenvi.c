
/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ͨ����������
����ͬƽ̨ʱAPI��ͬʱ������ͨ��ʱ
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

#include "GATT.h"

const JHAL_IO_Port LIN_WAKE_EN_Port=JHAL_IOC;
const u8 LIN_WAKE_EN_Pin=5;
const JHAL_IO_Port LIN_SLP_EN_Port=JHAL_IOA;
const u8 LIN_SLP_EN_Pin=3;
const JHAL_IO_Port GAS_VCVH_ALL_EN_Port=JHAL_IOD;
const u8 GAS_VCVH_ALL_EN_Pin=6;

//��Ӳ���汾���ֵ0x0F   һ��ÿ��������������ʱ����  debug����ʱ����Ƶ��������
const u8 hardwareVersion=2;
const u8 softwareVersion=1;

//�������ܵ�ѹ
const   u8 gasVCVH_All=5;

u8 adcChannelRemap[ADC_CHANNEL_NUM]= {6,4,0,3};


JHAL_ADCConfig adcConfig= {.vref=JHAL_ADC_ReferVoltage_VDD,.vrefVoltageValue=5,.channels=adcChannelRemap,.channelsNumber=ADC_CHANNEL_NUM};
const JHAL_LIN   SystemLin  = JHAL_LIN0;

const JHAL_ADC SystemADC=JHAL_ADC0;
//��GAS_ID˳����� ���ȴ�ƥ����裨����ѹ�ģ�
const float GAS_VH_RL[GAS_Number]= {310,175,233};
//���۵ĵļ��� ƥ���ѹ
const float GAS_VHL_Predefine[GAS_Number]= {gasVCVH_All-0.9f,gasVCVH_All-1.8f,gasVCVH_All-1.3f};
//����ƫ�� ��x
const float GAS_VHL_Tolerance= 0.4;

