#ifndef _SYSTEM_H
#define _SYSTEM_H


#include "JFML.h"




extern JHAL_CANBaudRate canBaudRate;

extern uint8_t Module_Mode_Select_Flag;
extern uint8_t g_mode_change_flag;
extern float g_abnormal_power_rs0[2];
extern uint8_t module_manufacture_date[10];



void jSystemInit(void);
void jSystemRun(void);

#endif



