#include "..\JHAL_Flash.h"
#include <xl_sim.h>
#include <xl_flash.h>
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




#if defined(XL6600A402L6)

//������С1K
#define FlashPageSize FLASH_SECTOR_SIZE
//128K
#define FlashMaxSize  128*FlashPageSize

#define FlashStartAddr FLASH_START_ADDRESS

#elif defined (KF32A140INP)
//128K
#define FlashMaxSize 128*1024
#else
��ҪΪ���ͺŵ�Ƭ��ָ��falsh��С
#endif

#define FlashEndAddr (FlashStartAddr+FlashMaxSize)



//flash��Ҳ��� �ҵ����ݾ�Ҳ����ҳ�� �����һҳ���
u32 __JHAL_flashPage2Addr(u16 page)
{   //flash��С����
    while(page>(FlashMaxSize/FlashPageSize)||page==0);

    return	FlashEndAddr-page*FlashPageSize;
}

bool __JHAL_flashGetCmdFinshFlag(void)
{
    if((*(volatile uint32_t*)0x40020000 & FMC_FSTAT_CCIF_MASK) == FMC_FSTAT_CCIF_MASK)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool __JHAL_flashErasePage(const uint32_t targetaddress )
{
    uint32_t timeout=0xfffff;
    bool isOK=true;
    if( FLASH_ERR_SUCCESS!=FLASH_EraseSector(targetaddress))  //��������
    {

        return false;
    }
    JHAL_disableInterrupts();

    //FLASH_LaunchCMD();	//����ִ��
    //FMC->FSTAT |= 0x80000000u;
    *(volatile uint32_t*)0x40020000 |= 0x80000000u;
    while(!__JHAL_flashGetCmdFinshFlag())
    {
        timeout--;
        if(timeout==0)
        {
            isOK = false;
            break;
        }
    }
    JHAL_enableInterrupts();
    return isOK;
}



bool __JHAL_flashWrite64Bit(const uint32_t TargetAddress, const uint32_t DwData0, const uint32_t DwData1)
{
    uint32_t timeout=0xfffff;
    bool isOK=true;


    if(FLASH_ERR_SUCCESS != FLASH_Program2LongWord( TargetAddress, DwData0, DwData1))//flashд8byte
    {
        return false;
    }
    else
    {
        JHAL_disableInterrupts();
        //FLASH_LaunchCMD();
        *(volatile uint32_t*)0x40020000 |= 0x80000000u;
        while(!__JHAL_flashGetCmdFinshFlag())
        {
            timeout--;
            if(timeout==0)
            {
                isOK=false;
                break;
            }
        }
        JHAL_enableInterrupts();
    }
    return isOK;
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
    uint32_t DwData0=(uint32_t) (*((uint32_t*)(p_FlashBuffer)));
    uint32_t DwData1= (uint32_t)(*(((uint32_t*)(p_FlashBuffer))+1));
//����û��������FF���� �Կռ任ʱ��
//��Ϊ�ڲ���const ��������ʱ��������ʱ�������뵱ָ��Խ����ܻᵼ��Ӳ������

    return  __JHAL_flashWrite64Bit(address,DwData0,DwData1);
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
    for(u16 i=0; i<leng; i++) //һҳ1024byte������һ��д64bit=8byte��128�������
    {
        if(!  __JHAL_flashWrite8Byte(address,p_FlashBuffer))
        {
            return false;
        }
        p_FlashBuffer+=8;
        address+=8;
    }
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
