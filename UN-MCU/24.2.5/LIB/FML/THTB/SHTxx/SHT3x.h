#ifndef _SHT3X_H
#define _SHT3X_H


#include "JHAL.h"





typedef enum
{

    //ADDR PIN ->VSS 0x44  <<1
    SHT3x_ADDRESS_VSS=0x88,
    //ADDR PIN ->VDD  0x45 <<1
    SHT3x_ADDRESS_VDD=0x8A,

} SHT3x_Addr;





typedef struct
{

//�������ʱϵ����СֵӦ��Ϊ1 Ҫ�ǽ������ʿ����޸�����ֵ  ע��IO�з�תʱ��ļ��̫С���ܵ���ʱ��׼ ����0��ȫ��û����ʱ��������¸�ʱ�����������

    JHAL_I2CSF i2c;

bool isSingleConversion;

}  SHT3x;






bool  sht3xOpen( SHT3x  *config);

//mdianNumbe ��λ������
//true��ȡ�ɹ� false ��ȡʧ��
bool sht3xGet (SHT3x  *config, float *  temperature,float * humidity );



#endif



