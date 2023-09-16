
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

    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO 外设时钟使能

    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))  //ADCBGR 外设时钟使能
    {
        return Error;
    }


    Gpio_SetAnalog(2, 4, TRUE);
    Gpio_SetAnalog(2, 6, TRUE);

    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;   //BGR必须使能
    M0P_BGR->CR_f.TS_EN = 0x1u;    //内置温度传感器，视使用需求
    delay100us(1);

    stcAdcCfg.enAdcOpMode = AdcScanMode;             //扫描采样模式
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv2;            //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;
    //stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;    //参考电压:内部2.5V(avdd>3V,SPS<=200kHz)
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;          //参考电压:AVDD
    stcAdcCfg.bAdcInBufEn = FALSE;                     //电压跟随器如果使能，SPS采样速率 <=200K
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;          //ADC转换自动触发设置
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);

    stcAdcScanCfg.u8AdcScanModeCh = ADC_SCAN_CH0_EN
                                    | ADC_SCAN_CH1_EN;
    stcAdcScanCfg.u8AdcSampCnt = (ADCCHANNEL)*10-1;                   //连续扫描转换次数，保持通道的倍数，6通道 = 0x5+1(1倍)，或者11+1(2倍)……
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
