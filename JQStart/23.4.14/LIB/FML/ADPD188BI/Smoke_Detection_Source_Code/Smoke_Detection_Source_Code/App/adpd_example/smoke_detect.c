#include "smoke_detect.h"
#include <drivers/pwr/adi_pwr.h>
#include <stdio.h>
#include <string.h>


uint32_t dcfg_org_188[] = {
  0x00020005,
  0x00060400,
  0x00030000,
  0x00050000, 
  
  0x000C000F,
  0x000D0000,
  0x00100000,
  0x001130A9,
  0x004F2090,
  0x000B0000,
  0x00500000,
  0x00510000,
  0x00520040,
  0x0053E400,
  0x00570000,
  0x005F0000,
  0x00460000,
  0x00470080,
  0x00480000,
  0x00490000,
  0x004A0000,
  0x00260000,
  0x00270800,
  0x00280000,
  0x0029A00A,
  0x00130190,
  0x002A9000,
  0x002B8F00,
  0x002C9400,
  0x002D9200,
  0x002E0000,
  0x002F0000,
  0x00320320,
  0x00330113,
  0x003A22D4,
  0x005C0000,
  
  0x001203EB,
  0x00150330,
  
  0x0014011D,
  
  0x00180000,
  0x00193FFF,
  0x001A3FFF,
  0x001B3FFF,
  0x00300320,
  0x0031080e,
  0x003922F0,
  0x00421C34,
  0x0043ADA5,
  0x005E0808,
  0x00163000,
  0x00170009,
  
  0x001E0000,
  0x001F3FFF,
  0x00203FFF,
  0x00213FFF,
  0x00350320,
  0x0036080e,
  0x003B22F0,
  0x00441C34,
  0x0045ADA5,
  0x00590808,
  0x001C3000,
  0x001D0009,
  
  0x00580544,
  
  0x00223539,
  0x00233536,
  0x00241530,
  0x0025630C,
  0x00340000,
  
  0x003C3006,
  0x003D0000,
  0x00540AA0,
  0x00550000,
  0x005A0010,
  
  0x00401010,
  0x0041004C,
  0x004E7040,

  0xFFFFFFFF,    
  
};


uint8_t value[16];
uint32_t readCodeBuffer[8];
uint8_t nLoopLim;


extern uint32_t calibrationcount;
extern uint8_t iscali;

extern uint16_t nAdpdDataSetSize;
extern uint16_t nAdpdFifoLevelSize;
extern uint32_t blueOffset, irOffset;

void LoadDefaultConfig(uint32_t *cfg)
{
  uint8_t regAddr, i;
  uint16_t regData;
  if (cfg == 0) {
    return;
  }
  

  AdpdDrvRegWrite(0x10, 0);
  AdpdDrvRegWrite(0x5F, 1);
  AdpdDrvRegWrite(0x00, 0x80FF);
  AdpdDrvRegWrite(0x5F, 0);
  i = 0;
  while (1) {

    regAddr = (uint8_t)(cfg[i] >> 16);
    regData = (uint16_t)(cfg[i]);
    i++;
    if (regAddr == 0xFF) {
      break;
    }
    

    if (AdpdDrvRegWrite(regAddr, regData) != ADPDDrv_SUCCESS) {
      break;
    }
  }
}


void VerifyDefaultConfig(uint32_t *cfg)
{
  uint16_t def_val;
  uint8_t  i;
  uint8_t  regAddr;
  uint16_t regData;
  if (cfg == 0) {
    return;
  }
  i = 0;

  regAddr = (uint8_t)(cfg[0] >> 16);
  def_val = (uint16_t)(cfg[0]);
  

  while (regAddr != 0xFF) {
    if (AdpdDrvRegRead(regAddr, &regData) != ADPDDrv_SUCCESS) {
      return;
    } else if (regData != def_val) {
      return;
    }
    i++;
    regAddr = (uint8_t)(cfg[i] >> 16);
    def_val = (uint16_t)(cfg[i]);
  }
}

void AdpdConfiguration(CONFIG_TYPE cType)
{

  AdpdDrvSoftReset();

  LoadDefaultConfig(dcfg_org_188);

  VerifyDefaultConfig(dcfg_org_188);

  AdpdDrvRegWrite(0x004B, 0x2695);
  AdpdDrvRegWrite(0x004D, 0x4272);

  AdpdDrvSetSlot(ADPDDrv_SUM_32, ADPDDrv_SUM_32);
  nLoopLim = nAdpdDataSetSize = 8; 
 

}


