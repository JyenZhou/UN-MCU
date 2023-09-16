#include "PT100_MAX31865.h"

/*这是配着PT100温度传感器的版本*/





#define MAX31865_SDI_H	HAL_GPIO_WritePin(MAX31865_PORT_SDI, MAX31865_PIN_SDI,GPIO_PIN_SET)
#define MAX31865_SDI_L 	HAL_GPIO_WritePin(MAX31865_PORT_SDI, MAX31865_PIN_SDI,GPIO_PIN_RESET)
#define MAX31865_SCLK_H 	HAL_GPIO_WritePin(MAX31865_PORT_SCLK, MAX31865_PIN_SCLK,GPIO_PIN_SET)
#define MAX31865_SCLK_L 	HAL_GPIO_WritePin(MAX31865_PORT_SCLK, MAX31865_PIN_SCLK,GPIO_PIN_RESET)



#define MAX31865_SDO_Read   HAL_GPIO_ReadPin(MAX31865_PORT_SDO, MAX31865_PIN_SDO)
#define MAX31865_DRDY_Read  HAL_GPIO_ReadPin(MAX31865_PORT_DRDY, MAX31865_PIN_DRDY)


#ifndef MAX31865_HARD_SPI		/* 定义硬件SPI */
void MAX31865_Delay(volatile u32 nCount);
#endif
uint8_t MAX31865_SB_Read(uint8_t addr);


GPIO_TypeDef* MAX31865_PORT_NCS;
uint16_t MAX31865_PIN_NCS ;




void MAX31865_GPIO_Configuration(void);

float MAX31865_Get_tempture(void);

float tempture;



 


void MAX31865_NCS_GPIO_Set(GPIO_TypeDef* GPIOX,uint16_t GPIO_PinX)
{
MAX31865_PORT_NCS=	 GPIOX;
MAX31865_PIN_NCS=GPIO_PinX;
}


void MAX31865_NCS_H()
{
    HAL_GPIO_WritePin(MAX31865_PORT_NCS, MAX31865_PIN_NCS,GPIO_PIN_SET);
}
void MAX31865_NCS_L()
{
    HAL_GPIO_WritePin(MAX31865_PORT_NCS, MAX31865_PIN_NCS,GPIO_PIN_RESET);
}


//正确时返回00
uint8 MAX31865_Get_Fault_Status()
{
	
return	MAX31865_SB_Read(0x07);
}
volatile uint16_t dtemp[2];
float MAX31865_Get_PT100_tempture(void)//PT100
{
    float temps;

    uint16_t data_temp;
    dtemp[0]=MAX31865_SB_Read(0x01);
    dtemp[1]=MAX31865_SB_Read(0x02);
    data_temp=(dtemp[0]<<7)+(dtemp[1]>>1);//Get 15Bit DATA;
    temps=data_temp;
    temps=(temps*402)/32768;//Here is the rtd R value;
    temps=(temps-100)/0.385055;//A gruad
    return temps;
}



float MAX31865_Get_PT1000_tempture(void)//PT1000
{
  float temps;
  uint16_t dtemp[2];
  uint16_t data_temp;
  dtemp[0]=MAX31865_SB_Read(0x01);
  dtemp[1]=MAX31865_SB_Read(0x02);
  data_temp=(dtemp[0]<<7)+(dtemp[1]>>1);//Get 15Bit DATA;
  temps=data_temp;
  temps=(temps*4020)/32768;//Here is the rtd R value;
  temps=(temps-1000)/0.385055;//A gruad
  return temps;
}


uint8_t MAX31865_SB_Read(uint8_t addr)//SPI Single-Byte Read
{


    uint8_t read = 0;

#ifndef MAX31865_HARD_SPI		 

    MAX31865_NCS_L();
    MAX31865_Delay(0x1f);
    for(uint8_t i = 0; i < 8; i++)
    {
        MAX31865_SCLK_H;
        if (addr & 0x80) {
            MAX31865_SDI_H;
        }
        else {
            MAX31865_SDI_L;
        }
        MAX31865_Delay(0x1f);
        MAX31865_SCLK_L;
        addr <<= 1;
        MAX31865_Delay(0x1f);
    }

    MAX31865_Delay(0x1f);
    for (uint8_t i = 0; i < 8; i++)
    {
        MAX31865_SCLK_H;
        read = read<<1;
        MAX31865_Delay(0x1f);
        if(MAX31865_SDO_Read==GPIO_PIN_SET)
        {
            read++;
        }
        MAX31865_SCLK_L;
        MAX31865_Delay(0x1f);
    }
    MAX31865_NCS_H();
#else

    HAL_SPI_Receive(&MAX31865_SPI,&read,1,100);

#endif
    return read;
}

