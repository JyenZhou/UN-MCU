
#include "ssd1306.h"




bool __ssd1306WiteCmds(SSD1306  *config,uint8_t *cmd,u8 cmdLength)
{
    bool status=true;
    for(u8 i=0; i<cmdLength; i++) {
        status&=  JHAL_i2csfMemWrite(&config->i2c,0,false,cmd,1);
        cmd++;
    }
    return status;
}

bool __ssd1306WiteData(SSD1306  *config,uint8_t data)
{
    return  JHAL_i2csfMemWrite(&config->i2c,0x40,false,&data,1);
}


//���ÿ�ʼ��ʾλ��
void ssd1306SetPos( SSD1306  *dev, u8 x, u8 y)
{
    u8 cmd[3]= {0xb0+y,((x&0xf0)>>4)|0x10,(x&0x0f)|0x01};
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}

#define SSD1306_CMD_DISPLAY_OFF                             0xAE        /**< command display off */ 
#define SSD1306_CMD_DISPLAY_ON                              0xAF        /**< command display on */ 
//����OLED��ʾ
void  ssd1306DisplayOnOff( SSD1306  *dev,  bool isOn )
{   //SET DCDC���� //DCDC ON 	//DISPLAY ON
    u8 cmd[3]= {0x8d,0x14,0xaf};
		if(!isOn)
		{
			cmd[1]=0x10;
			cmd[2]=0xae;
		}
    __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
}
 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void ssd1306DisplayClear(SSD1306  *dev)
{
    //����ҳ��ַ��0~7��;  //������ʾλ�á��е͵�ַ  //������ʾλ�á��иߵ�ַ
    u8 cmd[3]= {0xb0,0x00,0x10};
    u8 page= page<dev->oledHigth/8;
    for(  ; page>0; --page)
    {
        cmd[0]=0xb0+page ;
        __ssd1306WiteCmds(dev,cmd,sizeof(cmd));
        for(u8 n=0; n<dev->oledWidth; n++)
        {
            __ssd1306WiteData(dev,1);
        }
    }
}

void ss1306Draw(  SSD1306  *dev,u8 x,   u8 y,  u8 width, u8 height,const u8  *data)
{
    //ͨ���ַ�����Զ�����ʹ�ö�Ӧҳ
    u8 page=( height/8)+   (u8)(height%8!=0);
    for(u8 p=0; p<page ; p++)
    {
        ssd1306SetPos(dev,x,y+p);
        for (u8 j=0; j<width; j++)
        {
            __ssd1306WiteData(dev, data[width*p+j]);
        }
    }
}
 

void ssd1306displayChar(SSD1306  *dev,uint8_t x,uint8_t y,uint8_t chr)
{
    //�õ�ƫ�ƺ��ֵ
    ss1306Draw(dev,x,y,dev->fontWidth, dev->fontHigth,&dev->font[ chr-' ']);
}

void ssd1306DisplayNumber(SSD1306 *dev, uint8_t x, uint8_t y,double num, bool isRightAligned,  u8 integerLength,u8 decimalLength) {

    char numStr[integerLength+1+decimalLength];  // �����������Ϊx+2λ (С��. /0)
    char formatString[10];

    // �����ʽ���ַ���
    snprintf(formatString, sizeof(formatString), "%d.%dlf", integerLength, decimalLength);

    // ʹ�� snprintf ���ɸ�ʽ���ĸ������ַ���
    snprintf(numStr, sizeof(numStr), formatString, num);

    if (isRightAligned) {
        int totalWidth = strlen(numStr) * dev->fontWidth;
        x -= totalWidth;
    }

    for (int i = 0; numStr[i] != '\0'; i++) {
        ssd1306displayChar(dev, x, y, numStr[i]);
        x += dev->fontWidth;
        // �����߼�
        if (x > dev->oledWidth - dev->fontWidth) {
            x = 0;
            y += dev->fontHigth;
        }
    }
}




// ��ʾ�ַ���
void ssd1306DisplayString(SSD1306 *dev, uint8_t x, uint8_t y, const char *str) {
    unsigned char j = 0;

    while (str[j] != '\0') {
        ssd1306displayChar(dev, x, y, str[j]);
        x += dev->fontWidth;  // ����ÿ���ַ��Ŀ��Ϊ fontWidth
        // �����߼�
        if (x > dev->oledWidth-dev->fontWidth) {
            x = 0;
            y += dev->fontHigth;  // ����ÿ���ַ��ĸ߶�Ϊ fontHigth
        }
        j++;
    }
}


 
 

//��ʼ��SSD1306
bool ssd1306Open(SSD1306  *dev)
{

    JHAL_i2csfOpen(&dev->i2c);

    if( dev->font== NULL||dev->fontWidth+dev->fontHigth==0)
    {
        dev->font=jfont8x16;
			dev->fontWidth=8;
			dev->fontHigth=16;
			
    }

    if(  dev->oledWidth+dev->oledHigth==0)
    {
        dev->oledWidth=128;
        dev->oledHigth=64;
    }


 
		    u8 cmd[]={
0xAE, 0x20, 0x10, 0xb0, 0xc8, 0x00, 0x10, 0x40, 0x81, 0xff, 0xA1,
0xA6, 0xA8, 0x3F, 0xA4, 0xD3, 0x00, 0xD0, 0xF0, 0xD9, 0x22, 0xDA,
0x12, 0xDB, 0x20, 0x8D, 0x14, 0xAF};
				 
							/*
							
							0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xD8, 0x30, 0x8D, 0x14, 0xAF}; 
							
							*/
							
    if(!    __ssd1306WiteCmds(dev,cmd,sizeof(cmd)))
    {
        return false;
    }




    ssd1306DisplayClear(dev);
    ssd1306SetPos(dev,0,0);
    return true;
}

