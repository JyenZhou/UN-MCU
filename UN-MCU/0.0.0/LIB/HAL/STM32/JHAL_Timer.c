/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
CUBEMX ����
TIMER
1.Clock Source->internal clock  ʱ��Դѡ��
//����-Count Setting
2.Prescaler Ԥ��Ƶ��ֵ0-65535 72M������7199��Ӧ100us����һ��
3.Counter Period   ����������  ������һ�����ʹ��1000�������� 1s�ж�һ�� (���Լ����Ĺ���)
4. auto-reload preload (�Զ���װ�� ʹ��)
//�ж�
5.��ѡ�ж� update interrupt

*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/


#include "../JHAL_Timer.h"

#ifdef HAL_TIM_MODULE_ENABLED



//������ļ������

#ifdef TIM1
#define __TIM1_DEFINED 1
#else
#define __TIM1_DEFINED 0
#endif

#ifdef TIM2
#define __TIM2_DEFINED 1
#else
#define __TIM2_DEFINED 0
#endif

#ifdef TIM3
#define __TIM3_DEFINED 1
#else
#define __TIM3_DEFINED 0
#endif

#ifdef TIM4
#define __TIM4_DEFINED 1
#else
#define __TIM4_DEFINED 0
#endif

#ifdef TIM5
#define __TIM5_DEFINED 1
#else
#define __TIM5_DEFINED 0
#endif

#ifdef TIM6
#define __TIM6_DEFINED 1
#else
#define __TIM6_DEFINED 0
#endif

#ifdef TIM7
#define __TIM7_DEFINED 1
#else
#define __TIM7_DEFINED 0
#endif

#ifdef TIM8
#define __TIM8_DEFINED 1
#else
#define __TIM8_DEFINED 0
#endif


#define __TIM_COUNT COUNT_MACROS_CONST_VALUE(  __TIM1_DEFINED, __TIM2_DEFINED, __TIM3_DEFINED, __TIM4_DEFINED,  __TIM5_DEFINED, __TIM6_DEFINED, __TIM7_DEFINED, __TIM8_DEFINED)


JHAL_Timer* __JHAL_timerDevArray[__TIM_COUNT];


TIM_TypeDef* __JHAL_dev2timerInstance(u8 dev)
{

    if(false)
    {

    }
#ifdef TIM1
    else if(dev==0)
    {
        return TIM1;
    }
#endif

#ifdef TIM2
    else if(dev==1)
    {

        return TIM2 ;

    }
#endif

#ifdef TIM3
    else if(dev==2)
    {

        return TIM3 ;

    }
#endif
#ifdef TIM4
    else if(dev==3)
    {

        return TIM4 ;

    }
#endif

#ifdef TIM5
    else if(dev==4)
    {

        return TIM5 ;

    }
#endif
#ifdef TIM6
    else if(dev==5)
    {

        return TIM6;

    }
#endif
#ifdef TIM5
    else if(dev==6)
    {
        return TIM7 ;
    }
#endif

    else {

//�����ڻ�δʵ��

        JHAL_error("__JHAL_dev2timerInstance");

    }
    return NULL;
}





u8 __JHAL_timerInstance2Id (TIM_TypeDef*  instance )
{
    if(false)
    {
    }
#ifdef TIM1

    if(instance==TIM1)
    {
        return 0;
    }
#endif
#ifdef TIM2
    else  if(instance==TIM2)
    {
        return 1;
    }
#endif
#ifdef TIM3
    else  if(instance==TIM3)
    {
        return 2;
    }
#endif
#ifdef TIM4
    else  if(instance==TIM4)
    {
        return 3;
    }
#endif
#ifdef TIM5
    else  if(instance==TIM5)
    {
        return 4;
    }
#endif
#ifdef TIM6
    else  if(instance==TIM6)
    {
        return 5;
    }
#endif
#ifdef TIM7
    else  if(instance==TIM7)
    {
        return 6;
    }
#endif
    else {

        JHAL_error("__JHAL_timerInstance2Id");


    }
    return NULL;
}




bool JHAL_timerOpen(JHAL_Timer *timer )
{

    timer->id=__JHAL_timerInstance2Id(((TIM_HandleTypeDef *)(timer->dev))->Instance);


    __JHAL_timerDevArray[timer->id]=timer;

    HAL_TIM_Base_Start_IT( (TIM_HandleTypeDef *)(timer->dev));
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
    JHAL_timerInterruptCallback(__JHAL_timerInstance2Id(htim->Instance));
}

#endif
