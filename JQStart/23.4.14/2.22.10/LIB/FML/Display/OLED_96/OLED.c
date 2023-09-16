//////////////////////////////////////////////////////////////////////////////////

//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3
//              ----------------------------------------------------------------

//******************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "HALL_MyBSP\BoardLoadingPeripheral\LCD12864\lcd12864_zk.h"
//#include "OLED_Font.h"
#include "BMP.h"
#include "math.h"

static uint8_t Y1=0;
//static uint8_t Y2=1;
static uint8_t Y3=2;

//static uint8_t Y4=3;
static uint8_t  Y5=4;
//static uint8_t  Y6=5;
static uint8_t  Y7=6;
//static uint8_t  Y8=7;

//#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127


#if OLED_MODE==1
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;

void OLED_WR_Byte(u8 dat,u8 cmd)
{
    DATAOUT(dat);
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    OLED_WR_Clr();
    OLED_WR_Set();
    OLED_CS_Set();
    OLED_DC_Set();
}
#else
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i;
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    for(i=0; i<8; i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
}
#endif
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}
//����OLED��ʾ
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
        OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
        for(n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA);
    } //������ʾ
}

static int function(int a)   //�ж�һ�����Ǽ�λ��
{
    int n=0;
    while (a)
    {
        a = a / 10;
        n++;
    }
    if(a==0)//�˺���������С�����Զ�ȡ�������ָ�����������ʹ����0Ҳ�ܷ��ظ�����0
        return n+1;
    return n;
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//�õ�ƫ�ƺ��ֵ
    if(x>Max_Column-1)
    {
        x=0;
        y=y+2;
    }
    if(SIZE ==16)
    {
        OLED_Set_Pos(x,y);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x,y+1);
        for(i=0; i<6; i++)
            OLED_WR_Byte(F6x8[c][i],OLED_DATA);

    }
}
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ2������
//x,y :�������
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ');
                continue;
            }
            else enshow=1;

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0');
    }
} //���� Ϊ double e = 23.6666;
//С������ �� double r = e - (int) e;
//��ʾС��  ��ȡ���� ��С���� ��С�� ���ָ���ķ�ʽ
void OLED_MinNum(uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y����        ֵ            ������λС��
    int Zgsu;//����������������
    double XnSu1;//��������С������
    int XnSu10;//��������С���Ŵ󲿷� *10 ������һλС��
    int WwZi;//����λ�ã�������

    Zgsu=(int)Z;
    WwZi=function(Zgsu);//ȡ����λ������
    OLED_ShowNum(X,Y,Zgsu,WwZi,16);//д����
    if((Z!=0)||(BsX!=0))
    {
        OLED_ShowChar(X+(8*WwZi),Y,'.');
        XnSu1=Z-(int)Z;//ȡС��
        XnSu10=XnSu1*pow(10,BsX);//��ʾ10��BsX�η�
        OLED_ShowNum(X+8+(8*WwZi),Y,XnSu10,BsX,16);//дС��
    }
}
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

//��ʾ���� ����̬һά����
void OLED_ShowCHinese2(uint8_t x,uint8_t y,uchar *p)
{
	
	
    uint8_t t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(p[t],OLED_DATA);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(p[t+16],OLED_DATA);
        adder+=1;
    }
}

//��ʾ���� ����̬���������
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{
	
	
    uint8_t t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0; t<16; t++)
    {
			
        OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
        adder+=1;
    }
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;
    for(y=y0; y<y1; y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=x0; x<x1; x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}


//��ʼ��SSD1306
void OLED_Init(void)
{

    OLED_GPIO_Init();//GPIO��ʼ��

    OLED_RST_Set();
    HAL_Delay(100);
    OLED_RST_Clr();
    HAL_Delay(100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0,0);
}

