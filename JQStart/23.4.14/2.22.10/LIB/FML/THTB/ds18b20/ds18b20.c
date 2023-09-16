/********************************************************************************
	* @file    浼犳劅鍣ㄦā鍧楁祴璇曠▼搴�
	* @author  dotod
	* @version V1.0
	* @date    10-DEC-2017
	* @brief   Main program body.
	******************************************************************************
	* @attention	
	*
	******************************************************************************
	*/ 
/* Includes ------------------------------------------------------------------*/	


#include "ds18b20.h"
/**
  * @brief  DS18B20 鍒濆鍖�
  * @param  None
  * @retval None
  */  
void  init_ds18b20( void )  
{  
		init_onewire_out();  
		GPIO_ResetBits( GPIOE, GPIO_Pin_9 );  
		delay_us( 480 );  
		init_onewire_in();  
		delay_us( 60 );  
		if( !DQ_In )  
		{  
				delay_us( 120 );  
									
		}  
} 
/**
  * @brief  DS18B20 妫�祴
  * @param  None
  * @retval None
  */ 
void  check_ds18b20( void )  
{  
		init_onewire_out();  
		GPIO_ResetBits( GPIOE, GPIO_Pin_9 );  
		delay_us( 240 );  
		init_onewire_in();  
		delay_us( 60 );  
		if( !DQ_In )  
		{  
				delay_us( 80 );  
				if( !DQ_In )  
					printf( "妫�祴鍒癉S18B20!\r\n" );
		}  
	
}  
/**
  * @brief  璁剧疆涓轰富璁惧鍐欐�绾�浠庤澶囪鎬荤嚎
  * @param  None
  * @retval None
  */ 
void init_onewire_out( void )  
{  
	GPIO_InitTypeDef  GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );//寮�惎绔彛鏃堕挓 main()宸茬粡瀹氫箟 姝ゅ鍙互娉ㄩ噴

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//鎺ㄦ尳杈撳嚭
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz  

	GPIO_Init( GPIOE, &GPIO_InitStructure );//杈撳嚭绔彛鍒濆鍖�
}  
/**
  * @brief  璁剧疆涓轰富璁惧璇绘�绾�浠庤澶囧啓鎬荤嚎
  * @param  None
  * @retval None
  */ 
void init_onewire_in( void )  
{  
	GPIO_InitTypeDef  GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//寮�惎绔彛鏃堕挓 main()宸茬粡瀹氫箟 姝ゅ鍙互娉ㄩ噴

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//娴┖杈撳叆
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz  

	GPIO_Init( GPIOE, &GPIO_InitStructure );//杈撳叆绔彛鍒濆鍖�
}  
/**
  * @brief  DS18B20 鍐欏叆鏁版嵁
  * @param  None
  * @retval None
  */ 	
void ds18b20_write_byte( u8 data )  
{  
	u8 i;  
	u8 j = 0;  
	init_onewire_out();  
	for( i = 0; i < 8; i++ )  
	{  
		j = data & 0x01;  
		if( j )  
		{  
			DQ_Out = 0; //缃�1
			delay_us( 15 );  
			DQ_Out = 1;  
			delay_us( 60 );  
		}  
		else  
		{  
			DQ_Out = 0; //缃�0
			delay_us( 60 );  
			DQ_Out =  1;  
			delay_us( 1 );  
		}  
		data = data >> 1;  
	}     
}  
/**
  * @brief  DS18B20 璇诲彇1浣�
  * @param  None
  * @retval None
  */ 
u8 ds18b20_read_bit( void )  
{     
		u8 bit; 
	
		init_onewire_out(); 
		DQ_Out = 0;  
		delay_us( 2 );  
		DQ_Out=1; 
	
		init_onewire_in();  
		delay_us( 12 );  
		if( DQ_In )  
				bit = 1;  
		else  
				bit = 0;  
		delay_us( 50 );  
		return bit;  
}  
/**
  * @brief  DS18B20 璇诲彇瀛楄妭 鍏堝彂閫佸瓧鑺傚埌鎬荤嚎
  * @param  data 杩斿洖璇诲彇鏁版嵁
  * @retval None
  */ 
u8 ds18b20_read_byte( void )  
{  
	u8 data = 0;  
	u8 i;  
	u8 j = 0;  
	for( i = 0; i < 8; i++ )  
	{  
		j = ds18b20_read_bit();  
		if( j )       
			j = j << i;  
		data |= j;  

	}  
	return data;  
}  
/**
  * @brief  DS18B20 璇诲彇绯诲垪鐮佸拰48浣嶅簭鍒楀彿
  * @param  None
  * @retval None
  */ 
