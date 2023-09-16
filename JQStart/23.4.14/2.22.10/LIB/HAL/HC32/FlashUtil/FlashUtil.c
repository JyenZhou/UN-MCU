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



#include "flashUtil.h"


uint32 Robot_Num_Flash_Add=0;

/*******************************************************************************
* FLASH �жϷ���ص�����
******************************************************************************/
static volatile uint32_t u32FlashTestFlag   = 0;
void FlashCallback(void)
{
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);
        u32FlashTestFlag |= 0x01;
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        u32FlashTestFlag |= 0x02;
        Flash_DisableIrq(flash_int1);
    }

}

void flashUtilInit()
{
    Flash_Init(FlashCallback, 0);
//����flash��ַ �������С����������512�ı���ǿ�н�һλʹ��Ϊ521�ı���
    uint32 f=CodeSize/521;
    Robot_Num_Flash_Add=521*(f+1);

//��֪��Ϊʲô��Ҫ+8  ���Ǵ��ڴ��п������� �Ʋ��
    Robot_Num_Flash_Add+=8;

}


//д��ĵ�ַ��ȥ���˴����С����ʼ��ַ�ģ���д������,(FLASH_TYPEPROGRAM_HALFWORD,FLASH_TYPEPROGRAM_WORD,FLASH_TYPEPROGRAM_DOUBLEWORD)д�����ݣ�д��ĳ���
en_result_t wirteFlashWORD(uint32  writeFlashAddr,uint32 *writeFlashData,u16 NumToWrite,uint8 isSectorErase)
{
    en_result_t       enResult = Error;
    uint16 i;

    writeFlashAddr+= Robot_Num_Flash_Add;

    for(  i=0; i<NumToWrite; i++)
    {
//����512���ֽ�Ҫ�ٲ���Ҫд����һ����

        if(isSectorErase)
        {
            //512/4=128 Ҳ����˵129��ʱҪ�����µĵط���
            if(i%128==0)
            {
                Flash_SectorErase(writeFlashAddr);
            }
        }
//�ȶ�aȡ��ַ��Ȼ��������ַǿת��uint32_t *���ͣ��ٽ����ã���ʱ�������ᰴ�����εĹ���ȥ��������ַ�����ݣ�
//��ҪΪ���ܹ���ȡfloat���͵�ֵ ����ֱ��д Flash_WriteWord(writeFlashAddr,writeFlashData[i]);
        enResult = Flash_WriteWord(writeFlashAddr,*(volatile uint32*)&writeFlashData[i]);
        if (Ok != enResult)
        {
            return enResult;
        }
        //�ֽ�-->uint32
        writeFlashAddr+=4;//��ַ����4.

    }
    enResult=Ok;
    return enResult;
}



//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ȡ��ַ ��д���һ��ȥ������ʼ�ʹ����С
//pBuffer:���ݽ��ջ���
//NumToRead: ��ȡ����
void STMFLASH_Read_HALFWORD(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for( i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu16*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=2;//ƫ��2���ֽ�.
    }
}



void STMFLASH_Read_WORD(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for(i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu32*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=4;//ƫ��2���ֽ�.
    }
}
void STMFLASH_Read_WORD_Float(u32 ReadAddr,float *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for(i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile float*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=4;//ƫ��2���ֽ�.
    }
}
void STMFLASH_Read_DOUBLEWORD(u32 ReadAddr,u64 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for( i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu64*)ReadAddr;//��ȡ2���ֽ�.

        ReadAddr+=4;//ƫ��2���ֽ�.
    }
}

/*��С������*/
//ע�����¼���
//1.Flash�ڴ淶Χ�� 0x0800 0000 ~ 0x0802 0000  ��
//2.Ҫע��MCUһҳ�Ĵ�С��һЩMCUһҳ��1KB��һЩMCUһҳ��2KB
//3.����Flashд��ĵ�ַ������ż���� ����ǲ���16λ����,��ô����ʱ���밴ż����ַ���롣 ����Ͳ�һ�������������ơ�����Դ���Flash���ж������ǿ��Զ����ַ���е��ֽڶ�ȡ�ġ���
//void test()
//{
//		float datai[5]={3.141592654,0.618,0x99,0x1000};
//	float rdatai[5];
//		//����Ҫע��д������//FLASH_TYPEPROGRAM_WORD
//  //  wirteFlash(0x00,FLASH_TYPEPROGRAM_WORD,(uint32*)datai,4);
////HAL_Delay(1000);
////    STMFLASH_Read_WORD(0x00,(uint32*)rdatai,5);
//}


