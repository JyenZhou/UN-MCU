/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*   
CUBEMX ����
TIMER
1.Clock Source->internal clock  ʱ��Դѡ��
2.Prescaler Ԥ��Ƶ��ֵ0-65535 72M������7199��Ӧ100us����һ��
3.Counter Period   ����������  ������һ�����ʹ��1000�������� 1s�ж�һ�� (���Լ����Ĺ���)
4.��ѡ�ж� update interrupt  
 
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

 
#include "../JHAL_Timer.h"




TIM_TypeDef* __JHAL_dev2timerInstance(u8 dev)
{
    if(dev==0)
    {
        return TIM1;
    } else if(dev==1)
    {

        return TIM2 ;

    } else if(dev==2)
    {

        return TIM3 ;

    } else if(dev==3)
    {

        return TIM4 ;

    } else if(dev==4)
    {

        return TIM5 ;

    } else if(dev==5)
    {

        return TIM6;

    } else if(dev==6)
    {

        return TIM7 ;

    }

    else {

//�����ڻ�δʵ��
        while(true);

    }
}
 
u8 __JHAL_timerInstance2dev (TIM_TypeDef*  instance )
{

    if(instance==TIM1)
    {

        return 0;

    }


    else  if(instance==TIM2)
    {

        return 1;

    }  else  if(instance==TIM3)
    {

        return 2;

    }  else  if(instance==TIM4)
    {

        return 3;

    }  else  if(instance==TIM5)
    {

        return 4;

    }  else  if(instance==TIM6)
    {

        return 5;

    }  else  if(instance==TIM7)
    {

        return 6;

    }

    else {

//�����ڻ�δʵ��
        while(true);

    }
}


 
 
 bool JHAL_timerOpen(JHAL_Timer *timer ) 
{
	HAL_TIM_Base_Start_IT( (TIM_HandleTypeDef *)(timer->timer));
	return true;
} 




/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ����ִ�����
  */
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	 JHAL_timerInterruptCallBack(__JHAL_timerInstance2dev(htim->Instance));
 }

 
