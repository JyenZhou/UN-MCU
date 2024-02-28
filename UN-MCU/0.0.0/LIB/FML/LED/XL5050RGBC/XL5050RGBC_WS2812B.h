#ifndef __XL_5050RGBC_WS2812B__H__
#define __XL_5050RGBC_WS2812B__H__
#include "JFML.h"
#ifdef __cplusPlus
extern "C" {
#endif

    typedef struct {
        uint32_t r; // ��ɫ����
        uint32_t g; // ��ɫ����
        uint32_t b; // ��ɫ����
    } Color;

#define Color255_Black ((Color){0, 0, 0 }) // ��ɫ
#define Color255_White ((Color){255, 255, 255}) // ��ɫ
#define Color255_Red ((Color){255, 0, 0 }) // ��ɫ
#define Color255_Green ((Color){0, 255, 0 }) // ��ɫ
#define Color255_Blue ((Color){0, 0, 255}) // ��ɫ
#define Color255_Yellow ((Color){255, 255, 0 }) // ��ɫ
#define Color255_Magenta ((Color){255, 0, 255}) // ���ɫ
#define Color255_Cyan ((Color){0, 255, 255}) // ��ɫ
#define Color255_Pink ((Color){255, 192, 203}) // �ۺ�ɫ
#define Color255_Purple ((Color){128, 0, 128}) // ��ɫ
#define Color255_Orange ((Color){255, 165, 0 }) // ��ɫ
#define Color255_Golden ((Color){255, 215, 0 }) // ��ɫ
#define Color255_LightGray ((Color){192, 192, 192}) // ǳ��ɫ
#define Color255_DarkGray ((Color){128, 128, 128}) // ���ɫ
#define Color255_NavyBlue ((Color){0, 0, 128}) // ������
#define Color255_SkyBlue ((Color){135, 206, 235}) // ����ɫ
#define Color255_Lavender ((Color){230, 230, 250}) // Ѭ�²���
#define Color255_OliveGreen ((Color){128, 128, 0 }) // �����

#define Color255_IndianRed        ((Color){205, 92,  92 }) // ӡ�Ⱥ�ɫ
#define Color255_SaddleBrown      ((Color){139, 69,  19 }) // ����ɫ
#define Color255_RosyBrown        ((Color){188, 143, 143}) // õ����ɫ
#define Color255_SandyBrown       ((Color){244, 164, 96 }) // ɳ��ɫ
#define Color255_Goldenrod        ((Color){218, 165, 32 }) // ������ɫ
#define Color255_DarkGoldenrod    ((Color){184, 134, 11 }) // ����ɫ
#define Color255_Peru             ((Color){205, 133, 63 }) // ��³ɫ
#define Color255_Chocolate        ((Color){210, 105, 30 }) // �ɿ���ɫ
#define Color255_Tan              ((Color){210, 180, 140}) // ɹ��ɫ
#define Color255_RosyViolet       ((Color){131, 111, 255}) // õ����ɫ
#define Color255_Plum             ((Color){221, 160, 221}) // ����ɫ
#define Color255_Violet           ((Color){238, 130, 238}) // ������ɫ
#define Color255_MediumPurple     ((Color){147, 112, 219}) // ����ɫ
#define Color255_Orchid           ((Color){218, 112, 214}) // ����ɫ
#define Color255_DarkOrchid       ((Color){153, 50,  204}) // ��������ɫ
#define Color255_Thistle          ((Color){216, 191, 216}) // ��ɫ


    typedef struct  {
        JHAL_IO_Port port ;
        u8 pin;
        bool activeLevle;
        //��Ҫ����һ�������������Colorֵ�� ���ٿŵ��������С�Ͷ���
        Color *ledsArray;
        //����ĸ�����������Ĵ�С
        u8 ledsNumber;
    } XL_5050RGBC_WS2812B_Config;





    void XL_5050RGBC_WS2812B_open(XL_5050RGBC_WS2812B_Config *config);

    /**
    * @brief ����LED�������ɫ
    *
    * @param index LED���������
    * @param color LED�������ɫ
    *float brightness  ����0-1
    */
    bool XL_5050RGBC_WS2812B_setLedColor(XL_5050RGBC_WS2812B_Config *config,uint8_t index, Color color, float brightness) ;
    //���ú���Ҫ���²���Ч����ͬʱ���ö��
    void XL_5050RGBC_WS2812B_update(XL_5050RGBC_WS2812B_Config *config) ;





#ifdef CplusPlus
}
#endif

#endif


