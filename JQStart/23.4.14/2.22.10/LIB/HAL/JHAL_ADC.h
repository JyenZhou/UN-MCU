/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADCĬ��ͨ��HAL����
  ʹ��ADC0 �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_ADC__H__
#define  __JHAL_ADC__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        JHAL_ADC0=0,
        JHAL_ADC_Number
    } JHAL_ADC;


    typedef struct
    {
        uint32_t maxAD;
        uint32_t minAD;
        uint32_t middleAD;
    } JHAL_ADCValue;
    typedef struct
    {
        float maxVoltage;
        float minVoltage;
        float middleVoltage;
        JHAL_ADCValue adcValue;
    } JHAL_ADCInfo;

//ӳ��Ļ�׼Դ
    typedef enum {
        JHAL_ADC_ReferVoltage_BandGap,
        JHAL_ADC_ReferVoltage_VDD,
        JHAL_ADC_ReferVoltage_Vref,
    }
    JHAL_ADC_ReferSoure;
    /*ADC����Դ*/
    typedef enum {
        JHAL_ADCTriggerSoure_Software=0,
        JHAL_ADC_ReferVoltage_PWM,
    } JHAL_ADCTriggerSoure;


    typedef struct
    {   //��ѹϵ������
        float calculationCoefficient;
    } __JHAL_ADCOtherInfo;
#ifdef  	XL6600A402L6
    // ADC_CHANNEL_TEMPSENSOR;    /*!< ADC����ͨ���ڲ��¶ȴ����� FF*/
#define JHAL_ADC_CHANNEL_TEMPSENSOR 0x16
    //ADC_CHANNEL_BANDGAP ;  /*!< ADC������׼��ѹ */
#define JHAL_ADC_CHANNEL_BANDGAP       0x17
//ADC_CHANNEL_VSS  ;    /*!< ADC����ͨ��VSS */
#define JHAL_ADC_CHANNEL_VSS           0x1a
//     		ADC_CHANNEL_VREFH   ;    /*!< ADC����ͨ��Vrefh */
#define JHAL_ADC_CHANNEL_VREFH  0x1d
//		ADC_CHANNEL_VREFL;    /*!< ADC����ͨ��Vrefl */
#define JHAL_ADC_CHANNEL_VREFL      0x1e
#define JHAL_ADC_CHANNEL_VREF      JHAL_ADC_CHANNEL_VREFH
#endif



    typedef struct
    {
//�Ƿ��Ƕ�ͨ��ģʽ
        bool isMultichannelMode;
//��ͨ��ģʽ��AD���ջ�����
        JHAL_ADCInfo* adcInfosBuff;

//adcͨ�������е�ֵ0-0xff ��϶��׼���¶ȵȳ����ڲ�ͨ���� JHAL_ADC_CHANNEL_XXX ��ʾ
        u8* channels;
        //ͨ������
        u8 channelsNumber;
        JHAL_ADC_ReferSoure vref;
//��׼Դ�����̵�ѹֵ
        float vrefVoltageValue;
        JHAL_ADCTriggerSoure triggerSoure ;
//�����Ƿ��ʹ���������JHAL����ö��һ�� ʹ�����ʱ�������ø�ֵ
        u8 triggerSoureValue;

        __JHAL_ADCOtherInfo __otherInfo;
    } JHAL_ADCConfig;



    /*��ͨ�����βɼ�ģʽ��ʼ��
    isEN  true��ʼ��  false����ʼ��
    channel ��Ӧͨ��������
    length  Ҫ��ʼ��ͨ���ĸ���
    JHAL_ADC_ReferVoltage ʹ�õĲο���ѹ
    vrefValue �ο���ѹ��ֵ
    */
    bool  JHAL_adcInit(JHAL_ADC adc, JHAL_ADCConfig *config);
//��ͨ�����βɼ�  ����ֵADC��ֵ
    JHAL_ADCInfo  JHAL_adcAqcSingle(JHAL_ADC adc,u8 channel);
//�ж϶�ͨ��ת���Ƿ���� �������յ��������������´�ת��  ʵ�ʽ��������ڳ�ʼ���� dataBuff
    bool  JHAL_adcAqcMultiple (JHAL_ADC adc);

    bool  JHAL_adcDeInit(JHAL_ADC adc);

#ifdef CplusPlus
}
#endif

#endif








