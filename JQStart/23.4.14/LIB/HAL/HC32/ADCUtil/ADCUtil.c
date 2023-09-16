
#include "adcutil.h"

#define ADCCHANNEL  2
uint8 adcUtilInit(void)
{

    stc_adc_cfg_t      stcAdcCfg;
    stc_adc_scan_cfg_t stcAdcScanCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);

    Clk_SwitchTo(ClkRCL);
    Clk_SetRCHFreq(ClkFreq24Mhz);
    Clk_SwitchTo(ClkRCH);

    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO ����ʱ��ʹ��

    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))  //ADCBGR ����ʱ��ʹ��
    {
        return Error;
    }


    Gpio_SetAnalog(2, 4, TRUE);
    Gpio_SetAnalog(2, 6, TRUE);

    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;   //BGR����ʹ��
    M0P_BGR->CR_f.TS_EN = 0x1u;    //�����¶ȴ���������ʹ������
    delay100us(1);

    stcAdcCfg.enAdcOpMode = AdcScanMode;             //ɨ�����ģʽ
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv2;            //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;
    //stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;    //�ο���ѹ:�ڲ�2.5V(avdd>3V,SPS<=200kHz)
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;          //�ο���ѹ:AVDD
    stcAdcCfg.bAdcInBufEn = FALSE;                     //��ѹ���������ʹ�ܣ�SPS�������� <=200K
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;          //ADCת���Զ���������
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);

    stcAdcScanCfg.u8AdcScanModeCh = ADC_SCAN_CH0_EN
                                    | ADC_SCAN_CH1_EN;
    stcAdcScanCfg.u8AdcSampCnt = (ADCCHANNEL)*10-1;                   //����ɨ��ת������������ͨ���ı�����6ͨ�� = 0x5+1(1��)������11+1(2��)����
    Adc_ConfigScanMode(&stcAdcCfg, &stcAdcScanCfg);
    return Ok;
}


uint16_t  u16ScanResult[ADCCHANNEL];

void AdcStart_Util()
{

    Adc_Start();

    while(FALSE == M0P_ADC->IFR_f.CONT_INTF);

    M0P_ADC->ICLR_f.CONT_INTC = 0;

    for (uint8_t u8Channel = 0; u8Channel < ADCCHANNEL; u8Channel++)
    {
        Adc_GetScanResult(u8Channel, &u16ScanResult[u8Channel]);
    }
}

void test()
{

    while(1)
    {
        AdcStart_Util();
//TODO  u16ScanResult
        delay1ms(1000);
    }
}
