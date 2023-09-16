/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
            朗讯实训箱上开发板的按键
						S1  PC0     
						S2  PC1
						S3  PC2
						S4  PC3
						
						全部上拉了4.7K电阻 并且按下后低电平
						                    *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/


#include "radio_key.h"

void radioKey_GPIOInt()
{
	    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();


    //按键输入 S1
    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = K_S1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(K_S1_GPIO_GPIO_Port, &GPIO_InitStruct);
	
	//按键中断 S2 S3 S4
    /*Configure GPIO pins : PEPin PEPin */
    GPIO_InitStruct.Pin = K_S2_Pin|K_S3_Pin|K_S4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
    
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}



/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI1_IRQHandler(void)
{
    /* USER CODE END EXTI0_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
    /* USER CODE BEGIN EXTI0_IRQn 1 */

    /* USER CODE END EXTI0_IRQn 1 */
}
/**
* @brief This function handles EXTI line4 interrupt.
*/
void EXTI2_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_IRQn 0 */
   
    /* USER CODE END EXTI4_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    /* USER CODE BEGIN EXTI4_IRQn 1 */

    /* USER CODE END EXTI4_IRQn 1 */
}


/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI3_IRQn 0 */

    /* USER CODE END EXTI3_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
    /* USER CODE BEGIN EXTI3_IRQn 1 */

    /* USER CODE END EXTI3_IRQn 1 */
}

