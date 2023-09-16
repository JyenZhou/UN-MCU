/**
  ****************************JQStart.c******************************************

c语言支持版本： C99  （c98不支持）

命名规则：
下划线大致意思为语义分割
变量名定义规则 对外API全部以JHAL开头 驱动内部使用函数为__JHAL
类型/常量 大写开头  缩写/简短的格式为大写与  全拼为驼峰
函数名为小写开头驼峰形式


API模式：
初始化与反初始化：init和deInit组成，init参数为config  
——若config是指针类型  外部使用前不可释放（一般定义为全局/静态变量） 初始化后自动使能/启动  
——返回值 true代表成功  false失败的原因一般是已经是当前状态了本次设置无效 该模式满足常用需求
—— 不考虑底层初始化失败 有失败的会进行3次重试后返回false  保证每次顺序为交替后若还有false则可定为底层初始化失败 
——需要自行处理逻辑
中断, 外部直接使用内部已经请过标志位
发送：一般输入的是指针未发送完请勿修改值
接收：接收相关若字节过大或不定长的值一般保存在初始的配置中，需要从此处读取




  * 使用调度器时候需要定义任务数量  XXOS_MAXTASK


    InitTasks();
    while(true) {
        RunTask(systemTask0, 0);
        RunTask(systemTask1, 1);
    }



TASK systemTask0()
{
    _SS
    while(taskWhileTrue)
    {

//由RTC计时给标志位
        if(feedWdgTimesUp)
        {

            feedWDog();
            feedWdgTimesUp = false;
        }
        WaitX(0);
    }
    _EE
}


并且UpdateTimers();需要放在定时器中调用

空参数使用方法
JHAL_delayInit( *(JHAL_DealyConfig *)NULL  );


使用软件IIC 需要定义  JHAL_IIC_Number 数量
 
  *
    ******************************************************************************
  * @file     JQStart.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-11-11 
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
  









/************************************************笔记************************************************************************************************************************************************************


内存区分类
栈区
堆区
全局区
文字常量区
程序代码区

1、栈区（stack）：在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容量有限。
由编译器自动分配释放 ，存放函数的参数值，局部变量的值等，内存的分配是连续的，类似于平时我们所说的栈，如果还不清楚，那么就把它想成数组，
它的内存分配是连续分配的，即，所分配的内存是在一块连续的内存区域内．当我们声明变量时，那么编译器会自动接着当前栈区的结尾来分配内存．
若工程中使用的局部变量较多，定义的数据长度较大时，若不调整栈的空间大小，则会导致程序出现栈溢出，程序运行结果与预期的不符或程序跑飞。
这时我们就需要手动的调整栈的大小。

Stack_Size      EQU     0x00000800
表示栈大小是0X800,也就是2048字节.这样,CPU处理任务的时候,函数局部变量最多可占用的大小就是:2048字节,注意:是所有在处理的函数,包括函数嵌套,递归,等等,都是从这个"栈"里面,来分配的.
所以,如果一个函数的局部变量过多,比如在函数里面定义一个u8 buf[512],这一下就占了1/4的栈大小了,再在其他函数里面来搞两下,程序崩溃是很容易的事情,这时候,一般你会进入到hardfault....
这是初学者非常容易犯的一个错误.切记在函数里面放N多局部变量,尤其有大数组的时候!要计算空间，这样嵌套的编译器检测不出来

2、堆区（heap）
一般由程序员分配释放， 若程序员不释放，程序结束时可能由操作系统回收．类似于链表，在内存中的分布不是连续的，
它们是不同区域的内存块通过指针链接起来的．一旦某一节点从链中断开，我们要人为的把所断开的节点从内存中释放．
如果没有用到标准库的malloc，就是废物，纯属浪费内存，直接设置为0即可。
如果某动态内存不再使用，需要将其释放掉，否则，我们认为发生了内存泄漏现象。

3、全局区（静态区）（static）：内存在程序编译的时候就已经分配好（静态存储区在整个程序运行期间都存在）
全局变量和静态变量的存储是放在一块的，初始化的全局变量和静态变量在一块区域， 未初始化的全局变量和未初始化的静态变量在相邻的另一块区域。 程序结束后由系统释放

4、文字常量区
常量字符串就是放在这里的。 程序结束后由系统释放

5、程序代码区
存放函数体的二进制代码。



堆栈问题


这里建议：单片机空间大： 定义全局大数组 省事，不用过分考虑栈大小，用默认即可，且执行速度快；
			单片机过小时，定义局部大数组，空间重复利用率高，一定要注意很可能要手动指定栈大小，自动生成器覆盖还坑会堆栈的默认配置

空间问题

Code是代码占用的空间 53668为  52.41kB。
RO data是 （Read Only ）只读常量的大小，如const。
RW data是（Read Write） 初始化了的可读写变量的大小。
ZI data是（Zero Initialize） 没有初始化的可读写变量的大小，ZI-data不会被算做代码里因为不会被初始化。


烧写程序所占用的 Flash 空间的大小：Code + RO Data + RW Data
表示运行时占用的 RAM 的大小： RW Data + ZI Data


那么，我们可以得知，上面编译的结果，占用52.41kB的FLASH和2.66kB的RAM。

根据手册可得，STM32F103RCT6，有64kB的RAM和256kB的FLASH，能装得下这套代码。





        C语言 将函数名作为参数被另外一个函数调用 精选

https://www.jianshu.com/p/f2b0499afd3f
https://zhidao.baidu.com/question/363328873.html



关于CRC:
等式G(X)=X^4+X+1 可以写成
G(X)=1*(X^4) + 0*(X^3) + 0*(X^2) + 1*X + 1*1
从末位开始，判断是否为X的几次幂，是就为1，否则为0.如上G（X）中有x的0次、1次和4次幂，则在对应的位置上有10011，从最后一位开始写，
在cubex中是X4+X1+X0
https://zhidao.baidu.com/question/562211288283552404.html?qbl=relate_question_3&word=%B6%E0%CF%EE%CA%BD%D3%EB%B6%FE%BD%F8%D6%C6%CA%FD%C2%EB
结构体可以直接赋值 但是要注意 结构体的拷贝只是浅拷贝，即指针p_c的赋值并不会导致再申请一块内存区域，让foo2的p_c指向它。那么，如果释放掉foo1中的p_c指向的内存，此时foo2中p_c变成野指针，
结构体传引用好处1.效率高 2.有指针变量时不会发生浅拷贝问题
 -----------------------------------------------------------------------*/
