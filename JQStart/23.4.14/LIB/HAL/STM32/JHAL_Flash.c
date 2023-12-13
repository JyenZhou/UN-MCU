#include "..\JHAL_Flash.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		FlashĬ��ͨ��HAL����  ע���װ�ڲ��Ѿ����˵�ַƫ��   ����ȥ��00��ַ����falsh�����׵�ַ
 		����Ҫע���С��Ҫ����  ��Ȼ������Ҳ�з�������

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#define FLASH_WAITETIME 50000       //FLASH�ȴ���ʱʱ�� 


#define FlashStartAddr FLASH_BASE
#if defined STM32F103xE   || defined STM32F103xB

//������С1K
#define FlashPageSize 0x400
//256K xe�Ĵ�С��256-512 ����С�ļ����Ժ� һ����˵������  ����������ټӸ����嶨��ĺ궨��
#define FlashPageNumber 256
#define FlashMaxSize  (*((uint16_t *)FLASH_SIZE_DATA_REGISTER))


#elif defined (STM32F407xx)
//16K
#define FlashPageSize 0x4000
#define FlashPageNumber 10
#define FlashMaxSize  (*((uint16_t *)0x1FFF7A22))

#else
#error δ����flash��С
#endif



#define FlashEndAddr (FlashStartAddr+FlashMaxSize)


uint16_t getFlashSize()
{


    return FlashMaxSize;

}



//flash��Ҳ��� �ҵ����ݾ�Ҳ����ҳ�� �����һҳ���
u32 __JHAL_flashPage2Addr(u16 page)
{

    //flash��С����
    while(page>(FlashMaxSize/FlashPageSize)||page==0);

    return	FlashEndAddr-page*FlashPageSize;
}




bool __JHAL_flashErasePage(const uint32_t targetaddress )
{



    //1������FLASH

    //2������FLASH
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

        return false;
    }




    return   FLASH_WaitForLastOperation(FLASH_WAITETIME)==HAL_OK; //�ȴ��ϴβ������
}







u32 JHAL_flashGetFlashPageSize()
{
    return FlashPageSize;
}


u32 JHAL_flashGetEndAddr()
{
    return FlashEndAddr;
}


/*------------------Jyen--------------------------Jyen-----------------------*/
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

bool JHAL_flashErasePage(uint32_t startPageAddr, uint32_t endAddr)
{
    JHAL_disableInterrupts();
    HAL_FLASH_Unlock();

    //�жϿ�ʼ��ַ�Ƿ����������� ��������
    while((startPageAddr%FlashPageSize)!=0);
    //��ַ����
    while(endAddr>FlashEndAddr);
    //TODO ��ҳ������һ��ȫ�־��淽��debug
    u8 missionsRetriedCount=10;
    do {

        for (u32 i = startPageAddr; i <= endAddr; i += FlashPageSize)
        {
            if(! __JHAL_flashErasePage(i))//������ҳ��
            {
                continue;
            }
        }
        break;
    } while(--missionsRetriedCount!=0);

    HAL_FLASH_Lock();
    JHAL_enableInterrupts();


    if(missionsRetriedCount>0)
    {
        return true;
    } else {
        return false;
    }

}




/**
  * ����  flash ������д��������
  * ����  address�� ָ��flash��ַ
  *  p_FlashBuffer: д�����ݵ�ָ��
  *
  * ����  �ޡ�
  * ע��д��ǰҪ�Ȳ�����ַ���ڵ�ҳ  8�ֽڶ���(uint64)  д��
  */
bool __JHAL_flashWrite8Byte(uint32_t address,void* p_FlashBuffer)
{

    if(HAL_OK==  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,address,* (u64_t*)p_FlashBuffer))
    {
        return true;
    }

    return false;

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

    for(u16 i=0; i<leng; i++) //һҳ1024byte������һ��д64bit=8byte��128�������
    {
        if(!  __JHAL_flashWrite8Byte(address,p_FlashBuffer))
        {
            return false;
        }
        p_FlashBuffer+=8;
        address+=8;
    }
    JHAL_enableInterrupts();
    return true;
}
/*дfalsh
startAddr ��ʼ��ַ
data �����ǽṹ��
size  ���ݴ�С���ٸ��ֽ�(u8)
isbefoErase �Ƿ���д��ǰ���� ��Ҫע���������ҳ���� ��ЩMCU��Ҫ�������׵�ַ���ܲ���
length ���ݳ���  ע�����������д��Ҫע�ⱻ���Ǽ���ǰ���õĳ�����1-3  ���涼Ҫ�Ǵ�4��32λ���룩+4�ӣ�crc������8��ʼ
���Դ���10  ����󷵻�false
 */

bool JHAL_flashWirte(u32 page,void *data,u16 length)
{
    u8 missionsRetriedCount=10;
    JHAL_disableInterrupts();
    do
    {
        //CRCռ��1��   //��ʱֻ֧��һҳ����
        while((length+8>FlashPageSize));
        u32 startAddr=__JHAL_flashPage2Addr(page);
        if(!JHAL_flashErasePage(startAddr,startAddr+length+8))
        {
            continue;
        }
        u32  flashCrc=JHAL_crc(JHAL_CRC_Mode_16_Modbus,data, length);
        if(!__JHAL_flashWrite8Byte(startAddr,&flashCrc))
        {
            continue;
        }
        if(!JHAL_flashWriteNByte(startAddr+8,data,length))
        {
            continue;
        }

        if(flashCrc!=JHAL_crc(JHAL_CRC_Mode_16_Modbus,(u8 *)startAddr+8, length))
        {
            continue;
        }
        break;
    } while(--missionsRetriedCount!=0);
    JHAL_enableInterrupts();
    if(missionsRetriedCount>0)
    {
        return true;
    } else {
        return false;
    }

}



//Ĭ��ͨ��ָ��ֱ�Ӷ� ������ݲ�ͬ��Ƭ��

bool JHAL_flashRead(u32 page,void *data,u16 length)
{
    //CRCռ��1��
    while((length+8>FlashPageSize));
    u32 startAddr=__JHAL_flashPage2Addr(page);
    u32  flashCrc=*((u32 *)startAddr);
    startAddr+=8;

    uint16 crc= JHAL_crc(JHAL_CRC_Mode_16_Modbus,(u8 *)startAddr,length);
    if(flashCrc ==crc)
    {
        for(u16 i=0; i<length; i++)
        {
            ((u8*)data)[i]= *((u8 *)startAddr++);
        }

        return true;
    } else {
        return false;

    }
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

