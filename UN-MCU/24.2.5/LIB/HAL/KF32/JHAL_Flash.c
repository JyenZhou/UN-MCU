#include "../JHAL_Flash.h"

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


//������С1K
#define PageSize 1024

#define FlashStartAddr 0x00000000

#if defined(KF32A151MQV)
//512K
#define FlashMaxSize 0x80000

#elif defined (KF32A140INP)
//128K
#define FlashMaxSize 0x20000
#else
��ҪΪ���ͺŵ�Ƭ��ָ��falsh��С
#endif

#define FlashEndAddr (FlashStartAddr+FlashMaxSize)




//flash��Ҳ��� �ҵ����ݾ�Ҳ����ҳ�� �����һҳ���
u32 page2Addr(u8 page)
{
	if(page>FlashMaxSize/PageSize||page==0)
	{
		//flash��С����
		while(1);
	}

return	FlashEndAddr-page*PageSize;
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
	//�жϵ�ַ�Ƿ����������� ��������
	  while((startPageAddr%PageSize)!=0);

	if(endAddr>FlashEndAddr)
	{
		//flash��С����
		while(1);

	}

	INT_All_Enable(FALSE);
    for (u32 i = startPageAddr; i <= endAddr; i += PageSize)
    {
         FLASH_Wipe_Configuration_RAM(FLASH_WIPE_CODE_PAGE,i);//������ҳ��
    }

    INT_All_Enable(TRUE);
    return true;

}




/**
  * ����  flash ������д��������
  * ����  address�� ָ��flash��ַ
  *  p_FlashBuffer: д�����ݵ�ָ��
  *
  * ����  �ޡ�
  * ע��д��ǰҪ�Ȳ�����ַ���ڵ�ҳ  8�ֽڶ���  д��
  */
void JHAL_Write8Byte(uint32_t address,void* p_FlashBuffer)
{
	u32 baseAddr=address%16;
	u32 offsetAddr=address;
		//--------------------------------------------------------------------------------------------
		FLASH_ProgramTypeDef flash_write;
	    flash_write.m_Mode=FLASH_PROGRAM_WORD;//FLASH_PROGRAM_HALF_PAGE //FLASH_PROGRAM_WORD
		flash_write.m_Zone=FLASH_PROGRAM_CODE;//FLASH_PROGRAM_CFG  FLASH_PROGRAM_CODE
		flash_write.m_Addr=offsetAddr;
		flash_write.m_WriteSize=1;//��ҳ��̲���Ϊ0�����д1��+1��ʵ��д����2��˫�� ���ֱ����Ӱ��
		flash_write.m_Data=p_FlashBuffer;
		INT_All_Enable(FALSE);
		FLASH_Program_Configuration_RAM(&flash_write);
		INT_All_Enable(TRUE);
}
/**
  * ����  flash ������д����ֽ�����
  * ����  address�� ָ��flash��ַ
  *  p_FlashBuffer: д�����ݵ�ָ��
  *    leng�����ݵĳ���
  * ����  �ޡ�
  * ע��д��ǰҪ�Ȳ�����ַ���ڵ�ҳ //����Ĳ���8���ֽڰ��ĸ�ֱ�Ӵ���  ��Ϊ�Ƕ��������Ժ���ָ�볬��û��  �����´�д���ַҪ����8
  */
void FLASH_WriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng)//��ַ����Ϊ��8����
{
	leng=(leng/8)+(leng%8!=0);
	for(u16 i=0;i<leng;i++)//һҳ1024byte������һ��д64bit=8byte��128�������
	{
		JHAL_Write8Byte(address,p_FlashBuffer);//
		p_FlashBuffer+=8;
		address+=8;
	}
}
/*дfalsh
startAddr ��ʼ��ַ
data �����ǽṹ��
size  ���ݴ�С���ٸ��ֽ�(u8)
isbefoErase �Ƿ���д��ǰ���� ��Ҫע���������ҳ���� ��ЩMCU��Ҫ�������׵�ַ���ܲ���
length ���ݳ���  ע�����������д��Ҫע�ⱻ���Ǽ���ǰ���õĳ�����1-3  ���涼Ҫ�Ǵ�4��32λ���룩+4�ӣ�crc������8��ʼ
 */

bool JHAL_flashWirte(u32 page,void *data,u16 length,bool isbefoErase)
{
	u8 missionsRetriedCount=3;
	do
	{
	//CRCռ��1��
	if(length+1>PageSize)
	{
		//��ʱֻ֧��һҳ����
				while(1);
	}
	u32 startAddr=page2Addr(page);
    if(isbefoErase)
    {
        JHAL_flashErasePage(startAddr,startAddr+length);
    }


    u32  flashCrc=crc16_Modbus(data, length+1);
    JHAL_Write8Byte(startAddr,&flashCrc);

	FLASH_WriteNByte(startAddr+8,data,length);//

	if(flashCrc==crc16_Modbus((u8 *)startAddr+8, length+1))
	{
	return true;
	}

	}while(missionsRetriedCount--!=0);

	return false;
}

/**
  * ����  flash �����������ֽ�����
  * ����  address�� ָ��flash��ַ
  *  p_FlashBuffer: д�����ݵ�ָ��
  *
  * ����  �ޡ�
  */
void FLASH_READCODE_fun(uint32_t address,uint32_t *p_FlashBuffer,uint32_t length)
{

	volatile uint32_t read_num;

	for(read_num=0;read_num<length;read_num++)
	{
		*p_FlashBuffer=Read_Flash_or_CFR_RAM(address,FLASH_PROGRAM_CODE);
		p_FlashBuffer++;
		address=address+0x0004;
	}
}



bool JHAL_flashRead(u32 page,void *data,u16 length)
{
	//CRCռ��1��

	if(length*4+1>PageSize)
	{
		//��ʱֻ֧��һҳ����
				while(1);
	}
	u32 startAddr=page2Addr(page);



/*
    u32  flashCrc=0;
    FLASH_READCODE_fun(startAddr,  &flashCrc, 1);
    FLASH_READCODE_fun(startAddr+=4, data, length/4);
    u8 overlength=length%4;
    if(overlength!=0)
    {
    	u8 databuff[4];
    	 FLASH_READCODE_fun(startAddr+length-overlength, (u32*)databuff,1 );

    	 for(u8 i=0;i<overlength;i++)
    	 {
    		 ((u8*)data)[length-overlength+i]=databuff[i];
    	 }
    }
    uint16 crc= crc16_Modbus(data,length+1);

    */
	u32  flashCrc=*((u32 *)startAddr);
	startAddr+=8;

for(u16 i=0;i<length;i++)
{
((u8*)data)[i]= *((u8 *)startAddr++);
}


uint16 crc= crc16_Modbus(data,length+1);
    if(flashCrc ==crc)
    {

        return true;
    } else {
        return false;

    }
}

