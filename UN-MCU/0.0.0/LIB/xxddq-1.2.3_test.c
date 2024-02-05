#include "xxddq-1.2.3.h"
#include <reg52.h>

//******** IO���Ŷ��� ********
sbit LED0 = P2 ^ 0;
sbit LED1 = P2 ^ 1;
sbit LED2 = P2 ^ 2;
sbit LED3 = P2 ^ 3;
sbit LED4 = P2 ^ 4;
sbit LED5 = P2 ^ 5;

sbit KEY = P1 ^ 7;

//******** �������� ********
struct
{
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
} f; //��־�ṹ��

// union
// {
//     unsigned int bs;
//     struct
//     {
//        unsigned b0:1;
//        unsigned b1:1;
//        unsigned b2:1;
//        unsigned b3:1;
//        unsigned b4:1;
//        unsigned b5:1;
//        unsigned b6:1;
//        unsigned b7:1;
//        unsigned b8:1;
//        unsigned b9:1;
//        unsigned ba:1;
//        unsigned bb:1;
//        unsigned bc:1;
//        unsigned bd:1;
//        unsigned be:1;
//        unsigned bf:1;
//     } b;
// } T1F; //����һ�����������ڱ�־���� C51�����������ã���

//******** �ӳ��� ********
//��ʼ����ʱ��0
void InitT0(void)
{
    TMOD = 0x21;
    IE |= 0x82; // 12t
    TL0 = 0Xff;
    TH0 = 0XDB;
    TR0 = 1;
}
//��ʼ��LED
void InitLed(void)
{
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
}
//******** ������ ********
//������ LED4����
TASK sub_Led4(void)
{
    _SS
    LED4 = 1;
    WaitX(100); //LED4��100������������ʱ��
    LED4 = 0;
    _EE
}

//******** ���� ********
//����0
TASK task0(void)
{
    _SS while (1)
    {
        // WaitUtilTR(f.b0, 5,T1F.b.b0); //��ʵʱ�Եĳ�ʱ�ȴ�����f.b0==1;5:��ʱ5������������;T1F.b.b0:C51�����������ã���ǰ��ʱ������ʱ�����ڱ�־λ����һ��ΪT1F.b.b1,�Դ�����,�����ظ�!
        WaitUtilTR(f.b0, 5, 0); //��ʵʱ�Եĳ�ʱ�ȴ�����f.b0==1;5:��ʱ5������������;0:��ʵʱ�Գ�ʱ�ȴ��жϵ���������һ��Ϊ1,�Դ�����,�����ظ�!
        if (f.b0 == 0)          //��ʱִ��
        {
            LED0 ^= 1; //��תLED0
        }
        else //��������ִ��
        {
            LED1 ^= 1;
        }
    }
    _EE
}
//����1
TASK task1(void)
{
    _SS while (1)
    {
        //ÿ20�����������ڽ�����f.b0��ֵ��ת��0��1��
        WaitX(20);
        f.b0 ^= 1;
    }
    _EE
}
//����2
TASK task2(void)
{
    _SS while (1)
    {
        WAITUNTILT(f.b0, 0, 5); //�ȴ�����f.b0==0����ʱ5������������
        //Ҳ��������д�� WaitUtilT(f.b0==0,5);
        if (f.b0 == 1) //��ʱִ��
        {
            LED2 ^= 1;
        }
        else //��������ִ��
        {
            LED3 ^= 1;
        }
    }
    _EE
}
//����3
TASK task3(void)
{
    _SS while (1)
    {
        WaitUntil(f.b1);   //�ȴ�f.b1==1
        CallSub(sub_Led4); //����������
        f.b1 = 0;          //��f.b1��־
    }
    _EE
}
//����4
TASK task4(void)
{
    _SS while (1)
    {
        WaitUntil(KEY == 0); //�ȴ�KEY==0
        WaitX(2);            //�ȴ�2������������ ��������
        if (KEY == 0)
        {
            WaitUntil(KEY); //�ȴ�KEY==1���ȴ������ſ���
            f.b1 = 1;       //��f.b1��־
        }
    }
    _EE
}

//******** �жϷ��� ********
//��ʱ��0�ж�
void INTT0(void) interrupt 1 using 1
{
    //10ms
    TL0 = 0Xff;
    TH0 = 0XDB;
    LED5 ^= 1;      //���������ʱ�����ڣ����ڵ���
    UpdateTimers(); //��������
    //T1F.bs=0xFFFF;    //��һ��������ʱ�����ڱ�־����λȫ��1 C51�����������ã���
    // RunTask(task0, 0); //�߼����������ж���ִ�У��ɰ���ͼ�������
}

//******** ���� ********
void main(void)
{
    InitT0();    //��ʼ����ʱ��0
    InitLed();   //��ʼ��LED
    InitTasks(); //��ʼ���������
    while (1)
    {
        RunTask(task0, 0);  //�����ȼ�����
        RunTaskA(task1, 1); //��������
        RunTaskA(task2, 2);
        RunTaskA(task3, 3);
        RunTaskA(task4, 4);
    }
}
