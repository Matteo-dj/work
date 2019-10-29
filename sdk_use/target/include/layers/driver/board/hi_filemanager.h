#ifndef __HI_FILEMANAGER_H__
#define __HI_FILEMANAGER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <icc_protocal_filemanager.h>

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



#define HI_MDM_SdCardSync()                                             HI_FMGR_Sync()
#define HI_MDM_SetFileSysTime(slTimeSec)                                HI_FMGR_SetTime(slTimeSec)
#define HI_MDM_OpenSdCardFile(pPathName,slFlags,ulMode,pRetFd)          HI_FMGR_Open(pPathName, slFlags, ulMode, pRetFd, HI_FMGR_PATH_TYPE_SD_CARD)
#define HI_MDM_ReadSdCardFile(slFd,pBuf,ulLength,pRetReadLen)           HI_FMGR_Read(slFd, pBuf, ulLength, pRetReadLen)
#define HI_MDM_WriteSdCardFile(slFd,pBuf,ulLength,pRetWriteLen)         HI_FMGR_Write(slFd, pBuf, ulLength, pRetWriteLen)
#define HI_MDM_LseekSdCardFile(slFd,OffSet,slWhence,pRetPos)            HI_FMGR_LSeek(slFd, OffSet, slWhence, pRetPos)
#define HI_MDM_CloseSdCardFile(slFd,pRetResult)                         HI_FMGR_Close(slFd, pRetResult)
#define HI_MDM_GetSdCardInfo(pExist,pTotalSize,pFreeSize)               HI_SD_GetSdCardInfo(pExist, pTotalSize, pFreeSize)
#define HI_MDM_GetSdCardStat()                                          HI_FMGR_GetConnectStat()
#define HI_MDM_SdCardMakeDir(scPathName,bForce)                         HI_FMGR_MakeDir(scPathName, bForce, HI_FMGR_PATH_TYPE_SD_CARD)
#define HI_MDM_SdCardAccess(scPathName,slMode,slRetVal)                 HI_FMGR_Access(scPathName, slMode, slRetVal, HI_FMGR_PATH_TYPE_SD_CARD)


#if defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) 
//*****************************************************************************
// ��������: HI_FMGR_Open
// ��������: ���ļ�
//
// ����˵��:
//      pathname[in] �ļ�fd
//      flags[in] �ļ��򿪲���
//      mode[in] flags����HI_FMGR_O_CREAT�����壬��ʾ�ļ�Ȩ��
//      ret_fd[out] ��ϵͳ����ʱ����ʾ�ļ���fd
//      usr_ctrl[in] ��������
//          HI_FMGR_PATH_TYPE_SD_ROOT:����3518 FLASH
//          HI_FMGR_PATH_TYPE_SD_CARD:����3518 sdcard
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Open(HI_IN const char * pathname,HI_IN int flags,HI_IN HI_U32 mode,HI_OUT HI_S32 * ret_fd,HI_U16 usr_ctrl);

//*****************************************************************************
// ��������: HI_FMGR_Read
// ��������: ���ļ�
//
// ����˵��:
//      fd[in] �ļ�fd
//      buf[in]  ���ļ����ݻ���
//      length[in] ���ļ����ݳ���
//      ret_real_len[out] ��ϵͳ����ʱ����ʾ��ȡ�ļ���С
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Read(HI_IN int fd,HI_OUT const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len);

//*****************************************************************************
// ��������: HI_FMGR_Write
// ��������: д�ļ�
//
// ����˵��:
//      fd[in] �ļ�fd
//      buf[in]  д���ݻ���
//      length[in] д���ݳ���
//      ret_real_len[out] ��ϵͳ����ʱ����ʾд�ļ���С
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Write(HI_IN int fd,HI_IN const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len);

//*****************************************************************************
// ��������: HI_FMGR_LSeek
// ��������: �ض�λ���
//
// ����˵��:
//      fd[in] �ļ�fd
//      offset[in]  ���ƫ���ֽ���
//      whence[in]
//          HI_FMGR_SEEK_SET  ����ļ���ʼƫ��
//          HI_FMGR_SEEK_CUR  ��Ե�ǰλ��ƫ��
//          HI_FMGR_SEEK_END  ����ļ���βƫ��
//      ret_pos[out] ��ϵͳ����ʱ����ʾ�ӿ�ִ�к���λ��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_LSeek(HI_S32 fd,HI_OFFET_T offset,HI_S32 whence,HI_OFFET_T * ret_pos);

//*****************************************************************************
// ��������: HI_FMGR_Close
// ��������: �ر��ļ�
//
// ����˵��:
//      fd[in] �ļ�fd
//      ret_result[out] ��ϵͳ����ʱ����ʾ�ӿڵ���ִ�н��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Close(HI_IN int fd,HI_OUT HI_S32 * ret_result);

//*****************************************************************************
// ��������: HI_FMGR_Access
// ��������: ��ȡ·������Ȩ��
//
// ����˵��:
//      pathname[in] �����1970��1��1�յ�ʱ��
//      mode[in] ��ӦȨ�� HI_FMGR_R_OK HI_FMGR_W_OK HI_FMGR_X_OK HI_FMGR_F_OK
//      ret_val[out] ��ϵͳ����ʱ����ʾ�ӿڵ���ִ�н��
//      usr_ctrl[in] ��������
//          HI_FMGR_PATH_TYPE_SD_ROOT:����3518 FLASH
//          HI_FMGR_PATH_TYPE_SD_CARD:����3518 sdcard
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Access(HI_IN const char * pathname,HI_IN HI_S32 mode,HI_OUT HI_S32 * ret_val,HI_IN HI_U16 usr_ctrl);

