/* Includes -----------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "system_ADuCM4050.h"
#include "HwIfConfig.h"
#include <drivers/gpio/adi_gpio.h>
#include <drivers/beep/adi_beep.h>
#include <common.h>

/* Private typedef ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Basic functionality */
#define PLAY_NOTE     1
#define PLAY_ALARM    1
#define PLAY_TUNE     1

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* required beeper driver memory */
uint8_t BeepMemory[ADI_BEEP_MEMORY_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* Device handle */
ADI_BEEP_HANDLE hBeep = NULL;

//extern int32_t   adi_initpinmux (void);
extern void             beepCallback(void *pCBUnused, uint32_t Event, void *pvUnused);

/* ************************************************************************* */
/**
    * @brief    BEEP initialization. This function initiaises all the BEEP used.
    * @param    None
    * @retval   None
    */
ADI_BEEP_RESULT BEEP_Init(void) {
  if (ADI_BEEP_SUCCESS != (adi_beep_Open(ADI_BEEP_DEVID_0,
                                                     &BeepMemory,
                                                     sizeof(BeepMemory),
                                                     &hBeep)))
    {
      DEBUG_MESSAGE("adi_beep_Open failed\n");
    }
    
//    if (ADI_BEEP_SUCCESS != (adi_beep_RegisterCallback(hBeep,
//                                                                 beepCallback,
//                                                                 NULL)))
//    {
//      DEBUG_MESSAGE("adi_beep_RegisterCallback failed\n");
//    }
  return ADI_BEEP_SUCCESS;
}

/* ************************************************************************* */
/**
    * @brief    BEEP De-initialization. This function close the BEEP used.
    * @param    None
    * @retval   None
    */
ADI_BEEP_RESULT BEEP_De_Init(void) {
  if (ADI_BEEP_SUCCESS != (adi_beep_Close(hBeep)))
    {
      DEBUG_MESSAGE("adi_beep_Close failed\n");
    }
  return ADI_BEEP_SUCCESS;
}

/* the playlist */
#if PLAY_TUNE == 1
ADI_BEEP_NOTE playList1[] = {
    /* Close Encounters: D-E-C-C(octave lower)-G */
    { ADI_BEEP_FREQ_G5,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_C6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_C6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_E6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_A6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_E6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_G6,        ADI_BEEP_DUR_16_32 },
    { ADI_BEEP_FREQ_G6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_A6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_G6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_E6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_F6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_E6,        ADI_BEEP_DUR_8_32 },
    { ADI_BEEP_FREQ_D6,        ADI_BEEP_DUR_16_32 },    
};
uint8_t playLength1 = sizeof(playList1)/sizeof(playList1[0]);
#endif

/* Boolean flag indicating callback success */
volatile bool bNoteDone     = false;
volatile bool bSequenceDone = false;

/* Used for a delay loop */
volatile uint32_t cnt = 0;


/* Optional application callback */
void beepCallback(void *pCBUnused, uint32_t Event, void *pvUnused) {

    if(Event & (ADI_BEEP_INTERRUPT_NOTE_END)) {  
       bNoteDone = true;
    }

    if(Event & ADI_BEEP_INTERRUPT_SEQUENCE_END) { 
       bSequenceDone = true;
    }
 }


#if PLAY_NOTE == 1
ADI_BEEP_RESULT PlayOne(void)
{
  ADI_BEEP_RESULT eResult;
  ADI_BEEP_NOTE note = {ADI_BEEP_FREQ_C6, ADI_BEEP_DUR_8_32};
  
  bNoteDone     = false;
  bSequenceDone = false;
  
  
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_PlayNote(hBeep, note)))
  {
    DEBUG_MESSAGE("PlayOne: adi_beep_PlayNote failed\n");
    return eResult;
  }
  
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_Wait(hBeep)))
  {
    DEBUG_MESSAGE("PlayOne: adi_beep_Wait failed\n");
    return eResult;
  }
  
  
//  /* ... and check if we got the right callback */
//  if ((bNoteDone != true) || (bSequenceDone != false))
//  {
//    DEBUG_MESSAGE("PlayOne: Note callback failed \n");
//    return eResult;
//  }
  
  return 0;//eResult;
}
#endif

#if PLAY_ALARM == 1
ADI_BEEP_RESULT PlayAlarm(void)
{
  ADI_BEEP_RESULT eResult;
  ADI_BEEP_NOTE note1, note2;
  
  /* Play the following two notes, 5 times */
  uint8_t count   = 5u;
  note1.frequency = ADI_BEEP_FREQ_A4;     /* Low... */
  note1.duration  = ADI_BEEP_DUR_2_32;
  
  note2.frequency = ADI_BEEP_FREQ_A7;     /*... then high */
  note2.duration  = ADI_BEEP_DUR_2_32;
  
  /* Reset the callback checks */
  bNoteDone     = false;
  bSequenceDone = false;
  
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_PlayTwoTone(hBeep, note1, note2, count)))
  {
    DEBUG_MESSAGE("PlayAlarm: adi_beep_PlayTwoTone failed\n");
    return eResult;
  }
  
  /* Wait until the sequence is complete. */
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_Wait(hBeep)))
  {
    DEBUG_MESSAGE("PlayAlarm: adi_beep_Wait failed\n");
    return eResult;
  }
  
//  /* ... and check if we got the right callbacks */
//  if ((bNoteDone != false) || (bSequenceDone != true))
//  {
//    DEBUG_MESSAGE("PlayAlarm: Twotone callback failed \n");
//    return eResult;
//  }        
  return eResult;
}
#endif 

#if PLAY_TUNE == 1
#if ADI_BEEP_INCLUDE_PLAY_SEQUENCE != 1
#error To use the "adi_beep_PlaySequence()" API, the ADI_BEEP_INCLUDE_PLAY_SEQUENCE macro must be set to 1 in the adi_beep_config.h file.
#else 
ADI_BEEP_RESULT PlayTune(void)
{
  ADI_BEEP_RESULT eResult;
  
  bNoteDone     = false;
  bSequenceDone = false;
  
  /* Play the tune */
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_PlaySequence(hBeep, playList1, playLength1)))
  {
    DEBUG_MESSAGE("PlayTune: adi_beep_PlaySequence failed\n");
    return eResult;
  }
  
  /* wait for the sequence to complete */
  if (ADI_BEEP_SUCCESS != (eResult = adi_beep_Wait(hBeep)))
  {
    DEBUG_MESSAGE("PlayTune: adi_beep_Wait failed\n");
    return eResult;
  }
  
//  /* ...and check that we got the right callbacks */
//  if ((bNoteDone != false) || (bSequenceDone != true))
//  {
//    DEBUG_MESSAGE("PlayTune: Sequence callback failed \n");
//    return eResult;
//  }        
  
  return eResult;
}
#endif
#endif