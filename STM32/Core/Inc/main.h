/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EN_GND0_Pin GPIO_PIN_4
#define EN_GND0_GPIO_Port GPIOA
#define EN_GND1_Pin GPIO_PIN_5
#define EN_GND1_GPIO_Port GPIOA
#define EN_GND2_Pin GPIO_PIN_6
#define EN_GND2_GPIO_Port GPIOA
#define EN_GND3_Pin GPIO_PIN_7
#define EN_GND3_GPIO_Port GPIOA
#define EN_GND4_Pin GPIO_PIN_4
#define EN_GND4_GPIO_Port GPIOC
#define EN_GND5_Pin GPIO_PIN_5
#define EN_GND5_GPIO_Port GPIOC
#define EN_GND6_Pin GPIO_PIN_0
#define EN_GND6_GPIO_Port GPIOB
#define EN_GND7_Pin GPIO_PIN_1
#define EN_GND7_GPIO_Port GPIOB
#define EN_GND8_Pin GPIO_PIN_2
#define EN_GND8_GPIO_Port GPIOB
#define EN_GND9_Pin GPIO_PIN_10
#define EN_GND9_GPIO_Port GPIOB
#define SDA2_Pin GPIO_PIN_15
#define SDA2_GPIO_Port GPIOB
#define SCL2_Pin GPIO_PIN_6
#define SCL2_GPIO_Port GPIOC
#define SDA1_Pin GPIO_PIN_7
#define SDA1_GPIO_Port GPIOC
#define SCL1_Pin GPIO_PIN_8
#define SCL1_GPIO_Port GPIOC
#define EN_VCC9_Pin GPIO_PIN_15
#define EN_VCC9_GPIO_Port GPIOA
#define EN_VCC8_Pin GPIO_PIN_10
#define EN_VCC8_GPIO_Port GPIOC
#define EN_VCC7_Pin GPIO_PIN_11
#define EN_VCC7_GPIO_Port GPIOC
#define EN_VCC6_Pin GPIO_PIN_12
#define EN_VCC6_GPIO_Port GPIOC
#define EN_VCC5_Pin GPIO_PIN_2
#define EN_VCC5_GPIO_Port GPIOD
#define EN_VCC4_Pin GPIO_PIN_3
#define EN_VCC4_GPIO_Port GPIOB
#define EN_VCC3_Pin GPIO_PIN_4
#define EN_VCC3_GPIO_Port GPIOB
#define EN_VCC2_Pin GPIO_PIN_5
#define EN_VCC2_GPIO_Port GPIOB
#define EN_VCC1_Pin GPIO_PIN_6
#define EN_VCC1_GPIO_Port GPIOB
#define EN_VCC0_Pin GPIO_PIN_7
#define EN_VCC0_GPIO_Port GPIOB
#define SystemLed_Pin GPIO_PIN_9
#define SystemLed_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
