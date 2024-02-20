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

        JHAL_error("__JHAL_jport2port");
        return  GPIOA;
    }



}
JHAL_IO_Port JHAL_gpioPort2jport(void * port ) {

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

        JHAL_error("__JHAL_port2jport");
        return JHAL_IOA;
    }


}



u16 __JHAL_jpin2pin ( u8 pin )
{


    return 1<< pin ;


}
u8  JHAL_gpioPin2jpin ( u16 pin )
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
void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,u32 mode)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = __JHAL_jpin2pin(pin);




    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;




    if((mode&__JHAL_IOMODE_EXTI_Trigger_Mask)==JHAL_IOMODE_EXTI_Trigger_Low_Falling) {
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;

    } else if((__JHAL_IOMODE_EXTI_Trigger_Mask&mode)==JHAL_IOMODE_EXTI_Trigger_High_Rising) {

        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    } else if((__JHAL_IOMODE_EXTI_Trigger_Mask&mode)==JHAL_IOMODE_EXTI_Trigger_HightLow_Rising_Falling) {
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;

    } else if((mode&__JHAL_IOMODE_IO_Mask) ==JHAL_IOMODE_IO_Input ) {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    } else if((mode&__JHAL_IOMODE_IO_Mask)==JHAL_IOMODE_IO_OutputPushPull) {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    }
    else if((mode&__JHAL_IOMODE_IO_Mask)==JHAL_IOMODE_IO_OutputDrain) {

        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    }

    GPIO_InitStruct.Pull =(	__JHAL_IOMODE_PullResistor_Mask&mode)/JHAL_IOMODE_PullResistor_Up;
    HAL_GPIO_Init(__JHAL_jport2port(port ), &GPIO_InitStruct);

}
//IO翻转
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    HAL_GPIO_TogglePin(__JHAL_jport2port(port ),__JHAL_jpin2pin(pin));
}




