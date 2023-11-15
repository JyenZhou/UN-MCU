#ifndef __TPS7A4700_2_RGWR__H__
#define __TPS7A4700_2_RGWR__H__

#include "TPS7A4700_RGWR.h"
#ifdef __CplusPlus
extern "C" {
#endif
/*���Ŷ���*/

#define TPS7A4700_2_RWG_mVfor100_EN_Pin Uh_4700_100mV_Pin
#define TPS7A4700_2_RWG_mVfor100_EN_GPIO_Port Uh_4700_100mV_GPIO_Port
#define TPS7A4700_2_RWG_mVfor200_EN_Pin Uh_4700_200mV_Pin
#define TPS7A4700_2_RWG_mVfor200_EN_GPIO_Port Uh_4700_200mV_GPIO_Port
#define TPS7A4700_2_RWG_mVfor400_EN_Pin Uh_4700_400mV_Pin
#define TPS7A4700_2_RWG_mVfor400_EN_GPIO_Port Uh_4700_400mV_GPIO_Port
#define TPS7A4700_2_RWG_mVfor800_EN_Pin Uh_4700_800mV_Pin
#define TPS7A4700_2_RWG_mVfor800_EN_GPIO_Port Uh_4700_800mV_GPIO_Port
#define TPS7A4700_2_RWG_1V6_EN_Pin Uh_4700_1600mV_Pin
#define TPS7A4700_2_RWG_1V6_EN_GPIO_Port Uh_4700_1600mV_GPIO_Port
#define TPS7A4700_2_RWG_3V2_EN_Pin Uh_4700_3200mV_Pin
#define TPS7A4700_2_RWG_3V2_EN_GPIO_Port Uh_4700_3200mV_GPIO_Port 
//��ʱ�ò�������ȼ���ûд��
//#define TPS7A4700_2_RWG_6V4_EN_Pin EN_1V6_Pin
//#define TPS7A4700_2_RWG_6V4_EN_GPIO_Port EN_1V6_GPIO_Port

#define TPS7A4700_2_RWG_EN_Pin Uh_4700_3V1_5V_EN_Pin
#define TPS7A4700_2_RWG_EN_Port Uh_4700_3V1_5V_EN_GPIO_Port


#define TPS7A4700_2_RWG_ENABLE      HAL_GPIO_WritePin(TPS7A4700_2_RWG_EN_Port,TPS7A4700_2_RWG_EN_Pin,GPIO_PIN_SET)
#define TPS7A4700_2_RWG_DISABLE     HAL_GPIO_WritePin(TPS7A4700_2_RWG_EN_Port,TPS7A4700_2_RWG_EN_Pin,GPIO_PIN_RESET)










void TPS7A4700_2_RWG_init(void);
uint8_t TPS7A4700_2SelectFunc(TPS7A4700_SELECT_TypeDef TPS7A4700_2_select);

#ifdef CplusPlus
}
#endif

#endif

