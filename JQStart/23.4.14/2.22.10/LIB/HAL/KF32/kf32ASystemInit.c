/******************************************************************************

 *                  Shanghai ChipON Micro-Electronic Co.,Ltd
 ******************************************************************************

 *  @File Name       :system_init.c

 *  @Syntax          :gnu99

 *  @Author          :ChipON AE/FAE Group

 *  @Date            :2021-08-09

 *  @Version         :V3.0.0

 *  @Description     :This file contains the system clock configuration for KF32Axx devices
 ******************************************************************************

 *  Copyright (C) by Shanghai ChipON Micro-Electronic Co.,Ltd

 *  All rights reserved.

 *  This software is copyright protected and proprietary to

 *  Shanghai ChipON Micro-Electronic Co.,Ltd.
 *****************************************************************************/

/** @addtogroup System_Init_Private_Includes
  * @{
  */
#include "kf32aSystemInit.h"

/** @addtogroup System_Init_Private_Functions
  * @{
  */
static void SetSysClock(void);

/** @addtogroup System_Init_Private_Defines
  * @{
  */
#define INTHF_FREQ_16MHz	   	16000000
#define INTLF_FREQ_32KHz	   	32000
#ifdef SYSCLK_FREQ_HSE
#define EXTHF_FREQ_xMHz			SYSCLK_FREQ_HSE
#else
#define EXTHF_FREQ_xMHz			160000000
#endif
#define EXTLF_FREQ_32768Hz	   	32768
#define LP4M_FREQ_4MHz		   	4000000

/** @addtogroup System_Init_Private_Variables
  * @{
  */
 #ifdef SYSCLK_FREQ_48MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_48MHz;
#elif defined SYSCLK_FREQ_64MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_64MHz;
#elif defined SYSCLK_FREQ_72MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_72MHz;
#elif defined SYSCLK_FREQ_96MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_96MHz;
#elif defined SYSCLK_FREQ_120MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_120MHz;
#endif
static uint8_t 	fac_us;
static uint16_t fac_ms;

/**
 * @brief In the following line adjust the Internal High Speed oscillator (HSI) Startup
   Timeout value at least 200us default.
   */
#if !defined  (HSI_STARTUP_TIMEOUT)
  #define HSI_STARTUP_TIMEOUT    	((uint16_t)0x0070)   /*!< Time out for HSI start up */
#endif /* HSI_STARTUP_TIMEOUT */

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value at least 4ms default.
   */
#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT		((uint16_t)0xA39)   /*!< Time out for HSE start up */
#endif /* HSE_STARTUP_TIMEOUT */

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) fast
   startup Timeout value at least 200us default.In fast startup mode, the OSCIN pin should
   be temporarily configured  the output mode to ground, then it should be configured
   the analog mode.
   */
#if !defined  (HSE_FASTSTARTUP_TIMEOUT)
  #define HSE_FASTSTARTUP_TIMEOUT	((uint16_t)0x0070)   /*!< Time out for HSE fast start up mode */
#endif /* HSE_FASTSTARTUP_TIMEOUT */

/**
 * @brief In the following line adjust the PLL Startup Timeout value at least 800us default.
   */
#if !defined  (PLL_STARTUP_TIMEOUT)
  #define PLL_STARTUP_TIMEOUT    ((uint16_t)0x0400)   /*!< Time out for PLL startup */
#endif /* PLL_STARTUP_TIMEOUT */

/**
 *  @brief : Configure the System clock source,PLL Multiplier and Divider factors.
 * 			 If an external High Speed oscillator (HSE) is use, it would be the PLL
 * 			 clock source,otherwise the PLL clock source is HSI.Choose INTHF or EXTHF
 * 			 by define SYSCLK_FREQ_HSI or SYSCLK_FREQ_HSE in "system_init.h".
 *  @param : None
 *  @retval :None
 */