static void OLED_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */ 
    __HAL_RCC_GPIOX_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin|OLED_RES_Pin|OLED_DS_Pin|OLED_D0_SCL_Pin
                      |OLED_D1_SDA_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : OLED_CS_Pin OLED_RES_Pin OLED_DS_Pin OLED_D0_SCL_Pin */
    GPIO_InitStruct.Pin = OLED_CS_Pin|OLED_RES_Pin|OLED_DS_Pin|OLED_D0_SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_CS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OLED_D1_SDA_Pin */
    GPIO_InitStruct.Pin = OLED_D1_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_D1_SDA_GPIO_Port, &GPIO_InitStruct);

}
void OLED_main(void)
{
    __IO static uint32_t TimePled;//����������һ��ʱ�����
    static uint16_t QtJuflus=380;//��Ϊʵ������в���Ҫȫ��ˢ��  ȫ��ˢ�µ���˱�־λ
    QtJuflus++;

    if(QtJuflus==400)//400*15ms=6sȫ��ˢ�µ���˱�־λ
    {
        QtJuflus=0;
        TimePled++;
        if(TimePled==20)//20*15ms=300ms
        {
            OLED_Clear();   //������Ϊ100ʱ���� ��25535��100����һ��
        }
				OLED_ShowCHinese2(0,Y1,dian);
       // OLED_ShowCHinese(0,Y1,0);//��				
        //OLED_ShowCHinese(16,Y1,1);//��

        OLED_ShowCHinese(48,Y1,2);//��
        OLED_ShowCHinese(64,Y1,3);//��

        OLED_ShowCHinese(96,Y1,4);//��
        OLED_ShowCHinese(16*7,Y1,5);//��

        OLED_ShowChar(8,Y3,'M');
        OLED_ShowChar(16,Y3,'1');

        OLED_ShowChar(8,Y5,'M');
        OLED_ShowChar(16,Y5,'2');

        OLED_ShowChar(8,Y7,'M');
        OLED_ShowChar(16,Y7,'3');

    }    //����Ҫȡģ�ڸ�ֵ  �� ��  ��ֵ


    OLED_XlDvBuSu();//��Բ�����ʾ
    OLED_XlDvBuSu2();//��Բ�����ʾ
    OLED_XlDvBuSu3();//��Բ�����ʾ
//    JtDvBuSu();//���Բ�����ʾ
//    JtDvBuSu2();//���Բ�����ʾ
//    JtDvBuSu3();//���Բ�����ʾ
    OLED_V_Motor1();
    OLED_V_Motor2();
    OLED_V_Motor3();


    //OLED_ShowChar(0,6,OLED_SPeed_count);//��ʾASCII�ַ�
//
//		//if(TimePled>'~')TimePled=' ';


    //	OLED_ShowNum(0,6,(-5),6,16);//���㵱ǰ���� 1������2


    //HAL_Delay(5000);
    //OLED_Clear();
    //	HAL_Delay(80);


//		for(r=0;r<=6;r++)
//		{
//		OLED_DrawBMP(0,0,128,8,xin);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
//				OLED_DrawBMP(0,0,128,8,qi);
//		HAL_Delay(800);


}



/*************************************************************************************************
* @�������ܣ��Ѳ������1����Բ�����ʾ��OLED
* @�����������
* @˼·˵����  1.�������������������motor.c�����������չȫ�ֱ��� 2.��һ������������������֮�� 3�����ж��Ƿ���Ӹ��ź�ȡ����ֵ
* @����ֵ����
* @�ص�˵������
* @ʾ��˵����

//x,y :�������
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С
* ******************************************************************
*/
void OLED_XlDvBuSu(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����

    if(OLED_SPeed_count!=0)//��ֹ0ʱ�������ù� ʡ��Դ  ������Oʱ���������
    {
        if(SP==1)//�����½���ʱ����ж�
        {

            OLED_ShowChar(XWwZi-8,Y3,'-');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//���㵱ǰ���� 1������2
            //OLED_MinNum(XWwZi,Y3,(OLED_SPeed_count/(799)),1);//���㵱ǰ���� 1������2
            SP=1;
            Pull_Counnt=0;
        }
        if(SP==2)
        {
            OLED_ShowChar(XWwZi-8,Y3,'+');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//���㵱ǰ���� 1������2
            Pull_Counnt=0;
        }
    }
    else  //����0ʱ�� ȥ����
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            OLED_ShowChar(XWwZi-8,Y3,' ');
            OLED_ShowNum(XWwZi,Y3,(OLED_SPeed_count/(799)),2,16);//���㵱ǰ���� 1������2
        }
    }

}


