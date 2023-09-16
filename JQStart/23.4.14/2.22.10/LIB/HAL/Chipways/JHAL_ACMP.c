#include "../JHAL_ACMP.h"
#include "xl_acmp.h"

/*模拟比较器（ ACMP）*/



void  _JHAL_acmpConfig(ACMP_Type *ACMPx)
{


    ACMP_InitTypeDef ACMP_Initstructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    if(ACMPx==ACMP0) {

        SIM_SCGC_Cmd(SIM_SCGC_ACMP0,ENABLE);
    } else {
        SIM_SCGC_Cmd(SIM_SCGC_ACMP1,ENABLE);

    }


    ACMP_Initstructure.ACMP_HystMode = ACMP_Hyst_20mv;
    ACMP_Initstructure.ACMP_IntTrigMode = ACMP_Int_FallorRiseEdge;
    ACMP_Initstructure.ACMP_InputP = ACMP_Positive_EXT0;
    ACMP_Initstructure.ACMP_InputN = ACMP_Negative_DAC;
    ACMP_Initstructure.ACMP_InputPin = ACMP_IN0;

    ACMP_Initstructure.ACMP_DACEN = ENABLE;
    ACMP_Initstructure.ACMP_DACRefVoltage = ACMP_DACRef_VDDA;
    ACMP_Initstructure.ACMP_DACValue =  0x1F;		//输出电平选择 = Vin/64*(1+0x1f)

    ACMP_Init(ACMPx, &ACMP_Initstructure );
    ACMP_InterruptEn(ACMPx, ENABLE );
    ACMP_OutputPinEn(ACMPx, ENABLE );
    ACMP_EnableCmd(ACMPx, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    if(ACMPx==ACMP0) {
        NVIC_InitStructure.NVIC_IRQChannel = ACMP0_IRQn;
    } else {
        NVIC_InitStructure.NVIC_IRQChannel = ACMP1_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 单通道单次采集模式初始化
 * @输入参数:
isEN  true初始化  false反初始化
channel 对应通道的数组
number  要初始化通道的个数
 * @返 回 值: 无
 * @备注:
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/



void  JHAL_acmpInit(bool  isEN)
{
    static bool sEN= {false};
    if(sEN!=isEN)
    {
        sEN=isEN;

        if(isEN)
        {

            _JHAL_acmpConfig(ACMP0);
        } else {

            ACMP_DeInit(ACMP0);
        }
    }

}


/**
 * @brief  ACMP中断函数
 * @retval None
 */
void ACMP0_IRQHandler(void)
{
    ACMP_ClearIntFlag(ACMP0);
}
/**
 * @brief  ACMP中断函数
 * @retval None
 */
void ACMP1_IRQHandler(void)
{
    ACMP_ClearIntFlag(ACMP1);
}

