#include "..\JHAL_Flash.h"

#define  __JHAL_Flash_Exist
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



#define JHAL_FlashStartAddr FLASH_BASE
#if defined STM32F103xE   || defined STM32F103xB

//������С1K
#define JHAL_FlashPageSize 0x400
//256K xe�Ĵ�С��256-512 ����С�ļ����Ժ� һ����˵������  ����������ټӸ����嶨��ĺ궨��
#define JHAL_FlashPageNumber 256
#define JHAL_FlashMaxSize  (*((uint16_t *)FLASH_SIZE_DATA_REGISTER))


#elif defined (STM32F407xx)
//16K
#define JHAL_FlashPageSize 0x4000
#define JHAL_FlashPageNumber 10
#define JHAL_FlashMaxSize  (*((uint16_t *)0x1FFF7A22))

#else
#error δ����flash��С
#endif



#define JHAL_FlashEndAddr (JHAL_FlashStartAddr+JHAL_FlashMaxSize)







bool __JHAL_flashErasePage(const uint32_t targetaddress )
{



    //1������FLASH

    //2������FLASH
    JHAL_disableInterrupts();
    HAL_FLASH_Unlock();
    //��ʼ��FLASH_EraseInitTypeDef

    FLASH_EraseInitTypeDef f;
#if defined (STM32F407xx)
    f.TypeErase = FLASH_TYPEERASE_SECTORS;     //�������ͣ���������
    f.Sector = targetaddress ; //Ҫ������������ʼ��ַ
    f.VoltageRange = FLASH_VOLTAGE_RANGE_3;    //��ѹ��Χ��VCC=2.7~3.6V֮��!!
    f.NbSectors = 1;                           //һ��ֻ����һ������
#else
    f.TypeErase = FLASH_TYPEERASE_PAGES;//����Flashִ��ҳ��ֻ����������
    f.PageAddress = targetaddress; //Ҫ������������ʼ��ַ
    f.NbPages = 1; //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ  һҳ1K��С
#endif


    //����PageError
    uint32_t PageError = 0;
    //���ò�������

    if(  HAL_FLASHEx_Erase(&f, &PageError)!=HAL_OK)  //��������
    {
        HAL_FLASH_Lock();
        JHAL_enableInterrupts();
        return false;
    }




    bool isok=  FLASH_WaitForLastOperation(JHAL_FLASH_WAITETIME)==HAL_OK; //�ȴ��ϴβ������

    HAL_FLASH_Lock();
    JHAL_enableInterrupts();
    return  isok;
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
    leng=(leng/8)+(leng%8!=0);

    JHAL_disableInterrupts();
    HAL_FLASH_Unlock();

    for(u16 i=0; i<leng; i++) //һҳ1024byte������һ��д64bit=8byte��128�������
    {
        if(HAL_OK!=  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,address,* (u64_t*)p_FlashBuffer))
        {
            HAL_FLASH_Lock();
            JHAL_enableInterrupts();
            return false;
        }
        p_FlashBuffer+=8;
        address+=8;
    }

    bool isok=  FLASH_WaitForLastOperation(JHAL_FLASH_WAITETIME)==HAL_OK; //�ȴ��ϴβ������
    HAL_FLASH_Lock();
    JHAL_enableInterrupts();
    return isok;
}




u32 JHAL_uidGetHigh()
{
    return HAL_GetUIDw2();
}
u32 JHAL_uidGetMiddle()
{
    return HAL_GetUIDw1();
}
u32 JHAL_uidGetLow()
{
    return HAL_GetUIDw0();
}


