//////////////////////////////////////////////////////////////////////////////////

//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3
//              ----------------------------------------------------------------

//******************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "HALL_MyBSP\BoardLoadingPeripheral\LCD12864\lcd12864_zk.h"
//#include "OLED_Font.h"
#include "BMP.h"
#include "math.h"

static uint8_t Y1=0;
//static uint8_t Y2=1;
static uint8_t Y3=2;

//static uint8_t Y4=3;
static uint8_t  Y5=4;
//static uint8_t  Y6=5;
static uint8_t  Y7=6;
//static uint8_t  Y8=7;

//#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127


#if OLED_MODE==1
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;

void OLED_WR_Byte(u8 dat,u8 cmd)
{
    DATAOUT(dat);
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    OLED_WR_Clr();
    OLED_WR_Set();
    OLED_CS_Set();
    OLED_DC_Set();
}
#else
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i;
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    for(i=0; i<8; i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
}
#endif
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA);
    } //更新显示
}

static int function(int a)   //判断一个数是几位数
{
    int n=0;
    while (a)
    {
        a = a / 10;
        n++;
    }
    if(a==0)//此函数本用于小数中自动取整数部分个数，这里是使输入0也能返回个整数0
        return n+1;
    return n;
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值
    if(x>Max_Column-1)
    {
        x=0;
        y=y+2;
    }
    if(SIZE ==16)
    {
        OLED_Set_Pos(x,y);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x,y+1);
        for(i=0; i<6; i++)
            OLED_WR_Byte(F6x8[c][i],OLED_DATA);

    }
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//num:数值(0~4294967295);
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ');
                continue;
            }
            else enshow=1;

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0');
    }
} //假如 为 double e = 23.6666;
//小数部分 ： double r = e - (int) e;
//显示小数  用取整数 加小数点 加小数 这种隔离的方式
void OLED_MinNum(uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y坐标        值            保留几位小数
    int Zgsu;//用来储存整数部分
    double XnSu1;//用来储存小数部分
    int XnSu10;//用来储存小数放大部分 *10 即保留一位小数
    int WwZi;//整数位置（个数）

    Zgsu=(int)Z;
    WwZi=function(Zgsu);//取整数位数函数
    OLED_ShowNum(X,Y,Zgsu,WwZi,16);//写整数
    if((Z!=0)||(BsX!=0))
    {
        OLED_ShowChar(X+(8*WwZi),Y,'.');
        XnSu1=Z-(int)Z;//取小数
        XnSu10=XnSu1*pow(10,BsX);//表示10的BsX次方
        OLED_ShowNum(X+8+(8*WwZi),Y,XnSu10,BsX,16);//写小数
    }
}
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

//显示汉字 传动态一维数组
void OLED_ShowCHinese2(uint8_t x,uint8_t y,uchar *p)
{
	
	
    uint8_t t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(p[t],OLED_DATA);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(p[t+16],OLED_DATA);
        adder+=1;
    }
}