void Calibrate32KClock(void){
  uint8_t avgSlotA, avgSlotB;
  uint16_t rateSlot;
  uint16_t nAdpdFifoLevelSize = 0;
  uint16_t clock32k;
  uint32_t countnumber = 0;
  uint8_t counttimes = 0;
  dataADPD dataRead;
  uint32_t slotAtemp=0, slotBtemp=0;
  
  AdpdDrvGetSamplingFrequency(&rateSlot);
  AdpdDrvGetInternalAverage(&avgSlotA, SLOT_A);
  AdpdDrvGetInternalAverage(&avgSlotB, SLOT_B);
  
  while(1){
    SysTick_Config(26000);
    AdpdDrvSetSlot(ADPDDrv_SUM_32, ADPDDrv_SUM_32);
    AdpdDrvRegWrite(0x00, 0x80FF);
    AdpdDrvSetOperationMode(ADPDDrv_MODE_SAMPLE);
    
    
    calibrationcount = 0;
    
    while(1){
      AdpdDrvGetParameter(ADPD_FIFOLEVEL, &nAdpdFifoLevelSize);
      if(nAdpdFifoLevelSize == 8){
        dataRead=AdpdDataRead(OPTICAL);
        counttimes++;
        slotAtemp = dataRead.dataSlotA;
        slotBtemp = dataRead.dataSlotB;
        
        AdpdDrvSetOperationMode(ADPDDrv_MODE_IDLE); 
        countnumber = calibrationcount;
        break;
      }
    }
    
    
    printf("times: %d count: %d\r\n", counttimes, countnumber);
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
    
    AdpdDrvRegRead(0x4B,&clock32k);
    if(countnumber<990){
      clock32k++;
      AdpdDrvRegWrite(0x4B,clock32k);
    }
    else if(countnumber>1010){
      clock32k--;
      AdpdDrvRegWrite(0x4B,clock32k);
    }
    else{
      break;
    }    
  }
  blueOffset = slotAtemp;
  irOffset = slotBtemp;
  printf("%d, %d\r\n", blueOffset, irOffset);
  AdpdDrvSetSamplingFrequency(rateSlot);
  AdpdDrvSetInternalAverage(avgSlotA, SLOT_A);
  AdpdDrvSetInternalAverage(avgSlotB, SLOT_B);
}


void AdpdClockCalibration(void)
{
  iscali = 1;
  Calibrate32KClock();
  Calibrate32MClock();
  iscali = 0;
}

void Calibrate32MClock(void){
  uint16_t clkfifo, clk32m_en, clk32m;
  uint16_t clk32madj;
  uint32_t i;
  float clockerror=0;
  
  AdpdDrvSetSlot(ADPDDrv_SUM_32, ADPDDrv_SUM_32);
  AdpdDrvRegWrite(0x00, 0x80FF);
  AdpdDrvSetOperationMode(ADPDDrv_MODE_SAMPLE);
  
  AdpdDrvRegRead(REG_FIFO_CLK, &clkfifo);
  AdpdDrvRegWrite(REG_FIFO_CLK, 1);
  AdpdDrvRegWrite(REG_FIFO_CLK, 1);
  AdpdDrvRegRead(REG_OSC32M_CAL, &clk32m_en);
  AdpdDrvRegWrite(REG_OSC32M_CAL, 0x20);
  for(i=0; i<2000; i++);
  AdpdDrvRegRead(REG_CLK_RATIO, &clk32m);
  printf("32M count: %d\r\n", clk32m);
  
  clockerror = 32000000.0*(1-(float)clk32m/2000.0);
  printf("32M clockerror: %f\r\n", clockerror);
  AdpdDrvRegRead(REG_OSC32M_ADJ, &clk32madj);
  clk32madj = clk32madj+(int)(clockerror/109000);
  printf("32M clock adj: %d\r\n", clk32madj);
  
  AdpdDrvRegWrite(REG_OSC32M_ADJ, clk32madj);
 
  AdpdDrvRegWrite(REG_FIFO_CLK, 0);
  AdpdDrvRegWrite(REG_OSC32M_CAL, 0);
  
  AdpdDrvSetOperationMode(ADPDDrv_MODE_IDLE);
}