static void SetSysClock(void)
{
	volatile uint32_t StartUpCounter;
/**
 * ******************************************************************************
 * 	Enable INTHF & INTLF,and set the INTHF as the SCLK.
 * 	Wait till INTHF is ready and if Time out is reached exit.
 * ******************************************************************************
 */
	OSC_INTLF_Software_Enable(TRUE);
	OSC_INTHF_Software_Enable(TRUE);
	FLASH_CFG = 0xC7;
	/* Set the INTHF as the SCLK DIV<1:1> */
	OSC_SCK_Division_Config(SCLK_DIVISION_1); //主时钟分频选择
	OSC_SCK_Source_Config(SCLK_SOURCE_INTHF); //选择内部高频作为系统时钟
	StartUpCounter = HSI_STARTUP_TIMEOUT;
	while (--StartUpCounter);
	while (OSC_Get_INTHF_INT_Flag() != SET);
/**
 * ******************************************************************************
 * Configure the PLL clock source is HES or HEI by define SYSCLK_FREQ_HSI or SYSCLK_FREQ_HSE
 * in "system_init.h".
 * If the PLL clock source is HSE(SYSCLK_FREQ_HSE shoudld be defined), its should
 * choose the HSE pins.
 * If an external clock source is used, must be wait till EXTHF is ready and if
 * Time out is reached exit.
 * ******************************************************************************
 */
#ifdef SYSCLK_FREQ_HSI
	OSC_PLL_Input_Source_Config(PLL_INPUT_INTHF);
#else
	/* Config the BKP area is enable, BKP register and data can be write and read */
	SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
	/* If an external clock frequency is more than 20M, should be set the HSFSEN bit
	   in OSC_HFOSCCAL1 register */
	#if (SYSCLK_FREQ_HSE < 20000000)
	{
		SFR_SET_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HLSW_POS);
	}
	#endif
	/* Choose the HSE pins by define CHOOSE_EXTHF_PIN_PD14_PD15 in fast startup mode default */
	#ifdef CHOOSE_EXTHF_PIN_PD14_PD15
		GPIO_Write_Mode_Bits(GPIOD_SFR, GPIO_PIN_MASK_14, GPIO_MODE_OUT);
		GPIO_Set_Output_Data_Bits(GPIOD_SFR, GPIO_PIN_MASK_14, Bit_RESET);
		StartUpCounter = HSE_FASTSTARTUP_TIMEOUT;
		while (--StartUpCounter);
		GPIO_Write_Mode_Bits(GPIOD_SFR, GPIO_PIN_MASK_14, GPIO_MODE_AN);
		PM_EXTHF_PIN_Selection_Config(CHOOSE_EXTHF_PIN_PD14_PD15);
	#elif defined CHOOSE_EXTHF_PIN_PD9_PD10
		GPIO_Write_Mode_Bits(GPIOD_SFR, GPIO_PIN_MASK_9, GPIO_MODE_OUT);
		GPIO_Set_Output_Data_Bits(GPIOD_SFR, GPIO_PIN_MASK_9, Bit_RESET);
		StartUpCounter = HSE_FASTSTARTUP_TIMEOUT;
		while (--StartUpCounter);
		GPIO_Write_Mode_Bits(GPIOD_SFR, GPIO_PIN_MASK_9, GPIO_MODE_AN);
		PM_EXTHF_PIN_Selection_Config(CHOOSE_EXTHF_PIN_PD9_PD10);
	#endif
	/* Must be wait till EXTHF is ready and if Time out is reached exit */
	OSC_EXTHF_Software_Enable(TRUE);
	OSC_EXTHF_Start_Delay_Config(EXT_START_DELAY_256);
	StartUpCounter = HSE_STARTUP_TIMEOUT;
	while (StartUpCounter--);
	while(OSC_Get_EXTHF_INT_Flag() != SET);
	OSC_SCK_Source_Config(SCLK_SOURCE_EXTHF);
	OSC_PLL_Input_Source_Config(PLL_INPUT_EXTHF);
