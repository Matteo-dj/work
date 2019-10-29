/**@defgroup hct_mdm ͨ�ýӿ�
 * @ingroup mdm
 */

#ifndef __HI_MDM_H__
#define __HI_MDM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>
#include <hi_sal_nv.h>
#ifndef PRODUCT_CFG_HSO             /**<                HSO �������ú�*/
#include <hi_mdm_cpu_stat.h>
#include <hi_mdm_isr.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_time.h>
#include <hi_mdm_timer.h>
#include <hi_mdm_event.h>
#include <hi_mdm_mem.h>
#include <hi_mdm_sem.h>
#endif
#include <hi_mdm_sys.h>
#ifndef PRODUCT_CFG_HSO             /**<                HSO �������ú�*/
#include <hi_mdm_task.h>
#include <hi_mdm_nv.h>
#include <hi_mdm_led.h>
#include <hi_mdm_switch.h>
#include <hi_mdm_io.h>
#include <hi_mdm_diag.h>
#include <hi_mdm_dms.h>
#include <hi_mem.h>
#include <hi_stdlib.h>
#include <hi_mdm_uart.h>
#include <hi_mdm_cipher.h>
#include <hi_list.h>
#include <hi_hw_id.h>
#include <hi_mdm_i2c.h>
#include <hi_wifi.h>
#include <hi_ioexp.h>
#include <hi_battery.h>
#endif
#include <hi_mdm_mac.h>
#include <hi_mdm_upg.h>

#ifndef PRODUCT_CFG_HSO
#ifndef MAC_V200_LLT
#include <hi_driver_internal.h>
#else
#include<hi_driver_internal_llt.h>
#endif
#include <hi_mdm_datacollect.h>
#include <hi_algorithm.h>
#include <hi_mdm_network.h>
#include <hi_product_types.h>
#include <hi_mdm_flash.h>

#include <hi_mdm_dfxchl.h>
#endif

HI_START_HEADER

#ifndef PRODUCT_CFG_HSO             /**<                HSO �������ú�*/
/**
 * @ingroup hct_mdm
* @brief  NDCͨ���б��Ŀ��ƽṹ��
*/
typedef struct
{
    HI_MDM_MSG_CTRL_S stCtrl;   /*<�����ֶ�  */
    HI_PBYTE pucPacket;     /*<�û����ݰ�  */
    HI_U16 usPacketSize;    /*<�û����ݰ�����  */
    HI_U16 reserved;
} HI_MDM_MSG_APPRCV_S;
#endif


/**
* @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:0
 */
#define HI_MDM_NDM_PING_ID_0                0
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:1
 */
#define HI_MDM_NDM_PING_ID_1                1
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:2
 */
#define HI_MDM_NDM_PING_ID_2                2
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:3
 */
#define HI_MDM_NDM_PING_ID_3                3
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:4
 */
#define HI_MDM_NDM_PING_ID_4                4
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID:5
 */
#define HI_MDM_NDM_PING_ID_5                5
/**
 * @ingroup  hct_mdm
 *  NDM��Ե�ͨ������ID MAX
 */
#define HI_MDM_NDM_PING_ID_NUM_MAX          6

/**
* @ingroup  hct_mdm
 *  ��ϢID����  (0x3600,0x3700]����ϸ�ο�HI_MDM_NM_XXX��
 */
#define HI_MDM_IS_MDMMSG(id)  (((id)>=0x3600) && ((id) < 0x3700))

//*****************************************************************************
// ��Ϣ����:  HI_MDM_NET_SEARCH_CONN
// ��������: ͨ��HI_MDM_MsgSend�������͸���Ϣ����������MDM�㣬��������
// ����ṹ��: HI_MDM_NET_SEARCH_CONN_REQ_S
// ����ṹ��: HI_MDM_NET_SEARCH_CONN_IND_S
//*****************************************************************************
#define HI_MDM_NET_SEARCH_CONN              0x3671  //����������

