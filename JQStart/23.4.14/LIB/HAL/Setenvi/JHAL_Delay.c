#include "../JHAL_Delay.h"






/**
 * @brief  Coreϵͳʱ�ӵĳ�ʼ��
 * @param  None
 * @return None
 */
void  JHAL_delayInit(JHAL_DealyConfig config)
{

}




/**
 * @brief  ΢�뼶��ʱ
 * @param  nus  ��ʱ��ʱ��
 * @return None
 */
void  JHAL_delayUs(u32 nus)
{

}



/**
 * @brief  ���뼶��ʱ
 * @param  nms  ��ʱ��ʱ��
 * @return None
 */
void  JHAL_delayMs(u32 nms)
{

    for(uint32_t i=0; i<nms; i++) JHAL_delayUs(1000);
}



