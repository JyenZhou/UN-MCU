
#ifndef __LCD12864_M_H
#define __LCD12864_M_H
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "lcd12864_zk.h" //我的字库

#define uchar unsigned char
#define uint unsigned int
	

#define CS1_Pin GPIO_PIN_8
#define CS1_GPIO_Port GPIOD
#define CS2_Pin GPIO_PIN_9
#define CS2_GPIO_Port GPIOD
#define RS_Pin GPIO_PIN_10
#define RS_GPIO_Port GPIOD
#define RW_Pin GPIO_PIN_11
#define RW_GPIO_Port GPIOD
#define E_Pin GPIO_PIN_12
#define E_GPIO_Port GPIOD
#define RST_Pin GPIO_PIN_13
#define RST_GPIO_Port GPIOD

#define DB1_Pin GPIO_PIN_0
#define DB1_GPIO_Port GPIOD
#define DB2_Pin GPIO_PIN_1
#define DB2_GPIO_Port GPIOD
#define DB3_Pin GPIO_PIN_2
#define DB3_GPIO_Port GPIOD
#define DB4_Pin GPIO_PIN_3
#define DB4_GPIO_Port GPIOD
#define DB5_Pin GPIO_PIN_4
#define DB5_GPIO_Port GPIOD
#define DB6_Pin GPIO_PIN_5
#define DB6_GPIO_Port GPIOD
#define DB7_Pin GPIO_PIN_6
#define DB7_GPIO_Port GPIOD
#define DB8_Pin GPIO_PIN_7
#define DB8_GPIO_Port GPIOD
	
#define mLCD_CS1(n)  if((n)==0) HAL_GPIO_WritePin(GPIOD, CS1_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, CS1_Pin, GPIO_PIN_SET)//	PDout( 8 )	//传感器模块stm32 LCD12864 CS1 = 0; //开启左半屏
#define mLCD_CS2(n)  if((n)==0) HAL_GPIO_WritePin(GPIOD, CS2_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, CS2_Pin, GPIO_PIN_SET)//	PDout( 9 )	//传感器模块stm32 LCD12864 CS2 = 0; //开启右半屏
#define mLCD_RS(n) 	 if((n)==0) HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_RESET);  else HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_SET) //PDout( 10 )	//传感器模块stm32 LCD12864 RS = 0 命令 / = 1 数据
#define mLCD_RW(n)  	if((n)==0) HAL_GPIO_WritePin(GPIOD, RW_Pin, GPIO_PIN_RESET); else  HAL_GPIO_WritePin(GPIOD, RW_Pin, GPIO_PIN_SET)//PDout( 11 )	//传感器模块stm32 LCD12864 WR = 0 读 / = 1 写
#define mLCD_E(n) 		if((n)==0) HAL_GPIO_WritePin(GPIOD, E_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, E_Pin, GPIO_PIN_SET)//PDout( 12 )	//传感器模块stm32 LCD12864 E = 0 使能
#define mLCD_RST(n)	if((n)==0) HAL_GPIO_WritePin(GPIOD, RST_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, RST_Pin, GPIO_PIN_SET) //PDout( 13 )	//传感器模块stm32 LCD12864 RST
//#define Delay_Ms(n) HAL_Delay(n)
#define mLCD_ReadDatabus GPIOD -> IDR 	//读LCD12864数据总线
#define mLCD_WriteDatabus GPIOD -> ODR 	//写LCD12864数据总线

/* 变量定义 ------------------------------------------------------------------*/


/* 函数声明 ------------------------------------------------------------------*/
//void mLCDCmd_GPIO_Configuration( void );
//void mLCDRead_GPIO_Configuration( void );
//void mLCDWrite_GPIO_Configuration( void );

void mLCD_Reset( void );
void mRead_Busy( void );
void mWrite_LCD_command( uchar value ); //写命令函数
void mWrite_LCD_data( uchar value ); //写数据函数
void mSet_Page( uchar page ); //设置页 LCD12864共8页
void mSet_Line( uchar startline ); //设置显示的起始行	
void mSet_Column( uchar column ); //设置显示的列
void mSet_OnOff( uchar onoff ); //显示开关函数: 0x3e 关显示, 0x3f是开显示
void mSelectScreen( uchar screen );	//选择屏幕
void mClearScreen( uchar screen ); //清屏函数
void mInitLCD( void ); //LCD初始化
void	mDisplay_HZ( uchar ss, uchar page,  uchar *p );	//汉字显示函数	
void mDisplay_Eng( uchar ss, uchar page, uchar column, uchar *p,uchar num );

void mDisplay_9HZ( uchar ss, uchar page, uchar column, uchar *p );


void mdisplay_map( uchar *p );
extern void Delay_Ms( uint16_t time );
extern void DAC0832_ConversionData( uchar data );
void LCD_ShowChar(uint8_t x,uint8_t y,uint32_t chr);
void LCD_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t BL);
void LCD_MinNum(uint8_t X,uint8_t Y, double Z,uint8_t BsX);
void Lcd12864_TestMain(void);





#ifdef __cplusplus
}
#endif

#endif


