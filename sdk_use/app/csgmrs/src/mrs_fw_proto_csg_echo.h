//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo.h
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_FW_PROTO_CSG_ECHO_H_
#define _MRS_FW_PROTO_CSG_ECHO_H_
HI_START_HEADER

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

#define MRS_CSG_AFN_ACK_00H             (0x00)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_CSG_DI_ACK                  (0xE8010001)
#define MRS_CSG_DI_DENY                 (0xE8010002)
#else
#define MRS_CSG_DI_ACK                  (0xEA010001)
#define MRS_CSG_DI_DENY                 (0xEA010002)
#endif

#define MRS_CSG_AFN_TASK_02H            (0x02)
#define MRS_CSG_DI_IND_TASK_INF         (0xE8030205)
#define MRS_CSG_DI_IND_UNTASK_LIST      (0xE8040204)
#define MRS_CSG_DI_IND_UNTASK_INFO      (0xE8040205)

#define MRS_CSG_AFN_REQUEST_03H         (0x03)
#define MRS_CSG_DI_IND_RUN_MODE_INF     (0xE8000302)
#define MRS_CSG_DI_IND_COMM_DELAYTIMES  (0xE8040304)
#define MRS_CSG_DI_IND_NODE_INFS        (0xE8040306)
#define MRS_CSG_DI_IND_PARENTS_INF      (0xE8040308)

#define MRS_CSG_AFN_REPORT_05H          (0x05)
#define MRS_CSG_DI_REPORT_TASK_DATA     (0xE8050501)
#define MRS_CSG_DI_REPORT_EVENT         (0xE8050502)
#define MRS_CSG_DI_REPORT_NODE_INF      (0xE8050503)
#define MRS_CSG_DI_REPORT_SEARCH_END    (0xE8050504)
#define MRS_CSG_DI_REPORT_TASK_STATUS   (0xE8050505)
#define MRS_CSG_DI_REPORT_POWER_FAILURE (0xE8050506)


#define MRS_CSG_AFN_DEBUG_F0H           (0xF0)
#define MRS_CSG_DI_IND_PLC_TEST         (0xE804F001)
#define MRS_CSG_DI_IND_ZERO_CROSS       (0xE804F002)

#define MRS_CSG_AFN_MANAGE_METER_21H    (0x21)
#define MRS_CSG_DI_REQ_METER_NUM        (0xEA062101)
#define MRS_CSG_DI_REQ_METER_INF        (0xEA042102)
#define MRS_CSG_DI_REQ_DETECT_LIST      (0xEA052104)
#define MRS_CSG_DI_REQ_DETECT_STATUS    (0xEA062105)

#define MRS_CSG_AFN_TRANSMIT_22H        (0x22)
#define MRS_CSG_DI_REQ_TRANSMIT         (0xEA042201)
#define MRS_CSG_DI_IND_TRANSMIT         (0xEA032201)

#define  MRS_CSG_AFN_UPG_FILE_24H       (0x24)
#define  MRS_CSG_DI_REQ_UPG_START       (0xEA052401)
#define  MRS_CSG_DI_REQ_UPG_TRANSMIT    (0xEA052402)
#define  MRS_CSG_DI_REQ_UPG_FILE_INFO   (0xEA062403)
#define  MRS_CSG_DI_REQ_UPG_FILE_PROC   (0xEA062404)

#define  MRS_CSG_DI_IND_UPG_FAILED_PROC (0xE8040705)

typedef enum
{
    MRS_CSG_DENY_COMMUNICATION_OVERTIME,
    MRS_CSG_DENY_INVALID_DATA,
    MRS_CSG_DENY_ERROR_LENGTH,
    MSG_CSG_DENY_ERROR_CHECK_SUM,
    MSG_CSG_DENY_DI_NOT_EXIST,
    MSG_CSG_DENY_ERROR_FORMAT,
    MSG_CSG_DENY_METER_REPEAT,
    MSG_CSG_DENY_METER_NOT_EXIST,
    MSG_CSG_DENY_APP_NOT_REPLY,
    MSG_CSG_DENY_CCO_BUSY,
    MSG_CSG_DENY_CCO_NOT_SUPPORT,
    MSG_CSG_DENY_STA_NOT_REPLY,
    MSG_CSG_DENY_STA_NOT_JOIN_NETWORK,
    MSG_CSG_DENY_TASK_NOT_ENOUGH,
    MSG_CSG_DENY_TASK_NOT_EXIST,
    MSG_CSG_DENY_TASK_REPEAT,
    MSG_CSG_DENY_TASK_EMPTY,
    MSG_CSG_DENY_TASK_ID_NOT_EXIST,
    MSG_CSG_DENY_OTHER = 0xFF,
} MRS_CSG_DENY_STATUS_E;


