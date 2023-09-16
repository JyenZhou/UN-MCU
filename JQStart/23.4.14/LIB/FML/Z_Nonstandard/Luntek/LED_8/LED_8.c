#include "LED_8.h"
u8 smgduan[16]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
                };//0~F ����ܶ�ѡ����

								
								
								#define uchar unsigned char
#define uint unsigned int

//����˵����//SEG��P0.0;//P1.0~1.7��CS7~0//P2.0~2.7��LED0~7//P3.0~3.7��D7~0


uchar num;
uchar num2;
uchar const table[]= {0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6}; //��
uchar const tablewei[]= {0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xf7,0xf7}; //λ
uchar const liushui[]= {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0x7F,0x7F}; //��ˮ��



/*******************************************************************************
* �� �� ��         : smg_init
* ��������		   : ����ܶ˿ڳ�ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void smg_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;  //����һ���ṹ�������������ʼ��GPIO
    /* ����GPIOʱ�� */

  __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin =smg_duan;

    /*  ����GPIO��ģʽ��IO�� */
	
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
}

void static_smg_display()	//��̬�������ʾ
{
    u8 i;
    for(i=0; i<16; i++)
    {
      //  HAL_GPIO_TogglePin(GPIOC,(u16)(~smgduan[i]));
			 HAL_GPIO_TogglePin(GPIOC,(u16)(smgduan[i]));
        delay_ms(1000);
    }
}
//********************************************
//����ܼ���ˮ��ʵ��**************************
//������Ѹ�Ƽ����޹�˾************************
//���ߣ�LXX***********************************
//********************************************



void delay(uint ms)
{
    uint i,j;
    for(i=ms; i>0; i--)
        for(j=110; j>0; j--);
}
void testmain()
{
    while(1)
    {
        for(num=0; num<10; num++)
        {
            P1=tablewei[num];
            duan=1;
            P3=table[num];
            P2=liushui[num];
            delay(1000);
            duan=0;
        }
    }

}