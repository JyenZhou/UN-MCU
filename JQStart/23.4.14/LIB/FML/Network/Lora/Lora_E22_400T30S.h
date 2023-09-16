#ifndef _LORA_E22_400T30S_h_
#define _LORA_E22_400T30S_h_
#include "JHAL.h"

#define Lora_E22_400T30S_AUX_GPIO_Port Lora_AUX_GPIO_Port
#define Lora_E22_400T30S_AUX_Pin       Lora_AUX_Pin
#define Lora_E22_400T30S_M1_GPIO_Port  Lora_M1_GPIO_Port
#define Lora_E22_400T30S_M1_Pin			Lora_M1_Pin
#define Lora_E22_400T30S_M0_GPIO_Port		Lora_M0_GPIO_Port
#define Lora_E22_400T30S_M0_Pin				Lora_M0_Pin

#define  Lora_baseConfigurationDataLength 10

void Lora_E22_400T30S_Init(void);
void lora_Configuration(uint16 deviceAddress,uint8 networkAddress,uint8 channel);
void lora_WokeModeSwitc(uint8 mode);
bool lora_cheakLastConfiguration(uint8 *data,uint8 dataLength);
#endif

