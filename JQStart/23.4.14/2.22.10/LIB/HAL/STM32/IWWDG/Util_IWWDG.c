/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

1. 要将 stm32f1xx_hal_wwdg.c 添加进来

	已经把初始化和喂狗封装了   正常情况下直接如以下方法复制调用即可

	 MX_WWDG_Init(0x7F, 0X5F,WWDG_PRESCALER_8);//初始化狗

//默认开启了中断被动喂狗 只要在初始化时候加上上面初始化函数就行了
	 Wwdog();//主动喂狗
	 注意放喂狗的地方别跑while外面 或超时了
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "WWdog.h"
WWDG_HandleTypeDef hwwdg;
uint8_t wwdg_tr, wwdg_wr;
/**
  * 函数功能: WWDG初始化配置
  * 输入参数: tr ：递减计时器的值， 取值范围为：0x7f~0x40
  *           wr ：窗口值，取值范围为：0x7f~0x40
  *           prv：预分频器值，取值可以是
  *              参数 WWDG_PRESCALER_1: WWDG counter clock = (PCLK1/4096)/1
  *              参数 WWDG_PRESCALER_2: WWDG counter clock = (PCLK1/4096)/2
  *              参数 WWDG_PRESCALER_4: WWDG counter clock = (PCLK1/4096)/4
  *              参数 WWDG_PRESCALER_8: WWDG counter clock = (PCLK1/4096)/8
  * 返 回 值: 无
  * 说    明：无
  */
void MX_WWDG_Init(uint8_t tr, uint8_t wr, uint32_t WWDG_PRESCALER_X)
{
    // 最大值  窗口值  分频

#ifndef HAL_WWDG_MODULE_ENABLED
    未初始化看门狗
#endif
    hwwdg.Instance = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_X;
    hwwdg.Init.Window = wr;
    hwwdg.Init.Counter = tr;
    hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
    if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
    {
        //  Error_Handler();
    }
    /* 窗口值我们在初始化的时候设置成0X5F，这个值不会改变 */
    wwdg_wr = WWDG->CFR & 0X7F;//赋值 wwdg_wr
}
/**
  * 函数功能: WWDG配置
  * 输入参数: hwwdg：窗口看门狗句柄指针
  * 返 回 值: 无
  * 说    明：Hal库内部调用
  */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{

    if(hwwdg->Instance==WWDG)
    {
        /* 窗口看门狗时钟使能 */
        __HAL_RCC_WWDG_CLK_ENABLE();
        /* 外设中断优先级设置及使能 */
        HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(WWDG_IRQn);
    }
}

void HAL_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg)
{
    if(hwwdg->Instance==WWDG)
    {
        /* 窗口看门狗时钟禁用 */
        __HAL_RCC_WWDG_CLK_DISABLE();

        /* 外设中断禁用 */
        HAL_NVIC_DisableIRQ(WWDG_IRQn);
    }

}

/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介:这是主动喂狗 放在主函数中 但是有延时时就会产生喂狗超时
  * @输入参数:无
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void Wwdog (void)
{
    wwdg_tr = WWDG->CR & 0X7F;
    if( wwdg_tr < wwdg_wr )
    {
        /* 喂狗，重新设置计数器的值为最大0X7F */
        HAL_WWDG_Refresh(&hwwdg);
    }
}
//窗口看门狗中断服务函数
void WWDG_IRQHandler(void)
{
//Wwdog();
    //HAL_WWDG_Refresh(&hwwdg);//好像喂狗只能放在这了  下面都不行
    HAL_WWDG_IRQHandler(&hwwdg);//调用WWDG共用中断处理函数  这样就会进回调函数 在那里面喂狗


}

//中断服务函数处理过程
//此函数会被HAL_WWDG_IRQHandler()调用
/***该方法好像已废弃了 不知道什么时候执行**/
void HAL_WWDG_WakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    //Wwdog();
    // HAL_WWDG_Refresh(hwwdg);//更新窗口看门狗值

}


//喂狗本来该放这的 但现在好像只进中断喂狗后才回调  不然就复位
//上面说不行可能是受到打印的影响 说明虽然提前中断 但时间很短
/*看门狗中断回调函数 被动喂狗 在开启提前中断唤醒中喂狗*/
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{


    HAL_WWDG_Refresh(hwwdg);
    //HAL_WWDG_Refresh(hwwdg);//更新窗口看门狗值
}
