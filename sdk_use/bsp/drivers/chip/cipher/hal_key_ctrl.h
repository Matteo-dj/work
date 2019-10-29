/******************************************************************************
  File Name     : hal_key_ctrl.h
  Version       : Initial Draft
  Author        : HCT team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __HAL_KEY_CTRL_H__
#define __HAL_KEY_CTRL_H__

#include <hi_types.h>
#include <hi_mdm_cipher.h>

#define KEY_CTRL_BASE		            (0x101E8100)
#define KL_WKEY0 	                    (KEY_CTRL_BASE + 0x00)
#define KL_WKEY1 	                    (KEY_CTRL_BASE + 0x04)
#define KL_WKEY2 	                    (KEY_CTRL_BASE + 0x08)
#define KL_WKEY3 	                    (KEY_CTRL_BASE + 0x0c)
#define KL_CTRL                         (KEY_CTRL_BASE + 0x10)
#define KL_STA                          (KEY_CTRL_BASE + 0x14)
#define KL_STA1                         (KEY_CTRL_BASE + 0x1c)
#define RBC_WKEY0_STA                   (KEY_CTRL_BASE + 0x20)
#define RBC_WKEY1_STA                   (KEY_CTRL_BASE + 0x24)
#define RBC_WKEY2_STA                   (KEY_CTRL_BASE + 0x28)
#define RBC_WKEY3_STA                   (KEY_CTRL_BASE + 0x2c)

#define KL_STA_WT_FINISH                ((HI_U32)1<<31)
#define KL_STA_WT_ERROR                 (1<<30)
#define KL_STA_READY_MASK               (7<<27)
#define KL_STA_READY                    (1<<27)
#define KL_STA_HASH_FINISH              (1<<1)
#define KL_STA_CIPHER_FINISH            (1<<0)

#define KL_STA1_WT_FINISH               (1<<10)//��KL_STA_WT_FINISHһ��
#define KL_STA1_WT_ERROR                (1<<9) //��KL_STA_WT_ERRORһ��
#define KL_STA1_RB_CHECK_ERROR          (1<<8)
//#define KL_STA1_BURN_STATE_MASK         (0xF<<0)
#define KL_STA1_BURN_STATE_MASK         (0x3<<2)

#define KL_CTRL_CIPHER_ADDR_MASK        (7<<8)
#define KL_CTRL_KEY_ADDR_MASK           (1<<4)
#define KL_CTRL_MODE_HASH               (1<<3)
#define KL_CTRL_MODE_EFUSE              (1<<2)
#define KL_CTRL_MODE_CIPHER             (1<<1)
#define KL_CTRL_MODE_MASK               (7<<1)
#define KL_CTRL_MODE_START              (1<<0)

#define DEFAULT_CIPHER_KEY_ADDR         0//(0x4<<8) //Ĭ��512bit��
#define CIPHER_KEY_CHANNEL_NUM          8//8����Կ�����տ�����cipher�ں궨��ͳһ




//*****************************************************************************
// ��������: HAL_EFUSE_BurnKey
// ��������: ��дkey��֧��ÿ��д��128bit
//
// ����˵��:
//      enAera:ѡ��efuse key��������д���ڼ���128bit��
//      ucSrcData:��Կ��
//      ucErrState:��д����״̬��ÿ��bit��ucSrcDataһһ��Ӧ��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS : �ɹ�
//      ����ֵΪ������
//
// ����Ҫ��:
//      1.ֻ����дһ�Σ�����д����ע�⼰ʱ�������ram��keyֵ��
//      2.��ֹ�ж��е��ã�
//      3.��дǰ�������ȶ�ȡ��д״̬�����HAL_EFUSE_CheckState��
//*****************************************************************************
HI_U32 HAL_EFUSE_BurnKey(HI_EFUSE_KEY_IDX_E enArea, HI_U8 ucSrcData[EFUSE_AREA_SIZE],HI_OUT HI_U8 ucErrState[EFUSE_AREA_SIZE]);

//*****************************************************************************
// ��������: HAL_EFUSE_CheckState
// ��������: ���efuse��д״̬
//
// ����˵��:
//      pulState: ����״̬����ӦbitΪ1��ʾ�Ѿ���д������������״̬��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS : �ɹ�
//      ����ֵΪ������
//
// ����Ҫ��:
//      1.��ֹ�ж��е��ã�
//*****************************************************************************
#define EFUSE_AERA1_BURN_LOCK (1<<0)
#define EFUSE_AERA2_BURN_LOCK (1<<1)
#define EFUSE_AERA3_BURN_LOCK (1<<2)
#define EFUSE_AERA4_BURN_LOCK (1<<3)
#define EFUSE_AERA_ALL_BURN_LOCK (0xF)
HI_U32 HAL_EFUSE_CheckState(HI_OUT HI_U32 *pulState);

//*****************************************************************************
// ��������: HAL_EFUSE_LoadCipher
// ��������: ���ض�����Ӳ��key����cipherָ��ͨ��
//
// ����˵��:
//      enAera: ѡ��efuse key��������
//      ulKeyChannel: cipher ͨ��ѡ��������ƣ����ܴ��ڵ���CIPHER_KEY_CHANNEL_NUM
//
// �� �� ֵ:
//      HI_ERR_SUCCESS : �ɹ�
//      ����ֵΪ������
//
// ����Ҫ��:
//      1.��ֹ�ж��е��ã�
//*****************************************************************************
HI_U32 HAL_EFUSE_LoadCipher(HI_EFUSE_KEY_IDX_E enArea, HI_U32 ulKeyChannel);

//*****************************************************************************
// ��������: HAL_EFUSE_LoadHash
// ��������: ���ض�����Ӳ��key����hash
//
// ����˵��:
//      enAera: ѡ��efuse key��������
//
// �� �� ֵ:
//      HI_ERR_SUCCESS : �ɹ�
//      ����ֵΪ������
//
// ����Ҫ��:
//      1.��ֹ�ж��е��ã�
//*****************************************************************************
HI_U32 HAL_EFUSE_LoadHash(HI_EFUSE_KEY_IDX_E enArea);

//used for debug or test, to delete
#define EFUSE_CTRL_BASE  (0x101E8000)

#define EFUSE_WT_EN     (EFUSE_CTRL_BASE+0x0)
#define EFUSE_WT_ADDR   (EFUSE_CTRL_BASE+0x4)
#define EFUSE_RD_EN     (EFUSE_CTRL_BASE+0x8)
#define EFUSE_RD_ADDR   (EFUSE_CTRL_BASE+0xc)
#define EFUSE_STATUS    (EFUSE_CTRL_BASE+0x10)
#define EFUSE_RDATA     (EFUSE_CTRL_BASE+0x14)
#define EFUSE_AVDD_EN   (EFUSE_CTRL_BASE+0x2c)

#define EFUSE_AVDD_EN_EN    (1 << 0)
#define EFUSE_STATUS_MASK   (0x7 << 2)
#define EFUSE_STATUS_READY  (0x1 << 2)
#define EFUSE_CTRL_ST       (0x1 << 5)
#define EFUSE_EN_SWITCH     (1 << 0)
#define EFUSE_EN_OK         (0)

#define EFUSE_STATUS_RD (1 << 1)
#define EFUSE_8_BIT (8)
#define EFUSE_KEY_LOCK_BIT (2)
#define EFUSE_PGM_ADDR_SIZE (EFUSE_AREA_SIZE * (HI_EFUSE_FREE_RW_IDX_5 + 1) * EFUSE_8_BIT - EFUSE_KEY_LOCK_BIT)
#define FREE_AREA_LOCK_ADDR (EFUSE_AREA_SIZE * (HI_EFUSE_FREE_RW_IDX_5 + 1) - 1)

HI_U32 HAL_EFUSE_Read(HI_EFUSE_IDX_E enEfuseFreeId, HI_U8 ucKeyData[EFUSE_AREA_SIZE]);
HI_U32 HAL_EFUSE_Write(HI_EFUSE_IDX_E enEfuseFreeId, HI_U8 ucKeyData[EFUSE_AREA_SIZE],HI_U8 ucErrState[EFUSE_AREA_SIZE]);
#endif //__HAL_KEY_CTRL_H__

