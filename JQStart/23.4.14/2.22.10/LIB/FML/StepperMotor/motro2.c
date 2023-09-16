/***********************
  */
	/*���ö�ʱ��2 �ıȽ����ģʽ�ķ�תת���
	��Ҫ���붨ʱ�������XXXtim.c XXXtim_ex.c�ļ�������
		��������Ҫ��ʼ�����Ƕ�ʱ�� 
	ʹ�ö�ʱ�� ����ͬʱ�궨������� HAL_TIM_MODULE_ENABLED ��Ϊ��ʱ���кü��� �޷�����*/
/* ---------------------------------------------------------------*/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "motro2.h"
#include <math.h>
#include  "tim.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
//TIM_HandleTypeDef htimx_STEPMOTOR;
speedRampData2 srd2               = {STOP2,CW2,0,0,0,0,0};         // �Ӽ������߱���
__IO int32_t  step_position2     = 0;           // ��ǰλ��
__IO uint8_t  MotionStatus2      = 0;           //�Ƿ����˶���0��ֹͣ��1���˶�
__IO uint8_t wait2;

__IO int32_t  OLED_SPeed_count2;
__IO uint8_t SP2=0;



TIM_HandleTypeDef htim2;
/* TIM2 init function */

static void DIR_GPIOInit(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
		    //����ת
    HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_SET);
	    //����ת                     PCPin PCPin */
    GPIO_InitStruct.Pin = DIR2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIR2_GPIO_Port, &GPIO_InitStruct);

}

 static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
if(timHandle->Instance==TIM2)
    {
        /* USER CODE BEGIN TIM2_MspPostInit 0 */
        /* USER CODE END TIM2_MspPostInit 0 */

        /**TIM2 GPIO Configuration
        PA3     ------> TIM2_CH4
        */
        GPIO_InitStruct.Pin = BJ2_Motor2_PUL2_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(BJ2_Motor2_PUL2_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM2_MspPostInit 1 */
        /* USER CODE END TIM2_MspPostInit 1 */
    }
   
}

void Motor2_MX_TIM2_Init(void)
{
	#ifndef StepperMotor2
δ��ʼ���������2
#endif
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;
    __HAL_RCC_TIM2_CLK_ENABLE();//	 STEPMOTOR_TIM_RCC_CLK_ENABLE2();//bj.h

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 31;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 500;////////////////////
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);//////
    TIM_CCxChannelCmd(TIM2, TIM_CHANNEL_4, TIM_CCx_DISABLE);//////
    /* ʹ�ܱȽ����ͨ�� */
    if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_TIM_MspPostInit(&htim2);

    HAL_TIM_Base_MspInit(&htim2);/*�ж����ȼ���ʹ�� */
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC4);
    /* ʹ�ܶ�ʱ���Ƚ���� */
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC4);
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(&htim2);
    HAL_TIM_Base_Start(&htim2);// ʹ�ܶ�ʱ��
 DIR_GPIOInit();
}



















/**
  * ��������: ���λ���˶����˶������Ĳ���
  * �������: step���ƶ��Ĳ��� (����Ϊ˳ʱ�룬����Ϊ��ʱ��).
              accel  ���ٶ�,ʵ��ֵΪaccel*0.1*rad/sec^2
              decel  ���ٶ�,ʵ��ֵΪdecel*0.1*rad/sec^2
              speed  ����ٶ�,ʵ��ֵΪspeed*0.1*rad/sec
  * �� �� ֵ: ��
  * ˵    ��: �Ը����Ĳ����ƶ�����������ȼ��ٵ�����ٶȣ�Ȼ���ں���λ�ÿ�ʼ
  *           ������ֹͣ��ʹ�������˶�����Ϊָ���Ĳ���������Ӽ��ٽ׶κ̲ܶ���
  *           �ٶȺ������ǻ�û�ﵽ����ٶȾ�Ҫ��ʼ����
  */
