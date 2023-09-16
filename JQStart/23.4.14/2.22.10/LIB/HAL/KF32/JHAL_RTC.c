#include "../JHAL_RTC.h"




/**
  * 描述    设置RTC时间
  * 输入   无
  * 返回   无
  */
void JHAL_rtcTimeInit()
{
	RTC_InitTypeDef RTC_INIT;//定义RTC初始化信息结构体

	RTC_Reset();	//使能备份域读写、复位RTC模块

	/********************初始化结束后开始配置*************/
	/* 设置RTC时钟源 */
	RTC_INIT.m_ClockSource = BKP_RTC_INTLF;//外部低频32.768KHz,内部低频32KHz
	/* 设置RTC时间格式 */
	RTC_INIT.m_HourFormat = RTC_HOUR_FORMAT_24;//24小时制
	/* 设置时钟 */
	RTC_INIT.m_TimeStruct.m_Hours = 17;
	/* 设置分钟 */
	RTC_INIT.m_TimeStruct.m_Minutes = 48;
	/* 设置秒钟 */
	RTC_INIT.m_TimeStruct.m_Seconds = 0;
	/* 设置AMPM */
	//	RTC_INIT.m_TimeStruct.m_AMPM = RTC_TIME_AM; //24小时制不需要设置上午、下午
	/* 设置周 */
	RTC_INIT.m_DateStruct.m_WeekDay = RTC_WEEKDAY_WEDNESDAY_DEC;
	/* 设置年 */
	RTC_INIT.m_DateStruct.m_Year = 19;
	/* 设置月 */
	RTC_INIT.m_DateStruct.m_Month = RTC_MONTH_OCTOBER_DEC;
	/* 设置日 */
	RTC_INIT.m_DateStruct.m_Day = 30;
	RTC_Configuration(RTC_TIME_FORMAT_BIN,&RTC_INIT);//声明输入参数非BCD编码格式，将时间日期写入寄存器中
	/********************初始化结束后配置结束*************/
}


/***************************************************************************
 *设置RTC闹钟
 **************************************************************************/
void Set_rtc_Alarm()
{
	RTC_AlarmTypeDef RTC_AlarmA;
	RTC_Config_Mode_Enable (TRUE);//进入配置模式

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
	RTC_Alarm_A_Enable(TRUE);//设置闹钟A使能
	RTC_Alarm_A_INT_Enable (TRUE);//设置RTC闹钟A中断使能。

	RTC_Config_Mode_Enable (FALSE);//退出配置模式,执行更新
}

/***************************************************************************
 *设置RTC中断，
 *注意 ：RTC的中断源不能直接触发内核中断。RTC中断必须以外部中断Eint17-19做为信号传输通道，才能触发内核的中断。
 *		所以，使用RTC的中断必须要同时配置Eint17-19中断。
 **************************************************************************/
void JHAL_rtcInterruptEnable()
{
	RTC_Config_Mode_Enable (TRUE);//进入RTC配置模式
	//********外部中断(EINT)配置信息结构体,开放外部int17-19中断
	EINT_InitTypeDef EINT_17to19;

	//********外部中断(EINT)配置信息结构体
	EINT_17to19.m_Mask=TRUE; //对应外部中断使能
	EINT_17to19.m_Rise=TRUE; //上升沿触发
	EINT_17to19.m_Line=INT_EXTERNAL_INTERRUPT_17; //外部中断线选择，一次配置只能写入一个中断源
	EINT_17to19.m_Fall=FALSE;  //下降沿不触发
	INT_External_Configuration (&EINT_17to19);  //写入中断源信息，包括使能或失能

	RTC_Clock_Calibration_Config((int8_t)0x0);//配置RTC时钟校正值。如果不使用校准功能，则必须写入0值。
	RTC_Time_Tick_Config(RTC_TIME_TICK_DIV_1);//配置时间节拍1秒
	RTC_Time_Tick_INT_Enable (TRUE);//设置RTC时间节拍中断使能。

	RTC_Seconds_INT_Enable(TRUE);//设置RTC秒进程中断使能
	RTC_Enable(TRUE);//设置RTC使能

	RTC_Config_Mode_Enable (FALSE);//进入配置模式

	INT_Interrupt_Enable(INT_EINT19TO17,TRUE);//开放EIE的RTS中断
	INT_All_Enable (TRUE);//开放系统总中断
}


void JHAL_rtcInit()
{
	OSC_LFCK_Source_Config(LFCK_INPUT_INTLF); //LFCK时钟源选择内部低频晶振
	OSC_LFCK_Enable(TRUE); //LFCK使能，准备为RTC提供时钟

	JHAL_rtcTimeInit(); //设置初始时间
	Set_rtc_Alarm(); //设置闹钟
	JHAL_rtcInterruptEnable();  //配置中断

}


//*****************************************************************************************
//                              INT_EINTF17_19外部中断服务函数
//*****************************************************************************************
void __attribute__((interrupt)) _EINT19TO17_exception (void)
{
	//处理外部中断线17的中断
	if(INT_Get_External_Flag(INT_EXTERNAL_INTERRUPT_17)) //RTC中断入口使用的是外部中断17
	{
		INT_External_Clear_Flag(INT_EXTERNAL_INTERRUPT_17);//需清零该标志位后才可退出中断
		if(RTC_Get_Time_Tick_INT_Flag())//节拍中断
		{
			RTC_Clear_Time_Tick_INT_Flag(); //清除节拍中断标志位
			JHAL_rtcInterruptCallBack();

		}

		if(RTC_Get_Seconds_INT_Flag()) //分钟进程中断
		{
			RTC_Clear_Seconds_INT_Flag(); //清除节拍中断标志位
		}

		if(RTC_Get_Alarm_A_INT_Flag()) //闹钟A的中断
		{
			RTC_Clear_Alarm_A_INT_Flag(); //清除闹钟A的标志位
		}
	}
}


__attribute__((weak)) void JHAL_rtcInterruptCallBack()
{
}
