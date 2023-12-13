#ifndef _JHAL_I2CSFX_H
#define _JHAL_I2CSFX_H

#include "HAL\JHAL_GPIO.h"


/*ģ��IIC ʹ����Ҫ����ȫ�ֵ� JHAL_I2CSF_Number
 ʹ�õ�idҪС�ڶ����JHAL_I2CSF_Number  ���id�������±� ��0��ʼ

 */


typedef enum
{
//��׼��
    JHAL_I2CSF_Device_Standard=0,
//����TEA5767
    JHAL_I2CSF_Device_TEA5767

} JHAL_I2CSF_Device_Type;



typedef struct
{
/* �������ʱϵ��   ���浥λ��usΪ��׼ Ҫ�ǽ������ʿ����޸�����ֵ  ע��IO�з�תʱ��ļ��̫С���ܵ���ռ�ձȲ��� ���ñ��ش���ʱ�ӵ�IIC�ǿ������̵� ���Ŀǰû����У׼
  STM32 64M�߼������Ƕ�ӦƵ��(д��ʱ��Ƶ�ʱȽϵͣ�����ʱ��Ƶ�ʱȽϸ�,0�ǿ�©��ȡsht3x���е�) 
	0: 125-142K
  1: 91-111K
	5:43-61K
	10:25-37K
	15��19-27K
	50��6-9K
	*/
	u8 delayMultiple;
    JHAL_IO_Port sclPort;
    u8  sclPin;
    JHAL_IO_Port sdaPort;
    u8  sdaPin;
    // ������ַ��д��ַ �������8λ��Ҫ����һλ�ٴ�����
    u16 slaveAddress;
    JHAL_I2CSF_Device_Type deviceType;
//IO�Ƿ�����ģʽ ����©
    bool isIO_Mode_PP:1;

} JHAL_I2CSF;



//��ʼ��ģ���IIC  id0 ��ʼ
void JHAL_i2csfOpen( JHAL_I2CSF *init );
/*devAddress ��������ַ  д��ʱ��*/

//ֻ��������ַ�Ĳ���
//����/��ȡ  һ���Ĵ�����N������
bool  JHAL_i2csfTransmit(JHAL_I2CSF *handle,  uint8_t *pData, u8 dataSize);
bool JHAL_i2csfReceice(JHAL_I2CSF  * config, u8* memData,u8 memLength,bool isStop, u8 *readDataBuff, u8 dataSize);



//������ַ+�Ĵ�����ַ�Ĳ���
//����ָ���Ĵ���дһЩ����
bool JHAL_i2csfMemWrite(JHAL_I2CSF *handle, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize);
//��ָ���Ĵ����ж���һЩ����
bool JHAL_i2csfMemRead(JHAL_I2CSF *handle, u16 memAddress,bool memSizeIs16, u8 *pData, u8 dataSize);



#endif
