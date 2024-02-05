#ifndef __MCP4725_H
#define __MCP4725_H
#include "myiic2.h"   


void MCP4725_Init(void);
void MCP4725_setVoltage_CodeValue(uint16_t codeValue);
void MCP4725_setVoltage_mV(uint16_t Vout) ;

#endif

