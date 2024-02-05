
#include "nsa2300.h"
#include "stdio.h"
//是否启用硬件IIC
//#define HARDWARE_MYIIC0

uint8 readUUID()
{
    uint8 addr =0xa6;
    uint8 ntc_readData[1];
#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,addr,I2C_MEMADD_SIZE_8BIT,ntc_readData,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,addr,I2C_MEMADD_SIZE_8BIT,ntc_readData,1,100)==HAL_ERROR)
#endif
    {   if (CoreDebug->DHCSR & 1)
        {
            __breakpoint(0);
        }
        return  HAL_ERROR;
    }
    return HAL_OK;
}


uint8 nsa2300_Init()
{
    //寄存器对应的地址描述
    uint8_t MemAddr[6]= {0xa5,0xa6,0xa7,0xa8,0xab,0xb3};

		
    uint8_t writeData1[1]= {0x00};
    uint8_t writeData2[1]= {0x37};
    uint8_t writeData3[1]= {0x87};
    uint8_t writeData4[1]= {0x08};
    uint8_t writeData5[1]= {0x04};
    uint8_t writeData6[1]= {0x08};


#ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[0],I2C_MEMADD_SIZE_8BIT,writeData1,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[0],I2C_MEMADD_SIZE_8BIT,writeData1,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  
		#ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[1],I2C_MEMADD_SIZE_8BIT,writeData2,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[1],I2C_MEMADD_SIZE_8BIT,writeData2,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  
		#ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[2],I2C_MEMADD_SIZE_8BIT,writeData3,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[2],I2C_MEMADD_SIZE_8BIT,writeData3,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  #ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[3],I2C_MEMADD_SIZE_8BIT,writeData4,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[3],I2C_MEMADD_SIZE_8BIT,writeData4,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  #ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[4],I2C_MEMADD_SIZE_8BIT,writeData5,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[4],I2C_MEMADD_SIZE_8BIT,writeData5,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  #ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[5],I2C_MEMADD_SIZE_8BIT,writeData6,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[5],I2C_MEMADD_SIZE_8BIT,writeData6,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
  

    readUUID();
    return HAL_OK;

}


/**********************************************************/
//    传进来一组数据和数据个数
/*********************************************************/
void medina_filter( float temperature_array[],int length)
{
	uint8_t i, j;
	float temp;
  for(i = 0; i < length; ++i){

     for(j = i + 1; j < length; ++j){

         if(temperature_array[j] < temperature_array[i]){    //如果后一个元素小于前一个元素则交换

           temp = temperature_array[i];

           temperature_array[i] = temperature_array[j];

           temperature_array[j] = temp;

         }
				 
			 
				 

      }
  }

}


static uint8 startNSA2300Sample()
{   uint8_t MemAddr[2] = {0x30,0x02};
    uint8_t writeData[1] = {0x0a};
    uint8  ntc_readData[1];
    uint8_t read_flag=0;
		uint8_t read_Timeout=10;
#ifdef 		HARDWARE_MYIIC0
    if(	HAL_I2C_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[0],I2C_MEMADD_SIZE_8BIT,writeData,1,100)==HAL_ERROR)
#else
    if(	MYIIC0_Mem_Write(&hi2c1,NSA2300_DevAddress,MemAddr[0],I2C_MEMADD_SIZE_8BIT,writeData,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
    
    while((!read_flag)&--read_Timeout)
    {HAL_Delay(10);
#ifdef 		HARDWARE_MYIIC0
        if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,MemAddr[1],I2C_MEMADD_SIZE_8BIT,ntc_readData,1,100)==HAL_ERROR)
#else
        if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,MemAddr[1],I2C_MEMADD_SIZE_8BIT,ntc_readData,1,100)==HAL_ERROR)
