/**
  ****************************JHAL_NumberConverter.c******************************************
  * @brief     ������ֵת��
  *
    ******************************************************************************
  * @file     JHAL_NumberConverter.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-11-22
  * @attention �ʼǣ�����������εĿ���strcatƴ�� char* �Ĳ��ܵ���strcatƴ��
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
#include "JHAL_NumberConverter.h"
#include <string.h>
#include <ctype.h>
/*ȥ���ַ��������пո�*/
void JHAL_stringRemoveBlank(char *ch)
{
    char *c;//ָ���ַ������ַ���
    while (*ch != ' ')
    {
        if (*ch == '\0')
        {
            //�ַ�����û�пո�,ֱ�ӷ���
            return;
        }
        ch++;
    }//��blankָ��ָ���һ���ո�λ��
    //��cָ��ָ����ֿո��ĵ�һ���ַ���
    c = ch + 1;
    while (*c == ' ')
    {
        c++;
    }
    while (*c != '\0')
    {
        if (*c != ' ')
        {
            //���ո�Ϊ������ֵ��ַ�������ȥ���ַ���Ϊ�ո�
            *ch = *c;
            *c = ' ';
            ch++;
        }
        c++;
    }
    *ch = '\0';
    return;
}
/** ----------------------------JHAL_hexString2Number-----------------------------------
  * @������int���ַ���תint��atoi���� ����16���Ƶ�ȴû��
  *
  * @������
**	  	 str: [����16�����ַ�����"A8FF"]
  *
  * @����ֵ:��
  * @ע:�ո����Զ�ȥ��
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

int JHAL_hexString2Number( char * str)
{
    //����¼��16���Ƶ�MAC��ַ
    int value = 0;
    int sign = 1;
    int radix = 16;

    if(*str == '-')
    {
        sign = -1;
        str++;
    }
    JHAL_stringRemoveBlank(str);
    while(*str)
    {
        if(radix == 16)
        {
            if(*str >= '0' && *str <= '9')
                value = value * radix + *str - '0';
            //0��9���ַ���
            else if(*str >= 'A' && *str <= 'F')
                value = value * radix + *str - 'A' + 10;
            //��дʮ�����Ƶ�ABCDEF���ַ��������
            else if(*str >= 'a' && *str <= 'f')
                value = value * radix + *str - 'a' + 10;
            // Сдʮ�����Ƶ�abcdef���ַ��������
            else
                return -1; //����16���Ʋ�����MAC��ַ�ĸ�ʽ
        }
        else
            value = value * radix + *str - '0';
        str++;
    }
    return sign*value;
}



uint8_t string2hex(const uint8_t p_nNum)
{
    uint8_t dst;
    if (p_nNum < 10)
    {
        dst = p_nNum + '0';//���� +0x30
    }
    else
    {
        dst = p_nNum -10 +'A';//���� +55
    }
    return dst;
}



/*ȥ���ַ�����β�ո�*/
char * JHAL_stringRemoveBlank_SE(char *str)//ȥ����β�Ŀո�
{
    char *end,*sp,*ep;
    int len;
    sp = str;
    end = str + strlen(str) - 1;
    ep = end;

    while(sp<=end &&  *sp == ' ')// isspace(*ep)Ҳ����
        sp++;
    while(ep>=sp && *sp == ' ')
        ep--;
    len = (ep < sp) ? 0:(ep-sp)+1;//(ep < sp)�ж��Ƿ����ж��ǿո�
    sp[len] = '\0';
    return sp;
}


 

 



/*������ת��4���ֽ� srcԴ���ݣ�Ҫ��ų��������飬���ĸ�λ�ÿ�ʼ�� */
void JHAL_float2Byte(float src,uint8_t* outByteBuff,int startIndex)
{
    uint8_t byteBuff[4]= {0};
    *(float*)byteBuff=src;
    for(uint8 i=0; i<4; i++)
    {
        outByteBuff[startIndex+i]=byteBuff[i];
    }

}

/*4���ֽ�ת���ɸ�����   src����Դ���ݵ����飬startIndex ��ʼ��ַ*/
float JHAL_byte2Float(uint8_t* src,int startIndex )
{
    uint8_t byteBuff[4]= {0};
    for(uint8 i=0; i<4; i++)
    {
        byteBuff[i]=src[startIndex+i];
    }


    return *(float*)(uint8_t*)byteBuff;
}




/** ----------------------------JHAL_u32ToPowerExponent-----------------------------------
  * @������ ��һ��U32��ת������ָ����ʽ   ����ϵ����Χ���ֵ  ���ù̶����� ������ָ��
  *
  * @������
**	  	 value: Ҫת����ֵ
**			 itemMaxValue: ϵ�������ֵ
**			 base: ���õ���
**			 *item: [���]  ϵ��
**			 *powerExponent: [���] ָ��
  *
  * @����ֵ:��
  * @ע:��������ת�� ����Խ�󾫶�ԽС  ��Ϊ���Э��һ�㶼������������С���Ͳ�����������  ����������ǰ���������
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

void JHAL_u32ToPowerExponent( u32 value, u32 itemMaxValue, u32 base,u32 *item,u32 *powerExponent)
{
    *item=value;
    *powerExponent=0;
    while( value>(itemMaxValue*pow(base,*powerExponent)))
    {
        (*powerExponent)++;
    }
    if(*powerExponent!=0)
    {
				*item=value/(pow(base,*powerExponent));
      
    }

}




