
#ifndef __LCD12864_M_H
#define __LCD12864_M_H
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "lcd12864_zk.h" //�ҵ��ֿ�

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
	
#define mLCD_CS1(n)  if((n)==0) HAL_GPIO_WritePin(GPIOD, CS1_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, CS1_Pin, GPIO_PIN_SET)//	PDout( 8 )	//������ģ��stm32 LCD12864 CS1 = 0; //���������
#define mLCD_CS2(n)  if((n)==0) HAL_GPIO_WritePin(GPIOD, CS2_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, CS2_Pin, GPIO_PIN_SET)//	PDout( 9 )	//������ģ��stm32 LCD12864 CS2 = 0; //�����Ұ���
#define mLCD_RS(n) 	 if((n)==0) HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_RESET);  else HAL_GPIO_WritePin(GPIOD, RS_Pin, GPIO_PIN_SET) //PDout( 10 )	//������ģ��stm32 LCD12864 RS = 0 ���� / = 1 ����
#define mLCD_RW(n)  	if((n)==0) HAL_GPIO_WritePin(GPIOD, RW_Pin, GPIO_PIN_RESET); else  HAL_GPIO_WritePin(GPIOD, RW_Pin, GPIO_PIN_SET)//PDout( 11 )	//������ģ��stm32 LCD12864 WR = 0 �� / = 1 д
#define mLCD_E(n) 		if((n)==0) HAL_GPIO_WritePin(GPIOD, E_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, E_Pin, GPIO_PIN_SET)//PDout( 12 )	//������ģ��stm32 LCD12864 E = 0 ʹ��
#define mLCD_RST(n)	if((n)==0) HAL_GPIO_WritePin(GPIOD, RST_Pin, GPIO_PIN_RESET); else HAL_GPIO_WritePin(GPIOD, RST_Pin, GPIO_PIN_SET) //PDout( 13 )	//������ģ��stm32 LCD12864 RST
//#define Delay_Ms(n) HAL_Delay(n)
#define mLCD_ReadDatabus GPIOD -> IDR 	//��LCD12864��������
#define mLCD_WriteDatabus GPIOD -> ODR 	//дLCD12864��������

/* �������� ------------------------------------------------------------------*/


/* �������� ------------------------------------------------------------------*/
//void mLCDCmd_GPIO_Configuration( void );
//void mLCDRead_GPIO_Configuration( void );
//void mLCDWrite_GPIO_Configuration( void );

void mLCD_Reset( void );
void mRead_Busy( void );
void mWrite_LCD_command( uchar value ); //д�����
void mWrite_LCD_data( uchar value ); //д���ݺ���
void mSet_Page( uchar page ); //����ҳ LCD12864��8ҳ
void mSet_Line( uchar startline ); //������ʾ����ʼ��	
void mSet_Column( uchar column ); //������ʾ����
void mSet_OnOff( uchar onoff ); //��ʾ���غ���: 0x3e ����ʾ, 0x3f�ǿ���ʾ
void mSelectScreen( uchar screen );	//ѡ����Ļ
void mClearScreen( uchar screen ); //��������
void mInitLCD( void ); //LCD��ʼ��
void	mDisplay_HZ( uchar ss, uchar page,  uchar *p );	//������ʾ����	
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


