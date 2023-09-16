#ifdef  ADS1256
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
 		SPI��Ҫ�������� Ȼ���������ӳ�䵽.h������ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  ע�����õ�SPI�ٶ�



		�ⲿ�ֽ����������������ţ�RESET_N����SYNC_N/PDWN_N
RESET_N����оƬ��λ�����������߼���
SYNC_N/PDWN_N����ADת����ͬ�����ƺ�Power-Downģʽ�Ŀ��ƣ����Ʒ�����
--ͬ��ת�������������ߣ��͵�ƽ������20��DRDY_N����
--����Power-Downģʽ�����ֵ͵�ƽ����20��DRDY_N���ڣ����е�·����ʧ��

*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "ADS1256.h"

#define Hardware_SPI

//��ʼ��CS Ĭ����
	 GPIO_TypeDef *SPIX_NSS_Prot=SPIX_Default_NSS_Prot;
		 uint16 SPIX_NSS_Pin=SPIX_Default_NSS_Pin;

	
		 
//����
static void CS_0(void)
{
    HAL_GPIO_WritePin(SPIX_NSS_Prot, SPIX_NSS_Pin,GPIO_PIN_RESET);
	HAL_Delay(2);
}
static void CS_1(void)
{
    HAL_GPIO_WritePin(SPIX_NSS_Prot, SPIX_NSS_Pin,GPIO_PIN_SET);
}
static void ADS1256_DRDY()
{
	    //��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	   while(HAL_GPIO_ReadPin(ADS1256_DRDY_Prot,ADS1256_DRDY_Pin));
		
}


/*��ʼ��ADS1256 GPIO  SPIʹ��HAL��ʼ��  Ȼ����.h����ӳ������*/
void Init_ADS1256_Init(void)
{

    //���û�ж���Ӳ��SPI  ������ǳ�ʼ�����ģʽ
#ifndef Hardware_SPI



    GPIO_InitStruct.Pin = AD_DIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD_DIN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD_DOUT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD_SCLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD_SCLK_GPIO_Port, &GPIO_InitStruct);

#endif

    CS_1();
 //TODO �����RST��������Ҫ����
   ADS1256_Config(ADS1256_GAIN_1);   //��ʼ��ADS1256



}
//���û�ж���Ӳ��SPI  ������ǳ�ʼ�����ģʽ
#ifndef Hardware_SPI

static void ADS1256_DelaySCLK(void)
{
    uint16_t i;

    /*
    	ȡ 5 ʱ��ʵ��ߵ�ƽ200ns, �͵�ƽ250ns <-- ���ȶ�
    	ȡ 10 ���ϣ��������������� �͵�ƽ400ns �߶�400ns <--- �ȶ�
    */
    for (i = 0; i < 15; i++);
}
static void ADS1256_DIN_0(void)
{
    HAL_GPIO_WritePin(AD_DIN_GPIO_Port,AD_DIN_Pin,GPIO_PIN_RESET);
}
static void ADS1256_DIN_1(void)
{
    HAL_GPIO_WritePin(AD_DIN_GPIO_Port,AD_DIN_Pin,GPIO_PIN_SET);
}
static void SCK_0(void)
{
    HAL_GPIO_WritePin(AD_SCLK_GPIO_Port,AD_SCLK_Pin,GPIO_PIN_RESET);
}
static void SCK_1(void)
{
    HAL_GPIO_WritePin(AD_SCLK_GPIO_Port,AD_SCLK_Pin,GPIO_PIN_SET);
}
static uint8 ADS1256_DOUT(void)
{
    return	HAL_GPIO_ReadPin(AD_DOUT_GPIO_Port,AD_DOUT_Pin);
}
#endif

void SPI_WriteByte(unsigned char TxData)
{

#ifndef Hardware_SPI
    unsigned char i;
    /* �������Ͷ���ֽ�ʱ����Ҫ�ӳ�һ�� */
    ADS1256_DelaySCLK();
    ADS1256_DelaySCLK();
    /*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
    for(i = 0; i < 8; i++)
    {
        if (TxData & 0x80)
            ADS1256_DIN_1();
        else
            ADS1256_DIN_0();

        SCK_1();
        ADS1256_DelaySCLK();
        TxData <<= 1;
        SCK_0();			/* <----  ADS1256 ����SCK�½��ز���DIN����, ���ݱ���ά�� 50nS */
        ADS1256_DelaySCLK();
    }
