#ifndef __xxddq_H
#define __xxddq_H

/*
 * @Project      : СС���������������
 * @Descripttion : ������ѡ��8/16/32�汾����ԭ���Ļ����Ͻ�һ����ǿ
 * @version      : 1.2.3 ���ϰ�
 * @Author       : smset
 * @Date         : 2015-06-02 09:02:31
 * @LastEditors  : ����
 * @LastEditTime : 2020-11-06 19:48:20
 * @History      :
 * ԭ��СС��������Դ��ַ��https://github.com/smset028/xxddq
 * СС����������QQȺ��371719283
 *
 * 2020-11-06
 * 1������ #define WAITUTILTRTYPE 0 	//WaitUtilTR ��ʱ�ȴ��ж�����ѡ���
 *    0=��,WaitUtilTR(A,TimeOut,TINDEX) TINDEX:��ʱ�ȴ�����ֻ��0-7,ÿ��WaitUtilTR�����������ظ���ֻ֧��8��ʵʱ�Ըߵĳ�ʱ�ж�;
 *	  1=��� �Զ��������WaitUtilTR(A,TimeOut,T1FLAY) T1FLAY:1��������ʱ�����ڱ�־����,���ڵ�����ʱ�����ڶ�ʱ���ж��ｫ������1��
 *   �򵥵���ʾ����WaitUtilTR(f.b0,2,0);
 *
 * 2020-11-05
 * 1������Ⱥ����smset����������޸ġ�_count������ֵ��λ�ã�ʹ֮�����
 * 2���޸ġ�WaitUtilTR(A,TimeOut)��WAITUNTILTR(A,VAL,TimeOut)�����壬���ʵʱ�Ըߵ��жϵȴ���ʱʱ�䲻׼������
 *    ���ӡ�T1FLAY��һ�����������ڱ�־������������Ϊ��WaitUtilTR(A,TimeOut,T1FLAY)��WAITUNTILTR(A,VAL,TimeOut,T1FLAY)��
 *    ʹ�÷�ʽ��ο�����
	//����һ�����������ڱ�־���������������������������ṹ��Ӧ���ں��C51�������������BUG��
	//����ʵʱ�ԸߵĴ���ʱ�жϵĵȴ��жϣ�
	//�ڸ��µ������Ķ�ʱ���ж������ ��T1F.bs=0xFF;��
	//�������8����ʵʱ�Ըߵ��жϣ����޸ġ�T1F.bs��Ϊ16λ�����n����unsigned bn: 1;�� ����
	union
	{
	    struct
	    {
	        unsigned b0: 1;
	        unsigned b1: 1;
	        unsigned b2: 1;
	        unsigned b3: 1;
	        unsigned b4: 1;
	        unsigned b5: 1;
	        unsigned b6: 1;
	        unsigned b7: 1;
	    } b;
	    unsigned char bs;
	} T1F;  //һ�����������ڱ�־

 	//����0
	TASK task0(void)
	{
	    _SS
	    while(1)
	    {
	        //��ʵʱ�Եĵȴ�����f.b0==1����ʱ2������������
	        //f.b0:�ȴ�����������;
	        //2:��ʱ2��������ʱ������;
	        //T1F.b.b0:��ǰ��ʱ������ʱ�����ڱ�־λ�������ظ���
	        //��һ���봫�롰T1F.b.b1�����Դ����ƣ�
	        WaitUtilTR(f.b0, 2,T1F.b.b0); //��ʵʱ�Եĵȴ�����f.b0==1����ʱ2������������
	        if (f.b0 == 0) //��ʱִ��
	        {
	            LED0 ^= 1;  //��תLED0
	        }
	        else    //��������ִ��
	        {
	            LED1 ^= 1;
	        }
	    }
	    _EE
	}
	//��ʱ��0�ж�
	void INTT0(void) interrupt 1 using 1
	{
	    //10ms
	    TL0 = 0Xff;
	    TH0 = 0XDB;
	    UpdateTimers(); //��������
	    T1F.bs=0xFF;    //��һ��������ʱ�����ڱ�־����λȫ��1 ����
	    // RunTask(task0, 0); //�߼����������ж���ִ�У��ɰ���ͼ�������
	}
 *
 * 2020-11-04
 * 1���޸���ʱ�ж�ֻ����Ч1�ε�BUG��Ⱥ��"������ˮ"���֣�
 *    ��WaitUtilT(A,TimeOut)��WAITUNTILT(A,VAL,TimeOut)��WaitUtilTR(A,TimeOut)��WAITUNTILTR(A,VAL,TimeOut)���������ӡ�_count=TimeOut;��
 *
 * 2020-10-15
 * 1������SETJMP��.��ͨ������SETJMP�л�
 *    #define SETJMP 0            //�汾ѡ��:0=SWINCH�� 1=SETJMP��

 * 2020-10-14
 * 1������LCBIT�궨��������_lc��������.
 *	  #define LCBIT 0			//ѡ��_lc�ı�������: 0=8λ;1=16λ.
 *	  PICC�߰汾�������겻֧��__LINE__%255����,LCBIT����������Ϊ1�����۾���ÿ�������ռһ���ֽڵ�RAM.
 *    ����������������������� LCBIT ����Ϊ0,��ԭ��һ��.
 *
 * 2020-07-03
 * 1������ ��ͨ������������ʱ���غ궨�壬�����ڲ�ʹ����ͨ������������ʱ����ʱ����RAMռ�ú͸��������ʱ�䣺
 *     NORMALDELAY  Ĭ��Ϊ0�رգ��޸�Ϊ1���
 *
 * 2020-04-17
 *  1��ͬ��smset��СС������1.11��16λ��ʱ���棩,�����ĵȴ�ĳ��������ʱ���
 *      WaitUtilT(A,TimeOut)
 *  2����smset��WaitUtilT(A,TimeOut)������,���ӵȴ�ĳ������Ϊĳ��ֵ��ʱ���
 *      WAITUNTILT(A,VAL,TimeOut)
 *
 * 2019-05-30
 * 1�����ӵȴ�ĳ������Ϊĳ��ֵ��ʱ���
 *     A��������;VAL����ֵ;TaskVAL��ʱֵ������Ϊ0����TaskVAL��λ�������������λ��һ��;ONLY��ʱ�ж�Ψһ���
 *     WAITUNTILO(A,VAL,TaskVAL,ONLY)
 *     ÿ����һ����ʱ�жϻ�����һ��������λ����Ӧ�ڴ�ռ��
 * 2����� void runTasks(void); ������ȷ�����ֱ��������ᱨ��
 *
 * 2019-05-29
 *  1������Ⱥ��semet����ͨ������������ʱ
 *  2����ӱ���λ�����Ͷ��壬������ݱ��������������޸�
 *  ע�⣺��������ʱ ֻ�ܱ��������Լ���������õĺ���������������õĺ������õĺ���ʹ��
 */