//****************************************************************************
//fcntl.h
#define HI_FMGR_O_ACCMODE   00000003
#define HI_FMGR_O_RDONLY    00000000
#define HI_FMGR_O_WRONLY    00000001
#define HI_FMGR_O_RDWR      00000002
#ifndef HI_FMGR_O_CREAT
#define HI_FMGR_O_CREAT     00000100    /* not fcntl */
#endif
#ifndef HI_FMGR_O_EXCL
#define HI_FMGR_O_EXCL      00000200    /* not fcntl */
#endif
#ifndef HI_FMGR_O_NOCTTY
#define HI_FMGR_O_NOCTTY    00000400    /* not fcntl */
#endif
#ifndef HI_FMGR_O_TRUNC
#define HI_FMGR_O_TRUNC     00001000    /* not fcntl */
#endif
#ifndef HI_FMGR_O_APPEND
#define HI_FMGR_O_APPEND    00002000
#endif
#ifndef HI_FMGR_O_NONBLOCK
#define HI_FMGR_O_NONBLOCK  00004000
#endif
#ifndef HI_FMGR_O_SYNC
#define HI_FMGR_O_SYNC      00010000
#endif
#ifndef HI_FMGR_FASYNC
#define HI_FMGR_FASYNC      00020000    /* fcntl, for BSD compatibility */
#endif
#ifndef HI_FMGR_O_DIRECT
#define HI_FMGR_O_DIRECT    00040000    /* direct disk access hint */
#endif
#ifndef HI_FMGR_O_LARGEFILE
#define HI_FMGR_O_LARGEFILE 00100000
#endif
#ifndef HI_FMGR_O_DIRECTORY
#define HI_FMGR_O_DIRECTORY 00200000    /* must be a directory */
#endif
#ifndef HI_FMGR_O_NOFOLLOW
#define HI_FMGR_O_NOFOLLOW  00400000    /* don't follow links */
#endif
#ifndef HI_FMGR_O_NOATIME
#define HI_FMGR_O_NOATIME   01000000
#endif
#ifndef HI_FMGR_O_CLOEXEC
#define HI_FMGR_O_CLOEXEC   02000000    /* set close_on_exec */
#endif
#ifndef HI_FMGR_O_NDELAY
#define HI_FMGR_O_NDELAY    O_NONBLOCK
#endif


//stat.h
#define HI_FMGR_S_IRWXU 00700
#define HI_FMGR_S_IRUSR 00400
#define HI_FMGR_S_IWUSR 00200
#define HI_FMGR_S_IXUSR 00100

#define HI_FMGR_S_IRWXG 00070
#define HI_FMGR_S_IRGRP 00040
#define HI_FMGR_S_IWGRP 00020
#define HI_FMGR_S_IXGRP 00010

#define HI_FMGR_S_IRWXO 00007
#define HI_FMGR_S_IROTH 00004
#define HI_FMGR_S_IWOTH 00002
#define HI_FMGR_S_IXOTH 00001

//unistd.h
#define HI_FMGR_R_OK    4       /* Test for read permission.  */
#define HI_FMGR_W_OK    2       /* Test for write permission.  */
#define HI_FMGR_X_OK    1       /* Test for execute permission.  */
#define HI_FMGR_F_OK    0       /* Test for existence.  */



#define HI_FMGR_SEEK_SET    0   /* Seek from beginning of file.  */
#define HI_FMGR_SEEK_CUR    1   /* Seek from current position.  */
#define HI_FMGR_SEEK_END    2   /* Seek from end of file.  */

//�ļ�ϵͳ����
#define HI_FMGR_ADFS_SUPER_MAGIC      0xadf5
#define HI_FMGR_AFFS_SUPER_MAGIC      0xADFF
#define HI_FMGR_BEFS_SUPER_MAGIC      0x42465331
#define HI_FMGR_BFS_MAGIC             0x1BADFACE
#define HI_FMGR_CIFS_MAGIC_NUMBER     0xFF534D42
#define HI_FMGR_CODA_SUPER_MAGIC      0x73757245
#define HI_FMGR_COH_SUPER_MAGIC       0x012FF7B7
#define HI_FMGR_CRAMFS_MAGIC          0x28cd3d45
#define HI_FMGR_DEVFS_SUPER_MAGIC     0x1373
#define HI_FMGR_EFS_SUPER_MAGIC       0x00414A53
#define HI_FMGR_EXT_SUPER_MAGIC       0x137D
#define HI_FMGR_EXT2_OLD_SUPER_MAGIC  0xEF51
#define HI_FMGR_EXT2_SUPER_MAGIC      0xEF53
#define HI_FMGR_EXT3_SUPER_MAGIC      0xEF53
#define HI_FMGR_HFS_SUPER_MAGIC       0x4244
#define HI_FMGR_HPFS_SUPER_MAGIC      0xF995E849
#define HI_FMGR_HUGETLBFS_MAGIC       0x958458f6
#define HI_FMGR_ISOFS_SUPER_MAGIC     0x9660
#define HI_FMGR_JFFS2_SUPER_MAGIC     0x72b6
#define HI_FMGR_JFS_SUPER_MAGIC       0x3153464a
#define HI_FMGR_MINIX_SUPER_MAGIC     0x137F /* orig. minix */
#define HI_FMGR_MINIX_SUPER_MAGIC2    0x138F /* 30 char minix */
#define HI_FMGR_MINIX2_SUPER_MAGIC    0x2468 /* minix V2 */
#define HI_FMGR_MINIX2_SUPER_MAGIC2   0x2478 /* minix V2, 30 char names */
#define HI_FMGR_MSDOS_SUPER_MAGIC     0x4d44
#define HI_FMGR_NCP_SUPER_MAGIC       0x564c
#define HI_FMGR_NFS_SUPER_MAGIC       0x6969
#define HI_FMGR_NTFS_SB_MAGIC         0x5346544e
#define HI_FMGR_OPENPROM_SUPER_MAGIC  0x9fa1
#define HI_FMGR_PROC_SUPER_MAGIC      0x9fa0
#define HI_FMGR_QNX4_SUPER_MAGIC      0x002f
#define HI_FMGR_REISERFS_SUPER_MAGIC  0x52654973
#define HI_FMGR_ROMFS_MAGIC           0x7275
#define HI_FMGR_SMB_SUPER_MAGIC       0x517B
#define HI_FMGR_SYSV2_SUPER_MAGIC     0x012FF7B6
#define HI_FMGR_SYSV4_SUPER_MAGIC     0x012FF7B5
#define HI_FMGR_TMPFS_MAGIC           0x01021994
#define HI_FMGR_UDF_SUPER_MAGIC       0x15013346
#define HI_FMGR_UFS_MAGIC             0x00011954
#define HI_FMGR_USBDEVICE_SUPER_MAGIC 0x9fa2
#define HI_FMGR_VXFS_SUPER_MAGIC      0xa501FCF5
#define HI_FMGR_XENIX_SUPER_MAGIC     0x012FF7B4
#define HI_FMGR_XFS_SUPER_MAGIC       0x58465342
#define HI_FMGR__XIAFS_SUPER_MAGIC    0x012FD16D

