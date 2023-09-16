/***********************
  */
	/*���ö�ʱ��1 �ıȽ����ģʽ�ķ�תת���
	��Ҫ���붨ʱ�������XXXtim.c XXXtim_ex.c�ļ�������
	��������Ҫ��ʼ�����Ƕ�ʱ�� 
	ʹ�ö�ʱ�� ����ͬʱ�궨������� HAL_TIM_MODULE_ENABLED ��Ϊ��ʱ���кü��� �޷�����*/
/* ---------------------------------------------------------------*/

#include "motro1.h"
#include <math.h>
#include "tim.h"


/**
  * ��������: ���λ���˶����˶������Ĳ���
  * �������: step���ƶ��Ĳ��� (����Ϊ˳ʱ�룬����Ϊ��ʱ��).
              accel  ���ٶ�,ʵ��ֵΪaccel*0.1*rad/sec^2
              decel  ���ٶ�,ʵ��ֵΪdecel*0.1*rad/sec^2 ����/ÿ�� ƽ��
              speed  ����ٶ�,ʵ��ֵΪspeed*0.1*rad/sec
  * �� �� ֵ: ��
  * ˵    ��: �Ը����Ĳ����ƶ�����������ȼ��ٵ�����ٶȣ�Ȼ���ں���λ�ÿ�ʼ
  *           ������ֹͣ��ʹ�������˶�����Ϊָ���Ĳ���������Ӽ��ٽ׶κ̲ܶ���
  *           �ٶȺ������ǻ�û�ﵽ����ٶȾ�Ҫ��ʼ����
  */

speedRampData srd               = {STOP,CW,0,0,0,0,0};         // �Ӽ������߱���
__IO int32_t  step_position     = 0;           // ��ǰλ��
__IO uint8_t  MotionStatus      = 0;           //�Ƿ����˶���0��ֹͣ��1���˶�
__IO uint8_t wait1;

__IO int32_t OLED_SPeed_count;
__IO uint8_t SP=0;
/* TIM1 init function */
TIM_HandleTypeDef htim1;

static void DIR_GPIOInit()
{
		 GPIO_InitTypeDef GPIO_InitStruct;
		    //����ת
    HAL_GPIO_WritePin(GPIOC, DIR1_Pin, GPIO_PIN_SET);
	    //����ת                     PCPin PCPin */
    GPIO_InitStruct.Pin = DIR1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIR1_GPIO_Port, &GPIO_InitStruct);

}

static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(timHandle->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspPostInit 0 */
        /* USER CODE END TIM1_MspPostInit 0 */
        /**TIM1 GPIO Configuration
        PA11     ------> TIM1_CH4
        */
        GPIO_InitStruct.Pin = BJ1_Motor1_PUL1_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(BJ1_Motor1_PUL1_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM1_MspPostInit 1 */
        /* USER CODE END TIM1_MspPostInit 1 */
    }


}

void Motor1_MX_TIM1_Init(void)  
{
	    /*�������1�Ķ�ʱ����ʼ��*/
#ifndef StepperMotor1
δ��ʼ���������1
#endif
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

    STEPMOTOR_TIM_RCC_CLK_ENABLE();//bj.h
    /*******������ʼ��*******/
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 31;//Ԥ��Ƶ
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;//����ģʽ
    htim1.Init.Period = 0xFFFF;//����
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ
    htim1.Init.RepetitionCounter = 0;//�ظ�������
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    /*����Ƚϳ�ʼ��*/
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 500;//
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, TIM_CHANNEL_4);//////**********
    TIM_CCxChannelCmd(STEPMOTOR_TIMx, TIM_CHANNEL_4, TIM_CCx_DISABLE);//////*************
    /* ʹ�ܱȽ����ͨ�� */
    if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_TIM_MspPostInit(&htim1);//��Ӧ���ų�ʼ��

    HAL_TIM_Base_MspInit(&htim1);/*�ж����ȼ���ʹ�� */
    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC4);//*************����жϱ�־λ***************
    /* ʹ�ܶ�ʱ���Ƚ���� */
    __HAL_TIM_ENABLE_IT(&htimx_STEPMOTOR, TIM_IT_CC4);//

    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(&htimx_STEPMOTOR);
    HAL_TIM_Base_Start(&htimx_STEPMOTOR);// ʹ�ܶ�ʱ��

DIR_GPIOInit();
}

   


