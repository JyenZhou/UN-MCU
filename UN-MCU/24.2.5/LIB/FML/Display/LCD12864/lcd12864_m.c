/*****************************************************************************************************
   *                                                                *
 *       

*
*           
*
      使用方法  1.添加 字库.c   2.调用初始化  mInitLCD();

           OLED12864  不带字库版本 
					 不知道为什么.h中定义的数组变量不能放在其他的.h中
						甚至不能两个.c去引用他						
						弄的只能再配一个.c文件 并在定义字库的数组中用extern 
						因此要添加 字库.c
						
						
					 因为这个在初始化GPIO后才能初始化  我的befo main初始化 没有配置时钟  
					 会导致LCD本身的初始化无效
					 
				注意  若是在定时器中调用该显示程序做实时显示  需要给上一定时间	 
				像1ms执行一次的时间会导致显示异常1	 
				在初始化中 我配置的   htim4.Init.Period = 10000;//即1ms
				在调用的时候 做了等于400次才运行一次我要处理的程序 400*10ms=4s全局刷新
						                    *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

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
未初始化LCD12864
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
void Delay_Ms( uint16_t time ) //延时1ms函数
{ 
	uint16_t i,j;
	for( i = 0; i < time; i++ )
  		for( j = 10000; j > 0; j-- );
}

void mLCD_Reset( void ) 	//复位函数
{
	mLCD_CS1(0);		//片选使能
	mLCD_CS2 (0);		//片选使能
	
	mLCD_RST (0); 	//复位信号 低电平有效
	Delay_Ms( 1 );
	mLCD_RST (1); 
	Delay_Ms( 1 );
	
}
/*--------------------------------------------------
mRead_busy() 	//读"忙"函数 DB7 = 1 为busy
--------------------------------------------------*/
void mRead_Busy( void ) 	//读"忙"函数 DB7 = 1 为busy
{
MX_GPIO_Init_OP();//配置数据端口为输出模式
MX_GPIO_Init_In(); //配置数据端口为输入模式
	
	mLCD_RS (0);	//选择命令
	mLCD_RW ( 1); 	
	mLCD_E(1); 	//ENABLE
while( HAL_GPIO_ReadPin(DB8_GPIO_Port,DB8_Pin)); //1000 0000
	mLCD_E( 0); 	//DISABLE
MX_GPIO_Init_OP();//配置数据端口为输出模式
}
/*--------------------------------------------------
Write_LCD_command( uchar value ) //写命令函数
--------------------------------------------------*/
void mWrite_LCD_command( uchar value ) //写命令函数
{
	mRead_Busy();	//对LCD的每次读写都要读忙
	mLCD_RS (0);	//选择命令
	mLCD_RW (0); 	//读操作
	
	mLCD_WriteDatabus |= value;//发送数据
	
	mLCD_E (1); 	//ENABLE mLCD_E 由1到0锁存有效数据
	Delay_Ms( 1 );
	mLCD_E (0); 	//DISABLE	
}
/*--------------------------------------------------
Write_LCD_data( uchar value ) //写数据函数
--------------------------------------------------*/ 
void mWrite_LCD_data( uchar value ) //写数据函数
{
	mRead_Busy();	//对LCD的每次读写都要读忙
	mLCD_RS(1);	//选择数据
	mLCD_RW(0); 	//读操作
	
	mLCD_WriteDatabus |= value;//发送数据
	
	mLCD_E (1); 	//ENABLE mLCD_E 由1到0锁存有效数据
	Delay_Ms( 1 );
	mLCD_E (0); 	//DISABLE	
}

