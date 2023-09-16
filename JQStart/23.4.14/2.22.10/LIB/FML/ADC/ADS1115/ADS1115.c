
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
          所需依赖 我的模拟IIC1
					坑1：地址
					坑2:5V输入时不需要做IIC的电平转换 否则会读不到值

   直接调用 ADS1115_ConfigAndReadAVG(uint8_t channel,uint8_t VAGcount)  该函数即可
	       返回值要乘上 0.000125才是电压值
	 
	 注意 该版本 最大电压只能是0-4.095
	 

					 实际电压=AD数值/32768.0)*4.096;//0.000125


					 负电压处理
用于测量负电压，负电压从8000~ffff,负电压与正关于0有类似对称关系，按位取反后+1相同
if(data_get>=0x8000)
dianya=((float)(0xffff-data_get)/32768.0)*4.096;//0.000125
else
dianya=((float)data_get/32768.0)*4.096;

return dianya;
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "ads1115.h"


/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 电压读取
  * @输入参数:0 代表 只返回正电压  1代表原样返回	
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/

static uint32_t ADS1115_ReadData(uint8 Original)
{
    /*指向ADS1115指针寄存器用于准备读取数据*/
    uint8_t	Readbuff[2];
    uint32_t conv_data = 0;
    Readbuff[0] = 0x00;
    Readbuff[1] = 0x00;

    //HAL_Delay(100);
    /*读取数据**/
    IIC1_Start();
    IIC1_SendByte(ADS115_ADDRESS_R);  //slave read
    IIC1_WaitAck();

    Readbuff[0]= IIC1_ReadByte();
    IIC1_Ack();
    Readbuff[1]= IIC1_ReadByte();
    IIC1_NAck();


    IIC1_Stop();
    conv_data=(Readbuff[0]<<8)|Readbuff[1];
	if(Original==0)
	{
    if(conv_data > 0x8000)//负电压时候
    {
        conv_data = 0xffff-conv_data;
    }
	}
    return conv_data;
}

/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 配置ADS1115初始化  
  * @输入参数:1.通道  2.模式（0是单通道模式，1是差分模式） 3.精度（0是6.144 1是4.096 2是2.048 3是1.024 4是0.512  5是0.256 后面没意义了）                
  * @返 回 值: 无
  * @重点说明：这里默认为连续转换模式
  * @作者：Jyen Z
  *
  ******************************************************************************
000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/

static uint8_t ADS1115_Config(ADS1115Channel_TypeDefE channel ,uint8 Mode,uint8 Precision)
{
    uint8_t Chnel,WriteIntBuf[4],i;
    switch(channel)
    {

    case ADS1115Channel_1 :
			if(Mode==0)
			{
        Chnel=(0xc0);
			}else if(Mode==1)
			{
				// AINP = AIN0 and AINN = AIN1 
				 Chnel=(0x80);
			}		
        break;
    case ADS1115Channel_2:
      
					if(Mode==0)
			{
          Chnel=(0xd0);
			}else if(Mode==1)
			{
				 Chnel=(0x90);
			}
			
        break;
    case ADS1115Channel_3 :
			
					if(Mode==0)
			{
            Chnel=(0xe0);
			}else if(Mode==1)
			{
				 Chnel=(0xA0);
			}
      
        break;
    case ADS1115Channel_4:
					if(Mode==0)
			{
             Chnel=(0xf0);
			}else if(Mode==1)
			{
				 Chnel=(0xB0);
			}
      
        break;
    default :
        break;
    }
		//精度 其中默认连续转换
			if(Precision==0)
			{
				Chnel|=0x01;
			}else if(Precision==1)
			{
					Chnel|=0x03;
			}
			else if(Precision==2)
			{
				Chnel|=0x05;
			}
				else if(Precision==3)
			{
				Chnel|=0x07;
			}	else if(Precision==4)
			{
				Chnel|=0x09;
			}	else if(Precision==5)
			{
				Chnel|=0x0B;
			}
			
		
		
		
		
    WriteIntBuf[0] = ADS115_ADDRESS_W;      
/* 精度
000 : FS = ±6.144V (1) 100 : FS = ±0.512V
001 : FS = ±4.096V (1) 101 : FS = ±0.256V
010 : FS = ±2.048V (default) 110 : FS = ±0.256V
011 : FS = ±1.024V 
		*/		
    WriteIntBuf[1] = 0x01;     //0x01 //指向配置寄存器 Table 6. Register Addres
    WriteIntBuf[2] = Chnel;  // 写入要配置的通道
    WriteIntBuf[3] = 0x83;        // 83 e3 82    42
    //   3、发送0x42；//配置高位，选定为AIN0，基准为4.096,连续转换模式（配置方法由个人需求决定）
    //  4、发送0xe3；//配置低位，转换选860SPS，禁用比较（配置方法由个人需求决定）
    IIC1_Start();
    for(i=0; i<4; i++)
    {
        IIC1_SendByte(WriteIntBuf[i]);    /////发送寄存器配置信息
        if(IIC1_WaitAck())return 1;            ////注意，此处在最初调试时没有，读出数据一直不变，加上后正常
    }
    IIC1_Stop();
    HAL_Delay(50);//Ads1115延时
    return 0;
}

/*准备转换*/
static void ADS1115_Regrets (void)
{
    IIC1_Start();
    IIC1_SendByte(ADS115_ADDRESS_W);  //slave
    IIC1_WaitAck();
    IIC1_SendByte(0x00);//转换数据寄存器
    IIC1_WaitAck();
    IIC1_Stop();
}




/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 初始化通道并读取平均值  感觉求平均值意义不大  因为每次初始化通道后才会刷新值又太耗时  正常情况下读一次就行了
  * @输入参数:  通道0-3  平均次数 最小值1
  * @返 回 值: 原始ad值  要乘以  0.000125 才是电压
  * @重点说明：当求不连续电压时要考虑每次读取时间  必要时分开初始化  转换  读取 否则可能卡不住时间点
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
uint32_t ADS1115_ConfigAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount)
{
    uint8_t i=0;
    uint32_t AVGconv_data = 0;

    for(i=0; i<VAGcount; i++)
    {
			 
			  ADS1115_Config(channel,0,1);//初始化通道
        ADS1115_Config(channel,0,1);//初始化通道两次 防止旧数据存在
        ADS1115_Regrets();//转换准备读取
			 HAL_Delay(50);//Ads1115延时
			AVGconv_data= AVGconv_data+ ADS1115_ReadData(0);
    }
    AVGconv_data=AVGconv_data/VAGcount;
		float test=AVGconv_data*0.000125;	
    return AVGconv_data;
		
		
}


/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: Ads1115 差分模式读值
  * @输入参数:无                 
  * @返 回 值: 无
  * @重点说明：精度等级要和当前适配 
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
uint32_t  ADS1115_gainAmplifierConfigurAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount)
{
     uint8_t i=0;
    uint32_t AVGconv_data = 0;

    for(i=0; i<VAGcount; i++)
    {
			
			  ADS1115_Config(channel,1,5);//初始化通道
        ADS1115_Config(channel,1,5);//初始化通道两次 防止旧数据存在
        ADS1115_Regrets();//转换准备读取
			 HAL_Delay(50);//Ads1115延时
			AVGconv_data= AVGconv_data+ ADS1115_ReadData(1);
    }
    AVGconv_data=AVGconv_data/VAGcount;
		float test=AVGconv_data/32768.0f*0.256f;
    return AVGconv_data;
}




