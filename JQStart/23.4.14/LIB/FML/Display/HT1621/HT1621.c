
/**
****************************HT1621.c******************************************
* @brief     LCD����������
*
  ******************************************************************************
* @file     HT1621.c
* @author   Jyen
* @version  v1.0
* @date     2024-01-17
* @attention
* This software is supplied under the terms of a license
* agreement or non-disclosure agreement.
* Passing on and copying of this document,and communication
* of its contents is not permitted without prior written
* authorization.
*
*         (C) Copyright 2024,Jyen,China. All Rights Reserved
********************************************************************************
*/
#include "ht1621.h"





#define HT1621_CMD_BIAS   0x52                //0b1000 0101 0010  1/3duty 4com

#define HT1621_CMD_SYSDIS 0X00           //0b1000 0000 0000  ����ϵͳ������LCDƫѹ������
#define HT1621_CMD_SYSEN  0X02           //0b1000 0000 0010 ��ϵͳ����

#define HT1621_CMD_LCDOFF 0X04           //0b1000 0000 0100  ��LCDƫѹ
#define HT1621_CMD_LCDON  0X06           //0b1000 0000 0110  ��LCDƫѹ

#define HT1621_CMD_XTAL   0x28           //0b1000 0010 1000 �ⲿ��ʱ��
#define HT1621_CMD_RC256  0X30           //0b1000 0011 0000  �ڲ�ʱ��

#define HT1621_CMD_TONEON  0X12           //0b1000 0001 0010  ���������
#define HT1621_CMD_TONEOFF 0X10           //0b1000 0001 0000 �ر��������

#define HT1621_CMD_WDTDIS  0X0A        //0b1000 0000 1010  ��ֹ���Ź�







/** ----------------------------ht1621WriteBits-----------------------------------
  * @������д����λ
  *
  * @������
**	  	 dev: [����]
**			 bits: ����
**			 length: bit����
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2024-01-17-------------------------------------- */

void ht1621WriteBits(HT1621 *dev,u8 bits,u8 length)
{

    for (u8 i=0; i<length; i++)
    {
        JHAL_gpioWitePin(dev->wrPort,dev->wrPin,false);
        JHAL_gpioWitePin(dev->dataPort,dev->dataPin,(bool)(bits & 0x80));
        JHAL_gpioWitePin(dev->wrPort,dev->wrPin,true);

        bits<<=1;
    }
}

/** ----------------------------ht1621WriteCmd-----------------------------------
  * @������ д����
  *
  * @������
**	  	 dev: [����]
**			 cmd: [����]
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2024-01-17-------------------------------------- */

void ht1621WriteCmd(HT1621 *dev,u8 cmd)
{
    JHAL_gpioWitePin(dev->csPort,dev->csPin,false);
    ht1621WriteBits(dev,0x80,4);        //д�������־100
    ht1621WriteBits(dev,cmd,8);                //д����������
    // ht1621WriteBits(dev,0,1);
    JHAL_gpioWitePin(dev->csPort,dev->csPin,true);
}

//��bit��ʾ ���֧������SGE
void ht1621ShowBits(HT1621 *dev,u8 segment,u8 offsetBitAddr,u8 bits,u8 bitLength)
{
    //���һ����ַ��31 ��ʱ��ֻ�ܵ���seg������
    u8 rMaxLength=segment<31?8:4;
    if(segment>31)
    {
        //���ֻ��31 ��0��ʼ��
        JHAL_Fault_Handler("ht1621ShowBits_segmentAddrOut");
    }
    u8 rdata=0;
    if(bitLength!=8)
    {
        if(bitLength+offsetBitAddr>rMaxLength)
        {
            //���ȳ���
            JHAL_Fault_Handler("ht1621ShowBits_offsetBitAddrOut");
        }
        //TODO
        JHAL_gpioModeSet(dev->dataPort,dev->dataPin,JHAL_IOMODE_IO_Input|JHAL_IOMODE_PullResistor_Down);
        JHAL_gpioWitePin(dev->csPort,dev->csPin,false);
        ht1621WriteBits( dev,0xC0,3);        //�����ݱ�־110
        ht1621WriteBits( dev,segment<<2,6);        //д���ַ����

//ѭ��������
        for(u8 i=0; i<rMaxLength; i++)
        {
            JHAL_gpioWitePin(dev->rdPort,dev->rdPin,false);
            rdata|=((u8)JHAL_gpioReadPin(dev->dataPort,dev->dataPin))<<(rMaxLength-i-1);
            JHAL_gpioWitePin(dev->rdPort,dev->rdPin,true);
        }
        JHAL_gpioWitePin(dev->csPort,dev->csPin,true);
        JHAL_gpioModeSet(dev->dataPort,dev->dataPin,JHAL_IOMODE_IO_OutputPushPull);

    }

    rdata&=~(0xFF>>(rMaxLength-bitLength)<<(rMaxLength-offsetBitAddr-bitLength)) ;
    rdata|=bits<<(rMaxLength- offsetBitAddr-bitLength);

    JHAL_gpioWitePin(dev->csPort,dev->csPin,false);
    ht1621WriteBits( dev,0xa0,3);        //д�����ݱ�־101
    ht1621WriteBits( dev,segment<<2,6);        //д���ַ����
    ht1621WriteBits(dev,rdata,rMaxLength);       //д������  ����SEG��4  �ܵ���8
    JHAL_gpioWitePin(dev->csPort,dev->csPin,true);


}
//���byte   ���������byte���� ��󲿷���Ҫʹ��ht1621ShowBits�ֿ�д
void ht1621ShowBytes(HT1621 *dev,u8 startSegment,u8 *data,u8 length)
{
    if(startSegment>30)
    {
        //���ֻ��31 ���һ��SGE �޷����� ��ʹ��ht1621ShowBits ����д��
        JHAL_Fault_Handler("ht1621ShowBytes");
    }

    JHAL_gpioWitePin(dev->csPort,dev->csPin,false);
    ht1621WriteBits( dev,0xa0,3);        //д�����ݱ�־101
    ht1621WriteBits( dev,startSegment<<2,6);        //д���ַ����
    for (u8 i=0; i<length; i++)
    {
        ht1621WriteBits(dev,*data,8);       //д������  ����SEG��4  �ܵ���8
        data++;
    }
    JHAL_gpioWitePin(dev->csPort,dev->csPin,true);
}



