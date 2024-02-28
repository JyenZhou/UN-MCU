 #ifdef JHAL_FlashStartAddr

//flash��Ҳ��� �ҵ����ݾ�Ҳ����ҳ�� �����һҳ���
u32 __JHAL_flashPage2Addr(u16 page)
{

    //flash��С����
    while(page>(JHAL_FlashMaxSize/JHAL_FlashPageSize)||page==0);

    return	JHAL_FlashEndAddr-page*JHAL_FlashPageSize;
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

bool JHAL_flashErasePages(uint32_t startPageAddr, uint32_t endAddr)
{
	   
    //�жϿ�ʼ��ַ�Ƿ����������� ��������
    if((startPageAddr%JHAL_FlashPageSize)!=0)
		{
				JHAL_error("JHAL_flashErasePages_Unaligned");
		}
    //��ַ����
    if(endAddr>JHAL_FlashEndAddr)
		{
			JHAL_error("JHAL_flashErasePages_AddrOver");
		}
     
    u8 missionsRetriedCount=10;
    do {

        for (u32 i = startPageAddr; i <= endAddr; i += JHAL_FlashPageSize)
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
        while((length+8>JHAL_FlashPageSize));
        u32 startAddr=__JHAL_flashPage2Addr(page);
        if(!JHAL_flashErasePages(startAddr,startAddr+length+8))
        {
            continue;
        }
        u32  flashCrc=JHAL_crc(JHAL_CRC_Mode_16_Modbus,data, length);
        if(!JHAL_flashWriteNByte(startAddr,(u8*)&flashCrc,8))
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
    while((length+8>JHAL_FlashPageSize));
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



#endif


//96λUID�൱��3��32bitλ����  ÿ8bitλ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ� 
void uid2string(char* string,int buffSize )
{

    snprintf(string, buffSize, "%X%X%X",  JHAL_uidGetLow(), JHAL_uidGetMiddle(),JHAL_uidGetHigh());
 
 
}


