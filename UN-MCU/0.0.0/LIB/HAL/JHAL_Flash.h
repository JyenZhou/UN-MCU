/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		FlashĬ��ͨ��HAL����  ע���װ�ڲ��Ѿ����˵�ַƫ��   ����ȥ��00��ַ����falsh�����׵�ַ
 		����Ҫע���С��Ҫ����

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

#ifndef __JHAL_FLASH__H__
#define __JHAL_FLASH__H__
#include "JUTIL.h"
#ifdef __cplusPlus
extern "C" {
#endif



#define JHAL_FLASH_WAITETIME 50000       //FLASH�ȴ���ʱʱ�� 

    /*
    дfalsh  �Զ������ͼ�CRC��JHAL_flashReadһ����
    page ���2��ʼ ��һҳԤ����bootloader�˲�ʹ��bootloaderʱ�������   ʵ���ϴ����һҳ��ʼ��ǰд
    data �����ǽṹ��
    length  ���ݴ�С���ٸ��ֽڣ�u8) ע�ⳤ��ҪС������-2��crc��
    	ע�⣺��֧�ֿ�ҳд��ÿ��ֻ֧�ֵ�ҳ���Ѿ�������ʵ�ָú���
     */
    bool JHAL_flashWirte(u32 page,void *data,u16 length);

    /*��flash ����ֵtrue crcУ��ɹ� faslhУ��ʧ�� У��ʧ���ǲ����޸Ĵ�������ֵ
    data Ҫ�����ݻ�����
    length  ���ݴ�С���ٸ��ֽڣ�u8)
      ע�⣺�Ѿ�������ͨ��ֱ�Ӷ���ַ��ʽʵ�ָú���*/


    bool JHAL_flashRead(u32 page,void *data,u16 length);

    //������ַ���������� ע����ʼҪ�������룬������ַ�ڲ��Զ�����
    bool JHAL_flashErasePages(uint32_t startPageAddr, uint32_t endAddr);
    //��ָ����ַд��һ������
    // ע��Ҫ�ǵ��Ȳ��������ֵ�Ƭ���ڲ����õ���8���ֽڶ������ｨ��ÿ��8�ֽڶ���д �����Ҫ������д��һ��д�� �ϴε�β����1������8�ֽھͲ�Ӱ�챾��ͷ�������ϴ�β��
    bool JHAL_flashWriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng);



    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96λUID�൱��3��32λ����  ÿ8λ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ�
    void uid2string(char* string,int buffSize );


#ifdef CplusPlus
}
#endif

#endif








