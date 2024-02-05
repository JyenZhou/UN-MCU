#include "../JHAL_GPIO.h"

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
u16 __JHAL_gpio_jio2io(JHAL_IO_Port port ,u8 pin)
{
    if(port==JHAL_IOA) {
       return GPIOA_00+pin;
    } else if(port==JHAL_IOB) {
        return   	 GPIOB_00+pin;
    }  
    else {
       JHAL_Fault_Handler("__JHAL_jport2port");
			return  0;
    }
}
 

 
//��ȡIO״̬  true�ߵ�ƽ false�͵�ƽ
bool  JHAL_gpioReadPin(JHAL_IO_Port port,u8 pin)
{

    return  (bool) GPIO_GetBit(__JHAL_gpio_jio2io(port,pin));
}
//����IO��ƽ
void  JHAL_gpioWitePin(JHAL_IO_Port port,u8 pin,bool level)
{
 
	  GPIO_SetBit(__JHAL_gpio_jio2io(port,pin));
}
//gpioģʽ����  0���� 1��� 2��©
void  JHAL_gpioModeSet(JHAL_IO_Port port,u8 pin,u32 mode)
{

    GPIO_INIT_T init_t = {0};
		u16  io=__JHAL_gpio_jio2io(port,pin);
		
     GPIO_CLR_AFMODE(io);
     init_t.gpio = io;
		
     
    init_t.ext_mode = GPIO_PIN_PUSH_PULL;
   
		
		u32  modeIo=__JHAL_IOMODE_IO_Mask&mode;
			if(modeIo==JHAL_IOMODE_IO_OutputPushPull) {
        init_t.mode = GPIO_OUTPUT_MODE;
				   init_t.ext_mode = GPIO_PIN_PUSH_PULL;

    }else if(modeIo==JHAL_IOMODE_IO_OutputDrain)
			{
				 init_t.mode = GPIO_OUTPUT_MODE;
			   init_t.ext_mode = GPIO_PIN_OPEN_DRAIN;
			}else{
				
				 init_t.mode = GPIO_INPUT_MODE;
				
			}
		
 u32 modePull=  __JHAL_IOMODE_PullResistor_Mask&mode;
						if(modePull==JHAL_IOMODE_PullResistor_Up) {
       
				   init_t.pull_mode = GPIO_PULL_UP;

    }else if(modePull== JHAL_IOMODE_PullResistor_Down)
						{
							   init_t.pull_mode = GPIO_PULL_DOWN;
						}
			
	 
		  GPIO_Init(&init_t);
 

}
//IO��ת
void  JHAL_gpioTogglePin(JHAL_IO_Port port,u8 pin)
{
    GPIO_ToggleBit(__JHAL_gpio_jio2io(port,pin));
}




