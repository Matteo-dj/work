//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_tools.c
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2011-12-08
//  ��������   : ����֡��ʽ��Ҫ��һЩ����С���� 
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-08
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_res.h"
#include <hi_mac_nv.h>
#include "mrs_fw_n.h"

HI_VOID MRS_TimerCallback(HI_U32 ulData);

//*****************************************************************************
// ��������: mrsToolsCalcCheckSum
// ��������: ����У���     
// ����˵��:
//   HI_PBYTE [IN] ��������ʼ��ַ
//   HI_U16   [IN] �����Ƴ���
// �� �� ֵ: 
//    ����У���룬
//       ע�⣺mrsToolCalcCheckSum�Բ���������������,�����ؼ����0��
//    ��ȷ��У���Ҳ�п��ܷ���0!!!
// ����Ҫ��: 
// ���þ���: 
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U8 mrsToolsCalcCheckSum(HI_PBYTE ptr, HI_U16 len)
{
    return (HI_U8)mrsToolsCalcCheckSum2(ptr, len);
}

//*****************************************************************************
// ��������: mrsToolsGetLocalMac
// ��������: ��ȡ��ģ���MAC��ַ     
// ����˵��:
//   HI_U8 [] [OUT] ��ȡ��ģ���MAC��ַ
// �� �� ֵ: 
//   MAC��ַ(����ֵ��Ϊ����ʽ���)
// ����Ҫ��: 
// ���þ���: 
// ��    ��: niesongsong/kf62735 [2012-08-29]
//*****************************************************************************
HI_U8 * mrsToolsGetLocalMac(HI_U8 mac[])
{
    HI_U32 ret;
    ret = HI_MDM_GetLocalMacAddr(mac);
    if (ret == HI_ERR_SUCCESS) 
    {
        return mac;
    }

    return HI_NULL;
}

//*****************************************************************************
// ��������: mrsToolsCalcCheckSum2
// ��������: ����У��16λ��
// ����˵��:
//   HI_PBYTE [IN] ��������ʼ��ַ
//   HI_U16   [IN] �����Ƴ���
// �� �� ֵ: 
//    ����У���룬
//       ע�⣺mrsToolsCalcCheckSum2�Բ���������������,�����ؼ����0��
//    ��ȷ��У���Ҳ�п��ܷ���0!!!
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/kf51842 [2012-07-09]
//*****************************************************************************
HI_U16 mrsToolsCalcCheckSum2(HI_PBYTE ptr,HI_U16 len)
{
    HI_U16 CheckSum = 0;

    if ((ptr != HI_NULL) && (len > 0))  
    {
        HI_U16 i = 0;
        for(i = 0; i < len; i++)
        {
            CheckSum += ptr[i];
        }
    }
    return CheckSum;
}

//*****************************************************************************
// ��������: mrsToolsStreamFindByte
// ��������: �Ӷ�����������Ŀ���ַ�     
// ����˵��:
//   HI_PBYTE   [IN] ����ʼ
//   HI_U16     [IN] ������
//   HI_U8      [IN] Ŀ���ַ�
// �� �� ֵ: 
//    ����ҵ��򷵻������еĵ�ַ�����򷵻ؿ�
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_PBYTE mrsToolsStreamFindByte(HI_PBYTE ptr,HI_U16 len,HI_U8 v)
{
    HI_U16 i = 0;
    
    if (ptr != HI_NULL)
    {
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == v)
            {
                return ptr + i;
            }
        }
    }

    return HI_NULL;    
}

