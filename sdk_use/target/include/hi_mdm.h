/**@defgroup hct_mdm 通用接口
 * @ingroup mdm
 */

#ifndef __HI_MDM_H__
#define __HI_MDM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>
#include <hi_sal_nv.h>
#ifndef PRODUCT_CFG_HSO             /**<                HSO 工具启用宏*/
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
#ifndef PRODUCT_CFG_HSO             /**<                HSO 工具启用宏*/
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

#ifndef PRODUCT_CFG_HSO             /**<                HSO 工具启用宏*/
/**
 * @ingroup hct_mdm
* @brief  NDC通道中报文控制结构体
*/
typedef struct
{
    HI_MDM_MSG_CTRL_S stCtrl;   /*<控制字段  */
    HI_PBYTE pucPacket;     /*<用户数据包  */
    HI_U16 usPacketSize;    /*<用户数据包长度  */
    HI_U16 reserved;
} HI_MDM_MSG_APPRCV_S;
#endif


/**
* @ingroup  hct_mdm
 *  NDM点对点通信命令ID:0
 */
#define HI_MDM_NDM_PING_ID_0                0
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID:1
 */
#define HI_MDM_NDM_PING_ID_1                1
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID:2
 */
#define HI_MDM_NDM_PING_ID_2                2
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID:3
 */
#define HI_MDM_NDM_PING_ID_3                3
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID:4
 */
#define HI_MDM_NDM_PING_ID_4                4
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID:5
 */
#define HI_MDM_NDM_PING_ID_5                5
/**
 * @ingroup  hct_mdm
 *  NDM点对点通信命令ID MAX
 */
#define HI_MDM_NDM_PING_ID_NUM_MAX          6

/**
* @ingroup  hct_mdm
 *  消息ID定义  (0x3600,0x3700]，详细参考HI_MDM_NM_XXX。
 */
#define HI_MDM_IS_MDMMSG(id)  (((id)>=0x3600) && ((id) < 0x3700))

//*****************************************************************************
// 消息名称:  HI_MDM_NET_SEARCH_CONN
// 功能描述: 通过HI_MDM_MsgSend函数发送该消息给抄控器的MDM层，控制搜网
// 输入结构体: HI_MDM_NET_SEARCH_CONN_REQ_S
// 输出结构体: HI_MDM_NET_SEARCH_CONN_IND_S
//*****************************************************************************
#define HI_MDM_NET_SEARCH_CONN              0x3671  //搜网和连接

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

//文件系统类型
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

#define MSDU_LEN_OFFSET                                     (8)     /**< 测试系统用作为msdu的位置偏移         */
#define MAC_ADD_FLAG_OFFSET                             (11)     /**< 测试系统用是否存在MAC地址标志位偏移         */

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
* @brief  将PLC的MAC地址写到工厂区NV中
*
* @par   描述:
* 将PLC的MAC地址写到工厂区NV中
* @attention 入参PLC MAC地址要求不能为全0或全0xff，
                          且最后一个字节不能为0或0xff，重启生效。
* @param  ucPlcMac[HI_PLC_MAC_ADDR_LEN] [IN] 类型 HI_U8，
* @retval   ret [IN] 类型 #HI_U32
                            取值 HI_ERR_SUCCESS MAC地址写入成功，
                            取值 HI_ERR_MALLOC_FAILUE 申请内存失败，
                            其他错误码参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm.h: 文件用于描述链路层对外接口.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetPlcMac(HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN]);
HI_EAPI HI_U32 HI_MDM_ReadDevCfg(HI_INOUT HI_DMS_NV_FTM_DEVICE_CFG_S* pstDevCfg);

/**
* @ingroup  hct_mdm
* @brief 获取通信模块的产品的MAC地址。
*
* @par   描述:
* 获取通信模块的产品的MAC地址。
* @attention 无。
*
* @param aucMac[HI_PLC_MAC_ADDR_LEN] [OUT] 获取模块的PLC MAC地址。
*
* @retval #HI_ERR_SUCCESS 成功。
*
* @par Dependency:
* @li hi_mdm.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_GetLocalMacAddr(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);
/*****************************************************************************
 函 数 名  : HI_MDM_SetTestFreq
 功能描述  : 配置送检模式下频段为全频，仅支持国网协议使用。
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_MDM_SetTestFreq(HI_VOID);

/**@defgroup hct_Log 抄表日志接口
 * @ingroup hct_Log
*/

/**
* @ingroup  hct_Log
* @brief  存储抄表日志信息: 将内存信息g_stSdmEngMrsLog存储到FLASH中。
*
* @par 描述:
* 存储抄表日志信息: 将内存信息g_stSdmEngMrsLog存储到FLASH中。
* @attention
* @li 仅支持在任务中调用，不支持在中断中调用。
* @li 日志存储为异步事件，调用该接口后需等待一段时间后再更新#g_stSdmEngMrsLog的值。
* @li #g_stSdmEngMrsLog中的#stStorageHrd为抄表日志头信息，用户不要修改该值。
* @li 抄表日志在FLASH中所存储的位置，请参见《Hi3911V200 API 开发指南.doc》中FLASH小节。
*
* @param isIdxAdd [IN] 标识Flash地址是否增加，取值如下:
* @li  #HI_TRUE 存储完抄表日志后，指向下一块FLASH地址，下次在新的地址存储。
* @li  #HI_FALSE 存储完抄表日志后，地址不变，下次继续存储在该地址。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_WriteMrsLog(HI_BOOL isIdxAdd);

/**@defgroup hct_User 用户数据读写接口
* @ingroup  hct_User
*/

/**
* @ingroup  hct_User
* @brief   存储用户数据。
*
* @par  描述:
* 存储用户数据
* @attention 作用产品类型为CCO。
* @li 仅支持在任务中调用，不支持在中断中调用。
* @li 每次读写的Item大小必须是固定大小。
* @param  ucUserDataId [IN] 用户数据ID。
* @param  ulItemOffset [IN] 用户读取起始项号。
* @param  ulItemNum    [IN] 读取项个数。
* @param  usItemSize   [IN] 每项大小。
* @param  pItems       [IN] 内存指针。
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm.h: 该接口声明所在的头文件定义。</li></ul>
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SaveUserData(HI_U8 ucUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_IN HI_PVOID pItems);

/**
* @ingroup  hct_User
* @brief   获取用户数据。
*
* @par  描述:
* 获取用户数据
* @attention 作用产品类型为CCO。
* @li 仅支持在任务中调用，不支持在中断中调用；
* @li 每次读写的Item大小必须是固定大小。
* @param  ucUserDataId [IN] #HI_USERDATA_ID_APP: 表档案#HI_USERDATA_ID_USR: 用户预留。
* @param  ulItemOffset [IN]  用户读取起始项号。
* @param  ulItemNum    [IN]  读取项个数。
* @param  usItemSize   [IN]  每项大小。
* @param  pItems       [OUT] 内存指针。
* @param  reserved     [OUT] 预留。
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm.h: 该接口声明所在的头文件定义。</li></ul>
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

