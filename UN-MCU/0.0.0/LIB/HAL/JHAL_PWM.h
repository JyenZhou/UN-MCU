/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		PWMĬ��ͨ��HAL����

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_PWM__H__
#define __JHAL_PWM__H__
#include "JUTIL.h"
#include "JHAL_GPIO.h"
#ifdef __CplusPlus
extern "C" {
#endif

    typedef enum {
        JHAL_PWM_750HZ=0,
        JHAL_PWM_4HZ,
    } JHAL_PWM_Frequency;

    //JHAL_DEV_INIT
    typedef struct {
        bool isOpen:1;

    } __JHAL_PWM_OtherInfo;

    typedef struct {
        //ʵ���ϵ�ͨ��
        u8  channel;
        //��ʼʱ��ռ��ֵ
        u16 initialValue;
        //һ����Ƕ�ʱ����װ��ֵ
        u16 maxValue;
        JHAL_Align align;

    } JHAL_PWM_Channel;


    /*JHAL_PWM��ʹ�õ�PWM ӳ������ŵ���Ϣ��c��  prescaler:Ԥ��Ƶ���� ��Ӧʱ�Ӽ�c��   InitialValue����ʼֵ maxValue�����ֵ
    ע��:ʹ�ù��������õ�ֵ���ܴ��ڳ�ʼ���������ֵ
    */
    typedef struct {


        u8 id:3;
        u8 channelsNumber:4;
			  //�����ǵ�ͨ��ʵ��ֵ  ��Ϊ�˷���ʹ�ö�Ӧ��ö��/������.c����ע���ţ�
        JHAL_PWM_Channel *channels;
      

        JHAL_PWM_Frequency frequency;
        __JHAL_PWM_OtherInfo __info;
        void*  dev;

    } JHAL_PWM;



    bool JHAL_pwmOpen(JHAL_PWM *pwm );
    bool  JHAL_pwmClose(JHAL_PWM *pwm);
//���ﴫ����ͨ����������ʵ��ֵ��JHAL_PWM�ж���
    void JHAL_pwmSetValue(JHAL_PWM *pwm,u8 channelIndex,u16 value);
		//
		 

#ifdef CplusPlus
}
#endif

#endif








