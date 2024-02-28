#include "../JHAL_GPIO.h"
#include <xl_gpio.h>
#include <xl_sim.h>
#include <xl_irq.h>
#include "xl_nvic.h"
/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		GPIO�ķ�װ ���㲻ͬƽ̨ʱ��Ӱ��APL��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

//GPIO�����ת��
u8 __JHAL_jio2io(JHAL_IO_Port port,u8 pin)
{
    return  (port-1)*8+pin;
}


//��ȡIO״̬  true�ߵ�ƽ false�͵�ƽ
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{
    return GPIO_ReadPin(__JHAL_jio2io(port,pin));
}
//����IO��ƽ
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
    if(level)
    {
        GPIO_SetPin(__JHAL_jio2io(port,pin));
    } else {
        GPIO_ClrPin(__JHAL_jio2io(port,pin));
    }
}
//gpioģʽ����  0���� 1��� 2��©
void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,u32 mode)
{
    JHAL_IOMODE_IO  io=  (JHAL_IOMODE_IO) (__JHAL_IOMODE_IO_Mask&mode);
    if(JHAL_IOMODE_IO_Input==io )
    {

        GPIO_SetPinDir(__JHAL_jio2io(port,pin),GPIO_Direction_Input);

        JHAL_IOMODE_EXTI_Trigger  trigger= (JHAL_IOMODE_EXTI_Trigger)(__JHAL_IOMODE_EXTI_Trigger_Mask &mode);
        if(trigger==JHAL_IOMODE_EXTI_Trigger_High_Rising||JHAL_IOMODE_EXTI_Trigger_Low_Falling==trigger)
        {
            u8 _port=port-1;
            SIM_SCGC_Cmd(SIM_SCGC_IRQ,ENABLE);   //IRQʱ��ʹ��
            //�ֲ�247ҳ
            SIM_PINSEL_IRQ((u8)(_port/2));	//ӳ���4������ PTGx + PTHx  IRQ_PS_GROUP4


            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn+((_port%2)*8)+pin;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
            NVIC_Init(&NVIC_InitStruct); //�ж�-��������


            IRQ_InitTypeDef IRQ_InitStruct;
            IRQ_InitStruct.IRQ_EdgeSelect =trigger==JHAL_IOMODE_EXTI_Trigger_High_Rising? IRQ_Riseedge_Highlevel:IRQ_Falledge_Lowlevel;
            IRQ_InitStruct.IRQ_ModeSelect = IRQ_Detect_Edge;
            //define  EXTI0                 0                     < �ⲿ�ж�ͨ��0.
            IRQ_Init(IRQ,((_port%2)*8)+ pin, &IRQ_InitStruct); //�жϴ�������  �½���
            IRQ_PinEnableCmd(IRQ, ((_port%2)*8)+pin, ENABLE); //����ʹ��
            IRQ_InterruptEn(IRQ, ((_port%2)*8)+pin, ENABLE);//�ж�ʹ��
            IRQ_PullDevDisableCmd(IRQ,((_port%2)*8)+pin,ENABLE);
        }


    } else if(mode==JHAL_IOMODE_IO_OutputPushPull)
    {
        GPIO_SetPinDir(__JHAL_jio2io(port,pin),GPIO_Direction_Output);
    } else
    {
        GPIO_SetPinDir(__JHAL_jio2io(port,pin),GPIO_Direction_High_Z);
    }

}
//IO��ת
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    GPIO_TogglePin(__JHAL_jio2io(port,pin));
}



/**
 * @brief  �жϺ���
 */
void EXTI0_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI0))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI0);   //����жϱ�־λ
    }

}

void EXTI1_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI1))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI1);   //����жϱ�־λ
    }

}

void EXTI2_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI2))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI2);   //����жϱ�־λ
    }

}

void EXTI3_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI3))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI3);   //����жϱ�־λ
    }

}

void EXTI4_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI4))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI4);   //����жϱ�־λ
    }

}

void EXTI5_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI5))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI5);   //����жϱ�־λ
    }

}

void EXTI6_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI6))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI6);   //����жϱ�־λ
    }

}

void EXTI7_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI7))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI7);   //����жϱ�־λ
    }

}

void EXTI8_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI0))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI8);   //����жϱ�־λ
    }

}

void EXTI9_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI9))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI9);   //����жϱ�־λ
    }

}

void EXTI10_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI10))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI10);   //����жϱ�־λ
    }

}

void EXTI11_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI11))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI11);   //����жϱ�־λ
    }

}

void EXTI12_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI12))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI12);   //����жϱ�־λ
    }

}

void EXTI13_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI13))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI13);   //����жϱ�־λ
    }

}

void EXTI14_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI14))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI14);   //����жϱ�־λ
    }

}

void EXTI15_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI15))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI15);   //����жϱ�־λ
    }

}