#else
    while(HAL_SPI_GetState(&SPIX)==RESET);
    //  HAL_SPI_TransmitReceive(&SPIX,&TxData,&RxData,1, 1000);
    //������Ƿ��ͺͽ�����һ��ĺ��� ��������Ƿֿ���
    HAL_SPI_Transmit(&SPIX,&TxData,1,100);
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: SPI_ReadByte
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char SPI_ReadByte(void)
{
#ifndef Hardware_SPI
    unsigned char i;
    unsigned char read = 0;
    ADS1256_DelaySCLK();
    /*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
    for (i = 0; i < 8; i++)
    {
        SCK_1();
        ADS1256_DelaySCLK();
        read = read<<1;
        SCK_0();
        if (ADS1256_DOUT())
        {
            read++;
        }
        ADS1256_DelaySCLK();
    }
    return read;
#else

    unsigned char RxData=0x00;
    while(HAL_SPI_GetState(&SPIX)==RESET);
    HAL_SPI_Receive(&SPIX,&RxData,1,100);
    return RxData;
#endif

}




//-----------------------------------------------------------------//
//	��    �ܣ�ADS1256 д����
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע: ��ADS1256�е�ַΪregaddr�ļĴ���д��һ���ֽ�databyte
//-----------------------------------------------------------------//
void ADS1256WREG(unsigned char regaddr,unsigned char databyte)
{ 
 
		 ADS1256_DRDY();
	
    //��ADS1256_DRDYΪ��ʱ����д�Ĵ���


    //��Ĵ���д�����ݵ�ַ
    SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
    //д�����ݵĸ���n-1
    SPI_WriteByte(0x00);
    //��regaddr��ַָ��ļĴ���д������databyte
    SPI_WriteByte(databyte);
    //����SPIЭ���CS����
  			HAL_Delay(1);

}







/*------------------����Jyen--------------------------����Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: ��ʼ��ADS1256
  * @�������:   �Ŵ���
  * @�� �� ֵ: ��
  * @�ص�˵��: ��
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *------------------����Jyen--------------------------����Jyen-------------------------*/

void ADS1256_Config(ADS1256_Magnification_TypeDef magnification)
{
  
		CS_0();
    //*************��У׼****************

    //**********************************                                           
	
     ADS1256WREG(ADS1256_STATUS,0x06);           // 06 ��λ��ǰ���Զ�У׼��ʹ�û���
 
  //  ADS1256WREG(ADS1256_STATUS,0x04);               // ��λ��ǰ����ʹ�û���
//	ADS1256WREG(ADS1256_MUX,0x08);                  // ��ʼ���˿�A0Ϊ��+����AINCOMλ��-��
    ADS1256WREG(ADS1256_ADCON,magnification);         //����ads1256������       // �Ŵ���1
 
    //����ads��������
		
    ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_100SPS);  // ����10sps
		 
    //����IO״̬
    ADS1256WREG(ADS1256_IO,0x00);	
	   //����оƬ����У׼
    SPI_WriteByte(ADS1256_CMD_SELFCAL);
		HAL_Delay(1);
    CS_1();
   
}






//��ȡADֵ
void ADS1256ReadData(unsigned char channel,int32 *adcVule)
{
		    unsigned int sum=0;
			
	    CS_0();
    //�����´�ת����ͨ��
#if  Single_endedDifferenceModel
    //����ͨ��,���ģʽ
  ADS1256WREG(ADS1256_MUX,(channel<<1)|((channel<<1)+1));

		
#else
    //����ͨ��,����ģʽ
   ADS1256WREG(ADS1256_MUX,ADS1256_MUXN_AINCOM|(channel<<4));
	//  ADS1256WREG(ADS1256_MUX, ADS1256_MUXP_AIN1| ADS1256_MUXN_AINCOM);
#endif	 
    //����ͬ������
	
    SPI_WriteByte(ADS1256_CMD_SYNC);
    //���ͻ�������
    SPI_WriteByte(ADS1256_CMD_WAKEUP);
    //���Ͷ���������
    SPI_WriteByte(ADS1256_CMD_RDATA);
				 ADS1256_DRDY();
	 HAL_Delay(1);
 
    sum |= (SPI_ReadByte() << 16);
    sum |= (SPI_ReadByte() << 8);
    sum |= SPI_ReadByte();
    CS_1();
    if (sum>0x7FFFFF)           // if MSB=1,
    {
        sum -= 0x1000000;       // do 2's complement
    }
    *adcVule= sum;
}






void ADS1256Test()
{

		 Init_ADS1256_Init();
	int32 ad[4];
	float  adV[4];
	while(true)
	{
	for(uint8 i=0;i<4;i++)
	{
			ADS1256ReadData(1,&ad[i]);
		adV[i]=ad[i] * 5.0 / 8388607; //�����ѹ
	}
}
}

#endif




