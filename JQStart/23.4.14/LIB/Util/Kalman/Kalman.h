 #ifndef __Kalman__H__
#define __Kalman__H__
#ifdef __cplusplus
extern "C" {
#endif
 

float kalman_filter(float ADC_Value);
float kalman_filter2(float ADC_Value);
#ifdef __cplusplus
}
#endif

#endif


