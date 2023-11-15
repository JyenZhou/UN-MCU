#ifndef __communication__H__
#define __communication__H__
#include "JFML.h"
#include "gatt.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define CMD_GetVoltage  1




    void sendGASVoltage2NetUart (float* vt,u8 number,u8 ads1115Status);

    u8  parseNetUartData(void);
#ifdef CplusPlus
}
#endif

#endif


