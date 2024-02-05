#ifndef __TPS7A4700_RGWR__H__
#define __TPS7A4700_RGWR__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif
/*引脚定义*/

#define TPS7A4700_RWG_mVfor100_EN_Pin Uc_4700_100mV_Pin
#define TPS7A4700_RWG_mVfor100_EN_GPIO_Port Uc_4700_100mV_GPIO_Port
#define TPS7A4700_RWG_mVfor200_EN_Pin Uc_4700_200mV_Pin
#define TPS7A4700_RWG_mVfor200_EN_GPIO_Port Uc_4700_200mV_GPIO_Port
#define TPS7A4700_RWG_mVfor400_EN_Pin Uc_4700_400mV_Pin
#define TPS7A4700_RWG_mVfor400_EN_GPIO_Port Uc_4700_400mV_GPIO_Port
#define TPS7A4700_RWG_mVfor800_EN_Pin Uc_4700_800mV_Pin
#define TPS7A4700_RWG_mVfor800_EN_GPIO_Port Uc_4700_800mV_GPIO_Port
#define TPS7A4700_RWG_1V6_EN_Pin Uc_4700_1600mV_Pin
#define TPS7A4700_RWG_1V6_EN_GPIO_Port Uc_4700_1600mV_GPIO_Port
#define TPS7A4700_RWG_3V2_EN_Pin Uc_4700_3200mV_Pin
#define TPS7A4700_RWG_3V2_EN_GPIO_Port Uc_4700_3200mV_GPIO_Port 
//暂时用不上这个等级就没写了
//#define TPS7A4700_RWG_6V4_EN_Pin EN_1V6_Pin
//#define TPS7A4700_RWG_6V4_EN_GPIO_Port EN_1V6_GPIO_Port

#define TPS7A4700_RWG_EN_Pin Uc_4700_3V1_5V_EN_Pin
#define TPS7A4700_RWG_EN_Port Uc_4700_3V1_5V_EN_GPIO_Port






#define TPS7A4700_RWG_ENABLE     HAL_GPIO_WritePin(TPS7A4700_RWG_EN_Port,TPS7A4700_RWG_EN_Pin,GPIO_PIN_SET)
#define TPS7A4700_RWG_DISABLE    HAL_GPIO_WritePin(TPS7A4700_RWG_EN_Port,TPS7A4700_RWG_EN_Pin,GPIO_PIN_RESET)









typedef enum
{
	 TPS7A4700_SELECT_0V0     =  ((uint8_t)0x00),
    TPS7A4700_SELECT_1V4     =  ((uint8_t)0x01),
    TPS7A4700_SELECT_1V5     =  ((uint8_t)0x02),
    TPS7A4700_SELECT_1V6    =  ((uint8_t)0x03),
    TPS7A4700_SELECT_1V7     =  ((uint8_t)0x04),
    TPS7A4700_SELECT_1V8    =  ((uint8_t)0x05),
    TPS7A4700_SELECT_1V9     =  ((uint8_t)0x06),
    TPS7A4700_SELECT_2V0    =  ((uint8_t)0x07),  
    TPS7A4700_SELECT_2V1    =  ((uint8_t)0x08),
    TPS7A4700_SELECT_2V2     =  ((uint8_t)0x09),
    TPS7A4700_SELECT_2V3    =  ((uint8_t)0x0a),
    TPS7A4700_SELECT_2V4     =  ((uint8_t)0x0b),
    TPS7A4700_SELECT_2V5    =  ((uint8_t)0x0c),
    TPS7A4700_SELECT_2V6     =  ((uint8_t)0x0d),
    TPS7A4700_SELECT_2V7    =  ((uint8_t)0x0e),
    TPS7A4700_SELECT_2V8     =  ((uint8_t)0x0f),
    TPS7A4700_SELECT_2V9    =  ((uint8_t)0x10),
    TPS7A4700_SELECT_3V0     =  ((uint8_t)0x11),
    TPS7A4700_SELECT_3V1    =  ((uint8_t)0x12),
    TPS7A4700_SELECT_3V2     =  ((uint8_t)0x13),
    TPS7A4700_SELECT_3V3    =  ((uint8_t)0x14),
    TPS7A4700_SELECT_3V4     =  ((uint8_t)0x15),
    TPS7A4700_SELECT_3V5    =  ((uint8_t)0x16),
    TPS7A4700_SELECT_3V6     =  ((uint8_t)0x17),
    TPS7A4700_SELECT_3V7    =  ((uint8_t)0x18),
    TPS7A4700_SELECT_3V8     =  ((uint8_t)0x19),
    TPS7A4700_SELECT_3V9    =  ((uint8_t)0x1a),
    TPS7A4700_SELECT_4V0     =  ((uint8_t)0x1b),
    TPS7A4700_SELECT_4V1    =  ((uint8_t)0x1c),
    TPS7A4700_SELECT_4V2     =  ((uint8_t)0x1d),
    TPS7A4700_SELECT_4V3    =  ((uint8_t)0x1e),
    TPS7A4700_SELECT_4V4     =  ((uint8_t)0x1f),
    TPS7A4700_SELECT_4V5    =  ((uint8_t)0x20),
    TPS7A4700_SELECT_4V6     =  ((uint8_t)0x21),
    TPS7A4700_SELECT_4V7    =  ((uint8_t)0x22),
    TPS7A4700_SELECT_4V8     =  ((uint8_t)0x23),
    TPS7A4700_SELECT_4V9    =  ((uint8_t)0x24),
    TPS7A4700_SELECT_5V0     =  ((uint8_t)0x25),
    TPS7A4700_SELECT_5V1    =  ((uint8_t)0x26),
    TPS7A4700_SELECT_5V2     =  ((uint8_t)0x27),
    TPS7A4700_SELECT_5V3    =  ((uint8_t)0x28),
    TPS7A4700_SELECT_5V4     =  ((uint8_t)0x29),
    TPS7A4700_SELECT_5V5    =  ((uint8_t)0x2a),
    TPS7A4700_SELECT_5V6     =  ((uint8_t)0x2b),
    TPS7A4700_SELECT_5V7    =  ((uint8_t)0x2c),


} TPS7A4700_SELECT_TypeDef;



void TPS7A4700_RWG_init(void);
uint8_t TPS7A4700SelectFunc(TPS7A4700_SELECT_TypeDef TPS7A4700_select);

#ifdef CplusPlus
}
#endif

#endif


