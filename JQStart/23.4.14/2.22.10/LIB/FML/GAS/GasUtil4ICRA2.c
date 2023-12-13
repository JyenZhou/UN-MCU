/**
  ****************************GasUtil4ICRA2.c******************************************
  * @brief
  *��������ʱ��仯����  ���а�  ����ź���ǿ��  GasUtil4ICRA2=GasUtil for Instantaneous Change Rate Alarm

#define GasUtil4ICRA2_GasSensorNumber	2 ���������� Ĭ�����֧��16�� ��Ҫ����Ҫ�޸Ļص�����
#define GasUtil4ICRA2_BuffSiZe 300 ��alarmWindowSec��Ӧ   ����1��ɼ���5���� 300���㼴��
��RAM������������ٵ�Ҫ��֤��������Ӧ�ٶȹ� �����޷�������ֵ  ����Ҫ�����ʼ���Ĵ洢

    ******************************************************************************
  * @file     GasUtil4ICRA2.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-11-24
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
#include "GasUtil4ICRA2.h"

#if !GasUtil4ICRA2_BuffSiZe
��Ҫ�������ݻ����С
#endif


static uint32 gas_lastAcqTime =  0;
static uint32_t rsShieldMInTimerBuff[GasUtil4ICRA2_GasSensorNumber] = {0};
static uint32_t rsShieldMaxTimeBuff[GasUtil4ICRA2_GasSensorNumber] = { 0 };
//ָ����ʱ��Ӧ�ø��ڵ���ֵ ���򱨴���������
static GasUtil4ICRA2_InfoTypeDef utilInfo= {0};
//�û������ò���
static GasUtil4ICRA2_Configuration configurationParameters = {0};
//�����ص�
static GasUtil4ICRA2_OutCallback outCallback;

// 0.��������ʼ������
void GasUtil4ICRA2_Init( GasUtil4ICRA2_Configuration *parameters, GasUtil4ICRA2_OutCallback callback)
{
    outCallback = callback;

    configurationParameters=*parameters;
    GasUtil4ICRA2_gotPowerON();


}
static bool __simulatedCommonModeEnable=false;


/*rs���� ����Ӧʱ�� ���������ϴ����һ����λ�ã��Ƿ�������һ����λ��*/
static float rsDataBuff[GasUtil4ICRA2_GasSensorNumber][GasUtil4ICRA2_BuffSiZe] = {0};
static uint32 timeMsBuff[GasUtil4ICRA2_BuffSiZe] = {0};

/*true�򱨾�*/

// RS����  0���� 1��С 2����
GASA_Exception rsShieldAlarmDetection(float *rs, int gasIndex, u32 intervalSec )
{


    if (*rs < (configurationParameters.rsMin[gasIndex]))
    {

        //���½׶������ֵ�����Ԥ��ʱ��  �����±�־����˵��������������ȥ��  ��ʱ������������ֵ��������������
        if (!  utilInfo .isSteepGrade )
        {

            return GASAE_RS_LOW;
        }

        else if((rsShieldMInTimerBuff[gasIndex]+=intervalSec) > configurationParameters.alarmLimitRsTimeoutSec)
        {

            return GASAE_RS_LOW;
        }
        //��Ϊvc�Ĳ�������Ѿ��ǿ��ܵļ���ֵ�� Ϊ�˲��������� ����ֱ�Ӹ�ֵ��ֵ
        *rs=configurationParameters.rsMin[gasIndex];
    }
    else
    {
        rsShieldMInTimerBuff[gasIndex] = GASAE_Normal;
    }


    if (*rs >configurationParameters.rsMax[gasIndex])
    {
        if (!  utilInfo .isSteepGrade )
        {
            return GASAE_RS_HIGH;
        }
        else if((rsShieldMaxTimeBuff[gasIndex]+=intervalSec) > configurationParameters.alarmLimitRsTimeoutSec)
        {

            return GASAE_RS_HIGH;

        }


        *rs=configurationParameters.rsMax[gasIndex];
    }
    else
    {
        rsShieldMaxTimeBuff[gasIndex] = GASAE_Normal;
    }
    return GASAE_Normal;

}



/*
 * �Ƿ�����������
leftStartIndex ��ࣨ���£����ݿ�ʼ����
segmentsNumber �����ݷֳ�3�κ�ÿ�εĴ�С  �������3   ����Ƶ�ʸ����ֵ��Դ��Ϊ��
*/

