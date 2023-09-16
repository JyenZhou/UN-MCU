
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
       18λ�߾���AD
 

          
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/








/*
SCK:SCLK
MISO��DOUT
MOSI��CS

ע����� MOSI�ǲ��ӵ�

*/

#include "ad7608.h"

//#define AD7606_HARD_SPI		 

#ifndef AD7606_HARD_SPI		/* ����Ӳ��SPI */
	#define SCK_0()		HAL_GPIO_WritePin(AD7606_PORT_SCK, AD7606_PIN_SCK,GPIO_PIN_RESET)
	#define SCK_1()		HAL_GPIO_WritePin(AD7606_PORT_SCK, AD7606_PIN_SCK,GPIO_PIN_SET)

	#define MOSI_0()	HAL_GPIO_WritePin(AD7606_PORT_MOSI, AD7606_PIN_MOSI,GPIO_PIN_RESET)
	#define MOSI_1()	HAL_GPIO_WritePin(AD7606_PORT_MOSI, AD7606_PIN_MOSI,GPIO_PIN_SET)

	#define AD7606_MISO_PinState()	HAL_GPIO_ReadPin(AD7606_PORT_MISO, AD7606_PIN_MISO)
	
	
	/*
*********************************************************************************************************
*	�� �� ��: SpiDelay
*	����˵��: ʱ���ӳ�
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: InitAD7606
*	����˵��: ��ʼ��AD7606 SPI����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void spi_InitAD7606(uint8 g_tAD7606_Range )
{
	
	
	g_tAD7606.Range=g_tAD7606_Range;
	#ifndef AD7606_HARD_SPI
	AD7606_CfgSpiSOFTGPIO();
#endif
 
	AD7606_ConfigGPIO();		/* ����GPIO */
	


	
	AD7606_SetInputRange(g_tAD7606.Range);	/* ���ò������� */

	/* ���ù�����ģʽ */
	AD7606_SetOS(0);
	AD7606_Reset();				/* Ӳ����λ��AD7606 */
	
	AD7606_CONVST_HIGH();			/* CONVST������Ϊ�ߵ�ƽ */	
}
/*
*********************************************************************************************************
*	�� �� ��: AD7606_ConfigGPIO
*	����˵��: ����GPIO�� ������ SCK  MOSI  MISO �����SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 static void AD7606_ConfigGPIO( )
{


	
	/* ��GPIOʱ�� */
  AD7606_RCC_CS 		   ;
  AD7606_RCC_RESET 	;
  AD7606_RCC_RANGE 	;
  AD7606_RCC_CONVST 	  ;
  AD7606_RCC_BUSY ;
    AD7606_RCC_OS2;
  AD7606_RCC_OS1;
  AD7606_RCC_OS0;
	
	/* ���ü����������IO */
 
 		
		 
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
	


	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pin = AD7606_PIN_BUSY;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(AD7606_PORT_BUSY, &GPIO_InitStruct);
		

}

#ifndef AD7606_HARD_SPI
/*
*********************************************************************************************************
*	�� �� ��: AD7606_CfgSpiSOFTGPIO
*	����˵��: �������ģʽ��GPIO�� SCK  MOSI  MISO �����SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD7606_CfgSpiSOFTGPIO(void)
{
		AD7606_RCC_SCK;
AD7606_RCC_MISO;
AD7606_RCC_MOSI;
		/* ����SPI����SCK��MISO �� MOSIΪ����ģʽ */
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 
	  GPIO_InitStruct.Pin = AD7606_PIN_MOSI;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD7606_PORT_MOSI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD7606_PIN_SCK;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD7606_PORT_SCK, &GPIO_InitStruct);
	
/* MISO ����Ϊ�������� */
    GPIO_InitStruct.Pin = AD7606_PIN_MISO;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(AD7606_PORT_MISO, &GPIO_InitStruct);

	
	
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: ad7606_SetOS
*	����˵��: ���ù�����ģʽ�������˲���Ӳ����ƽ��ֵ)
*	��    �Σ�_ucMode : 0-6  0��ʾ�޹�������1��ʾ2����2��ʾ4����3��ʾ8����4��ʾ16��
*				5��ʾ32����6��ʾ64��
*	�� �� ֵ: ��
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
	else	/* ��0���� */
	{
		AD7606_OS2_0();
		AD7606_OS1_0();
		AD7606_OS0_0();
	}
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: AD7606_SetInputRange
*	����˵��: ����AD7606ģ���ź��������̡�
*	��    ��: _ucRange : 0 ��ʾ����5V   1��ʾ����10V
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		AD7606_RANGE_5V();	/* ����Ϊ����5V */
	}
	else
	{
		AD7606_RANGE_10V();	/* ����Ϊ����10V */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_RESET
*	����˵��: Ӳ����λ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	/* CS  SCLK���� */
#ifdef SOFT_SPI
		AD7606_CS_1();
		//SCK_1();
#endif

 	/* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
	AD7606_RESET_LOW();
	
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	AD7606_RESET_HIGH();
	
	AD7606_RESET_LOW();
}
/*
*********************************************************************************************************
*	�� �� ��: ad7606_StartConv
*	����˵��: ����AD7606��ADCת��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartConv(void)
{
	/* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
	AD7606_CONVST_LOW();
	AD7606_CONVST_LOW();
	AD7606_CONVST_LOW();	/* ����ִ��2�Σ��͵�ƽԼ50ns */
	
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
*	�� �� ��: AD7606_scan
*	����˵��: ɨ����ñ����������ڶ�ȡADת��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Scan(void) 		/* �˺������밴��ʱ���д */
{
	 		volatile uint8  is=0;

	/* BUSY = 0 ʱ.ad7606���ڿ���״̬adת������ */	
	if (AD7606_BUSY_PinState == GPIO_PIN_RESET)	   
    {
		AD7606_CS_0(); /* SPIƬѡ = 0 */	
		AD7606_StartConv();	/* ����ʼ�ź� */	

			
		if(spiRead_AD7608_data()!=HAL_OK) /* ������ */	
			{
				 is=1;
			}		
		AD7606_CS_1(); /* SPIƬѡ = 1 */


	}
}	  

/*
*********************************************************************************************************
*	�� �� ��: GetAdcFormFifo
*	����˵��: ��FIFO�ж�ȡһ��ADCֵ
*	��    �Σ�_usReadAdc : ���ADC����ı���ָ��
*	�� �� ֵ: 1 ��ʾOK��0��ʾ��������
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/







void test_AD7606main()
{

	  int32_t s_dat[8];
	  float s_volt[8];
	
		spi_InitAD7606(1);	/* 0��5v��׼   1��10v��׼*/
	
		while (1)
	{
		//Idle();		/* ����ʱִ�еĺ���,����ι��. ��bsp.c�� */

		
			/* ÿ��500ms ����һ��. ���������ת�� */
			AD7606_Scan();
		
			/* �������� */
		
			
				for (uint8 i = 0;i < CH_NUM; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		131072 = 5V , ��������ֵ��ʵ�ʿ��Ը���5V��׼��ʵ��ֵ���й�ʽ���� 
		volt[i] = ((int16_t)dat[i] * 5000) / 131072;	����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼            
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
