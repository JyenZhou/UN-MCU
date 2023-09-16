#ifndef __TPS7A7100_2_RGW__H__
#define __TPS7A7100_2_RGW__H__

#in
#include "TPS7A7100_RGW.h"

#ifdef __CplusPlus
extern "C" {
#endif
    /*Òý½Å¶¨Òå*/
#define TPS7A7100_2_mVfor50_EN_Pin Uh_7100_50mV_Pin
#define TPS7A7100_2_mVfor50_EN_GPIO_Port Uh_7100_50mV_GPIO_Port
#define TPS7A7100_2_mVfor100_EN_Pin Uh_7100_100mV_Pin
#define TPS7A7100_2_mVfor100_EN_GPIO_Port Uh_7100_100mV_GPIO_Port
#define TPS7A7100_2_mVfor200_EN_Pin Uh_7100_200mV_Pin
#define TPS7A7100_2_mVfor200_EN_GPIO_Port Uh_7100_200mV_GPIO_Port
#define TPS7A7100_2_mVfor400_EN_Pin Uh_7100_400mV_Pin
#define TPS7A7100_2_mVfor400_EN_GPIO_Port Uh_7100_400mV_GPIO_Port
#define TPS7A7100_2_mVfor800_EN_Pin Uh_7100_800mV_Pin
#define TPS7A7100_2_mVfor800_EN_GPIO_Port Uh_7100_800mV_GPIO_Port
#define TPS7A7100_2_for_1V6_Pin Uh_7100_1600mV_Pin
#define TPS7A7100_2_for_1V6_GPIO_Port Uh_7100_1600mV_GPIO_Port
#define TPS7A7100_2_RWG_EN_Pin Uh_7100_0V9_3V_EN_Pin
#define TPS7A7100_2_RWG_EN_Port Uh_7100_0V9_3V_EN_GPIO_Port




#define TPS7A7100_2_RWG_ENABLE      HAL_GPIO_WritePin(TPS7A7100_2_RWG_EN_Port,TPS7A7100_2_RWG_EN_Pin,GPIO_PIN_SET)

#define TPS7A7100_2_RWG_DISABLE	 HAL_GPIO_WritePin(TPS7A7100_2_RWG_EN_Port,TPS7A7100_2_RWG_EN_Pin,GPIO_PIN_RESET)








    void TPS7A7100_2_RWG_init(void);
    uint8_t TPS7A7100_2SelectFunc(TPS7A7100_SELECT_TypeDef TPS7A7100_select);


#ifdef CplusPlus
}
#endif

#endif


