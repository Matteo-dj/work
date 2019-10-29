/**********************************************
;**********************************************
;***      出入栈结构
;***      Created by:              caojiajia
;***      Created date:            2014-12-09
;**********************************************
;**********************************************/	

#ifndef __FIFO_H
#define __FIFO_H

#include "rf_common.h"

/*-----------------FIFO堆栈类型-----------------*/
typedef struct fifo_node
{
  u16 read;
  u16 write;
}FIFO;

#endif  /* __FIFO_H */
	
#ifndef __FIFO_C
#define __FIFO_C	

/*--------------------FIFO缓冲区空--------------------*/
extern u8 FIFO_EMPTY(FIFO *fifo);
/*功能：判断FIFO缓冲区是否为空
  输入：FIFO缓冲区指针
  输出：无
  返回：非0值表示为空*/


/*--------------------FIFO缓冲区满--------------------*/
extern u8 FIFO_FULL(FIFO *fifo,u16 size);
/*功能：判断FIFO缓冲区是否为满
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：非0值表示为满*/


/*--------------------弹出一个(POP)--------------------*/
extern u16 FIFO_POP(FIFO *fifo,u16 size);
/*功能：弹出FIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待弹出数据的位置序号*/

/*--------------------压入一个(PUSH)--------------------*/
extern u16 FIFO_PUSH(FIFO *fifo,u16 size);
/*功能：压入FIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待压入数据的位置序号*/


/*--------------------压入一个(LIFO)--------------------*/
extern u16 LIFO_PUSH(FIFO *fifo,u16 size);
/*功能：压入LIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待压入数据的位置序号*/

/*--------------------计数器(FIFO)--------------------*/
extern u16 FIFO_COUNT(FIFO *fifo,u16 size);
/*功能：FIFO缓冲区内部数据计数器，计数当前数据使用量
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：FIFO缓冲区内部数据使用个数*/

/*--------------------读一个(READ)--------------------*/
extern u16 FIFO_READ(FIFO *fifo);
/*功能：读取FIFO缓冲区下一个数据，完成后指针不移动
  输入：FIFO缓冲区指针
  输出：无
  返回：待读数据的位置序号*/

/*--------------------写一个(WRITE)--------------------*/
extern u16 FIFO_WRITE(FIFO *fifo);

u16 FIFO_POP_IDX(FIFO *fifo,u16 size,u16 index);

#endif  /* __FIFO_C */