/********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:               fifo_API.c
** Descriptions:            fifo堆栈缓冲区应用程序接口
**
**--------------------------------------------------------------------------------------------------------
** Created by:              caojiajia
** Created date:            2014-12-09
** Version:                 v0.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#include "fifo_API.h"

/*--------------------FIFO缓冲区空--------------------*/
u8 FIFO_EMPTY(FIFO *fifo)
/*功能：判断FIFO缓冲区是否为空
  输入：FIFO缓冲区指针
  输出：无
  返回：非0值表示为空*/
{
  if (fifo->write == fifo->read) 
		return(1);
  return(0);
}

/*--------------------FIFO缓冲区满--------------------*/
u8 FIFO_FULL(FIFO *fifo,u16 size)
/*功能：判断FIFO缓冲区是否为满
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：非0值表示为满*/
{
  if ((fifo->write+1)%size == fifo->read) 
		return(1);
  return(0);
}

/*--------------------弹出一个(POP)--------------------*/
u16 FIFO_POP(FIFO *fifo,u16 size)
/*功能：弹出FIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待弹出数据的位置序号*/
{
  u16 returnval = fifo->read;
  
  fifo->read++;
  if (fifo->read >= size) 
		fifo->read = 0;
		
  return(returnval);
}

/*-------------------读FIFO指定位置的数据-------------------*/
u16 FIFO_POP_IDX(FIFO *fifo,u16 size,u16 index)
{
  u16 returnval = fifo->read;
  
  for(u8 i = 0; i < index;i++)
  {
    returnval++;
    if(returnval >= size)
    {
      returnval = 0;
    }
  }

  return(returnval);
}


/*--------------------压入一个(PUSH)--------------------*/
u16 FIFO_PUSH(FIFO *fifo,u16 size)
/*功能：压入FIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待压入数据的位置序号*/
{
  u16 returnval = fifo->write;
 
  fifo->write++;
  if (fifo->write >= size)
		fifo->write = 0;
		
  return(returnval);
}


/*--------------------压入一个(LIFO)--------------------*/
u16 LIFO_PUSH(FIFO *fifo,u16 size)
/*功能：压入LIFO缓冲区下一个数据，指针移动
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：待压入数据的位置序号*/
{
  if (fifo->read==0) 
		fifo->read = size-1;
  else 
		fifo->read--;
		
  return(fifo->read);
}


/*--------------------计数器(FIFO)--------------------*/
u16 FIFO_COUNT(FIFO *fifo,u16 size)
/*功能：FIFO缓冲区内部数据计数器，计数当前数据使用量
  输入：FIFO缓冲区指针，size为缓冲区的容量大小
  输出：无
  返回：FIFO缓冲区内部数据使用个数*/
{
	return((size + fifo->write - fifo->read)%size);
}


/*--------------------读一个(READ)--------------------*/
u16 FIFO_READ(FIFO *fifo)
/*功能：读取FIFO缓冲区下一个数据，完成后指针不移动
  输入：FIFO缓冲区指针
  输出：无
  返回：待读数据的位置序号*/
{
  return(fifo->read);
}


/*--------------------写一个(WRITE)--------------------*/
u16 FIFO_WRITE(FIFO *fifo)
/*功能：写入FIFO缓冲区下一个数据，完成后指针不移动
  输入：FIFO缓冲区指针
  输出：无
  返回：待写数据的位置序号*/
{
  return(fifo->write);
}


// /*********************************************************************************************************
//       END FILE
// *********************************************************************************************************/
 