void STEPMOTOR_AxisMoveRel2(__IO int32_t step, __IO uint32_t accel, __IO uint32_t decel, __IO uint32_t speed)
{
    __IO uint16_t tim_count2;
    // �ﵽ����ٶ�ʱ�Ĳ���
    __IO uint32_t max_s_lim2;
    // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
    __IO uint32_t accel_lim2;
    wait2=0;
    SP2=0;
    if(step < 0) // ����Ϊ����
    {

        srd2.dir = CCW2; // ��ʱ�뷽����ת
        STEPMOTOR_DIR_REVERSAL2();
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_RESET);
        step =-step;   // ��ȡ��������ֵ
        SP2=1;
    }
    else
    {
        srd2.dir = CW2; // ˳ʱ�뷽����ת
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_SET);//1
        STEPMOTOR_DIR_FORWARD2();
        SP2=2;
    }

    if(step == 1)    // ����Ϊ1
    {
        srd2.accel_count = -1;   // ֻ�ƶ�һ��
        srd2.run_state = DECEL2;  // ����״̬.
        srd2.step_delay = 1000;	// ����ʱ
    }
    else if(step != 0)  // ���Ŀ���˶�������Ϊ0
    {
        // ���ǵ��������û��ֲ�����ϸ�ļ��㼰�Ƶ�����

        // ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
        // min_delay = (alpha / tt)/ w
        srd2.min_delay = (int32_t)(A_T_x102/speed);

        // ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
        srd2.step_delay = (int32_t)((T1_FREQ_1482 * sqrt(A_SQ2 / accel))/10);

        // ������ٲ�֮��ﵽ����ٶȵ�����
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim2 = (uint32_t)(speed*speed/(A_x2002*accel/10));
        // ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
        // ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
        if(max_s_lim2 == 0)
        {
            max_s_lim2 = 1;
        }

        // ������ٲ�֮�����Ǳ��뿪ʼ����
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim2 = (uint32_t)(step*decel/(accel+decel));
        // ���Ǳ����������1�����ܲ��ܿ�ʼ����.
        if(accel_lim2 == 0)
        {
            accel_lim2 = 1;
        }

        // ʹ�������������ǿ��Լ�������ٽ׶β���
        if(accel_lim2 <= max_s_lim2)
        {
            srd2.decel_val = accel_lim2 - step;
        }
        else
        {
            srd2.decel_val = -(max_s_lim2*accel/decel);
        }
        // ��ֻʣ��һ�����Ǳ������
        if(srd2.decel_val == 0)
        {
            srd2.decel_val = -1;
        }

        // ���㿪ʼ����ʱ�Ĳ���
        srd2.decel_start = step + srd2.decel_val;

        // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
        if(srd2.step_delay <= srd2.min_delay)
        {
            srd2.step_delay = srd2.min_delay;
            srd2.run_state = RUN2;
        }
        else
        {
            srd2.run_state = ACCEL2;
        }
        // ��λ���ٶȼ���ֵ
        srd2.accel_count = 0;
    }
    MotionStatus2 = 1; // ���Ϊ�˶�״̬
    tim_count2=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR2);
    __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR2,STEPMOTOR_TIM_CHANNEL_x2,tim_count2+srd2.step_delay); // ���ö�ʱ���Ƚ�ֵ
    TIM_CCxChannelCmd(STEPMOTOR_TIMx2, STEPMOTOR_TIM_CHANNEL_x2, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ��
    STEPMOTOR_OUTPUT_ENABLE2();
}

/**
  * ��������: ��ʱ���жϷ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ʵ�ּӼ��ٹ���
  */
