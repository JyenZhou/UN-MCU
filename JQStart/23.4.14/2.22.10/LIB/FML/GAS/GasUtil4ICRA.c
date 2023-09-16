#include "GasUtil4ICRA.h"
/*��������ʱ��仯����  GasUtil4ICRA=GasUtil for Instantaneous Change Rate Alarm  */
//��ŷķΪ��λ�ĵ���


//����������
//#define GasUtil4ICRA_GasSensorNumber	2
//��alarmWindowTimeMs��Ӧ   ����1��ɼ���5���� 300���㼴��  ��RAM������������ٵ�Ҫ��֤��������Ӧ�ٶȹ� �����޷�������ֵ  ����Ҫ�����ʼ���Ĵ洢
//#define GasUtil4ICRA_BuffSiZe 300
#if (GasUtil4ICRA_GasSensorNumber*GasUtil4ICRA_BuffSiZe)



static uint32 gas_lastAcqTime[GasUtil4ICRA_GasSensorNumber] =  {0};
static uint32_t rsShieldMIn_count[GasUtil4ICRA_GasSensorNumber] = {0};
static uint32_t rsShieldMax_count[GasUtil4ICRA_GasSensorNumber] = { 0 };
//ָ����ʱ��Ӧ�ø��ڵ���ֵ ���򱨴���������
static GasUtil4ICRA_InfoTypeDef utilInfo[GasUtil4ICRA_GasSensorNumber]= {0};
//�û������ò���
static GasUtil4ICRA_Configuration configurationParameters[GasUtil4ICRA_GasSensorNumber] = {0};
//�����ص�
static GasUtil4ICRA_SensorAlertCallback alert;
static GasUtil4ICRA_GetSystemDayMs systemRunDayMS;
// 0.��������ʼ������
void GasUtil4ICRA_Init(const GasUtil4ICRA_Configuration*parameters,GasUtil4ICRA_GetSystemDayMs getSystemDayMs,GasUtil4ICRA_SensorAlertCallback GasUtil4ICRA_SensorAlertCallback)
{
    alert = GasUtil4ICRA_SensorAlertCallback;
    systemRunDayMS  =getSystemDayMs;
    //��ֹ�û�����ֲ�ָ��������ͷŵ� �������
    memcpy(configurationParameters, parameters, sizeof(configurationParameters));
    for (uint8_t i = 0; i < GasUtil4ICRA_GasSensorNumber; i++)
    {
        GasUtil4ICRA_gotPowerON(i);
        //��ʱֻ����������δ��ֵ�������ȡ
        utilInfo[i].simulatedCommonModeRejectoValue=0;
    }
}
static bool __simulatedCommonModeEnable=false;
void GasUtil4ICRA_updateVCInfo(float vcMax, float vcMin, int gasIndex,bool simulatedCommonModeEnable)
{
//����һ�㲻��ﵽ���������ֵ��  ������������Ҳ�ǰ��ռ�ֵ����� ����ʱ�򲨶������µĲ��ǵ����  ��2����ΪRL��������˲�
    utilInfo[gasIndex].vcUndulate = (vcMax - vcMin)/2;
    //ѡ�����ֵ����VC���Ծ���������ָ�ֵ
    utilInfo[gasIndex].vc = vcMax;
    //������vl���ʱ�� ��RS��С����Чֵ  ���ഫ�������ڴ��ڹ���/С��ֵ����û�취����
    utilInfo[gasIndex].rsShield_Min  = utilInfo[gasIndex].vcUndulate *  configurationParameters[gasIndex].rl / ( utilInfo[gasIndex].vc - utilInfo[gasIndex].vcUndulate);
    utilInfo[gasIndex].rsShield_Max = ( utilInfo[gasIndex].vc - utilInfo[gasIndex].vcUndulate) * configurationParameters[gasIndex].rl   / utilInfo[gasIndex].vcUndulate;
    __simulatedCommonModeEnable=simulatedCommonModeEnable;
}


