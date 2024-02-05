#include "../JHAL_LowPower.h"
 /*
  * STOP唤醒后需要重新对Flash进行校准，STOP0休眠模式属于芯片内核不掉电，但是Flash会掉电的模式，唤醒后芯片会继续顺序执行，不会重新启动初始化。所以此时需要对芯片进行初始化校准。
  * */
void __attribute__((section(".indata")))FLASH_CYCLE_SET(void)
{
	 asm("push lr");
	 asm("push R4 ");
	 asm("push R3 ");
	 asm("push R2  ");
	 asm("push R1  ");
//	 asm("push R0  ");
	 asm("ld r2,#0x40200100");
	 asm("ld r1,#0x87654321");
	 asm("ST.W	[r2 + #10],r1");
	 asm("ld  r1,#0x05040302");
	 asm("ST.W	[r2 + #10],r1");										//解锁ISP    ");
	 asm("ld  r1,#0x879A43DE");
	 asm("ST.W	[r2 + #12],r1 ");
	 asm("ld r1,#0x27262524");
	 asm("ST.W	[r2 + #12],r1");										//解锁CFG"r1");

	 asm("ld r1,#0x00000018");
	 asm("st.w 	[r2],r1");													//set dwen ifen  ");

	 asm("mov	r1,#0x02");
	 asm("st.w	[r2 + #1],R1");									//开启recallen"r1");

	 asm("ld  r2,#0x10001C00");									//校准值存放地址    ");
	 asm("Trim_info_new:  ");
	 asm("LD r1,#0x00100000  ");
	 asm("Trim_info1_new:    ");
	 asm("ld.w	r3,[r1] ");
	 asm("mov r0,R3 ");
	 asm("ror r0,#16");

	 asm("not		r0,R0");
	 asm("cmp		r0,R3");
	 asm("jnz		cal_err");
	 asm("ld.w	r4,[r1 + #2]");
	 asm("mov		r0,R4");										//解锁ISP    ");
	 asm("ror		r0,#16");
	 asm("not		r0,R0");
	 asm("cmp		r0,R4");
	 asm("jnz		cal_err");										//解锁CFG"r1");

	 asm("zxt.h	r3,r3");
	 asm("lsl		r4,#16");													//set dwen ifen  ");

	 asm("xrl		r3,R4");
	 asm("st.w	[r2++],R3");									//开启recallen"r1");

	 asm("add		r1,#0x10");									//校准值存放地址    ");
	 asm("LD		r3,#0x00100030");
	 asm("cmp		r1,r3");
	 asm("JNZ		Trim_info1_new");
	 asm("Trim_Flash_new: ");
	 asm("ld		r2,#0x40200100");
	 asm("ld		r0,#0xa05f0000");

	 asm("st.w	[r2 + #1],R1");
	 asm("ld		r4,#0x10001C00");
	 asm("Trim_Flash1_new:");										//解锁CFG"r1");
	// asm
	 asm("LD		r1,#0x00000000");
	 asm("Trim_Flash2_new:");													//set dwen ifen  ");

	 asm("ld.w	r3,[r4++]");
	 asm("ST.w	[R1],R3");									//开启recallen"r1");

	 asm("mov		r3,#0x04");									//校准值存放地址    ");
	 asm("orl		r3,r3,r0");
	 asm("st.w	[r2 + #1],R3");
	 asm("st.w	[r2 + #1],R0");
	 asm("add		r1,#0x8");
	 asm("LD		r3,#0x18");
	 asm("cmp		r1,r3");

	 asm("JNZ		Trim_Flash2_new");
	 asm("Trim_End:");									//开启recallen"r1");

	 asm("add		r3,r2,#4");									//校准值存放地址    ");
	 asm("clr		[r3],#0");
	 asm("clr		[r2],#3");

	 asm("mov	r0,#0");
	 asm("jmp	cal_end");
	 asm("cal_err:");
	 asm("mov r0,#1");
	 asm("cal_end:");
	 asm("ld 		r2,#0x40200100");
	 asm("ld		r0,#0xa05f0000");
	 asm("add		r1,r2,#4");
	 asm("clr		[r1],#0");

	 asm("jnb		[r1],#1");
	 asm("add		r3,#0x02");
	 asm("st.w		[r1],R3");
	// asm
	 asm("mov	r3,#0x00");
	 asm("st.w	[r2 + #2],r3");

	 asm("ld	r3,#0x50af0007");
	 asm("st.w	[r2],r3");
	// asm
//	 asm("pop R0");
	 asm("pop R1");
	 asm("pop R2");
	 asm("pop R3");
	 asm("pop R4");
	 asm("pop lr");
	 asm("jmp lr");
}


void Goto_Stop0()
{
	static bool isFirst=true;
	if(isFirst)
	{
		BKP_Write_And_Read_Enable(TRUE); //备份域读写使能
		//3.10  使能 PM_CTL0 寄存器的 IOLATCH 位， 掉电后 IO 口工作模式保存为之前配置的形式。
		PM_IO_Latch_Enable(TRUE);
		isFirst=false;
	}


	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_STOP_0); //进入Stop0模式
//PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //使能WakeUp5引脚
//	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //上升沿触发，但该脚需要配置为模拟通道
//	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//清除标志位后要直接休眠，中间如果检测到Wakup有上升沿会导致再次置位标志位，然后导致直接进入普通Sleep模式


	asm("SLEEP" );
//flash初始化
	FLASH_CYCLE_SET();
//	PM_IO_Latch_Enable(FALSE) ;
}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: Stop模式 内存数据保留，基本外设开启
 
  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

 void enterLowPowerByStopMode(void)
{
	 Goto_Stop0();
}

