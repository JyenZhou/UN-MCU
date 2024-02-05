#include "../JHAL_Timer.h"

/**
  ******************************************************************************
  * 文件名  Tz_user.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.1
  * 日  期  2019-11-16
  * 描  述  该文件提供了Tz系列定时器的基本配置方法
  *
  *********************************************************************/



void JHAL_timerInterruptEnable(InterruptIndex Peripheral)
{

	INT_Interrupt_Priority_Config(Peripheral,4,0);							//抢占优先级4,子优先级0
		INT_Interrupt_Enable(Peripheral,TRUE);									//外设中断使能
		INT_Clear_Interrupt_Flag(Peripheral);									//清中断标志
		INT_Stack_Align_Config(INT_STACK_SINGLE_ALIGN);						//中断自动堆栈使用单字对齐
		INT_All_Enable (TRUE);												//全局可屏蔽中断使能,该中断使能控制不包含复位/NMI/硬件错误中断
	}

//计算出来大概周期为1s的定时器
void JHAL_timerInit()
{

	ATIM_SFRmap* ATIMx =T6_SFR;
			TIM_Reset(ATIMx);													//定时器外设复位，使能外设时钟
			ATIM_Z_Updata_Immediately_Config(ATIMx,TRUE);						//立即更新控制
			ATIM_Z_Updata_Enable(ATIMx,TRUE);									//配置更新使能
			ATIM_Z_Work_Mode_Config(ATIMx,ATIM_TIMER_MODE);					//定时模式选择
			ATIM_Z_Set_Counter(ATIMx,0);										//定时器计数值
			ATIM_Z_Set_Period(ATIMx,30000);									//定时器周期值
			ATIM_Z_Set_Prescaler(ATIMx,71);										//定时器预分频值1:16
			ATIM_Z_Postscaler_Config(ATIMx,ATIM_POSTSCALER_DIV_8);				//分频比为1:1

			ATIM_Z_Counter_Mode_Config(ATIMx,ATIM_COUNT_UP_OF);			//向上-向下计数,上溢和下溢产生中断标志
			ATIM_Z_Clock_Config(ATIMx,ATIM_HFCLK);								//选用HFCLK时钟
			ATIM_Z_Overflow_INT_Enable(ATIMx,TRUE);							//计数溢出中断使能
			ATIM_Z_Cmd(ATIMx,TRUE);											//定时器启动控制使能


	JHAL_timerInterruptEnable(INT_T6);  //配置中断

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
