#ifndef __IS31FL3733__H__
#define __IS31FL3733__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

// 101 00 00 X
#define DeviceAddress 0x50
#define Write 1
#define Rade 1

//�������� �Ĵ���
#define ConfigureCommandRegister 0xFD
//����Ĵ���д��
#define CommandRegisterWriteLock  0xFE
//�ж����μĴ���
#define InterruptMaskRegister  0xF0
//�ж�״̬�Ĵ���
#define InterruptStatusRegister   0xF1


//LED���� �Ĵ��� 
#define LEDCOntrolRegister 0x00
//PWM �Ĵ���
#define PWMRegister 0x01
//�Զ�����ģʽ �Ĵ���
#define AutoBreathModeRegister 0x02
//���� �Ĵ���
#define FunctionRegister 0x03



typedef enum
{
	FirstWrite=0xFF,
	Nthof_Write=0xFE,
	Last_Write=0xEE,
	Once_Write=0xEF
}WhichoneWrite_TypeDefE;
//����ȷ���ǵڼ���д������    ��һ��д���������  ��һ��Ҫ����IIC��д��ַ  �������ֱ��д����
//Nthof_Write ����Ҫд
//Once_Write ����ֻдһ��
//���һ����Stop

typedef enum
{
	ON=0x01,
	OFF=0x00
}ONOrOFF_TypeDefE;
//���ڶ������ƵĿ���

typedef enum
{
	Left,
	Reight
}Direction_TypeDefE;
//�����а�߷������λ�ÿ��Ƶ�ö�� 


void IS31FL3733_Testmain (void);
#ifdef CplusPlus
}
#endif

#endif


