#ifndef __STATEANALYSIS__H__
#define  __STATEANALYSIS__H__
#include "JSYS.h"

#ifdef __CplusPlus
extern "C" {
#endif

    extern uint8 ISLogin;//µÇÂ¼×´Ì¬

    void Login(void);
    uint8 LoginState (char* MsgType);
    void json_dataSynthesis (char data[]);
    void sendHeartbeat(void);
    void sendDataForUarttoNetworkport(char dat[]);
#ifdef CplusPlus
}
#endif

#endif


