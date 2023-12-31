#include "../JHAL_GPIO.h"

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
GPIO_TypeDef * __JHAL_jport2port(JHAL_IO_Port port )
{



    if(port==JHAL_IOA) {
        return  	 GPIOA;
    } else if(port==JHAL_IOB) {
        return  	 GPIOB;
    } else if(port==JHAL_IOC) {
        return  	 GPIOC;
    } else if(port==JHAL_IOD) {
        return  	 GPIOD;
    } else if(port==JHAL_IOE) {
        return  	 GPIOE;
    }
#if !defined STM32F103xB
    else if(port==JHAL_IOF) {
        return  	 GPIOF;
    } else if(port==JHAL_IOG) {
        return  	 GPIOG;
    }

#endif
    else {

       JHAL_Fault_Handler();
			return  GPIOA;
    }



}
JHAL_IO_Port __JHAL_port2jport(GPIO_TypeDef * port ) {

    if(port==GPIOA ) {
        return  	 JHAL_IOA;
    } else if(port==GPIOB ) {
        return  	 JHAL_IOB;
    } else if(port==GPIOC ) {
        return  	 JHAL_IOC;
    } else if(port==GPIOD ) {
        return  	 JHAL_IOD;
    } else if(port==GPIOE ) {
        return  	 JHAL_IOE;
    }
#if !defined STM32F103xB
    else if(port==GPIOF ) {
        return  	 JHAL_IOF;
    } else if(port==GPIOG ) {
        return  	 JHAL_IOG;
    }
#endif
    else {

        while(1);
    }


}

 

u16 __JHAL_jpin2pin ( u8 pin )
{


    return 1<< pin ;


}
u8  JHAL_pin2jpin ( u16 pin )
{

    int position = 0;
    while (!(pin & 0x01)) {
        pin >>= 1;
        position++;
    }

    return position;

}
//读取IO状态  true高电平 false低电平
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{

    return  HAL_GPIO_ReadPin(__JHAL_jport2port(port ),__JHAL_jpin2pin(pin));
}
//设置IO电平
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
    HAL_GPIO_WritePin(__JHAL_jport2port(port ),__JHAL_jpin2pin(pin),(GPIO_PinState)level);
}
//gpio模式设置  0输入 1输出 2开漏
void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,JHAL_IO_MODE mode)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = __JHAL_jpin2pin(pin);


    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    if(JHAL_IO_IN==mode ) {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    } else if(JHAL_IO_IN_EXTI_FET==mode) {
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;

    } else if(JHAL_IO_IN_EXTI_RET==mode) {

        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    } else if(JHAL_IO_IN_EXTI_FET_RET==mode) {
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;

    } else if(JHAL_IO_PP==mode) {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    } else if(JHAL_IO_PP_UP==mode) {
			  GPIO_InitStruct.Pull =GPIO_PULLUP;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    } else if(JHAL_IO_PP_DOWN==mode) {
			  GPIO_InitStruct.Pull =GPIO_PULLDOWN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    } 
		
		else if(JHAL_IO_OD==mode) {

        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    } else if(JHAL_IO_OD_UP==mode) {
        GPIO_InitStruct.Pull =GPIO_PULLUP;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;

    } else if(JHAL_IO_OD_DOWN==mode) {

        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull =GPIO_PULLDOWN;
    }



    HAL_GPIO_Init(__JHAL_jport2port(port ), &GPIO_InitStruct);

}
//IO翻转
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    HAL_GPIO_TogglePin(__JHAL_jport2port(port ),__JHAL_jpin2pin(pin));
}

OS_WEAK void JHAL_gpioInterruptCallback()
{

}

OS_WEAK void JHAL_gpio4IRKeyInterruptCallback()
{

}


