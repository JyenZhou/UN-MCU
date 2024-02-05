/**
***************************************************************************
@file         example188.c
@author       ADI
@version      V3.0.0
@date         23-Apr-2018
@brief        Sample application to use ADI ADPD188 driver.
*/

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "AdpdDrv.h"

#include "smoke_detect.h"

/* Macros -------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
uint8_t gnAdpdDataReady = 0;
uint32_t calibrationcount = 0;
uint8_t iscali = 0;

uint16_t nAdpdFifoLevelSize;
//���ô�С  AdpdConfiguration()��������ϸ˵��
uint16_t nAdpdDataSetSize;

float bluePTRFactor, irPTRFactor;
uint32_t blueOffset, irOffset;
uint32_t sampleIndex=0;
extern uint32_t dcfg_org_188[];

/* Private function prototypes ----------------------------------------------*/


void AdpdGetPTRFactor(void);


void timer_SysTick_Handler(void)
{
    if(iscali)
    {
        calibrationcount++;
    }
    else
    {
        AdpdDrvGetParameter(ADPD_FIFOLEVEL, &nAdpdFifoLevelSize);
        if(nAdpdFifoLevelSize >= nAdpdDataSetSize)
        {
            gnAdpdDataReady = 1;
        }
    }
}

void adpd188Test(void)
{
    dataADPD sensorData;
    dataCal eFuse_Cal;
    uint32_t CalibrationWindowA[5];
    uint32_t CalibrationWindowB[5];
    uint8_t CalibrationCount=0;

//��ʼ��ADPD  �̸��ǹ�ѧ�� ������OPTICAL
    AdpdConfiguration(OPTICAL);
//ʱ��У׼ FAE�����㲻У׼Ҳûɶ�� ��Ϊ���ǲɼ������뼶��
    AdpdClockCalibration();

    float ratioRaw, ratioPTR, ratioDelta;
    float ratioThreshold = 1.5;

    float blueThreshold = 1.7;
    float irThreshold = 0.7;
    float blueRaw,irRaw;
    float bluePTR, irPTR;
    float blueDelta, irDelta;
//��ȡ ����������
    AdpdGetPTRFactor();

//����ģʽΪ����
    AdpdDrvSetOperationMode(ADPDDrv_MODE_SAMPLE);
    eFuse_Cal=eFuseCal();
// printf("Gain Cal Blue: %f,Gain Cal Blue IR: %f\r\n", eFuse_Cal.dataBlue,eFuse_Cal.dataIR );


    while(1)
    {
        if(gnAdpdDataReady==1)
        {

            gnAdpdDataReady = 0;
//ǰ��εĵ���У׼����    ����װ����Ĳ�����������Ӱ���  ���ʱ��Ҳ���б仯��
            if(CalibrationCount<5)
            {
                printf("Calibrating...\r\n");
                sensorData=AdpdDataRead(OPTICAL);
                CalibrationWindowA[CalibrationCount]=sensorData.dataSlotA;
                CalibrationWindowB[CalibrationCount]=sensorData.dataSlotB;
                CalibrationCount++;
                if(CalibrationCount==5)
                {
                    blueOffset=OffsetCalibration(CalibrationWindowA);
                    irOffset=OffsetCalibration(CalibrationWindowB);
                }
            }
            else
            {
                for(uint8_t times=0; times<nAdpdFifoLevelSize/nAdpdDataSetSize; times++)
                {
                    sensorData=AdpdDataRead(OPTICAL);

                    blueRaw = sensorData.dataSlotA;
                    irRaw = sensorData.dataSlotB;
                    ratioRaw=blueRaw/irRaw;

                    bluePTR = (int)blueRaw*bluePTRFactor;
                    irPTR = (int)irRaw*irPTRFactor;
                    ratioPTR=bluePTR/irPTR;

                    blueDelta = ((int)sensorData.dataSlotA-(int)blueOffset)*bluePTRFactor;
                    irDelta = ((int)sensorData.dataSlotB-(int)irOffset)*irPTRFactor;
                    if((blueDelta>0.5)&(irDelta>0.3))
                    {
                        ratioDelta=blueDelta/irDelta;
                    }
                    else
                    {
                        ratioDelta=1;
                    }

                    printf("PTR: %f,%f,%f\r\n", blueDelta, irDelta, ratioDelta);

                }

                if( (blueDelta>blueThreshold) && (irDelta>irThreshold) && (ratioDelta>ratioThreshold)) {
//���������ֵ�򱨾�    FAE����ֵ����1����ˮ����ܱȽϴ�  ����Ŀ����ǱȽ�С��
                    //  PlayOne();
                }
            }
        }
    }
}




void AdpdGetPTRFactor(void)
{
    bluePTRFactor = CodeToPTRFactor(SLOT_A);
    irPTRFactor =  CodeToPTRFactor(SLOT_B);
}