/*************************************************************************************************
* @�������ܣ��Ѳ������2����Բ�����ʾ��OLED
* @�����������
* @˼·˵����  1.�������������������motor.c�����������չȫ�ֱ��� 2.��һ������������������֮�� 3�����ж��Ƿ���Ӹ��ź�ȡ����ֵ
* @����ֵ����
* @�ص�˵������
* @ʾ��˵����

//x,y :�������
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С
* ******************************************************************
*/
void OLED_XlDvBuSu2(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����
    if(OLED_SPeed_count2!=0)//��ֹ0ʱ�������ù� ʡ��Դ  ������Oʱ���������
    {
        if(SP2==1)//�����½���ʱ����ж�
        {

            OLED_ShowChar(XWwZi-8,Y5,'-');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//���㵱ǰ���� 1������2
            SP2=1;
            Pull_Counnt=0;
        }
        if(SP2==2)
        {
            OLED_ShowChar(XWwZi-8,Y5,'+');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//���㵱ǰ���� 1������2
            Pull_Counnt=0;
        }
    }
    else  //����0ʱ�� ȥ����
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            OLED_ShowChar(XWwZi-8,Y5,' ');
            OLED_ShowNum(XWwZi,Y5,(OLED_SPeed_count2/(799)),2,16);//���㵱ǰ���� 1������2
        }
    }

}



/*************************************************************************************************
* @�������ܣ��Ѳ������3����Բ�����ʾ��OLED
* @�����������
* @˼·˵����  1.�������������������motor.c�����������չȫ�ֱ��� 2.��һ������������������֮�� 3�����ж��Ƿ���Ӹ��ź�ȡ����ֵ
* @����ֵ����
* @�ص�˵������
* @ʾ��˵����

//x,y :�������
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С
* ******************************************************************
*/
void OLED_XlDvBuSu3(void)
{
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����
    if(OLED_SPeed_count3!=0)//��ֹ0ʱ�������ù� ʡ��Դ  ������Oʱ���������
    {
        if(SP3==1)//�����½���ʱ����ж�
        {

            OLED_ShowChar(XWwZi-8,Y7,'-');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//���㵱ǰ���� 1������2
            SP3=1;
            Pull_Counnt=0;
        }
        if(SP3==2)
        {
            OLED_ShowChar(XWwZi-8,Y7,'+');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//���㵱ǰ���� 1������2
            Pull_Counnt=0;
        }
    }
    else  //����0ʱ�� ȥ����
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt==1;
            OLED_ShowChar(XWwZi-8,Y7,' ');
            OLED_ShowNum(XWwZi,Y7,(OLED_SPeed_count3/(799)),2,16);//���㵱ǰ���� 1������2
        }
    }

}