/****СС��������ʼ********************************************/

#include "JSYS.h"
//�ɸ��ݱ����������޸Ķ������λ�����ͣ���������ظ����������ʾ������ע�͵��������Ͷ���

#define MAXTASKS XXOS_MAXTASK         //�������������������1-255
#define BITS 16             //���������λ����ѡֵ��8/16/32
#define SETJMP 0            //�汾ѡ��:0=SWINCH�� 1=SETJMP��
#define NORMALDELAY 0       //��ͨ������������ʱ���أ�0=�أ�1=��
#define WAITUTILTRTYPE 0 	//WaitUtilTR ��ʱ�ȴ��ж����ͣ� 
//0=��,WaitUtilTR(A,TimeOut,TINDEX) TINDEX:��ʱ�ȴ�����ֻ��0-7,ÿ��WaitUtilTR�����������ظ�;
//1=����Զ��������WaitUtilTR(A,TimeOut,T1FLAY) T1FLAY:1��������ʱ�����ڱ�־����,���ڵ�����ʱ�����ڶ�ʱ���ж��ｫ������1��
#define SEM U16    			//�ź��������

#define LCBIT 0			//ѡ��_lc�ı�������: 0=8λ;1=16λ.SETJMP�������Ч

#if (BITS==8)
#define TASK U8
#define TICKET_MAX 0xFF
#endif

#if (BITS==16)
#define TASK U16
#define TICKET_MAX 0xFFFF
#endif

#if (BITS==32)
#define TASK U32
#define TICKET_MAX 0xFFFFFFFF
#endif