/* trueȫ��ʾ  falseȫ���*/
void ht1621ClearScreen (HT1621 *dev,bool level )
{
    for(u8 i=0; i<16; i++) {
        ht1621ShowBits(dev,i*2,0,level?0xff:0,8);
    }

}

void ht1621BuzzerConter (HT1621 *dev,bool isOpen )
{

    ht1621WriteCmd(dev,isOpen?HT1621_CMD_TONEON: HT1621_CMD_TONEOFF);

}


/** ----------------------------HT1621Open-----------------------------------
  * @�������򿪺�ȫ��
  *
  * @������
**	  	 dev: [����/��]
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2024-01-17-------------------------------------- */

void ht1621Open(HT1621 *dev)
{
    JHAL_gpioModeSet(dev->csPort,dev->csPin,JHAL_IOMODE_IO_OutputPushPull);
    JHAL_gpioModeSet(dev->wrPort,dev->wrPin,JHAL_IOMODE_IO_OutputPushPull);
    JHAL_gpioModeSet(dev->dataPort,dev->dataPin,JHAL_IOMODE_IO_OutputPushPull);
    JHAL_gpioModeSet(dev->rdPort,dev->rdPin,JHAL_IOMODE_IO_OutputPushPull);

    JHAL_gpioWitePin(dev->csPort,dev->csPin,true);
    JHAL_gpioWitePin(dev->wrPort,dev->wrPin,true);
    JHAL_gpioWitePin(dev->dataPort,dev->dataPin,true);
    JHAL_gpioWitePin(dev->rdPort,dev->rdPin,true);

    ht1621WriteCmd(dev,HT1621_CMD_BIAS);
    ht1621WriteCmd(dev,HT1621_CMD_RC256);            //ʹ���ڲ�����
    ht1621WriteCmd(dev,HT1621_CMD_SYSDIS);
    ht1621WriteCmd(dev,HT1621_CMD_WDTDIS);
    ht1621WriteCmd(dev,HT1621_CMD_SYSEN);
    ht1621WriteCmd(dev,HT1621_CMD_LCDON);

    ht1621ClearScreen(dev,true);
}







//������

//����Ϊ 0-F �� Ϩ�� - , .
u8  commonCathodeNixieTubeNumber[19]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x40,0x80};
//����Ϊʮ�����ƴ�С���� 0.-F.
u8  commonCathodeNixieTubeNumberPoint[16]= {0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0xf7,0xfc,0xb9,0xce,0xf9,0xf1};

//������
//����Ϊ 0-F �� Ϩ�� - , .
u8  commonAnodeNixieTubeNumber[19]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff,0xbf,0x7f};
//����Ϊʮ�����ƴ�С���� 0.-F.
u8   commonAnodeNixieTubeNumberPoint[16]= {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};





u8 __rearrangeBits(u8 inputByte, u8 order[]) {
    u8 result = 0;
    // ���ո�����˳����������λ
    for (u8 i = 0; i < 8; i++) {
        u8 bitPosition = order[i];
        u8 bitValue = (inputByte >> bitPosition) & 1;
        result |= (bitValue << i);
    }

// ��������ֽڰ��ո�λ��ǰ��������
    u8 tempResult = 0;
    for (u8 i = 0; i < 8; i++) {
        u8 bitValue = (result >> i) & 1;
        tempResult |= (bitValue << (7 - i));
    }

    result = tempResult;

    return result;
}
void ht1621_nixieTubeSort (u8   tubeNumberTable[],u8 tableLength, u8 order0, u8 order1,u8 order2,u8 order3,u8 order4,u8 order5,u8 order6,u8 order7)
{
    u8  customOrder[] = {order0, order1, order2, order3, order4, order5, order6, order7};
    for (u8 i=0; i<tableLength; i++)
    {
        tubeNumberTable[i]=__rearrangeBits(tubeNumberTable[i],customOrder);
    }
}


