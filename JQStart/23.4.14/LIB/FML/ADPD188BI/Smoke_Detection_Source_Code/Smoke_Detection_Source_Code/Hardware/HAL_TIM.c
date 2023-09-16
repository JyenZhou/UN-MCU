/**
    ***************************************************************************
    * @file         HAL_TIM.c
    * @author       ADI
    * @version      V1.0.0
    * @date         11-June-2015
    * @brief        MCU TIMER related functions
    *
    ***************************************************************************
    * @attention
    ***************************************************************************
*/
/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2017 Analog Devices Inc.                                      *
* All rights reserved.                                                        *
*                                                                             *
* This source code is intended for the recipient only under the guidelines of *
* the non-disclosure agreement with Analog Devices Inc.                       *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
*                                                                             *
* This software is intended for use with the ADPD142 and derivative parts     *
* only                                                                        *
*                                                                             *
******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "system_ADuCM4050.h"
#include "HwIfConfig.h"
#include <drivers/tmr/adi_tmr.h>
#include <services/pwr/adi_pwr.h>
#include <services/gpio/adi_gpio.h>


#define TEMPERATURE_SAMPLING_PERIOD   1 /* 1 Hz*/
#define GP1_LOAD_VALUE_FOR_1S_PERIOD (510u)  

volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004; 
volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC; 

/* Private typedef ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//static uint8_t aDeviceMemory1[ADI_TMR_MEMORY_SIZE];
//ADI_TMR_HANDLE hDeviceTim;
//ADI_TMR_HANDLE hDeviceAD7689Tim;
static uint32_t initialHfoscCycles = 0;
uint8_t Timer_Flag = 0;
uint32_t finalHfoscCycles = 0;
//static uint8_t AD7689timerON = 0;


/* Private function prototypes -----------------------------------------------*/
static void HfoscClockCalCallback(void *pCBParam, uint32_t Event, void *pArg);
static void GP1CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg);
static  ADI_TMR_RESULT TIM1_Init(void);
uint32_t time_in_msec = 0;



/* ************************************************************************* */
/**
    * @brief    EDA TIMER initialization. This function initiaises the EDA timer
    * @param    None
    * @retval   None
    */
ADI_TMR_RESULT HAL_TIM_AD7689TimerDeInit() {
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
    
  //result = adi_tmr_Close(hDeviceAD7689Tim);
  
  return result;
}

/* ************************************************************************* */
/**
    * @brief    AD7689 TIMER initialization. This function initiaises the EDA timer
    * @param    None
    * @retval   None
    */
ADI_TMR_RESULT HAL_TIM_AD7689TimerInit(uint32_t *clkFrequency,uint8_t *preescaler,ADI_CALLBACK const pfCallback) {
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  ADI_TMR_CONFIG        tmrConfig;
  
  
  /*result = adi_tmr_Open(1, aDeviceMemory1, ADI_TMR_MEMORY_SIZE,
                        &hDeviceAD7689Tim);*/
  result = adi_tmr_Init(ADI_TMR_DEVICE_GP1, pfCallback, NULL, true);
  
  /*if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_RegisterCallback(hDeviceAD7689Tim, pfCallback, hDeviceAD7689Tim);*/
  
  /*if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetPrescaler(hDeviceAD7689Tim, ADI_GPT_PRESCALER_16);
  
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetClockSource(hDeviceAD7689Tim, ADI_TMR_CLOCK_HFOSC);
  
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetRunMode(hDeviceAD7689Tim, ADI_TMR_PERIODIC_MODE);
  
  adi_pwr_GetClockFrequency(ADI_CLOCK_HCLK, clkFrequency);
  *preescaler = ADI_GPT_PRESCALER_16;

    
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetCountMode(hDeviceAD7689Tim, ADI_TMR_COUNT_DOWN);*/
  /* Configure GP1 to have a period of 1 ms */
  tmrConfig.bCountingUp  = false;
  tmrConfig.bPeriodic    = true;
  tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_16;
  tmrConfig.eClockSource = ADI_TMR_CLOCK_HFOSC;
  tmrConfig.nLoad        = 0; //TODO :??
  tmrConfig.nAsyncLoad   = 0; //TODO :??
  tmrConfig.bReloading   = true;
  tmrConfig.bSyncBypass  = false; //TODO :??
  result = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmrConfig);
  *preescaler = ADI_TMR_PRESCALER_16;
  
  return result;
  
}