#define MSDU_LEN_OFFSET                                     (8)     /**< ����ϵͳ����Ϊmsdu��λ��ƫ��         */
#define MAC_ADD_FLAG_OFFSET                             (11)     /**< ����ϵͳ���Ƿ����MAC��ַ��־λƫ��         */

typedef struct
{
    HI_U32 ulId;
    HI_U32 ulPacketSn;
    HI_U16 usCtrl;
    HI_U16 usPayloadSize;
    HI_BYTE pucPayload[0];
} HI_MDM_DIAG_PK_REQ_S;

//****************************************************************************

//*****************************************************************************

#define HI_MDM_UpdateSysStatCounter(pulStatObj) do { OBJ64_COUNT_ADD(pulStatObj); }while(0)


//*****************************************************************************



//****************************************************************************
// EQT
// HI_MDM_EQT_CHL_PK  : HI_MDM_EQT_CHL_PK_REQ_S  HI_MDM_EQT_CHL_PK_IND_S
//****************************************************************************
#define HI_MDM_EQT_CHL_PK          0xB000    // HI_MDM_EQT_CHL_PK_REQ_S  HI_MDM_EQT_CHL_PK_IND_S

#define EQT_CHL_PK_IND_IND_CHK_OFFSET   (HI_FIELDOFFSET(HI_MDM_EQT_CHL_PK_IND_S, usEqtId))
#define EQT_CHL_PK_IND_REQ_CHK_OFFSET   (HI_FIELDOFFSET(HI_MDM_EQT_CHL_PK_REQ_S, usEqtId))

typedef struct
{
  HI_U32 ulCheckSum;
  HI_U16 usEqtId;
  HI_U16 reserved;
  HI_U8  ucMode;
  HI_U8  ucTransId;
  HI_U16 usPayloadSize;
  HI_U8 pucPayload[0];
} HI_MDM_EQT_CHL_PK_REQ_S;

typedef struct
{
  HI_U32 ulCheckSum;
  HI_U16 usEqtId;
  HI_U16 usError;
  HI_U8  ucMode;
  HI_U8  ucTransId;
  HI_U16 usPayloadSize;
  HI_U8 pucPayload[0]; // pointer HI_EQT_FRAME_XXX_S
} HI_MDM_EQT_CHL_PK_IND_S;



//****************************************************************************

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

