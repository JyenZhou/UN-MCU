#ifndef __74HC595_H__
#define __74HC595_H__
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif


#define DigitalTube_74HC595__GPIO_DIO_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DigitalTube_74HC595__GPIO_SCLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DigitalTube_74HC595__GPIO_RCLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
//������������
#define   DigitalTube_74HC595_DIO_GPIO_Port DIG_LED_SDI_GPIO_Port
#define   DigitalTube_74HC595_DIO_GPIO_Pin DIG_LED_SDI_Pin
//ʱ�������źš�����������Ч
#define    DigitalTube_74HC595_SCLK_GPIO_Port    DIG_LED_SCLK_GPIO_Port
#define   DigitalTube_74HC595_SCLK_GPIO_Pin    DIG_LED_SCLK_Pin
//�����źš���������������Ч
#define   DigitalTube_74HC595_RCLK_GPIO_Port DIG_LED_LOAD_GPIO_Port
#define   DigitalTube_74HC595_RCLK_GPIO_Pin DIG_LED_LOAD_Pin



void DigitalTube_74HC595_test (void) ;
void S4_74HC595_init (void);
void LED4_Display (void);			// LED��ʾ
void LED3_Display(uint8 bit, uint8 value);















#endif
