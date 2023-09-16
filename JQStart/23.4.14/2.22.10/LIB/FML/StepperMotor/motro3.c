/***********************
  */
	/*���ö�ʱ��3 �ıȽ����ģʽ�ķ�תת���
	��Ҫ���붨ʱ�������XXXtim.c XXXtim_ex.c�ļ�������
	��������Ҫ��ʼ�����Ƕ�ʱ�� 
	ʹ�ö�ʱ�� ����ͬʱ�궨������� HAL_TIM_MODULE_ENABLED ��Ϊ��ʱ���кü��� �޷�����*/
/* ---------------------------------------------------------------*/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "motro3.h"
#include <math.h>
#include "tim.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
//TIM_HandleTypeDef htimx_STEPMOTOR;
speedRampData3 srd3               = {STOP3,CW3,0,0,0,0,0};         // �Ӽ������߱���
__IO int32_t  step_position3     = 0;           // ��ǰλ��
__IO uint8_t  MotionStatus3      = 0;           //�Ƿ����˶���0��ֹͣ��1���˶�
__IO uint8_t wait3;


__IO int32_t  OLED_SPeed_count3;
__IO uint8_t SP3=0;


TIM_HandleTypeDef htim3;
static void DIR_GPIOInit(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
		    //����ת
    HAL_GPIO_WritePin(DIR3_GPIO_Port, DIR3_Pin, GPIO_PIN_SET);
	    //����ת                     PCPin PCPin */
    GPIO_InitStruct.Pin = DIR3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIR3_GPIO_Port, &GPIO_InitStruct);

}

static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
 if(timHandle->Instance==TIM3)
    {
        /* USER CODE BEGIN TIM3_MspPostInit 0 */
        /* USER CODE END TIM3_MspPostInit 0 */
			
        /**TIM3 GPIO Configuration
        PA6     ------> TIM3_CH1
        */
        GPIO_InitStruct.Pin = BJ3_Motor3_PUL3_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(BJ3_Motor3_PUL3_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM3_MspPostInit 1 */
        /* USER CODE END TIM3_MspPostInit 1 */
    }

}

/* TIM3 init function */
void Motor3_MX_TIM3_Init(void)
{#ifndef StepperMotor3
δ��ʼ���������3
#endif
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;

    __HAL_RCC_TIM3_CLK_ENABLE();//	 STEPMOTOR_TIM_RCC_CLK_ENABLE2();//bj.h

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 31;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0xFFFF;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 500;///
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);//////
    TIM_CCxChannelCmd(TIM3, TIM_CHANNEL_1, TIM_CCx_DISABLE);//////
    /* ʹ�ܱȽ����ͨ�� */
    if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_TIM_MspPostInit(&htim3);
    HAL_TIM_Base_MspInit(&htim3);/*�ж����ȼ���ʹ�� */
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_CC1);
    /* ʹ�ܶ�ʱ���Ƚ���� */
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1);
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(&htim3);
    HAL_TIM_Base_Start(&htim3);// ʹ�ܶ�ʱ��
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
void STEPMOTOR_AxisMoveRel3(__IO int32_t step, __IO uint32_t accel, __IO uint32_t decel, __IO uint32_t speed)
{
    __IO uint16_t tim_count;
    // �ﵽ����ٶ�ʱ�Ĳ���
    __IO uint32_t max_s_lim;
    // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
    __IO uint32_t accel_lim;

    wait3=0;
    SP3=0;
    if(step < 0) // ����Ϊ����
    {
        srd3.dir = CCW3; // ��ʱ�뷽����ת
        STEPMOTOR_DIR_REVERSAL3();
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_RESET);
        step =-step;   // ��ȡ��������ֵ
        SP3=1;
    }
    else
    {
        srd3.dir = CW3; // ˳ʱ�뷽����ת
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_SET);//1
        STEPMOTOR_DIR_FORWARD3();
        SP3=2;
    }

    if(step == 1)    // ����Ϊ1
    {
        srd3.accel_count = -1;   // ֻ�ƶ�һ��
        srd3.run_state = DECEL3;  // ����״̬.
        srd3.step_delay = 1000;	// ����ʱ
    }
    else if(step != 0)  // ���Ŀ���˶�������Ϊ0
    {
        // ���ǵ��������û��ֲ�����ϸ�ļ��㼰�Ƶ�����

        // ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
        // min_delay = (alpha / tt)/ w
        srd3.min_delay = (int32_t)(A_T_x103/speed);

        // ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
        srd3.step_delay = (int32_t)((T1_FREQ_1483 * sqrt(A_SQ3 / accel))/10);

        // ������ٲ�֮��ﵽ����ٶȵ�����
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (uint32_t)(speed*speed/(A_x2003*accel/10));
        // ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
        // ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
        if(max_s_lim == 0)
        {
            max_s_lim = 1;
        }

        // ������ٲ�֮�����Ǳ��뿪ʼ����
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim = (uint32_t)(step*decel/(accel+decel));
        // ���Ǳ����������1�����ܲ��ܿ�ʼ����.
        if(accel_lim == 0)
        {
            accel_lim = 1;
        }

        // ʹ�������������ǿ��Լ�������ٽ׶β���
        if(accel_lim <= max_s_lim)
        {
            srd3.decel_val = accel_lim - step;
        }
        else
        {
            srd3.decel_val = -(max_s_lim*accel/decel);
        }
        // ��ֻʣ��һ�����Ǳ������
        if(srd3.decel_val == 0)
        {
            srd3.decel_val = -1;
        }

        // ���㿪ʼ����ʱ�Ĳ���
        srd3.decel_start = step + srd3.decel_val;

        // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
        if(srd3.step_delay <= srd3.min_delay)
        {
            srd3.step_delay = srd3.min_delay;
            srd3.run_state = RUN3;
        }
        else
        {
            srd3.run_state = ACCEL3;
        }
        // ��λ���ٶȼ���ֵ
        srd3.accel_count = 0;
    }
    MotionStatus3 = 1; // ���Ϊ�˶�״̬
    tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR3);
    __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR3,STEPMOTOR_TIM_CHANNEL_x3,tim_count+srd3.step_delay); // ���ö�ʱ���Ƚ�ֵ
    TIM_CCxChannelCmd(STEPMOTOR_TIMx3, STEPMOTOR_TIM_CHANNEL_x3, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ��
    STEPMOTOR_OUTPUT_ENABLE3();
}