#endif
        {
            return HAL_ERROR;
        }
        read_flag=ntc_readData[0];
    }
		if(!read_Timeout)
		{
			 return HAL_ERROR;
		}
    return HAL_OK;
}
//读NTC温度电压然后计算成电阻返回
uint8 readNTCResistor(float* ntcResistor)
{
    uint8_t	ntc_readData1[1]= {0};
		 uint8_t	ntc_readData2[1]= {0};
    uint16 ntc_sum=0;

#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,0x09,I2C_MEMADD_SIZE_8BIT,ntc_readData1,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,0x09,I2C_MEMADD_SIZE_8BIT,ntc_readData1,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }

#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,0x0a,I2C_MEMADD_SIZE_8BIT,ntc_readData2,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,0x0a,I2C_MEMADD_SIZE_8BIT,ntc_readData2,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
    ntc_sum = (ntc_readData1[0] <<8) + ntc_readData2[0];
    *ntcResistor = (float) (100000.0*((ntc_sum-6400.0)/(32768.0-(ntc_sum-6400.0))));
    return HAL_OK;

}
//读热电堆
uint8 readThermopileVoltage(float* thermopile_voltage )
{

    uint8_t	thermopile_readData[1]= {0};
    uint8_t	thermopile_readData2[1]= {0};
    uint8_t	thermopile_readData3[1]= {0};
    int thermopile_sum=0;

    //分别读高中低位
#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,0x06,I2C_MEMADD_SIZE_8BIT,thermopile_readData,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,0x06,I2C_MEMADD_SIZE_8BIT,thermopile_readData,1,100)==HAL_ERROR)
#endif
    {
        volatile	uint16 ss=55;
        return HAL_ERROR;
    }

#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,0x07,I2C_MEMADD_SIZE_8BIT,thermopile_readData2,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,0x07,I2C_MEMADD_SIZE_8BIT,thermopile_readData2,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }
#ifdef 		HARDWARE_MYIIC0
    if(HAL_I2C_Mem_Read(&hi2c1,NSA2300_DevAddress,0x08,I2C_MEMADD_SIZE_8BIT,thermopile_readData3,1,100)==HAL_ERROR)
#else
    if(MYIIC0_Mem_Read(&hi2c1,NSA2300_DevAddress,0x08,I2C_MEMADD_SIZE_8BIT,thermopile_readData3,1,100)==HAL_ERROR)
#endif
    {
        return HAL_ERROR;
    }

    thermopile_sum = (thermopile_readData[0]<<16)+(thermopile_readData2[0]<<8)+thermopile_readData3[0];
    if (CoreDebug->DHCSR & 1)
    {
        if (!thermopile_sum)
        {
            //  __breakpoint(0);
        }
    }

    if(thermopile_sum > 0x800000 )
    {
      //  thermopile_sum = 0xffffff- thermopile_sum;
				return 0;
    }
    *thermopile_voltage = (1.75*(thermopile_sum/8388608.0))/64.0;     //基准怎么默认是1.8V    此处改变成1.75V
    return HAL_OK;

}


uint8 NSA2300SigalProcess(float* temperature,float* ntcResistor)
{
    uint8_t samplingTimes=10;
    float ntc_array[10]= {0};
    float temperature_array[10]= {0};
 
    //采样多次
    for(uint8 i = 0; i<samplingTimes; i++)
    { 
			float ntcResistor_;
      float thermopileVoltage;
        startNSA2300Sample();

        if( readNTCResistor(&ntcResistor_))
        {
            return HAL_ERROR;
        }
        ntc_array[i] = ntcResistor_;



        if( readThermopileVoltage(&thermopileVoltage))
        {
            return HAL_ERROR;
        }
        temperature_array[i] = (thermopileVoltage)*1000.0;




    }
    //调用冒泡排序采样结果
    medina_filter(ntc_array,samplingTimes);
    medina_filter(temperature_array,samplingTimes);

    *temperature = (temperature_array[5]+temperature_array[6])/2;
    *ntcResistor         = (ntc_array[5]+ntc_array[6])/2;
		
				//	printf("卡前：(ntc:%f  The:%f ) \n",*ntcResistor,*temperature);
//		*temperature=  kalman_filter(*temperature);
  //  *ntcResistor =  kalman_filter2(*ntcResistor);
    return HAL_OK;
}



