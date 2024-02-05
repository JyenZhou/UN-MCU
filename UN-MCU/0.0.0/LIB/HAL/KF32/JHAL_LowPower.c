#include "../JHAL_LowPower.h"
 /*
  * STOP���Ѻ���Ҫ���¶�Flash����У׼��STOP0����ģʽ����оƬ�ں˲����磬����Flash������ģʽ�����Ѻ�оƬ�����˳��ִ�У���������������ʼ�������Դ�ʱ��Ҫ��оƬ���г�ʼ��У׼��
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
	 asm("ST.W	[r2 + #10],r1");										//����ISP    ");
	 asm("ld  r1,#0x879A43DE");
	 asm("ST.W	[r2 + #12],r1 ");
	 asm("ld r1,#0x27262524");
	 asm("ST.W	[r2 + #12],r1");										//����CFG"r1");

	 asm("ld r1,#0x00000018");
	 asm("st.w 	[r2],r1");													//set dwen ifen  ");

	 asm("mov	r1,#0x02");
	 asm("st.w	[r2 + #1],R1");									//����recallen"r1");

	 asm("ld  r2,#0x10001C00");									//У׼ֵ��ŵ�ַ    ");
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
	 asm("mov		r0,R4");										//����ISP    ");
	 asm("ror		r0,#16");
	 asm("not		r0,R0");
	 asm("cmp		r0,R4");
	 asm("jnz		cal_err");										//����CFG"r1");

	 asm("zxt.h	r3,r3");
	 asm("lsl		r4,#16");													//set dwen ifen  ");

	 asm("xrl		r3,R4");
	 asm("st.w	[r2++],R3");									//����recallen"r1");

	 asm("add		r1,#0x10");									//У׼ֵ��ŵ�ַ    ");
	 asm("LD		r3,#0x00100030");
	 asm("cmp		r1,r3");
	 asm("JNZ		Trim_info1_new");
	 asm("Trim_Flash_new: ");
	 asm("ld		r2,#0x40200100");
	 asm("ld		r0,#0xa05f0000");

	 asm("st.w	[r2 + #1],R1");
	 asm("ld		r4,#0x10001C00");
	 asm("Trim_Flash1_new:");										//����CFG"r1");
	// asm
	 asm("LD		r1,#0x00000000");
	 asm("Trim_Flash2_new:");													//set dwen ifen  ");

	 asm("ld.w	r3,[r4++]");
	 asm("ST.w	[R1],R3");									//����recallen"r1");

	 asm("mov		r3,#0x04");									//У׼ֵ��ŵ�ַ    ");
	 asm("orl		r3,r3,r0");
	 asm("st.w	[r2 + #1],R3");
	 asm("st.w	[r2 + #1],R0");
	 asm("add		r1,#0x8");
	 asm("LD		r3,#0x18");
	 asm("cmp		r1,r3");

	 asm("JNZ		Trim_Flash2_new");
	 asm("Trim_End:");									//����recallen"r1");

	 asm("add		r3,r2,#4");									//У׼ֵ��ŵ�ַ    ");
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
		BKP_Write_And_Read_Enable(TRUE); //�������дʹ��
		//3.10  ʹ�� PM_CTL0 �Ĵ����� IOLATCH λ�� ����� IO �ڹ���ģʽ����Ϊ֮ǰ���õ���ʽ��
		PM_IO_Latch_Enable(TRUE);
		isFirst=false;
	}


	PM_Low_Power_Mode_Config(PM_LOW_POWER_MODE_STOP_0); //����Stop0ģʽ
//PM_External_Wakeup_Pin_Enable(PM_PIN_WKP5,TRUE); //ʹ��WakeUp5����
//	PM_External_Wakeup_Edge_Config(PM_PIN_WKP5 , PM_TRIGGER_RISE_EDGE); //�����ش��������ý���Ҫ����Ϊģ��ͨ��
//	PM_Clear_External_Wakeup_Pin_Flag(PM_WAKEUP_EXTERNAL_PIN_WKP5);//�����־λ��Ҫֱ�����ߣ��м������⵽Wakup�������ػᵼ���ٴ���λ��־λ��Ȼ����ֱ�ӽ�����ͨSleepģʽ


	asm("SLEEP" );
//flash��ʼ��
	FLASH_CYCLE_SET();
//	PM_IO_Latch_Enable(FALSE) ;
}


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: Stopģʽ �ڴ����ݱ������������迪��
 
  * @��ע: ��
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

 void enterLowPowerByStopMode(void)
{
	 Goto_Stop0();
}