/**
  *@ 功能或简介: 去除指定警告    这个cjson 是库文件 没必要在意它的警告   注意不要放在.h  不然你调用了.h的c若是也出现了这种问题就不会警告了
	要是想让一种警告让所有地方都不在出现  for target”，在C/C++选项中，可以看到有个Misc Controls选项

  ******************************************************************************
#pragma diag_suppress 1293
#pragma diag_suppress 144
#pragma diag_suppress 111




    #include "time.h"
    clock_t start, stop;
    //不在测试范围内的准备工作写在 clock() 调用之前
    start = clock();     //开始计时
    foo();              // 把被测函数加在这里
    stop = clock();     // 停止计时
    duration = ((double)(stop-start))/CLOCKS_PER_SEC;
    //其他不在测试范围的处理写在后面，例如输出 duration 的值
    return 0;




*********************/


/*  RAM.sct



LR_IROM1 0x00000000  0x00020000  {    ; load region size_region
  ER_IROM1 0x00000000  0x00020000  {  ; load address = execution address
   *.o (RESET, +First)
   *(InRoot$$Sections)
   .ANY (+RO)
  }


  ER_IROM2 0x1FFFE800 0x00000800  {  ; load address = execution address
   *flash.o (+RO)
  }
  RW_IRAM1 0x1FFFF000 0x00003800  {  ; RW data
   .ANY (+RW +ZI)
  *.o(JRAMCODE)
  }
  }



生成BIN文件指令  魔术棒->user->after build 勾选Run1后面填写

fromelf.exe --bin --bincombined --bincombined_padding=1,0xff --output !L.bin !L




单片机特殊注意事项总汇

低功耗
RTC无法唤醒低功耗

Flash
单总线导致操作flash时候必须RAM中运行

ADC
带隙电压不准
首次采样可能不准 建议配置后调用一次采集



*/



#include "JFML.h"
 

char  UIDString[25];
 




#include "Util/JHAL_CRC.c"
#include "Util/JHAL_Math.c"
#include "Util/JHAL_NumberConverter.c"
#include "Util/zdmalloc.c"
#include "SystemSelfTest.c"


 
#include "HAL/JHAL_SoftwareIIC.c"
 
// XL6600A402L6 Flash 128K  RAM16K

