#ifdef  ADS1256
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
 		SPI需要另外配置 然后把引脚重映射到.h中来　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  注意配置的SPI速度



		这部分仅引出两个功能引脚，RESET_N，和SYNC_N/PDWN_N
RESET_N用于芯片复位，拉低再拉高即可
SYNC_N/PDWN_N用于AD转换的同步控制和Power-Down模式的控制，控制方法：
--同步转换：拉低再拉高，低电平不超过20个DRDY_N周期
--进入Power-Down模式：保持低电平大于20个DRDY_N周期，所有电路均会失能

*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "ADS1256.h"

#define Hardware_SPI

//初始化CS 默认项
	 GPIO_TypeDef *SPIX_NSS_Prot=SPIX_Default_NSS_Prot;
		 uint16 SPIX_NSS_Pin=SPIX_Default_NSS_Pin;

	
		 
//操作
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
	    //当ADS1256_DRDY为低时才能写寄存器
	   while(HAL_GPIO_ReadPin(ADS1256_DRDY_Prot,ADS1256_DRDY_Pin));
		
}


/*初始化ADS1256 GPIO  SPI使用HAL初始化  然后在.h中重映射引脚*/
void Init_ADS1256_Init(void)
{

    //如果没有定义硬件SPI  下面的是初始化软件模式
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
 //TODO 如果有RST引脚这里要上拉
   ADS1256_Config(ADS1256_GAIN_1);   //初始化ADS1256



}
//如果没有定义硬件SPI  下面的是初始化软件模式
#ifndef Hardware_SPI

static void ADS1256_DelaySCLK(void)
{
    uint16_t i;

    /*
    	取 5 时，实测高电平200ns, 低电平250ns <-- 不稳定
    	取 10 以上，可以正常工作， 低电平400ns 高定400ns <--- 稳定
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
    /* 连续发送多个字节时，需要延迟一下 */
    ADS1256_DelaySCLK();
    ADS1256_DelaySCLK();
    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
    for(i = 0; i < 8; i++)
    {
        if (TxData & 0x80)
            ADS1256_DIN_1();
        else
            ADS1256_DIN_0();

        SCK_1();
        ADS1256_DelaySCLK();
        TxData <<= 1;
        SCK_0();			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
        ADS1256_DelaySCLK();
    }
#else
    while(HAL_SPI_GetState(&SPIX)==RESET);
    //  HAL_SPI_TransmitReceive(&SPIX,&TxData,&RxData,1, 1000);
    //上面的是发送和接受在一块的函数 下面这个是分开的
    HAL_SPI_Transmit(&SPIX,&TxData,1,100);
#endif

}

/*
*********************************************************************************************************
*	函 数 名: SPI_ReadByte
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned char SPI_ReadByte(void)
{
#ifndef Hardware_SPI
    unsigned char i;
    unsigned char read = 0;
    ADS1256_DelaySCLK();
    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
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
//	功    能：ADS1256 写数据
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: 向ADS1256中地址为regaddr的寄存器写入一个字节databyte
//-----------------------------------------------------------------//
void ADS1256WREG(unsigned char regaddr,unsigned char databyte)
{ 
 
		 ADS1256_DRDY();
	
    //当ADS1256_DRDY为低时才能写寄存器


    //向寄存器写入数据地址
    SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
    //写入数据的个数n-1
    SPI_WriteByte(0x00);
    //向regaddr地址指向的寄存器写入数据databyte
    SPI_WriteByte(databyte);
    //拉高SPI协议的CS引脚
  			HAL_Delay(1);

}







/*------------------作者Jyen--------------------------作者Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 初始化ADS1256
  * @输入参数:   放大倍数
  * @返 回 值: 无
  * @重点说明: 无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *------------------作者Jyen--------------------------作者Jyen-------------------------*/

void ADS1256_Config(ADS1256_Magnification_TypeDef magnification)
{
  
		CS_0();
    //*************自校准****************

    //**********************************                                           
	
     ADS1256WREG(ADS1256_STATUS,0x06);           // 06 高位在前、自动校准、使用缓冲
 
  //  ADS1256WREG(ADS1256_STATUS,0x04);               // 高位在前、不使用缓冲
//	ADS1256WREG(ADS1256_MUX,0x08);                  // 初始化端口A0为‘+’，AINCOM位‘-’
    ADS1256WREG(ADS1256_ADCON,magnification);         //设置ads1256的增益       // 放大倍数1
 
    //设置ads采样速率
		
    ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_100SPS);  // 数据10sps
		 
    //设置IO状态
    ADS1256WREG(ADS1256_IO,0x00);	
	   //开启芯片的自校准
    SPI_WriteByte(ADS1256_CMD_SELFCAL);
		HAL_Delay(1);
    CS_1();
   
}






//读取AD值
void ADS1256ReadData(unsigned char channel,int32 *adcVule)
{
		    unsigned int sum=0;
			
	    CS_0();
    //设置下次转换的通道
#if  Single_endedDifferenceModel
    //设置通道,差分模式
  ADS1256WREG(ADS1256_MUX,(channel<<1)|((channel<<1)+1));

		
#else
    //设置通道,单端模式
   ADS1256WREG(ADS1256_MUX,ADS1256_MUXN_AINCOM|(channel<<4));
	//  ADS1256WREG(ADS1256_MUX, ADS1256_MUXP_AIN1| ADS1256_MUXN_AINCOM);
#endif	 
    //发送同步命令
	
    SPI_WriteByte(ADS1256_CMD_SYNC);
    //发送唤醒命令
    SPI_WriteByte(ADS1256_CMD_WAKEUP);
    //发送读数据命令
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
		adV[i]=ad[i] * 5.0 / 8388607; //计算电压
	}
}
}

#endif




