#ifndef __SMOKEDETECT__H
#define __SMOKEDETECT__H

#include <stdint.h>
#include "AdpdDrv.h"

typedef enum {
    BLUE_LED = 0x00,
    IR_LED = 0x01
} LED_WAVELENGTH;

typedef enum {
    OPTICAL = 0x00,
    TEMP_HUMI = 0x01
} CONFIG_TYPE;

typedef struct
{
    uint32_t dataSlotA;
    uint32_t dataSlotB;
} dataADPD;

typedef struct
{
    float dataBlue;
    float dataIR;
} dataCal;


void AdpdConfiguration(CONFIG_TYPE cType);
void LoadDefaultConfig(uint32_t *cfg);
void VerifyDefaultConfig(uint32_t *cfg);


void Calibrate32KClock(void);
void Calibrate32MClock(void);
void AdpdClockCalibration(void);

dataADPD AdpdDataRead(CONFIG_TYPE cType);

float CodeToPTRFactor(AdpdSlotId nSlotId);

uint32_t OffsetCalibration(uint32_t *w);
dataCal LedCurrent(void);
dataCal eFuseCal(void);
#endif

