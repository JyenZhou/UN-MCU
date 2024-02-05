#include "../JHAL_Flash.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		FlashĬ��ͨ��HAL����  ע���װ�ڲ��Ѿ����˵�ַƫ��   ����ȥ��00��ַ����falsh�����׵�ַ
 		����Ҫע���С��Ҫ����  ��Ȼ������Ҳ�з������ ע���õ����Զ���������ֵ��ֲ��ͬ��Ƭ��ʱ����Ҫ�޸��Զ����ɵ�ֵ��������Ŀģ��Ҳ�����Զ�������������

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

 
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

        if((startPageAddr%EFLASH_PAGE_SIZE)!=0)
        {
//�жϵ�ַ�Ƿ����������� ��������
            while(true);
        }
startPageAddr+=EFLASH_BASE_ADDRESS;
endAddr+=EFLASH_BASE_ADDRESS;
if(endAddr>EFLASH_END_ADDRESS)
{
//��С����
while(1);
}



    uint32_t i;
    EFLASH_StatusType FLASHStatus = EFLASH_STATUS_SUCCESS;
    __set_PRIMASK(1);

    EFLASH_UnlockCtrl();
    for (i = startPageAddr; i <= endAddr; i += EFLASH_PAGE_SIZE)
    {
        FLASHStatus = EFLASH_PageErase(i);

        if (FLASHStatus != EFLASH_STATUS_SUCCESS)
        {
            EFLASH_LockCtrl();
            return	(bool)(FLASHStatus==EFLASH_STATUS_SUCCESS);
        }
    }
    EFLASH_LockCtrl();
    __set_PRIMASK(0);
    return (bool)(FLASHStatus==EFLASH_STATUS_SUCCESS);

}


/*дfalsh
startAddr ��ʼ��ַ
data �����ǽṹ��
size  ���ݴ�С���ٸ��ֽ�(u8)
isbefoErase �Ƿ���д��ǰ���� ��Ҫע���������ҳ���� ��ЩMCU��Ҫ�������׵�ַ���ܲ���
 */
void JHAL_flashWirte(u32 startAddr,u32 *data,u16 length,bool isbefoErase)
{
	if((((length+1)*4)+startAddr)>EFLASH_SIZE)
	{

		//flash��С����
		while(1);

	}

startAddr+=EFLASH_BASE_ADDRESS;
 


    if(isbefoErase)
    {

        JHAL_flashErasePage(startAddr,startAddr+length);

    }
    u32  flashCrc=crc16_Modbus((u8*)data, length);
//  flash����
    __set_PRIMASK(1);
    EFLASH_UnlockCtrl();

    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_WaitForLastOperation(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageErase(startAddr);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageProgram(startAddr, &flashCrc, 1);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageProgram(startAddr+4, data,length/ 4);
    EFLASH_LockCtrl();
    __set_PRIMASK(0);
}






bool JHAL_flashRead(u32 startAddr,u32 *data,u16 length)
{
	if((((length+1)*4)+startAddr)>EFLASH_SIZE)
	{

		//flash��С����
		while(1);

	}
startAddr+=EFLASH_BASE_ADDRESS;
    u32  flashCrc=0;
    EFLASH_Read(startAddr,  &flashCrc, 1);
    EFLASH_Read(startAddr+4, data, length/ 4);
    uint16 crc= crc16_Modbus((u8*)data,length);
    if(flashCrc ==crc)
    {

        return true;
    } else {
        return false;

    }
}

