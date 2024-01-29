/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                    所需依赖 我的模拟IIC1                                                   *
            IS31FL3733 LED灯控制芯片驱动封装函数
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#include "IS31FL3733.h"

uint8 RowLeftDataBuff[9];
uint8 RowReightDataBuff[9];
//用来缓存所有行状态的 数组0不算 分左右12*2  这里面的状态将于实际灯的状态一致

uint32_t ADS1115_ReadData()
{
    /*指向ADS1115指针寄存器用于准备读取数据*/
    uint8_t	Readbuff[2];
    uint32_t conv_data = 0;
    Readbuff[0] = 0x00;
    Readbuff[1] = 0x00;

    //HAL_Delay(100);
    /*读取数据**/
    IIC1_Start();
    IIC1_SendByte(0x91);  //slave read
    IIC1_WaitAck();

    Readbuff[0]= IIC1_ReadByte();
    IIC1_Ack();
    Readbuff[1]= IIC1_ReadByte();
    IIC1_NAck();


    IIC1_Stop();
    conv_data=(Readbuff[0]<<8)|Readbuff[1];
    if(conv_data > 0x8000)//负电压时候
    {
        conv_data = 0xffff-conv_data;
    }
    return conv_data;
}



/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 要选择PG0~PG3，需要先解锁该寄存器，以避免该寄存器误操作。
              当用0xC5写入FEH时，允许FDH修改一次，修改FDH(命令寄存器)后，FEH将立即重置为0x00。
  * @输入参数:0x00 FDh write disable   0xC5 FDh write enable once
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
static/*第一步必用 后面看情况用*/ void UnLoCk (void)
{
    IIC1_Start();
    IIC1_SendByte(DeviceAddress<<1|Write);
    IIC1_WaitAck();
    IIC1_SendByte(CommandRegisterWriteLock);
    IIC1_WaitAck();
    IIC1_SendByte(0xC5);
    IIC1_Stop();

}



/*-----------------------------------------------------------------------*/
/**
*@ 函数功能或简介: LED开关行控制  当第一次对灯控制时要要给予标识
* @输入参数:1.行号 1-12  2. 分左右两面   3数据 FF代表全亮 4.是否是IIC停止后第一次操作
  * @返 回 值:  返回寄存器地址
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/

//用于存储控制开关行寄存器地址 0不算
uint8 LedRows_ON_OFF_Control[13]={0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0X0C,0X0E,0x10,0X12,0X14,0x16};
void Led_ON_OFF_LineControl(uint8 Row,Direction_TypeDefE direction,uint8 data,WhichoneWrite_TypeDefE WhichoneWrite)
{

    //第一次进来时要发送到设备地址-->控制寄存器-->LED控制寄存器 -->之后则可以连续数据操作
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(LEDCOntrolRegister);
        IIC1_WaitAck();
    }

    //借助在枚举中使用的就是实际地址直接操作
    if(direction==Left)
    {
        IIC1_SendByte(LedRows_ON_OFF_Control[Row]);
			RowLeftDataBuff[Row]=data;
    }
    else
    {
        IIC1_SendByte((LedRows_ON_OFF_Control[Row]+0x01));
			RowReightDataBuff[Row]=data;
    }
    IIC1_WaitAck();
    IIC1_SendByte(data);
    IIC1_WaitAck();	
		 	if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
}

/*
data|=(1<<n);					//第n位置1
data&=~(1<<n);				//第n位置0
data^=(1<<n);				//第n位取反
(data>>n)&1;				//取第n位的值
*/


/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: LED 开关的独立操作
  * @输入参数:行号1-12 列1-16 开或关  是否是首个通信控制                   
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void Led_ON_OFF_RadioControl(uint8 Row,u8 Column,ONOrOFF_TypeDefE ONorOFF,WhichoneWrite_TypeDefE WhichoneWrite)
{
    Direction_TypeDefE direction;
    //当第8列之后 行的地址将会变化 //借助在数组中使用的就是实际地址直接操作
    if(Column<8)
    {
        direction=Left;
    }
    else
    {
        direction=Reight;
    }
    if(ONorOFF==ON)//当要开灯时 对指定位置1操作
    {
        //用来缓存所有灯 开关状态
        RowLeftDataBuff[Row]=RowLeftDataBuff[Row]|(1<<Column);
        Led_ON_OFF_LineControl(Row,direction,RowLeftDataBuff[Row],WhichoneWrite);
    }
    else  //用来缓存所有灯 开关状态
    {
        RowReightDataBuff[Row]=RowReightDataBuff[Row]&(~(1<<Column));
        Led_ON_OFF_LineControl(Row,direction,RowReightDataBuff[Row],WhichoneWrite);
    }

}



