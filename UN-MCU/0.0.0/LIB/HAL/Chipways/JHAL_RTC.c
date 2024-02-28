#include "../JHAL_RTC.h"
#include "xl_rtc.h"
#include <xl_ics.h>
/**
 * @param  RTC_SOURCE_CLK  RTCʱ��Դѡ��
 *		@arg 0: bus clock
 *		@arg 1���ڲ���׼ʱ��
 *		@arg 2���ⲿ����
 *		@arg 3���͹�������
 */
#define RTC_SOURCE_CLK  1

/**
  * ����    ����RTCʱ��
  * ����   ��
  * ����   ��
  */

void __JHAL_rtcTimeInit(RTC_Type *RTCx,JHAL_TimeUnits type,u16 itTimeValue)
{
    RTC_InitTypeDef RTC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = (uint8_t)RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SIM_SCGC_Cmd(SIM_SCGC_RTC,ENABLE);  //��RTCģ������ʱ��


#if (RTC_SOURCE_CLK == 1)
    {
        ICS_IRCEnableCmd(ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_Internal_Clock;  // 100/150KHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //��Ƶ 64
        RTC_Init(RTCx,&RTC_InitStructure);  //��ʼ��
        if(type==JHAL_TimeUnits_S) {

            while(3000*itTimeValue>65535);
            RTC_SetModValue(RTCx, 2363*itTimeValue);		//ģ����count ~1S
        } else if(type==JHAL_TimeUnits_MS) {
            while(((2363*itTimeValue)/1000)>65535);
            RTC_SetModValue(RTCx, (2363*itTimeValue)/1000);		//ģ����count ~1MS

        } else {
            while(true);
        }




    }
#elif (RTC_SOURCE_CLK == 2)
    {
        OSC_InitTypeDef OSC_InitStructure;

        OSC_InitStructure.OSC_OutputSelect = OSC_OSCCILLATOR_COLOR;
        OSC_InitStructure.OSC_RangeSelect = OSC_RANGE_HIGH;
        OSC_InitStructure.OSC_ModeSelect = OSC_HG_MODE;
        OSC_Init(&OSC_InitStructure);

        OSC_EnableCmd(ENABLE);

        SIM_SCGC1_Cmd(SIM_SCGC1_RTCEC,ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_External_Clock;  //24MHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //��Ƶ 64
        RTC_Init(RTCx,&RTC_InitStructure);  //��ʼ��
        RTC_SetModValue(RTCx, 37000);		//ģ����count 100ms
    }
#elif (RTC_SOURCE_CLK == 3)
    {
        SIM_SCGC1_Cmd(SIM_SCGC1_RTCLPOC,ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_Realtime_Clock;  //32KHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_32or100; //����Ƶ
        RTC_Init(RTCx,&RTC_InitStructure);  //��ʼ��
        RTC_SetModValue(RTCx, 320);		//ģ����count
    }
#else //Bus clock
    {
        RTC_InitStructure.RTC_ClkSrc = RTC_Bus_Clock;  //ʹ��busʱ��Դ 48MHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //��Ƶ 1000
        RTC_Init(RTCx,&RTC_InitStructure);  //��ʼ��
        RTC_SetModValue(RTCx, 48000);		//ģ����count 1S
    }
#endif



}



/***************************************************************************
 *����RTC�жϣ�
 *ע�� ��RTC���ж�Դ����ֱ�Ӵ����ں��жϡ�RTC�жϱ������ⲿ�ж�Eint17-19��Ϊ�źŴ���ͨ�������ܴ����ں˵��жϡ�
 *		���ԣ�ʹ��RTC���жϱ���Ҫͬʱ����Eint17-19�жϡ�
 **************************************************************************/
void __JHAL_rtcInterruptEnable(RTC_Type *RTCx)
{
    RTC_InterruptEn(RTCx,ENABLE );		//ʹ���ж�
    RTC_EnableCmd(RTCx, ENABLE);	//RTC ʹ��
    JHAL_enableInterrupts();

}



bool JHAL_rtcOpen( JHAL_RTC  *config)
{
    if(!config->__info.isOpen) {
        __JHAL_rtcTimeInit(RTC,config->itTimeUnit,config->itTimeValue); //���ó�ʼʱ��
        __JHAL_rtcInterruptEnable(RTC);  //�����ж�


        return   config->__info.isOpen=true;
    }
    return false;
}
bool  JHAL_rtcClose(JHAL_RTC *config)
{
    if( config->__info.isOpen) {
        //��RTCģ������ʱ��  ��Ϊ�͹��ĺ���ܻ�ر����ʱ��  ��ʱ�������Ч
        SIM_SCGC_Cmd(SIM_SCGC_RTC,ENABLE);
        RTC_DeInit(RTC);


        config->__info.isOpen=false;
        return true;
    }
    return false;

}

/**
 * @brief  RTC �жϺ���
 * @param
 * @retval �޷���ֵ
 */
void RTC_IRQHandler(void)
{

    RTC_ClrInterruptFlag(RTC); //����ж�
    JHAL_rtcInterruptCallback(0);

}



__attribute__((weak)) void JHAL_rtcInterruptCallback(u8 dev)
{
}
