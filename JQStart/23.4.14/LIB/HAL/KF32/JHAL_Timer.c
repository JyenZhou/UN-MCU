#include "../JHAL_Timer.h"

/**
  ******************************************************************************
  * �ļ���  Tz_user.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.1
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��Tzϵ�ж�ʱ���Ļ������÷���
  *
  *********************************************************************/



void JHAL_timerInterruptEnable(InterruptIndex Peripheral)
{

	INT_Interrupt_Priority_Config(Peripheral,4,0);							//��ռ���ȼ�4,�����ȼ�0
		INT_Interrupt_Enable(Peripheral,TRUE);									//�����ж�ʹ��
		INT_Clear_Interrupt_Flag(Peripheral);									//���жϱ�־
		INT_Stack_Align_Config(INT_STACK_SINGLE_ALIGN);						//�ж��Զ���ջʹ�õ��ֶ���
		INT_All_Enable (TRUE);												//ȫ�ֿ������ж�ʹ��,���ж�ʹ�ܿ��Ʋ�������λ/NMI/Ӳ�������ж�
	}

//��������������Ϊ1s�Ķ�ʱ��
void JHAL_timerInit()
{

	ATIM_SFRmap* ATIMx =T6_SFR;
			TIM_Reset(ATIMx);													//��ʱ�����踴λ��ʹ������ʱ��
			ATIM_Z_Updata_Immediately_Config(ATIMx,TRUE);						//�������¿���
			ATIM_Z_Updata_Enable(ATIMx,TRUE);									//���ø���ʹ��
			ATIM_Z_Work_Mode_Config(ATIMx,ATIM_TIMER_MODE);					//��ʱģʽѡ��
			ATIM_Z_Set_Counter(ATIMx,0);										//��ʱ������ֵ
			ATIM_Z_Set_Period(ATIMx,30000);									//��ʱ������ֵ
			ATIM_Z_Set_Prescaler(ATIMx,71);										//��ʱ��Ԥ��Ƶֵ1:16
			ATIM_Z_Postscaler_Config(ATIMx,ATIM_POSTSCALER_DIV_8);				//��Ƶ��Ϊ1:1

			ATIM_Z_Counter_Mode_Config(ATIMx,ATIM_COUNT_UP_OF);			//����-���¼���,�������������жϱ�־
			ATIM_Z_Clock_Config(ATIMx,ATIM_HFCLK);								//ѡ��HFCLKʱ��
			ATIM_Z_Overflow_INT_Enable(ATIMx,TRUE);							//��������ж�ʹ��
			ATIM_Z_Cmd(ATIMx,TRUE);											//��ʱ����������ʹ��


	JHAL_timerInterruptEnable(INT_T6);  //�����ж�

}


void __attribute__((interrupt))_T6_exception (void)
{
	if(ATIM_Z_Get_Overflow_INT_Flag(T6_SFR))
	{
		ATIM_Z_Clear_Overflow_INT_Flag(T6_SFR);
		JHAL_timerInterruptCallBack();
	}
}


__attribute__((weak)) void JHAL_timerInterruptCallBack()
{


}