void STEPMOTOR_TIMx_IRQHandler2(void)//��ʱ���жϴ���
{
    __IO uint16_t tim_count2=0;
    // �����£��£�һ����ʱ����
    uint16_t new_step_delay2=0;
    // ���ٹ��������һ����ʱ���������ڣ�.
    __IO static uint16_t last_accel_delay2=0;
    // ���ƶ�����������
    __IO static uint32_t step_count2 = 0;
    // ��¼new_step_delay�е������������һ������ľ���
    __IO static int32_t rest2 = 0;
    //��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������
    __IO static uint8_t i2=0;

    OLED_SPeed_count2=step_count2;

    if(__HAL_TIM_GET_IT_SOURCE(&htimx_STEPMOTOR2, STEPMOTOR_TIM_IT_CCx2) !=RESET)
    {
        // �����ʱ���ж�
        __HAL_TIM_CLEAR_IT(&htimx_STEPMOTOR2, STEPMOTOR_TIM_IT_CCx2);

        // ���ñȽ�ֵ
        tim_count2=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR2);
        __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR2,STEPMOTOR_TIM_CHANNEL_x2,tim_count2+srd2.step_delay);

        i2++;     // ��ʱ���жϴ�������ֵ
        if(i2==2) // 2�Σ�˵���Ѿ����һ����������
        {
            i2=0;   // ���㶨ʱ���жϴ�������ֵ
            switch(srd2.run_state) // �Ӽ������߽׶�
            {
            case STOP2:
                step_count2 = 0;  // ���㲽��������
                rest2 = 0;        // ������ֵ
                // �ر�ͨ��
                TIM_CCxChannelCmd(STEPMOTOR_TIMx2, STEPMOTOR_TIM_CHANNEL_x2, TIM_CCx_DISABLE);
                __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR2, STEPMOTOR_TIM_FLAG_CCx2);
                STEPMOTOR_OUTPUT_DISABLE2();
                MotionStatus2 = 0;  //  ���Ϊֹͣ״̬
                break;

            case ACCEL2:
                step_count2++;      // ������1
                if(srd2.dir==CW2)
                {
                    step_position2++; // ����λ�ü�1
                }
                else
                {
                    step_position2--; // ����λ�ü�1
                }
                srd2.accel_count++; // ���ټ���ֵ��1
                new_step_delay2 = srd2.step_delay - (((2 *srd2.step_delay) + rest2)/(4 * srd2.accel_count + 1));//������(��)һ����������(ʱ����)
                rest2 = ((2 * srd2.step_delay)+rest2)%(4 * srd2.accel_count + 1);// �����������´μ��㲹���������������
                if(step_count2 >= srd2.decel_start)// ����ǹ�Ӧ�ÿ�ʼ����
                {
                    srd2.accel_count = srd2.decel_val; // ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
                    srd2.run_state = DECEL2;           // �¸����������ٽ׶�
                }
                else if(new_step_delay2 <= srd2.min_delay) // ����Ƿ񵽴�����������ٶ�
                {
                    last_accel_delay2 = new_step_delay2; // ������ٹ��������һ����ʱ���������ڣ�
                    new_step_delay2 = srd2.min_delay;    // ʹ��min_delay����Ӧ����ٶ�speed��
                    rest2 = 0;                          // ������ֵ
                    srd2.run_state = RUN2;               // ����Ϊ��������״̬
                }
                break;

            case RUN2:
                step_count2++;  // ������1
                if(srd2.dir==CW2)
                {
                    step_position2++; // ����λ�ü�1
                }
                else
                {
                    step_position2--; // ����λ�ü�1
                }
                new_step_delay2 = srd2.min_delay;     // ʹ��min_delay����Ӧ����ٶ�speed��
                if(step_count2 >= srd2.decel_start)   // ��Ҫ��ʼ����
                {
                    srd2.accel_count = srd2.decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
                    new_step_delay2 = last_accel_delay2;// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
                    srd2.run_state = DECEL2;            // ״̬�ı�Ϊ����
                }
                break;

            case DECEL2:
                step_count2++;  // ������1
                if(srd2.dir==CW2)
                {
                    step_position2++; // ����λ�ü�1
                }
                else
                {
                    step_position2--; // ����λ�ü�1
                }
                srd2.accel_count++;
                new_step_delay2 = srd2.step_delay - (((2 * srd2.step_delay) + rest2)/(4 * srd2.accel_count + 1)); //������(��)һ����������(ʱ����)
                rest2 = ((2 * srd2.step_delay)+rest2)%(4 * srd2.accel_count + 1);// �����������´μ��㲹���������������

                //����Ƿ�Ϊ���һ��
                if(srd2.accel_count >= 0)
                {
                    srd2.run_state = STOP2;
                    wait2=1;
                }
                break;
            }
            srd2.step_delay = new_step_delay2; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
        }
    }
}

