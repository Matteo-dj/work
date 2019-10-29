#ifndef __ICC_PROTOCAL_FILEMANAGER_H__
#define __ICC_PROTOCAL_FILEMANAGER_H__

#include <hi_types.h>
#include <hi_mdm_types.h>

#define WIFI_SSID_LENGTH 19
typedef struct
{
    HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1];
}HI_SET_SSID_CMD_S;//����ssid����

typedef struct
{
    int ret_result;
}HI_SET_SSID_IND_S;//����ssidӦ��

typedef struct
{
    HI_U32 pad;
}HI_GET_SSID_CMD_S;//��ȡssid����

typedef struct
{
    HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1];
}HI_GET_SSID_IND_S;//��ȡssidӦ��


typedef struct
{
    HI_U32 mode;
    HI_CHAR filename[0];
}HI_FMGR_CREATE_CMD_S;//�����ļ������

typedef struct
{
    int ret_result;
}HI_FMGR_CREATE_IND_S;//�����ļ��������

typedef struct
{
    HI_S32 flags;
    HI_U32 mode;
    HI_U32 sys_ctrl;
    HI_CHAR pathname[0];
}HI_FMGR_OPEN_CMD_S;// ���ļ������

typedef struct
{
    int ret_fd;
}HI_FMGR_OPEN_IND_S;//���ļ��������


typedef struct
{
    int fd;
    HI_SiZE_T length;
}HI_FMGR_READ_CMD_S;//���ļ������

typedef struct
{
    HI_SSiZE_T ret_real_read_size;
    HI_U8 buf[0];
}HI_FMGR_READ_IND_S;//���ļ��������

typedef struct
{
    int fd;
    HI_SiZE_T length;
    HI_U8 buf[0];
}HI_FMGR_WRITE_CMD_S;//д�ļ������

typedef struct
{
    HI_SSiZE_T ret_real_write;
}HI_FMGR_WRITE_IND_S;//д�ļ��������

typedef struct
{
    HI_S32 fd;
    HI_OFFET_T offset;//__off_t
    HI_S32 whence;
}HI_FMGR_ISEEK_CMD_S;//����ض�λ����


typedef struct
{
    HI_OFFET_T ret_pos; //__off_t
}HI_FMGR_ISEEK_IND_S;//����ض�λ���

typedef struct
{
    int fd;
}HI_FMGR_CLOSE_CMD_S;//�ر��ļ�����


typedef struct
{
    int ret_result;
}HI_FMGR_CLOSE_IND_S;//�ر��ļ����

typedef struct
{

    HI_U32 reserver;
    char pathName[0];
}HI_FMGR_STAT_CMD_S;


typedef struct
{
    HI_S32 ret_val;
    //HI_U64 st_dev
    //HI_U32 st_ino
    HI_U32 st_mode;
    //HI_U32 st_nlink
    //HI_U32 st_uid
    //HI_U32 st_gid
    //HI_U64 st_rdev
    //HI_U16 __pad1
    HI_S32 st_size;
    HI_S32 st_blksize;
    //HI_U16 __pad2
    HI_S32 st_blocks;
    HI_S32 st_atimex;
    HI_S32 st_mtimex;
    HI_S32 st_ctimex;
    //HI_U32 __unused4
    //HI_U32 __unused5
}HI_FMGR_STAT_IND_S;

typedef struct
{
    HI_U32 reserver;
    char pathName[0];
}HI_FMGR_STATFS_CMD_S;


typedef struct
{
    HI_S32 ret_val;
    HI_S32 f_type;     /* �ļ�ϵͳ����  */
    HI_S32 f_bsize;    /* �����Ż��Ĵ�����С  */
    HI_U32 f_blocks;   /* �ļ�ϵͳ���ݿ����� */
    HI_U32 f_bfree;    /* ���ÿ��� */
    HI_U32 f_bavail;   /* �ǳ����û��ɻ�ȡ�Ŀ��� */
    HI_U32 f_files;    /* �ļ�������� */
    HI_U32 f_ffree;    /* �����ļ������ */
    HI_S32 f_namelen;  /* �ļ�������󳤶� */
}HI_FMGR_STATFS_IND_S;

typedef struct
{
    HI_S32 mode;
    char pathName[0];
}HI_FMGR_ACCESS_CMD_S;

typedef struct
{
    int ret_val;
}HI_FMGR_ACCESS_IND_S;

typedef struct
{
    HI_U32 sys_ctrl;
    char cmdBuf[0];
}HI_FMGR_SYSOUT_CMD_S;

typedef struct
{
    HI_U32 reserver;
}HI_FMGR_SYSOUT_IND_S;
typedef struct
{
    HI_S32 tv_sec;
    HI_S32 tv_usec;
}HI_FMGR_SET_TIME_CMD_S;

typedef struct
{
    HI_S32 ret_val;
}HI_FMGR_SET_TIME_IND_S;

typedef struct
{
    HI_U32 reserver;
}HI_FMGR_INIT_CMD_S;

typedef struct
{
    HI_U32 ret_result;
}HI_FMGR_INIT_IND_S;

//��Ŀ¼����:��4λ
#define HI_FMGR_PATH_TYPE_MASK          0x0000000F
#define HI_FMGR_PATH_TYPE_SD_ROOT       0x00000000  //��������Ŀ¼
#define HI_FMGR_PATH_TYPE_SD_CARD       0x00000001  //����SD��Ŀ¼
//��������:
#define HI_FMGR_CMD_TYPE_MASK           0x000F0000
#define HI_FMGR_CMD_NONE                0x00000000
#define HI_FMGR_CMD_MK_DIR_NORMAL       0x00010000   //mkdir
#define HI_FMGR_CMD_MK_DIR_PARENTS      0x00020000  //mkdir -p

#endif


