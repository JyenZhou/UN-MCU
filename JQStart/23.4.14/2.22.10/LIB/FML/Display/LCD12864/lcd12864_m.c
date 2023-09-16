/*****************************************************************************************************
   *                                                                *
 *       

*
*           
*
      ʹ�÷���  1.��� �ֿ�.c   2.���ó�ʼ��  mInitLCD();

           OLED12864  �����ֿ�汾 
					 ��֪��Ϊʲô.h�ж��������������ܷ���������.h��
						������������.cȥ������						
						Ū��ֻ������һ��.c�ļ� ���ڶ����ֿ����������extern 
						���Ҫ��� �ֿ�.c
						
						
					 ��Ϊ����ڳ�ʼ��GPIO����ܳ�ʼ��  �ҵ�befo main��ʼ�� û������ʱ��  
					 �ᵼ��LCD����ĳ�ʼ����Ч
					 
				ע��  �����ڶ�ʱ���е��ø���ʾ������ʵʱ��ʾ  ��Ҫ����һ��ʱ��	 
				��1msִ��һ�ε�ʱ��ᵼ����ʾ�쳣1	 
				�ڳ�ʼ���� �����õ�   htim4.Init.Period = 10000;//��1ms
				�ڵ��õ�ʱ�� ���˵���400�β�����һ����Ҫ����ĳ��� 400*10ms=4sȫ��ˢ��
						                    *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#include "lcd12864_m.h"


 void MX_GPIO_Init_In(void)
{


  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : DB1_Pin DB2_Pin DB3_Pin DB4_Pin 
                           DB5_Pin DB6_Pin DB8_Pin */
  GPIO_InitStruct.Pin = DB1_Pin|DB2_Pin|DB3_Pin|DB4_Pin 
                          |DB5_Pin|DB6_Pin|DB7_Pin|DB8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


}
void MX_GPIO_Init_OP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DB8_Pin|DB1_Pin 
                          |DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin 
                          |DB6_Pin|DB7_Pin, GPIO_PIN_RESET);
	 
	
	  GPIO_InitStruct.Pin = DB8_Pin|DB1_Pin 
                          |DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin 
                          |DB6_Pin|DB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
 void MX_LCD12864_GPIO_Init_(void)
{
#ifndef LCD12864_M
δ��ʼ��LCD12864
#endif
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
 // __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, CS1_Pin|CS2_Pin|RS_Pin|RW_Pin 
                          |E_Pin|DB8_Pin|DB1_Pin 
                          |DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin 
                          |DB6_Pin|DB7_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,RST_Pin,GPIO_PIN_SET);
  /*Configure GPIO pin : LED_1_Pin */


  /*Configure GPIO pins : CS1_Pin CS2_Pin RS_Pin RW_Pin 
                           E_Pin RST_Pin DB0_Pin DB1_Pin 
                           DB2_Pin DB3_Pin DB4_Pin DB5_Pin 
                           DB6_Pin DB7_Pin */
  GPIO_InitStruct.Pin = CS1_Pin|CS2_Pin|RS_Pin|RW_Pin 
                          |E_Pin|RST_Pin|DB8_Pin|DB1_Pin 
                          |DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin 
                          |DB6_Pin|DB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void Delay_Ms( uint16_t time ) //��ʱ1ms����
{ 
	uint16_t i,j;
	for( i = 0; i < time; i++ )
  		for( j = 10000; j > 0; j-- );
}