#ifdef XL6600A402L6
#include "HAL/Chipways/JHAL_GPIO.c"
#include "HAL/Chipways/JHAL_Delay.c"
#include "HAL/Chipways/JHAL_PWM.c"
#include "HAL/Chipways/JHAL_ADC.c"
#include "HAL/Chipways/JHAL_CAN.c"
#include "HAL/Chipways/JHAL_RTC.c"
#include "HAL/Chipways/JHAL_LowPower.c"
#include "HAL/Chipways/JHAL_ACMP.c"
#pragma arm section code = "JRAMCODE"
#include "HAL/Chipways/JHAL_Flash.c"
#pragma arm section
#include "HAL/Chipways/JHAL_Uart.c"
#include "HAL/Chipways/JHAL_LIN.c"
#include "HAL/Chipways/JHAL_Timer.c"
#include "HAL/Chipways/JHAL_WDG.c"


void JHAL_systemReset()
{
	NVIC_SystemReset();
}
u32 JHAL_uidGetHigh()
{
	return 0;
}
u32 JHAL_uidGetMiddle()
{
	return 0;
}
u32 JHAL_uidGetLow()
{
	return SIM_GetUUIDL();
}
#endif

#ifdef ASM31X003 
#include  "HAL/Setenvi/JHAL_GPIO.c"
#include  "HAL/Setenvi/JHAL_RTC.c" 
#include  "HAL/Setenvi/JHAL_Delay.c" 
#include  "HAL/Setenvi/JHAL_Flash.c" 
#include  "HAL/Setenvi/JHAL_Uart.c"
#include  "HAL/Setenvi/JHAL_LIN.c"
#include  "HAL/Setenvi/JHAL_Timer.c"
#include  "HAL/Setenvi/JHAL_ADC.c"


#endif



#ifdef USE_HAL_DRIVER 


void JHAL_systemReset()
{
	NVIC_SystemReset();
}
u32 JHAL_uidGetHigh()
{
	return HAL_GetUIDw2();
}
u32 JHAL_uidGetMiddle()
{
	return HAL_GetUIDw1();
}
u32 JHAL_uidGetLow()
{
	return HAL_GetUIDw0();
}  
	

#include  "HAL/STM32/JHAL_CAN.c"	
#include  "HAL/STM32/JHAL_GPIO.c"	
#include  "HAL/STM32/JHAL_Delay.c" 
#include  "HAL/STM32/JHAL_Uart.c"	
#include  "HAL/STM32/JHAL_Flash.c" 

#include  "HAL/STM32/stm32f1xx_hal_wwdg.c" 

#endif




 
	



#include "HAL/JHAL_BootLoader.c"



void uid2string()
{
 
    snprintf(UIDString, sizeof(UIDString), "%08X%08X%08X", JHAL_uidGetHigh(), JHAL_uidGetMiddle(), JHAL_uidGetLow());
 
}



#ifdef ADPD_IIC_ID
#include "FML/ADPD188BI/AdpdDrv.c"
#include "FML/ADPD188BI/smoke_detect.c"
#endif

#include "FML/THTB/NTC/NTC.c"

#ifdef GasUtil4ICRA_GasSensorNumber 
#include "FML/GAS/GasUtil4ICRA.c"
#endif
#ifdef GasUtil4ICRA2_GasSensorNumber 
#include "FML/GAS/GasUtil4ICRA2.c"
#endif





















#if XXOS_MAXTASK
//来自调度器的
volatile  TASK   timers[MAXTASKS]= {0};
U8 T1FS=0 ;
u8  taskWhileTrue=1;
#endif

volatile u64 jsystemMs=0;























/** ----------------------------JHAL_autoInit----------------------------------- 
  * 描述：累计的一些通用的可能必要的自初始化 不受外设和MCU影响  
  *
  * 参数：
**	  	 : [输入/出] 
  *
  * 返回值:无
  * 注:无
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

OS_BEFORE_MAIN_EXE void __JHAL_selfInit()
{
  __JHAL_systemSelfTest();
}

/*------------------Jyen--------------------------Jyen-----------------------
******************************************************************************
*@ 函数功能或简介: 封装的调度器启动， 同时通用的初始化自动初始化
  * @输入参数:   无
  * @返 回 值: 无
  * @备注: 考虑到灵活使用，这里不封装批量反初始化
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

void JQStart()
{
	uid2string();
	my_mem_init();
 
#ifdef USE_HAL_DRIVER 
  
JHAL_delayInit((JHAL_DealyConfig){NULL});
 
 

  
#ifdef HAL_WWDG_MODULE_ENABLED
//当debug时候不要初始化看门狗  默认提前喂狗  最好在主程序主动喂 
    if (!(CoreDebug->DHCSR & 1))    //check C_DEBUGEN == 1 -> Debugger Connected
    {			
     JHAL_wdgInit(JHAL_WDG_WWDG,(JHAL_WDGConfig){NULL});
    }
#endif  


    /*使用定时器 */
#if  UTTIL_TIMER
    utilTimer_Init();
#endif
 
 