//显示汉字 传静态数组的坐标
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{
	
	
    uint8_t t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
        adder+=1;
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;
    for(y=y0; y<y1; y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=x0; x<x1; x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}


//初始化SSD1306
void OLED_Init(void)
{

    OLED_GPIO_Init();//GPIO初始化

    OLED_RST_Set();
    HAL_Delay(100);
    OLED_RST_Clr();
    HAL_Delay(100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0,0);
}

static void OLED_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */ 
    __HAL_RCC_GPIOX_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin|OLED_RES_Pin|OLED_DS_Pin|OLED_D0_SCL_Pin
                      |OLED_D1_SDA_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : OLED_CS_Pin OLED_RES_Pin OLED_DS_Pin OLED_D0_SCL_Pin */
    GPIO_InitStruct.Pin = OLED_CS_Pin|OLED_RES_Pin|OLED_DS_Pin|OLED_D0_SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_CS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OLED_D1_SDA_Pin */
    GPIO_InitStruct.Pin = OLED_D1_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_D1_SDA_GPIO_Port, &GPIO_InitStruct);

}
void OLED_main(void)
{
    __IO static uint32_t TimePled;//用于清屏的一个时间记数
    static uint16_t QtJuflus=380;//因为实际情况中不需要全部刷新  全局刷新的设此标志位
    QtJuflus++;

    if(QtJuflus==400)//400*15ms=6s全局刷新的设此标志位
    {
        QtJuflus=0;
        TimePled++;
        if(TimePled==20)//20*15ms=300ms
        {
            OLED_Clear();   //当计数为100时清屏 并25535后到100再清一次
        }
				OLED_ShowCHinese2(0,Y1,dian);
       // OLED_ShowCHinese(0,Y1,0);//电				
        //OLED_ShowCHinese(16,Y1,1);//机

        OLED_ShowCHinese(48,Y1,2);//距
        OLED_ShowCHinese(64,Y1,3);//离

        OLED_ShowCHinese(96,Y1,4);//速
        OLED_ShowCHinese(16*7,Y1,5);//度

        OLED_ShowChar(8,Y3,'M');
        OLED_ShowChar(16,Y3,'1');

        OLED_ShowChar(8,Y5,'M');
        OLED_ShowChar(16,Y5,'2');

        OLED_ShowChar(8,Y7,'M');
        OLED_ShowChar(16,Y7,'3');

    }    //汉字要取模在赋值  行 列  数值


    OLED_XlDvBuSu();//相对步数显示
    OLED_XlDvBuSu2();//相对步数显示
    OLED_XlDvBuSu3();//相对步数显示
//    JtDvBuSu();//绝对步数显示
//    JtDvBuSu2();//绝对步数显示
//    JtDvBuSu3();//绝对步数显示
    OLED_V_Motor1();
    OLED_V_Motor2();
    OLED_V_Motor3();


    //OLED_ShowChar(0,6,OLED_SPeed_count);//显示ASCII字符
//
//		//if(TimePled>'~')TimePled=' ';


    //	OLED_ShowNum(0,6,(-5),6,16);//计算当前步数 1步等于2


    //HAL_Delay(5000);
    //OLED_Clear();
    //	HAL_Delay(80);


//		for(r=0;r<=6;r++)
//		{
//		OLED_DrawBMP(0,0,128,8,xin);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
//				OLED_DrawBMP(0,0,128,8,qi);
//		HAL_Delay(800);


}



/*************************************************************************************************
* @函数功能：把步进电机1的相对步数显示到OLED
* @输入参数：无
* @思路说明：  1.把输出的脉冲正负数从motor.c程序中添加扩展全局变量 2.设一个变量等于正负脉冲之和 3再做判断是否添加负号和取绝对值
* @返回值：无
* @重点说明：无
* @示例说明：

//x,y :起点坐标
//num:数值(0~4294967295);
//len :数字的位数
//size:字体大小
* ******************************************************************
*/
void OLED_XlDvBuSu(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率

    if(OLED_SPeed_count!=0)//防止0时候做无用功 省资源  并能在O时候清除负号
    {
        if(SP==1)//负（下降）时标记判断
        {

            OLED_ShowChar(XWwZi-8,Y3,'-');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//计算当前步数 1步等于2
            //OLED_MinNum(XWwZi,Y3,(OLED_SPeed_count/(799)),1);//计算当前步数 1步等于2
            SP=1;
            Pull_Counnt=0;
        }
        if(SP==2)
        {
            OLED_ShowChar(XWwZi-8,Y3,'+');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//计算当前步数 1步等于2
            Pull_Counnt=0;
        }
    }
    else  //即当0时候 去符号
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            OLED_ShowChar(XWwZi-8,Y3,' ');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//计算当前步数 1步等于2
        }
    }

}


/*************************************************************************************************
* @函数功能：把步进电机2的相对步数显示到OLED
* @输入参数：无
* @思路说明：  1.把输出的脉冲正负数从motor.c程序中添加扩展全局变量 2.设一个变量等于正负脉冲之和 3再做判断是否添加负号和取绝对值
* @返回值：无
* @重点说明：无
* @示例说明：

//x,y :起点坐标
//num:数值(0~4294967295);
//len :数字的位数
//size:字体大小
* ******************************************************************
*/
void OLED_XlDvBuSu2(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率
    if(OLED_SPeed_count2!=0)//防止0时候做无用功 省资源  并能在O时候清除负号
    {
        if(SP2==1)//负（下降）时标记判断
        {

            OLED_ShowChar(XWwZi-8,Y5,'-');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//计算当前步数 1步等于2
            SP2=1;
            Pull_Counnt=0;
        }
        if(SP2==2)
        {
            OLED_ShowChar(XWwZi-8,Y5,'+');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//计算当前步数 1步等于2
            Pull_Counnt=0;
        }
    }
    else  //即当0时候 去符号
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            OLED_ShowChar(XWwZi-8,Y5,' ');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//计算当前步数 1步等于2
        }
    }

}