bool __GasUtil4ICRA2_isUptrend(uint8 leftStartIndex, uint8 segmentsNumber, float data[])
{
//���ƶԱȵ�ÿ�����ݸ���  ��Ϊ3��  ��һ�κ͵����ζԱ�

    u8 regihtStartIndex=leftStartIndex+segmentsNumber*2;
    float  argLeftData=data[leftStartIndex];
    float  argRegihtData=data[regihtStartIndex];
    for(u8 i=1; i<segmentsNumber-1; i++)
    {
        argLeftData+=data[leftStartIndex+i];
        argLeftData/=2;

        argRegihtData+=data[i+regihtStartIndex];
        argRegihtData/=2;

    }
    //��������ߵ����������µ�
    if(argRegihtData<argLeftData)
    {
        return true;
    } else {
        return false;
    }
}
//�仯�ʲ���1-n%����յ� ��ֹ���貨����ɵļٹյ�
//#define Filtration_FlectionPoint 1
//����յ����   ���±�0��ʼ�ҳ������һ���յ�  ����ֵ0�޹յ�  �����жϵ�ǰ�����������½�
u16   findInflectionPoint(u32* array,u16 maxIndex)
{
    for(u16 i=1; i<maxIndex-1; i++)
    {
        if(array[i]<array[-i])
        {
            if(array[i+1]>array[i])
            {
                return i;
            }
        } else
        {
            if(array[i+1]<array[i])
            {
                return i;
            }

        }

    }
    return 0;
}




/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
 *@ �������ܻ���: ������⴫ֵ  ��������ʱ����һͬ���� gasIndex��Ч
 * @�������:   ��
 * @�� �� ֵ:  ��
 * @��ע: ��
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/

//�ϵ�ʱ�������������Ҫ�ų��ȶ�ǰ��ֵ  ֻ��Ϊ0ʱ���������в���Ч
static u16 powerOnFilterCount=0;
static u16 powerOnFilterEnd=0;