void MAX31865_SB_Write(uint8_t addr,uint8_t wdata)//SPI Single-Byte Write
{
#ifndef MAX31865_HARD_SPI		 
    MAX31865_NCS_L();
    MAX31865_Delay(0x1f);
    for(uint8_t i = 0; i < 8; i++)
    {
        MAX31865_SCLK_H;
        if (addr & 0x80) {
            MAX31865_SDI_H;
        }
        else {
            MAX31865_SDI_L;
        }
        MAX31865_Delay(0x1f);
        MAX31865_SCLK_L;
        addr <<= 1;
        MAX31865_Delay(0x1f);
    }

    for(uint8_t i = 0; i < 8; i++)
    {
        MAX31865_SCLK_H;
        if (wdata & 0x80) {
            MAX31865_SDI_H;
        }
        else {
            MAX31865_SDI_L;
        }
        MAX31865_Delay(0x1f);
        MAX31865_SCLK_L;
        wdata <<= 1;
        MAX31865_Delay(0x1f);
    }
    MAX31865_NCS_H();
#else

    HAL_SPI_Transmit(&MAX31865_SPI,&wdata,1,100);

#endif


}
#ifndef MAX31865_HARD_SPI 
void MAX31865_CfgSpiSOFTGPIO(void)
{
    MAX31865_RCC_SCLK;
    MAX31865_RCC_SDI 	;
    MAX31865_RCC_SDO ;

    /* 配置SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = MAX31865_PIN_SDI;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MAX31865_PORT_SDI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MAX31865_PIN_SCLK;
    HAL_GPIO_Init(MAX31865_PORT_SCLK, &GPIO_InitStruct);


    /* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
    GPIO_InitStruct.Pin = MAX31865_PIN_SDO;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(MAX31865_PORT_SDO, &GPIO_InitStruct);



}


void MAX31865_Delay(volatile u32 nCount)
{
	
    for(; nCount != 0; nCount--);
    uint32_t i;for (i = 0; i < 2; i++);
}



#endif

void MAX31865_GPIO_Configuration(void)
{

	
	
#ifndef MAX31865_HARD_SPI	 
    MAX31865_CfgSpiSOFTGPIO();
#endif

    /* 打开GPIO时钟 */

    MAX31865_RCC_DRDY 	;
    MAX31865_RCC_NCS ;


	
	MAX31865_NCS_GPIO_Set(MAX31865_PORT_NCS_Default,MAX31865_PIN_NCS_Default);
    /* 配置几个推完输出IO */

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = MAX31865_PIN_NCS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MAX31865_PORT_NCS, &GPIO_InitStruct);





    /* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = MAX31865_PIN_DRDY;
    HAL_GPIO_Init(MAX31865_PORT_DRDY, &GPIO_InitStruct);
}


void PT100_max3185_init(void)
{
    MAX31865_GPIO_Configuration();

 
    MAX31865_SB_Write(0x80,0xC1);//二线、四线配置
    //MAX31865_SB_Write(0x80,0xD1);//三线配置

    //MAX31865_SB_Write(0x80,0xFF);//Set High Fault Threshold MSB
    //MAX31865_SB_Write(0x80,0xFF);//Set High Fault Threshold LSB
    //MAX31865_SB_Write(0x80,0x00);//Set Low Fault Threshold MSB
    //MAX31865_SB_Write(0x80,0x00);//Set Low Fault Threshold LSB
}
 volatile uint8_t Fault_Status;
void unitTest_PT100_max3185(void)
{
   
 
PT100_max3185_init();
    HAL_Delay(500);
    while (1)
    {
        HAL_Delay(500);
        tempture=MAX31865_Get_PT100_tempture();
        Fault_Status=MAX31865_SB_Read(0x07);//Get Fault_Status 0x00是OK
     //   printf("%d\n",Fault_Status);
    }
}

