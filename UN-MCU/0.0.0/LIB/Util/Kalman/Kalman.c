#include "kalman.h"

#include "math.h"
float kalman_filter(float ADC_Value)
{
    float x_k1_k1, x_k_k1;
    static float ADC_OLD_Value;
    float Z_k;
    static float P_k1_k1;
    /*-------------------------------------------------------------------------------------------------------------*/
    /*
            Q:过程噪声协方差，Q增大，动态响应变快，收敛稳定性变坏
            R:测量噪声协方差，R增大，动态响应变慢，收敛稳定性变好
            Kg 卡尔曼增益
    */

    static float Q = 0.0001;
    static float R = 5;
    static float Kg = 0;
    static float P_k_k1 = 1;

    float kalman_adc;
    static float kalman_adc_old = 0;
    Z_k = ADC_Value;

    if (fabs(kalman_adc_old - ADC_Value) >= 10)
    {
        x_k1_k1 = ADC_Value * 0.382 + kalman_adc_old * 0.618;
    }
    else
    {
        x_k1_k1 = kalman_adc_old;
    }
    x_k_k1 = x_k1_k1;
    P_k_k1 = P_k1_k1 + Q;

    Kg = P_k_k1 / (P_k_k1 + R);

    kalman_adc = x_k_k1 + Kg * (Z_k - kalman_adc_old);
    P_k1_k1 = (1 - Kg) * P_k_k1;
    P_k_k1 = P_k1_k1;

    ADC_OLD_Value = ADC_Value;
    kalman_adc_old = kalman_adc;

    return kalman_adc;
}



float kalman_filter2(float ADC_Value)
{
    float x_k1_k1, x_k_k1;
    static float ADC_OLD_Value;
    float Z_k;
    static float P_k1_k1;
    /*-------------------------------------------------------------------------------------------------------------*/
    /*
            Q:过程噪声协方差，Q增大，动态响应变快，收敛稳定性变坏
            R:测量噪声协方差，R增大，动态响应变慢，收敛稳定性变好
            Kg 卡尔曼增益
    */

    static float Q = 0.0001;
    static float R = 50;
    static float Kg = 0;
    static float P_k_k1 = 1;

    float kalman_adc;
    static float kalman_adc_old = 0;
    Z_k = ADC_Value;

    if (fabs(kalman_adc_old - ADC_Value) >= 10)
    {
        x_k1_k1 = ADC_Value * 0.382 + kalman_adc_old * 0.618;
    }
    else
    {
        x_k1_k1 = kalman_adc_old;
    }
    x_k_k1 = x_k1_k1;
    P_k_k1 = P_k1_k1 + Q;

    Kg = P_k_k1 / (P_k_k1 + R);

    kalman_adc = x_k_k1 + Kg * (Z_k - kalman_adc_old);
    P_k1_k1 = (1 - Kg) * P_k_k1;
    P_k_k1 = P_k1_k1;

    ADC_OLD_Value = ADC_Value;
    kalman_adc_old = kalman_adc;

    return kalman_adc;
}