/*rs���� ����Ӧʱ�� ���������ϴ����һ����λ�ã��Ƿ�������һ����λ��*/
static float rsDataBuff[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
static uint32 timeMsBuff[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
//��ģ��������ֵ����
static float simulatedCommonModeRejectorValueList[GasUtil4ICRA_GasSensorNumber][GasUtil4ICRA_BuffSiZe] = {0};
/*true�򱨾�*/

// RS����  true����  false����
bool rsShieldAlarmDetection(float *rs, int gasIndex)
{

//1����Ϊ�˷��㸡�����ıȽ�
    if (*rs <= (utilInfo[gasIndex].rsShield_Min+1))
    {

        //���½׶������ֵ�����Ԥ��ʱ��  �����±�־����˵��������������ȥ��  ��ʱ������������ֵ��������������
        if (!  utilInfo [gasIndex].isSteepGrade )
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_LOW, gasIndex);
            return true;
        }

        else if(rsShieldMIn_count[gasIndex]++ > configurationParameters[gasIndex].alarm_timeout_limitRsTimeMs)
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_LOW, gasIndex);
            return true;
        }
        //��Ϊvc�Ĳ�������Ѿ��ǿ��ܵļ���ֵ�� Ϊ�˲��������� ����ֱ�Ӹ�ֵ��ֵ
        *rs=utilInfo[gasIndex].rsShield_Min;
    }
    else
    {
        rsShieldMIn_count[gasIndex] = 0;
    }


    if (*rs >= (utilInfo[gasIndex].rsShield_Max-1))
    {
        if (!  utilInfo [gasIndex].isSteepGrade )
        {
            alert(*rs, GASA_INVALID_DIRECTION, GASA_SRC_RS_HIGH, gasIndex);
            return true;
        }
        else if(rsShieldMax_count[gasIndex]++ > configurationParameters[gasIndex].alarm_timeout_limitRsTimeMs)
        {
            alert(*rs, GASA_INVALID_DIRECTION,GASA_SRC_RS_HIGH, gasIndex);
            return true;

        }


        *rs=utilInfo[gasIndex].rsShield_Max;
    }
    else
    {
        rsShieldMax_count[gasIndex] = 0;
    }
    return false;

}

//ģ�⹲ģ������
void __GasUtil4ICRA_simulatedCommonModeRejector(float vl, float *rs, float *rsMin,u8 gasIndex)
{

    *rs = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
    if(!__simulatedCommonModeEnable)
    {
        return;
    }

    //��ֹ��ģ����ʱvl>vc
    if (vl >utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate)
    {
        vl=utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate;
    } else if(vl<utilInfo[gasIndex].vcUndulate)
    {
        vl=utilInfo[gasIndex].vcUndulate;
    }


    *rs = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
    // VC�Ĳ���Ҳ��Ӱ��VL��� ���Լ�������
    vl +=  utilInfo[gasIndex].vcUndulate;
    if (vl >= utilInfo[gasIndex].vc)
    {
        vl =utilInfo[gasIndex].vc-utilInfo[gasIndex].vcUndulate;

    }
    *rsMin = ((utilInfo[gasIndex].vc - vl) * configurationParameters[gasIndex].rl) / vl;
}


/*
 * �Ƿ�����������
leftStartIndex ��ࣨ���£����ݿ�ʼ����
segmentsNumber �����ݷֳ�3�κ�ÿ�εĴ�С  �������3   ����Ƶ�ʸ����ֵ��Դ��Ϊ��
*/

bool __GasUtil4ICRA_isUptrend(uint8 leftStartIndex, uint8 segmentsNumber, float data[])
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
    //��������ߵ����������µ� //+0.001��Ϊ�˷�ֹ�͵�����ʹ�û���ֵ����ƽ��ֵ��Ӱ��Ӷ���ǰ�������� �ܿ촥���ϵ����  ������µıȽϴ��Ǿ����������� �����½��������Ԥ��
    if(argRegihtData<argLeftData+0.001f)
    {
        return true;
    } else {
        return false;
    }
}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
 *@ �������ܻ���: ���������ݴ���
 * @�������:   ��
 * @�� �� ֵ: true ��������  false  ������
 * @��ע: ��
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/

