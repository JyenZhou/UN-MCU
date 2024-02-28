#include "JFML.h"
/***
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
��������ֵ true����ɹ�  falseʧ�ܵ�ԭ��һ�����Ѿ��ǵ�ǰ״̬�˱���������Ч ��ģʽ���㳣������
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
��ʾջ��С��0X800,Ҳ����2048�ֽ�.����,CPU���������ʱ��,�����ֲ���������ռ�õĴ�С����:2048�ֽ�,ע��:�������ڴ���ĺ���,��������Ƕ��,�ݹ�,�ȵ�,���Ǵ����"ջ"����,�������.
����,���һ�������ľֲ���������,�����ں������涨��һ��u8 buf[512],��һ�¾�ռ��1/4��ջ��С��,������������������������,��������Ǻ����׵�����,��ʱ��,һ�������뵽hardfault....
���ǳ�ѧ�߷ǳ����׷���һ������.�м��ں��������N��ֲ�����,�����д������ʱ��!Ҫ����ռ䣬����Ƕ�׵ı�������ⲻ����

2��������heap��
һ���ɳ���Ա�����ͷţ� ������Ա���ͷţ��������ʱ�����ɲ���ϵͳ���գ��������������ڴ��еķֲ����������ģ�
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

bootLoderʱ��ֻҪ�޸�����ľ�����  ʹ���ⲿsct Target���޸���Ч�� Ϊ�˺ÿ�����ͬ�� ���������в���Ҫ�޸��˲�Ȼ����Ӱ��оƬʶ��

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








#include "Util\JHAL_CRC.c"
#include "Util\JHAL_Math.c"
#include "Util\JHAL_NumberConverter.c"
#include "Util\FontLib\jfontLib.c"
#include "Util\zdmalloc.c"
#include "SystemSelfTest.c"

#include "HAL\JHAL_SoftwareIIC.c"






//�ַ�����
#define STRINGIFY(x) #x
//�ݹ�չ��
#define EXPAND_AND_STRINGIFY(x) STRINGIFY(x)
//����ռλ��ƴ��const ·��
#define CONNECT_FILE_PATH_HEIP(_1, _2, _3,  ...)   _1\_2\_3
#define CONNECT_FILE_PATH(...) CONNECT_FILE_PATH_HEIP(__VA_ARGS__,,, )





#if defined USE_HAL_DRIVER
#define MCU_NAME STM32

//�Ϻ�����ά
#elif  defined XL6600A402L6
// XL6600A402L6 Flash 128K  RAM16K
#define MCU_NAME Chipways

//����΢
#elif  defined ASM31X003
#define MCU_NAME Setenvi

//�鶯΢
#elif  defined MM32G0001
#define MCU_NAME MM32
#ifdef 
#include  "HAL/MM32/Manufacturer/system_mm32g0001.c"
 
#include  "HAL/MM32/Manufacturer/hal_rcc.c"
#include  "HAL/MM32/Manufacturer/hal_dbg.c"
#include  "HAL/MM32/Manufacturer/hal_flash.c"
#include  "HAL/MM32/Manufacturer/hal_exti.c"
#include  "HAL/MM32/Manufacturer/hal_crc.c"
#include  "HAL/MM32/Manufacturer/hal_adc.c"
#include  "HAL/MM32/Manufacturer/hal_gpio.c"
#include  "HAL/MM32/Manufacturer/hal_iwdg.c"
#include  "HAL/MM32/Manufacturer/hal_misc.c"
#include  "HAL/MM32/Manufacturer/hal_pwr.c"
#include  "HAL/MM32/Manufacturer/hal_spi.c"
#include  "HAL/MM32/Manufacturer/hal_uid.c"
#include  "HAL/MM32/Manufacturer/hal_usart.c"
#include  "HAL/MM32/Manufacturer/hal_tim.c"
#include  "HAL/MM32/Manufacturer/hal_i2c.c"
#endif

//̩��΢
#elif  defined   TC04
#define MCU_NAME TC
#include "HAL/TC/Manufacturer/TC04/system_tc04xx.c"
#include "HAL/TC/Manufacturer/TC04/tc04xx_uart.c"
#include "HAL/TC/Manufacturer/TC04/tc04xx_debug.c"
#include "HAL/TC/Manufacturer/TC04/tc04xx_delay.c"
#include "HAL/TC/Manufacturer/TC04/tc04xx_gpio.c"


#endif






#if defined MCU_NAME
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_Timer.c))//pwm��
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_PWM.c))//adc��
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_ADC.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_DAC.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_CAN.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_GPIO.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_Delay.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_Uart.c))
#ifdef XL6600A402L6
#pragma arm section code = "JRAMCODE"
#endif
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_Flash.c))
#ifdef XL6600A402L6
#pragma arm section
#endif
#include  "HAL/JHAL_Flash.c"
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_WDG.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_RTC.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_ACMP.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_LowPower.c))
#include EXPAND_AND_STRINGIFY( CONNECT_FILE_PATH(HAL,MCU_NAME,JHAL_LIN.c))
#include "HAL/JHAL_BootLoader.c"




void JHAL_systemReset()
{
//�ú��������ڳ���pack����ͷ�ļ�  
#if defined(__ARMCC_VERSION)
    NVIC_SystemReset();
#endif


}

#else
#warning  ����֧�ֵ�MCU�ͺ� �����㽫�޷�ʹ��
#endif








//ͨ�õĶ�������״̬����߼� �����
#include "FML/Key/JHAL_IRKeyListeningTask.c"


#ifdef ADPD_IIC_ID
#include "FML/ADPD188BI/AdpdDrv.c"
#include "FML/ADPD188BI/smoke_detect.c"
#endif
#include "FML/ADC/ADS111x/ads1110.c"
#include "FML/ADC/ADS111x/ads1115.c"

#include "FML/THTB/NTC/NTC.c"
#include "FML/THTB/SHTxx/SHT3x.c"

//�ʵ� RGB led
#include "FML/LED/XL5050RGBC/XL5050RGBC_WS2812B.c"
//����������
#include "FML/Display/HT1621/HT1621.c"
//OLED-0.96/0.91������
#include "FML/Display/SSD1306/SSD1306.c"



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


 



OS_WEAK UpdateDateTime

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

void JHAL_error(char  * msg)
{
    char   debugMsg [50];

    memcpy(debugMsg,msg,sizeof(debugMsg));


    //__ARM_ARCH_7M__ Cortex-M3�ںˣ�__ARM_ARCH_7EM__��ʾARM Cortex-M4��Cortex-M7�������ܹ�
#if (defined __ARM_ARCH_7M__) ||(defined __ARM_ARCH_7EM__)
    if (CoreDebug->DHCSR & 1)    //check C_DEBUGEN == 1 -> Debugger Connected
    {
        __breakpoint(0);  // halt program execution here
        return;
    }
#else
    u32 timeout=0x7FFFFF;
    while(--timeout!=0)
    {
        if(timeout==0x7FFFFF)
        {
            return;
        }
    }
    JHAL_systemReset();
#endif


}











