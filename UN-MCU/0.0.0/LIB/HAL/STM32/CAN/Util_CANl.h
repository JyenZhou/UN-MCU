#ifndef __CANUTIL__H__
#define  __CANUTIL__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "Hall_includes.h"

 #if defined(EN_CAN1_STDMODE) || defined(EN_CAN1_EXTMODE)
 extern CAN_HandleTypeDef hcan;

#define hcan1 hcan
#endif


    void bisp_CANUtil_Init (void);
void can1_sendData(uint8_t txData[]);


#ifdef CplusPlus
}
#endif

#endif




