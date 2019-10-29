/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _HI_ENV_H_
#define _HI_ENV_H_

 #include <hi_types.h>

#define CONFIG_ENV_SECT_SIZE 0x1000 //后续考虑对64K兼容
#define CONFIG_ENV_SIZE  CONFIG_ENV_SECT_SIZE
#ifndef offsetof
#define offsetof(type, member) ((HI_U32) &((type *) 0)->member)
#endif

//启动升级相关环境变量结构体
//#define  RESERVE_SIZE (CONFIG_ENV_SIZE - offsetof(HI_ENV_DATA_ST, load_direct))
//#define  ENV_RESERVE_SIZE (CONFIG_ENV_SIZE - 37)
#define ENV_RESERVE_SIZE 1
#define ENV_DECOMPRESS_NV 0x55
typedef struct
{
   HI_U32 id;
   HI_U32 magic;
   HI_U32 addr_start;   //启动地址
   HI_U32 addr_backup;  //备份区地址
   HI_U32 env1;         //备份env地址1
   HI_U32 env2;         //备份env地址2
   HI_U16 mode;         //模式：0.正常 1.升级（统计重启次数）
   HI_U16 ver;          //预留版本号
   HI_U32 reset_cnt;    //mode=升级模式下，重启次数记录
   HI_U32 cnt_max;      //mode=升级模式下，重启次数上限，超过则切换addr_backup执行
   HI_U8  load_direct;  //是否直接引导 
   HI_U8  file_type;    //升级文件类型，kernel端判断是否更新nv文件用	
   HI_U8  ucDecompressNv;
   HI_U8  pad[ENV_RESERVE_SIZE];
   //此处基线，成员如有扩展下面添加，需要屏蔽0xFFFFFFFF做历史兼容
}HI_ENV_DATA_ST;

//****************************************************************************
// 函数名称: HI_MDM_InitEnv
// 功能描述: 初始化env
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
// 使用注意:仅调用一次，不允许多次初始化
//****************************************************************************
HI_U32 HI_MDM_InitEnv(HI_VOID);

//****************************************************************************
// 函数名称: HI_MDM_GetEnv
// 功能描述: 获取env
// 参数说明: * pstEnvdata: env结构体
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
// 使用注意: 含阻塞接口，不能中断中调用
//****************************************************************************
HI_U32 HI_MDM_GetEnv(HI_ENV_DATA_ST *pstEnvdata);

//****************************************************************************
// 函数名称: HI_MDM_SaveEnv
// 功能描述: 保存env
// 参数说明: * pstEnvdata: env结构体
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
// 使用注意: 
//                    1.含阻塞接口，不能中断中调用
//                    2.成员中id需要自行加一存储
//****************************************************************************
HI_U32 HI_MDM_SaveEnv(HI_ENV_DATA_ST *pstEnvdata);

#endif /* _ENVIRONMENT_H_ */