static   JHAL_FloatInde4Set rs_r0[GasUtil4ICRA2_GasSensorNumber];
void GasUtil4ICRA2_PushData(float *vl)
{

    float rs;

    GASA_Exception exception=GASAE_Normal;
    int8 exIndex=-1;


    float rsShieldMaxRs=0;
    float rsShieldMinRs=0x7f7fffff;
    //��ǰϵͳ���еļ�ʱ
    uint32 nowTime = jsystemSec;

    if(jsystemSec<=gas_lastAcqTime)
    {
//�������һ��Ĳ�����
        return;
    }
    uint32 intervalSec = jsystemSec-gas_lastAcqTime;
    gas_lastAcqTime = nowTime;



    for(u8 gasIndex=0; gasIndex<GasUtil4ICRA2_GasSensorNumber; gasIndex++) {

        rs = ((configurationParameters.vc[gasIndex] - vl[gasIndex]) * configurationParameters.rl[gasIndex]) / vl[gasIndex];

        exception =rsShieldAlarmDetection(&(rs), gasIndex,intervalSec);
        if(exception==GASAE_RS_LOW)
        {
            if(rsShieldMinRs>rs)
            {
                exIndex=gasIndex;
                rsShieldMinRs=rs;

            }


        } else if(exception==GASAE_RS_HIGH)
        {

            if(rsShieldMaxRs<rs)
            {
                exIndex=gasIndex;
                rsShieldMinRs=rs;

            }
        }



        //������ʱ���ۼ� �����СRS
        uint32 addUpPointTimeMS = 0;
        float maxRS = 0;
        float minRS = 0x7f7fffff ;




        //���еĻ������ݺ���һλ  �±�0�����µ�����
        for (int i = GasUtil4ICRA2_BuffSiZe - 1; i > 0; i--)
        {
            rsDataBuff[gasIndex][i] = rsDataBuff[gasIndex][i - 1];
            timeMsBuff[i] = timeMsBuff[i - 1];
        }

        //��ֵ
        timeMsBuff[0] = intervalSec;
        rsDataBuff[gasIndex][0] = rs;
        /****** �����ֵ��Сֵ�ı�ֵ *********/
        //��Ч�㶨�� ��Ҫ�����������б��
        int effectivePoint;
//�ж��ȶ����Ƶı�־λ��
        u16 startEffectiveIndex=0;
//�ۼƵ����ݴﵽĿ��ʱ�� ��Ҫ���״��ϵ��һ�׼��
        bool alarmWindowSecOver=false;

        u16 maxRsIndex=0;
        u16 minRsIndex=0;

        //�����ۼƵĵ���
        for (uint16 i = 0; i < GasUtil4ICRA2_BuffSiZe; i++)
        {
            effectivePoint = i;
            //�ۼ�ʱ��
            addUpPointTimeMS += timeMsBuff[i];
            //����0 ˵���ô���ʼ���涼��û�б���ֵ�� û��Ҫ�ȽϹ�ȥ
            if (timeMsBuff[i] == 0)
            {
                break;
            }
            //���ۼ�ʱ�䵽��ָ��ʱ��   û��Ҫ�ȽϹ�ȥ �˳�����ļ���
            if ((addUpPointTimeMS >= configurationParameters.alarmWindowSec)||(i==GasUtil4ICRA2_BuffSiZe-1))
            {
                alarmWindowSecOver=true;
                break;
            }


            if(rsDataBuff[gasIndex][i]>maxRS)
            {
                maxRsIndex=i;
                maxRS=rsDataBuff[gasIndex][i];
            }
        }
        if(effectivePoint==0)
        {
//ֻ��ͨ���ֶ��л�������ʱʱ��˲���󳬹�Ԥ��5��������û����Ч�� �ᵼ��0/1 �仯�ʱ���
            return;
        }



        for (uint16 i = 0; i < effectivePoint; i++)
        {

            if(rsDataBuff[gasIndex][i]<minRS)
            {

                if(!utilInfo.isSteepGrade)

                {
                    if(i<powerOnFilterCount) {
                        minRsIndex=i;
                        minRS=rsDataBuff[gasIndex][i];
                    }
                } else {
//�ϵ��ȶ�ǰ��minrs����Ҫ��max��ɼ���
                    if(maxRsIndex>=i) {
                        minRS=rsDataBuff[gasIndex][i];
                        minRsIndex=i;
                    }
                }
            }
        }

        if(utilInfo.isSteepGrade)
        {
//����Ч�����ܹ���3�� ��������ʱ��=������
            if(alarmWindowSecOver)
            {
                //��ȡĩ������б�������ж��Ƿ��ȶ�  (effectivePoint-startEffectiveIndex)%3 ��(effectivePoint-startEffectiveIndex)/3��� ��֤�ܹ��Աȵ���Ч���ݵ����һ�� ��ֹ���һ���ϵ�ʱ���ѹ�������ж�����ʱû���뱨�ϵ���� ����rs/r0ʱ��ȴ����ȥ�� ��һ�µ�����
                if( !__GasUtil4ICRA2_isUptrend(startEffectiveIndex+((effectivePoint-startEffectiveIndex)%3),(effectivePoint-startEffectiveIndex)/3, rsDataBuff[gasIndex]))
                {
                    utilInfo.isSteepGrade=false;
                    powerOnFilterEnd=effectivePoint;
                    powerOnFilterCount=maxRsIndex;
                }
            }
        }
//�����һ������ƽ��ʱ Ԥ�����
        if(utilInfo.isSteepGrade&&maxRsIndex>minRsIndex)
        {   //�仯�ʴﵽ10%�ж�ΪԤ�����
            if(minRS/maxRS<0.9f)
            {
                utilInfo.isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
                powerOnFilterCount=maxRsIndex;
            }

        }


//���ߵĴ���
//Ԥ��ʱ���������RS
        if(utilInfo.isSteepGrade)
        {
            utilInfo.baseLine[gasIndex]=maxRS;
            rs_r0[gasIndex].value=0.999f;
        } else
        {
//�����ǰֵ�ʹ��������ֵ/��Сֵ �仯С��x% ���������Իָ�
            if((minRS/maxRS)> configurationParameters.baseLineSelfRecoveryaAllow4WindowsRate )
            {
                float differenceRsValue=((float)intervalSec /configurationParameters.baseLineSelfRecoverySec*fabs(utilInfo.baseLine[gasIndex]-rs));

                if(utilInfo.baseLine[gasIndex]>rs)
                {
                    utilInfo.baseLine[gasIndex]-=differenceRsValue;
                } else
                {
                    utilInfo.baseLine[gasIndex]+=differenceRsValue;
                }

            }

            rs_r0[gasIndex].value=rs/utilInfo.baseLine[gasIndex];

        }
        rs_r0[gasIndex].index=gasIndex;
    }



//���仯��
    JHAL_sortFloatIndexSetAsc(rs_r0,GasUtil4ICRA2_GasSensorNumber);

    outCallback( exception,exIndex,rs_r0);


//�ϵ���Чֵ����
    if(0!=powerOnFilterEnd&&powerOnFilterCount!=GasUtil4ICRA2_BuffSiZe)
    {
        powerOnFilterCount++;
    }

}


//��ձ���������������� һ���л�������ʱ����
void GasUtil4ICRA2_gotPowerON()
{
    for(u16 gasIndex=0; gasIndex<GasUtil4ICRA2_GasSensorNumber; gasIndex++)
    {
        for(u16 j=0; j<GasUtil4ICRA2_BuffSiZe; j++)
        {
            timeMsBuff[j]=0;
            rsDataBuff[gasIndex][j]=0;

        }

        gas_lastAcqTime = 0;
        rsShieldMInTimerBuff[gasIndex]=0;
    }

    utilInfo.isSteepGrade  = true;
    powerOnFilterEnd=0;
    powerOnFilterCount=0;

}

void GasUtil4ICRA2_getUtilInfo( GasUtil4ICRA2_InfoTypeDef* infos)
{
    *infos=utilInfo;
}

