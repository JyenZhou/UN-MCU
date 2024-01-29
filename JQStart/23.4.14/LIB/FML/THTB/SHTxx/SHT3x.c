#include "sht3x.h"



/* �����λ���� */

#define    SOFT_RESET_CMD   0x30A2
/*
���β���ģʽ
������ʽ��Repeatability_CS_CMD
CS��Clock stretching
*/
#define  HIGH_ENABLED_CMD     0x2C06
#define   MEDIUM_ENABLED_CMD    0x2C0D
#define   LOW_ENABLED_CMD      0x2C10
#define   HIGH_DISABLED_CMD     0x2400
#define    MEDIUM_DISABLED_CMD  0x240B
#define   LOW_DISABLED_CMD      0x2416

/*
���ڲ���ģʽ
������ʽ��Repeatability_MPS_CMD
MPS��measurement per second
*/
#define   HIGH_0_5_CMD     0x2032
#define  MEDIUM_0_5_CMD   0x2024
#define  LOW_0_5_CMD      0x202F
#define   HIGH_1_CMD       0x2130
#define   MEDIUM_1_CMD     0x2126
#define   LOW_1_CMD        0x212D
#define  HIGH_2_CMD       0x2236
#define   MEDIUM_2_CMD     0x2220
#define   LOW_2_CMD        0x222B
#define   HIGH_4_CMD       0x2334
#define  MEDIUM_4_CMD     0x2322
#define   LOW_4_CMD         0x2329
#define  HIGH_10_CMD      0x2737
#define   MEDIUM_10_CMD     0x2721
#define  LOW_10_CMD       0x272A
/* ���ڲ���ģʽ��ȡ�������� */
//	ע�⣺�ڴ�ģʽ�²���ѡ��ʱ������  ���ݲɼ�Ƶ�ʺͿ��ظ������û�Ӱ�촫�����Ĳ�������ʱ��͵������ġ�
#define    READOUT_FOR_PERIODIC_MODE   0xE000

 


bool  sht3xOpen( SHT3x  *config)
{
    JHAL_i2csfOpen(	&config->i2c); 
    // 0x2130  ��ʾ�������ݲɼ�ģʽ ����Ƚ�ͨ��   ��ʱ��Ҫ��ߵ���Ҫ���·�װ
//  ����1ms
 	if(!config-> isSingleConversion)
	{
    u8 cmdData[2]= {HIGH_1_CMD>>8,(u8)HIGH_1_CMD};
    u8 timeoutCount=10;

    while(!JHAL_i2csfTransmit(&config->i2c,  cmdData,2))
    {
        if(timeoutCount--==0) {
            return false;
        }
    }
	}
    return true;

}


 

int  sht3x_crc8_compute(u8 *check_data, u8 num_of_data)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

// calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) {
        crc ^= (check_data[byteCtr]);
        //crcУ�飬���λ��1��^0x31
        for(bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            }  else {
                crc = (crc << 1);
            }
        }
    }
    return crc;

}
bool sht30_crc8_check(u8 *p,u8 num_of_data,u8 CrcData)
{
    uint8_t crc;
    crc = sht3x_crc8_compute(p, num_of_data);// calculates 8-Bit checksum
    if(crc != CrcData)
    {
        return false;
    }
    return true;
}


bool sht3xGet(SHT3x  *config,  float *  temperature,float * humidity )
{
//�����ǰ���һ����Ҳ��дһ�������SHT30��0xe000��������Ƿ���SHT30ת������ļĴ�����

//�����ǿ�ʼ��ȡ���ݣ����е�����p��Ž����ǰ��������¶�ֵ����������ʪ��ֵ����ǰ�����¶�ֵ���棬
//p[0]���¶ȵĸ߰�λ��p[1]�ǵͰ�λ��p[2]��CRCУ�飬�й�CRCУ���֪ʶ���ǰٶ����濴�ģ�
//Ҫ�Ǹ�λ�����û������Բ���crcУ�飬ֱ����p[0]��p[1]�Ϳ���ת�������¶ȵ�ֵ��

    uint8_t	readbuff[6];
  
    /*��ȡ����**/
	 if( config->  isSingleConversion){
		   if(  !JHAL_i2csfMemRead(&config->i2c,HIGH_DISABLED_CMD,true,readbuff,6))
    {
        return false ;
    }
		 
	 }else{

    if(  !JHAL_i2csfMemRead(&config->i2c,READOUT_FOR_PERIODIC_MODE,true,readbuff,6))
    {
        return false ;
    }

	}

    u16 data;
   

    //�ȴ����¶ȵ�������ݣ�λ�������ǰ����

    //crcУ��
 
	
    
    //crcУ��Ҫ�ǲ��ɹ��ͷ���1��ͬʱ��������¶�ֵ
    if(sht30_crc8_check(readbuff,2,readbuff[2]) )
    {
        //��2��8λ����ƴ��Ϊһ��16λ������
        data=( readbuff[0] << 8) | readbuff[1];
        //�¶�ת��
        *temperature =   175.0f *  data  / 65535  - 45 ;
    }
    else
    {
        return false;
    }

    //ʪ�ȴ���
    
    if(sht30_crc8_check(&readbuff[3],2, readbuff[5]) )
    {
        //�ο������¶ȵĴ���
        data= (readbuff[3] << 8) | readbuff[4];
        *humidity =   100.0f *  data / 65535.0f   ;

    }
    else
    {
        return false;
    }

    return true;
}





