/**
***************************************************************************
@file         example188.c
@author       ADI
@version      V3.0.0
@date         23-Apr-2018
@brief        Sample application to use ADI ADPD188 driver.
*/

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "AdpdDrv.h"
#include "Hardware.h"
#include "smoke_detect.h"

/* Macros -------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
uint8_t gnAdpdDataReady = 0;
uint32_t calibrationcount = 0;
uint8_t iscali = 0;  

uint16_t nAdpdFifoLevelSize;
uint16_t nAdpdDataSetSize;

float bluePTRFactor, irPTRFactor;
uint32_t blueOffset, irOffset;
uint32_t sampleIndex=0;
extern uint32_t dcfg_org_188[];

/* Private function prototypes ----------------------------------------------*/
void InitPinMux(void);
void MCUInit(void);
void AdpdGetPTRFactor(void);


void SysTick_Handler(void)
{
  if(iscali)  
  {
    calibrationcount++;
  }
  else  
  {
    AdpdDrvGetParameter(ADPD_FIFOLEVEL, &nAdpdFifoLevelSize);
    if(nAdpdFifoLevelSize >= nAdpdDataSetSize) 
    {
      gnAdpdDataReady = 1;
    }
  }
}

void main(void)
{
  dataADPD sensorData;
  dataCal eFuse_Cal;
  uint32_t CalibrationWindowA[5];
  uint32_t CalibrationWindowB[5];
  uint8_t CalibrationCount=0;
  MCUInit();
  AdpdConfiguration(OPTICAL);
  AdpdClockCalibration();  
  
  float ratioRaw, ratioPTR, ratioDelta;
  float ratioThreshold = 1.5;  

  float blueThreshold = 1.7;  
  float irThreshold = 0.7;
  float blueRaw,irRaw;
  float bluePTR, irPTR;  
  float blueDelta, irDelta;
  AdpdGetPTRFactor();
  
  PlayOne();
  AdpdDrvSetOperationMode(ADPDDrv_MODE_SAMPLE);
  eFuse_Cal=eFuseCal();
  printf("Gain Cal Blue: %f,Gain Cal Blue IR: %f\r\n", eFuse_Cal.dataBlue,eFuse_Cal.dataIR ); 
  SysTick_Config(26000*500);
  

  while(1)
  {   
    if(gnAdpdDataReady==1)
    {
         
      gnAdpdDataReady = 0; 

      if(CalibrationCount<5)
      {
        printf("Calibrating...\r\n");
        sensorData=AdpdDataRead(OPTICAL);
        CalibrationWindowA[CalibrationCount]=sensorData.dataSlotA;
        CalibrationWindowB[CalibrationCount]=sensorData.dataSlotB;
        CalibrationCount++;
        if(CalibrationCount==5)
        {
          blueOffset=OffsetCalibration(CalibrationWindowA);
          irOffset=OffsetCalibration(CalibrationWindowB);
        }
      }
      else
      {
      for(uint8_t times=0; times<nAdpdFifoLevelSize/nAdpdDataSetSize; times++)
      {
        sensorData=AdpdDataRead(OPTICAL);

        blueRaw = sensorData.dataSlotA;
        irRaw = sensorData.dataSlotB;
        ratioRaw=blueRaw/irRaw;

        bluePTR = (int)blueRaw*bluePTRFactor;
        irPTR = (int)irRaw*irPTRFactor;         
        ratioPTR=bluePTR/irPTR;
        
        blueDelta = ((int)sensorData.dataSlotA-(int)blueOffset)*bluePTRFactor;
        irDelta = ((int)sensorData.dataSlotB-(int)irOffset)*irPTRFactor;  
        if((blueDelta>0.5)&(irDelta>0.3))
        {
          ratioDelta=blueDelta/irDelta;
        }
        else
        {
          ratioDelta=1;
        }

        printf("PTR: %f,%f,%f\r\n", blueDelta, irDelta, ratioDelta);
       
      }
      
      if( (blueDelta>blueThreshold) && (irDelta>irThreshold) && (ratioDelta>ratioThreshold)){

        PlayOne();
      }
      }
    }
  }    
}

void InitPinMux(void)
{
#define UART0_TX_PORTP0_MUX  (1u<<20)
#define UART0_RX_PORTP0_MUX  (1u<<22)
  *((volatile uint32_t *)REG_GPIO0_CFG)|= UART0_TX_PORTP0_MUX | UART0_RX_PORTP0_MUX; 
  
#ifdef ADPD_SPI
#define SPI1_CS0_PORTP1_MUX  (1u<<18)
#define SPI1_MISO_PORTP1_MUX (1u<<16)
#define SPI1_MOSI_PORTP1_MUX (1u<<14)
#define SPI1_CLK_PORTP1_MUX  (1u<<12)
  *((volatile uint32_t *)REG_GPIO1_CFG) |= SPI1_CS0_PORTP1_MUX | SPI1_MISO_PORTP1_MUX | SPI1_MOSI_PORTP1_MUX | SPI1_CLK_PORTP1_MUX;  
  
#else
#define I2C0_SCL0_PORTP0_MUX (1u<< 8)
#define I2C0_SDA0_PORTP0_MUX (1u<<10)
  *((volatile uint32_t *)REG_GPIO0_CFG) |= I2C0_SCL0_PORTP0_MUX | I2C0_SDA0_PORTP0_MUX; 
#endif
  
#define BEEPER0_TONE_N_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<16))
#define BEEPER0_TONE_P_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<18)) 
  *pREG_GPIO0_CFG |= BEEPER0_TONE_N_PORTP0_MUX | BEEPER0_TONE_P_PORTP0_MUX; 
}


void MCUInit(void)
{
  Clock_Init(); 
  GPIO_Init();
  InitPinMux();
  UART_Init();
#ifdef ADPD_SPI
#else
  SPI_Init();
#endif
  I2C_Init();
  BEEP_Init();
}

void AdpdGetPTRFactor(void)
{
  bluePTRFactor = CodeToPTRFactor(SLOT_A); 
  irPTRFactor =  CodeToPTRFactor(SLOT_B);
}