extern volatile TASK timers[MAXTASKS];
extern U8 taskWhileTrue;
#if (NORMALDELAY==1)
volatile TASK delaycount;
void runTasks(void);
#endif
#if (WAITUTILTRTYPE==0)

extern U8 T1FS;
#endif
//**********     SWINCH��-��ʼ    **********************************//
#if (SETJMP==0)
//����ͷ
#if (LCBIT==0)
#define _SS static U8 _lc=0; switch(_lc){default:
#endif
#if (LCBIT==1)
#define _SS static U16 _lc=0; switch(_lc){default:
#endif
//����β
#define _EE ;}; _lc=0; return TICKET_MAX;
//�ȴ�X��ʱ������
#if (LCBIT==0)
#define WaitX(tickets)  do { _lc=(__LINE__&255)+1; return (tickets) ;case (__LINE__&255)+1:;} while(0);
#endif
#if (LCBIT==1)
#define WaitX(tickets)  do { _lc=__LINE__; return (tickets) ;case __LINE__:;} while(0);
#endif
//�ȴ�ĳ������
#define WaitUntil(A)    do { while(!(A)) WaitX(1);} while(0);
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������
#define WaitUntilR(A)    do { while(!(A)) WaitX(0);} while(0);
//�ȴ�ĳ������Ϊĳ��ֵ	A��������;VAL����ֵ
#define WAITUNTIL(A,VAL)  do { WaitX(1);} while((A)!=VAL);
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ	A��������;VAL����ֵ
#define WAITUNTILR(A,VAL)  do { WaitX(0);} while((A)!=VAL);
//�ȴ�ĳ��������ʱ���
#define WaitUtilT(A,TimeOut)  do {static TASK _count; _count=(TimeOut); do { WaitX(1); _count--; } while((!(A))&&(_count>0));} while(0);
#if (WAITUTILTRTYPE==1)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ��������ʱ��� A��������;TimeOut��ʱֵ;T1FLAYһ�����������ڱ�־���� ʹ�÷����ο�����
#define WaitUtilTR(A,TimeOut,T1FLAY)  do {static TASK _count; _count=(TimeOut); do { WaitX(0); if(T1FLAY){_count--;T1FLAY=0;}} while((!(A))&&(_count>0));} while(0);
#endif
#if (WAITUTILTRTYPE==0)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ��������ʱ��� A��������;TimeOut��ʱֵ;TINDEXʵʱ�ж�������ȡֵ��Χ 0-7��ÿ���жϵ����������ظ������֧��8��ʵʱ�ж�
#define WaitUtilTR(A,TimeOut,TINDEX)  do {static TASK _count; _count=(TimeOut); do { WaitX(0); if(T1FS&(1<<(TINDEX))){_count--;T1FS&=~(1<<(TINDEX));}} while((!(A))&&(_count>0));} while(0);
#endif
//�ȴ�ĳ������Ϊĳ��ֵ��ʱ���
#define WAITUNTILT(A,VAL,TimeOut)  do {static TASK _count; _count=(TimeOut); do { WaitX(1); _count--; } while(((A)!=VAL)&&(_count>0));} while(0);
#if (WAITUTILTRTYPE==1)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ��ʱ��� A��������;VAL����ֵ;TimeOut��ʱֵ;T1FLAYһ�����������ڱ�־���� ʹ�÷����ο�����
#define WAITUNTILTR(A,VAL,TimeOut,T1FLAY)  do {static TASK _count; _count=(TimeOut); do { WaitX(0); if(T1FLAY){_count--;T1FLAY=0;}} while(((A)!=VAL)&&(_count>0));} while(0);
#endif
#if (WAITUTILTRTYPE==0)
// ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ��������ʱ��� A��������;VAL����ֵ;TimeOut��ʱֵ;TINDEXʵʱ�ж�������ȡֵ��Χ 0-7��ÿ���жϵ����������ظ������֧��8��ʵʱ�ж�
#define WAITUNTILTR(A,VAL,TimeOut,TINDEX)  do {static TASK _count; _count=(TimeOut); do { WaitX(0); if(T1FS&(1<<(TINDEX))){_count--;T1FS&=~(1<<(TINDEX));}} while(((A)!=VAL)&&(_count>0));} while(0);
#endif
//��������
#define RunTask(TaskName,TaskID)  do { if (timers[TaskID]==0) {TASK d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;} } while(0);
//��������ǰ����������ȱ�ִ֤��
#define RunTaskA(TaskName,TaskID) do { if (timers[TaskID]==0) {TASK d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;continue;} } while(0);
//����������
#if (LCBIT==0)
#define CallSub(SubTaskName) do {TASK currdt; _lc=(__LINE__%255)+1; return 0; case (__LINE__%255)+1:  currdt=SubTaskName(); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
#if (LCBIT==1)
#define CallSub(SubTaskName) do {TASK currdt; _lc=__LINE__; return 0; case __LINE__:  currdt=SubTaskName(); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
//����������,����һ������,����Ĳ��������ͷ�CPUǰʹ��
#if (LCBIT==0)
#define CallSub1(SubTaskName,p1) do {TASK currdt; _lc=(__LINE__%255)+1; return 0; case (__LINE__%255)+1:  currdt=SubTaskName(p1); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
#if (LCBIT==1)
#define CallSub1(SubTaskName,p1) do {TASK currdt; _lc=__LINE__; return 0; case __LINE__:  currdt=SubTaskName(p1); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
//����������,�����������,����Ĳ��������ͷ�CPUǰʹ��
#if (LCBIT==0)
#define CallSub2(SubTaskName,p1,p2) do {TASK currdt; _lc=(__LINE__%255)+1; return 0; case (__LINE__%255)+1:  currdt=SubTaskName(p1,p2); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
#if (LCBIT==1)
#define CallSub2(SubTaskName,p1,p2) do {TASK currdt; _lc=__LINE__; return 0; case __LINE__:  currdt=SubTaskName(p1,p2); if(currdt!=TICKET_MAX) return currdt;} while(0);
#endif
//��ʼ���������
#define InitTasks() do {U8 i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; } while(0);

