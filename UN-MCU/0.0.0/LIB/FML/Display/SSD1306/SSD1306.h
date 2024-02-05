//////////////////////////////////////////////////////////////////////////////////

//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3

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
    //如果不指定字库 默认使用8*16的  取模时候点阵大小必须8字节对齐 字体大小随意
    const u8 * font;
    u8 fontWidth;
    u8 fontHigth;
    //oled 默认 0.91寸的128x32
    u8 oledWidth;
    u8 oledHigth;



    __SSD1306__OtherInfo __other;

} SSD1306;





#endif




