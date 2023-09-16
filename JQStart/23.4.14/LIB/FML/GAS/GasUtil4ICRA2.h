#ifndef __GasUtil4ICRA2_H__
#define __GasUtil4ICRA2_H__


#include "JHAL.h"
//����������
//#define GasUtil4ICRA2_GasSensorNumber	2
//��alarmWindowTimeMs��Ӧ   ����1��ɼ���5���� 300���㼴��  ��RAM������������ٵ�Ҫ��֤��������Ӧ�ٶȹ� �����޷�������ֵ  ����Ҫ�����ʼ���Ĵ洢
//#define GasUtil4ICRA2_BuffSiZe 300

typedef enum
{

    //����
    GASAE_Normal=0,
    //��������ʱ����ֵ���� ��Ӧ΢й©
    GASAE_RS_LOW,
//�൱����������  �ഫ�������Բ������д�����
    GASAE_RS_HIGH
}
//�쳣
GASA_Exception;



typedef enum
{
    //������
    GASA_OXIDATION,
    //��ԭ��
    GASA_REDUCTION

}//��Ӧ
GASA_Type;

typedef struct
{

    //����������Чʱ��  ��������1������Ҫ���ݵ͹����·ǵȼ���ɼ����ʱ���������������


    uint32_t alarmWindowSec;
//���߸���ʱ��
    u32 baseLineSelfRecoverySec;
//���ڲ���>rSmin_rsMax ʱ����ָ�    0-1 ԽСԽ���׻ָ�
    float baseLineSelfRecoveryaAllow4WindowsRate;
    //rs����ʱ�����/�߱���������ʱ����ֵ
    float alarmLimitRsTimeoutSec;
//rs�ļ���ֵ
    float  rsMin[GasUtil4ICRA2_GasSensorNumber];
    float  rsMax[GasUtil4ICRA2_GasSensorNumber];

    //ƥ������С ���ʱ�����ʱ���뵥λһ��
    float rl[GasUtil4ICRA2_GasSensorNumber];
    float vc[GasUtil4ICRA2_GasSensorNumber];


} GasUtil4ICRA2_Configuration;
typedef struct
{
    //�Ƿ����ϵ�����״̬
    bool isSteepGrade;
    float baseLine[GasUtil4ICRA2_GasSensorNumber];
} GasUtil4ICRA2_InfoTypeDef;

//����ص� JHAL_FloatInde4Set �仯��rs/r0
typedef void        (*GasUtil4ICRA2_OutCallback)(GASA_Exception ex, int8 exIndex,JHAL_FloatInde4Set *arraySet);



//���崫������ʼ��������  ���ò�������ȫ��/��̬����
void GasUtil4ICRA2_Init( GasUtil4ICRA2_Configuration *parameters, GasUtil4ICRA2_OutCallback callback);
//������⴫ֵһͬ����
void GasUtil4ICRA2_PushData(float *vl);
//������ÿ���ϵ��ʱ�����
void GasUtil4ICRA2_gotPowerON(void);
//��ȡһЩ�ڲ���ֵ��Ϣ ����debug
void GasUtil4ICRA2_getUtilInfo( GasUtil4ICRA2_InfoTypeDef* infos);

#endif