#endif
/**
 * ******************************************************************************
 * Configure the System clock source,PLL Multiplier and Divider factors,must be
 * wait till PLL is ready and if Time out is reached exit.
 * Enable HFCLK and if EXTHF is invalid configure the HFCLK default clock source is HSI.
 * Enable LFCLK and if configure the LFCLK default clock source is LSI.
 * ******************************************************************************
 */
#ifdef SYSCLK_FREQ_48MHz
	OSC_PLL_Multiple_Value_Select(48, 2, 8);
#elif defined SYSCLK_FREQ_64MHz
	OSC_PLL_Multiple_Value_Select(32,2,4);
#elif defined SYSCLK_FREQ_72MHz
	OSC_PLL_Multiple_Value_Select(36, 2, 4);
#elif defined SYSCLK_FREQ_96MHz
	OSC_PLL_Multiple_Value_Select(12, 1, 2);
#elif defined SYSCLK_FREQ_120MHz
	OSC_PLL_Multiple_Value_Select(30, 2, 2);
#endif
#ifdef SYSCLK_FREQ_HSI
	OSC_PLL_Start_Delay_Config(PLL_START_DELAY_8192);
#else
	#if (SYSCLK_FREQ_HSE <= 8000000)
		OSC_PLL_Start_Delay_Config(PLL_START_DELAY_4096);
	#elif (SYSCLK_FREQ_HSE <= 16000000)
		OSC_PLL_Start_Delay_Config(PLL_START_DELAY_8192);
	#else
		OSC_PLL_Start_Delay_Config(PLL_START_DELAY_16384);
	#endif
#endif
	OSC_PLL_Software_Enable(TRUE);
	OSC_PLL_RST();
	while (OSC_Get_PLL_INT_Flag() != SET);
#ifdef SYSCLK_FREQ_48MHz
	FLASH_CFG = 0xC2;
#elif defined SYSCLK_FREQ_64MHz
	FLASH_CFG = 0xC3;
#elif defined SYSCLK_FREQ_72MHz
	FLASH_CFG = 0xC3;
#elif defined SYSCLK_FREQ_96MHz
	FLASH_CFG = 0xC3;
#elif defined SYSCLK_FREQ_120MHz
	FLASH_CFG = 0xC4;
#endif
	/* Configure the System clock source is PLL */
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_PLL);
	/* Enable HFCLK and if EXTHF is invalid configure the HFCLK default clock source is HSI DIV<1:1> */
	OSC_HFCK_Division_Config(HFCK_DIVISION_1);
#ifdef SYSCLK_FREQ_HSI
	OSC_HFCK_Source_Config(HFCK_SOURCE_INTHF);
#else
	OSC_HFCK_Source_Config(HFCK_SOURCE_EXTHF);
#endif
	OSC_HFCK_Enable(TRUE);
	/* Enable LFCLK and if configure the LFCLK default clock source is LSI DIV<1:1> */
	OSC_LFCK_Division_Config(LFCK_DIVISION_1);
	OSC_LFCK_Source_Config(LFCK_INPUT_INTLF);
	OSC_LFCK_Enable(TRUE);
}

/**
 *  @brief : Setup the microcontroller system.
 * 			 Initialize PLL, HFCLK, LFCLK, and the PLL and update the systemFrequency variable.
 *  @param : None
 *  @retval :None
 */
void SystemInit(void)
{
	/* Configure the System clock source, PLL Multiplier and Divider factors */
	SetSysClock();
}

/**
 *  @brief : Initialize the systick timer for delay function without interrupt.
 * 			 Must be Initialized before use "systick_delay_us" and "systick_delay_ms".
 *  @param : None
 *  @retval :None
 */
void systick_delay_init(void)
{
	SYSTICK_Cmd(FALSE);
	SYSTICK_Clock_Config(SYSTICK_SYS_CLOCK_DIV_2);
	SFR_CLR_BIT_ASM(ST_CALI, ST_CALII_STCLKREF_POS);
	fac_us = SystemCoreClock / 2000000;
	fac_ms = fac_us * 1000;
}