#define MRS_CSG_COCODE_VER_IND_LEN          (10)
#define MRS_CSG_COMMUNICATE_DELAYTIME_LEN   (7)

#define MRS_CSG_TASK_STATUS_SUCCESS         (0)
#define MRS_CSG_TASK_STATUS_NO_RESPONSE     (1)
#define MRS_CSG_TASK_STATUS_INVALID_DATA    (2)
#define MRS_CSG_TASK_STATUS_OTHER_ERR       (0xFF)

#define MRS_CSG_UPG_CURRENT_SEG_OFFSET      (0)
#define MRS_CSG_UPG_DATA_LEN_OFFSET         (2) // 传输文件段长度L偏移量
#define MRS_CSG_UPG_DATA_OFFSET             (4)
#define MRS_CSG_UPG_CS_LEN                  (2)
#define MRS_CSG_FILE_INFO_FIX_LEN           (18)
#define MRS_CSG_FILE_PROCESS_FIX_LEN        (4)
#define MRS_CSG_FILE_FAILED_INFO_FIX_LEN    (3)

#define MRS_EVENT_REPORT_DISABLE            (HI_FALSE)
#define MRS_EVENT_REPORT_ENABLE             (HI_TRUE)


typedef struct
{
    MRS_CSG_FRAME_STRU  *pstFrame;
    HI_PBYTE            *ppucOutBuffer; // 输出报文缓存
    HI_U16              *pusBufferLen;  // 输出报文长度
} AFN_DI_UL_PARAM;


typedef struct
{
    HI_U8 aucNodeAddr[HI_METER_ADDR_LEN];
} AFN_03_DI06_NODE_INFO;


typedef struct
{
    HI_S8 szManuCode[2];
    HI_S8 szChipCode[2];
    HI_U8 ucDay;
    HI_U8 ucMonth;
    HI_U8 ucYear;
    HI_U8 aucVer[2];
} AFN_CSG_MANU_INFO_STRU;


typedef struct
{
    HI_U8 ucIdLo;
    HI_U8 ucIdHi;
    HI_U8 ucPriority : 4;
    HI_U8 ucReserved : 3;
    HI_U8 ucRespFlag : 1;
    HI_U8 ucTimeoutLo;
    HI_U8 ucTimeoutHi;
    HI_U8 ucFrameLen;
    HI_U8 aucFrame[0];
} MRS_CSG_MR_TASK_UNICAST_STRU;


typedef struct
{
    HI_U8 ucIdLo;
    HI_U8 ucIdHi;
    HI_U8 ucPriority : 4;
    HI_U8 ucReserved : 3;
    HI_U8 ucRespFlag : 1;
    HI_U8 ucAddrNumLo;
    HI_U8 ucAddrNumHi;
    HI_U8 aucAddrInf[0];
} MRS_CSG_MR_TASK_MULTICAST_STRU;


//AFN_FN处理函数
typedef HI_U32 (*MRS_CSG_AFN_FUN)(HI_VOID * pParam);

typedef struct
{
    HI_U32 ulDi;
    MRS_CSG_AFN_FUN pfnFunc;
} MRS_CSG_AFN_FUNC_MAP;


typedef struct
{
    HI_U32 ulFuncNum;
    MRS_CSG_AFN_FUNC_MAP *pFuncMap;
} MRS_PROTO_CSG_CONTEXT;


HI_PUBLIC HI_U32 mrsProtoCsgEchoInit(HI_VOID);

HI_PUBLIC HI_U32 mrsProtoCsgEchoDeinit(HI_VOID);

HI_PUBLIC HI_U32 mrsProtoCsgEchoRegFuncMap(MRS_CSG_AFN_FUNC_MAP *pFuncMap, HI_U32 ulFuncNum);

HI_PUBLIC MRS_PROTO_CSG_CONTEXT *mrsProtoCsgEchoCtx(HI_VOID);

HI_EXTERN HI_U32 mrsCsgAfnDispatch(HI_IN MRS_CSG_FRAME_STRU * pstFrame, HI_OUT HI_PBYTE * ppOutBuffer, HI_U16 * pusBufferLen);

// 确认帧
HI_EXTERN HI_U32 mrsCsgProtoAckFrame(HI_IN HI_U16 usWaitTime, HI_IN AFN_DI_UL_PARAM * pstDlFrameInf);

// 否认帧
HI_EXTERN HI_U32 mrsCsgProtoDenyFrame(HI_IN HI_U8 *pstDenyFrame, HI_IN AFN_DI_UL_PARAM * pstDlFrameInf);

HI_EXTERN HI_U32 mrsCsgProtoManuInfo(AFN_CSG_MANU_INFO_STRU *pstManuInf);

#endif
HI_END_HEADER

#endif // _MRS_FW_PROTO_CSG_ECHO_H_