void mLCD_Reset( void ) 	//��λ����
{
	mLCD_CS1(0);		//Ƭѡʹ��
	mLCD_CS2 (0);		//Ƭѡʹ��
	
	mLCD_RST (0); 	//��λ�ź� �͵�ƽ��Ч
	Delay_Ms( 1 );
	mLCD_RST (1); 
	Delay_Ms( 1 );
	
}
/*--------------------------------------------------
mRead_busy() 	//��"æ"���� DB7 = 1 Ϊbusy
--------------------------------------------------*/
void mRead_Busy( void ) 	//��"æ"���� DB7 = 1 Ϊbusy
{
MX_GPIO_Init_OP();//�������ݶ˿�Ϊ���ģʽ
MX_GPIO_Init_In(); //�������ݶ˿�Ϊ����ģʽ
	
	mLCD_RS (0);	//ѡ������
	mLCD_RW ( 1); 	
	mLCD_E(1); 	//ENABLE
while( HAL_GPIO_ReadPin(DB8_GPIO_Port,DB8_Pin)); //1000 0000
	mLCD_E( 0); 	//DISABLE
MX_GPIO_Init_OP();//�������ݶ˿�Ϊ���ģʽ
}
/*--------------------------------------------------
Write_LCD_command( uchar value ) //д�����
--------------------------------------------------*/
void mWrite_LCD_command( uchar value ) //д�����
{
	mRead_Busy();	//��LCD��ÿ�ζ�д��Ҫ��æ
	mLCD_RS (0);	//ѡ������
	mLCD_RW (0); 	//������
	
	mLCD_WriteDatabus |= value;//��������
	
	mLCD_E (1); 	//ENABLE mLCD_E ��1��0������Ч����
	Delay_Ms( 1 );
	mLCD_E (0); 	//DISABLE	
}
/*--------------------------------------------------
Write_LCD_data( uchar value ) //д���ݺ���
--------------------------------------------------*/ 
void mWrite_LCD_data( uchar value ) //д���ݺ���
{
	mRead_Busy();	//��LCD��ÿ�ζ�д��Ҫ��æ
	mLCD_RS(1);	//ѡ������
	mLCD_RW(0); 	//������
	
	mLCD_WriteDatabus |= value;//��������
	
	mLCD_E (1); 	//ENABLE mLCD_E ��1��0������Ч����
	Delay_Ms( 1 );
	mLCD_E (0); 	//DISABLE	
}

/*--------------------------------------------------
Set_page( uchar page ) //����ҳ LCD12864��8ҳ 1ҳ8�е���
--------------------------------------------------*/
void mSet_Page( uchar page ) //����ҳ LCD12864��8ҳ
{
	page = 0xb8 | page; //ҳ���׵�ַΪ0xb8
	mWrite_LCD_command( page );
}
/*--------------------------------------------------
Set_line( uchar startline ) //������ʾ����ʼ��
--------------------------------------------------*/
void mSet_Line( uchar startline ) //������ʾ����ʼ��
{
	startline = startline & 0x3f; //�е����ֵΪ64
	startline = 0xc0 | startline; //��ʼ�е�ַΪ0xc0
	mWrite_LCD_command( startline ); //���ô����п�ʼ
}
/*--------------------------------------------------
Set_column( uchar column ) //������ʾ����
--------------------------------------------------*/
void mSet_Column( uchar column ) //������ʾ����
{
	column = column & 0x3f; //�е����ֵΪ64 ˫��128��
	column = 0x40 | column; //�е���ʼ��ַΪ0x40
	mWrite_LCD_command( column );	//��λ������
}
/*--------------------------------------------------
 Set_onoff( uchar onoff ) //��ʾ���غ���: 0x3e ����ʾ, 0x3f�ǿ���ʾ
--------------------------------------------------*/
void mSet_onoff( uchar onoff ) //��ʾ���غ���: 0x3e ����ʾ, 0x3f�ǿ���ʾ
{
	onoff = 0x3e | onoff; // 1010 111x, onoffֻ��Ϊ0����1
	mWrite_LCD_command( onoff );
}
/*--------------------------------------------------
SelectScreen( uchar screen )	//ѡ����Ļ
--------------------------------------------------*/
void mSelectScreen( uchar screen )	//ѡ����Ļ
{
	switch( screen )
	{
		case 0: 
			mLCD_CS1(0); 
			Delay_Ms( 1 );
			mLCD_CS2(0);
			Delay_Ms( 1 );		
			break; //ȫ��
		
		case 1:
			mLCD_CS1(0); 
			Delay_Ms( 1 );
			mLCD_CS2( 1); 
			Delay_Ms( 1 );
			break; //�����
		
		case 2: 
			mLCD_CS1(1); 
			Delay_Ms( 1 );
			mLCD_CS2(0);
			Delay_Ms( 1 );
			break; //�Ұ���
		
		default:
			break;
	}
}
/*--------------------------------------------------
 ClearScreen( uchar screen ) //��������
--------------------------------------------------*/
void mClearScreen( uchar screen ) //��������
{
	uchar i, j;
	
	mSelectScreen( screen ); //0ȫ����1�������2�Ұ���
	for( i = 0; i < 8; i++ ) //����ҳ��0-7����8ҳ
	{
		mSet_Page( i );
		mSet_Column( 0 );
		for( j = 0; j < 64; j++ ) //��������0-64����64��
		{
			mWrite_LCD_data( 0x00 ); //д��0��LCD12864��ַָ���Լ�1
		}
	}
}
/*--------------------------------------------------
InitLCD() //LCD��ʼ��
--------------------------------------------------*/
void mInitLCD() //LCD��ʼ��
{
	  MX_LCD12864_GPIO_Init_();//GPIO��ʼ��
	mLCD_Reset();
	mRead_Busy(); //��LCD��ÿ�ζ�д��Ҫ��æ
	mSelectScreen( 0 ); //0ȫ��
	mSet_onoff( 0 ); //����ʾ
	
	mSelectScreen( 0 ); //0ȫ��
	mSet_onoff( 1 ); //����ʾ
	
	mSelectScreen( 0 ); //0ȫ��
	mClearScreen( 0 ); //����
	
	mSet_Line( 0 ); //��ʼ��:��0��
	
	Delay_Ms( 10 );
		
}
/*--------------------------------------------------
Display_HZ( uchar ss, uchar page, uchar column, uchar *p ) //��ʾ����
--------------------------------------------------*/


