
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
       18位高精度AD
 

          
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/








/*
SCK:SCLK
MISO：DOUT
MOSI：CS

注意这个 MOSI是不接的

*/

#include "ad7608.h"

//#define AD7606_HARD_SPI		 

#ifndef AD7606_HARD_SPI		/* 定义硬件SPI */
	#define SCK_0()		HAL_GPIO_WritePin(AD7606_PORT_SCK, AD7606_PIN_SCK,GPIO_PIN_RESET)
	#define SCK_1()		HAL_GPIO_WritePin(AD7606_PORT_SCK, AD7606_PIN_SCK,GPIO_PIN_SET)

	#define MOSI_0()	HAL_GPIO_WritePin(AD7606_PORT_MOSI, AD7606_PIN_MOSI,GPIO_PIN_RESET)
	#define MOSI_1()	HAL_GPIO_WritePin(AD7606_PORT_MOSI, AD7606_PIN_MOSI,GPIO_PIN_SET)

	#define AD7606_MISO_PinState()	HAL_GPIO_ReadPin(AD7606_PORT_MISO, AD7606_PIN_MISO)
	
	
	/*
*********************************************************************************************************
*	函 数 名: SpiDelay
*	功能说明: 时序延迟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void spiDelay(void)
{
	uint32_t i;

	for (i = 0; i < 2; i++);
}
#endif

 



#define AD7606_BUSY_PinState				HAL_GPIO_ReadPin(AD7606_PORT_BUSY, AD7606_PIN_BUSY)
 
static void AD7606_ConfigGPIO(void);
void AD7606_Reset(void);	
void AD7606_SetInputRange(uint8_t _ucRange);
void AD7606_StartConv(void);
void AD7606_CfgSpiSOFTGPIO(void);

volatile int32_t AD7608_ADValue[8];

AD7606_T g_tAD7606;




/*
*********************************************************************************************************
*	函 数 名: InitAD7606
*	功能说明: 初始化AD7606 SPI口线
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void spi_InitAD7606(uint8 g_tAD7606_Range )
{
	
	
	g_tAD7606.Range=g_tAD7606_Range;
	#ifndef AD7606_HARD_SPI
	AD7606_CfgSpiSOFTGPIO();
#endif
 
	AD7606_ConfigGPIO();		/* 配置GPIO */
	


	
	AD7606_SetInputRange(g_tAD7606.Range);	/* 设置采样量程 */

	/* 设置过采样模式 */
	AD7606_SetOS(0);
	AD7606_Reset();				/* 硬件复位复AD7606 */
	
	AD7606_CONVST_HIGH();			/* CONVST脚设置为高电平 */	
}
/*
*********************************************************************************************************
*	函 数 名: AD7606_ConfigGPIO
*	功能说明: 配置GPIO。 不包括 SCK  MOSI  MISO 共享的SPI总线。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
 static void AD7606_ConfigGPIO( )
{


	
	/* 打开GPIO时钟 */
  AD7606_RCC_CS 		   ;
  AD7606_RCC_RESET 	;
  AD7606_RCC_RANGE 	;
  AD7606_RCC_CONVST 	  ;
  AD7606_RCC_BUSY ;
    AD7606_RCC_OS2;
  AD7606_RCC_OS1;
  AD7606_RCC_OS0;
	
	/* 配置几个推完输出IO */
 
 		
		 
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 
	  GPIO_InitStruct.Pin = AD7606_PIN_RESET;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD7606_PORT_RESET, &GPIO_InitStruct);

 
    GPIO_InitStruct.Pin = AD7606_PIN_CONVST;
    HAL_GPIO_Init(AD7606_PORT_CONVST, &GPIO_InitStruct);
 
    GPIO_InitStruct.Pin = AD7606_PIN_RANGE;
    HAL_GPIO_Init(AD7606_PORT_RANGE, &GPIO_InitStruct);

	
    GPIO_InitStruct.Pin = AD7606_PIN_CS;
    HAL_GPIO_Init(AD7606_PORT_CS, &GPIO_InitStruct);
	
     GPIO_InitStruct.Pin = AD7606_PIN_OS2;
    HAL_GPIO_Init(AD7606_PORT_OS2, &GPIO_InitStruct);
 

     GPIO_InitStruct.Pin = AD7606_PIN_OS1;
    HAL_GPIO_Init(AD7606_PORT_OS1, &GPIO_InitStruct);
		
     GPIO_InitStruct.Pin = AD7606_PIN_OS0;
    HAL_GPIO_Init(AD7606_PORT_OS0, &GPIO_InitStruct);
	


	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pin = AD7606_PIN_BUSY;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(AD7606_PORT_BUSY, &GPIO_InitStruct);
		

}

#ifndef AD7606_HARD_SPI
/*
*********************************************************************************************************
*	函 数 名: AD7606_CfgSpiSOFTGPIO
*	功能说明: 配置软件模式的GPIO。 SCK  MOSI  MISO 共享的SPI总线。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AD7606_CfgSpiSOFTGPIO(void)
{
		AD7606_RCC_SCK;
AD7606_RCC_MISO;
AD7606_RCC_MOSI;
		/* 配置SPI引脚SCK、MISO 和 MOSI为推挽模式 */
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 
	  GPIO_InitStruct.Pin = AD7606_PIN_MOSI;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD7606_PORT_MOSI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD7606_PIN_SCK;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD7606_PORT_SCK, &GPIO_InitStruct);
	
/* MISO 设置为输入上拉 */
    GPIO_InitStruct.Pin = AD7606_PIN_MISO;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(AD7606_PORT_MISO, &GPIO_InitStruct);

	
	
}
#endif

