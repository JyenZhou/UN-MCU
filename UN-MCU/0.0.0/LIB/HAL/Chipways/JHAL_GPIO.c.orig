#include "../JHAL_GPIO.h"
#include <xl_gpio.h>
#include <xl_sim.h>
#include <xl_irq.h>
#include "xl_nvic.h"
/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		GPIO的封装 方便不同平台时候不影响APL层
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

//GPIO定义的转换
u8 __JHAL_jio2io(JHAL_IO_Port port,u8 pin)
{
    return  (port-1)*8+pin;
}


//读取IO状态  true高电平 false低电平
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{
    return GPIO_ReadPin(__JHAL_jio2io(port,pin));
}
//设置IO电平
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
    if(level)
    {
        GPIO_SetPin(__JHAL_jio2io(port,pin));
    } else {
        GPIO_ClrPin(__JHAL_jio2io(port,pin));
    }
}
//gpio模式设置  0输入 1输出 2开漏
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
            SIM_SCGC_Cmd(SIM_SCGC_IRQ,ENABLE);   //IRQ时钟使能
            //手册247页
            SIM_PINSEL_IRQ((u8)(_port/2));	//映射第4组引脚 PTGx + PTHx  IRQ_PS_GROUP4


            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn+((_port%2)*8)+pin;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
            NVIC_Init(&NVIC_InitStruct); //中断-》处理函数


            IRQ_InitTypeDef IRQ_InitStruct;
            IRQ_InitStruct.IRQ_EdgeSelect =trigger==JHAL_IOMODE_EXTI_Trigger_High_Rising? IRQ_Riseedge_Highlevel:IRQ_Falledge_Lowlevel;
            IRQ_InitStruct.IRQ_ModeSelect = IRQ_Detect_Edge;
            //define  EXTI0                 0                     < 外部中断通道0.
            IRQ_Init(IRQ,((_port%2)*8)+ pin, &IRQ_InitStruct); //中断触发设置  下降沿
            IRQ_PinEnableCmd(IRQ, ((_port%2)*8)+pin, ENABLE); //引脚使能
            IRQ_InterruptEn(IRQ, ((_port%2)*8)+pin, ENABLE);//中断使能
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
//IO翻转
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    GPIO_TogglePin(__JHAL_jio2io(port,pin));
}

 

/**
 * @brief  中断函数
 */
void EXTI0_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI0))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI0);   //清除中断标志位
    }

}

void EXTI1_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI1))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI1);   //清除中断标志位
    }

}

void EXTI2_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI2))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI2);   //清除中断标志位
    }

}

void EXTI3_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI3))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI3);   //清除中断标志位
    }

}

void EXTI4_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI4))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI4);   //清除中断标志位
    }

}

void EXTI5_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI5))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI5);   //清除中断标志位
    }

}

void EXTI6_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI6))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI6);   //清除中断标志位
    }

}

void EXTI7_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI7))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI7);   //清除中断标志位
    }

}

void EXTI8_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI0))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI8);   //清除中断标志位
    }

}

void EXTI9_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI9))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI9);   //清除中断标志位
    }

}

void EXTI10_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI10))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI10);   //清除中断标志位
    }

}

void EXTI11_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI11))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI11);   //清除中断标志位
    }

}

void EXTI12_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI12))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI12);   //清除中断标志位
    }

}

void EXTI13_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI13))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI13);   //清除中断标志位
    }

}

void EXTI14_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI14))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI14);   //清除中断标志位
    }

}

void EXTI15_IRQHandler(void)
{
    if(IRQ_GetFlag(IRQ, EXTI15))
    {

        JHAL_gpioInterruptCallback();
        JHAL_gpio4IRKeyInterruptCallback();
        IRQ_ClrFlag(IRQ, EXTI15);   //清除中断标志位
    }

}




