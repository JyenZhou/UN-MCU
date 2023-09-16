#include "../JHAL_Delay.h"






/**
 * @brief  Core系统时钟的初始化
 * @param  None
 * @return None
 */
void  JHAL_delayInit(JHAL_DealyConfig config)
{

}




/**
 * @brief  微秒级延时
 * @param  nus  延时的时间
 * @return None
 */
void  JHAL_delayUs(u32 nus)
{

}



/**
 * @brief  毫秒级延时
 * @param  nms  延时的时间
 * @return None
 */
void  JHAL_delayMs(u32 nms)
{

    for(uint32_t i=0; i<nms; i++) JHAL_delayUs(1000);
}