/*
*********************************************************************************************************
*	函 数 名: ad7606_SetOS
*	功能说明: 设置过采样模式（数字滤波，硬件求平均值)
*	形    参：_ucMode : 0-6  0表示无过采样，1表示2倍，2表示4倍，3表示8倍，4表示16倍
*				5表示32倍，6表示64倍
*	返 回 值: 无
*********************************************************************************************************
*/
#if 1
void AD7606_SetOS(uint8_t _ucMode)
{
	if (_ucMode == 1)
	{
		AD7606_OS2_0();
		AD7606_OS1_0();
		AD7606_OS0_1();
	}
	else if (_ucMode == 2)
	{
		AD7606_OS2_0();
		AD7606_OS1_1();
		AD7606_OS0_0();
	}
	else if (_ucMode == 3)
	{
		AD7606_OS2_0();
		AD7606_OS1_1();
		AD7606_OS0_1();
	}
	else if (_ucMode == 4)
	{
		AD7606_OS2_1();
		AD7606_OS1_0();
		AD7606_OS0_0();
	}
	else if (_ucMode == 5)
	{
		AD7606_OS2_1();
		AD7606_OS1_0();
		AD7606_OS0_1();
	}
	else if (_ucMode == 6)
	{
		AD7606_OS2_1();
		AD7606_OS1_1();
		AD7606_OS0_0();
	}
	else	/* 按0处理 */
	{
		AD7606_OS2_0();
		AD7606_OS1_0();
		AD7606_OS0_0();
	}
}
#endif

/*
*********************************************************************************************************
*	函 数 名: AD7606_SetInputRange
*	功能说明: 配置AD7606模拟信号输入量程。
*	形    参: _ucRange : 0 表示正负5V   1表示正负10V
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		AD7606_RANGE_5V();	/* 设置为正负5V */
	}
	else
	{
		AD7606_RANGE_10V();	/* 设置为正负10V */
	}
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_RESET
*	功能说明: 硬件复位
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	/* CS  SCLK拉高 */
#ifdef SOFT_SPI
		AD7606_CS_1();
		//SCK_1();
#endif

 	/* AD7606是高电平复位，要求最小脉宽50ns */
	AD7606_RESET_LOW();
	
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	
	AD7606_RESET_LOW();
}
/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StartConv(void)
{
	/* 上升沿开始转换，低电平持续时间至少25ns  */
	AD7606_CONVST_LOW();
	AD7606_CONVST_LOW();
	AD7606_CONVST_LOW();	/* 连续执行2次，低电平约50ns */
	
	AD7606_CONVST_HIGH();
}




HAL_StatusTypeDef spiRead_AD7608_data( )
{	
	#ifndef AD7606_HARD_SPI		
			for (uint8 i = 0; i < CH_NUM; i++)
		{
		uint32_t read = 0;


	for (uint8_t i = 0; i < 18; i++)
	{
		SCK_0();
		spiDelay();
		read = read << 1;
		if (AD7606_MISO_PinState() == GPIO_PIN_SET)
		{
			read++;
		}
		SCK_1();
		spiDelay();
	}
	//read= read && 0x03ffffff;
	
	AD7608_ADValue[i]= read;
	return HAL_OK;
}
#endif
return	HAL_SPI_Receive(&AD7606_SPI,(uint8*)AD7608_ADValue,CH_NUM,100);
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_scan
*	功能说明: 扫描调用本函数，用于读取AD转换器数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Scan(void) 		/* 此函数代码按照时序编写 */
{
	 		volatile uint8  is=0;

	/* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */	
	if (AD7606_BUSY_PinState == GPIO_PIN_RESET)	   
    {
		AD7606_CS_0(); /* SPI片选 = 0 */	
		AD7606_StartConv();	/* 给开始信号 */	

			
		if(spiRead_AD7608_data()!=HAL_OK) /* 读数据 */	
			{
				 is=1;
			}		
		AD7606_CS_1(); /* SPI片选 = 1 */


	}
}	  

/*
*********************************************************************************************************
*	函 数 名: GetAdcFormFifo
*	功能说明: 从FIFO中读取一个ADC值
*	形    参：_usReadAdc : 存放ADC结果的变量指针
*	返 回 值: 1 表示OK，0表示暂无数据
*********************************************************************************************************
*/
int32_t AD7606_ReadAdc(uint8_t _ch)
{
	int32_t sAdc;
	
	//DISABLE_INT();	
	sAdc = AD7608_ADValue[_ch];
	//ENABLE_INT();

	return sAdc;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/







void test_AD7606main()
{

	  int32_t s_dat[8];
	  float s_volt[8];
	
		spi_InitAD7606(1);	/* 0是5v基准   1是10v基准*/
	
		while (1)
	{
		//Idle();		/* 空闲时执行的函数,比如喂狗. 在bsp.c中 */

		
			/* 每隔500ms 进来一次. 由软件启动转换 */
			AD7606_Scan();
		
			/* 处理数据 */
		
			
				for (uint8 i = 0;i < CH_NUM; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		131072 = 5V , 这是理论值，实际可以根据5V基准的实际值进行公式矫正 
		volt[i] = ((int16_t)dat[i] * 5000) / 131072;	计算实际电压值（近似估算的），如需准确，请进行校准            
		volt[i] = dat[i] * 0.3051850947599719
	*/
		
	float	adc = s_dat[i];
		
		
		if (g_tAD7606.Range == 0)
		{
			s_volt[i] = ((float)adc * 5000) / 131072;
		}
		else
		{
			s_volt[i] = ((float)adc * 10000) / 131072;
		}
	}
			 
		
			
			printf("V:%f\n",	s_volt[0] );				
				
		
		HAL_Delay(500);

	}
	
}