/*************************************************************************************************
* @函数功能：把步进电机3的相对步数显示到OLED
* @输入参数：无
* @思路说明：  1.把输出的脉冲正负数从motor.c程序中添加扩展全局变量 2.设一个变量等于正负脉冲之和 3再做判断是否添加负号和取绝对值
* @返回值：无
* @重点说明：无
* @示例说明：

//x,y :起点坐标
//num:数值(0~4294967295);
//len :数字的位数
//size:字体大小
* ******************************************************************
*/
void OLED_XlDvBuSu3(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率
    if(OLED_SPeed_count3!=0)//防止0时候做无用功 省资源  并能在O时候清除负号
    {
        if(SP3==1)//负（下降）时标记判断
        {

            OLED_ShowChar(XWwZi-8,Y7,'-');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//计算当前步数 1步等于2
            SP3=1;
            Pull_Counnt=0;
        }
        if(SP3==2)
        {
            OLED_ShowChar(XWwZi-8,Y7,'+');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//计算当前步数 1步等于2
            Pull_Counnt=0;
        }
    }
    else  //即当0时候 去符号
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt==1;
            OLED_ShowChar(XWwZi-8,Y7,' ');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//计算当前步数 1步等于2
        }
    }

}




/*************************************************************************************************
* @函数功能：把步进电机的绝对步数显示到OLED
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void JtDvBuSu(void)
{

    __IO static uint8_t LooK1=0;//目前只定义出来相对步数最多移动8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//用来记录绝对步数
    __IO static uint8_t F1=0;//用来转换相对步数
    __IO static double   XWwZi=56;

    if(JtDv==1)//在main中定义的绝对步数标志
    {
        if(OLED_SPeed_count!=0)//防止0时候做无用功 省资源
        {


            //	if(SP==1)//反转标志

            if(SP==2)//正转标志
            {


                if(OLED_SPeed_count/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1+1;  //初始值+1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//负（下降）时标记判断
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1-1;  //初始值-1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y3,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y3,F1,2,16);//计算当前步数 1步等于2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y3,'+');
        OLED_ShowNum(XWwZi,Y3,Sw1,2,16);//计算当前步数 1步等于2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y3,' ');
        OLED_ShowNum(XWwZi,Y3,Sw1,2,16);//计算当前步数 1步等于2

    }



}



/*************************************************************************************************
* @函数功能：把步进电机的绝对步数显示到OLED
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void JtDvBuSu2(void)
{

    __IO static uint8_t LooK1=0;//目前只定义出来相对步数最多移动8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//用来记录绝对步数
    __IO static uint8_t F1=0;//用来转换相对步数
    __IO static double   XWwZi=56;
    if(JtDv==1)//在main中定义的绝对步数标志
    {
        if(OLED_SPeed_count2!=0)//防止0时候做无用功 省资源
        {


            //	if(SP==1)//反转标志

            if(SP==2)//正转标志
            {


                if(OLED_SPeed_count2/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1+1;  //初始值+1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count2/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count2/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count2/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count2/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count2/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count2/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count2/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//负（下降）时标记判断
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count2/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1-1;  //初始值-1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count2/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count2/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count2/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count2/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count2/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count2/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count2/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y5,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y5,F1,2,16);//计算当前步数 1步等于2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y5,'+');
        OLED_ShowNum(XWwZi,Y5,Sw1,2,16);//计算当前步数 1步等于2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y5,' ');
        OLED_ShowNum(XWwZi,Y5,Sw1,2,16);//计算当前步数 1步等于2

    }



}


/*************************************************************************************************
* @函数功能：把步进电机的绝对步数显示到OLED
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void JtDvBuSu3(void)
{

    __IO static uint8_t LooK1=0;//目前只定义出来相对步数最多移动8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//用来记录绝对步数
    __IO static uint8_t F1=0;//用来转换相对步数
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率

    if(JtDv==1)//在main中定义的绝对步数标志
    {
        if(OLED_SPeed_count3!=0)//防止0时候做无用功 省资源
        {


            //	if(SP==1)//反转标志

            if(SP==2)//正转标志
            {


                if(OLED_SPeed_count3/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1+1;  //初始值+1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count3/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count3/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count3/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count3/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count3/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count3/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count3/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//负（下降）时标记判断
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count3/(799)==1)
                {
                    if(LooK1==0)//当防止重复进入标志关闭时
                    {
                        Sw1=Sw1-1;  //初始值-1
                        LooK1=1;//锁住 防止799到下个799之间一直进入
                        LooK2=0;//解锁后面所有  因为1是开始、
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count3/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//在上面减过的基础上再减1
                        LooK1=0;
                        LooK2=1;//因为递增除以出来等于2  就不会再等于1  所以解锁1
                    }
                }

                if(OLED_SPeed_count3/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count3/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count3/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count3/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count3/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count3/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y7,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y7,F1,2,16);//计算当前步数 1步等于2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y7,'+');
        OLED_ShowNum(XWwZi,Y7,Sw1,2,16);//计算当前步数 1步等于2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y7,' ');
        OLED_ShowNum(XWwZi,Y7,Sw1,2,16);//计算当前步数 1步等于2

    }



}



/*************************************************************************************************
* @函数功能：速度显示子函数  可调节   圈/s   圈/min
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void OLED_V_Motor1(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率
    if(OLED_SPeed_count!=0)//即电机在运动
    {
        timmer1++;
        V_Motor=(((OLED_SPeed_count)/799)/(timmer1*0.015));//*60;//乘以60就是圈每分钟
        OLED_MinNum(XWwZi,Y3,V_Motor,2);//显示速度
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y3,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y3,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*2),Y3,' ');	//指定位清零处理 因为上面是取三位小数并且实际情况整数位只有一个例1.235
            OLED_ShowChar(XWwZi+(8*3),Y3,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*4),Y3,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
        }
    }
}

/*************************************************************************************************
* @函数功能：速度显示子函数  可调节   圈/s   圈/min
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void OLED_V_Motor2(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率
    if(OLED_SPeed_count2!=0)//即电机在运动
    {
        timmer1++;
        //V_Motor=((OLED_SPeed_count/799)/(timmer1*0.015));//*60;//乘以60就是圈每分钟
        V_Motor=((OLED_SPeed_count2/799)/(timmer1*0.015));//*60;//乘以60就是圈每分钟
        OLED_MinNum(XWwZi,Y5,V_Motor,2);//显示速度
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y5,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y5,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*2),Y5,' ');	//指定位清零处理 因为上面是取三位小数并且实际情况整数位只有一个例1.235
            OLED_ShowChar(XWwZi+(8*3),Y5,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*4),Y5,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
        }
    }
}


/*************************************************************************************************
* @函数功能：速度显示子函数  可调节   圈/s   圈/min
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例说明：无
* **************@原创;六安职业技术学院星期天创客工坊*********************************************************
*/
void OLED_V_Motor3(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//减少在0时刷新率
    if(OLED_SPeed_count3!=0)//即电机在运动
    {
        timmer1++;
        //V_Motor=((OLED_SPeed_count/799)/(timmer1*0.015));//*60;//乘以60就是圈每分钟
        V_Motor=((OLED_SPeed_count3/799)/(timmer1*0.015));//*60;//乘以60就是圈每分钟
        OLED_MinNum(XWwZi,Y7,V_Motor,2);//显示速度
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y7,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y7,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*2),Y7,' ');	//指定位清零处理 因为上面是取三位小数并且实际情况整数位只有一个例1.235
            OLED_ShowChar(XWwZi+(8*3),Y7,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
            OLED_ShowChar(XWwZi+(8*4),Y7,' ');	//共5位  我们清除前4位即可 后面留0（实际情况只要处理1和第5个在保留三位小数下第4-1位自动为0 由上一行代码执行）
        }
    }
}
