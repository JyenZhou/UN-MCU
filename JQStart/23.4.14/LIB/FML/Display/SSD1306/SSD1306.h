//////////////////////////////////////////////////////////////////////////////////

//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3

//******************************************************************************/
#ifndef __SSD1306_H
#define __SSD1306_H

#include "JHAL.h"


 

typedef enum
{
    SSD1306_Mode_4PinIIC,
    SSD1306_Mode_7PinSPI,

} SSD1306_Mode;



typedef enum
{

    SSD1306_ThemeStyle_White=1,
    SSD1306_ThemeStyle_Black=0,

} SSD1306_ThemeStyle;






typedef struct
{
    bool isOpen:1;



} __SSD1306__OtherInfo;



typedef struct
{

    JHAL_IO_Port  csPort ;
    u8  csPin;
    JHAL_IO_Port  dcPort ;
    u8  dcPin;
    JHAL_IO_Port  resPort ;
    u8  resPin;
    JHAL_IO_Port  d1Port ;
    u8  d1Pin;
    JHAL_IO_Port  d0Port ;
    u8  d0Pin;

    SSD1306_Mode mode;
    JHAL_I2CSF	i2c;

    SSD1306_ThemeStyle themeStyle;
    //�����ָ���ֿ� Ĭ��ʹ��8*16��  ȡģʱ������С����8�ֽڶ��� �����С����
    const u8 * font;
    u8 fontWidth;
    u8 fontHigth;
    //oled Ĭ�� 0.91���128x32
    u8 oledWidth;
    u8 oledHigth;



    __SSD1306__OtherInfo __other;

} SSD1306;

bool ssd1306Open(SSD1306  *dev);
void ss1306Draw(  SSD1306  *dev,u8 x,   u8 y,  u8 width, u8 height,const u8  *data);
void ssd1306displayChar(SSD1306  *dev,uint8_t x,uint8_t y,uint8_t chr);

#endif




