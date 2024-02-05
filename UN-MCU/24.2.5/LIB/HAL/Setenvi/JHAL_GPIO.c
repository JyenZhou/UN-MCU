#include "../JHAL_GPIO.h"
 #include "asm32lx003_gpio.h"
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
 //��Ч��GPIO
 while(1);
       
}
u8 __JHAL_jpin2Pin(u8 pin)
{
	 
		 //��Ч��PIN
 while(pin>7);
    return  1<<pin;
}

//��ȡIO״̬  true�ߵ�ƽ false�͵�ƽ
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{
    return (bool)GPIO_ReadInputDataBit(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin));
}
//����IO��ƽ
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
  GPIO_WriteBit(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin),(BitAction)level);
}
//gpioģʽ����  0���� 1��� 2��©
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
//IO��ת
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    GPIO_ToggleBits(__JHAL_jio2IO(port), __JHAL_jpin2Pin(pin));
}

