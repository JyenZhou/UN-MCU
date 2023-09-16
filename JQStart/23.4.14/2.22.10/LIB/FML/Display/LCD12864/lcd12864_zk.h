#ifndef __LCD12864_ZK_H
#define __LCD12864_ZK_H

//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
/************************************6*8的点阵************************************/
extern const unsigned char F6x8[][6];
/****************************************8*16的点阵************************************/
extern unsigned char F8X16[];


extern unsigned char huan[];
extern unsigned char ying[];
extern unsigned char fang[];
extern unsigned char zhou[];

extern unsigned char dian[];/*"电",0*/

extern unsigned char ji[];/*"机",1*/

extern unsigned char can[];/*"参",2*/

extern unsigned char shu[];/*"数",3*/

extern unsigned char she[];/*"设",4*/

extern unsigned char zhi[];/*"置",5*/

extern unsigned char tiao[];/*"调",0*/

extern unsigned char zhuan[];/*"转",1*/

extern unsigned char su[];/*"速",2*/

extern unsigned char dot[];/*":",3*/
	



/*--  文字:  距  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
extern unsigned char ju[];

/*--  文字:  离  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
extern unsigned char li[];

/*--  文字:  速  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
extern unsigned char sou[];

/*--  文字:  度  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
extern unsigned char du[];


/*数字和rmin等*/
extern unsigned char shuzi[];
/*字母*/
extern unsigned char ZiMu[];
/****************************************/
/*        定义ASCII字库8列*16行		    */
/****************************************/
extern unsigned char Engzk[];
extern unsigned char niu[1024];
			
extern	char Hzk[][32];
#endif	
