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

#include "ssd1306.h"
 
#define SSD1306_CMD  0	//写命令
#define SSD1306_DATA 0x40	//写数据
 
 
 
//#include "OLED_Font.h"
 

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


 
 
 

 

 bool __ssd1306WiteCmds(SSD1306  *config,uint8_t *cmd,u8 cmdLength)
{  
		bool status=true;
	for(u8 i=0;i<cmdLength;i++){
   	status&=  JHAL_i2csfMemWrite(&config->i2c,SSD1306_CMD,false,cmd,1); 
		cmd++;
	}
	return status;
}

 bool __ssd1306WiteData(SSD1306  *config,uint8_t data)
{  
    return  JHAL_i2csfMemWrite(&config->i2c,SSD1306_DATA,false,&data,1); 
}


//设置开始显示位置 
void ssd1306SetPos( SSD1306  *dev, u8 x, u8 y)
{
	u8 cmd[3]={0xb0+y,((x&0xf0)>>4)|0x10,(x&0x0f)|0x01};
	  __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}
//开启OLED显示
void  ssd1306DisplayShow( SSD1306  *dev,  ADS1115Channel ch )
{ //SET DCDC命令 //DCDC ON 	//DISPLAY ON
	u8 cmd[3]={0x8d,0x14,0xaf};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}
//关闭OLED显示
void ssd1306DisplayHide(SSD1306  *dev)
{ 
 
 //SET DCDC命令 //DCDC OFF //DISPLAY OFF
	u8 cmd[3]={0x8d,0x10,0xae};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd) );
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void ssd1306DisplayClear(SSD1306  *dev)
{
 
	//设置页地址（0~7）;  //设置显示位置—列低地址  //设置显示位置—列高地址
	u8 cmd[3]={0xb0,0x00,0x10};
    for(u8 page=0; page<8; page++)
    {
			cmd[0]=0xb0+page ;
  			 __ssd1306WiteCmds(dev,cmd,sizeof(cmd));     	
        for(u8 n=0; n<128; n++)
			{
				 __ssd1306WiteData(dev,!dev->themeStyle);
			}
    } 
}
 
  int getIntegerLength(int a)   //判断一个整数是几位数  0也代表1位 
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
void ss1306displayChar(SSD1306  *dev,uint8_t x,uint8_t y,uint8_t chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值
     
        ssd1306SetPos(dev,x,y);
	//通过字符宽度自动计算使用对应页
	u8 page=( dev->fontWidth/8)+   (u8)(dev->fontWidth%8!=0);
	
        for(i=0; i<dev->fontRowLength ; i++)
			{
					 __ssd1306WiteData(dev,!dev->font[c*dev->fontColumnLength+i]);  
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

void ss1306displayNumber(SSD1306  *dev, uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
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
                ss1306displayChar(dev,x+(size/2)*t,y,' ');
                continue;
            }
            else enshow=1;
        }
        ss1306displayChar(dev,x+(size/2)*t,y,temp+'0');
    } 
} //假如 为 double e = 23.6666;
//小数部分 ： double r = e - (int) e;
//显示小数  用取整数 加小数点 加小数 这种隔离的方式
void OLED_MinNum(SSD1306  *dev,uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y坐标        值            保留几位小数
    int Zgsu;//用来储存整数部分
    double XnSu1;//用来储存小数部分
    int XnSu10;//用来储存小数放大部分 *10 即保留一位小数
    int WwZi;//整数位置（个数）

    Zgsu=(int)Z;
    WwZi=getIntegerLength(Zgsu);//取整数位数函数
    ss1306displayNumber(dev,X,Y,Zgsu,WwZi,16);//写整数
    if((Z!=0)||(BsX!=0))
    {
        ss1306displayChar(X+(8*WwZi),Y,'.');
        XnSu1=Z-(int)Z;//取小数
        XnSu10=XnSu1*pow(10,BsX);//表示10的BsX次方
        ss1306displayNumber(dev,X+8+(8*WwZi),Y,XnSu10,BsX,16);//写小数
    }
}
//显示一个字符号串
void OLED_ShowString(SSD1306  *dev,uint8_t x,uint8_t y,uint8_t *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        ss1306displayChar(dev,x,y,chr[j]);
        x+=8;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}
/*
//显示汉字 传动态一维数组
void OLED_ShowCHinese2(SSD1306  *dev,uint8_t x,uint8_t y,uchar *p)
{
	
	
    uint8_t t,adder=0;
    ssd1306SetPos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(p[t],OLED_DATA);
        adder+=1;
    }
    ssd1306SetPos(x,y+1);
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
    ssd1306SetPos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
        adder+=1;
    }
    ssd1306SetPos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
        adder+=1;
    }
}
*/
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
/*
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;
    for(y=y0; y<y1; y++)
    {
        ssd1306SetPos(x0,y);
        for(x=x0; x<x1; x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}
*/
 

//初始化SSD1306
bool ssd1306Open(SSD1306  *dev)
{

  JHAL_i2csfOpen(dev->i2c)
	
	 if(dev->fontSize==SSD1306_FontSize_8x16)
		 
	 {
		 dev->__other->foint=jfont8x16;
	 }else{
		 
		  dev->__other->foint=jfont6x8;
	 }
 
		bool status=true;
	 	u8 cmd[]={0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,0xA6,
		0xA8,0x3f,0xD3,0x00,0xd5,0x80,0xD9,0xF1,
			0xDA,0x12,0xDB,0x40,0x20,0x02,0x8D,0x14,0xA4,0xA6,0xAF
		};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
	 
 
 
 
    ssd1306DisplayClear();
    ssd1306SetPos(0,0);
}

