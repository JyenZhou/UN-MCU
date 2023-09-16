#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define RTC_DEVICE_NUM   1

/* Private variables ---------------------------------------------------------*/
static uint8_t aRtcDevMem[ADI_RTC_MEMORY_SIZE]; // Device memrory to operate the RTC device
ADI_RTC_HANDLE  hDevRTC;        // RTC device handle
static uint64_t gRtcTick=0;
static uint8_t gnRtcPrescaler = 5;
//static uint8_t gsDateTimeUpdateFlag = 0;

/* Private function prototypes -----------------------------------------------*/

/**
  *@brief       RTC Initialization
  *@param       None.
  *@return      None.
 */
void RTC_Init(void)
{
  adi_rtc_Open(RTC_DEVICE_NUM, aRtcDevMem, ADI_RTC_MEMORY_SIZE, &hDevRTC);
  adi_rtc_SetPreScale(hDevRTC, gnRtcPrescaler);
//  adi_rtc_RegisterCallback(hDevRTC, RtcCallback, hDevRTC);
//  adi_rtc_EnableInterrupts(hDevRTC, ADI_RTC_COUNT_INT, true);
  adi_rtc_Enable(hDevRTC, true);
}

///*!
//  *@brief       RTC1 peripheral set timestamp count
//  *@param       ts_sec time in seconds.
//  *@return      None.
// */
//void HAL_RTC_ClockSetTS(uint32_t ts_sec)
//{
//  /* Set the RTC device count value */
//  adi_rtc_SetCount(hDevRTC, ts_sec);
//  gsDateTimeUpdateFlag = 1;      /*This function gets called only when updating the rtc to real date time*/      
//  return;
//}

/**
    * @brief This function returns ticks value.
    * @param    None
    * @retval   ticks value in 1/32 ms resolution
    */
uint32_t HAL_RTC_GetTick()
{
  uint32_t nRtcCount, nRtcFraction;
  uint64_t nAux;
  
  adi_rtc_GetCountRegs(hDevRTC, &nRtcCount, &nRtcFraction) ;
  nAux = ((uint64_t) nRtcCount) << gnRtcPrescaler;
  nAux += nRtcFraction;
  gRtcTick = gRtcTick+100;
  nAux = (gRtcTick * 1000) >> 10; // Time in one thirty two of miliseconds
  return  ((uint32_t) nAux);
}

/**
    * @brief This function returns ticks value.
    * @param    None
    * @retval   ticks value in 1/32 ms resolution
    */
uint32_t MCU_HAL_GetTick() { 
    return (uint32_t)HAL_RTC_GetTick();
}
