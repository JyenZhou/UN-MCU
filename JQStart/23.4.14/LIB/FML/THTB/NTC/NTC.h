
 #ifndef __ALL_NTC__H__
#define __ALL_NTC__H__
#ifdef __cplusplus
extern "C" {
#endif
 
#include "JHAL.h"


/*ntc电阻转摄氏度*/


float  ntc2temp_b57332v5103f360( float kRes);
float  ntc2temp_MF52A103F3950( float kRes);
float  ntc2temp_3950(double kRes);
//返回值单位kΩ
double     temp2ntc_3950(float temp);
#ifdef __cplusplus
}
#endif

#endif




