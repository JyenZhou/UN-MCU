#ifndef __GasUtil4ICRA_H__
#define __GasUtil4ICRA_H__


#include "JHAL.h"
//����������
//#define GasUtil4ICRA_GasSensorNumber	2
//��alarmWindowTimeMs��Ӧ   ����1��ɼ���5���� 300���㼴��  ��RAM������������ٵ�Ҫ��֤��������Ӧ�ٶȹ� �����޷�������ֵ  ����Ҫ�����ʼ���Ĵ洢
//#define GasUtil4ICRA_BuffSiZe 300


typedef enum
{

    //������˲ʱ�仯����
    GASA_SRC_SHARP_REACTION=0,
    //��������ʱ����ֵ���� ��Ӧ΢й©
    GASA_SRC_RS_LOW,
//�൱����������  �ഫ�������Բ������д�����
    GASA_SRC_RS_HIGH
}
//��������Դ
GASA_TriggerSource;



typedef enum
{
    //��Ч�Ĳ���
    GASA_INVALID_DIRECTION=0,
    //������
    GASA_OXIDATION,
    //��ԭ��
    GASA_REDUCTION

}//�źŷ���
GASA_TriggerDirection;

typedef struct
{


    //�������ڵ�λʱ��  ��������1����   �����Ǹ�ʱ����rs/r0������ֵ�ű���  ��Ҫ���ݵ͹����·ǵȼ���ɼ����ʱ���������������  ����ֵ5����  5*60*1000
    uint32_t alarmWindowTimeMs;
    //��λʱ���ڱ�����ֵ ע���ǡ��仯�ˡ����ǡ��仯���� abs(rs/r0 -1) ����ֵ��HGS1000>=0.2  HGS1001>=0.4
    float alarm_s_s0_abs;

    //rs����ʱ�����/�߱���������ʱ����ֵ
    float alarm_timeout_limitRsTimeMs;
    //ƥ������С ���ʱ�����ʱ���뵥λһ��
    float rl;

} GasUtil4ICRA_Configuration;
typedef struct
{


    //rs�ļ���ֵ
    float  rsShield_Min;
    float  rsShield_Max;
    //VC����ֵ  �� GasUtil4ICRA_updateVCInfo ����ȥ�Ĳ�ֵ
    float  vcUndulate;
    //ѡ�����ֵ����VC���Ծ���������ָ�ֵ
    float vc;
    //��ģ���Ʊ仯��   ����Ƕ�̬��     ������ֵ�൱�����ֵ +�û���������alarm_s_s0_abs
    float simulatedCommonModeRejectoValue;
    //�Ƿ����ϵ�����״̬
    bool isSteepGrade;
} GasUtil4ICRA_InfoTypeDef;

//GasUtil4ICRA_SensorAlertCallback �����ص�   rete �仯��    triggeroure����Դ   Ŀ�괫��������
typedef void        (*GasUtil4ICRA_SensorAlertCallback)(float rete, GASA_TriggerDirection direction,GASA_TriggerSource triggeroure, u8 gasIndex);
typedef uint32        (*GasUtil4ICRA_GetSystemDayMs)(void);


//���崫������ʼ��������  ���õĸ�����ÿ����������Ӧһ��   ��������⵽��ζʱ�������жϻص�
void GasUtil4ICRA_Init(const GasUtil4ICRA_Configuration *parameters,GasUtil4ICRA_GetSystemDayMs getSystemDayMs, GasUtil4ICRA_SensorAlertCallback GasUtil4ICRA_SensorAlertCallback);
//����VC��Ϣ  ���ɼ��Ĳ��ǲ���ź� ���ܻ������ģ����  ����ͨ������ģ�⹲ģ������
//������vc��ֵ����  ���ǵ����ڸ��ֹ���ģʽ��Ӱ�쵽���ֵ ���ﵥ���ó���  ����Դ�����б仯ʱ��������У׼
//���ô������Ĳ����ú�������Ҫ����һ�� ��λ������V  simulatedCommonModeEnable�Ƿ�����ģ����  ������һ���̶��Ͻ��ͱ仯�� �Ʋ���ʱ������  һ����������
void GasUtil4ICRA_updateVCInfo(float vcMax, float vcMin, int gasIndex,bool simulatedCommonModeEnable);
//������� ��ֵ  �����󴥷��ص�
void GasUtil4ICRA_PushData(float vl, u8 gasIndex);
//������ÿ���ϵ��ʱ�����
void GasUtil4ICRA_gotPowerON(u8 gasIndex);
//��ȡ�Զ����ɵ�һЩʵʱֵ/��ֵ��Ϣ
void GasUtil4ICRA_getUtilInfo( GasUtil4ICRA_InfoTypeDef** infos);

#endif

