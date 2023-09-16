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
#ifndef __OLED_H
#define __OLED_H

#include "JHAL.h"


//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64
//-----------------OLED�˿ڶ���----------------

#define   __HAL_RCC_GPIOX_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define OLED_CS_Pin GPIO_PIN_0
#define OLED_CS_GPIO_Port GPIOE

#define OLED_DS_Pin GPIO_PIN_2
#define OLED_DS_GPIO_Port GPIOE

#define OLED_RES_Pin GPIO_PIN_4
#define OLED_RES_GPIO_Port GPIOE

#define OLED_D1_SDA_Pin GPIO_PIN_6
#define OLED_D1_SDA_GPIO_Port GPIOE

#define OLED_D0_SCL_Pin GPIO_PIN_8
#define OLED_D0_SCL_GPIO_Port GPIOE



#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET)//CS  0
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_SET)//CS  1

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DS_GPIO_Port,OLED_DS_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DS_GPIO_Port,OLED_DS_Pin,GPIO_PIN_SET)

//#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
//#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

//#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
//#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  
//ʹ��4�ߴ��нӿ�ʱʹ��

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_D0_SCL_GPIO_Port,OLED_D0_SCL_Pin,GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set()  HAL_GPIO_WritePin(OLED_D0_SCL_GPIO_Port,OLED_D0_SCL_Pin,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_D1_SDA_GPIO_Port,OLED_D1_SDA_Pin,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_D1_SDA_GPIO_Port,OLED_D1_SDA_Pin,GPIO_PIN_SET)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
static void OLED_GPIO_Init(void);
void  OLED_main(void);


void OLED_XlDvBuSu(void);
void OLED_XlDvBuSu2(void);
void OLED_XlDvBuSu3(void);
void OLED_JtDvBuSu(void);
void OLED_JtDvBuSu2(void);
void OLED_JtDvBuSu3(void);
void OLED_V_Motor1(void);
void OLED_V_Motor2(void);
void OLED_V_Motor3(void);


#endif