/*--------------------------------------------------
Set_page( uchar page ) //设置页 LCD12864共8页 1页8行点阵
--------------------------------------------------*/
void mSet_Page( uchar page ) //设置页 LCD12864共8页
{
	page = 0xb8 | page; //页的首地址为0xb8
	mWrite_LCD_command( page );
}
/*--------------------------------------------------
Set_line( uchar startline ) //设置显示的起始行
--------------------------------------------------*/
void mSet_Line( uchar startline ) //设置显示的起始行
{
	startline = startline & 0x3f; //行的最大值为64
	startline = 0xc0 | startline; //起始行地址为0xc0
	mWrite_LCD_command( startline ); //设置从哪行开始
}
/*--------------------------------------------------
Set_column( uchar column ) //设置显示的列
--------------------------------------------------*/
void mSet_Column( uchar column ) //设置显示的列
{
	column = column & 0x3f; //列的最大值为64 双屏128列
	column = 0x40 | column; //列的起始地址为0x40
	mWrite_LCD_command( column );	//列位置设置
}
/*--------------------------------------------------
 Set_onoff( uchar onoff ) //显示开关函数: 0x3e 关显示, 0x3f是开显示
--------------------------------------------------*/
void mSet_onoff( uchar onoff ) //显示开关函数: 0x3e 关显示, 0x3f是开显示
{
	onoff = 0x3e | onoff; // 1010 111x, onoff只能为0或者1
	mWrite_LCD_command( onoff );
}
/*--------------------------------------------------
SelectScreen( uchar screen )	//选择屏幕
--------------------------------------------------*/
void mSelectScreen( uchar screen )	//选择屏幕
{
	switch( screen )
	{
		case 0: 
			mLCD_CS1(0); 
			Delay_Ms( 1 );
			mLCD_CS2(0);
			Delay_Ms( 1 );		
			break; //全屏
		
		case 1:
			mLCD_CS1(0); 
			Delay_Ms( 1 );
			mLCD_CS2( 1); 
			Delay_Ms( 1 );
			break; //左半屏
		
		case 2: 
			mLCD_CS1(1); 
			Delay_Ms( 1 );
			mLCD_CS2(0);
			Delay_Ms( 1 );
			break; //右半屏
		
		default:
			break;
	}
}
/*--------------------------------------------------
 ClearScreen( uchar screen ) //清屏函数
--------------------------------------------------*/
void mClearScreen( uchar screen ) //清屏函数
{
	uchar i, j;
	
	mSelectScreen( screen ); //0全屏；1左半屏；2右半屏
	for( i = 0; i < 8; i++ ) //控制页数0-7，共8页
	{
		mSet_Page( i );
		mSet_Column( 0 );
		for( j = 0; j < 64; j++ ) //控制列数0-64，共64列
		{
			mWrite_LCD_data( 0x00 ); //写入0，LCD12864地址指针自加1
		}
	}
}
/*--------------------------------------------------
InitLCD() //LCD初始化
--------------------------------------------------*/
void mInitLCD() //LCD初始化
{
	  MX_LCD12864_GPIO_Init_();//GPIO初始化
	mLCD_Reset();
	mRead_Busy(); //对LCD的每次读写都要读忙
	mSelectScreen( 0 ); //0全屏
	mSet_onoff( 0 ); //关显示
	
	mSelectScreen( 0 ); //0全屏
	mSet_onoff( 1 ); //开显示
	
	mSelectScreen( 0 ); //0全屏
	mClearScreen( 0 ); //清屏
	
	mSet_Line( 0 ); //开始行:第0行
	
	Delay_Ms( 10 );
		
}
/*--------------------------------------------------
Display_HZ( uchar ss, uchar page, uchar column, uchar *p ) //显示汉字
--------------------------------------------------*/


void mDisplay_HZ1( uchar ss, uchar page, uchar column, uchar *p )
{
	uchar i;
	mSelectScreen( ss );
	mSet_Page( page );	//写上半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 16; i++ )	//控制16列的数据输出
	{
		mWrite_LCD_data( p[ i ] );	//汉字的上半部分，LCD12864地址指针自加1
	}
	
	mSet_Page( page + 1 );	//写下半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 16; i++ )	//控制16列的数据输出
	{
		mWrite_LCD_data( p[ i + 16 ] );	//汉字的下半部分，LCD12864地址指针自加1
	}

}

/*--------------------------------------------------
Display_HZ( uchar ss, uchar page, uchar column, uchar *p ) //显示汉字

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
	mSet_Page( page );	//写上半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 12; i++ )	//控制16列的数据输出
	{
		mWrite_LCD_data( p[ i ] );	//汉字的上半部分，LCD12864地址指针自加1
	}
	
	mSet_Page( page + 1 );	//写下半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 12; i++ )	//控制16列的数据输出
	{
		mWrite_LCD_data( p[ i + 12 ] );	//汉字的下半部分，LCD12864地址指针自加1
	}

}


/*--------------------------------------------------
Display_Eng( uchar ss, uchar page, uchar column, uchar *p ) //显示半角汉字 / 英文  / 数字
--------------------------------------------------*/
void mDisplay_Eng( uchar ss, uchar page, uchar column, uchar *p, uchar num )
{
	uchar i;
	int number;
	
	number = 16 * num;
	
	mSelectScreen( ss );
	mSet_Page( page );	//写上半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 8; i++ )	//控制8列的数据输出
	{
		mWrite_LCD_data( p[ i + number ] );	
	}
	
	mSet_Page( page + 1 );	//写下半页
	mSet_Column( column );	//控制列
	
	for( i = 0; i < 8; i++ )	//控制8列的数据输出
	{
		mWrite_LCD_data( p[ i + 8 + number ] );
	}

}
/*--------------------------------------------------
display_map( uchar *p ) //显示图片
--------------------------------------------------*/
void dmisplay_map( uchar *p )
{
	uchar seg;
	uchar page;
	for( page = 0; page < 8; page++ )
	{
		mSet_Page( page );	//写页
		mSet_Column( 0x00 );	//控制列
		for( seg = 0; seg < 128; seg++ )
		{
			mWrite_LCD_data( *p++ );
		}
	}
}