void mDisplay_HZ1( uchar ss, uchar page, uchar column, uchar *p )
{
	uchar i;
	mSelectScreen( ss );
	mSet_Page( page );	//д�ϰ�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 16; i++ )	//����16�е��������
	{
		mWrite_LCD_data( p[ i ] );	//���ֵ��ϰ벿�֣�LCD12864��ַָ���Լ�1
	}
	
	mSet_Page( page + 1 );	//д�°�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 16; i++ )	//����16�е��������
	{
		mWrite_LCD_data( p[ i + 16 ] );	//���ֵ��°벿�֣�LCD12864��ַָ���Լ�1
	}

}

/*--------------------------------------------------
Display_HZ( uchar ss, uchar page, uchar column, uchar *p ) //��ʾ����

--------------------------------------------------*/
void	mDisplay_HZ( uchar ss, uchar page,  uchar *p )
{
		if(ss<4)
	{
		mDisplay_HZ1( 1, page, ss*16, p );
	}
	else{
		mDisplay_HZ1( 2, page, (ss-4)*16, p );
	}
}





void mDisplay_9HZ( uchar ss, uchar page, uchar column, uchar *p )
{
	uchar i;
	mSelectScreen( ss );
	mSet_Page( page );	//д�ϰ�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 12; i++ )	//����16�е��������
	{
		mWrite_LCD_data( p[ i ] );	//���ֵ��ϰ벿�֣�LCD12864��ַָ���Լ�1
	}
	
	mSet_Page( page + 1 );	//д�°�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 12; i++ )	//����16�е��������
	{
		mWrite_LCD_data( p[ i + 12 ] );	//���ֵ��°벿�֣�LCD12864��ַָ���Լ�1
	}

}


/*--------------------------------------------------
Display_Eng( uchar ss, uchar page, uchar column, uchar *p ) //��ʾ��Ǻ��� / Ӣ��  / ����
--------------------------------------------------*/
void mDisplay_Eng( uchar ss, uchar page, uchar column, uchar *p, uchar num )
{
	uchar i;
	int number;
	
	number = 16 * num;
	
	mSelectScreen( ss );
	mSet_Page( page );	//д�ϰ�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 8; i++ )	//����8�е��������
	{
		mWrite_LCD_data( p[ i + number ] );	
	}
	
	mSet_Page( page + 1 );	//д�°�ҳ
	mSet_Column( column );	//������
	
	for( i = 0; i < 8; i++ )	//����8�е��������
	{
		mWrite_LCD_data( p[ i + 8 + number ] );
	}

}
/*--------------------------------------------------
display_map( uchar *p ) //��ʾͼƬ
--------------------------------------------------*/
void dmisplay_map( uchar *p )
{
	uchar seg;
	uchar page;
	for( page = 0; page < 8; page++ )
	{
		mSet_Page( page );	//дҳ
		mSet_Column( 0x00 );	//������
		for( seg = 0; seg < 128; seg++ )
		{
			mWrite_LCD_data( *p++ );
		}
	}
}


/***************�Լ�**************/


int function(int a)   //�ж�һ�����Ǽ�λ��
{
    int n=0;
    while (a)
    {
        a = a / 10;
        n++;
    }
    return n;

}
/*************************************************
** �ѵ����ַ�д����Ӧ��λ��  X (0-15)  y(0-8)  �����ַ�
**
**
********************************************************/

void LCD_ShowChar(uint8_t x,uint8_t y,uint32_t chr)//
{
	unsigned char c=0;
    c=chr-' ';//�õ�ƫ�ƺ��ֵ
	if(x<8)
	{
		mDisplay_Eng( 1, y, x*8, F8X16, c );//F8X16 ��8*16���ֿ�����
	}
	else{
		mDisplay_Eng( 2, y, (x-8)*8, F8X16, c );
	}
}