//*****************************************************************************
// ��������: HI_FMGR_SetTime
// ��������: ����ϵͳʱ��
//
// ����˵��:
//      timeSec[in] �����1970��1��1�յ�ʱ��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ϵͳʱ��ɹ�
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_SetTime(HI_IN HI_S32 timeSec);

//*****************************************************************************
// ��������: HI_FMGR_MakeDir
// ��������: ����Ŀ¼
//
// ����˵��:
//      pathName[in] ·��
//      bForce[in] HI_TRUE:�м�Ŀ¼�����ڣ������м�Ŀ¼
//      usr_ctrl[in] ��������
//          HI_FMGR_PATH_TYPE_SD_ROOT:����3518 FLASH
//          HI_FMGR_PATH_TYPE_SD_CARD:����3518 sdcard
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_MakeDir(HI_IN const char * pathname,HI_IN HI_BOOL bForce,HI_IN HI_U16 usr_ctrl);

//*****************************************************************************
// ��������: HI_FMGR_FileInfo
// ��������: ��ȡ�ļ���Ϣ
//
// ����˵��:
//      pathName[in] �ļ�·��
//      pFsInfo[out] �ļ���Ϣ
//      usr_ctrl[in] ��������
//          HI_FMGR_PATH_TYPE_SD_ROOT:����3518 FLASH
//          HI_FMGR_PATH_TYPE_SD_CARD:����3518 sdcard
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_FileInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STAT_IND_S * pFileInfo,HI_IN HI_U16 usr_ctrl);

//*****************************************************************************
// ��������: HI_FMGR_FsInfo
// ��������: ��ȡ�ļ�ϵͳ��Ϣ
//
// ����˵��:
//      pathName[in] �ļ�ϵͳ·��
//      pFsInfo[out] �ļ�ϵͳ��Ϣ
//      usr_ctrl[in] ��������
//          HI_FMGR_PATH_TYPE_SD_ROOT:����3518 FLASH
//          HI_FMGR_PATH_TYPE_SD_CARD:����3518 sdcard
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_FsInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STATFS_IND_S * pFsInfo,HI_IN HI_U16 usr_ctrl);

//*****************************************************************************
// ��������: HI_FMGR_Sync
// ��������: ͬ��IO����
//
// ����˵��:
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :����ʾ�ӿڵ��ù����У�δ����ͨ�ŵ�ϵͳ���󣬲�����ӿ�ִ�н��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32  HI_FMGR_Sync(HI_VOID);

//*****************************************************************************
// ��������: HI_SD_GetSdCardInfo
// ��������: ��ȡSD��״̬��Ϣ
//
// ����˵��:
//      pExist[out] HI_TRUE:sd����λ HI_FALSE:sd������λ
//      pTotalSize[out] sd���ܴ�С(��λKB)
//      pFreeSize[out] sd��ʣ���С(��λKB)
//
// �� �� ֵ:
//      HI_ERR_SUCCESS ��ȡSD��״̬�ɹ�
//      ����:��ȡSD��״̬ʧ��
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_SD_GetSdCardInfo(HI_OUT HI_BOOL *pExist,HI_OUT HI_U32 *pTotalSize,HI_OUT HI_U32 *pFreeSize);

//*****************************************************************************
//*****************************************************************************
// ��������: HI_FMGR_GetConnectStat
// ��������: ��ȡfile managerģ������״̬
//
// ����˵��:
//
// �� �� ֵ:
//      HI_TRUE:��ǰfile manager ģ�����
//      HI_FALSE:��ǰfile managerģ�鲻����
//
// ����Ҫ��:
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_BOOL HI_FMGR_GetConnectStat(HI_VOID);

//*****************************************************************************
// ��������: HI_FMGR_Init
// ��������: ��ʼ��file manager clientģ��׼������
//
// ����˵��:
//
// �� �� ֵ:
//
// ����Ҫ��:
//      �ϵ�ʱϵͳ����
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_U32 HI_FMGR_Init(HI_VOID);
#else//defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) && !defined (PRODUCT_CFG_EQUIP_TEST_MODE)
HI_PRVL HI_U32 HI_FMGR_Open(HI_IN const char * pathname,HI_IN int flags,HI_IN HI_U32 mode,HI_OUT HI_S32 * ret_fd,HI_U16 usr_ctrl){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_Read(HI_IN int fd,HI_OUT const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_Write(HI_IN int fd,HI_IN const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_LSeek(HI_S32 fd,HI_OFFET_T offset,HI_S32 whence,HI_OFFET_T * ret_pos){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_Close(HI_IN int fd,HI_OUT HI_S32 * ret_result){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_Access(HI_IN const char * pathname,HI_IN HI_S32 mode,HI_OUT HI_S32 * ret_val,HI_IN HI_U16 usr_ctrl){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_SetTime(HI_IN HI_S32 timeSec){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_MakeDir(HI_IN const char * pathname,HI_IN HI_BOOL bForce,HI_IN HI_U16 usr_ctrl){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_FileInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STAT_IND_S * pFileInfo,HI_IN HI_U16 usr_ctrl){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_FMGR_FsInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STATFS_IND_S * pFsInfo,HI_IN HI_U16 usr_ctrl){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32  HI_FMGR_Sync(HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_SD_GetSdCardInfo(HI_OUT HI_BOOL *pExist,HI_OUT HI_U32 *pTotalSize,HI_OUT HI_U32 *pFreeSize){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_BOOL HI_FMGR_GetConnectStat(HI_VOID){return HI_FALSE;}
HI_PRVL HI_U32 HI_FMGR_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}
#endif//defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) && !defined (PRODUCT_CFG_EQUIP_TEST_MODE)
#endif