/* ************************************************************************* */
/**
    * @brief    HFOSC TIMER clock cal. This function is used to calibrate internal HFOSC
    * @param    None
    * @retval   None
    */
ADI_TMR_RESULT HAL_TIM_HfoscClockCalInit() {
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  ADI_TMR_CONFIG        tmrConfig;
  
  // enable the use DWT
    *DEMCR = *DEMCR | 0x01000000;

// Reset cycle counter
    *DWT_CYCCNT = 0; 

// enable cycle counter
    *DWT_CONTROL = *DWT_CONTROL | 1 ; 

  /*result = adi_tmr_Open(1, aDeviceMemory1, ADI_TMR_MEMORY_SIZE,
                        &hDeviceTim);*/
  result = adi_tmr_Init(ADI_TMR_DEVICE_GP1, HfoscClockCalCallback, NULL, true);
  
  /*if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_RegisterCallback(hDeviceTim,HfoscClockCalCallback, hDeviceTim);
  
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetPrescaler(hDeviceTim, ADI_GPT_PRESCALER_1);
  
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetClockSource(hDeviceTim, ADI_TMR_CLOCK_LFXTL);
  
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetRunMode(hDeviceTim, ADI_TMR_PERIODIC_MODE);
      
  if (ADI_TMR_SUCCESS == result)
    result = adi_tmr_SetCountMode(hDeviceTim, ADI_TMR_COUNT_DOWN);
  
  adi_tmr_SetLoadValue(hDeviceTim, 2048);
  initialHfoscCycles = *DWT_CYCCNT;
  adi_tmr_Enable(hDeviceTim, true);*/
  tmrConfig.bCountingUp  = false;
  tmrConfig.bPeriodic    = true;
  tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_1;
  tmrConfig.eClockSource = ADI_TMR_CLOCK_LFXTAL;
  tmrConfig.nLoad        = 2048; //TODO :??
  tmrConfig.nAsyncLoad   = 0; //TODO :??
  tmrConfig.bReloading   = true;
  tmrConfig.bSyncBypass  = false; //TODO :??
  result = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmrConfig);

  initialHfoscCycles = *DWT_CYCCNT;
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP1, true);
    
  return result;

}

ADI_TMR_RESULT HAL_TIM_AD7689SetTimerValue(uint16_t timerValue){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  ADI_TMR_CONFIG        tmrConfig;
  
  //adi_tmr_SetLoadValue(hDeviceAD7689Tim, timerValue);
  tmrConfig.bCountingUp  = false;
  tmrConfig.bPeriodic    = true;
  tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_16;
  tmrConfig.eClockSource = ADI_TMR_CLOCK_HFOSC;
  tmrConfig.nLoad        = timerValue; //TODO :??
  tmrConfig.nAsyncLoad   = 0; //TODO :??
  tmrConfig.bReloading   = true;
  tmrConfig.bSyncBypass  = false; //TODO :??
  result = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmrConfig);
  
  return result;
}

ADI_TMR_RESULT HAL_TIM_AD7689SetTimerON(){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  //adi_tmr_Enable(hDeviceAD7689Tim, true);
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP1, true);
  
  //AD7689timerON = 1;
  
  
  return result;
}
  
ADI_TMR_RESULT HAL_TIM_AD7689SetTimerOFF(){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  //adi_tmr_Enable(hDeviceAD7689Tim, false);
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP1, false);
  
  //AD7689timerON = 0;
  
  return result;
}

/**
 * @brief  Callback for GPT-1. Set the boolean flag to corresponding to the the event occured.
 *
 * @return none
 *
 */
static void HfoscClockCalCallback(void *pCBParam, uint32_t Event, void *pArg) {
  switch (Event) {
     case ADI_TMR_EVENT_TIMEOUT:
        finalHfoscCycles = *DWT_CYCCNT - initialHfoscCycles;
        //adi_tmr_Enable(hDeviceTim, false);
       // adi_tmr_Close(hDeviceTim);
        adi_tmr_Enable(ADI_TMR_DEVICE_GP1, false);
        break;

     default:
        break;
  }
}