void ds18b20_read_rom_number()  
{  
		u32 number = 0;  
		u8 data, i, serial_num, ds18b20_crc; 
	
		init_ds18b20();  
		ds18b20_write_byte( 0x33 );
  
		serial_num = ds18b20_read_byte();  
		for( i = 0; i < 6; i++ )  
		{  
				data = ds18b20_read_byte();  
				number |= data;  
				number = number<<8;  
		}  
		ds18b20_crc = ds18b20_read_byte();  
			
		printf( "????:%d\r\n", serial_num );  
		printf( "????:%d\r\n", number );  
		printf( "CRC???:%d\r\n", ds18b20_crc );  
			
}  
/**
  * @brief  DS18B20 娓╁害杞崲寮�
  * @param  None
  * @retval None
  */ 
void tem_chage( void )  
{  
		init_ds18b20();  
		ds18b20_write_byte(0xcc);       //??rom??  
		ds18b20_write_byte(0x44);   //????  
}  
	
short  get_temp( void )  
{  
		int temp=0;  
		u8 i,TH,TL;  
		short tem;  
	
		tem_chage();  
		delay_us(10);  
		init_ds18b20();  
		ds18b20_write_byte(0xcc);       //??rom??  
		ds18b20_write_byte(0xbe);   //???????  
		TL=ds18b20_read_byte();  
		TH=ds18b20_read_byte();  
			
		if(TH > 7)       //????????????0,1????????,  
		{  
				temp = 0;   //??  
				TH =~TH;  
				TL =~TL;   
		}  
		else   
				temp = 1;   //??  
		tem = TH;  
		tem =tem<<8;  
		tem =tem+TL;  
		tem = (double)tem * 0.625;  
		if(temp)  
				return tem;  
		else  
				return -tem;  
			
}  
void ds18b20_return_TH_TL_CONF( void )  
{  
		char data,data_TH,data_TL,CONF;  
		init_ds18b20();  
		ds18b20_write_byte(0xcc);       //??rom??  
		ds18b20_write_byte(0xbe);   //???????  
		data = ds18b20_read_byte();  
		data = ds18b20_read_byte();  
		data_TH = ds18b20_read_byte();  
		data_TL = ds18b20_read_byte();  
		CONF =ds18b20_read_byte();  
		printf("????????:%d?\r\n",data_TH);  
		printf("????????:%d?\r\n",-(data_TL-128));  
		CONF &=0x60 ;  
		CONF =CONF>>5;  
		switch (CONF) {  
				case 0:  
						printf("ds18b20??????9?,???0.5?\r\n");  
						break;  
				case 1:  
						printf("ds18b20??????10?,???0.25?\r\n");  
						break;  
				case 2:  
						printf("ds18b20??????11?,???0.125?\r\n");  
						break;  
				case 3:  
						printf("ds18b20??????12?,???0.0625?\r\n");  
						break;  
				default:  
						printf("error!!\r\n");  
						break;  
		}  
} 
/**
  * @brief  DS18B20 璁剧疆娓╁害鎶ヨ鍊煎拰閰嶇疆绮惧害 TH杩囨俯鎶ヨ鍙� TH > 0 ) TL浣庢俯鎶ヨ鍊� TL < 0 ) mode閰嶇疆妯″紡濡備笅
  * @param  None
  * @retval None
  */ 
//mode = 0 绮惧害涓�浣� 	00011111 		dat = 31
//mode = 1 绮惧害涓�0浣�	00111111   	dat = 63
//mode = 2 绮惧害涓�1浣�	01011111    dat = 95
//mode = 3 绮惧害涓�2浣� 	01111111   	dat = 127
void ds18b20_write_TH_TL_CONF( u8 TH, u8 TL, u8 mode )  
{  
		u8 dat;  
		switch (mode){  
				case 0:  
						dat=31;  
						break;  
				case 1:  
						dat=63;  
						break;  
				case 2:  
						dat=95;  
						break;  
				case 3:  
						dat=127;  
						break;  
				default:  
						printf("mode error!!\r\n");  
						dat=127;  
						break;  
		}  
		TL=TL+128;  
		init_ds18b20();  
		ds18b20_write_byte(0xcc);       //??rom??  
		ds18b20_write_byte(0x4e);   //??????? ,????????  
		ds18b20_write_byte(TH); //??20锟�????
		ds18b20_write_byte(TL); //??-20锟�????
		ds18b20_write_byte(dat);    //????  
		init_ds18b20();  
		ds18b20_write_byte(0xcc);       //??rom??  
		ds18b20_write_byte(0x48);   //???????????EEPROM  
}  
void ds18b20_chack_self( void )  
{  
		check_ds18b20();  
		ds18b20_read_rom_number();  
		ds18b20_return_TH_TL_CONF();  
}  
