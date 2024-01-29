
/**
****************************XL_5050RGBC_WS2812B.c******************************************
* @brief    XL_5050RGBC_WS2812B Color控制驱动
* 该版本是在Chipway 中测试的   通过示波器匹配的（ SystemSetFEE(8000000, ICS_FLLFACTOR_1920, ICS_RDIV_1or32,ICS_RDIV2_5);）
  ******************************************************************************
* @file     XL_5050RGBC_WS2812B.c
* @author   Jyen
* @version  v1.0
* @date     2023-04-11
* @attention
* This software is supplied under the terms of a license
* agreement or non-disclosure agreement.
* Passing on and copying of this document,and communication
* of its contents is not permitted without prior written
* authorization.
*
*         (C) Copyright 2023,Jyen,China. All Rights Reserved
********************************************************************************
*/

#include "xl5050rgbc_ws2812b.h"






void XL_5050RGBC_WS2812B_open(XL_5050RGBC_WS2812B_Config *config)
{


    JHAL_gpioModeSet(config->port,config->pin,JHAL_IOMODE_IO_OutputPushPull);
    JHAL_gpioWitePin(config->port,config->pin,!config->activeLevle);


}


Color __255BrightnessCalculate(Color color, float brightness) {



    Color result= {0};

    if (brightness > 1.0f) {
        brightness = 1.0f;
    }




    // 如果计算后小于最小值1，就设为1，并重新计算g=1时的r和b
    if(color.r!=0) {
        result.r = (uint8_t) (color.r * brightness);
        if (result.r < 1) {
            result.g = (uint8_t) (1.0f * color.g / color.r);
            result.b = (uint8_t) (1.0f * color.b / color.r);
            result.r = 1;
        }
    }
    if(color.g!=0) {
        result.g = (uint8_t) (color.g * brightness);
        if (result.g < 1) {
            result.r = (uint8_t) (1.0f * color.r / color.g);
            result.b = (uint8_t) (1.0f * color.b / color.g);
            result.g = 1;
        }
    }
    if(color.b!=0) {
        result.b = (uint8_t) (color.b * brightness);
        if (result.b < 1) {
            result.r = (uint8_t) (1.0f * color.r / color.b);
            result.g = (uint8_t) (1.0f * color.g / color.b);
            result.b = 1;
        }
    }

    // 如果计算后大于最大值255，就设为255，并重新计算g=255时的r和b
    if (result.r > 255) {
        result.g = (uint8_t) (255.0f * color.g / color.r);
        result.b = (uint8_t) (255.0f * color.b / color.r);
        result.r = 255;
    }
    if (result.g > 255) {
        result.r = (uint8_t) (255.0f * color.r / color.g);
        result.b = (uint8_t) (255.0f * color.b / color.g);
        result.g = 255;
    }
    if (result.b > 255) {
        result.r = (uint8_t) (255.0f * color.r / color.b);
        result.g = (uint8_t) (255.0f * color.g / color.b);
        result.b = 255;
    }

    return result;
}



/**
 * @brief 设置LED灯珠的颜色
 *
 * @param index LED灯珠的索引
 * @param color LED灯珠的颜色
  *float brightness  亮度0-1
 */
bool XL_5050RGBC_WS2812B_setLedColor(XL_5050RGBC_WS2812B_Config *config,uint8_t index, Color color, float brightness) {

    if(index>= config->ledsNumber )
    {
        return false;
    }

    color=__255BrightnessCalculate(color,brightness);


    if(color.r!= config->ledsArray [index].r|| color.g!= config->ledsArray [index].g||color.b!= config->ledsArray [index].b)
    {   config->ledsArray [index]=color;
        JHAL_enableInterrupts();
        return true;
    }

    return false;

}

/**
 * @brief 更新所有LED灯珠的颜色
 *
 */

#define delayT0HT1LTime()      //__nop(); __nop();   __nop();   __nop() ;   __nop();   __nop(); __nop();   __nop();   __nop() ; __nop()
void delayT0LT1HTime()
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
}





//#pragma arm section code = "JRAMCODE"
#pragma O3
void XL_5050RGBC_WS2812B_update(XL_5050RGBC_WS2812B_Config *config) {

    uint8_t i,g,r,b;

    JHAL_gpioWitePin(config->port,config->pin,!config->activeLevle);
    JHAL_delayUs(1);
    // 关中断
    JHAL_disableInterrupts();

    // 依次控制每个LED灯珠的颜色

    for (i = 0; i < config->ledsNumber; i++) {
        g=config->ledsArray [i].g;
        r=config->ledsArray [i].r;
        b=config->ledsArray [i].b;



        /**
        for (j = 0; j < 8; j++) {
        bitVal = (__XL_5050RGBC_WS2812B_config->ledsArray [i].g & (0x80 >> j)) ? 1 : 0;
        JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,__XL_5050RGBC_WS2812B_config->activeLevle);
        JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1LTime : T0LTime);
        JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,!__XL_5050RGBC_WS2812B_config->activeLevle);
        JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1LTime : T0LTime);
        }
         for (j = 0; j < 8; j++) {
        bitVal = (__XL_5050RGBC_WS2812B_config->ledsArray [i].r & (0x80 >> j)) ? 1 : 0;
        JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,__XL_5050RGBC_WS2812B_config->activeLevle);
        JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1HTime : T0HTime);
        JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,!__XL_5050RGBC_WS2812B_config->activeLevle);
        JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1LTime : T0LTime);
        }
        for (j = 0; j < 8; j++) {
                  bitVal = (__XL_5050RGBC_WS2812B_config->ledsArray [i].b & (0x80 >> j)) ? 1 : 0;
                  JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,__XL_5050RGBC_WS2812B_config->activeLevle);;
                  JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1HTime : T0HTime);
                  JHAL_gpioWitePin(__XL_5050RGBC_WS2812B_config->port,__XL_5050RGBC_WS2812B_config->pin,!__XL_5050RGBC_WS2812B_config->activeLevle);
                  JHAL_delay(JHAL_TimeUnits_NS,bitVal ? T1LTime : T0LTime);
        }
                  */

//因为单片机速度不够用循环时间超出 下面就拆分了

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x80 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x80 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }




/////////////////////////////////////////////////////////////////////////////////////////////////////////////


        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x40 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x40 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x20 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x20 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x10 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x10 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x08 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x08 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x04 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x04 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x02 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x02 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(g & 0x01 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(g & 0x01 ))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        //***************************************************************************************************************************************************************************
        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x80)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x80))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x40)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x40))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x20)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x20))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x10)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x10))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x08)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x08))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x04)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x04))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x02)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x02))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |= (0x400000) ;
        if(r & 0x01)
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(r & 0x01))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }




//***************************************************************************************************************************************************************************



        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x80 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x80))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x40 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x40))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x20 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x20))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x10 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x10))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x08 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x08))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x04 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x04))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x02 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x02))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        *(u32*)(0x40100004) |=(0x400000) ;
        if(b & 0x01 )
        {
            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }

        *(u32*)(0x40100008) |= (0x400000) ;
        if(!(b & 0x01))
        {

            delayT0LT1HTime();
        } else {

            delayT0HT1LTime();
        }




    }



    // 开中断
    JHAL_enableInterrupts();
}

//#pragma arm section