#if (WAITUTILTRTYPE==0)
#define ST1FS (T1FS=0xFF)
#endif
#if (WAITUTILTRTYPE==1)
#define ST1FS
#endif

#if (NORMALDELAY==0)
//��������
#define UpdateTimers() do{U8 i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=TICKET_MAX)) timers[i-1]--;} ST1FS;} while(0);
#endif
#if (NORMALDELAY==1)
//��������
#define UpdateTimers() do{U8 i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=TICKET_MAX)) timers[i-1]--;} if(delaycount>0) delaycount--; ST1FS;} while(0);
//��ͨ������������ʱ ֻ�ܱ��������Լ���������õĺ���������������õĺ������õĺ��� ʹ��
#define DELAYX(tickets) do{delaycount=tickets;while(delaycount>0) {runTasks();}}while(0);
#endif

#endif
//**********     SWINCH��-����    **********************************//

//**********     SETJMP��-��ʼ    **********************************//
#if (SETJMP==1)
#include <setjmp.h>
//����ͷ
#define _SS static jmp_buf env; static TASK started; if(started) longjmp(env,1); started=1;
//����β
#define _EE started=0; return TICKET_MAX;
//�ȴ�X������������
#define WaitX(tickets) do { if(setjmp(env)==0) return tickets;} while(0);
//�ȴ�ĳ������
#define WaitUntil(A) do { WaitX(1);} while((A)==0);
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ	A��������
#define WaitUntilR(A) do { WaitX(0);} while((A)==0);
//�ȴ�ĳ������Ϊĳ��ֵ  A��������;VAL����ֵ
#define WAITUNTIL(A,VAL)  do { WaitX(1);} while((A)!=VAL);
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ	A��������;VAL����ֵ
#define WAITUNTILR(A,VAL) do { WaitX(0);} while((A)!=VAL);
//�ȴ�ĳ��������ʱ���
#define WaitUtilT(A,TimeOut)  do {static TASK _count; _count=(TimeOut); do { WaitX(1); _count--; } while((!(A))&&(_count>0));} while(0);
#if (WAITUTILTRTYPE==1)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ��ʱ��� A��������;TimeOut��ʱֵ;T1FLAYһ�����������ڱ�־���� ʹ�÷����ο�����
#define WaitUtilTR(A,TimeOut,T1FLAY) do {static TASK _count; _count=(TimeOut); do { WaitX(0);if(T1FLAY){_count--;T1FLAY=0;}} while((!(A))&&(_count>0));} while(0);
#endif
#if (WAITUTILTRTYPE==0)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ��������ʱ��� A��������;TimeOut��ʱֵ;TINDEXʵʱ�ж�������ȡֵ��Χ 0-7��ÿ���жϵ����������ظ������֧��8��ʵʱ�ж�
#define WaitUtilTR(A,TimeOut,TINDEX) do {static TASK _count; _count=(TimeOut); do { WaitX(0);if(T1FS&(1<<(TINDEX))){_count--;T1FS&=~(1<<(TINDEX));}} while((!(A))&&(_count>0));} while(0);
#endif
//�ȴ�ĳ������Ϊĳ��ֵ��ʱ���
#define WAITUNTILT(A,VAL,TimeOut)  do {static TASK _count; _count=(TimeOut); do { WaitX(1); _count--; } while(((A)!=VAL)&&(_count>0));} while(0);
#if (WAITUTILTRTYPE==1)
//ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ������Ϊĳ��ֵ��ʱ��� A��������;VAL����ֵ;TimeOut��ʱֵ;T1FLAYһ�����������ڱ�־���� ʹ�÷����ο�����
#define WAITUNTILTR(A,VAL,TimeOut,T1FLAY) do {static TASK _count; _count=(TimeOut); do { WaitX(0);if(T1FLAY){_count--;T1FLAY=0;}} while(((A)!=VAL)&&(_count>0));} while(0);
#endif
#if (WAITUTILTRTYPE==0)
// ֻ������RunTask���õ������� ʵʱ�Ը��� �ȴ�ĳ��������ʱ��� A��������;VAL����ֵ;TimeOut��ʱֵ;TINDEXʵʱ�ж�������ȡֵ��Χ 0-7��ÿ���жϵ����������ظ������֧��8��ʵʱ�ж�
#define WAITUNTILTR(A,VAL,TimeOut,TINDEX) do {static TASK _count; _count=(TimeOut); do { WaitX(0);if(T1FS&(1<<(TINDEX))){_count--;T1FS&=~(1<<(TINDEX));}} while(((A)!=VAL)&&(_count>0));} while(0);
#endif
//��������
#define RunTask(TaskName,TaskID)  do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0);
//��������ǰ����������ȱ�ִ֤��
#define RunTaskA(TaskName,TaskID) do { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;}}while(0);   //ǰ����������ȱ�ִ֤��
//����������
#define CallSub(SubTaskName) do {TASK currdt; WaitX(0);  currdt=SubTaskName(); if(currdt!=TICKET_MAX) return currdt;} while(0);
//����������
#define CallSub1(SubTaskName,p1) do {TASK currdt; WaitX(0);  currdt=SubTaskName(p1); if(currdt!=TICKET_MAX) return currdt;} while(0);
//����������
#define CallSub2(SubTaskName,p1,p2) do {TASK currdt; WaitX(0);  currdt=SubTaskName(p1,p2); if(currdt!=TICKET_MAX) return currdt;} while(0);
//��ʼ���������
#define InitTasks() do {U8 i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; } while(0);

