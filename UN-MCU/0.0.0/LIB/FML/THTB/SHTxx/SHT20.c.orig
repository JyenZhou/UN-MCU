/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                     所需依赖 我的模拟SHT20_IIC                                                *

						该函数是非阻塞模式下的温湿度读取（实际上这里也成了阻塞式 发送转换命令后就采取延时后读取了） 其他方法已删除

       定义了   SHT20_PARAM  g_SHT20_param    外部结构变量

         我们在外面	 可以直接利用这个本来是存放在测试函数的变量
				         g_SHT20_param.TEMP_HM = SHT20_MeasureTempHM();
                 g_SHT20_param.HUMI_HM = SHT20_MeasureHumiHM();
								 或者再新建变量接收此函数返回值
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/


#include "SHT20.h"
#include "SHT20_iic.h"

 

////////////////////////////////////////////*********************************************************////////////////
void SHT20_Init()
{
    SHT20_IIC_Init();
}

/***读取温度 */
uint16 SHT20_MeasureTemp(void)//读取温度
{
    uint8_t Restart,Restarttimeout=15;

    u8 tmp1, tmp2;
    uint16 ST;
    do
    {
        Restart=0;
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_W);//address+write
        if(SHT20_IIC_WaitAck())Restart=1;
        SHT20_IIC_SendByte(TRIG_TEMP_MEASUREMENT_POLL);//e3触发温度测量e5湿度
        if(SHT20_IIC_WaitAck())Restart=1;
        HAL_Delay(90);
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_R);//address+read
        if(SHT20_IIC_WaitAck())Restart=1;

        tmp1 = SHT20_IIC_ReadByte();
        SHT20_IIC_Ack();
        tmp2 = SHT20_IIC_ReadByte();
        //  SHT20_IIC_Ack();
        //  crc = SHT20_IIC_ReadByte();
        SHT20_IIC_NAck();
        SHT20_IIC_Stop();

        ST = (tmp1 << 8) | (tmp2 << 0);
        ST &= ~0x0003;
    }
    while(Restart&Restarttimeout--);

    return ST;// TEMP = (float)ST*175.72f/0xffff-46.85f;//当性能较好时直接单片机算((float)ST * 0.00268127) - 46.85;
}

/***读取湿度 */
uint16 SHT20_MeasureHumi(void)
{
    uint8_t Restart,Restarttimeout=15;

    u8 tmp1, tmp2;
    uint16 SRH;
    do
    {
        Restart=0;
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_W);//80 写
        if(SHT20_IIC_WaitAck())Restart=1;
        SHT20_IIC_SendByte(TRIG_HUMI_MEASUREMENT_POLL);//e3触发温度测量e5湿度
        if(SHT20_IIC_WaitAck())Restart=1;
        HAL_Delay(90);
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_R);
        if(SHT20_IIC_WaitAck())Restart=1;
    }
    while(Restart&Restarttimeout--);
    tmp1 = SHT20_IIC_ReadByte();
    SHT20_IIC_Ack();
    tmp2 = SHT20_IIC_ReadByte();
//   c=SHT20_IIC_ReadByte();
    SHT20_IIC_NAck();
    SHT20_IIC_Stop();

    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    //	g_SHT20_param.HUMI_HM_32=(uint16)SRH;




    return SRH;// HUMI = SRH*125.0f/0xffff-6.0f;	//当性能较好时直接单片机算((float)SRH * 0.00190735) - 6;
}






float Temp=0,HUMI =0;
void SHT20_Test(void)
{

    SHT20_Init();
    Temp=   SHT20_MeasureTemp() *175.72f/0xffff-46.85f;//当性能较好时直接单片机算((float)ST * 0.00268127) - 46.85;;
    HUMI =  SHT20_MeasureHumi()*125.0f/0xffff-6.0f;	//当性能较好时直接单片机算((float)SRH * 0.00190735) - 6;



}




