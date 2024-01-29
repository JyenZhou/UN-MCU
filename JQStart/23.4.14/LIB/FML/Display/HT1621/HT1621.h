#ifndef __HT1621_H
#define __HT1621_H

#include "JHAL.h"



typedef struct
{

    JHAL_IO_Port  csPort ;
    u8  csPin;
    JHAL_IO_Port  wrPort ;
    u8  wrPin;
    JHAL_IO_Port  rdPort ;
    u8  rdPin;
    JHAL_IO_Port  dataPort ;
    u8  dataPin;


} HT1621;



void ht1621ShowBits(HT1621 *dev,u8 segment,u8 offsetBitAddr,u8 bits,u8 bitLength);
void ht1621ShowBytes(HT1621 *dev,u8 startSegment,u8 *data,u8 length);
void ht1621Open(HT1621 *dev);
void ht1621ClearScreen (HT1621 *dev,bool level );
void ht1621BuzzerConter (HT1621 *dev,bool isOpen );







/***8λ����ܹ��ߺ���  8λ��������ǵ�λ��ǰ���������  ���Ƕ��������������ĺ�ht1621��˳����ܲ�һ�� �����ṩ��������� ********/


//������

//����Ϊ 0-F �� Ϩ�� - , .
extern u8  commonCathodeNixieTubeNumber[19];
//����Ϊʮ�����ƴ�С���� 0.-F.
extern u8  commonCathodeNixieTubeNumberPoint[16] ;

//������
//����Ϊ 0-F �� Ϩ�� - , .
extern u8  commonAnodeNixieTubeNumber[19] ;
//����Ϊʮ�����ƴ�С���� 0.-F.
extern u8   commonAnodeNixieTubeNumberPoint[16] ;

/** ----------------------------ht1621_nixieTubeSort-----------------------------------
  * @��������ȡģ����еı�׼���������ת���ɶ��ƶ������е�8λ����ܵ���ģ
  *
  * @������
**	  	 tubeNumberTable: [����] ��Ҫ�������ģ��  �����ṩ�˹��������ĳ���ģ��
**			 tableLength: [����] ��ģ����
**			 order0-7: [����]  ht1621��Ҫ��ʵ��λ��
    SEGX ˳��Ϊ AFED (dp)BGC  �򴫽�����ֵΪ {0, 5, 4, 3, 7, 1, 6, 2} ����A�൱��0;

  *
  * @����ֵ:��
  * @ע:
  *-----------------------------Jyen-2024-01-24-------------------------------------- */

void ht1621_nixieTubeSort (u8 * tubeNumberTable,u8 tableLength, u8 order0, u8 order1,u8 order2,u8 order3,u8 order4,u8 order5,u8 order6,u8 order7);






#endif


