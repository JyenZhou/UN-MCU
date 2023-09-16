#include "LED_8.h"
u8 smgduan[16]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
                };//0~F 数码管段选数据

								
								
								#define uchar unsigned char
#define uint unsigned int

//接线说明：//SEG接P0.0;//P1.0~1.7接CS7~0//P2.0~2.7接LED0~7//P3.0~3.7接D7~0


uchar num;
uchar num2;
uchar const table[]= {0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6}; //段
uchar const tablewei[]= {0xef,0xdf,0xbf,0x7f,0xfe,0xfd,0xfb,0xf7,0xf7,0xf7}; //位
uchar const liushui[]= {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0x7F,0x7F}; //流水灯



/*******************************************************************************
* 函 数 名         : smg_init
* 函数功能		   : 数码管端口初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void smg_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;  //声明一个结构体变量，用来初始化GPIO
    /* 开启GPIO时钟 */

  __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin =smg_duan;

    /*  配置GPIO的模式和IO口 */
	
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
}

void static_smg_display()	//静态数码管显示
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
//数码管及流水灯实验**************************
//杭州朗迅科技有限公司************************
//作者：LXX***********************************
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