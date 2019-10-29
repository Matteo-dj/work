//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_tools.c
//  版 本 号   : V1.0
//  作    者   : 聂松松/KF62735
//  生成日期   : 2011-12-08
//  功能描述   : 解析帧格式需要的一些工具小函数
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2011-10-08
//    作    者 : 聂松松/KF62735
//    修改内容 : 创建文件
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_res.h"
#include "mrs_fw_nv.h"
#include "mrs_fw_n.h"

HI_VOID MRS_TimerCallback(HI_U32 ulData);

//*****************************************************************************
// 函数名称: mrsToolsCalcCheckSum
// 功能描述: 计算校验和
// 参数说明:
//   HI_PBYTE [IN] 二进制起始地址
//   HI_U16   [IN] 二进制长度
// 返 回 值:
//    返回校验码，
//       注意：mrsToolCalcCheckSum对参数错误不做特殊标记,将返回检验和0，
//    正确的校验和也有可能返回0!!!
// 调用要求:
// 调用举例:
// 作    者: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U8 mrsToolsCalcCheckSum(HI_PBYTE ptr, HI_U16 len)
{
    return (HI_U8)mrsToolsCalcCheckSum2(ptr, len);
}

//*****************************************************************************
// 函数名称: mrsToolsGetLocalMac
// 功能描述: 获取本模块的MAC地址
// 参数说明:
//   HI_U8 [] [OUT] 获取本模块的MAC地址
// 返 回 值:
//   MAC地址(输入值，为了链式表达)
// 调用要求:
// 调用举例:
// 作    者: niesongsong/kf62735 [2012-08-29]
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
// 函数名称: mrsToolsCalcCheckSum2
// 功能描述: 计算校验16位和
// 参数说明:
//   HI_PBYTE [IN] 二进制起始地址
//   HI_U16   [IN] 二进制长度
// 返 回 值:
//    返回校验码，
//       注意：mrsToolsCalcCheckSum2对参数错误不做特殊标记,将返回检验和0，
//    正确的校验和也有可能返回0!!!
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/kf51842 [2012-07-09]
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
// 函数名称: mrsToolsStreamFindByte
// 功能描述: 从二进制流查找目标字符
// 参数说明:
//   HI_PBYTE   [IN] 流起始
//   HI_U16     [IN] 流长度
//   HI_U8      [IN] 目标字符
// 返 回 值:
//    如果找到则返回在流中的地址，否则返回空
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/kf62735 [2011-12-13]
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
// 函数名称: mrsToolsCheck645Frame
// 功能描述: 检测645帧合法性
// 参数说明:
//   HI_PBYTE [IN] 缓存起址
//   HI_U16 [IN] 缓存长度
// 返 回 值:
//    成功返回HI_ERR_SUCCESS,失败返回HI_ERR_BAD_DATA
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U32 mrsToolsCheck645Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8 checkSum;

    //帧长检测
    if ((!ptr) ||
        (len > MRS_PROTO645_DATAGRAM_LEN_MAX) ||
        (len < MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }

    //帧标志检查
    if ((ptr[0] != 0x68) || (ptr[7] != 0x68) || (ptr[len-1] != 0x16))
    {
        return HI_ERR_BAD_DATA;
    }

    //校验和检查
    checkSum = mrsToolsCalcCheckSum(ptr,(HI_U16)(len-2));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsToolsCheck3762Frame
// 功能描述: 检测3762帧合法性
// 参数说明:
//   HI_PBYTE [IN] 缓存起址
//   HI_U16 [IN] 缓存长度
// 返 回 值:
//    成功返回HI_ERR_SUCCESS,失败返回HI_ERR_BAD_DATA
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U32 mrsToolsCheck3762Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8  checkSum;
    HI_U16 frameLen;

    //帧长检测
    //帧长度不能大于2048，不能小于
    //固定长度=0x68 + 长度 + 控制域 + 信息域 + AFN + FN + CS + 0x16
    ///1 + 2 + 1 + 6 + 1 + 2 + 1 + 1 = 15
    if (!ptr || (len > PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX) || (len < 15))
    {
        return HI_ERR_BAD_DATA;
    }

    //帧标志检查
    if ((ptr[0] != 0x68) || (ptr[len-1] != 0x16))
    {
        return HI_ERR_BAD_DATA;
    }

    //校验和
    checkSum = mrsToolsCalcCheckSum(ptr+3,(HI_U16)(len-5));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }

    //附加检查
    //3762帧中有长度域，与实际传参做对比
    (hi_void)memcpy_s(&frameLen, sizeof(frameLen), ptr+1, sizeof(frameLen));
    if (frameLen != len)
    {
       return HI_ERR_BAD_DATA;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsToolsCheckBCD
// 功能描述: 检测数据流是否是BCD流
// 参数说明:
//   HI_PBYTE [IN] 缓存起址
//   HI_U16 [IN] 缓存长度
// 返 回 值:
//    如果传入参数有误或者不是BCD流返回HI_FALSE，其它情况返回HI_TRUE
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/kf62735 [2012-01-05]
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
// 函数名称: mrsToolsCheckBCDEx
// 功能描述: 检测数据流是否是BCD流(用于645帧数据域的判断)
// 参数说明:
//   HI_PBYTE [IN] 缓存起址
//   HI_U16 [IN] 缓存长度
// 返 回 值:
//    如果传入参数有误或者不是BCD流返回HI_FALSE，其它情况返回HI_TRUE
// 调用要求: 无
// 调用举例: 无
// 作    者: liuxipeng/lKF54842 [2013-12-18]
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
// 函数名称: mrsToolsGet645Ver
// 功能描述: 根据功能码返回645帧的版本
// 参数说明:
//   HI_U8 [IN] 功能码
// 返 回 值:
//    版本1997或者2007，默认为1997
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/kf62735 [2012-01-05]
//*****************************************************************************
HI_U8 mrsToolsGet645Ver(HI_U8 ucFn)
{
    HI_U8 ret = METER_PROTO_645_2007;

    if ((!(ucFn & 0x10) && (ucFn != MRS_COLLECTOR_07_PROTOCOL_CONTROL)
        && (ucFn != MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL))
        || (MRS_MAXIMUM_DEMAND_CLEAR_CONTROL == (ucFn & 0x1F)))
    {
        ret = METER_PROTO_645_1997;
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


// 判断该MAC地址是否由电表表号转换而来 (是:HI_TRUE; 否:HI_FALSE)
HI_BOOL mrsToolsIsMacAddrFromMeter(HI_U8* pucMac)
{
    HI_U8 aucTempMac[HI_PLC_MAC_ADDR_LEN] = { 0 };

    (hi_void)memcpy_s(aucTempMac, sizeof(aucTempMac), pucMac, HI_PLC_MAC_ADDR_LEN);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_TOOLS_FE_TO_00(aucTempMac[MRS_MAC_ADDR_INDEX_5]);
#endif

    return mrsToolsCheckBCD(aucTempMac, HI_PLC_MAC_ADDR_LEN);
}


// 表地址转换为MAC地址
// 入参表地址顺序必须为645报文顺序
// 入参必须给定规约类型
// 使用者通过返回值判断是否转换成功
HI_U32 mrsToolsConvertMeter2Mac(HI_IN HI_U8 aucMeter[HI_METER_ADDR_LEN],
                                           HI_IN HI_U8 ucProto,
                                           HI_OUT HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN])
{
    mrsMeterAddrPreProc(aucMac, aucMeter, ucProto);
    mrsHexInvert(aucMac, HI_PLC_MAC_ADDR_LEN);

    return HI_ERR_SUCCESS;
}
/*将整数转化成定长BCD码,并逆序*/
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

// HEX码流逆序
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
// 函数名称: mrsFindStrEnd
// 功能描述:返回查找到的字符串中相同字符串位置的尾部
//
// 参数说明:
//    pStr1 被查找的字符串
//    pStr2 要查找的字符串
//
// 返 回 值:
//    版本号
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-14]
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
// 函数名称: mrsStrToInt
// 功能描述:将字符串转换成整数
//
// 参数说明:
//    pStr 被转换的字符串
//    ucType 转换的进制类型
//
// 返 回 值:
//    转换后的整数
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-14]
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
// 函数名称: mrsIsLetterValid
// 功能描述: 判断是否是合法数字
//
// 参数说明:
//    pStr 被判断的字符串
//    ucType 判断的进制类型依据
//
// 返 回 值:
//    TRUE  合法
//    FALSE 非法
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-14]
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


