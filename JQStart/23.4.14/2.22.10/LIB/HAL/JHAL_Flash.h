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
#ifdef __CplusPlus
extern "C" {
#endif




    /*
    дfalsh  �Զ���CRC��JHAL_flashReadһ����
    page ��2��ʼ ��һҳԤ����bootloader�˲�ʹ��bootloaderʱ�������   ʵ���ϴ����һҳ��ʼ��ǰд
    data �����ǽṹ��
    length  ���ݴ�С���ٸ��ֽڣ�u8) ע�ⳤ��ҪС������-2��crc��
    isbefoErase �Ƿ���д��ǰ����
     */
    bool JHAL_flashWirte(u32 page,void *data,u16 length);

    /*��flash ����ֵtrue crcУ��ɹ� faslhУ��ʧ�� У��ʧ���ǲ����޸Ĵ�������ֵ
    data Ҫ�����ݻ�����
    length  ���ݴ�С���ٸ��ֽڣ�u8)

    */
    bool JHAL_flashRead(u32 page,void *data,u16 length);

    //������ַ���������� ע����ʼҪ��������
    bool JHAL_flashErasePage(uint32_t startPageAddr, uint32_t endAddr);
    //��ָ����ַд��һ������  ע��Ҫ�ǵò���
    bool JHAL_flashWriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng);

    //��ȡFlash������ַ��
    u32 JHAL_flashGetEndAddr(void);
    //��ȡ������С
    u32 JHAL_flashGetPageSize(void);



#ifdef CplusPlus
}
#endif

#endif








