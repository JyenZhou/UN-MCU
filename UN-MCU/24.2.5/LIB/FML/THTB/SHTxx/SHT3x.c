#include "sht3x.h"



/* 软件复位命令 */

#define    SOFT_RESET_CMD   0x30A2
/*
单次测量模式
命名格式：Repeatability_CS_CMD
CS：Clock stretching
*/
#define  HIGH_ENABLED_CMD     0x2C06
#define   MEDIUM_ENABLED_CMD    0x2C0D
#define   LOW_ENABLED_CMD      0x2C10
#define   HIGH_DISABLED_CMD     0x2400
#define    MEDIUM_DISABLED_CMD  0x240B
#define   LOW_DISABLED_CMD      0x2416

/*
周期测量模式
命名格式：Repeatability_MPS_CMD
MPS：measurement per second
*/
#define   HIGH_0_5_CMD     0x2032
#define  MEDIUM_0_5_CMD   0x2024
#define  LOW_0_5_CMD      0x202F
#define   HIGH_1_CMD       0x2130
#define   MEDIUM_1_CMD     0x2126
#define   LOW_1_CMD        0x212D
#define  HIGH_2_CMD       0x2236
#define   MEDIUM_2_CMD     0x2220
#define   LOW_2_CMD        0x222B
#define   HIGH_4_CMD       0x2334
#define  MEDIUM_4_CMD     0x2322
#define   LOW_4_CMD         0x2329
#define  HIGH_10_CMD      0x2737
#define   MEDIUM_10_CMD     0x2721
#define  LOW_10_CMD       0x272A
/* 周期测量模式读取数据命令 */
//	注意：在此模式下不能选择时钟拉伸  数据采集频率和可重复性设置会影响传感器的测量持续时间和电流消耗。
#define    READOUT_FOR_PERIODIC_MODE   0xE000

 


bool  sht3xOpen( SHT3x  *config)
{
    JHAL_i2csfOpen(	&config->i2c); 
    // 0x2130  表示定期数据采集模式 这个比较通用   对时间要求高的需要重新封装
//  周期1ms
 	if(!config-> isSingleConversion)
	{
    u8 cmdData[2]= {HIGH_1_CMD>>8,(u8)HIGH_1_CMD};
    u8 timeoutCount=10;

    while(!JHAL_i2csfTransmit(&config->i2c,  cmdData,2))
    {
        if(timeoutCount--==0) {
            return false;
        }
    }
	}
    return true;

}


 

int  sht3x_crc8_compute(u8 *check_data, u8 num_of_data)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

// calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) {
        crc ^= (check_data[byteCtr]);
        //crc校验，最高位是1就^0x31
        for(bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            }  else {
                crc = (crc << 1);
            }
        }
    }
    return crc;

}
bool sht30_crc8_check(u8 *p,u8 num_of_data,u8 CrcData)
{
    uint8_t crc;
    crc = sht3x_crc8_compute(p, num_of_data);// calculates 8-Bit checksum
    if(crc != CrcData)
    {
        return false;
    }
    return true;
}


bool sht3xGet(SHT3x  *config,  float *  temperature,float * humidity )
{
//这里和前面的一样，也是写一个命令给SHT30，0xe000这个命令是访问SHT30转换结果的寄存器的

//下面是开始读取数据，其中的数组p存放结果，前三个存放温度值，后三个是湿度值，在前三个温度值里面，
//p[0]是温度的高八位，p[1]是低八位，p[2]是CRC校验，有关CRC校验的知识我是百度上面看的，
//要是各位不懂得话，可以不用crc校验，直接用p[0]、p[1]就可以转换出来温度的值。

    uint8_t	readbuff[6];
  
    /*读取数据**/
	 if( config->  isSingleConversion){
		   if(  !JHAL_i2csfMemRead(&config->i2c,HIGH_DISABLED_CMD,true,readbuff,6))
    {
        return false ;
    }
		 
	 }else{

    if(  !JHAL_i2csfMemRead(&config->i2c,READOUT_FOR_PERIODIC_MODE,true,readbuff,6))
    {
        return false ;
    }

	}

    u16 data;
   

    //先处理温度的相关数据，位于数组的前三个

    //crc校验
 
	
    
    //crc校验要是不成功就返回1，同时不会更新温度值
    if(sht30_crc8_check(readbuff,2,readbuff[2]) )
    {
        //把2个8位数据拼接为一个16位的数据
        data=( readbuff[0] << 8) | readbuff[1];
        //温度转换
        *temperature =   175.0f *  data  / 65535  - 45 ;
    }
    else
    {
        return false;
    }

    //湿度处理
    
    if(sht30_crc8_check(&readbuff[3],2, readbuff[5]) )
    {
        //参考上面温度的代码
        data= (readbuff[3] << 8) | readbuff[4];
        *humidity =   100.0f *  data / 65535.0f   ;

    }
    else
    {
        return false;
    }

    return true;
}