//*****************************************************************************
// ��������: mrsToolsCheck645Frame
// ��������: ���645֡�Ϸ���    
// ����˵��:
//   HI_PBYTE [IN] ������ַ     
//   HI_U16 [IN] ���泤��
// �� �� ֵ: 
//    �ɹ�����HI_ERR_SUCCESS,ʧ�ܷ���HI_ERR_BAD_DATA
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U32 mrsToolsCheck645Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8 checkSum;

    //֡�����
    if ((!ptr) || 
        (len > MRS_PROTO645_DATAGRAM_LEN_MAX) || 
        (len < MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //֡��־���
    if ((ptr[0] != 0x68) || (ptr[7] != 0x68) || (ptr[len-1] != 0x16)) 
    {
        return HI_ERR_BAD_DATA;
    }
    
    //У��ͼ��
    checkSum = mrsToolsCalcCheckSum(ptr,(HI_U16)(len-2));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }
    
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsToolsCheck3762Frame
// ��������: ���3762֡�Ϸ���    
// ����˵��:
//   HI_PBYTE [IN] ������ַ     
//   HI_U16 [IN] ���泤��
//   HI_U32 [IN] ֡���
// �� �� ֵ: 
//    �ɹ�����HI_ERR_SUCCESS,ʧ�ܷ���HI_ERR_BAD_DATA
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U32 mrsToolsCheck3762Frame(HI_PBYTE ptr,HI_U16 len, HI_U16 type)
{
    HI_U8  checkSum;
    HI_U16 frameLen;
    HI_U16 minLen = (type != MRS_CCO_FRAME_TYPE_3762) ? MRS_CSG_FRAME_MIN_SIZE : MRS_3762_FRAME_MIN_SIZE;
    
    //֡�����
    //֡���Ȳ��ܴ���2048������С��minLen
    if (!ptr || (len > PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX) || (len < minLen)) 
    {
        return HI_ERR_BAD_DATA;
    }

    //֡��־���
    if ((ptr[0] != 0x68) || (ptr[len-1] != 0x16)) 
    {
        return HI_ERR_BAD_DATA;
    }

    //У���
    checkSum = mrsToolsCalcCheckSum(ptr+3,(HI_U16)(len-5));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }

    //���Ӽ��
    //3762֡���г�������ʵ�ʴ������Ա�
    (hi_void)memcpy_s(&frameLen, sizeof(frameLen), ptr+1, sizeof(frameLen));
    if (frameLen != len) 
    {
       return HI_ERR_BAD_DATA;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsToolsCheckBCD
// ��������: ����������Ƿ���BCD��  
// ����˵��:
//   HI_PBYTE [IN] ������ַ     
//   HI_U16 [IN] ���泤��
// �� �� ֵ: 
//    ����������������߲���BCD������HI_FALSE�������������HI_TRUE
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_BOOL mrsToolsCheckBCD(HI_PBYTE ptr,HI_U16 len)
{
    if (ptr == HI_NULL) 
    {
        return HI_FALSE;
    }
    while (len-- > 0) 
    {
        if (((ptr[len] & 0x0f) > 9) || ((ptr[len] & 0xf0) > 0x90))
        {
            return HI_FALSE;
        }   
    }
    return HI_TRUE;
}

//*****************************************************************************
// ��������: mrsToolsCheckBCDEx
// ��������: ����������Ƿ���BCD��(����645֡��������ж�)
// ����˵��:
//   HI_PBYTE [IN] ������ַ     
//   HI_U16 [IN] ���泤��
// �� �� ֵ: 
//    ����������������߲���BCD������HI_FALSE�������������HI_TRUE
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: liuxipeng/lKF54842 [2013-12-18]
//*****************************************************************************
HI_BOOL mrsToolsCheckBCDEx(HI_PBYTE ptr,HI_U16 len)
{
    if (ptr == HI_NULL) 
    {
        return HI_FALSE;
    }
    while (len-- > 0) 
    {
        if ((((ptr[len] - 0x33) & 0x0f) > 9) || (((ptr[len] - 0x33) & 0xf0) > 0x90))
        {
            return HI_FALSE;
        }   
    }
    return HI_TRUE;
}


//*****************************************************************************
// ��������: mrsToolsGet645Ver
// ��������: ���ݹ����뷵��645֡�İ汾
// ����˵��:
//   HI_U8 [IN] ������
// �� �� ֵ: 
//    �汾1997����2007��Ĭ��Ϊ1997
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U8 mrsToolsGet645Ver(HI_U8 ucFn)
{
    HI_U8 ret = MRS_PROTO645_VER_2007;
    
    if (!(ucFn & 0x10) && (ucFn != MRS_COLLECTOR_07_PROTOCOL_CONTROL) 
        && (ucFn != MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL))
    {
        ret = MRS_PROTO645_VER_1997;
    }

    return ret;
}


HI_BOOL mrsToolsMemIsXX(HI_U8* pucBuf, HI_U32 ulBufLen, HI_U8 ucCh)
{
    HI_U32 i = 0;

    if (!pucBuf)
    {
        return HI_FALSE;
    }

    for (i = 0; i < ulBufLen; i++)
    {
        if (ucCh != *(pucBuf + i))
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


// �жϸ�MAC��ַ�Ƿ��ɵ����ת������ (��:HI_TRUE; ��:HI_FALSE)
HI_BOOL mrsToolsIsMacAddrFromMeter(HI_U8* pucMac)
{
    HI_U8 aucTempMac[HI_PLC_MAC_ADDR_LEN] = { 0 };

    (hi_void)memcpy_s(aucTempMac, sizeof(aucTempMac), pucMac, HI_PLC_MAC_ADDR_LEN);    
    MRS_TOOLS_FE_TO_00(aucTempMac[MRS_MAC_ADDR_INDEX_5]);

    return mrsToolsCheckBCD(aucTempMac, HI_PLC_MAC_ADDR_LEN);
}


// ���ַת��ΪMAC��ַ
// ��α��ַ˳�����Ϊ645����˳��
// ��α��������Լ����
// ʹ����ͨ������ֵ�ж��Ƿ�ת���ɹ�
HI_U32 mrsToolsConvertMeter2Mac(HI_IN HI_U8 aucMeter[HI_METER_ADDR_LEN],  
                                           HI_IN HI_U8 ucProto, 
                                           HI_OUT HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN])
{
    mrsMeterAddrPreProc(aucMac, aucMeter, ucProto);
    mrsHexInvert(aucMac, HI_PLC_MAC_ADDR_LEN);
    MRS_TOOLS_00_TO_FE(aucMac[MRS_MAC_ADDR_INDEX_5]);

    return HI_ERR_SUCCESS;
}
/*������ת���ɶ���BCD��,������*/
HI_U32 mrsIntToBcd(HI_U32 data,HI_U8 bcd[],HI_U8 len)
{
    HI_U32 i = 0;
    HI_U32 remain = (HI_U32)data;
    HI_U32 tail = 0;
    
    if (!bcd) 
    {
        return (HI_U32)-1;
    }

    for (i = 0; i < len; i++)
    {
        tail = remain % 100;
        remain /= 100;
        
        bcd[i] = (HI_U8)(tail / 10 * 16 + tail % 10);
    }
    return 0;
}

// HEX��������
HI_VOID mrsHexInvert(HI_U8 * buf, HI_U16 len)
{
    HI_U8 a = 0;
    HI_U16 i = 0;
    
    for(i = 0; i < len / 2; i++)
    {
        a = buf[i];
        buf[i] = buf[len - i - 1];
        buf[len - i - 1] = a;
    }

    return;
}

//*****************************************************************************
// ��������: mrsFindStrEnd
// ��������:���ز��ҵ����ַ�������ͬ�ַ���λ�õ�β��
//           
// ����˵��:
//    pStr1 �����ҵ��ַ���
//    pStr2 Ҫ���ҵ��ַ���
//
// �� �� ֵ: 
//    �汾��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
HI_CHAR* mrsFindStrEnd(HI_CHAR* pStr1, HI_CHAR* pStr2)
{
    HI_CHAR * pStr = HI_NULL;
    HI_U32 ulStr2Len = 0;
    
    if (!pStr1 || !pStr2)
    {
        return HI_NULL;
    }

    ulStr2Len = (HI_U32)strlen(pStr2);
    
    while (*pStr1)
    {
        if ((HI_U32)strlen(pStr1) < ulStr2Len)
        {
            return HI_NULL;
        }
        
        if(!memcmp(pStr1, pStr2, ulStr2Len))
        {
            pStr = (*(pStr1 + ulStr2Len) != '\0') ? (pStr1 + ulStr2Len) : HI_NULL;
            break;
        }

        pStr1++;
    }
    
    return pStr;
}
        
//*****************************************************************************
// ��������: mrsStrToInt
// ��������:���ַ���ת��������
//           
// ����˵��:
//    pStr ��ת�����ַ���
//    ucType ת���Ľ�������
//
// �� �� ֵ: 
//    ת���������
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
HI_U32 mrsStrToInt(HI_CHAR* pStr, HI_U8 ucType)
{
    HI_U32 ulData = 0;

    HI_UNREF_PARAM(ucType);

    if (!pStr)
    {
        return ulData;
    }
    
    while(mrsIsLetterValid(pStr) && (*pStr != '\0'))
    {
        ulData = ucType * ulData + (HI_U8)(*pStr - '0');
        pStr++;
    }

    return ulData;
}

//*****************************************************************************
// ��������: mrsIsLetterValid
// ��������: �ж��Ƿ��ǺϷ�����
//           
// ����˵��:
//    pStr ���жϵ��ַ���
//    ucType �жϵĽ�����������
//
// �� �� ֵ: 
//    TRUE  �Ϸ�
//    FALSE �Ƿ�
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
HI_BOOL mrsIsLetterValid(HI_CHAR *pStr)
{
    if (!pStr)
    {
        return HI_FALSE;
    }
    
    return (HI_BOOL)((*pStr >= '0') && (*pStr <= '9'));
}

HI_BOOL mrsIsLetterValidHex(HI_CHAR *pStr)
{
    if (!pStr)
    {
        return HI_FALSE;
    }

    return (HI_BOOL)(((*pStr >= 'A') && (*pStr <= 'F')) || ((*pStr >= 'a') && (*pStr <= 'f')));
}


#define MRS_TIMER_INTERVAL_MIN  (10)

HI_VOID MRS_TimerCallback(HI_U32 ulData)
{
    if (ulData < EN_APP_TIMER_MAX)
    {
        HI_SYS_QUEUE_MSG_S stMsg;

        (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
        stMsg.ulMsgId = ID_MRS_MSG_MR_TIMER;
        stMsg.ulTimeStamp = HI_MDM_GetMilliSeconds();
        stMsg.ulParam[0] = ulData;

        (HI_VOID)MRS_SendMessage(EN_APP_QUEUE_MRS, &stMsg, 0);
    }
}


HI_U32 MRS_StartTimer(HI_U8 ucTimerId, HI_U32 ulPeriod, HI_U8 ucOption)
{
    MRS_FW_CTX_STRU * pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ucTimerId >= EN_APP_TIMER_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    if (ulPeriod < MRS_TIMER_INTERVAL_MIN)
    {
        ulPeriod = MRS_TIMER_INTERVAL_MIN;
    }

    return HI_MDM_TIMER_Start(&pstFwCtx->stResCfg.astTimerHandle[ucTimerId], MRS_TimerCallback, ulPeriod, (HI_TIMER_TYPE_E)ucOption, ucTimerId);
}


HI_U32 MRS_StopTimer(HI_U8 ucTimerId)
{
    MRS_FW_CTX_STRU * pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ucTimerId >= EN_APP_TIMER_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    return HI_MDM_TIMER_Stop(&pstFwCtx->stResCfg.astTimerHandle[ucTimerId]);
}

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT))
HI_BOOL mrsToolCheckBbu(HI_U8 ucFuncCfg)
{
    return (HI_BOOL)(ucFuncCfg == MRS_CCO_FUNC_BBU);
}

HI_BOOL mrsToolIsBbu(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    return (HI_BOOL)(pstCcoCtx->ucFuncCfg == MRS_CCO_FUNC_BBU);
}

HI_BOOL mrsToolsCheckBroadCast(HI_PBYTE ptr,HI_U16 len)
{
    if ((ptr == HI_NULL) || (len != MRS_BOARDCAST_FRAME_LEN)) 
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("param invalid"));
        return HI_FALSE;
    }

    // �����ַ�Ƿ�ȫ0x99, ��������
    if (!mrsToolsBroadCastAddr(ptr + 1)
        || (MRS_BOARDCAST_TIMEING_CONTROL != (ptr[8] & 0x1F)))
    {
        return HI_FALSE;
    }
    
    return HI_TRUE;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
// ��������: mrsToolsGetTimeOut
// ��������: ��ȡ14F1��06F2��06F4��06F5��06F10����֡��ʱʱ�䣨����ģ��ͨ������״̬��ȡ��
//           
// ����˵��:
//   ulSimuChlClosedTimeOut [in] ģ��ͨ���ر�ʱ��ʱʱ���������λms��
//
// �� �� ֵ: 
//    ��ʱʱ�䣨��λms�� 
//
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: jirui/00203265 [2016-02-09]
//*****************************************************************************
HI_U32 mrsToolsGetTimeOut(HI_U32 ulSimuChlClosedTimeOut)
{
	if (HI_MDM_DfxGetHostCtrl() == HI_NULL)
	{
		return MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT;     
	}
	
    if (HI_MDM_DfxGetHostCtrl()->ucMrRedirect2HsoPort == 0)
    {
        return ulSimuChlClosedTimeOut;
    }
    return MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT;     
}
#endif


HI_BOOL mrsIsSimuChlOpened(HI_VOID)
{
	if (HI_MDM_DfxGetHostCtrl() == HI_NULL)
	{
        return HI_FALSE;
	}
	
    if (HI_MDM_DfxGetHostCtrl()->ucMrRedirect2HsoPort == 0)
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}


HI_BOOL mrsIsMeterMatch(HI_U8* pucMeter, HI_U8* pucCmpMeter)         
{
    if (mrsMeterAddrMatch(pucMeter, pucCmpMeter) || mrsMeterAddrMatch(pucCmpMeter, pucMeter))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_BOOL mrsSrvVerifyMac(HI_U8 *pucMac)
{
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 aucIp[HI_IPV4_ADDR_LEN] = {0};

    if (mrsToolsBroadCastMac(pucMac))
    {
        return HI_TRUE;
    }

    (HI_VOID)HI_MDM_NM_GetAddr(aucMac, aucIp);

    return (HI_BOOL)mrsToolsMemEq(aucMac, pucMac, HI_PLC_MAC_ADDR_LEN);
}
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */


