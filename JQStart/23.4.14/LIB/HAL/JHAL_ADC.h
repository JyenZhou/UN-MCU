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



    typedef struct
    {
        int32 maxAD;
        int32 minAD;
        int32 ad;
    } JHAL_ADCValue;
    typedef struct
    {
        double maxVoltage;
        double minVoltage;
        double voltage;
        JHAL_ADCValue adcValue;
    } JHAL_ADCInfo;

//ӳ��Ļ�׼Դ
    typedef enum {
        JHAL_ADC_ReferVoltage_BandGap,
        JHAL_ADC_ReferVoltage_VDD,
        JHAL_ADC_ReferVoltage_Vref,
        JHAL_ADC_ReferVoltage_NONE
    }
    JHAL_ADC_ReferSoure;
    /*ADC����Դ*/
    typedef enum {
        JHAL_ADCTriggerSoure_Software,
        JHAL_ADC_ReferVoltage_PWM,
    } JHAL_ADCTriggerSoure;


    typedef struct
    {   //��ѹϵ������
        float calculationCoefficient;
        //dma������ȡʱ���ܴ��ڵ�ת��ֵ
        void  *convertedValue ;

        bool isOpen : 1;

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
    typedef enum
    {
        //ȥ��ֵƽ���˲�  Ҳ����˵������Ҫ3�����ϵ�ֵ�Żᱻƽ��
        JHAL_FilteredModel_DeExtremalAverage,
        //��ֵ�˲�
        JHAL_FilteredModel_Median
    } JHAL_FilteredMode   ;

    ;

    typedef struct
    {
        u8 dev :3;
//�Ƿ��Ƕ�ͨ��ģʽ
        bool isMultichannelMode:1;
        //ͨ������  t���ʹ��bangdgap�¶����ҲҪ����
        u8 channelsNumber:4;

        //�����Ƿ��ʹ���������JHAL����ö��һ�� ʹ������ʱ�������ø�ֵ
        u8 triggerSoureValue:4;
        //�˲��õĲ�������  0�������˲�
        u8 samplingCount:4;

//��ͨ��ģʽ��AD���ջ�����
        JHAL_ADCInfo* adcInfosBuff;

//adcͨ�������е�ֵ0-0xff ��϶��׼���¶ȵȳ����ڲ�ͨ���� JHAL_ADC_CHANNEL_XXX ��ʾ
        u8* channels;

        JHAL_ADC_ReferSoure vref;
//��׼Դ�����̵�ѹֵ
        float vrefVoltageValue;
        //ADC����Դ
        JHAL_ADCTriggerSoure triggerSoure ;
        //�˲�ģʽ
        JHAL_FilteredMode  filteredModel;


        __JHAL_ADCOtherInfo __info;
    } JHAL_ADC;



    /*��ͨ�����βɼ�ģʽ��ʼ��
    isEN  true��ʼ��  false����ʼ��
    channel ��Ӧͨ��������
    length  Ҫ��ʼ��ͨ���ĸ���
    JHAL_ADC_ReferVoltage ʹ�õĲο���ѹ
    vrefValue �ο���ѹ��ֵ
    */
    bool  JHAL_adcOpen(  JHAL_ADC *adc);
//��ͨ�����βɼ�  ����ֵADC��ֵ
    JHAL_ADCInfo  JHAL_adcAqcSingle(JHAL_ADC *adc,u8 channel);
//�ж϶�ͨ��ת���Ƿ���� �������յ��������������´�ת��  ʵ�ʽ��������ڳ�ʼ���� dataBuff
    bool  JHAL_adcAqcMultiple (JHAL_ADC *adc);

    bool  JHAL_adcClose(JHAL_ADC *adc  );

#ifdef CplusPlus
}
#endif

#endif