/**
  * ��������: ��ʱ���жϷ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ʵ�ּӼ��ٹ���
  */
void STEPMOTOR_TIMx_IRQHandler3(void)//��ʱ���жϴ���
{
    __IO uint16_t tim_count=0;
    // �����£��£�һ����ʱ����
    uint16_t new_step_delay=0;
    // ���ٹ��������һ����ʱ���������ڣ�.
    __IO static uint16_t last_accel_delay=0;
    // ���ƶ�����������
    __IO static uint32_t step_count = 0;
    // ��¼new_step_delay�е������������һ������ľ���
    __IO static int32_t rest = 0;
    //��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������
    __IO static uint8_t i=0;

    OLED_SPeed_count3=step_count;

    if(__HAL_TIM_GET_IT_SOURCE(&htimx_STEPMOTOR3, STEPMOTOR_TIM_IT_CCx3) !=RESET)
    {
        // �����ʱ���ж�
        __HAL_TIM_CLEAR_IT(&htimx_STEPMOTOR3, STEPMOTOR_TIM_IT_CCx3);

        // ���ñȽ�ֵ
        tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR3);
        __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR3,STEPMOTOR_TIM_CHANNEL_x3,tim_count+srd3.step_delay);

        i++;     // ��ʱ���жϴ�������ֵ
        if(i==2) // 2�Σ�˵���Ѿ����һ����������
        {
            i=0;   // ���㶨ʱ���жϴ�������ֵ
            switch(srd3.run_state) // �Ӽ������߽׶�
            {
            case STOP3:
                step_count = 0;  // ���㲽��������
                rest = 0;        // ������ֵ
                // �ر�ͨ��
                TIM_CCxChannelCmd(STEPMOTOR_TIMx3, STEPMOTOR_TIM_CHANNEL_x3, TIM_CCx_DISABLE);
                __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR3, STEPMOTOR_TIM_FLAG_CCx3);
                STEPMOTOR_OUTPUT_DISABLE3();
                MotionStatus3 = 0;  //  ���Ϊֹͣ״̬
                break;

            case ACCEL3:
                step_count++;      // ������1
                if(srd3.dir==CW3)
                {
                    step_position3++; // ����λ�ü�1
                }
                else
                {
                    step_position3--; // ����λ�ü�1
                }
                srd3.accel_count++; // ���ټ���ֵ��1
                new_step_delay = srd3.step_delay - (((2 *srd3.step_delay) + rest)/(4 * srd3.accel_count + 1));//������(��)һ����������(ʱ����)
                rest = ((2 * srd3.step_delay)+rest)%(4 * srd3.accel_count + 1);// �����������´μ��㲹���������������
                if(step_count >= srd3.decel_start)// ����ǹ�Ӧ�ÿ�ʼ����
                {
                    srd3.accel_count = srd3.decel_val; // ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
                    srd3.run_state = DECEL3;           // �¸����������ٽ׶�
                }
                else if(new_step_delay <= srd3.min_delay) // ����Ƿ񵽴�����������ٶ�
                {
                    last_accel_delay = new_step_delay; // ������ٹ��������һ����ʱ���������ڣ�
                    new_step_delay = srd3.min_delay;    // ʹ��min_delay����Ӧ����ٶ�speed��
                    rest = 0;                          // ������ֵ
                    srd3.run_state = RUN3;               // ����Ϊ��������״̬
                }
                break;

            case RUN3:
                step_count++;  // ������1
                if(srd3.dir==CW3)
                {
                    step_position3++; // ����λ�ü�1
                }
                else
                {
                    step_position3--; // ����λ�ü�1
                }
                new_step_delay = srd3.min_delay;     // ʹ��min_delay����Ӧ����ٶ�speed��
                if(step_count >= srd3.decel_start)   // ��Ҫ��ʼ����
                {
                    srd3.accel_count = srd3.decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
                    new_step_delay = last_accel_delay;// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
                    srd3.run_state = DECEL3;            // ״̬�ı�Ϊ����
                }
                break;

            case DECEL3:
                step_count++;  // ������1
                if(srd3.dir==CW3)
                {
                    step_position3++; // ����λ�ü�1
                }
                else
                {
                    step_position3--; // ����λ�ü�1
                }
                srd3.accel_count++;
                new_step_delay = srd3.step_delay - (((2 * srd3.step_delay) + rest)/(4 * srd3.accel_count + 1)); //������(��)һ����������(ʱ����)
                rest = ((2 * srd3.step_delay)+rest)%(4 * srd3.accel_count + 1);// �����������´μ��㲹���������������

                //����Ƿ�Ϊ���һ��
                if(srd3.accel_count >= 0)
                {
                    srd3.run_state = STOP3;
                    wait3=1;
                }
                break;
            }
            srd3.step_delay = new_step_delay; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
        }
    }
}

