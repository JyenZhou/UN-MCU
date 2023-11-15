/**
  ****************************JQStart.c******************************************

c����֧�ְ汾�� C99  ��c98��֧�֣�

��������
�»��ߴ�����˼Ϊ����ָ�
������������� ����APIȫ����JHAL��ͷ �����ڲ�ʹ�ú���Ϊ__JHAL
����/���� ��д��ͷ  ��д/��̵ĸ�ʽΪ��д��  ȫƴΪ�շ�
������ΪСд��ͷ�շ���ʽ


APIģʽ��
��ʼ���뷴��ʼ����init��deInit��ɣ�init����Ϊconfig
������config��ָ������  �ⲿʹ��ǰ�����ͷţ�һ�㶨��Ϊȫ��/��̬������ ��ʼ�����Զ�ʹ��/����
��������ֵ true�����ɹ�  falseʧ�ܵ�ԭ��һ�����Ѿ��ǵ�ǰ״̬�˱���������Ч ��ģʽ���㳣������
���� �����ǵײ��ʼ��ʧ�� ��ʧ�ܵĻ����3�����Ժ󷵻�false  ��֤ÿ��˳��Ϊ�����������false��ɶ�Ϊ�ײ��ʼ��ʧ��
������Ҫ���д����߼�
�ж�, �ⲿֱ��ʹ���ڲ��Ѿ������־λ
���ͣ�һ���������ָ��δ�����������޸�ֵ
���գ�����������ֽڹ���򲻶�����ֵһ�㱣���ڳ�ʼ�������У���Ҫ�Ӵ˴���ȡ




  * ʹ�õ�����ʱ����Ҫ������������  XXOS_MAXTASK


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

//��RTC��ʱ����־λ
        if(feedWdgTimesUp)
        {

            feedWDog();
            feedWdgTimesUp = false;
        }
        WaitX(0);
    }
    _EE
}


����UpdateTimers();��Ҫ���ڶ�ʱ���е���

�ղ���ʹ�÷���
JHAL_delayInit( *(JHAL_DealyConfig *)NULL  );


ʹ������IIC ��Ҫ����  JHAL_IIC_Number ����

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










/************************************************�ʼ�************************************************************************************************************************************************************


�ڴ�������
ջ��
����
ȫ����
���ֳ�����
���������

1��ջ����stack������ִ�к���ʱ�������ھֲ������Ĵ洢��Ԫ��������ջ�ϴ���������ִ�н���ʱ��Щ�洢��Ԫ�Զ����ͷš�ջ�ڴ�������������ڴ�������ָ��У�Ч�ʺܸߣ����Ƿ�����ڴ��������ޡ�
�ɱ������Զ������ͷ� ����ź����Ĳ���ֵ���ֲ�������ֵ�ȣ��ڴ�ķ����������ģ�������ƽʱ������˵��ջ����������������ô�Ͱ���������飬
�����ڴ��������������ģ�������������ڴ�����һ���������ڴ������ڣ���������������ʱ����ô���������Զ����ŵ�ǰջ���Ľ�β�������ڴ森
��������ʹ�õľֲ������϶࣬��������ݳ��Ƚϴ�ʱ����������ջ�Ŀռ��С����ᵼ�³������ջ������������н����Ԥ�ڵĲ���������ܷɡ�
��ʱ���Ǿ���Ҫ�ֶ��ĵ���ջ�Ĵ�С��

Stack_Size      EQU     0x00000800
��ʾջ��С��0X800,Ҳ����2048�ֽ�.����,CPU���������ʱ��,�����ֲ���������ռ�õĴ�С����:2048�ֽ�,ע��:�������ڴ����ĺ���,��������Ƕ��,�ݹ�,�ȵ�,���Ǵ����"ջ"����,�������.
����,���һ�������ľֲ���������,�����ں������涨��һ��u8 buf[512],��һ�¾�ռ��1/4��ջ��С��,������������������������,��������Ǻ����׵�����,��ʱ��,һ�������뵽hardfault....
���ǳ�ѧ�߷ǳ����׷���һ������.�м��ں��������N��ֲ�����,�����д������ʱ��!Ҫ����ռ䣬����Ƕ�׵ı�������ⲻ����

2��������heap��
һ���ɳ���Ա�����ͷţ� ������Ա���ͷţ��������ʱ�����ɲ���ϵͳ���գ����������������ڴ��еķֲ����������ģ�
�����ǲ�ͬ������ڴ��ͨ��ָ�����������ģ�һ��ĳһ�ڵ�����жϿ�������Ҫ��Ϊ�İ����Ͽ��Ľڵ���ڴ����ͷţ�
���û���õ���׼���malloc�����Ƿ�������˷��ڴ棬ֱ������Ϊ0���ɡ�
���ĳ��̬�ڴ治��ʹ�ã���Ҫ�����ͷŵ�������������Ϊ�������ڴ�й©����

3��ȫ��������̬������static�����ڴ��ڳ�������ʱ����Ѿ�����ã���̬�洢�����������������ڼ䶼���ڣ�
ȫ�ֱ����;�̬�����Ĵ洢�Ƿ���һ��ģ���ʼ����ȫ�ֱ����;�̬������һ������ δ��ʼ����ȫ�ֱ�����δ��ʼ���ľ�̬���������ڵ���һ������ �����������ϵͳ�ͷ�

4�����ֳ�����
�����ַ������Ƿ�������ġ� �����������ϵͳ�ͷ�

5�����������
��ź�����Ķ����ƴ��롣



��ջ����


���ｨ�飺��Ƭ���ռ�� ����ȫ�ִ����� ʡ�£����ù��ֿ���ջ��С����Ĭ�ϼ��ɣ���ִ���ٶȿ죻
			��Ƭ����Сʱ������ֲ������飬�ռ��ظ������ʸߣ�һ��Ҫע��ܿ���Ҫ�ֶ�ָ��ջ��С���Զ����������ǻ��ӻ��ջ��Ĭ������

�ռ�����

Code�Ǵ���ռ�õĿռ� 53668Ϊ  52.41kB��
RO data�� ��Read Only ��ֻ�������Ĵ�С����const��
RW data�ǣ�Read Write�� ��ʼ���˵Ŀɶ�д�����Ĵ�С��
ZI data�ǣ�Zero Initialize�� û�г�ʼ���Ŀɶ�д�����Ĵ�С��ZI-data���ᱻ������������Ϊ���ᱻ��ʼ����


��д������ռ�õ� Flash �ռ�Ĵ�С��Code + RO Data + RW Data
��ʾ����ʱռ�õ� RAM �Ĵ�С�� RW Data + ZI Data


��ô�����ǿ��Ե�֪���������Ľ����ռ��52.41kB��FLASH��2.66kB��RAM��

�����ֲ�ɵã�STM32F103RCT6����64kB��RAM��256kB��FLASH����װ�������״��롣





        C���� ����������Ϊ����������һ���������� ��ѡ

https://www.jianshu.com/p/f2b0499afd3f
https://zhidao.baidu.com/question/363328873.html



����CRC:
��ʽG(X)=X^4+X+1 ����д��
G(X)=1*(X^4) + 0*(X^3) + 0*(X^2) + 1*X + 1*1
��ĩλ��ʼ���ж��Ƿ�ΪX�ļ����ݣ��Ǿ�Ϊ1������Ϊ0.����G��X������x��0�Ρ�1�κ�4���ݣ����ڶ�Ӧ��λ������10011�������һλ��ʼд��
��cubex����X4+X1+X0
https://zhidao.baidu.com/question/562211288283552404.html?qbl=relate_question_3&word=%B6%E0%CF%EE%CA%BD%D3%EB%B6%FE%BD%F8%D6%C6%CA%FD%C2%EB
�ṹ�����ֱ�Ӹ�ֵ ����Ҫע�� �ṹ��Ŀ���ֻ��ǳ��������ָ��p_c�ĸ�ֵ�����ᵼ��������һ���ڴ�������foo2��p_cָ��������ô������ͷŵ�foo1�е�p_cָ����ڴ棬��ʱfoo2��p_c���Ұָ�룬
�ṹ�崫���úô�1.Ч�ʸ� 2.��ָ�����ʱ���ᷢ��ǳ��������
 -----------------------------------------------------------------------*/