/**
 *  @brief : Using SysTick timer to realize microsecond delay
 * 			 nus<=0xffffff*1000*1000/SYSCLK/2
 *  @param : nus
 * 			 In the following line describe the max time at different frequencies
 * 			 48M： 699050us
 * 			 72M： 466033us
 * 			 96M： 349525us
 * 			 120M：279620us
 *  @retval :None
 */
void systick_delay_us(unsigned int nus)
{
	uint32_t temp;
	SYSTICK_SFR->RELOAD = fac_us * nus;
	SYSTICK_SFR->CV = 0;
	SYSTICK_SFR->CTL |= ST_CTL_STEN;
	do
	{
		temp = ST_CTL;
	} while ((temp & 0x01) && !(temp & (1 << 16)));
	SYSTICK_SFR->CTL &= ~ST_CTL_STEN;
	SYSTICK_SFR->CV = 0;
}

/**
 *  @brief : Using SysTick timer to realize millisecond delay
 * 			 nus<=0xffffff*1000/SYSCLK/2
 *  @param : nus
 * 			 In the following line describe the max time at different frequencies
 * 			 48M： 699ms
 * 			 72M： 466ms
 * 			 96M： 349ms
 * 			 120M：279ms
 *  @retval :None
 */
void systick_delay_ms(unsigned int nms)
{
	uint32_t temp;
	SYSTICK_SFR->RELOAD = (uint32_t)nms * fac_ms;
	SYSTICK_SFR->CV = 0;
	SYSTICK_SFR->CTL |= ST_CTL_STEN;
	do
	{
		temp = ST_CTL;
	} while ((temp & 0x01) && !(temp & (1 << 16)));
	SYSTICK_SFR->CTL &= ~ST_CTL_STEN;
	SYSTICK_SFR->CV = 0;
}

/**
 *  @brief : Update SystemCoreClock variable according to Clock Register Values.
 * 			 The SystemCoreClock variable contains the core clock (SCLK), it can
 *           be used by the user application to setup the SysTick timer or configure
 *           other parameters.
 *  @param : None
 *  @retval :None
 */
void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0, pll_n = 1, pll_m = 0, pll_no = 1;
	uint32_t sckdivfactor = 1;
	uint32_t pllsource = INTHF_FREQ_16MHz;
	tmp = ((OSC_CTL0 >> OSC_CTL0_SCKS0_POS) & 0x07);
	switch (tmp) {
		case SCLK_SOURCE_INTHF:
			SystemCoreClock = INTHF_FREQ_16MHz;
			break;
		case SCLK_SOURCE_INTLF:
			SystemCoreClock = INTLF_FREQ_32KHz;
			break;
		case SCLK_SOURCE_EXTHF:
			SystemCoreClock = EXTHF_FREQ_xMHz;
			break;
		case SCLK_SOURCE_EXTLF:
			SystemCoreClock = EXTLF_FREQ_32768Hz;
			break;
		case SCLK_SOURCE_LP4M:
			SystemCoreClock = LP4M_FREQ_4MHz;
			break;
		case SCLK_SOURCE_PLL:
			if ((OSC_CTL0 >> OSC_CTL0_PLL0CKS_POS) & 0x01) {
				pllsource = EXTHF_FREQ_xMHz;
			} else {
				pllsource = INTHF_FREQ_16MHz;
			}
			pll_m = ((PLL_CTL >> PLL_CTL_M0_POS) & 0x3fff);
			pll_n = ((PLL_CTL >> PLL_CTL_N0_POS) & 0x0f);
			pll_no = (0x01 << ((PLL_CTL >> PLL_CTL_OD0_POS) & 0x03));
			sckdivfactor = (0x01 << ((OSC_CTL0 >> OSC_CTL0_SCKDIV0_POS) & 0x07));
			SystemCoreClock = pllsource * pll_m / (pll_n * pll_no * sckdivfactor);
			break;
		default:
			SystemCoreClock = INTHF_FREQ_16MHz;
			break;
	}
}
