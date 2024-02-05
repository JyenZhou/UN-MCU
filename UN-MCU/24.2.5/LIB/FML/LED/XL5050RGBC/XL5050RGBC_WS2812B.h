#ifndef __XL_5050RGBC_WS2812B__H__
#define __XL_5050RGBC_WS2812B__H__
#include "JFML.h"
#ifdef __CplusPlus
extern "C" {
#endif

    typedef struct {
        uint32_t r; // 红色分量
        uint32_t g; // 绿色分量
        uint32_t b; // 蓝色分量
    } Color;

#define Color255_Black ((Color){0, 0, 0 }) // 黑色
#define Color255_White ((Color){255, 255, 255}) // 白色
#define Color255_Red ((Color){255, 0, 0 }) // 红色
#define Color255_Green ((Color){0, 255, 0 }) // 绿色
#define Color255_Blue ((Color){0, 0, 255}) // 蓝色
#define Color255_Yellow ((Color){255, 255, 0 }) // 黄色
#define Color255_Magenta ((Color){255, 0, 255}) // 洋红色
#define Color255_Cyan ((Color){0, 255, 255}) // 青色
#define Color255_Pink ((Color){255, 192, 203}) // 粉红色
#define Color255_Purple ((Color){128, 0, 128}) // 紫色
#define Color255_Orange ((Color){255, 165, 0 }) // 橙色
#define Color255_Golden ((Color){255, 215, 0 }) // 金色
#define Color255_LightGray ((Color){192, 192, 192}) // 浅灰色
#define Color255_DarkGray ((Color){128, 128, 128}) // 深灰色
#define Color255_NavyBlue ((Color){0, 0, 128}) // 海军蓝
#define Color255_SkyBlue ((Color){135, 206, 235}) // 天蓝色
#define Color255_Lavender ((Color){230, 230, 250}) // 熏衣草紫
#define Color255_OliveGreen ((Color){128, 128, 0 }) // 橄榄绿

#define Color255_IndianRed        ((Color){205, 92,  92 }) // 印度红色
#define Color255_SaddleBrown      ((Color){139, 69,  19 }) // 马鞍棕色
#define Color255_RosyBrown        ((Color){188, 143, 143}) // 玫瑰棕色
#define Color255_SandyBrown       ((Color){244, 164, 96 }) // 沙棕色
#define Color255_Goldenrod        ((Color){218, 165, 32 }) // 金麒麟色
#define Color255_DarkGoldenrod    ((Color){184, 134, 11 }) // 深金黄色
#define Color255_Peru             ((Color){205, 133, 63 }) // 秘鲁色
#define Color255_Chocolate        ((Color){210, 105, 30 }) // 巧克力色
#define Color255_Tan              ((Color){210, 180, 140}) // 晒黑色
#define Color255_RosyViolet       ((Color){131, 111, 255}) // 玫瑰紫色
#define Color255_Plum             ((Color){221, 160, 221}) // 洋李色
#define Color255_Violet           ((Color){238, 130, 238}) // 紫罗兰色
#define Color255_MediumPurple     ((Color){147, 112, 219}) // 中紫色
#define Color255_Orchid           ((Color){218, 112, 214}) // 暗紫色
#define Color255_DarkOrchid       ((Color){153, 50,  204}) // 暗兰花紫色
#define Color255_Thistle          ((Color){216, 191, 216}) // 蓟色


    typedef struct  {
        JHAL_IO_Port port ;
        u8 pin;
        bool activeLevle;
        //需要定义一个数组用来存放Color值的 多少颗灯珠数组大小就多少
        Color *ledsArray;
        //灯珠的个数，即数组的大小
        u8 ledsNumber;
    } XL_5050RGBC_WS2812B_Config;





    void XL_5050RGBC_WS2812B_open(XL_5050RGBC_WS2812B_Config *config);

    /**
    * @brief 设置LED灯珠的颜色
    *
    * @param index LED灯珠的索引
    * @param color LED灯珠的颜色
    *float brightness  亮度0-1
    */
    bool XL_5050RGBC_WS2812B_setLedColor(XL_5050RGBC_WS2812B_Config *config,uint8_t index, Color color, float brightness) ;
    //设置后需要更新才生效方便同时设置多个
    void XL_5050RGBC_WS2812B_update(XL_5050RGBC_WS2812B_Config *config) ;





#ifdef CplusPlus
}
#endif

#endif