/**
  *@ ���ܻ���: ȥ��ָ������    ���cjson �ǿ��ļ� û��Ҫ�������ľ���   ע�ⲻҪ����.h  ��Ȼ�������.h��c����Ҳ��������������Ͳ��ᾯ����
	Ҫ������һ�־��������еط������ڳ���  for target������C/C++ѡ���У����Կ����и�Misc Controlsѡ��

  ******************************************************************************
#pragma diag_suppress 1293
#pragma diag_suppress 144
#pragma diag_suppress 111




    #include "time.h"
    clock_t start, stop;
    //���ڲ��Է�Χ�ڵ�׼������д�� clock() ����֮ǰ
    start = clock();     //��ʼ��ʱ
    foo();              // �ѱ��⺯����������
    stop = clock();     // ֹͣ��ʱ
    duration = ((double)(stop-start))/CLOCKS_PER_SEC;
    //�������ڲ��Է�Χ�Ĵ���д�ں��棬������� duration ��ֵ
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

bootLoderʱ��ֻҪ�޸�����ľ�����  ʹ���ⲿsc Target�޸���Ч�� Ϊ�˺ÿ�����ͬ�� ���������в���Ҫ�޸��˲�Ȼ����Ӱ��оƬʶ��

���ǵ�hex����bugӰ��bin�ļ� ���涼���Լ�����λ������hex
����BIN�ļ�ָ��  ħ����->user->after build ��ѡRun1������д

fromelf.exe --bin --bincombined --bincombined_padding=1,0xff --output !L.bin !L




��Ƭ������ע�������ܻ�

�͹���
RTC�޷����ѵ͹���

Flash
�����ߵ��²���flashʱ�����RAM������

ADC
��϶��ѹ��׼
�״β������ܲ�׼ �������ú����һ�βɼ�



*/



#include "JFML.h"








