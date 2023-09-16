#include "../JHAL_Timer.h"
#include  "asm32lx003_timer.h"


 

//���������
#define __Timer_Number  2


//���ص���id ��UARTX��Ӧ
u8 __JHAL_jtimer2Id(JHAL_Timer timer)
{
    if(timer==JHAL_Timer0)
    {
        return 0;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

TIM_TypeDef * __JHAL_jtimer2Timer(JHAL_Timer timer)
{
    if(timer==JHAL_Timer0)
    {
        return TIM10;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

TIM_TypeDef* __JHAL_id2Timer(u8 id)
{
    if(id==0)
    {
        return  TIM10;
    } 
else {

//�����ڻ�δʵ��
        while(true);
    }
}
 

  
void __JHAL_timerInit(JHAL_Timer jtimer, JHAL_TimeUnits unti,u16 itTimeValue)
{
 TIM_TypeDef* timer  =  __JHAL_jtimer2Timer( jtimer);

     	/* ��ʱ����ʼ�� */
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_Prescale_DIV0;//
	TIM_TimeBaseInitStruct.TIM_TMROS = TIM_Counter_TMROS_WRAPPING;
	TIM_TimeBaseInitStruct.TIM_TMRSZ = TIM_Counter_TMRSZ_32BIT;       // ������ֵ32λ
	TIM_TimeBaseInitStruct.TIM_TMRMS = TIM_Counter_TMRMS_PERIODIC;    // �Զ���װ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CT_TIMER;  // ��ʱ��ģʽ
	TIM_TimeBaseInitStruct.TIM_GATE = TIM_GATE_DISABLE;     // ���ſؿ���
	TIM_TimeBaseInitStruct.TIM_GATE_Polarity = TIM_GATE_Polarity_High;
	TIM_TimeBaseInit(timer, &TIM_TimeBaseInitStruct);
	
  	/* �������ؼ����� */
	TIM_SetTimerLoadRegister(timer, 0xFFFFFFFF - 1600000); //100mS
	//��ʱʱ�� = ����ֵ *2 / ��ϵͳʱ�� / ��Ƶϵ����

	/* ��ʱ���ж����� */
	TIM_ITConfig(timer, ENABLE);

	/* TIM NVIC�ж����� */
    NVIC_SetPriority(TIM10_IRQn, 1);
    NVIC_EnableIRQ(TIM10_IRQn);

	/* ʹ��TOG���� */
	TIM_TogCmd(timer, ENABLE);  

	/* ������ʱ�� */
	TIM_Cmd(timer, ENABLE);

}




bool __JHAL_timerIsENCheak(JHAL_Timer jtimer, bool isEN) {
	u8 id=__JHAL_jtimer2Id(jtimer);
    static bool sEN[__Timer_Number]= {false};
    if(sEN[id]!=isEN)
    {
        sEN[id]=isEN;
        return true;
    }
    return  false;
}

void JHAL_timerInit(JHAL_Timer timer,JHAL_TimerConfig config)
{
    if(__JHAL_timerIsENCheak(timer,true)) {
        __JHAL_timerInit(timer,config.itTimeUnit,config.itTimeValue); //���ó�ʼʱ��
    }
}
void  JHAL_timerDeInit(JHAL_Timer timer)
{
    if(__JHAL_timerIsENCheak(timer,false)) {
        TIM_DeInit(__JHAL_jtimer2Timer(timer));
    }
}






 
void TIM10_IRQHandler(void)
{
  	TIM_ClearITFlag(TIM10, TIM_IT_FLAG);
    JHAL_timerInterruptCallBack(JHAL_Timer0);
}

 

__attribute__((weak)) void JHAL_timerInterruptCallBack(JHAL_Timer timer)
{
	
	
}
