
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
 		外设控制
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "controller.h"




void  vhAllClose( )
{
    JHAL_gpioWitePin(  EN_VCC0_GPIO_JPort,EN_VCC0_JPin,false);
    JHAL_gpioWitePin(  EN_VCC1_GPIO_JPort,EN_VCC1_JPin,false);
    JHAL_gpioWitePin(  EN_VCC2_GPIO_JPort,EN_VCC2_JPin,false);
    JHAL_gpioWitePin(  EN_VCC3_GPIO_JPort,EN_VCC3_JPin,false);
    JHAL_gpioWitePin(  EN_VCC4_GPIO_JPort,EN_VCC4_JPin,false);
    JHAL_gpioWitePin(  EN_VCC5_GPIO_JPort,EN_VCC5_JPin,false);
    JHAL_gpioWitePin(  EN_VCC6_GPIO_JPort,EN_VCC6_JPin,false);
    JHAL_gpioWitePin(  EN_VCC7_GPIO_JPort,EN_VCC7_JPin,false);
    JHAL_gpioWitePin(  EN_VCC8_GPIO_JPort,EN_VCC8_JPin,false);
    JHAL_gpioWitePin(  EN_VCC9_GPIO_JPort,EN_VCC9_JPin,false);

    JHAL_gpioWitePin(  EN_GND0_GPIO_JPort,EN_GND0_JPin,false);
    JHAL_gpioWitePin(  EN_GND1_GPIO_JPort,EN_GND1_JPin,false);
    JHAL_gpioWitePin(  EN_GND2_GPIO_JPort,EN_GND2_JPin,false);
    JHAL_gpioWitePin(  EN_GND3_GPIO_JPort,EN_GND3_JPin,false);
    JHAL_gpioWitePin(  EN_GND4_GPIO_JPort,EN_GND4_JPin,false);
    JHAL_gpioWitePin(  EN_GND5_GPIO_JPort,EN_GND5_JPin,false);
    JHAL_gpioWitePin(  EN_GND6_GPIO_JPort,EN_GND6_JPin,false);
    JHAL_gpioWitePin(  EN_GND7_GPIO_JPort,EN_GND7_JPin,false);
    JHAL_gpioWitePin(  EN_GND8_GPIO_JPort,EN_GND8_JPin,false);
    JHAL_gpioWitePin(  EN_GND9_GPIO_JPort,EN_GND9_JPin,false);
}



/** ----------------------------vhRadioOpen-----------------------------------
  * @描述： 测试模式时候用的只会打开指定行，其他列关闭
  *
  * @参数：row 0-9需要加热的行
**	  	 row: [输入/出]
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-08-------------------------------------- */

void  vhRadioOpen(u8 column)
{

    vhAllClose( );


    switch (column)
    {
    case 0:
        JHAL_gpioWitePin(  EN_GND0_GPIO_JPort,EN_GND0_JPin,true);
        JHAL_gpioWitePin(  EN_VCC0_GPIO_JPort,EN_VCC0_JPin,true);


        break;
    case 1:
        JHAL_gpioWitePin(  EN_GND1_GPIO_JPort,EN_GND1_JPin,true);
        JHAL_gpioWitePin(  EN_VCC1_GPIO_JPort,EN_VCC1_JPin,true);

        break;
    case 2:
        JHAL_gpioWitePin(  EN_GND2_GPIO_JPort,EN_GND2_JPin,true);
        JHAL_gpioWitePin(  EN_VCC2_GPIO_JPort,EN_VCC2_JPin,true);
        break;
    case 3:
        JHAL_gpioWitePin(  EN_GND3_GPIO_JPort,EN_GND3_JPin,true);
        JHAL_gpioWitePin(  EN_VCC3_GPIO_JPort,EN_VCC3_JPin,true);
        break;
    case 4:
        JHAL_gpioWitePin(  EN_GND4_GPIO_JPort,EN_GND4_JPin,true);
        JHAL_gpioWitePin(  EN_VCC4_GPIO_JPort,EN_VCC4_JPin,true);
        break;
    case 5:
        JHAL_gpioWitePin(  EN_GND5_GPIO_JPort,EN_GND5_JPin,true);
        JHAL_gpioWitePin(  EN_VCC5_GPIO_JPort,EN_VCC5_JPin,true);
        break;
    case 6:
        JHAL_gpioWitePin(  EN_GND6_GPIO_JPort,EN_GND6_JPin,true);
        JHAL_gpioWitePin(  EN_VCC6_GPIO_JPort,EN_VCC6_JPin,true);
        break;
    case 7:
        JHAL_gpioWitePin(  EN_GND7_GPIO_JPort,EN_GND7_JPin,true);
        JHAL_gpioWitePin(  EN_VCC7_GPIO_JPort,EN_VCC7_JPin,true);
        break;
    case 8:
        JHAL_gpioWitePin(  EN_GND8_GPIO_JPort,EN_GND8_JPin,true);
        JHAL_gpioWitePin(  EN_VCC8_GPIO_JPort,EN_VCC8_JPin,true);
        break;
    case 9:
        JHAL_gpioWitePin(  EN_GND9_GPIO_JPort,EN_GND9_JPin,true);
        JHAL_gpioWitePin(  EN_VCC9_GPIO_JPort,EN_VCC9_JPin,true);
        break;

    default:
        break;
    }

}







/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 初始化 所有处于关闭状态
  * @输入参数:   无
  * @返 回 值: 无
  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

void controllerInit(void)
{
    vhAllClose( );
}



