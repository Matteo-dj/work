#ifndef __OS_VX_VER_H_
#define __OS_VX_VER_H_
//
// 版本号定义, 格式为 x,y,z 表示vx.y.z, x/y/z为数字,每个数字最大为999
// 1,0,1 B018稳定版本
// 1,0,2 修改为时间片轮转方式, 问题单号DTS2013022006448
//
#define OS_VERSION() HI_SYS_CONFIG_OS_VER(1,0,2)
typedef int	                TASK_ID;
#endif
