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

#define CONFIG_ENV_SECT_SIZE 0x1000 //�������Ƕ�64K����
#define CONFIG_ENV_SIZE  CONFIG_ENV_SECT_SIZE
#ifndef offsetof
#define offsetof(type, member) ((HI_U32) &((type *) 0)->member)
#endif

//����������ػ��������ṹ��
//#define  RESERVE_SIZE (CONFIG_ENV_SIZE - offsetof(HI_ENV_DATA_ST, load_direct))
//#define  ENV_RESERVE_SIZE (CONFIG_ENV_SIZE - 37)
#define ENV_RESERVE_SIZE 1
#define ENV_DECOMPRESS_NV 0x55
typedef struct
{
   HI_U32 id;
   HI_U32 magic;
   HI_U32 addr_start;   //������ַ
   HI_U32 addr_backup;  //��������ַ
   HI_U32 env1;         //����env��ַ1
   HI_U32 env2;         //����env��ַ2
   HI_U16 mode;         //ģʽ��0.���� 1.������ͳ������������
   HI_U16 ver;          //Ԥ���汾��
   HI_U32 reset_cnt;    //mode=����ģʽ�£�����������¼
   HI_U32 cnt_max;      //mode=����ģʽ�£������������ޣ��������л�addr_backupִ��
   HI_U8  load_direct;  //�Ƿ�ֱ������ 
   HI_U8  file_type;    //�����ļ����ͣ�kernel���ж��Ƿ����nv�ļ���	
   HI_U8  ucDecompressNv;
   HI_U8  pad[ENV_RESERVE_SIZE];
   //�˴����ߣ���Ա������չ������ӣ���Ҫ����0xFFFFFFFF����ʷ����
}HI_ENV_DATA_ST;

//****************************************************************************
// ��������: HI_MDM_InitEnv
// ��������: ��ʼ��env
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
// ʹ��ע��:������һ�Σ��������γ�ʼ��
//****************************************************************************
HI_U32 HI_MDM_InitEnv(HI_VOID);

//****************************************************************************
// ��������: HI_MDM_GetEnv
// ��������: ��ȡenv
// ����˵��: * pstEnvdata: env�ṹ��
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
// ʹ��ע��: �������ӿڣ������ж��е���
//****************************************************************************
HI_U32 HI_MDM_GetEnv(HI_ENV_DATA_ST *pstEnvdata);

//****************************************************************************
// ��������: HI_MDM_SaveEnv
// ��������: ����env
// ����˵��: * pstEnvdata: env�ṹ��
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
// ʹ��ע��: 
//                    1.�������ӿڣ������ж��е���
//                    2.��Ա��id��Ҫ���м�һ�洢
//****************************************************************************
HI_U32 HI_MDM_SaveEnv(HI_ENV_DATA_ST *pstEnvdata);

#endif /* _ENVIRONMENT_H_ */
