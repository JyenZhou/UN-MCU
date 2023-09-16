/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX 配置
ADC
1.number of Conversion 选择要连续转换的通道数与这儿.h一直(为了兼容这里的程序 只有一个的话 可以加个温度或基准通道 剩下的不用管)
2.continuos Conversion mode ->ENABLE 使能连续转换模式
3.配置Rank

DMA
1.DMA-->Mode-->Circular
2.DMA-->Data Width-->word


            16通道ADC1+DMA处理
						 ADC开始后 一定要延时再读取  不然第一次读的是空值  之后读的都是上一次值
						 
						 	 //转换为温度值,实际应用中,可考虑用毫伏为单位,避免浮点运算
						   float  TEMP=	(1.43 - (ad*3.3/4096))/0.0043 + 25 = 55.23
					
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#include "Util_ADC.h"
// AD转换结果值
uint32_t  ADC_ConvertedValue[ADC_NUMOFCHANNEL];
uint32_t DMA_Transfer_Complete_Count=0;


/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 初始化DMA——ADC的转换
  * @输入参数:无
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void utilADC_Init (void)
{
 
  //  HAL_ADCEx_Calibration_Start(&hadc1);
  //  HAL_ADCEx_Calibration_Start(&hadc1);
    /* 启动AD转换并使能DMA传输和中断 */
    HAL_ADC_Start_DMA(&hadc1,ADC_ConvertedValue,ADC_NUMOFCHANNEL);
    HAL_Delay(200);//这个延时是为了第一次读取adc就有值了  是给他转换时间的等待
}

/**
  * 函数功能: ADC转换完成回调函数
  * 输入参数: hadc：ADC外设设备句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    DMA_Transfer_Complete_Count++;
	
 
}





