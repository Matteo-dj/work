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
* @brief ע��DIAGͨ��״̬����ص�������
*
* @par ����:
* ע��DIAGͨ������ص�����(��:��DIAGͨ�����ӻ�Ͽ�ʱ����ص����ӿ�ע��ĺ���)��
* @attention �ޡ�
*
* @param  pfnUserConnect[IN] �û�������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_DiagRegisterConnect(HI_DIAG_CONNECT_F pfnUserConnect);


/**
* @ingroup  hct_diag
* @brief  �������ͨ��ռ�ú��ͷŵ��Դ��ڡ�
*
* @par ����:
* ͨ���ýӿڿ������ͨ��ռ�ú��ͷŵ��Դ��ڣ�DIAG��ʼ��֮��Ĭ��ռ�õ��Դ��ڡ�
* @attention
* @li �ýӿ���Ҫ��DIAG��ʼ�����֮����á�
* @li �ýӿڲ�֧�����ж��е��á�
*
* @param  bConnect[IN] HI_TRUE:ʹ��DIAG�������ͨ�� HI_FALSE:ȥ��DIAG�Ե��Դ��ڵ�ռ�á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_DiagUartPortCtrl(HI_BOOL bConnect);


/**
* @ingroup  hct_diag
* @brief  ע�����������
*
* @par ����:
* @attention
* @li �ڳ�ʼ���׶ε��ã���֧�ֶ�������á�
* @li �����ע��20����ͬ�������
* @li �����������ͬ�������ã����Ǵ��еģ���Ҫ��ע������������
*����ʱ�価���̣����ܶ�������������գ�Ŀǰ���ô���ʱΪ5s��
* @li ��ͬʵ��IDע������ID������ͬ��ͬһʵ��ע�������ID����ΪΨһ������ע��ʧ�ܡ�
*
* @param  pstCmdTbl[IN] �������������Ϊ�������鴫���ò�����
* @param  usCmdNum[IN] ������������ usCmdNum Ϊ�㣬�� pstCmdTbl��Ϊ����ȡ��ע�ᡣ
* @param  ucInstanceId[IN] ʵ��ID��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterCommand(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId);

/**
* @ingroup  hct_diag
* @brief  �����û��Զ���DIAG������в������ܡ�
*
* @par ����:
* ע�Ტ��������ID,Ĭ�����������֮ǰ������ڵ�һ������ִ�����֮ǰ�յ�����HSO�ڶ��������᷵��chanel busy��
* @attention �ޡ�
*
* @param  usMinId [IN] ����ID���ޡ�
* @param  usMaxId [IN] ����ID���ޡ�
* @param  pfnGetPkCache [IN] �ڴ�ռ����뺯����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterParallelCmd(HI_U16 usMinId, HI_U16 usMaxId, HI_DIAG_GET_CMD_PK_CACHE_F pfnGetPkCache);

/**
* @ingroup  hct_diag
* @brief  ע��ϵͳ���ͳ������
*
* @par ����:
* �ú���������DIAGͨ��ע��ͳ������DIAGͨ���ᶨ���ϱ���ע���ͳ������
*ͬʱͳ����֧�ֲ�ѯ��
* @attention �ýӿڲ�֧�����ж��е��á�
*
* @param  pstSysStatObjTbl [IN] ͳ���������б�ÿ��ͳ�������������ֵΪ�ṹ��#HI_DIAG_SYS_STAT_OBJ_S�ĳ�Ա��
* @param  usObjNum [IN] ��ע��ͳ�����ĸ�������ͳ���������б��а���ͳ������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RegisterSysStat(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum);

/**
* @ingroup  hct_diag
* @brief  DIAG���ϱ���
*
* @par ����:
* �ú������ڽ�DIAGͨ�������ر�ͨ���ϱ���DIAG�ͻ��ˡ�
* @attention �ýӿڲ�֧�����ж��е��á�

* @param  ulId [IN] DIAGӦ���ID��
* @param  ucInstanceId [IN] ʵ��ID��
* @param  pucPacket [IN] ���ݰ���buffer��ַ���ú��������ͷŸ�ָ�롣
* @param  usPacketSize [IN] ���ݰ���С����λ:byte.
* @param  bSync[IN] DIAG��ͬ��/�첽�ϱ����á�TRUE��ʾͬ���ϱ�������������
* FALSE��ʾ�첽(ͨ�������ڴ����OS���л������ϱ�)��������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ReportPacket(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);

/**
* @ingroup  hct_diag
* @brief  Ӧ��ظ���
*
* @par ����:
* �ú������ڻظ�ACK��DIAG�ͻ��ˡ�
* @attention
* @li �ýӿڲ�֧�����ж��е��á�
* @li �������ͬ����ǰ���£�����ʹ��HI_MDM_ReportPacket�ӿڡ�
*
* @param  ulId [IN] DIAG��ID��
* @param  pBuffer [IN] buffer��ַ��
* @param  usBufferSize [IN] buffer��С����λ:byte��
* @param  bSync [IN] Ӧ��ظ�ͬ��/�첽�ϱ����ã�ȡֵ����:
* @li TURE��ʾͬ�����͸�DIAG�ͻ��ˣ�ͨ��OS���л��档
* @li FALSE��ʾ�첽���͸�DIAG�ͻ��ˣ���ͨ��OS���л��档
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SendAckPacket(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync);

/**
* @ingroup  hct_diag
* @brief  ϵͳ��Ϣ�ϱ��ӿڡ�
*
* @par ����:
* �ú��������ϱ�ϵͳ���Ĳ��/ģ������Ϣ(����CCO��Ʒ��̬��֧��)��
* @attention �ýӿڲ�֧�����ж��е��á�
* @param  usId [IN] ϵͳ��ϢID��
* @param  usSrcModId [IN] Դģ��ID��0��ʾ��ָ��ģ��ID(��HI_MDM_Malloc��ʹ��ģ��ID��ͬ)��
* @param  usDestModId [IN] Ŀ��ģ��ID��0��ʾ��ָ��ģ��ID(��HI_MDM_Malloc��ʹ��ģ��ID��ͬ)��
* @param  pPacket [IN] ��Ϣ��ָ�롣
* @param  usPacketSize [IN] ��Ϣ����С��
* @param  ulOption [IN] ȡֵ����
* @li HI_SYS_DMID_BUSINESS_L1 ��ʾҵ����ȼ�Ϊ1����Ϣ��
* @li HI_SYS_DMID_BUSINESS_L2 ��ʾҵ����ȼ�Ϊ2����Ϣ��
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption);

/**
* @ingroup  hct_diag
* @brief  ϵͳ��ʼ���¼��ϱ���
*
* @par ����:
* �ú��������ϱ���ʼ���¼������¼������ڴ��л��棬
*������Ϊδ����HSO�����������ڴ�Message����ʱ���Զ��ϱ���
* @attention �ޡ�
*
* @param  usEvtId [IN] ��ʼ���¼�ID��
* @param  ucOption [IN] ��ʼ���¼�ѡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReportInitEvt(HI_U16 usEvtId, HI_U8 ucOption);

/**
* @ingroup  hct_diag
* @brief  �ϱ�ϵͳ�¼���
*
* @par ����:
* �ú��������ϱ�ϵͳ���Ĵ���͸澯�¼���
* @attention �ýӿڲ�֧�����ж��е��á�
*
* @param  usEvtId [IN] ��ϵͳ�¼�ID��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReportSysEvent(HI_U16 usEvtId);

/**
* @ingroup  hct_diag
* @brief  ע��DIAG����֪ͨ�ص�������
*
* @par ����:
* �ú������ڵ�HSO�뵥�������ݽ�������ʱ��������ͨ���ص��������͸�Ӧ�ò㡣
* @attention �ޡ�
*
* @param  usrId [IN] �û�ID��
* @param  func [IN] ��HSO�뵥�������ݽ�������ʱ��ͨ���ýӿ�֪ͨӦ�ò㡣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegistDiagCmdNotify(HI_U32 usrId, HI_DIAG_CMD_NOTIFY_F *func);

/**
* @ingroup  hct_diag
* @brief  ��������DIAG���
*
* @par ����:
* �ú�������Ӧ�ò��·�DIAG��������ڲ�����HSO������
* @attention �ýӿڲ�֧�����ж��е��á�
*
* @param  cmdId [IN] ����ID��
* @param  ulOption [IN] ����ѡ�
* @param  pData[IN] ���������
* @param  ulDataSize [IN] ���������С��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RunDiagCmd(HI_U32 cmdId,HI_U32 ulOption,HI_PVOID pData,HI_U16 ulDataSize);

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

//*****************************************************************************
// ��������: HI_DIAG_LOG_MSG_E4 HI_DIAG_LOG_MSG_W4 HI_DIAG_LOG_MSG_I4 HI_DIAG_LOG_BUF
// ��������:
//           Report the UserPlane message to 'User Plane Message Viewer' on PC side.
// ����˵��:
//           usFmtId  [in] LOG��Ϣ�ı�ID, ��־��Ϣ�ַ���sz�ı��ʶ. ȡֵ��Χ0-0x0FFF,��ͬһ���ļ��б���Ψһ����ʹ�����ֳ���(���ǽ�βΪ�������ֵ�����)
//           szFmt    [in] �ַ�������, ��֧�ֻس���. ������ʽ����ǰ�汾��֧��,��ֱ����ʾ���ַ���
//           ulData0  [in] 32bit����0
//           ulData1  [in] 32bit����1
//           ulData2  [in] 32bit����2
//           ulData3  [in] 32bit����3
//           pLogBuffer    [in] ��־buffer
//           usLogBufSize  [in] ��־buffer����, ��λ�ֽ�
// �� �� ֵ:
//           ��
//           HI_ERR_NO_INITILIZATION   �豸δ��ʼ��
//           HI_ERR_QUEUE_FULL         OS������,���͵���̨����ʧ��
//           HI_ERR_CFG_NOT_ALLOW      DIAGͨ�����رյ��²������ϱ�
//           HI_ERR_INVALID_PARAMETER  ���ηǷ�
//           HI_ERR_NOT_ENOUGH_MEMORY  �޿��õĻ����ʹ��,���Ǵ����ݿ����ڴ����ʧ��
//           HI_ERR_FAILURE            δ֪����
//
// ����Ҫ��: 1) ulData0/ulData1/ulData2/ulData3���Ϊ��̬������ڴ����ɵ������ͷ�.
//           2) szFmt ��֧�ֻس���.
//           3) usFmtId ��ͬһ���ļ��б���Ψһ�ұ��������ֳ���(���ǽ�βΪ�������ֵ�����),���usFmtId����Ϊ0, ���ʾ�ýӿ��Զ�ȡֵʹ�õ�ǰ�к�.
//           4) HI_DIAG_LOG_MSG_XX  usFmtId szFmt ����д��ͬһ����.
// ���þ���:
//           HI_DIAG_LOG_MSG_W1(MY_ANY_MT_ID_101, "print warning message, this is v1=%d,2=%d,3=%d,4=%d", v1)
//           HI_DIAG_LOG_MSG_E4(0x1FF, "print error message 4, this is v1=%d,2=%d,3=%d,4=%d", v1,v2,v3,v4)
//           HI_DIAG_LOG_MSG_E4(MY_ANY_MT_ID_102, "print error message 4, this is v1=%d,2=%d,3=%d,4=%d"
//                              , v1,v2,v3,v4)
// ��    ��: ���ɸ�/00149569/20090903
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
* @brief  �����ϱ���־��Ϣ�����ı���Ϣ��ʽ+��������HSO��message�����С�
*
* @par ����:
* �����ϱ���־��Ϣ�����ı���Ϣ��ʽ+��������HSO��message�����У�Ŀǰ��֧���޷������͵Ĳ�����
*
* @attention HI_DIAG_LOG_MSG_E0�ӿ����е�"E0"��ʵ��ʹ���и�����Ҫ���滻Ϊ������Чֵ�����ṩ���½ӿ�:
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E0(HI_U16 usFmtId, HI_PCSTR szFmt)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E1(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E2(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E3(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2)
* @li HI_EAPI HI_U32 HI_DIAG_LOG_MSG_E4(HI_U16 usFmtId, HI_PCSTR szFmt, HI_U32 ulData0, HI_U32 ulData1, HI_U32 ulData2, HI_U32 ulData3)
*
* @param usFmtId [IN] LOG��ϢID����־��Ϣ�ַ���szFmt�ı�ʶ��ȡֵ��Χ0��0x0FFF����ͬһ���ļ��б���Ψһʹ�����ֳ���������ʹ�ñ��������������ñ����ȣ����궨�塣
* @param szFmt [IN] �ַ����������ַ�������ʹ��HI_DIAG_LMT�������Ҳ�֧�ֻس�������ǰ�汾��֧�ֲ�����ʽ������ֱ����ʾ���ַ�����
* @param ulDatax[IN] ���ݵĲ�����
*
* @retval #HI_ERR_NO_INITILIZATION �豸δ��ʼ����
* @retval #HI_ERR_NO_INITILIZATION �豸δ��ʼ����
* @retval #HI_ERR_QUEUE_FULL OS�����������͵���̨����ʧ�ܡ�
* @retval #HI_ERR_CFG_NOT_ALLOW DIAGͨ���رյ��²������ϱ���
* @retval #HI_ERR_INVALID_PARAMETER ���ηǷ���
* @retval #HI_ERR_NOT_ENOUGH_MEMORY �޿��õĻ����ʹ�û�����ݿ����ڴ����ʧ�ܡ�
* @retval ����ֵ ����
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
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
* @brief �����ϱ�ָ��buffer���磺�������ݣ������ݵ�HSO��message�����С�
*
* @par ����:
* �����ϱ�ָ��buffer���磺�������ݣ������ݵ�HSO��message�����С�
*���У�HI_DIAG_LOG_BUF��ʾʹ��DIAG�ڲ��Ļ����ϱ������������ڻ���ʣ���С����
*
* @attention �ޡ�
*
* @param usFmtId [IN] LOG��ϢID����־��Ϣ�ַ���szFmt�ı�ʶ��ȡֵ��Χ0��0x0FFF����ͬһ���ļ��б���Ψһʹ�����ֳ���������ʹ�ñ��������������ñ����ȣ����궨�塣
* @param szFmt [IN] �ַ����������ַ�������ʹ��HI_DIAG_LMT�������Ҳ�֧�ֻس�������ǰ�汾��֧�ֲ�����ʽ������ֱ����ʾ���ַ�����
* @param pLogBuffer[IN] ��־buffer��
* @param usLogBufSize[IN] ��־buffer���ȣ���λ��byte��
*
* @retval #HI_ERR_NO_INITILIZATION �豸δ��ʼ����
* @retval #HI_ERR_NO_INITILIZATION �豸δ��ʼ����
* @retval #HI_ERR_QUEUE_FULL OS�����������͵���̨����ʧ�ܡ�
* @retval #HI_ERR_CFG_NOT_ALLOW DIAGͨ���رյ��²������ϱ���
* @retval #HI_ERR_INVALID_PARAMETER ���ηǷ���
* @retval #HI_ERR_NOT_ENOUGH_MEMORY �޿��õĻ����ʹ�û�����ݿ����ڴ����ʧ�ܡ�
* @retval ����ֵ ����
*
* @par Dependency:
* @li hi_mdm.h: �ýӿ��������ڵ�ͷ�ļ���
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