//�ϵ�ʱ�������������Ҫ�ų��ȶ�ǰ��ֵ  ֻ��Ϊ0ʱ���������в���Ч
static u16 powerOnFilterCount=0;
static u16 powerOnFilterEnd=0;
void GasUtil4ICRA_PushData(float vl, u8 gasIndex)
{
    float rs = 0, rsMin = 0;
    __GasUtil4ICRA_simulatedCommonModeRejector(vl, &rs, &rsMin, gasIndex);
    if( rsShieldAlarmDetection(&rs, gasIndex))
    {
        return;
    }
    //��ǰϵͳ���еļ�ʱ
    uint32 nowTime = systemRunDayMS();
    //������ʱ���ۼ� �����СRS
    uint32 addUpPointTimeMS = 0;
    float maxRS = 0;
    float minRS = 0x7f7fffff ;




    //���еĻ������ݺ���һλ  �±�0�����µ�����
    for (int i = GasUtil4ICRA_BuffSiZe - 1; i > 0; i--)
    {
        rsDataBuff[gasIndex][i] = rsDataBuff[gasIndex][i - 1];
        timeMsBuff[gasIndex][i] = timeMsBuff[gasIndex][i - 1];
        simulatedCommonModeRejectorValueList[gasIndex][i]=simulatedCommonModeRejectorValueList[gasIndex][i - 1];
    }

    //  �ϴ�ʱ��Ϊ0˵���״ν���  ���ʱ��Ӧ��Ϊ0 ����0�ں���������Чֵ ������1
    //���������ʱ��<��ʱ��С̨��Ҳ����Ϊ0һ��Ҫ�������ַ��գ���������ִ��һ�γ����������ʱ�ͻ�������������ʱ�����Ƕ�����ʱ��λ�ģ��������ȡϵͳ��ϢʱΪ����λ���ý������ͱ�����Ϣ���žͼ�����ʱ
    //�ڶ��������Ҫ����Daytimesunset_MS��ȥ����ǰһ���
    uint32 intervalMs = (gas_lastAcqTime[gasIndex] == 0 || nowTime == gas_lastAcqTime[gasIndex]) ? 1 : (nowTime > gas_lastAcqTime[gasIndex] ? nowTime - gas_lastAcqTime[gasIndex] : nowTime + Day2Msec - gas_lastAcqTime[gasIndex]);
    gas_lastAcqTime[gasIndex] = nowTime;
    //��ֵ
    timeMsBuff[gasIndex][0] = intervalMs;
    rsDataBuff[gasIndex][0] = rs;
    simulatedCommonModeRejectorValueList[gasIndex][0]=rsMin/rs;
    /****** �����ֵ��Сֵ�ı�ֵ *********/
    //��Ч�㶨�� ��Ҫ�����������б��
    int effectivePoint;
//�ж��ȶ����Ƶı�־λ��
    u16 startEffectiveIndex=0;
//�ۼƵ����ݴﵽĿ��ʱ�� ��Ҫ���״��ϵ��һ�׼��
    bool alarmWindowTimeMsOver=false;


    u16 maxRsIndex=0;
    u16 minRsIndex=0;
    float simulatedCommonModeRejectoValue = 0;
    //�����ۼƵĵ���
    for (uint16 i = 0; i < GasUtil4ICRA_BuffSiZe; i++)
    {
        effectivePoint = i;
        //�ۼ�ʱ��
        addUpPointTimeMS += timeMsBuff[gasIndex][i];
        //����0 ˵���ô���ʼ���涼��û�б���ֵ�� û��Ҫ�ȽϹ�ȥ
        if (timeMsBuff[gasIndex][i] == 0)
        {
            break;
        }
        //���ۼ�ʱ�䵽��ָ��ʱ��   û��Ҫ�ȽϹ�ȥ �˳�����ļ���
        if ((addUpPointTimeMS >= configurationParameters[gasIndex].alarmWindowTimeMs)||(i==GasUtil4ICRA_BuffSiZe-1))
        {
            alarmWindowTimeMsOver=true;
            break;
        }
//�ϵ�״̬�ѽ����ж� ��̬��ֵ����ѡȡ������״̬
        simulatedCommonModeRejectoValue=max(simulatedCommonModeRejectoValue,simulatedCommonModeRejectorValueList[gasIndex][i]);

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

    utilInfo[gasIndex].simulatedCommonModeRejectoValue=1-simulatedCommonModeRejectoValue;

    float s_s0;

    for (uint16 i = 0; i < effectivePoint; i++)
    {

        if(rsDataBuff[gasIndex][i]<minRS)
        {

            if(!utilInfo [gasIndex].isSteepGrade)

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


            if(__simulatedCommonModeEnable)
            {
                s_s0 = 1-(minRS/maxRS)-utilInfo[gasIndex].simulatedCommonModeRejectoValue;
            } else {
                s_s0 = 1-(minRS/maxRS);
            }
            //�����жϴ��������������Ϊ�˷����жϱ���Դ�����һ��
            if ( s_s0  > configurationParameters[gasIndex].alarm_s_s0_abs)
            {

                break;
            }

        }

    }



    if(utilInfo [gasIndex].isSteepGrade)
    {
//����Ч�����ܹ���3�� ��������ʱ��=������
        if(alarmWindowTimeMsOver)
        {
            //��ȡĩ������б�������ж��Ƿ��ȶ�  (effectivePoint-startEffectiveIndex)%3 ��(effectivePoint-startEffectiveIndex)/3��� ��֤�ܹ��Աȵ���Ч���ݵ����һ�� ��ֹ���һ���ϵ�ʱ���ѹ�������ж�����ʱû���뱨�ϵ���� ����rs/r0ʱ��ȴ����ȥ�� ��һ�µ�����
            if( !__GasUtil4ICRA_isUptrend(startEffectiveIndex+((effectivePoint-startEffectiveIndex)%3),(effectivePoint-startEffectiveIndex)/3, rsDataBuff[gasIndex]))
            {
                utilInfo [gasIndex].isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
                powerOnFilterCount=maxRsIndex;
            }
        }
    }

    if ( s_s0  > configurationParameters[gasIndex].alarm_s_s0_abs)
    {
//�����һ������ƽ��ʱ ֻ��֮ǰĳ�����ݱȽ�Сʱ ֻ�ᵱʱ�������Ե�
        if(maxRsIndex>minRsIndex)
        {
            if(utilInfo [gasIndex].isSteepGrade)
            {
                utilInfo [gasIndex].isSteepGrade=false;
                powerOnFilterEnd=effectivePoint;
//��ǰ����+��һ������ ������2
                powerOnFilterCount=maxRsIndex;
            }
            alert(s_s0, GASA_REDUCTION, GASA_SRC_SHARP_REACTION, gasIndex);
        } //��������Ӧ�ı��� ����ʱ�򲻱�
        else if (!utilInfo [gasIndex].isSteepGrade)
        {
            alert(s_s0, GASA_OXIDATION, GASA_SRC_SHARP_REACTION, gasIndex);
        }
    }


    if(0!=powerOnFilterEnd&&powerOnFilterCount!=GasUtil4ICRA_BuffSiZe)
    {
        powerOnFilterCount++;
    }
}

//��ձ���������������� һ���л�������ʱ����
void GasUtil4ICRA_gotPowerON(u8 gasIndex)
{
    for(u16 i=0; i<GasUtil4ICRA_BuffSiZe; i++)
    {
        timeMsBuff[gasIndex][i]=0;
        rsDataBuff[gasIndex][i]=0;
        simulatedCommonModeRejectorValueList[gasIndex][i]=0;
    }


    gas_lastAcqTime[gasIndex] = 0;
    utilInfo [gasIndex].isSteepGrade  = true;
    rsShieldMIn_count[gasIndex]=0;
    powerOnFilterEnd=0;
    powerOnFilterCount=0;

}

void GasUtil4ICRA_getUtilInfo( GasUtil4ICRA_InfoTypeDef** infos)
{
    *infos=utilInfo;
}

#endif


