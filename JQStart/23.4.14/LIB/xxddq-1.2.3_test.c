#include "xxddq-1.2.3.h"
#include <reg52.h>

//******** IO引脚定义 ********
sbit LED0 = P2 ^ 0;
sbit LED1 = P2 ^ 1;
sbit LED2 = P2 ^ 2;
sbit LED3 = P2 ^ 3;
sbit LED4 = P2 ^ 4;
sbit LED5 = P2 ^ 5;

sbit KEY = P1 ^ 7;

//******** 变量定义 ********
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
} f; //标志结构体

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
// } T1F; //定义一个调度器周期标志变量 C51编译器不适用！！

//******** 子程序 ********
//初始化定时器0
void InitT0(void)
{
    TMOD = 0x21;
    IE |= 0x82; // 12t
    TL0 = 0Xff;
    TH0 = 0XDB;
    TR0 = 1;
}
//初始化LED
void InitLed(void)
{
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
}
//******** 子任务 ********
//子任务 LED4控制
TASK sub_Led4(void)
{
    _SS
    LED4 = 1;
    WaitX(100); //LED4亮100个调度器周期时间
    LED4 = 0;
    _EE
}

//******** 任务 ********
//任务0
TASK task0(void)
{
    _SS while (1)
    {
        // WaitUtilTR(f.b0, 5,T1F.b.b0); //高实时性的超时等待变量f.b0==1;5:超时5个调度器周期;T1F.b.b0:C51编译器不适用，当前超时调度器时钟周期标志位，下一个为T1F.b.b1,以此类推,不可重复!
        WaitUtilTR(f.b0, 5, 0); //高实时性的超时等待变量f.b0==1;5:超时5个调度器周期;0:高实时性超时等待判断的索引，下一个为1,以此类推,不可重复!
        if (f.b0 == 0)          //超时执行
        {
            LED0 ^= 1; //翻转LED0
        }
        else //符合条件执行
        {
            LED1 ^= 1;
        }
    }
    _EE
}
//任务1
TASK task1(void)
{
    _SS while (1)
    {
        //每20个调度器周期将变量f.b0的值翻转（0或1）
        WaitX(20);
        f.b0 ^= 1;
    }
    _EE
}
//任务2
TASK task2(void)
{
    _SS while (1)
    {
        WAITUNTILT(f.b0, 0, 5); //等待变量f.b0==0，超时5个调度器周期
        //也可以这样写： WaitUtilT(f.b0==0,5);
        if (f.b0 == 1) //超时执行
        {
            LED2 ^= 1;
        }
        else //符合条件执行
        {
            LED3 ^= 1;
        }
    }
    _EE
}
//任务3
TASK task3(void)
{
    _SS while (1)
    {
        WaitUntil(f.b1);   //等待f.b1==1
        CallSub(sub_Led4); //调用子任务
        f.b1 = 0;          //清f.b1标志
    }
    _EE
}
//任务4
TASK task4(void)
{
    _SS while (1)
    {
        WaitUntil(KEY == 0); //等待KEY==0
        WaitX(2);            //等待2个调度器周期 按键防抖
        if (KEY == 0)
        {
            WaitUntil(KEY); //等待KEY==1（等待按键放开）
            f.b1 = 1;       //置f.b1标志
        }
    }
    _EE
}

//******** 中断服务 ********
//定时器0中断
void INTT0(void) interrupt 1 using 1
{
    //10ms
    TL0 = 0Xff;
    TH0 = 0XDB;
    LED5 ^= 1;      //输出调度器时钟周期，用于调试
    UpdateTimers(); //更新任务
    //T1F.bs=0xFFFF;    //将一个调度器时钟周期标志变量位全置1 C51编译器不适用！！
    // RunTask(task0, 0); //高级别任务，在中断里执行，可剥夺低级别任务。
}

//******** 任务 ********
void main(void)
{
    InitT0();    //初始化定时器0
    InitLed();   //初始化LED
    InitTasks(); //初始化任务调度
    while (1)
    {
        RunTask(task0, 0);  //高优先级任务
        RunTaskA(task1, 1); //正常任务
        RunTaskA(task2, 2);
        RunTaskA(task3, 3);
        RunTaskA(task4, 4);
    }
}
