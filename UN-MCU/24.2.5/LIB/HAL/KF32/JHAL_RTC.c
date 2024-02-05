#include "../JHAL_RTC.h"




/**
  * ����    ����RTCʱ��
  * ����   ��
  * ����   ��
  */
void JHAL_rtcTimeInit()
{
	RTC_InitTypeDef RTC_INIT;//����RTC��ʼ����Ϣ�ṹ��

	RTC_Reset();	//ʹ�ܱ������д����λRTCģ��

	/********************��ʼ��������ʼ����*************/
	/* ����RTCʱ��Դ */
	RTC_INIT.m_ClockSource = BKP_RTC_INTLF;//�ⲿ��Ƶ32.768KHz,�ڲ���Ƶ32KHz
	/* ����RTCʱ���ʽ */
	RTC_INIT.m_HourFormat = RTC_HOUR_FORMAT_24;//24Сʱ��
	/* ����ʱ�� */
	RTC_INIT.m_TimeStruct.m_Hours = 17;
	/* ���÷��� */
	RTC_INIT.m_TimeStruct.m_Minutes = 48;
	/* �������� */
	RTC_INIT.m_TimeStruct.m_Seconds = 0;
	/* ����AMPM */
	//	RTC_INIT.m_TimeStruct.m_AMPM = RTC_TIME_AM; //24Сʱ�Ʋ���Ҫ�������硢����
	/* ������ */
	RTC_INIT.m_DateStruct.m_WeekDay = RTC_WEEKDAY_WEDNESDAY_DEC;
	/* ������ */
	RTC_INIT.m_DateStruct.m_Year = 19;
	/* ������ */
	RTC_INIT.m_DateStruct.m_Month = RTC_MONTH_OCTOBER_DEC;
	/* ������ */
	RTC_INIT.m_DateStruct.m_Day = 30;
	RTC_Configuration(RTC_TIME_FORMAT_BIN,&RTC_INIT);//�������������BCD�����ʽ����ʱ������д��Ĵ�����
	/********************��ʼ�����������ý���*************/
}


/***************************************************************************
 *����RTC����
 **************************************************************************/
void Set_rtc_Alarm()
{
	RTC_AlarmTypeDef RTC_AlarmA;
	RTC_Config_Mode_Enable (TRUE);//��������ģʽ

	RTC_AlarmA.m_WeekDay=RTC_WEEKDAY_MONDAY_DEC;
	RTC_AlarmA.m_Hours=17;
	RTC_AlarmA.m_Minutes=48;
	RTC_AlarmA.m_Seconds=10;
	RTC_AlarmA.m_AMPM=RTC_TIME_AM;
	RTC_AlarmA.m_WeekDayEnable=FALSE;
	RTC_AlarmA.m_HoursEnable=TRUE;
	RTC_AlarmA.m_MinutesEnable=TRUE;
	RTC_AlarmA.m_SecondsEnable=TRUE;
	RTC_Alarm_Configuration (RTC_ALARM_A_ADDR_OFFSET,RTC_TIME_FORMAT_BIN,&RTC_AlarmA);
	RTC_Alarm_A_Enable(TRUE);//��������Aʹ��
	RTC_Alarm_A_INT_Enable (TRUE);//����RTC����A�ж�ʹ�ܡ�

	RTC_Config_Mode_Enable (FALSE);//�˳�����ģʽ,ִ�и���
}

/***************************************************************************
 *����RTC�жϣ�
 *ע�� ��RTC���ж�Դ����ֱ�Ӵ����ں��жϡ�RTC�жϱ������ⲿ�ж�Eint17-19��Ϊ�źŴ���ͨ�������ܴ����ں˵��жϡ�
 *		���ԣ�ʹ��RTC���жϱ���Ҫͬʱ����Eint17-19�жϡ�
 **************************************************************************/
void JHAL_rtcInterruptEnable()
{
	RTC_Config_Mode_Enable (TRUE);//����RTC����ģʽ
	//********�ⲿ�ж�(EINT)������Ϣ�ṹ��,�����ⲿint17-19�ж�
	EINT_InitTypeDef EINT_17to19;

	//********�ⲿ�ж�(EINT)������Ϣ�ṹ��
	EINT_17to19.m_Mask=TRUE; //��Ӧ�ⲿ�ж�ʹ��
	EINT_17to19.m_Rise=TRUE; //�����ش���
	EINT_17to19.m_Line=INT_EXTERNAL_INTERRUPT_17; //�ⲿ�ж���ѡ��һ������ֻ��д��һ���ж�Դ
	EINT_17to19.m_Fall=FALSE;  //�½��ز�����
	INT_External_Configuration (&EINT_17to19);  //д���ж�Դ��Ϣ������ʹ�ܻ�ʧ��

	RTC_Clock_Calibration_Config((int8_t)0x0);//����RTCʱ��У��ֵ�������ʹ��У׼���ܣ������д��0ֵ��
	RTC_Time_Tick_Config(RTC_TIME_TICK_DIV_1);//����ʱ�����1��
	RTC_Time_Tick_INT_Enable (TRUE);//����RTCʱ������ж�ʹ�ܡ�

	RTC_Seconds_INT_Enable(TRUE);//����RTC������ж�ʹ��
	RTC_Enable(TRUE);//����RTCʹ��

	RTC_Config_Mode_Enable (FALSE);//��������ģʽ

	INT_Interrupt_Enable(INT_EINT19TO17,TRUE);//����EIE��RTS�ж�
	INT_All_Enable (TRUE);//����ϵͳ���ж�
}


void JHAL_rtcInit()
{
	OSC_LFCK_Source_Config(LFCK_INPUT_INTLF); //LFCKʱ��Դѡ���ڲ���Ƶ����
	OSC_LFCK_Enable(TRUE); //LFCKʹ�ܣ�׼��ΪRTC�ṩʱ��

	JHAL_rtcTimeInit(); //���ó�ʼʱ��
	Set_rtc_Alarm(); //��������
	JHAL_rtcInterruptEnable();  //�����ж�

}


//*****************************************************************************************
//                              INT_EINTF17_19�ⲿ�жϷ�����
//*****************************************************************************************
void __attribute__((interrupt)) _EINT19TO17_exception (void)
{
	//�����ⲿ�ж���17���ж�
	if(INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_17)) //RTC�ж����ʹ�õ����ⲿ�ж�17
	{
		INT_External_Clear_Flag(INT_EXTERNAL_INTERRUPT_17);//������ñ�־λ��ſ��˳��ж�
		if(RTC_Get_Time_Tick_INT_Flag())//�����ж�
		{
			RTC_Clear_Time_Tick_INT_Flag(); //��������жϱ�־λ
			JHAL_rtcInterruptCallBack();

		}

		if(RTC_Get_Seconds_INT_Flag()) //���ӽ����ж�
		{
			RTC_Clear_Seconds_INT_Flag(); //��������жϱ�־λ
		}

		if(RTC_Get_Alarm_A_INT_Flag()) //����A���ж�
		{
			RTC_Clear_Alarm_A_INT_Flag(); //�������A�ı�־λ
		}
	}
}


__attribute__((weak)) void JHAL_rtcInterruptCallBack()
{
}