/*************************************************
** �Ѷ���ַ�д����Ӧ��λ��  X (0-15)  y(0-8)  �����ַ�
**
**
********************************************************/

void LCD_ShowString(uint8_t x,uint8_t y,uint8_t*chr)//
{
	unsigned char j=0;
    while (chr[j]!='\0')
    {
        LCD_ShowChar(x,y,chr[j]);
        x+=1;
        if(x>15)
        {
            x=0;
            y+=1;//����
        }
        j++;
    }
}

//��ʾ2������
//x,y :�������
//num:��ֵ
//BL:������λ�������㲹0��Ĭ����0������

void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t BL)
{  
	int Gesu;//����λ�ã�������
        int unitPlace = num / 1 % 10;//��λ��
         int tenPlace = num / 10 % 10;
     int hundredPlace = num / 100 % 10;
    int thousandPlace = num / 1000 % 10;
	           int Wjww = num / 10000 % 10;//��λ
	         int SiWjww = num / 100000 % 10;//ʮ��λ
	Gesu=function(num);
	unsigned char c=0;
		if((Gesu>=6)||(BL>=6))
	{
		
		 LCD_ShowChar(x+c,y,SiWjww+'0');
		c++;
		
	}
	if(Gesu>=5||(BL>=65))
	{
		
		 LCD_ShowChar(x+c,y,Wjww+'0');
		c++;
		
	}
	if(Gesu>=4||(BL>=4))
	{
		 LCD_ShowChar(x+c,y,thousandPlace+'0');
		c++;
	}
	if(Gesu>=3||(BL>=3))
	{
		 LCD_ShowChar(x+c,y,hundredPlace+'0');
		c++;
	}
	if(Gesu>=2||(BL>=2))
	{
		 LCD_ShowChar(x+c,y,tenPlace+'0');
		c++;
	}
	//if(Gesu>=1||(BL>=1))
	{
		 LCD_ShowChar(x+c,y,unitPlace+'0');
		c++;
	}
	
}

void LCD_ShowNum2(uint8_t x,uint8_t y,uint32_t num,uint32_t len)//ʧ�ܵ���ʾֻ��3λ����  ��֪���ǲ��Ƕ���˸��ֺ���if���浼�µ��� �Ѹ�δ��֤
{
    uint32_t t,temp;
    uint8_t enshow=0;
		unsigned char c=0;
	
     for(t=0; t<len; t++)
    {
			//warning ���Ͳ�һ��   
			  c=pow(10,len-t-1);
        temp=(num/c)%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(1*t),y,' ');
                continue;
            }
            else enshow=1;

        }
        LCD_ShowChar(x+(1*t),y,temp+'0');
    }
}


//���� Ϊ double e = 23.6666;
//С������ �� double r = e - (int) e;
//��ʾС��  ��ȡ���� ��С���� ��С�� ���ָ���ķ�ʽ
void LCD_MinNum(uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y����        ֵ            ������λС��
    int Zgsu;//����������������
    double XnSu1;//��������С������
    int XnSu10;//��������С���Ŵ󲿷� *10 ������һλС��
    int WwZi;//����λ�ã�������

    Zgsu=(int)Z;
    WwZi=function(Zgsu);//ȡ����λ������
    LCD_ShowNum(X,Y,Zgsu,0);//д����
    if((Z!=0)&&(BsX!=0))
		{
        LCD_ShowChar(X+(1*WwZi),Y,'.');
    XnSu1=Z-(int)Z;//ȡС��
			//warning ���Ͳ�һ��  
    XnSu10=XnSu1*pow(10,BsX);//��ʾ10��BsX�η�
    LCD_ShowNum(X+1+(1*WwZi),Y,XnSu10,BsX);//дС��
		}

}

/*************************************************************************************************
* @�������ܣ����Ե�һ��С����
*
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ����˵������
* @����;Jyen Z
* **************@����;Jyen Z*****************************@����;Jyen Z******************@����;Jyen Z*********
*/

void Lcd12864_TestMain(void)
{
	static uint8_t Y1=0;
static uint8_t Y3=2;

	
	 mClearScreen( 0 );	//����
  	
		mDisplay_HZ( 0, Y1,dian );
					mDisplay_HZ( 1, Y1,ji );
			
					mDisplay_HZ( 3, Y1,ju );
					mDisplay_HZ( 4, Y1,li );
			
					mDisplay_HZ( 6, Y1,sou );
					mDisplay_HZ( 7, Y1,du );
			
			LCD_ShowString(0,Y3,"ce shi cheng xu");
		
}







