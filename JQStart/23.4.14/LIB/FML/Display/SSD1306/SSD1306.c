//////////////////////////////////////////////////////////////////////////////////

//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3
//              ----------------------------------------------------------------

//******************************************************************************/

#include "ssd1306.h"
 
#define SSD1306_CMD  0	//д����
#define SSD1306_DATA 0x40	//д����
 
 
 
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
//OLED���Դ�
//��Ÿ�ʽ����.
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


//���ÿ�ʼ��ʾλ�� 
void ssd1306SetPos( SSD1306  *dev, u8 x, u8 y)
{
	u8 cmd[3]={0xb0+y,((x&0xf0)>>4)|0x10,(x&0x0f)|0x01};
	  __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}
//����OLED��ʾ
void  ssd1306DisplayShow( SSD1306  *dev,  ADS1115Channel ch )
{ //SET DCDC���� //DCDC ON 	//DISPLAY ON
	u8 cmd[3]={0x8d,0x14,0xaf};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}
//�ر�OLED��ʾ
void ssd1306DisplayHide(SSD1306  *dev)
{ 
 
 //SET DCDC���� //DCDC OFF //DISPLAY OFF
	u8 cmd[3]={0x8d,0x10,0xae};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd) );
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void ssd1306DisplayClear(SSD1306  *dev)
{
 
	//����ҳ��ַ��0~7��;  //������ʾλ�á��е͵�ַ  //������ʾλ�á��иߵ�ַ
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
 
  int getIntegerLength(int a)   //�ж�һ�������Ǽ�λ��  0Ҳ����1λ 
{
    int n=0;
    while (a)
    {
        a = a / 10;
        n++;
    }
    if(a==0)//�˺���������С�����Զ�ȡ�������ָ�����������ʹ����0Ҳ�ܷ��ظ�����0
        return n+1;
    return n;
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12
void ss1306displayChar(SSD1306  *dev,uint8_t x,uint8_t y,uint8_t chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//�õ�ƫ�ƺ��ֵ
     
        ssd1306SetPos(dev,x,y);
	//ͨ���ַ�����Զ�����ʹ�ö�Ӧҳ
	u8 page=( dev->fontWidth/8)+   (u8)(dev->fontWidth%8!=0);
	
        for(i=0; i<dev->fontRowLength ; i++)
			{
					 __ssd1306WiteData(dev,!dev->font[c*dev->fontColumnLength+i]);  
			}				
}
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ2������
//x,y :�������
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ

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
} //���� Ϊ double e = 23.6666;
//С������ �� double r = e - (int) e;
//��ʾС��  ��ȡ���� ��С���� ��С�� ���ָ���ķ�ʽ
void OLED_MinNum(SSD1306  *dev,uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y����        ֵ            ������λС��
    int Zgsu;//����������������
    double XnSu1;//��������С������
    int XnSu10;//��������С���Ŵ󲿷� *10 ������һλС��
    int WwZi;//����λ�ã�������

    Zgsu=(int)Z;
    WwZi=getIntegerLength(Zgsu);//ȡ����λ������
    ss1306displayNumber(dev,X,Y,Zgsu,WwZi,16);//д����
    if((Z!=0)||(BsX!=0))
    {
        ss1306displayChar(X+(8*WwZi),Y,'.');
        XnSu1=Z-(int)Z;//ȡС��
        XnSu10=XnSu1*pow(10,BsX);//��ʾ10��BsX�η�
        ss1306displayNumber(dev,X+8+(8*WwZi),Y,XnSu10,BsX,16);//дС��
    }
}
//��ʾһ���ַ��Ŵ�
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
//��ʾ���� ����̬һά����
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

//��ʾ���� ����̬���������
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
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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
 

//��ʼ��SSD1306
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

