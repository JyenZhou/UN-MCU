#include "../JHAL_Timer.h"
#include "xl_pit.h"
#include "xl_sim.h"
#include "xl_nvic.h"



void __JHAL_timerInit(u8 id, JHAL_TimeUnits unti,u16 itTimeValue)
{
    JHAL_disableInterrupts();
    SIM_SOPT0_BusClockDivide(BUSCLOCK_OUTPUT_DIVIDE_128);
    SIM_SOPT0_BusClockOutputCmd(ENABLE); //bus clock out

    SIM_SCGC_Cmd(SIM_SCGC_PIT,ENABLE); //PITģ��ʹ�� ϵͳ����ģ�� SCGC = 0x2

    PIT_DeInit(); //timer = 0 ���ö�ʱ��



    if(id==0) {
        PIT_Init((uint8_t)PIT_Channel0,PIT_Count_Mode,0); //set PIT[0] �û�����ģʽ 0��Ƶ
        PIT_InterruptEn((uint8_t)PIT_Channel0,ENABLE); //�ж�ʹ��
        PIT_SetLoadCount((uint8_t)PIT_Channel0,unti/JHAL_TimeUnits_MS*48000* itTimeValue); //���� 48000  		//1ms��ʱ��
        PIT_EnableCmd((uint8_t)PIT_Channel0,ENABLE );	 // ʹ�ܶ�ʱ��

    } else if(id==1) {

        PIT_Init((uint8_t)PIT_Channel1,PIT_Count_Mode,0);//set PIT[1]
        PIT_InterruptEn((uint8_t)PIT_Channel1,ENABLE);
        PIT_SetLoadCount((uint8_t)PIT_Channel1,unti/JHAL_TimeUnits_MS*48000* itTimeValue);
        PIT_EnableCmd((uint8_t)PIT_Channel1,ENABLE );


    } else {
        while(true);
    }

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //
    NVIC_InitStructure.NVIC_IRQChannel = (  id==0)?(uint8_t)PIT_CH0_IRQn: (uint8_t)PIT_CH1_IRQn;  //�ж�ͨ�����жϺ�->�жϺ���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);


    JHAL_enableInterrupts();

}





bool JHAL_timerOpen(JHAL_Timer   *config)
{
    if(!config->__info. isOpen) {

        __JHAL_timerInit(config->id,config->itTimeUnit,config->itTimeValue); //���ó�ʼʱ��

        return  config->__info.isOpen=  true;
    }
    return  false;
}

bool  JHAL_timerClose(JHAL_Timer *timer)
{
    if( timer->__info. isOpen) {

        PIT_DeInit(); //timer = 0 ���ö�ʱ��
        timer->__info.isOpen=false;
        return true;
    }
    return  false;
}






/**
 * @brief  PITͨ��0�жϴ�����
 */
void PIT_CH0_IRQHandler(void)
{

    JHAL_timerInterruptCallback(0);
    PIT_ClrInterrupt((uint8_t)PIT_Channel0); //���PIT�ж�
}

/**
 * @brief  PITͨ��1�жϴ�����
 */
void PIT_CH1_IRQHandler(void)
{

    JHAL_timerInterruptCallback(1);
    PIT_ClrInterrupt((uint8_t)PIT_Channel1); //���PIT�ж�
}



