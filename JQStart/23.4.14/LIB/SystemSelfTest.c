#include "JSYS.h"



/** ----------------------------__JHAL_instructionTest-----------------------------------
  * ������ָ�����-���ò�����
  *
  * ������
**	  	 : [����/��]
  *
  * ����ֵ:��
  * ע:��
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

void __JHAL_instructionTest()

{
    u8 _8op1,_8op2;
    u32_t op1, op2, op3, result;
    float fop1, fop2      ;
    u32_t ret;

    ret = 0u;
    op1 = 0x33333333u;
    op2 = 0x22222222u;

    /* �ӷ� */
    result = op1 + op2;
    if (result != 0x55555555u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* ���� */
    result = op1 - op2;
    if (result != 0x11111111u)
    {
        ret = 0xFFFFFFFFu;
    }

    op2 = 2u;

    /* �˷� */
    result = op1 * op2;
    if (result != 0x66666666u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* ���� */
    result = op1 / op2;
    if (result != 0x19999999u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* ȡ�� */
    result = ~op1;
    if (result != ~0x33333333u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* ���� */
    result = op1 << 12;
    if (result != (0x33333000u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* ���� */
    result = op1 >> 12;
    if (result != (0x00033333u) )
    {
        ret = 0xFFFFFFFFu;
    }

    op2 = 0x22222222u;

    /* ��λ�� */
    result = op1 & op2;
    if (result != (0x22222222u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* ��λ�� */
    result = op1 | op2;
    if (result != (0x33333333u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* ��� */
    result = op1 ^ op2;
    if (result != (0x11111111u) )
    {
        ret = 0xFFFFFFFFu;
    }

    op1 = 0x77777777u;
    op2 = 0x33333333u;
    op3 = 2u;

    /* �˼� */
    result = op1 + (op2 * op3);
    if (result != (0xDDDDDDDDu) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* �˼� */
    result = op1 - (op2 * op3);
    if (result != (0x11111111u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* �Զ����Ͳ��� */
    _8op1=1;

    fop1 =0.999999 ;
    fop2 = 9.99998;

    if (!_8op1>fop1 )
    {
        ret=1;
    }

    _8op1=10;
    if( ! _8op1*fop1>fop2)
    {
        ret=2;
    }
    _8op2=26;
    if(  _8op2*_8op1<_8op2)
    {
        ret=3;
    }

#ifndef  NO_JSystemSelfTest
    // ָ�����ʧ��
    while(ret!=0u);
#endif		

}







void __JHAL_systemSelfTest(void)
{
    // ָ�����
    __JHAL_instructionTest();


}














