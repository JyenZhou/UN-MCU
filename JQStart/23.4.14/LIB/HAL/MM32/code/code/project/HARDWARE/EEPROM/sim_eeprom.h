#ifndef __SIM_EEPROM_H
#define __SIM_EEPROM_H

#include "main.h"

void FLASH_SIM_EEPROM_Init(void);

int32_t  EEPROM_Write	(uint8_t* ptr, uint16_t len);		//len 必须是2的指数倍
int32_t  EEPROM_Read	(uint8_t* ptr, uint16_t len);
int32_t  FLASH_SIM_EEPROM_Test(void);

#endif

