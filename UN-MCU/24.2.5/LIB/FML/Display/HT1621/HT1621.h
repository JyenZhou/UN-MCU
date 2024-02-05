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



void ht1621DisplayBits(HT1621 *dev,u8 segment,u8 offsetBitAddr,u8 bits,u8 bitLength);
void ht1621DisplayBytes(HT1621 *dev,u8 startSegment,u8 *data,u8 length);
void ht1621Open(HT1621 *dev);
void ht1621ClearScreen (HT1621 *dev,bool level );
void ht1621BuzzerConter (HT1621 *dev,bool isOpen );







/***8位数码管工具函数  8位数码管中是低位在前的且有序的  但是断码屏厂家生产的和ht1621中顺序可能不一致 这里提供相关排序函数 ********/


//共阴极

//内容为 0-F 和 熄灭 - , .
extern u8  commonCathodeNixieTubeNumber[19];
//内容为十六进制带小数点 0.-F.
extern u8  commonCathodeNixieTubeNumberPoint[16] ;

//共阳极
//内容为 0-F 和 熄灭 - , .
extern u8  commonAnodeNixieTubeNumber[19] ;
//内容为十六进制带小数点 0.-F.
extern u8   commonAnodeNixieTubeNumberPoint[16] ;

/** ----------------------------ht1621_nixieTubeSort-----------------------------------
  * @描述：将取模软件中的标准数码管数字转化成定制断码屏中的8位数码管的字模
  *
  * @参数：
**	  	 tubeNumberTable: [输入] 需要排序的字模表  上面提供了共阴共阳的常用模板
**			 tableLength: [输入] 字模表长度
**			 order0-7: [输入]  ht1621需要的实际位置
    SEGX 顺序为 AFED (dp)BGC  则传进来的值为 {0, 5, 4, 3, 7, 1, 6, 2} 其中A相当于0;

  *
  * @返回值:无
  * @注:
  *-----------------------------Jyen-2024-01-24-------------------------------------- */

void ht1621_nixieTubeSort (u8 * tubeNumberTable,u8 tableLength, u8 order0, u8 order1,u8 order2,u8 order3,u8 order4,u8 order5,u8 order6,u8 order7);






#endif


