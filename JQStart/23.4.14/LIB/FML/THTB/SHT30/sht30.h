#ifndef _SHT30_H
#define _SHT30_H
 
#include "myiic.h"
#include "delay.h"
typedef struct
{
 uint8_t sht30_data_buffer[6];
 int SHT30_humidity ;
 int SHT30_temperature;
}Data_Process;

void SHT_Init(void);
extern Data_Process  data_process;
int sht30_data_process(void);
#endif