/**
* @ingroup  hct_mdm
* @brief  ��PLC��MAC��ַд��������NV��
*
* @par   ����:
* ��PLC��MAC��ַд��������NV��
* @attention ���PLC MAC��ַҪ����Ϊȫ0��ȫ0xff��
                          �����һ���ֽڲ���Ϊ0��0xff��������Ч��
* @param  ucPlcMac[HI_PLC_MAC_ADDR_LEN] [IN] ���� HI_U8��
* @retval   ret [IN] ���� #HI_U32
                            ȡֵ HI_ERR_SUCCESS MAC��ַд��ɹ���
                            ȡֵ HI_ERR_MALLOC_FAILUE �����ڴ�ʧ�ܣ�
                            ����������μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm.h: �ļ�����������·�����ӿ�.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetPlcMac(HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN]);
HI_EAPI HI_U32 HI_MDM_ReadDevCfg(HI_INOUT HI_DMS_NV_FTM_DEVICE_CFG_S* pstDevCfg);

/**
* @ingroup  hct_mdm
* @brief ��ȡͨ��ģ��Ĳ�Ʒ��MAC��ַ��
*
* @par   ����:
* ��ȡͨ��ģ��Ĳ�Ʒ��MAC��ַ��
* @attention �ޡ�
*
* @param aucMac[HI_PLC_MAC_ADDR_LEN] [OUT] ��ȡģ���PLC MAC��ַ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
*
* @par Dependency:
* @li hi_mdm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_GetLocalMacAddr(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);
/*****************************************************************************
 �� �� ��  : HI_MDM_SetTestFreq
 ��������  : �����ͼ�ģʽ��Ƶ��ΪȫƵ����֧�ֹ���Э��ʹ�á�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_MDM_SetTestFreq(HI_VOID);

/**@defgroup hct_Log ������־�ӿ�
 * @ingroup hct_Log
*/

/**
* @ingroup  hct_Log
* @brief  �洢������־��Ϣ: ���ڴ���Ϣg_stSdmEngMrsLog�洢��FLASH�С�
*
* @par ����:
* �洢������־��Ϣ: ���ڴ���Ϣg_stSdmEngMrsLog�洢��FLASH�С�
* @attention
* @li ��֧���������е��ã���֧�����ж��е��á�
* @li ��־�洢Ϊ�첽�¼������øýӿں���ȴ�һ��ʱ����ٸ���#g_stSdmEngMrsLog��ֵ��
* @li #g_stSdmEngMrsLog�е�#stStorageHrdΪ������־ͷ��Ϣ���û���Ҫ�޸ĸ�ֵ��
* @li ������־��FLASH�����洢��λ�ã���μ���Hi3911V200 API ����ָ��.doc����FLASHС�ڡ�
*
* @param isIdxAdd [IN] ��ʶFlash��ַ�Ƿ����ӣ�ȡֵ����:
* @li  #HI_TRUE �洢�곭����־��ָ����һ��FLASH��ַ���´����µĵ�ַ�洢��
* @li  #HI_FALSE �洢�곭����־�󣬵�ַ���䣬�´μ����洢�ڸõ�ַ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_WriteMrsLog(HI_BOOL isIdxAdd);

/**@defgroup hct_User �û����ݶ�д�ӿ�
* @ingroup  hct_User
*/

/**
* @ingroup  hct_User
* @brief   �洢�û����ݡ�
*
* @par  ����:
* �洢�û�����
* @attention ���ò�Ʒ����ΪCCO��
* @li ��֧���������е��ã���֧�����ж��е��á�
* @li ÿ�ζ�д��Item��С�����ǹ̶���С��
* @param  ucUserDataId [IN] �û�����ID��
* @param  ulItemOffset [IN] �û���ȡ��ʼ��š�
* @param  ulItemNum    [IN] ��ȡ�������
* @param  usItemSize   [IN] ÿ���С��
* @param  pItems       [IN] �ڴ�ָ�롣
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm.h: �ýӿ��������ڵ�ͷ�ļ����塣</li></ul>
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SaveUserData(HI_U8 ucUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_IN HI_PVOID pItems);

/**
* @ingroup  hct_User
* @brief   ��ȡ�û����ݡ�
*
* @par  ����:
* ��ȡ�û�����
* @attention ���ò�Ʒ����ΪCCO��
* @li ��֧���������е��ã���֧�����ж��е��ã�
* @li ÿ�ζ�д��Item��С�����ǹ̶���С��
* @param  ucUserDataId [IN] #HI_USERDATA_ID_APP: ����#HI_USERDATA_ID_USR: �û�Ԥ����
* @param  ulItemOffset [IN]  �û���ȡ��ʼ��š�
* @param  ulItemNum    [IN]  ��ȡ�������
* @param  usItemSize   [IN]  ÿ���С��
* @param  pItems       [OUT] �ڴ�ָ�롣
* @param  reserved     [OUT] Ԥ����
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm.h: �ýӿ��������ڵ�ͷ�ļ����塣</li></ul>
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_GetUserData(HI_U8 ucUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_OUT HI_PVOID pItems,HI_OUT HI_U16* reserved);


/* END:   PN: DTS2016062501922 MODIFY\ADD\DEL by cuiate/00233580 at 2016/6/25 */




#endif // #if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

#include <hi_sal_nv.h>


HI_BOOL HI_MDM_PHY_IsAccessPhyMem(HI_U32 MemoryAdrr);
HI_U32 MAC_Eth_Send2Plc(HI_VOID *netif, HI_VOID *pbuf, HI_U8 *payload, HI_U16 uslen);
HI_END_HEADER
#endif // __HI_MDM_H__

