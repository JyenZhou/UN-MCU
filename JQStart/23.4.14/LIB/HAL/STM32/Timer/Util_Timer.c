/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*   
CUBEMX ����
TIMER
1.Clock Source->internal clock  ʱ��Դѡ��
2.Prescaler Ԥ��Ƶ��ֵ0-65535 72M������7199��Ӧ100us����һ��
3.Counter Period   ����������  ������һ�����ʹ��1000�������� 1s�ж�һ��
4.��ѡ�����ж�
	5.������ص������ŵ�Ҫ�õĵط�
      void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
        {
				
        }
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#ifdef UTTIL_TIMER
#include "Util_timer.h"


/**

  * ��������: ������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void utilTimer_Init(void)
{
//ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����жϣ�TIM_IT_UPDATE 
#ifdef UTIL_TIMER1_EN

 HAL_TIM_Base_Start_IT(&htim1); 
#endif
   

}





/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ����ִ�����
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//}

#endif