//*****************************************************************************
// 函数名称: MRS_StartTimer MRS_StopTimer
// 功能描述: 启动/停止定时器
//
// 参数说明:
//   ucTimeId [in] 取值为 [EN_APP_TASK_TIMER_MRS_1, EN_APP_TIMER_MAX)
//   ulPeriod [in] 定时周期,定时周期到后，会产生 消息 ID_MRS_MSG_MR_TIMER 通过注册的入口传递给Service.
//   ucOption [in] 取值为 HI_SYS_TIMER_PERIODIC 或是 HI_SYS_TIMER_ONESHOT 表示周期还是非周期
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它为错误码
//
// 调用要求: 仅在任务中的调用，不能在中断中调用
//           ucOption取值为SAL_TIMER_ONESHOT，可以不需要调用MRS_StopTimer释放定时器
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_BOOL mrsToolCheckBbu(HI_U8 ucFuncCfg)
{
    return ucFuncCfg & MRS_CCO_BBU_MASK;
}

HI_BOOL mrsToolIsBbu(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    if(pstCcoCtx == HI_NULL)
    {
        return HI_FALSE;
    }
    return pstCcoCtx->ucFuncCfg & MRS_CCO_BBU_MASK;
}

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

//*****************************************************************************
// 函数名称: mrsToolsGetTimeOut
// 功能描述: 获取14F1、06F2、06F4、06F5、06F10上行帧超时时间（根据模拟通道开关状态获取）
//
// 参数说明:
//   ulSimuChlClosedTimeOut [in] 模拟通道关闭时超时时间参数（单位ms）
//
// 返 回 值:
//    超时时间（单位ms）
//
// 调用要求: 无
// 调用举例: 无
// 作    者: jirui/00203265 [2016-02-09]
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

