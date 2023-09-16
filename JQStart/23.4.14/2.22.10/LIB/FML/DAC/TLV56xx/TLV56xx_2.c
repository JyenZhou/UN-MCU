#include "TLV56xx_2.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
   12λDA


          
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/



#define TLV56xx_2_SCLK_HIGH      HAL_GPIO_WritePin(TLV56xx_2_PORT_SCK,TLV56xx_2_PIN_SCK,GPIO_PIN_SET);
#define TLV56xx_2_DATA_HIGH         HAL_GPIO_WritePin(TLV56xx_2_PORT_DATA,TLV56xx_2_PIN_DATA,GPIO_PIN_SET);
#define TLV56xx_2_FS_HIGH        HAL_GPIO_WritePin(TLV56xx_2_PORT_FS,TLV56xx_2_PIN_FS,GPIO_PIN_SET);
#define TLV56xx_2_LDAC_HIGH       HAL_GPIO_WritePin(TLV56xx_2_PORT_LDAC,TLV56xx_2_PIN_LDAC,GPIO_PIN_SET);
#define TLV56xx_2_FRE_HIGH         HAL_GPIO_WritePin(TLV56xx_2_PORT_FRE,TLV56xx_2_PIN_FRE,GPIO_PIN_SET);

#define TLV56xx_2_SCLK_LOW      HAL_GPIO_WritePin(TLV56xx_2_PORT_SCK,TLV56xx_2_PIN_SCK,GPIO_PIN_RESET);
#define TLV56xx_2_DATA_LOW        HAL_GPIO_WritePin(TLV56xx_2_PORT_DATA ,TLV56xx_2_PIN_DATA ,GPIO_PIN_RESET);
#define TLV56xx_2_FS_LOW             HAL_GPIO_WritePin(TLV56xx_2_PORT_FS,TLV56xx_2_PIN_FS,GPIO_PIN_RESET);
#define TLV56xx_2_LDAC_LOW          HAL_GPIO_WritePin(TLV56xx_2_PORT_LDAC,TLV56xx_2_PIN_LDAC,GPIO_PIN_RESET);
#define TLV56xx_2_FRE_LOW       	  HAL_GPIO_WritePin(TLV56xx_2_PORT_FRE,TLV56xx_2_PIN_FRE,GPIO_PIN_RESET);
/*
*********************************************************************************************************
*	�� �� ��: TLV56xx_2_CfgSpiSOFTGPIO
*	����˵��: �������ģʽ��GPIO�� SCK  MOSI  MISO �����SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TLV56xx_2_CfgSpiSOFTGPIO(void)
{

    TLV56xx_2_RCC_SCK ;
    TLV56xx_2_RCC_DATA	;
    TLV56xx_2_RCC_DOUT ;

    /* ����SPI����SCK��MISO �� MOSIΪ��������ģʽ */
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    GPIO_InitStruct.Pin = TLV56xx_2_PIN_DATA;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TLV56xx_2_PORT_DOUT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TLV56xx_2_PIN_SCK;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TLV56xx_2_PORT_SCK, &GPIO_InitStruct);

    /* MISO ����Ϊ�������� */
    GPIO_InitStruct.Pin = TLV56xx_2_PIN_DOUT;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TLV56xx_2_PORT_DATA, &GPIO_InitStruct);



}



void TLV56xx_2_Init(void)
{
    TLV56xx_2_CfgSpiSOFTGPIO();

    /* ��GPIOʱ�� */

    TLV56xx_2_RCC_LDAC;
    TLV56xx_2_RCC_FS;
    TLV56xx_2_RCC_FRE ;

    /* ���ü����������IO */



    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TLV56xx_2_PIN_LDAC;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TLV56xx_2_PORT_LDAC, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = TLV56xx_2_PIN_FS;
    HAL_GPIO_Init(TLV56xx_2_PORT_FS, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TLV56xx_2_PIN_FRE;
    HAL_GPIO_Init(TLV56xx_2_PORT_FRE, &GPIO_InitStruct);






    TLV56xx_2_LDAC_HIGH;
    TLV56xx_2_FRE_HIGH;
    HAL_Delay(100);
    for(uint8 i = 0; i < 8; i++)
        TLV56xx_2_WriteChannelValue(i, 0);
    HAL_Delay(50);
}

 


////volatile  uint16_t Cmd;
////void WriteChannelValue(uint8_t channel, uint16_t value)
////{
////
////  int i;
////  uint16_t bit_state;
////  //TLV5608     10BIT
////  Cmd = (channel<<12)|((value<<2)&0X0FFC);
////
////  LDAC_HIGH;
////  FS_HIGH;
////  FS_LOW;
////  for(i = 0; i < 16; i++)
////  {
////    SCLK_HIGH;
////    bit_state = Cmd&0X8000;
////    if(bit_state)
////      DATA_HIGH;
////    else
////      DATA_LOW;
////    SCLK_LOW;
////    Cmd <<= 1;
////  }
////  FS_HIGH;
////  LDAC_LOW;
////}

void TLV56xx_2_WriteChannelValue(uint8_t channel, uint16_t value)
{
    uint16_t Cmd;
    int i;
    uint16_t bit_state;
    //TLV5610     12BIT
    Cmd = (channel<<12)|((value)&0X0FFF);

//  LDAC_HIGH;
    TLV56xx_2_FS_HIGH;
    TLV56xx_2_FS_LOW;
    for(i = 0; i < 16; i++)
    {
        TLV56xx_2_SCLK_HIGH;
        bit_state = Cmd&0X8000;
        if(bit_state) {
            TLV56xx_2_DATA_HIGH;
        }
        else {
            TLV56xx_2_DATA_LOW;
        }
        TLV56xx_2_SCLK_LOW;
        Cmd <<= 1;
    }
    TLV56xx_2_FS_HIGH;
    TLV56xx_2_LDAC_LOW;
//  LDAC_LOW;

    TLV56xx_2_LDAC_HIGH;
}


/*���Դ���  ����һֱѭ����ʼ��*/
void unitTest_TLV56xx_2(void)
{
    TLV56xx_2_Init();


    TLV56xx_2_WriteChannelValue(1, 4095/8*2);
    TLV56xx_2_WriteChannelValue(2, 4095/8*3);
    TLV56xx_2_WriteChannelValue(3, 4095/8*4);
    TLV56xx_2_WriteChannelValue(4, 4095/8*5);
    TLV56xx_2_WriteChannelValue(5, 4095/8*6);
    TLV56xx_2_WriteChannelValue(6, 4095/8*7);
    TLV56xx_2_WriteChannelValue(7, 4095/8*8);

}