/*************************************************************************************************
* @�������ܣ��Ѳ�������ľ��Բ�����ʾ��OLED
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void JtDvBuSu(void)
{

    __IO static uint8_t LooK1=0;//Ŀǰֻ���������Բ�������ƶ�8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//������¼���Բ���
    __IO static uint8_t F1=0;//����ת����Բ���
    __IO static double   XWwZi=56;

    if(JtDv==1)//��main�ж���ľ��Բ�����־
    {
        if(OLED_SPeed_count!=0)//��ֹ0ʱ�������ù� ʡ��Դ
        {


            //	if(SP==1)//��ת��־

            if(SP==2)//��ת��־
            {


                if(OLED_SPeed_count/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1+1;  //��ʼֵ+1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//�����½���ʱ����ж�
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1-1;  //��ʼֵ-1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y3,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y3,F1,2,16);//���㵱ǰ���� 1������2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y3,'+');
        OLED_ShowNum(XWwZi,Y3,Sw1,2,16);//���㵱ǰ���� 1������2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y3,' ');
        OLED_ShowNum(XWwZi,Y3,Sw1,2,16);//���㵱ǰ���� 1������2

    }



}



/*************************************************************************************************
* @�������ܣ��Ѳ�������ľ��Բ�����ʾ��OLED
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void JtDvBuSu2(void)
{

    __IO static uint8_t LooK1=0;//Ŀǰֻ���������Բ�������ƶ�8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//������¼���Բ���
    __IO static uint8_t F1=0;//����ת����Բ���
    __IO static double   XWwZi=56;
    if(JtDv==1)//��main�ж���ľ��Բ�����־
    {
        if(OLED_SPeed_count2!=0)//��ֹ0ʱ�������ù� ʡ��Դ
        {


            //	if(SP==1)//��ת��־

            if(SP==2)//��ת��־
            {


                if(OLED_SPeed_count2/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1+1;  //��ʼֵ+1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count2/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count2/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count2/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count2/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count2/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count2/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count2/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//�����½���ʱ����ж�
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count2/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1-1;  //��ʼֵ-1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count2/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count2/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count2/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count2/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count2/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count2/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count2/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y5,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y5,F1,2,16);//���㵱ǰ���� 1������2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y5,'+');
        OLED_ShowNum(XWwZi,Y5,Sw1,2,16);//���㵱ǰ���� 1������2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y5,' ');
        OLED_ShowNum(XWwZi,Y5,Sw1,2,16);//���㵱ǰ���� 1������2

    }



}


/*************************************************************************************************
* @�������ܣ��Ѳ�������ľ��Բ�����ʾ��OLED
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void JtDvBuSu3(void)
{

    __IO static uint8_t LooK1=0;//Ŀǰֻ���������Բ�������ƶ�8
    __IO static uint8_t LooK2=0;
    __IO static uint8_t LooK3=0;
    __IO static uint8_t LooK4=0;
    __IO static uint8_t LooK5=0;
    __IO static uint8_t LooK6=0;
    __IO static uint8_t LooK7=0;
    __IO static uint8_t LooK8=0;
    __IO static int8_t Sw1;//������¼���Բ���
    __IO static uint8_t F1=0;//����ת����Բ���
    __IO static double   XWwZi=56;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����

    if(JtDv==1)//��main�ж���ľ��Բ�����־
    {
        if(OLED_SPeed_count3!=0)//��ֹ0ʱ�������ù� ʡ��Դ
        {


            //	if(SP==1)//��ת��־

            if(SP==2)//��ת��־
            {


                if(OLED_SPeed_count3/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1+1;  //��ʼֵ+1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }

                }

                if(OLED_SPeed_count3/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1+1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count3/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1+1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count3/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK3=0;
                        LooK4=1;
                    }


                }

                if(OLED_SPeed_count3/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1+1;
                        LooK4=0;
                        LooK5=1;
                    }


                }

                if(OLED_SPeed_count3/(799)==6)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK5=0;
                        LooK6=1;
                    }


                }


                if(OLED_SPeed_count3/(799)==7)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK6=0;
                        LooK7=1;
                    }


                }
                if(OLED_SPeed_count3/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1+1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }

            if(SP==1)//�����½���ʱ����ж�
            {
                //	OLED_ShowChar(80,Y3,'*');

                if(OLED_SPeed_count3/(799)==1)
                {
                    if(LooK1==0)//����ֹ�ظ������־�ر�ʱ
                    {
                        Sw1=Sw1-1;  //��ʼֵ-1
                        LooK1=1;//��ס ��ֹ799���¸�799֮��һֱ����
                        LooK2=0;//������������  ��Ϊ1�ǿ�ʼ��
                        LooK2=0;
                        LooK3=0;
                        LooK4=0;
                        LooK5=0;
                        LooK6=0;
                        LooK7=0;
                        LooK8=0;
                    }
                }

                if(OLED_SPeed_count3/(799)==2)
                {

                    if(LooK2==0)
                    {
                        Sw1=Sw1-1;//����������Ļ������ټ�1
                        LooK1=0;
                        LooK2=1;//��Ϊ�������Գ�������2  �Ͳ����ٵ���1  ���Խ���1
                    }
                }

                if(OLED_SPeed_count3/(799)==3)
                {

                    if(LooK3==0)
                    {
                        Sw1=Sw1-1;
                        LooK2=0;
                        LooK3=1;
                    }
                }

                if(OLED_SPeed_count3/(799)==4)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK3=0;
                        LooK4=1;
                    }



                }
                if(OLED_SPeed_count3/(799)==5)
                {

                    if(LooK5==0)
                    {
                        Sw1=Sw1-1;
                        LooK4=0;
                        LooK5=1;
                    }

                }
                if(OLED_SPeed_count3/(799)==6)
                {

                    if(LooK6==0)
                    {
                        Sw1=Sw1-1;
                        LooK5=0;
                        LooK6=1;
                    }



                }

                if(OLED_SPeed_count3/(799)==7)
                {

                    if(LooK7==0)
                    {
                        Sw1=Sw1-1;
                        LooK6=0;
                        LooK7=1;
                    }



                }
                if(OLED_SPeed_count3/(799)==8)
                {

                    if(LooK4==0)
                    {
                        Sw1=Sw1-1;
                        LooK7=0;
                        LooK8=1;
                    }


                }



            }






        }
    }

    if(Sw1<0)
    {
        OLED_ShowChar(XWwZi-8,Y7,'-');
        F1=-Sw1;
        OLED_ShowNum(XWwZi,Y7,F1,2,16);//���㵱ǰ���� 1������2

    }
    else if(Sw1>0)
    {
        OLED_ShowChar(XWwZi-8,Y7,'+');
        OLED_ShowNum(XWwZi,Y7,Sw1,2,16);//���㵱ǰ���� 1������2

    }
    else
    {
        OLED_ShowChar(XWwZi-8,Y7,' ');
        OLED_ShowNum(XWwZi,Y7,Sw1,2,16);//���㵱ǰ���� 1������2

    }



}



/*************************************************************************************************
* @�������ܣ��ٶ���ʾ�Ӻ���  �ɵ���   Ȧ/s   Ȧ/min
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void OLED_V_Motor1(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����
    if(OLED_SPeed_count!=0)//��������˶�
    {
        timmer1++;
        V_Motor=(((OLED_SPeed_count)/799)/(timmer1*0.015));//*60;//����60����Ȧÿ����
        OLED_MinNum(XWwZi,Y3,V_Motor,2);//��ʾ�ٶ�
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y3,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y3,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*2),Y3,' ');	//ָ��λ���㴦�� ��Ϊ������ȡ��λС������ʵ���������λֻ��һ����1.235
            OLED_ShowChar(XWwZi+(8*3),Y3,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*4),Y3,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
        }
    }
}

/*************************************************************************************************
* @�������ܣ��ٶ���ʾ�Ӻ���  �ɵ���   Ȧ/s   Ȧ/min
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void OLED_V_Motor2(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����
    if(OLED_SPeed_count2!=0)//��������˶�
    {
        timmer1++;
        //V_Motor=((OLED_SPeed_count/799)/(timmer1*0.015));//*60;//����60����Ȧÿ����
        V_Motor=((OLED_SPeed_count2/799)/(timmer1*0.015));//*60;//����60����Ȧÿ����
        OLED_MinNum(XWwZi,Y5,V_Motor,2);//��ʾ�ٶ�
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y5,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y5,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*2),Y5,' ');	//ָ��λ���㴦�� ��Ϊ������ȡ��λС������ʵ���������λֻ��һ����1.235
            OLED_ShowChar(XWwZi+(8*3),Y5,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*4),Y5,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
        }
    }
}


/*************************************************************************************************
* @�������ܣ��ٶ���ʾ�Ӻ���  �ɵ���   Ȧ/s   Ȧ/min
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ��˵������
* **************@ԭ��;����ְҵ����ѧԺ�����촴�͹���*********************************************************
*/
void OLED_V_Motor3(void)
{

    __IO static uint32_t timmer1;
    __IO static double   V_Motor;
    __IO static double   XWwZi=88;
    __IO static double   Pull_Counnt=0;//������0ʱˢ����
    if(OLED_SPeed_count3!=0)//��������˶�
    {
        timmer1++;
        //V_Motor=((OLED_SPeed_count/799)/(timmer1*0.015));//*60;//����60����Ȧÿ����
        V_Motor=((OLED_SPeed_count3/799)/(timmer1*0.015));//*60;//����60����Ȧÿ����
        OLED_MinNum(XWwZi,Y7,V_Motor,2);//��ʾ�ٶ�
        Pull_Counnt=0;
    }
    else
    {
        if(Pull_Counnt==0)
        {
            Pull_Counnt=1;
            timmer1=0;
            V_Motor=0;
            OLED_MinNum(XWwZi,Y7,V_Motor,0);
            OLED_ShowChar(XWwZi+(8*1),Y7,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*2),Y7,' ');	//ָ��λ���㴦�� ��Ϊ������ȡ��λС������ʵ���������λֻ��һ����1.235
            OLED_ShowChar(XWwZi+(8*3),Y7,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
            OLED_ShowChar(XWwZi+(8*4),Y7,' ');	//��5λ  �������ǰ4λ���� ������0��ʵ�����ֻҪ����1�͵�5���ڱ�����λС���µ�4-1λ�Զ�Ϊ0 ����һ�д���ִ�У�
        }
    }
}
