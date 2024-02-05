#include "../JHAL_GPIO.h"
 #include "asm32lx003_gpio.h"
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
GPIO_TypeDef* __JHAL_jio2IO(JHAL_IO_Port port)
{
 if(port==JHAL_IOA)
 {
	 return GPIOA;
 }else   if(port==JHAL_IOB)
 {
	  return GPIOB;
 }else   if(port==JHAL_IOC)
 {
	  return GPIOC;
 }else   if(port==JHAL_IOD)
 {
	  return GPIOD;
 }
 //无效的GPIO
 while(1);
       
}
u8 __JHAL_jpin2Pin(u8 pin)
{
	 
		 //无效的PIN
 while(pin>7);
    return  1<<pin;
}

//读取IO状态  true高电平 false低电平
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{
    return (bool)GPIO_ReadInputDataBit(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin));
}
//设置IO电平
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
  GPIO_WriteBit(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin),(BitAction)level);
}
//gpio模式设置  0输入 1输出 2开漏
void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,JHAL_IO_MODE mode)
{
 GPIO_InitTypeDef GPIO_InitStructor;
    
if(mode==JHAL_IO_PP)
{
	GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructor.GPIO_OType = GPIO_OType_PP;
	 
}else if(mode==JHAL_IO_OD){
		GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructor.GPIO_OType = GPIO_OType_OD;
}else if(mode==JHAL_IO_OD_UP){
			GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructor.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructor.GPIO_PuPd  = GPIO_PuPd_UP;
}else if(mode==JHAL_IO_OD_DOWM){
				GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructor.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructor.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
}
	
	GPIO_InitStructor.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructor.GPIO_Pin   = __JHAL_jpin2Pin(pin);
	GPIO_Init(__JHAL_jio2IO(port), &GPIO_InitStructor);  //
	
 
}
//IO翻转
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    GPIO_ToggleBits(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin));
}