HI_BOOL mrsToolsCheckBroadCast(HI_PBYTE ptr,HI_U16 len)
{
    if ((ptr == HI_NULL) || (len != MRS_BOARDCAST_FRAME_LEN))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("param invalid"));
        return HI_FALSE;
    }

    // 检查表地址是否全0x99, 检查控制字
    if (!mrsToolsBroadCastAddr(ptr + 1)
        || (MRS_BOARDCAST_TIMEING_CONTROL != (ptr[8] & 0x1F)))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}
#endif

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

    if(HI_NULL == pucMac)
    {
        return HI_FALSE;
    }

    if (mrsToolsBroadCastMac(pucMac))
    {
        return HI_TRUE;
    }

    (HI_VOID)HI_MDM_NM_GetAddr(aucMac, aucIp);

    return (HI_BOOL)mrsToolsMemEq(aucMac, pucMac, HI_PLC_MAC_ADDR_LEN);
}
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_BOOL mrsSrvVerifyCcoMac(HI_U8 *pucMac)
{
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN] = {0};

    if (mrsToolsBroadCastMac(pucMac))
    {
        return HI_TRUE;
    }

    HI_MDM_GetCCOMacAddr(aucMac);

    return (HI_BOOL)mrsToolsMemEq(aucMac, pucMac, HI_PLC_MAC_ADDR_LEN);
}
#endif
#define BITSET(x,i)  ((x)=(HI_U8)(((1<<(i))|(x))))
#define BITGET(x,i) (((1<<(i)) & (x))?1:0)

HI_VOID mrsSetBitMap(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    BITSET(pBitmap[ucPos], ucBit);
}

HI_BOOL mrsGetBitMap(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    return (HI_BOOL)(1 == BITGET(pBitmap[ucPos], ucBit));
}

