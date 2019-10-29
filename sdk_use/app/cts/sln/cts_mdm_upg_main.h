#ifndef _CTS_MDM_UPG_MAIN_H
#define _CTS_MDM_UPG_MAIN_H

#define UART_PORT_NUM0 0   //操控器调试口为串口0，其他为串口1

typedef struct
{
    HI_U32   ulUpgId;         //申请获取升级缓存时的用户ID
    HI_U32   ulObtainFileSize;//用户申请的升级文件大小
    HI_U32   ulRealFileSize;  //用户当前实际缓存大小
    HI_PVOID pUpgObtainBuf;   //获取到的升级缓存地址
} CTS_UPG_OBTAIN_MEM_CTX; //申请升级缓存相关上下文

typedef struct
{
    HI_U16 buf_len; //串口缓存区长度
    HI_U8 reserved[2];
    HI_U8 *buffer; //串口缓存区
}CTS_UART_DATA_STRU;

//初始化对象
HI_PUBLIC HI_VOID cts_InitMdmUpgObj(HI_VOID);

//获取文件传输完成标志
HI_PUBLIC HI_BOOL upgCtsGetLoadFileFinishFlag(HI_VOID);

//计算升级文件校验和
HI_PUBLIC HI_U16 upgCtsCalcCheckSum(HI_PBYTE ptr, HI_U16 len);

//获取升级缓存信息上下文
HI_PUBLIC CTS_UPG_OBTAIN_MEM_CTX * upgGetMemObtainCtx(HI_VOID);

HI_VOID upgCtsSetLdFileFinishFlag(HI_BOOL flag);

#endif
