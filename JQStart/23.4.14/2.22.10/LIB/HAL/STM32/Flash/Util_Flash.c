/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*             ʹ�÷�����������Գ���                                                        *
            STM32�ڲ�flashģ��EEPROM��д
       ���ܳ���汾����  Ҫ�ֶ� ������� void FLASH_PageErase(uint32_t PageAddress);
			 ��  #include "stm32f1xx_hal_flash_ex.h" ��
*                                                                   *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/



#include "FlashSimulationEeprom.h"


//д��ĵ�ַ��ȥ���˴����С����ʼ��ַ�ģ���д������,(FLASH_TYPEPROGRAM_HALFWORD,FLASH_TYPEPROGRAM_WORD,FLASH_TYPEPROGRAM_DOUBLEWORD)д�����ݣ�д��ĳ���
void wirteFlash(uint32  writeFlashAddr,uint32_t TypeProgram,uint32 *writeFlashData,u16 NumToWrite)
{

    writeFlashAddr+= Robot_Num_Flash_Add;
    //1������FLASH
    HAL_FLASH_Unlock();
    //2������FLASH
    //��ʼ��FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;//����Flashִ��ҳ��ֻ����������
    f.PageAddress = writeFlashAddr;//����Ҫ�����ĵ�ַ
    f.NbPages = 1; //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ  һҳ1K��С
    //����PageError
    uint32_t PageError = 0;
    //���ò�������
    HAL_FLASHEx_Erase(&f, &PageError);

    //3����FLASH��д	for(i=0;i<NumToWrite;i++)
    for(uint16 i=0; i<NumToWrite; i++)
    {
        HAL_FLASH_Program(TypeProgram,writeFlashAddr,writeFlashData[i]);

        if(TypeProgram==FLASH_TYPEPROGRAM_HALFWORD)
        {   //���ֽ�-->uint16

            writeFlashAddr+=2;//��ַ����2.
        } else if(TypeProgram==FLASH_TYPEPROGRAM_WORD)
        {   //�ֽ�-->uint32
            writeFlashAddr+=4;//��ַ����4.
        } else if(TypeProgram==FLASH_TYPEPROGRAM_DOUBLEWORD)
        {   //���ֽ�-->uint64
            writeFlashAddr+=8;
        }
    }
    //4����סFLASH
    HAL_FLASH_Lock();
}



//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ȡ��ַ ��д���һ��ȥ������ʼ�ʹ����С
//pBuffer:���ݽ��ջ���
//NumToRead: ��ȡ����
void STMFLASH_Read_HALFWORD(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{   ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu16*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=2;//ƫ��2���ֽ�.
    }
}
void STMFLASH_Read_WORD(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)
{
    ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu32*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=4;//ƫ��2���ֽ�.
    }
}
void STMFLASH_Read_DOUBLEWORD(u32 ReadAddr,u64 *pBuffer,u16 NumToRead)
{
    ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu64*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=4;//ƫ��2���ֽ�.
    }
}

/*��С������*/
//ע�����¼���
//1.Flash�ڴ淶Χ�� 0x0800 0000 ~ 0x0802 0000  ��
//2.Ҫע��MCUһҳ�Ĵ�С��һЩMCUһҳ��1KB��һЩMCUһҳ��2KB
//3.����Flashд��ĵ�ַ������ż���� ����ǲ���16λ����,��ô����ʱ���밴ż����ַ���롣 ����Ͳ�һ�������������ơ�����Դ���Flash���ж������ǿ��Զ����ַ���е��ֽڶ�ȡ�ġ���
void test()
{
		float datai[5]={3.141592654,0.618,0x99,0x1000};
	float rdatai[5];
		//����Ҫע��д������//FLASH_TYPEPROGRAM_WORD
    wirteFlash(0x00,FLASH_TYPEPROGRAM_WORD,(uint32*)datai,4);
    HAL_Delay(1000);
    STMFLASH_Read_WORD(0x00,(uint32*)rdatai,5);
}
