#ifndef __JUTIL_INCLUDES__H__
#define  __JUTIL_INCLUDES__H__
#ifdef __CplusPlus
extern "C" {
#endif

#include "JSYS.h"

#include "Util/JHAL_CRC.h"
#include "Util/JHAL_Math.h"
#include "Util/JHAL_NumberConverter.h"

void JHAL_systemReset(void);
u32 JHAL_uidGetHigh(void);
u32 JHAL_uidGetMiddle(void);
u32 JHAL_uidGetLow(void);

void  JHAL_enableInterrupts(void);
	void  JHAL_disableInterrupts(void);
extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




