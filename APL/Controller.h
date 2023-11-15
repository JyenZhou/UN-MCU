#ifndef __CONTROL__H__
#define __CONTROL__H__
#include "JFML.h"
#include "gatt.h"
#ifdef __CplusPlus
extern "C" {
#endif


    void controllerInit(void);
    void vhAllClose(void);
    void  vhRadioOpen(u8 column) ;

#ifdef CplusPlus
}
#endif

#endif


