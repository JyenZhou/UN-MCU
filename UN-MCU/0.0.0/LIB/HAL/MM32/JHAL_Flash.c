#include "..\JHAL_Flash.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		FlashĬ��ͨ��HAL����  ע���װ�ڲ��Ѿ����˵�ַƫ��   ����ȥ��00��ַ����falsh�����׵�ַ
 		����Ҫע���С��Ҫ����  ��Ȼ������Ҳ�з������

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/



#define JHAL_FlashStartAddr  ((uint32_t)0x08000000)
#if defined MM32G0001

//������С1K
#define JHAL_FlashPageSize 1024

#define JHAL_FlashPageNumber 16
#define JHAL_FlashMaxSize  JHAL_FlashPageSize*JHAL_FlashPageNumber


#else
#error δ����flash��С
#endif



#define JHAL_FlashEndAddr (JHAL_FlashStartAddr+JHAL_FlashMaxSize)



/*******************************************************************************
*@ �������ܻ���: ����ָ�����������Flash����
  * @�������:StartPage ��ʼ��ַ (������������)
							EndPage ������ַ  (���Զ���������)

  * @�� �� ֵ: ��������״̬
  * @��ע: ��
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/


bool __JHAL_flashErasePage(const uint32_t targetaddress )
{

    JHAL_enableInterrupts();
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);


    FLASH_ErasePage(targetaddress);
    if(FLASH_COMPLETE!= FLASH_WaitForLastOperation(FLASH_WAITETIME))
    {
        FLASH_Lock();
        JHAL_enableInterrupts();
        return false;
    }

    FLASH_ClearFlag(FLASH_FLAG_EOP);

    FLASH_Lock();
    JHAL_enableInterrupts();
    return true;

}




/**
  * ����  flash ������д����ֽ�����
  * ����  address�� ָ��flash��ַ
  *  p_FlashBuffer: д�����ݵ�ָ��
  *    leng�����ݵĳ���
  * ����  �ޡ�
  * ע��д��ǰҪ�Ȳ�����ַ���ڵ�ҳ //����Ĳ���8���ֽڰ��ĸ�ֱ�Ӵ���  ��Ϊ�Ƕ��������Ժ���ָ�볬��û��  �����´�д���ַҪ����8
  */
bool JHAL_flashWriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng)//��ַ����Ϊ��8����
{


    JHAL_disableInterrupts();
    FLASH_Unlock();
    for(u16 i=0; i<leng; i++) //һҳ1024byte������һ��д64bit=8byte��128�������
    {
        FLASH_ProgramWord(address, *p_FlashBuffer);
        if(FLASH_COMPLETE!= FLASH_WaitForLastOperation(JHAL_FLASH_WAITETIME))
        {
            return false;
        }
        FLASH_ClearFlag(FLASH_FLAG_EOP);
        p_FlashBuffer+=4;
        address+=4;
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }
    JHAL_enableInterrupts();
    return true;
}


