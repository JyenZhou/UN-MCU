#include "../JHAL_Delay.h"
#include <xl_nvic.h>
#include <system_XL6600.h>


static u32  __delay_fac_us=0;							//us延时倍乘数


static u32  __delay_fac_100ns=0;							//100ns延时倍乘数

/**
	* @param SysTick 使能定义
*/
#define SysTick_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Msk            (1UL /*<< SysTick_CTRL_ENABLE_Pos*/)           /*!< SysTick CTRL: ENABLE Mask */

/**
	* @param SysTick 中断使能定义
*/
#define SysTick_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define SysTick_CTRL_TICKINT_Msk           (1UL << SysTick_CTRL_TICKINT_Pos)              /*!< SysTick CTRL: TICKINT Mask */




/**
 * @brief  SysTick中断函数
 * @param  None
 * @return None
 */
void SysTick_Handler(void)
{

}


static bool __JHAL_isdelayInited=false;

/**
 * @brief  Core系统时钟的初始化
 * @param  None
 * @return None
 */
void  JHAL_delayOpen(JHAL_Delay  config)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    __delay_fac_us=Get_SystemCoreClock()/1000000;
    __JHAL_isdelayInited=true;
}




void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value)
{
    //延时倍乘数
    u32 delay_fac=Get_SystemCoreClock()/(Sec2Nsec/timeUntis);
    if(value==0)
    {
        return;
    }
    if(timeUntis<JHAL_TimeUnits_US)
    {

        for (u32 i = 0; i < delay_fac; i++) {
            __NOP();
        }
        return;
    }

    uint32_t temp;
    SysTick->LOAD=value*delay_fac; 				//时间加载
    SysTick->VAL=0x00;        				//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ; //开始倒数
    do
    {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达
    SysTick->CTRL&=~ SysTick_CTRL_ENABLE_Msk; //关闭计数器
    SysTick->VAL =0X00;       				//清空计数器

}




/**
 * @brief  微秒级延时
 * @param  nus  延时的时间
 * @return None
 */
void  JHAL_delayUs(u32 nus)
{
    if(nus==0)
    {
        nus++;
        return;
    }

    uint32_t temp;
    SysTick->LOAD=nus*__delay_fac_us; 				//时间加载
    SysTick->VAL=0x00;        				//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ; //开始倒数
    do
    {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达
    SysTick->CTRL&=~ SysTick_CTRL_ENABLE_Msk; //关闭计数器
    SysTick->VAL =0X00;       				//清空计数器
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

void  JHAL_delayTryCheckedExceptions(void)

{
    if(!__JHAL_isdelayInited)
    {
        //未初始化
        while(true);
    }
}