dataADPD AdpdDataRead(CONFIG_TYPE cType)
{
  uint32_t LoopCnt;
  uint16_t nRetValue = 0;
  
  dataADPD readsensor;
    nRetValue = AdpdDrvReadFifoData(&value[0], nAdpdDataSetSize);
    if (nRetValue == ADPDDrv_SUCCESS) 
    {
      for (LoopCnt = 0; LoopCnt < nLoopLim; LoopCnt += 4){
        
        readCodeBuffer[LoopCnt>>2] = (value[LoopCnt] << 8) 
          | (value[LoopCnt+1] )
            |(value[LoopCnt+2] << 24)
              |value[LoopCnt + 3]<<16;
      }
      nAdpdFifoLevelSize = nAdpdFifoLevelSize - nAdpdDataSetSize;      
    }
    readsensor.dataSlotA = readCodeBuffer[0];
    readsensor.dataSlotB = readCodeBuffer[1];
  
  return readsensor;
}

float CodeToPTRFactor(AdpdSlotId nSlotId)
{
  float valuePTR = 1.0;
  float pdResolution = 0.41;
  float currentSetting = 100.0;
  
  uint8_t nPulse;
  float pdAmpPerWatt, ledWattPerAmp, nomialWattPerAmp, derateFactor;
  
  uint16_t pulseWidth = 0;
  if( nSlotId == SLOT_A ){
    AdpdDrvRegRead(REG_AFE_CTRL_A, &pulseWidth);
  }
  else{
    AdpdDrvRegRead(REG_AFE_CTRL_B, &pulseWidth);
  }
  
  uint8_t tiaGain;  
  AdpdDrvGetAfeTiaGain(&tiaGain, nSlotId);
  if( (pulseWidth>>11) == 4){
    switch (tiaGain) {
    case 0:
      pdResolution = 0.41;
      break;
    case 1:
      pdResolution = 0.82;
      break;
    case 2:
      pdResolution = 1.64;
      break;
    case 3:
      pdResolution = 3.27;
      break;
    default:
      break;
    }
  }
  else{
    switch (tiaGain) {
    case 0:
      pdResolution = 0.58;
      break;
    case 1:
      pdResolution = 1.15;
      break;
    case 2:
      pdResolution = 2.31;
      break;
    case 3:
      pdResolution = 4.62;
      break;
    default:
      break;    
    }
  }
  
  AdpdDrvGetNumPulses(&nPulse, nSlotId);
  
  uint16_t currentTemp1, currentTemp2;
  float coarseLED, scaleLED, fineLED;
  AdpdDrvRegRead(REG_LED_TRIM, &currentTemp2);
  if( nSlotId == SLOT_A ){
    AdpdDrvRegRead(REG_LED1_DRV, &currentTemp1);  
    fineLED = 0.74 + 0.022 *(currentTemp2&0x1F);
  }
  else{
    AdpdDrvRegRead(REG_LED3_DRV, &currentTemp1);
    fineLED = 0.74 + 0.022 *(currentTemp2>>11);
  }
  
  
  coarseLED = 50.3 + 19.8 * (currentTemp1&0x000F);
  if( (currentTemp1&0x2000) == 0){
    scaleLED = 0.1;
  }
  else{
    scaleLED = 1.0;
  }
  
  currentSetting = coarseLED*scaleLED*fineLED;
    
  if( nSlotId == SLOT_A ){
    pdAmpPerWatt = 0.26;
    nomialWattPerAmp = 0.38;
    float a0, a1, a2, a3;
    a0 = 9.8976e-1;
    a1 = -5.1448e-3;
    a2 = 2.0287e-5;
    a3 = -2.9645e-8;    
    derateFactor = a0 + a1 * currentSetting + 
      a2 * currentSetting * currentSetting + 
      a3* currentSetting * currentSetting * currentSetting;
  }
  else{
    pdAmpPerWatt = 0.41;
    nomialWattPerAmp = 0.22;
    derateFactor = 1.0;
  }
  
  ledWattPerAmp = nomialWattPerAmp * derateFactor;
      
  valuePTR = pdResolution/(float)nPulse/currentSetting/pdAmpPerWatt/ledWattPerAmp;
  return valuePTR;
}

uint32_t OffsetCalibration(uint32_t *w)
{
  uint8_t k,j;
  uint32_t temp;
  for (k = 0; k < 5; k++){
    for (j = k+1; j < 5; j++){
      if (w[k] > w[j]){
        temp = w[k];
        w[k] = w[j];
        w[j] = temp;
      }
    }
  }
  return w[2];
}


