
 #ifndef __ALL_NTC__H__
#define __ALL_NTC__H__
#ifdef __cplusplus
extern "C" {
#endif
 
#include "JHAL.h"


/*ntc����ת���϶�*/


float  ntc2temp_b57332v5103f360( float kRes);
float  ntc2temp_MF52A103F3950( float kRes);
float  ntc2temp_3950(double kRes);
//����ֵ��λk��
double     temp2ntc_3950(float temp);
#ifdef __cplusplus
}
#endif

#endif




