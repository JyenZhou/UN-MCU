#include "../JHAL_WDG.h"
#include "xl_wdog.h"




void __JHAL_wdgNVIC(FunctionalState isEn)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = WDOG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = isEn;
    NVIC_Init(&NVIC_InitStructure);

}

void  JHAL_wdgFeed(JHAL_WDG  *wdg)
{

    WDOG_CountRestart(WDOG);
}

bool  JHAL_wdgClose(JHAL_WDG  *wdg)
{
    if(  wdg->__info.isOpen ) {

        SIM_SCGC_Cmd(SIM_SCGC_WDG,DISABLE);
        __JHAL_wdgNVIC(DISABLE);
        WDOG_DeInit(WDOG);
        WDOG_EnableCmd(WDOG,DISABLE);
        wdg->__info.isOpen=false;
        return true;
    }
    return false;
}


bool  JHAL_wdgOpen (JHAL_WDG   *config)
{
    if(! config->__info.isOpen ) {



        __JHAL_wdgNVIC(ENABLE);
        SIM_SCGC_Cmd(SIM_SCGC_WDG,ENABLE);
        WDOG_DeInit(WDOG);
        WDOG_EnableCmd(WDOG,DISABLE);
        WDOG_InitTypeDef WDOG_InitStructure;
        WDOG_InitStructure.WDOG_ClkSource = WDOG_Bus_Clock; //bus clockʱ��Դ 45MHZ
        WDOG_InitStructure.WDOG_PulseLength = WDOG_PclkCycles_32; //��Ƶ
        WDOG_InitStructure.WDOG_DEBUGEn = DISABLE;
        WDOG_InitStructure.WDOG_STOPEn = DISABLE;
        WDOG_InitStructure.WDOG_WAITEn = DISABLE;
        WDOG_InitStructure.WDOG_PRESEn = ENABLE;//256Ԥ��Ƶʹ��
        WDOG_InitStructure.WDOG_ResponseMode = WDOG_ResponseMode_Interrupt; //����һ���ж� WDOG_ResponseMode_Reset
        WDOG_Init(WDOG,&WDOG_InitStructure);
        // WDOG_SetTimeoutPeriod(WDOG,13);		//1S		//0~31 : 0~28 t = 2^(num + 4) 29~31 : t = 2^4 = 16
        WDOG_SetTimeoutPeriod(WDOG,20);	//�ȴ���ø���
        WDOG_CountRestart(WDOG);
        WDOG_EnableCmd(WDOG,ENABLE);
        return config->__info.isOpen=true;
    }
    return  false;
}


/** ----------------------------WDOG_IRQHandler-----------------------------------
  * ������ ��ǰ�ж�  ��Ҫʹ�����º�������ι���������κ�λ
  * WDOG_CountRestart(WDOG);
    WDOG_ClrInt(WDOG);
  * ������
**	  	 None
  *
  * ����ֵ:��
  *-----------------------------Jyen-2022-11-14-------------------------------------- */

void WDOG_IRQHandler(void)
{
    JHALwdgBeforeInterruptCallback(JHAL_WDG_GWDG);
}


__weak   void JHALwdgBeforeInterruptCallback(JHAL_WDG_Dev dev)
{
}