dataCal LedCurrent(void)
{
  dataCal Led_Current;
  uint16_t currentTemp1, currentTemp2;
  float coarseLED, scaleLED, fineLED;
  AdpdDrvRegRead(REG_LED_TRIM, &currentTemp2);
  
  AdpdDrvRegRead(REG_LED1_DRV, &currentTemp1); 
  fineLED = 0.74 + 0.022 *(currentTemp2&0x1F);
  coarseLED = 50.3 + 19.8 * (currentTemp1&0x000F);  
  if( (currentTemp1&0x2000) == 0){
    scaleLED = 0.1;
  }
  else{
    scaleLED = 1.0;
  } 
  Led_Current.dataBlue = coarseLED*scaleLED*fineLED;
  
  AdpdDrvRegRead(REG_LED3_DRV, &currentTemp1);//register 0x22
  fineLED = 0.74 + 0.022 *(currentTemp2>>11);
  coarseLED = 50.3 + 19.8 * (currentTemp1&0x000F);  
  if( (currentTemp1&0x2000) == 0){
    scaleLED = 0.1;
  }
  else{
    scaleLED = 1.0;
  } 
  Led_Current.dataIR = coarseLED*scaleLED*fineLED;
  
  return Led_Current;
}


dataCal eFuseCal(void)
{
  dataCal Gain_Cal;
  dataCal Led_Current;
  
  uint16_t pReg_Values[5];
  uint8_t ModuleID;
  float LED1_GAIN_COEFF;
  float LED3_GAIN_COEFF;
  float LED1_INT_COEFF;
  float LED3_INT_COEFF;
   
  float Gain_Blue;
  float Gain_IR;

  float Device_Scalar_Blue;
  float Device_Scalar_IR;

  float Intercept_Blue;
  float Intercept_IR;

  float Nominal_Scalar_Blue;
  float Nominal_Scalar_IR;

  float Mean_Gain_Blue=17;
  float Mean_Gain_IR=34;

  float Mean_Intercept_Blue=622;
  float Mean_Intercept_IR=128;
  
  AdpdDrvEfuseRead(pReg_Values,5); 
  
  ModuleID=pReg_Values[0];
  LED1_GAIN_COEFF=pReg_Values[1];
  LED3_GAIN_COEFF=pReg_Values[2];
  LED1_INT_COEFF=pReg_Values[3];
  LED3_INT_COEFF=pReg_Values[4];
  
  Led_Current=LedCurrent();

  if((ModuleID==0x1E)|(ModuleID==0x1F))//ID 30 and 31
  {      
    Mean_Gain_Blue=17;
    Mean_Gain_IR=34;

    Mean_Intercept_Blue=622;
    Mean_Intercept_IR=128;
  
    Gain_Blue=((LED1_GAIN_COEFF-112)*17+17*256)/256; 
    Gain_IR=((LED3_GAIN_COEFF-112)*34+34*256)/256;
    
    Intercept_Blue=8*(LED1_INT_COEFF-128);
    Intercept_IR=5*(LED3_INT_COEFF-128);
    
    Device_Scalar_Blue=Gain_Blue*Led_Current.dataBlue+Intercept_Blue;
    Device_Scalar_IR=Gain_IR*Led_Current.dataIR+Intercept_IR;
    
    Nominal_Scalar_Blue=Mean_Gain_Blue*Led_Current.dataBlue+Mean_Intercept_Blue;
    Nominal_Scalar_IR=Mean_Gain_IR*Led_Current.dataIR+Mean_Intercept_IR;    
  }
  else if(ModuleID==0x21)//ID 33
  {
    Mean_Gain_Blue=21;
    Mean_Gain_IR=42;

    Mean_Intercept_Blue=753;
    Mean_Intercept_IR=156;
    
    Gain_Blue=((LED1_GAIN_COEFF-112)*21+21*256)/256;
    Gain_IR=((LED3_GAIN_COEFF-112)*42+42*256)/256;
    
    Intercept_Blue=8*(LED1_INT_COEFF-80);
    Intercept_IR=5*(LED3_INT_COEFF-80);
    
    Device_Scalar_Blue=Gain_Blue*Led_Current.dataBlue+Intercept_Blue;
    Device_Scalar_IR=Gain_IR*Led_Current.dataIR+Intercept_IR;
    
    Nominal_Scalar_Blue=Mean_Gain_Blue*Led_Current.dataBlue+Mean_Intercept_Blue;
    Nominal_Scalar_IR=Mean_Gain_IR*Led_Current.dataIR+Mean_Intercept_IR;      
  }
  
  Gain_Cal.dataBlue=Device_Scalar_Blue/Nominal_Scalar_Blue;
  Gain_Cal.dataIR=Device_Scalar_IR/Nominal_Scalar_IR;  
  
  return Gain_Cal;
}