#if defined HAL_CAN_MODULE_ENABLED
    bisp_CANUtil_Init();
#endif

 
		
		
		
		
		
		
		
		
		
		
		


    /*************************************************************************************************
    * @
    * @上面是初始化Bip层
    * @
    * @下面是初始化bsp层 （bsp里面包含了初始化bip层 所以可以不分先后初始化了 这里是为了对齐）
    * @
    * @
    * **************Jyen *****************************Jyen ******************Jyen *********
    */
    /*步进电机1的定时器初始化*/
#ifdef StepperMotor1
    Motor1_MX_TIM1_Init();
#endif

    /*步进电机2的定时器初始化*/
#ifdef StepperMotor2
    Motor2_MX_TIM2_Init();
#endif

    /*步进电机3的定时器初始化*/
#ifdef StepperMotor3
    Motor3_MX_TIM3_Init();
#endif


    /*LCD12864初始化*/
#ifdef LCD12864_M
    mInitLCD();
#endif
    /*OLED0.96寸的液晶屏初始化*/
#ifdef OLED_96
    OLED_Init();//OLED初始化
#endif
#ifdef S4_74HC595
    S4_74HC595_init();
#endif


    /*TPS7A7100_RGW 0.9-3.5V 可选电压模块*/
#ifdef TPS7A7100_RGW
    TPS7A7100_RWG_init();
#endif
#ifdef TPS7A7100_RGW_2
    TPS7A7100_2_RWG_init();
#endif

#ifdef TPS7A4700_RWG
    TPS7A4700_RWG_init();
#endif
#ifdef TPS7A4700_RWG_2
    TPS7A4700_2_RWG_init();
#endif


    /*通过电阻改变输出电压的模块*/
#ifdef TPS7A7001
    .....................空的  占位置   懒得找他初始化的时候补上
#endif

    /*通过真值表 切换开关*/
#ifdef ADG704
    ADG704_init();
#endif
    /*通过真值表 切换开关  这是电路上出现两个时使用*/
#ifdef ADG704_2
    ADG704_2_init();
#endif

#ifdef ADG706
    //  ADG706_init();
#endif

#ifdef ADG706_2
    ADG706_2_init();
#endif

#ifdef CD4051B
    CD4051B_init();
#endif

#ifdef CD4051B_2
    CD4051B_2_init();
#endif

#ifdef CD4051B_3
    CD4051B_3_init();
#endif

//彩屏
#ifdef RGB_LCD128X160
    .h文件没添加 用的时候搞下并把Lcd_Init重命名成RGB_LCD128X160_Init
    用的模拟SPI通信也在.C文件中说明下
    Lcd_Init();
//清屏置全白
    LCD_Clear(WHITE);
#endif
//ADC

#ifdef ADS1256

    //  Init_ADS1256_Init();

#endif



#ifdef BL1551
    bl1551_init();
#endif


#ifdef BL1551_2
    bl1551_2_init();
#endif

#ifdef ADG1204_EN
    ADG1204_init();
#endif

#ifdef ADG1204_2_EN
    ADG1204_2_init();
#endif

#ifdef ADG1204_3_EN
    ADG1204_init();
#endif

#ifdef ADG1204_4_EN
    ADG1204_init();
#endif

#ifdef ADG1204_5_EN
    ADG1204_init();
#endif

#ifdef ADG1204_6_EN
    ADG1204_init();
#endif



#ifdef AD7608_EN
    /* 0是5v基准   1是10v基准*/
    spi_InitAD7606(1);
#endif


    /*温湿度*/
#ifdef SHT20_EN
    SHT20_Init();
#endif

#ifdef PT100_MAX31865_EN
    PT100_max3185_init();
#endif


#endif


#ifdef Lora_E22_EN
    Lora_E22_400T30S_Init();
#endif









}

void  JHAL_disableInterrupts()
{
	__disable_irq();
}
void  JHAL_enableInterrupts()
{
	__enable_irq();
}

 

/**
*  
*程序跑飞后会进入该方法
*/
 
void JHAL_HardFault_Handler(void)
{
     
	#ifdef USE_HAL_DRIVER
    if (CoreDebug->DHCSR & 1)    //check C_DEBUGEN == 1 -> Debugger Connected
    {
        __breakpoint(0);  // halt program execution here
    }
    
#endif
		JHAL_systemReset();
}
 

 

//
//0.1->产品ID
//2->5A升级Boot  A9升级APP
//3->date0
//4->date1
//5->date2
//6.7->CRC_Mudbus16
void JHAL_bootLoaderTryUpdate(u8 *data)
{
	
	
}
 



 