#if (WAITUTILTRTYPE==0)
#define ST1FS T1FS=0xFF
#endif
#if (WAITUTILTRTYPE==1)
#define ST1FS
#endif

//��������
#if (NORMALDELAY==0)
#define UpdateTimers() do{U8 i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=TICKET_MAX)) timers[i-1]--;}ST1FS;} while(0);
#endif
#if (NORMALDELAY==1)
//��������
#define UpdateTimers() do{U8 i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=TICKET_MAX)) timers[i-1]--;} if(delaycount>0) delaycount--;ST1FS;} while(0);
//��ͨ������������ʱ ֻ�ܱ��������Լ���������õĺ���������������õĺ������õĺ��� ʹ��
#define DELAYX(tickets) do{delaycount=tickets;while(delaycount>0) {runTasks();}}while(0);
#endif

#endif
//**********     SETJMP��-����   **********************************//

//��ʼ���ź���
#define InitSem(sem) do{sem=0;}while(0);
//�ȴ��ź���
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//�����ź���
#define SendSem(sem)  do {sem=0;} while(0);
//�ȴ��ź�����ʱ�����
#define WaitSemX(sem, tickets) do{sem=tickets+1;WaitX(0);if(sem>1){sem--;return 1;}}while (0);

/*****СС����������*******************************************************/
#endif