/*-----------------------------------------------------------------------*/
/**
*@ 函数功能或简介: LED PWM独立控制  当第一次对灯控制时要要给予标识
* @输入参数:1.行 枚举类Row1-12  2.列枚举类Column1-16    3.PWM等级
  * @返 回 值:  返回寄存器地址
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/

//设置13个是0不算
uint8 Led_Row_A_L[13]= {0x00,0x00,0x10,0x20,0x30,0x40,0x05,0x60,0x70,0x80,0x90,0xA0,0xB0};
//设置17个是0不算
uint8 Led_Column_1_16[17]= {0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0X0F};

//用于位置控制的枚举
void Led_PWM_RadioControl(uint8 Row,u8 Column,u8 PWMGrade,WhichoneWrite_TypeDefE WhichoneWrite)
{   //位置地址
    uint8_t position;
    //第一次进来时要发送到设备地址-->控制寄存器-->LED控制寄存器 -->之后则可以连续数据操作
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(PWMRegister);
        IIC1_WaitAck();
    }

    //借助在数组中使用的就是实际地址直接操作
    position=Led_Row_A_L[Row]|Led_Column_1_16[Column];
    IIC1_SendByte(position);
    IIC1_WaitAck();
    IIC1_SendByte(PWMGrade);
    IIC1_WaitAck();
		 if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
}




/*-----------------------------------------------------------------------*/
/**
*@ 函数功能或简介: LED 自动呼吸模式独立控制  当第一次对灯控制时要要给予标识
* @输入参数:1.行 枚举类Row1-12  2.列枚举类Column1-16    3.PWM等级
  * @返 回 值:  返回寄存器地址
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
typedef enum
{
    PWMControlMode  =0x00,//PWM控制模式
    AutoBreathMode_1 =0x01,//自动呼吸模式123
    AutoBreathMode_2 =0x02,
    AutoBreathMode_3 =0x03,

} AutoBreathMode_TypeDefE;
//自动呼吸模式控制类
//用于位置控制的枚举
void Led_AutoBreath_RadioControl(uint8 Row,u8 Column,AutoBreathMode_TypeDefE autoBreathMode,WhichoneWrite_TypeDefE WhichoneWrite)
{
    uint8_t position;//位置地址
    //第一次进来时要发送到设备地址-->控制寄存器-->LED控制寄存器 -->之后则可以连续数据操作
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(AutoBreathModeRegister);
        IIC1_WaitAck();
    }
    //借助在数组中使用的就是实际地址直接操作
    position=Led_Row_A_L[Row]|Led_Column_1_16[Column];
    IIC1_SendByte(position);
    IIC1_WaitAck();
    IIC1_SendByte(autoBreathMode);
		IIC1_WaitAck();
   	if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
    
		
}



//灯控制1测试程序  行控制 暂时只控制两行
void test_ON_LED1()
{
	    //1.解锁控制寄存器
    UnLoCk();
  Led_ON_OFF_LineControl(1,Left,0xFF,FirstWrite);
	Led_ON_OFF_LineControl(1,Reight,0xFF,Last_Write);
	HAL_Delay(1000);
	   //1.解锁控制寄存器
    UnLoCk();
	  Led_ON_OFF_LineControl(1,Left,0x00,FirstWrite);
	Led_ON_OFF_LineControl(1,Reight,0x00,Last_Write);
	HAL_Delay(1000);
	//不解锁 看看能不能控制
	 // UnLoCk();
  Led_ON_OFF_LineControl(1,Left,0xFF,FirstWrite);
	Led_ON_OFF_LineControl(2,Reight,0xFF,Last_Write);
		HAL_Delay(1000);
}
//灯控制2测试程序  简单流水灯
void test_ON_LED2()
{
    for(uint8 Rows; Rows<=12; Rows++)
    {
        for(uint8 Column; Column<=16; Column++)
        {
					 //1.解锁控制寄存器
     UnLoCk();
            Led_ON_OFF_RadioControl(Rows,Column,ON,Once_Write);
        }
    }
		HAL_Delay(1000);
		   for(uint8 Rows; Rows<=12; Rows++)
    {
        for(uint8 Column; Column<=16; Column++)
        {
					//1.解锁控制寄存器
             UnLoCk();
            Led_ON_OFF_RadioControl(Rows,Column,OFF,Once_Write);
        }
    }
	HAL_Delay(1000);
}


/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 测试程序
  * @输入参数:无
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void IS31FL3733_Testmain (void)
{


    //1.调用测试程序控制灯
    test_ON_LED1();
//	test_ON_LED2();
	
}
