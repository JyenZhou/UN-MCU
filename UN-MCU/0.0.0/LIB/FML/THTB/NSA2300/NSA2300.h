#ifndef __NSA2300__H__
#define __NSA2300__H__
#ifdef __cplusplus
extern "C" {
#endif
#include "JHAL.h"

//NSA2300Æ÷¼þµØÖ·
#define  NSA2300_DevAddress 0xDA
extern I2C_HandleTypeDef hi2c1;
void systemRun(void);
uint8 nsa2300_Init(void);
uint8 NSA2300SigalProcess(float* temperature,float* ntc);
#ifdef __cplusplus
}
#endif

#endif

