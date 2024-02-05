#include "S4_74HC595.h"
uint8   fseg[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint8   segbit[]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
uint8 disbuf[4]= {0,0,0,0};


//-----------------------------------------------------------------------------

static uint8 LED[8];	//用于LED的8位显示缓存
// LED字模表
uint8   LED_0F[] =
{   // 0	 1	  2	   3	4	 5	  6	   7	8	 9	 
 	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
 //A	   b	C    d	  E    F    - 
 0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf,
// r(error)
 0xAF, 0x2F

//这是倒立180°的 0-9
 //   0xC0,0xCF,0xA4,	0x86,	0x8B,0x92,0x90,0xC7,0x80,0x82,0xFF,
};


void S4_74HC595_init (void)
{
    DigitalTube_74HC595__GPIO_DIO_ENABLE();
    DigitalTube_74HC595__GPIO_SCLK_ENABLE();
    DigitalTube_74HC595__GPIO_RCLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = DigitalTube_74HC595_DIO_GPIO_Pin;
    HAL_GPIO_Init(DigitalTube_74HC595_DIO_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DigitalTube_74HC595_RCLK_GPIO_Pin;
    HAL_GPIO_Init(DigitalTube_74HC595_RCLK_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DigitalTube_74HC595_SCLK_GPIO_Pin;
    HAL_GPIO_Init(DigitalTube_74HC595_SCLK_GPIO_Port, &GPIO_InitStruct);

LED3_Display(0,0);
LED3_Display(1,0);
LED3_Display(2,0);
}




static void RCLK( uint8 pinState)
{
    if(pinState==0)
    {
        HAL_GPIO_WritePin(DigitalTube_74HC595_RCLK_GPIO_Port,DigitalTube_74HC595_RCLK_GPIO_Pin,GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(DigitalTube_74HC595_RCLK_GPIO_Port,DigitalTube_74HC595_RCLK_GPIO_Pin,GPIO_PIN_SET);
    }
}
static void DIO( uint8 pinState)
{
    if(pinState==0)
    {
        HAL_GPIO_WritePin(DigitalTube_74HC595_DIO_GPIO_Port,DigitalTube_74HC595_DIO_GPIO_Pin,GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(DigitalTube_74HC595_DIO_GPIO_Port,DigitalTube_74HC595_DIO_GPIO_Pin,GPIO_PIN_SET);
    }
}
static void SCLK( uint8 pinState)
{
    if(pinState==0)
    {
        HAL_GPIO_WritePin(DigitalTube_74HC595_SCLK_GPIO_Port,DigitalTube_74HC595_SCLK_GPIO_Pin,GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(DigitalTube_74HC595_SCLK_GPIO_Port,DigitalTube_74HC595_SCLK_GPIO_Pin,GPIO_PIN_SET);
    }
}
static void DIGLED_OUT(uchar X)
{
    uchar i;
    for(i=8; i>=1; i--)
    {
        if (X&0x80) DIO(1);
        else DIO(0);
        X<<=1;
        SCLK (0);
        SCLK ( 1);
    }
}



void LOAD_DIGLED(void)
{
    RCLK(1);
    HAL_Delay(5);
    RCLK(0)  ;

}
/*3位显示  参数1 指定的数码管，2 值的数组下表*/
void LED3_Display(uint8 bit, uint8 value)
{
    LED[bit]=LED_0F[value];

    DIGLED_OUT(LED[2]);
    DIGLED_OUT(LED[1]);
    DIGLED_OUT(LED[0]);
    LOAD_DIGLED();

}
/*4位的未测试*/
void LED4_Display (void)
{
    unsigned char   *led_table;          // 查表指针
    uchar i;
    //显示第1位
    led_table = LED_0F + LED[0];
    i = *led_table;

    DIGLED_OUT(i);
    DIGLED_OUT(0x01);

    RCLK(0);
    RCLK(1);
    //显示第2位
    led_table = LED_0F + LED[1];
    i = *led_table;

    DIGLED_OUT(i);
    DIGLED_OUT(0x02);
    RCLK(0);
    RCLK(1);
    //显示第3位
    led_table = LED_0F + LED[2];
    i = *led_table;

    DIGLED_OUT(i);
    DIGLED_OUT(0x04);

    RCLK(0);
    RCLK(1);
    //显示第4位
    led_table = LED_0F + LED[3];
    i = *led_table;

    DIGLED_OUT(i);
    DIGLED_OUT(0x08);
    RCLK(0);
    RCLK(1);
}



void DigitalTube_74HC595_test (void)
{

//	LED[0]=1;
//	LED[1]=2;
//	LED[2]=3;
//	LED[3]=4;
//LED4_Display();

    for(uint8 i=0; i<=10; i++)
    {
        LED3_Display (0,i);
        LED3_Display (1,i);
        LED3_Display (2,10-i);
        HAL_Delay(2000);
    }


}