/***************自加**************/


int function(int a)   //判断一个数是几位数
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
** 把单个字符写到相应的位置  X (0-15)  y(0-8)  单个字符
**
**
********************************************************/

void LCD_ShowChar(uint8_t x,uint8_t y,uint32_t chr)//
{
	unsigned char c=0;
    c=chr-' ';//得到偏移后的值
	if(x<8)
	{
		mDisplay_Eng( 1, y, x*8, F8X16, c );//F8X16 是8*16的字库数组
	}
	else{
		mDisplay_Eng( 2, y, (x-8)*8, F8X16, c );
	}
}

/*************************************************
** 把多个字符写到相应的位置  X (0-15)  y(0-8)  单个字符
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
            y+=1;//换行
        }
        j++;
    }
}

//显示2个数字
//x,y :起点坐标
//num:数值
//BL:保留几位数，不足补0；默认用0不保留

void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t BL)
{  
	int Gesu;//整数位置（个数）
        int unitPlace = num / 1 % 10;//各位·
         int tenPlace = num / 10 % 10;
     int hundredPlace = num / 100 % 10;
    int thousandPlace = num / 1000 % 10;
	           int Wjww = num / 10000 % 10;//万位
	         int SiWjww = num / 100000 % 10;//十万位
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

void LCD_ShowNum2(uint8_t x,uint8_t y,uint32_t num,uint32_t len)//失败的显示只能3位正常  不知道是不是多加了个分号在if后面导致的吗 已改未验证
{
    uint32_t t,temp;
    uint8_t enshow=0;
		unsigned char c=0;
	
     for(t=0; t<len; t++)
    {
			//warning 类型不一致   
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


//假如 为 double e = 23.6666;
//小数部分 ： double r = e - (int) e;
//显示小数  用取整数 加小数点 加小数 这种隔离的方式
void LCD_MinNum(uint8_t X,uint8_t Y, double Z,uint8_t BsX)
{
    //x,y坐标        值            保留几位小数
    int Zgsu;//用来储存整数部分
    double XnSu1;//用来储存小数部分
    int XnSu10;//用来储存小数放大部分 *10 即保留一位小数
    int WwZi;//整数位置（个数）

    Zgsu=(int)Z;
    WwZi=function(Zgsu);//取整数位数函数
    LCD_ShowNum(X,Y,Zgsu,0);//写整数
    if((Z!=0)&&(BsX!=0))
		{
        LCD_ShowChar(X+(1*WwZi),Y,'.');
    XnSu1=Z-(int)Z;//取小数
			//warning 类型不一致  
    XnSu10=XnSu1*pow(10,BsX);//表示10的BsX次方
    LCD_ShowNum(X+1+(1*WwZi),Y,XnSu10,BsX);//写小数
		}

}

/*************************************************************************************************
* @函数介绍：测试的一个小程序
*
* @输入参数：无
* @参数说明：无
* @返回值：无
* @重点说明：无
* @示例及说明：无
* @作者;Jyen Z
* **************@作者;Jyen Z*****************************@作者;Jyen Z******************@作者;Jyen Z*********
*/

void Lcd12864_TestMain(void)
{
	static uint8_t Y1=0;
static uint8_t Y3=2;

	
	 mClearScreen( 0 );	//清屏
  	
		mDisplay_HZ( 0, Y1,dian );
					mDisplay_HZ( 1, Y1,ji );
			
					mDisplay_HZ( 3, Y1,ju );
					mDisplay_HZ( 4, Y1,li );
			
					mDisplay_HZ( 6, Y1,sou );
					mDisplay_HZ( 7, Y1,du );
			
			LCD_ShowString(0,Y3,"ce shi cheng xu");
		
}