uint8_t HAL_TIM_GetAD7689Lock(uint16_t lockValue){
  
  return 0;
}


//static ADI_TMR_HANDLE hDeviceLowTouchTim;
static ADI_TMR_CONFIG        gsTmrConfig;
/* ************************************************************************* */
/**
    * @brief    Low Touch TIMER initialization. This function initiaises the timer for low touch detection
    * @param    None
    * @retval   None
    */
ADI_TMR_RESULT HAL_TIM_LowTouchTimerInit(uint32_t *clkFrequency,uint8_t *preescaler,ADI_CALLBACK const pfCallback) {
    ADI_TMR_RESULT        eResult;
    
    /* Set up GP0 callback function */
    eResult = adi_tmr_Init(ADI_TMR_DEVICE_GP0, pfCallback, NULL, true);    

    /* Configure GP0 to have a period of 10 ms */
    gsTmrConfig.bCountingUp  = false;
    gsTmrConfig.bPeriodic    = true;
        
    *preescaler = ADI_TMR_PRESCALER_256;
    *clkFrequency = 32768;//ADI_TMR_CLOCK_LFXTAL;
    gsTmrConfig.ePrescaler   = ADI_TMR_PRESCALER_256;
    gsTmrConfig.eClockSource = ADI_TMR_CLOCK_LFXTAL;
    
    gsTmrConfig.nLoad        = 0;
    gsTmrConfig.nAsyncLoad   = 0;
    gsTmrConfig.bReloading   = false;
    gsTmrConfig.bSyncBypass  = false;     
//    
//    eResult = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP0, gsTmrConfig);  
    return eResult;

}


ADI_TMR_RESULT HAL_TIM_LowTouchSetTimerValue(uint16_t timerValue){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  gsTmrConfig.nLoad        = timerValue;
  gsTmrConfig.nAsyncLoad   = timerValue;  
  
  result = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP0, gsTmrConfig);
  
  return result;
}

ADI_TMR_RESULT HAL_TIM_LowTouchSetTimerON(){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP0, true);
  
//  LowTouchTimerON = 1;
  
  
  return result;
}
  
ADI_TMR_RESULT HAL_TIM_LowTouchSetTimerOFF(){
  
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP0, false);
  
//  LowTouchTimerON = 0;
  
  return result;
}
void TIM_Init(void) {
  TIM1_Init();
}

/*
GP1 timer init function for 10 msec period

*/
static  ADI_TMR_RESULT TIM1_Init(void)
{
    ADI_TMR_CONFIG        tmrConfig;
    ADI_TMR_RESULT        eResult = ADI_TMR_SUCCESS;
   eResult = adi_tmr_Init(ADI_TMR_DEVICE_GP1, GP1CallbackFunction, NULL, true);
   
   if (eResult == ADI_TMR_SUCCESS)
    {
     /* Configure GP1 to have a period of 1 sec */
        tmrConfig.bCountingUp  = false;
        tmrConfig.bPeriodic    = true;
        tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_64;
        tmrConfig.eClockSource = ADI_TMR_CLOCK_LFOSC;
        tmrConfig.nLoad        = GP1_LOAD_VALUE_FOR_1S_PERIOD * TEMPERATURE_SAMPLING_PERIOD; 
        tmrConfig.nAsyncLoad   = GP1_LOAD_VALUE_FOR_1S_PERIOD * TEMPERATURE_SAMPLING_PERIOD;
        tmrConfig.bReloading   = false;
        tmrConfig.bSyncBypass  = false;                        
       eResult = adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmrConfig);
      }
   return eResult;
}

/*
**GP1call back function will call for every 1sec
*/
void GP1CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg)
{
  if((Event & ADI_TMR_EVENT_TIMEOUT) == ADI_TMR_EVENT_TIMEOUT) {
     Timer_Flag = 1;
  } 
}
/**
    * @brief    1-Enable Timer,0-disable
    * @param    None
    * @retval   None
    */
ADI_TMR_RESULT TIM1Enable(bool state) {
  ADI_TMR_RESULT result = ADI_TMR_SUCCESS;
  result = adi_tmr_Enable(ADI_TMR_DEVICE_GP1, state);
  return result;
}