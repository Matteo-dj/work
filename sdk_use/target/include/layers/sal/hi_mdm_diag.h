/**@defgroup hct_diag DIAG
 * @ingroup sal
 */

#ifndef __HI_MDM_DIAG_H__
#define __HI_MDM_DIAG_H__

#ifndef MAC_V200_LLT
#include <hi_sal.h>
#else
#include <hi_sal_llt.h>
#endif

HI_START_HEADER

/**
* @ingroup  hct_diag
* @brief 注册DIAG通道状态变更回调函数。
*
* @par 描述:
* 注册DIAG通道变更回调函数(即:当DIAG通道连接或断开时，会回调本接口注册的函数)。
* @attention 无。
*
* @param  pfnUserConnect[IN] 用户函数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_DiagRegisterConnect(HI_DIAG_CONNECT_F pfnUserConnect);


/**
* @ingroup  hct_diag
* @brief  控制诊断通道占用和释放调试串口。
*
* @par 描述:
* 通过该接口控制诊断通道占用和释放调试串口，DIAG初始化之后默认占用调试串口。
* @attention
* @li 该接口需要在DIAG初始化完成之后调用。
* @li 该接口不支持在中断中调用。
*
* @param  bConnect[IN] HI_TRUE:使能DIAG串口诊断通道 HI_FALSE:去除DIAG对调试串口的占用。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_DiagUartPortCtrl(HI_BOOL bConnect);


/**
* @ingroup  hct_diag
* @brief  注册命令处理函数。
*
* @par 描述:
* @attention
* @li 在初始化阶段调用，不支持多任务调用。
* @li 最多能注册20个不同的命令表。
* @li 由于命令处理是同步被调用，且是串行的，故要求注册后的命令处理入口
*运行时间尽量短，不能堵塞其它命令接收，目前设置处理超时为5s。
* @li 不同实例ID注册命令ID可以相同，同一实例注册的命令ID必须为唯一，否则注册失败。
*
* @param  pstCmdTbl[IN] 命令表，必须申明为常量数组传给该参数。
* @param  usCmdNum[IN] 命令个数，如果 usCmdNum 为零，而 pstCmdTbl不为零则取消注册。
* @param  ucInstanceId[IN] 实例ID。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterCommand(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId);

/**
* @ingroup  hct_diag
* @brief  设置用户自定义DIAG命令具有并发功能。
*
* @par 描述:
* 注册并发的命令ID,默认所有命令串行之前，如果在第一条命令执行完毕之前收到来自HSO第二条命令，则会返回chanel busy。
* @attention 无。
*
* @param  usMinId [IN] 命令ID下限。
* @param  usMaxId [IN] 命令ID上限。
* @param  pfnGetPkCache [IN] 内存空间申请函数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterParallelCmd(HI_U16 usMinId, HI_U16 usMaxId, HI_DIAG_GET_CMD_PK_CACHE_F pfnGetPkCache);

/**
* @ingroup  hct_diag
* @brief  注册系统诊断统计量。
*
* @par 描述:
* 该函数用于向DIAG通道注册统计量，DIAG通道会定期上报所注册的统计量，
*同时统计量支持查询。
* @attention 该接口不支持在中断中调用。
*
* @param  pstSysStatObjTbl [IN] 统计量对象列表，每条统计量对象的属性值为结构体#HI_DIAG_SYS_STAT_OBJ_S的成员。
* @param  usObjNum [IN] 待注册统计量的个数，即统计量对象列表中包含统计量的条数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterSysStat(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum);

/**
* @ingroup  hct_diag
* @brief  DIAG包上报。
*
* @par 描述:
* 该函数用于将DIAG通道、抄控宝通道上报给DIAG客户端。
* @attention 该接口不支持在中断中调用。

* @param  ulId [IN] DIAG应答包ID。
* @param  ucInstanceId [IN] 实例ID。
* @param  pucPacket [IN] 数据包的buffer地址，该函数不会释放该指针。
* @param  usPacketSize [IN] 数据包大小，单位:byte.
* @param  bSync[IN] DIAG包同步/异步上报设置。TRUE表示同步上报，阻塞操作；
* FALSE表示异步(通过分配内存后，由OS队列缓存再上报)，非阻塞操作。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ReportPacket(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);

/**
* @ingroup  hct_diag
* @brief  应答回复。
*
* @par 描述:
* 该函数用于回复ACK给DIAG客户端。
* @attention
* @li 该接口不支持在中断中调用。
* @li 在完成相同功能前提下，建议使用HI_MDM_ReportPacket接口。
*
* @param  ulId [IN] DIAG包ID。
* @param  pBuffer [IN] buffer地址。
* @param  usBufferSize [IN] buffer大小，单位:byte。
* @param  bSync [IN] 应答回复同步/异步上报设置，取值如下:
* @li TURE表示同步发送给DIAG客户端，通过OS队列缓存。
* @li FALSE表示异步发送给DIAG客户端，不通过OS队列缓存。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SendAckPacket(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync);

/**
* @ingroup  hct_diag
* @brief  系统消息上报接口。
*
* @par 描述:
* 该函数用于上报系统级的层间/模块间的消息(仅在CCO产品形态上支持)。
* @attention 该接口不支持在中断中调用。
* @param  usId [IN] 系统消息ID。
* @param  usSrcModId [IN] 源模块ID，0表示不指定模块ID(与HI_MDM_Malloc所使用模块ID相同)。
* @param  usDestModId [IN] 目的模块ID，0表示不指定模块ID(与HI_MDM_Malloc所使用模块ID相同)。
* @param  pPacket [IN] 消息包指针。
* @param  usPacketSize [IN] 消息包大小。
* @param  ulOption [IN] 取值如下
* @li HI_SYS_DMID_BUSINESS_L1 表示业务类等级为1的消息。
* @li HI_SYS_DMID_BUSINESS_L2 表示业务类等级为2的消息。
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption);

/**
* @ingroup  hct_diag
* @brief  系统初始化事件上报。
*
* @par 描述:
* 该函数用于上报初始化事件，该事件会在内存中缓存，
*不会因为未连接HSO而被丢弃，在打开Message界面时会自动上报。
* @attention 无。
*
* @param  usEvtId [IN] 初始化事件ID。
* @param  ucOption [IN] 初始化事件选项。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReportInitEvt(HI_U16 usEvtId, HI_U8 ucOption);

/**
* @ingroup  hct_diag
* @brief  上报系统事件。
*
* @par 描述:
* 该函数用于上报系统级的错误和告警事件。
* @attention 该接口不支持在中断中调用。
*
* @param  usEvtId [IN] 初系统事件ID。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReportSysEvent(HI_U16 usEvtId);

/**
* @ingroup  hct_diag
* @brief  注册DIAG操作通知回调函数。
*
* @par 描述:
* 该函数用于当HSO与单板有数据交互发生时，将数据通过回调函数抄送给应用层。
* @attention 无。
*
* @param  usrId [IN] 用户ID。
* @param  func [IN] 当HSO与单板有数据交互发生时，通过该接口通知应用层。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegistDiagCmdNotify(HI_U32 usrId, HI_DIAG_CMD_NOTIFY_F *func);

/**
* @ingroup  hct_diag
* @brief  主动运行DIAG命令。
*
* @par 描述:
* 该函数用于应用层下发DIAG命令，适用于不适用HSO场景。
* @attention 该接口不支持在中断中调用。
*
* @param  cmdId [IN] 命令ID。
* @param  ulOption [IN] 操作选项。
* @param  pData[IN] 命令参数。
* @param  ulDataSize [IN] 命令参数大小。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RunDiagCmd(HI_U32 cmdId,HI_U32 ulOption,HI_PVOID pData,HI_U16 ulDataSize);

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

//*****************************************************************************
// 函数名称: HI_DIAG_LOG_MSG_E4 HI_DIAG_LOG_MSG_W4 HI_DIAG_LOG_MSG_I4 HI_DIAG_LOG_BUF
// 功能描述:
//           Report the UserPlane message to 'User Plane Message Viewer' on PC side.
// 参数说明:
//           usFmtId  [in] LOG消息文本ID, 日志消息字符串sz的表标识. 取值范围0-0x0FFF,在同一个文件中必须唯一必须使用数字常量(或是结尾为整形数字的名称)
//           szFmt    [in] 字符串常量, 不支持回车符. 参数格式化当前版本不支持,仅直接显示该字符串
//           ulData0  [in] 32bit数据0
//           ulData1  [in] 32bit数据1
//           ulData2  [in] 32bit数据2
//           ulData3  [in] 32bit数据3
//           pLogBuffer    [in] 日志buffer
//           usLogBufSize  [in] 日志buffer长度, 单位字节
// 返 回 值:
//           无
//           HI_ERR_NO_INITILIZATION   设备未初始化
//           HI_ERR_QUEUE_FULL         OS队列满,发送到后台任务失败
//           HI_ERR_CFG_NOT_ALLOW      DIAG通道不关闭导致不允许上报
//           HI_ERR_INVALID_PARAMETER  传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY  无可用的缓存块使用,或是大数据块下内存分配失败
//           HI_ERR_FAILURE            未知错误
//
// 调用要求: 1) ulData0/ulData1/ulData2/ulData3如果为动态分配的内存则由调用者释放.
//           2) szFmt 不支持回车符.
//           3) usFmtId 在同一个文件中必须唯一且必须是数字常量(或是结尾为整形数字的名称),如果usFmtId参数为0, 则表示该接口自动取值使用当前行号.
//           4) HI_DIAG_LOG_MSG_XX  usFmtId szFmt 仅能写在同一行中.
// 调用举例:
//           HI_DIAG_LOG_MSG_W1(MY_ANY_MT_ID_101, "print warning message, this is v1=%d,2=%d,3=%d,4=%d", v1)
//           HI_DIAG_LOG_MSG_E4(0x1FF, "print error message 4, this is v1=%d,2=%d,3=%d,4=%d", v1,v2,v3,v4)
//           HI_DIAG_LOG_MSG_E4(MY_ANY_MT_ID_102, "print error message 4, this is v1=%d,2=%d,3=%d,4=%d"
//                              , v1,v2,v3,v4)
// 作    者: 郭成钢/00149569/20090903
//*****************************************************************************
#ifdef MAKE_PRIM_XML_PROCESS_IN
#define HI_DIAG_MT(x) x

#define HI_DIAG_LOG_MSG_E0(id, sz)                  {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_E1(id, sz, d0)              {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_E2(id, sz, d0, d1)          {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_E3(id, sz, d0, d1, d2)      {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_E4(id, sz, d0, d1, d2, d3)  {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}

#define HI_DIAG_LOG_MSG_W0(id, sz)                  {_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_W1(id, sz, d0)              {_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_W2(id, sz, d0, d1)          {_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_W3(id, sz, d0, d1, d2)      {_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_W4(id, sz, d0, d1, d2, d3)  {_PRIM_ST,_PRIM_PRI_=1,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}

#define HI_DIAG_LOG_MSG_I0(id, sz)                  {_PRIM_ST,_PRIM_PRI_=2,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_I1(id, sz, d0)              {_PRIM_ST,_PRIM_PRI_=2,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_I2(id, sz, d0, d1)          {_PRIM_ST,_PRIM_PRI_=2,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_I3(id, sz, d0, d1, d2)      {_PRIM_ST,_PRIM_PRI_=2,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}
#define HI_DIAG_LOG_MSG_I4(id, sz, d0, d1, d2, d3)  {_PRIM_ST,_PRIM_PRI_=2,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}

#define HI_DIAG_LOG_BUF(id, sz, buffer, size)       {_PRIM_ST,_PRIM_PRI_=0,_PRIM_ID_=id,_PRIM_SZ_=sz,_PRIM_LINE_=__LINE__,_PRIM_FILE_=__FILE_NAME__,_PRIM_FILE_ID_=__FILE_IDX__,_PRIM_MOD_ID_=__MOD_IDX__,_PRIM_END_}

#else
#define HI_DIAG_MT(x)

HI_EAPI HI_U32 HI_DIAG_LOG_MSG_I0(HI_U16 usFmtId, HI_PCSTR szFmt);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_W0(HI_U16 usFmtId, HI_PCSTR szFmt);
/**
* @ingroup  hct_diag
* @brief  用于上报日志消息（带文本信息格式+参数）到HSO的message界面中。
*
* @par 描述:
* 用于上报日志消息（带文本信息格式+参数）到HSO的message界面中，目前仅支持无符号整型的参数。
*
* @attention HI_DIAG_LOG_MSG_E0接口名中的"E0"在实际使用中根据需要可替换为其他有效值，即提供如下接口:
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E0(HI_U16 usFmtId, HI_PCSTR szFmt)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E1(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E2(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E3(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E4(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2, HI_U32 ulData3)
*
* @param usFmtId [IN] LOG消息ID，日志消息字符串szFmt的标识。取值范围0～0x0FFF，在同一个文件中必须唯一使用数字常量，不能使用变量（编译器内置变量等）、宏定义。
* @param szFmt [IN] 字符串常量，字符串必须使用HI_DIAG_LMT宏括起，且不支持回车符。当前版本不支持参数格式化，仅直接显示该字符串。
* @param ulDatax[IN] 传递的参数。
*
* @retval #HI_ERR_NO_INITILIZATION 设备未初始化。
* @retval #HI_ERR_NO_INITILIZATION 设备未初始化。
* @retval #HI_ERR_QUEUE_FULL OS队列满，发送到后台任务失败。
* @retval #HI_ERR_CFG_NOT_ALLOW DIAG通道关闭导致不允许上报。
* @retval #HI_ERR_INVALID_PARAMETER 传参非法。
* @retval #HI_ERR_NOT_ENOUGH_MEMORY 无可用的缓存块使用或大数据块下内存分配失败。
* @retval 其他值 错误。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see HI_DIAG_LOG_BUF
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E0(HI_U16 usFmtId, HI_PCSTR szFmt);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_I1(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_W1(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E1(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_I2(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_W2(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E2(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_I3(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_W3(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E3(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_I4(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2, HI_U32 ulData3);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_W4(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2, HI_U32 ulData3);
HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E4(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2, HI_U32 ulData3);
/**
* @ingroup  hct_diag
* @brief 用于上报指定buffer（如：报文内容）的内容到HSO的message界面中。
*
* @par 描述:
* 用于上报指定buffer（如：报文内容）的内容到HSO的message界面中。
*其中：HI_DIAG_LOG_BUF表示使用DIAG内部的缓存上报（长度受限于缓存剩余大小）。
*
* @attention 无。
*
* @param usFmtId [IN] LOG消息ID，日志消息字符串szFmt的标识。取值范围0～0x0FFF，在同一个文件中必须唯一使用数字常量，不能使用变量（编译器内置变量等）、宏定义。
* @param szFmt [IN] 字符串常量，字符串必须使用HI_DIAG_LMT宏括起，且不支持回车符。当前版本不支持参数格式化，仅直接显示该字符串。
* @param pLogBuffer[IN] 日志buffer。
* @param usLogBufSize[IN] 日志buffer长度，单位：byte。
*
* @retval #HI_ERR_NO_INITILIZATION 设备未初始化。
* @retval #HI_ERR_NO_INITILIZATION 设备未初始化。
* @retval #HI_ERR_QUEUE_FULL OS队列满，发送到后台任务失败。
* @retval #HI_ERR_CFG_NOT_ALLOW DIAG通道关闭导致不允许上报。
* @retval #HI_ERR_INVALID_PARAMETER 传参非法。
* @retval #HI_ERR_NOT_ENOUGH_MEMORY 无可用的缓存块使用或大数据块下内存分配失败。
* @retval 其他值 错误。
*
* @par Dependency:
* @li hi_mdm.h: 该接口声明所在的头文件。
* @see HI_DIAG_LOG_MSG_x
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_DIAG_LOG_BUF(HI_U16 usFmtId, HI_PCSTR szFmt, HI_PVOID pLogBuffer, HI_U16 usLogBufSize);
HI_EAPI HI_U32 HI_DIAG_LOG_BUFG(HI_U16 usFmtId, HI_PCSTR szFmt, HI_PVOID pLogBuffer, HI_U16 usLogBufSize);
HI_EAPI HI_U32 HI_DIAG_LOG_BUFD(HI_U16 usFmtId, HI_PCSTR szFmt, HI_PVOID pLogBuffer, HI_U16 usLogBufSize);

#if defined(PRODUCT_CFG_DIAG_SHELL_API) && !defined(PRODUCT_CFG_FEATURE_UT)
HI_EXTERN HI_U32 HI_DIAG_LogMsg0(HI_U32 ulModId, HI_U32 id);
HI_EXTERN HI_U32 HI_DIAG_LogMsg1(HI_U32 ulModId, HI_U32 id, HI_U32 d0);
HI_EXTERN HI_U32 HI_DIAG_LogMsg2(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1);
HI_EXTERN HI_U32 HI_DIAG_LogMsg3(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2);
HI_EXTERN HI_U32 HI_DIAG_LogMsg4(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2, HI_U32 d3);
HI_EXTERN HI_U32 HI_DIAG_LogBuffer(HI_U32 ulModId, HI_U32 id, HI_U16 usDataType, HI_PVOID pBuffer, HI_U16 usSize);

#define HI_DIAG_LOG_MSG_E0(id, sz)                     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg0(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id))
#define HI_DIAG_LOG_MSG_E1(id, sz, d0)                 HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg1(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), (HI_U32)(d0))
#define HI_DIAG_LOG_MSG_E2(id, sz, d0, d1)             HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg2(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), (HI_U32)(d0), (HI_U32)(d1))
#define HI_DIAG_LOG_MSG_E3(id, sz, d0, d1, d2)         HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg3(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2))
#define HI_DIAG_LOG_MSG_E4(id, sz, d0, d1, d2, d3)     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg4(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2), (HI_U32)(d3))
#if defined(PRODUCT_CFG_DIAG_SHELL_DISABLE_LOG_MSG_WI)
#define HI_DIAG_LOG_MSG_W0(id, sz)
#define HI_DIAG_LOG_MSG_I0(id, sz)
#define HI_DIAG_LOG_MSG_W1(id, sz, d0)
#define HI_DIAG_LOG_MSG_I1(id, sz, d0)
#define HI_DIAG_LOG_MSG_W2(id, sz, d0, d1)
#define HI_DIAG_LOG_MSG_I2(id, sz, d0, d1)
#define HI_DIAG_LOG_MSG_W3(id, sz, d0, d1, d2)
#define HI_DIAG_LOG_MSG_I3(id, sz, d0, d1, d2)
#define HI_DIAG_LOG_MSG_W4(id, sz, d0, d1, d2, d3)
#define HI_DIAG_LOG_MSG_I4(id, sz, d0, d1, d2, d3)
#else
#define HI_DIAG_LOG_MSG_W0(id, sz)                     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg0(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_W(id))
#define HI_DIAG_LOG_MSG_I0(id, sz)                     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg0(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_I(id))
#define HI_DIAG_LOG_MSG_W1(id, sz, d0)                 HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg1(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_W(id), (HI_U32)(d0))
#define HI_DIAG_LOG_MSG_I1(id, sz, d0)                 HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg1(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_I(id), (HI_U32)(d0))
#define HI_DIAG_LOG_MSG_W2(id, sz, d0, d1)             HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg2(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_W(id), (HI_U32)(d0), (HI_U32)(d1))
#define HI_DIAG_LOG_MSG_I2(id, sz, d0, d1)             HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg2(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_I(id), (HI_U32)(d0), (HI_U32)(d1))
#define HI_DIAG_LOG_MSG_W3(id, sz, d0, d1, d2)         HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg3(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_W(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2))
#define HI_DIAG_LOG_MSG_I3(id, sz, d0, d1, d2)         HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg3(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_I(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2))
#define HI_DIAG_LOG_MSG_W4(id, sz, d0, d1, d2, d3)     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg4(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_W(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2), (HI_U32)(d3))
#define HI_DIAG_LOG_MSG_I4(id, sz, d0, d1, d2, d3)     HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogMsg4(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_I(id), (HI_U32)(d0), (HI_U32)(d1), (HI_U32)(d2), (HI_U32)(d3))
#endif
#define HI_DIAG_LOG_BUF(id, sz, buffer, size)          HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogBuffer(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), HI_DIAG_USERPLANE_MSG_DT_LBUF, (HI_PVOID)(buffer), (HI_U16)(size))
#define HI_DIAG_LOG_BUFG(id, sz, buffer, size)         HI_DIAG_IS_CONNECT_CHK HI_DIAG_LogBuffer(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), HI_DIAG_USERPLANE_MSG_DT_GBUF, (HI_PVOID)(buffer), (HI_U16)(size))
#define HI_DIAG_LOG_BUFD(id, sz, buffer, size)         HI_DIAG_IS_CONNECT_CHK do {HI_PVOID pData = (HI_PVOID)HI_MDM_Malloc(HI_MOD_ID_SAL_UP_API, size);memcpy_s(pData, size, buffer, size); if(HI_DIAG_LogBuffer(HI_DIAG_LOG_MSG_MK_MOD_ID(id), HI_DIAG_LOG_MSG_MK_ID_E(id), HI_DIAG_USERPLANE_MSG_DT_DBUF, (HI_PVOID)(pData), (HI_U16)(size))) HI_MDM_Free(HI_MOD_ID_SAL_UP_API, pData);}while(0)
#else
#define HI_DIAG_LOG_MSG_E0(id, sz)
#define HI_DIAG_LOG_MSG_E1(id, sz, d0)
#define HI_DIAG_LOG_MSG_E2(id, sz, d0, d1)
#define HI_DIAG_LOG_MSG_E3(id, sz, d0, d1, d2)
#define HI_DIAG_LOG_MSG_E4(id, sz, d0, d1, d2, d3)
#define HI_DIAG_LOG_MSG_W0(id, sz)
#define HI_DIAG_LOG_MSG_I0(id, sz)
#define HI_DIAG_LOG_MSG_W1(id, sz, d0)
#define HI_DIAG_LOG_MSG_I1(id, sz, d0)
#define HI_DIAG_LOG_MSG_W2(id, sz, d0, d1)
#define HI_DIAG_LOG_MSG_I2(id, sz, d0, d1)
#define HI_DIAG_LOG_MSG_W3(id, sz, d0, d1, d2)
#define HI_DIAG_LOG_MSG_I3(id, sz, d0, d1, d2)
#define HI_DIAG_LOG_MSG_W4(id, sz, d0, d1, d2, d3)
#define HI_DIAG_LOG_MSG_I4(id, sz, d0, d1, d2, d3)
#define HI_DIAG_LOG_BUF(id, sz, buffer, size)
#define HI_DIAG_LOG_BUFG(id, sz, buffer, size)
#define HI_DIAG_LOG_BUFD(id, sz, buffer, size)
#endif
#endif
//*****************************************************************************
#endif
HI_END_HEADER

#endif
