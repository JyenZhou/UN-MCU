#ifndef __MALLOC_H
#define __MALLOC_H
#include "JSYS.h"

#ifndef NULL
#define NULL 0
#endif

#define MEM_BLOCK_SIZE			  32  	  					          //�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE			    16*1024  						        //�������ڴ� 2K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE //�ڴ����С
		 
//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(void);					//��ʼ��
	u8   (*perused)(void);		  	    	//�ڴ�ʹ����
	u8 	  *membase;					//�ڴ��
	u16   *memmap; 					//�ڴ����״̬��
	u8     memrdy; 						//�ڴ�����Ƿ����
};

extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	 //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ� 
void my_mem_init(void);					 //�ڴ������ʼ������
u32 my_mem_malloc(u32 size);		 //�ڴ����
u8 my_mem_free(u32 offset);		 //�ڴ��ͷ�
u8 my_mem_perused(void);				 //����ڴ�ʹ���� 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);  			//�ڴ��ͷ�
void *mymalloc(u32 size);			//�ڴ����
void *myrealloc(void *ptr,u32 size);//���·����ڴ�
#endif