void STEPMOTOR_AxisMoveRel(__IO int32_t  step, __IO uint32_t accel, __IO uint32_t decel, __IO uint32_t speed)
{

    __IO uint16_t tim_count;
    // �ﵽ����ٶ�ʱ�Ĳ���
    __IO uint32_t max_s_lim;
    // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
    __IO uint32_t accel_lim;
    wait1=0;
    SP=0;
    if(step < 0) // ����Ϊ����
    {
        srd.dir = CCW; // ��ʱ�뷽����ת
        STEPMOTOR_DIR_REVERSAL();
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_RESET);
        step =-step;   // ��ȡ��������ֵ
        SP=1;//����������־λ����������OLED����ʾ���Բ���
    }
    else
    {
        srd.dir = CW; // ˳ʱ�뷽����ת
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_SET);//1
        STEPMOTOR_DIR_FORWARD();
        SP=2;
    }

    if(step == 1)    // ����Ϊ1
    {
        srd.accel_count = -1;   // ֻ�ƶ�һ��
        srd.run_state = DECEL;  // ����״̬.
        srd.step_delay = 1000;	// ����ʱ

    }
    else if(step != 0)  // ���Ŀ���˶�������Ϊ0
    {

        // ���ǵ��������û��ֲ�����ϸ�ļ��㼰�Ƶ�����

        // ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
        // min_delay = (alpha / tt)/ w
        srd.min_delay = (int32_t)(A_T_x10/speed);

        // ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
        srd.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);

        // ������ٲ�֮��ﵽ����ٶȵ�����
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
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
            srd.decel_val = accel_lim - step;
        }
        else
        {
            srd.decel_val = -(max_s_lim*accel/decel);
        }
        // ��ֻʣ��һ�����Ǳ������
        if(srd.decel_val == 0)
        {
            srd.decel_val = -1;
        }

        // ���㿪ʼ����ʱ�Ĳ���
        srd.decel_start = step + srd.decel_val;

        // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
        if(srd.step_delay <= srd.min_delay)
        {
            srd.step_delay = srd.min_delay;
            srd.run_state = RUN;
        }
        else
        {
            srd.run_state = ACCEL;
        }
        // ��λ���ٶȼ���ֵ
        srd.accel_count = 0;
    }
    MotionStatus = 1; // ���Ϊ�˶�״̬
    tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
    __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x,tim_count+srd.step_delay); // ���ö�ʱ���Ƚ�ֵ
    TIM_CCxChannelCmd(STEPMOTOR_TIMx, STEPMOTOR_TIM_CHANNEL_x, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ��
    STEPMOTOR_OUTPUT_ENABLE();


}

/**
  * ��������: ��ʱ���жϷ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ʵ�ּӼ��ٹ���
  */
void STEPMOTOR_TIMx_IRQHandler(void)//��ʱ���жϴ���
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



    OLED_SPeed_count=step_count;



    if(__HAL_TIM_GET_IT_SOURCE(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx) !=RESET)
    {
        // �����ʱ���ж�
        __HAL_TIM_CLEAR_IT(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx);

        // ���ñȽ�ֵ
        tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
        __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x,tim_count+srd.step_delay);

        i++;     // ��ʱ���жϴ�������ֵ
        if(i==2) // 2�Σ�˵���Ѿ����һ����������
        {
            i=0;   // ���㶨ʱ���жϴ�������ֵ
            switch(srd.run_state) // �Ӽ������߽׶�
            {
            case STOP:
                step_count = 0;  // ���㲽��������
                rest = 0;        // ������ֵ
                // �ر�ͨ��
                TIM_CCxChannelCmd(STEPMOTOR_TIMx, STEPMOTOR_TIM_CHANNEL_x, TIM_CCx_DISABLE);
                __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR, STEPMOTOR_TIM_FLAG_CCx);
                STEPMOTOR_OUTPUT_DISABLE();
                MotionStatus = 0;  //  ���Ϊֹͣ״̬
                break;

            case ACCEL:
                step_count++;      // ������1
                if(srd.dir==CW)
                {
                    step_position++; // ����λ�ü�1
                }
                else
                {
                    step_position--; // ����λ�ü�1
                }
                srd.accel_count++; // ���ټ���ֵ��1
                new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4 * srd.accel_count + 1));//������(��)һ����������(ʱ����)
                rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������
                if(step_count >= srd.decel_start)// ����ǹ�Ӧ�ÿ�ʼ����
                {
                    srd.accel_count = srd.decel_val; // ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
                    srd.run_state = DECEL;           // �¸����������ٽ׶�
                }
                else if(new_step_delay <= srd.min_delay) // ����Ƿ񵽴�����������ٶ�
                {
                    last_accel_delay = new_step_delay; // ������ٹ��������һ����ʱ���������ڣ�
                    new_step_delay = srd.min_delay;    // ʹ��min_delay����Ӧ����ٶ�speed��
                    rest = 0;                          // ������ֵ
                    srd.run_state = RUN;               // ����Ϊ��������״̬
                }
                break;

            case RUN:
                step_count++;  // ������1
                if(srd.dir==CW)
                {
                    step_position++; // ����λ�ü�1
                }
                else
                {
                    step_position--; // ����λ�ü�1
                }
                new_step_delay = srd.min_delay;     // ʹ��min_delay����Ӧ����ٶ�speed��
                if(step_count >= srd.decel_start)   // ��Ҫ��ʼ����
                {
                    srd.accel_count = srd.decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
                    new_step_delay = last_accel_delay;// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
                    srd.run_state = DECEL;            // ״̬�ı�Ϊ����
                }
                break;

            case DECEL:
                step_count++;  // ������1
                if(srd.dir==CW)
                {
                    step_position++; // ����λ�ü�1
                }
                else
                {
                    step_position--; // ����λ�ü�1
                }
                srd.accel_count++;
                new_step_delay = srd.step_delay - (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); //������(��)һ����������(ʱ����)
                rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������

                //����Ƿ�Ϊ���һ��
                if(srd.accel_count >= 0)
                {
                    srd.run_state = STOP;
                    wait1=1;
                }
                break;
            }
            srd.step_delay = new_step_delay; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
        }
    }
}