#include "Util/JHAL_CRC.c"
#include "Util/JHAL_Math.c"
#include "Util/JHAL_NumberConverter.c"
#include "Util/JHAL_JSON.c"
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


#include  "HAL/STM32/JHAL_ADC.c"
#include  "HAL/STM32/JHAL_DAC.c"
#include  "HAL/STM32/JHAL_CAN.c"
#include  "HAL/STM32/JHAL_GPIO.c"
#include  "HAL/STM32/JHAL_Delay.c"
#include  "HAL/STM32/JHAL_Uart.c"
#include  "HAL/STM32/JHAL_Flash.c"
#include  "HAL/STM32/JHAL_Wdg.c"

#endif









#include "HAL/JHAL_BootLoader.c"





#ifdef ADPD_IIC_ID
#include "FML/ADPD188BI/AdpdDrv.c"
#include "FML/ADPD188BI/smoke_detect.c"
#endif

#include "FML/ADC/ADS1115/ads1115.c"

#include "FML/THTB/NTC/NTC.c"
#include "FML/THTB/SHTxx/SHT3x.c"

#ifdef GasUtil4ICRA_GasSensorNumber
#include "FML/GAS/GasUtil4ICRA.c"
#endif
#ifdef GasUtil4ICRA2_GasSensorNumber
#include "FML/GAS/GasUtil4ICRA2.c"
#endif





















#if XXOS_MAXTASK
//���Ե�������
volatile  TASK   timers[MAXTASKS]= {0};
U8 T1FS=0 ;
u8  taskWhileTrue=1;
#endif

volatile u64 jsystemMs=0;























/** ----------------------------JHAL_autoInit-----------------------------------
  * �������ۼƵ�һЩͨ�õĿ��ܱ�Ҫ���Գ�ʼ�� ���������MCUӰ��
  *
  * ������
**	  	 : [����/��]
  *
  * ����ֵ:��
  * ע:��
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

OS_BEFORE_MAIN_EXE void __JHAL_selfInit()
{
    __JHAL_systemSelfTest();
}

/*------------------Jyen--------------------------Jyen-----------------------
******************************************************************************
*@ �������ܻ���: ��װ�ĵ����������� ͬʱͨ�õĳ�ʼ���Զ���ʼ��
  * @�������:   ��
  * @�� �� ֵ: ��
  * @��ע: ���ǵ����ʹ�ã����ﲻ��װ��������ʼ��
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

void JQStart()
{



#ifdef USE_HAL_DRIVER





#ifdef HAL_WWDG_MODULE_ENABLED
//��debugʱ��Ҫ��ʼ�����Ź�  Ĭ����ǰι��  ���������������ι
    if (!(CoreDebug->DHCSR & 1))    //check C_DEBUGEN == 1 -> Debugger Connected
    {
        JHAL_wdgOpen(  NULL );
    }
#endif


    /*ʹ�ö�ʱ�� */
#if  UTTIL_TIMER
    utilTimer_Init();
#endif



#if defined HAL_CAN_MODULE_ENABLED
    bisp_CANUtil_Init();
#endif















    /*************************************************************************************************
    * @
    * @�����ǳ�ʼ��Bip��
    * @
    * @�����ǳ�ʼ��bsp�� ��bsp��������˳�ʼ��bip�� ���Կ��Բ����Ⱥ��ʼ���� ������Ϊ�˶��룩
    * @
    * @
    * **************Jyen *****************************Jyen ******************Jyen *********
    */
    /*�������1�Ķ�ʱ����ʼ��*/
#ifdef StepperMotor1
    Motor1_MX_TIM1_Init();
#endif

    /*�������2�Ķ�ʱ����ʼ��*/
#ifdef StepperMotor2
    Motor2_MX_TIM2_Init();
#endif

    /*�������3�Ķ�ʱ����ʼ��*/
#ifdef StepperMotor3
    Motor3_MX_TIM3_Init();
#endif


    /*LCD12864��ʼ��*/
#ifdef LCD12864_M
    mInitLCD();
#endif
    /*OLED0.96���Һ������ʼ��*/
#ifdef OLED_96
    OLED_Init();//OLED��ʼ��
#endif
#ifdef S4_74HC595
    S4_74HC595_init();
#endif


    /*TPS7A7100_RGW 0.9-3.5V ��ѡ��ѹģ��*/
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


    /*ͨ������ı������ѹ��ģ��*/
#ifdef TPS7A7001
    .....................�յ�  ռλ��   ����������ʼ����ʱ����
#endif

    /*ͨ����ֵ�� �л�����*/
#ifdef ADG704
    ADG704_init();
#endif
    /*ͨ����ֵ�� �л�����  ���ǵ�·�ϳ�������ʱʹ��*/
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

//����
#ifdef RGB_LCD128X160
    .h�ļ�û���� �õ�ʱ����²���Lcd_Init��������RGB_LCD128X160_Init
    �õ�ģ��SPIͨ��Ҳ��.C�ļ���˵����
    Lcd_Init();
//������ȫ��
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
    /* 0��5v��׼   1��10v��׼*/
    spi_InitAD7606(1);
#endif


    /*��ʪ��*/
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
*�����ܷɺ�����÷���
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










