#ifndef _SMG_H
#define _SMG_H
#include "JHAL.h"
#define smg_duan (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7)//PC0~PC7 

void smg_init(void);//����ܳ�ʼ��
void static_smg_display(void);	//��̬�������ʾ

#endif
