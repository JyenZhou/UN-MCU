#ifndef _JHAL_IICX_H
#define _JHAL_IICX_H

#include "HAL\JHAL_GPIO.h"


/*ģ��IIC ʹ����Ҫ����ȫ�ֵ� JHAL_IIC_Number
 ʹ�õ�idҪС�ڶ����JHAL_IIC_Number  ���id�������±� ��0��ʼ

 */


typedef enum
{
//��׼��
    JHAL_IIC_Device_Standard=0,
//����TEA5767
    JHAL_IIC_Device_TEA5767

} JHAL_IIC_Device_Type;



typedef struct
{
//�������ʱϵ����СֵӦ��Ϊ1 Ҫ�ǽ������ʿ����޸�����ֵ  ע��IO�з�תʱ��ļ��̫С���ܵ���ʱ��׼ ����0��ȫ��û����ʱ��������¸�ʱ�����������
    u8 delayMultiple;
    JHAL_IO_Port sclPort;
    u8  sclPin;
    JHAL_IO_Port sdaPort;
    u8  sdaPin;
    u16 slaveAddress;
    JHAL_IIC_Device_Type deviceType;
//IO�Ƿ�����ģʽ ����©
    bool isIO_Mode_PP;

} JHAL_IICConfig;



//��ʼ��ģ���IIC
void JHAL_iicInit(u8 id,JHAL_IICConfig init );
/*devAddress ��������ַ  д��ʱ��*/

//ֻ��������ַ�Ĳ���
//����/��ȡ  һ���Ĵ�����N������
bool  JHAL_iicTransmit(u8 id, uint8_t *pData, u8 dataSize);
bool  JHAL_iicReceice(u8 id, uint8_t *pData,  u8 dataSize);



//������ַ+�Ĵ�����ַ�Ĳ���
//����ָ���Ĵ���дһЩ����
bool JHAL_iicMemWrite(u8 id, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize);
//��ָ���Ĵ����ж���һЩ����
bool JHAL_iicMemRead(u8 id, u16 memAddress,bool memSizeIs16, u8 *pData, u8 dataSize);



#endif

