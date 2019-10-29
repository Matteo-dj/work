
#include <hi_types.h> 
#if defined (PRODUCT_CFG_HSO_COLLECT_DATA)
#include "drv_dfx_cmn_inc.h"
#include <drv_collectdata.h>
#include <hi_diag_async.h>
#include <hi_drv_com.h>
#include <hi_share_memory.h>
/***************************************************************
 1.HSO下发开始采数ID_DIAG_CMD_DRV_UPLOAD_DATA_START命令。
 2.单板端根据输入采数参数进行采数，、
 		--->采数成功，给HSO返回SUCCESS和采数长度，紧接着调用UPLAD_DATA上传10k数据
   		--->失败则返回错误码结束
 3.HSO接受10K数据后，采数总长度不够，则发送ID_DIAG_CMD_DRV_UPLOAD_DATA命令请求剩余数据(以10k为单位)。
 4.HSO停止采数，单板接收ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP命令则中止采数。
***************************************************************/
#define HSO_DATA_COLLECT_MAX_TIME (6*60*1000)
typedef struct
{
    HI_BUF_S strDcInfo;
    HI_PVOID pDCAddr;
    TIMER_HANDLE_T timerHandle;
}HI_HSO_DC_CTRL_S;
HI_HSO_DC_CTRL_S g_hso_dc_ctrl;

HI_PRV HI_VOID hso_dc_free_mem(HI_U32 ulData)
{
    //LOCK
    if ((HI_PVOID)g_hso_dc_ctrl.pDCAddr!=HI_NULL)
    {
        HI_SHARE_MEM_FreeMem(g_hso_dc_ctrl.pDCAddr);
        g_hso_dc_ctrl.pDCAddr = HI_NULL;
    }
    //UNLOCK
}
HI_PRV HI_PVOID hso_dc_get_mem(HI_U32 ulSize)
{
    (HI_VOID)HI_MDM_TIMER_Stop((HI_TIMER_HANDLE_S *)&(g_hso_dc_ctrl.timerHandle));

    //LOCK
    if(g_hso_dc_ctrl.pDCAddr == HI_NULL)
    {
        g_hso_dc_ctrl.pDCAddr = HI_SHARE_MEM_GetMem(ulSize);
    }

    if(g_hso_dc_ctrl.pDCAddr == HI_NULL)
    {
        return HI_NULL;
    }
    //UNLOCK


	//启动定时器
    if(HI_ERR_SUCCESS != HI_MDM_TIMER_Start((HI_TIMER_HANDLE_S *)&(g_hso_dc_ctrl.timerHandle), (TIMER_PROC_FUNC)hso_dc_free_mem, HSO_DATA_COLLECT_MAX_TIME, HI_TIMER_TYPE_ONCE, 0))
    {
        hso_dc_free_mem(0);
    }

    return g_hso_dc_ctrl.pDCAddr;
}

//对用户传来的地址进行判断后，发送给上传处理。
HI_U32 upload_data(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	HI_U32 ulTransmitedLen = 0;  /* 不足1K数据翻转，ulTransmitedLen记录第一部分传输长度，不翻转情况下为0 */
	HI_U32 ulSendLen = 0;
	HI_U32 ulTotalLeft = 0;
	HI_U32 ulSendedCount = 0;
	ulTotalLeft = g_hso_dc_ctrl.strDcInfo.bufSize[0] + g_hso_dc_ctrl.strDcInfo.bufSize[1];
	while (ulTotalLeft > 0)
	{
		ulSendLen = (ulTotalLeft >= HI_DRV_UPLOAD_DATA_PACKET_SIZE - ulTransmitedLen)?(HI_DRV_UPLOAD_DATA_PACKET_SIZE - ulTransmitedLen):ulTotalLeft;
		ulTransmitedLen = 0;

		/* 数据翻转 记录第一部分长度ulTransmitedLen */
		if (ulSendLen > g_hso_dc_ctrl.strDcInfo.bufSize[0])
		{
			ulSendLen = g_hso_dc_ctrl.strDcInfo.bufSize[0];
			ulTransmitedLen = ulSendLen;
		}
		if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)g_hso_dc_ctrl.strDcInfo.bufAddr[0], (HI_U16)ulSendLen, HI_TRUE) != HI_ERR_SUCCESS)
		{
			HI_DIAG_LOG_MSG_E2(upload_data_012,HI_DIAG_MT("<-- SEND ERR!send_addr=0x%8x, len1 = %d \n"), g_hso_dc_ctrl.strDcInfo.bufAddr[0], ulSendLen);
			hso_dc_free_mem(0);
			return	HI_ERR_INVALID_PARAMETER;
		}

		ulTotalLeft -= ulSendLen;
		ulSendedCount += ulSendLen;
		g_hso_dc_ctrl.strDcInfo.bufSize[0] -= ulSendLen;
		g_hso_dc_ctrl.strDcInfo.bufAddr[0] += ulSendLen;


		/* 更新buf内的内容 */
		if (0 == g_hso_dc_ctrl.strDcInfo.bufSize[0])
		{
			g_hso_dc_ctrl.strDcInfo.bufSize[0] = g_hso_dc_ctrl.strDcInfo.bufSize[1];
			g_hso_dc_ctrl.strDcInfo.bufAddr[0] = g_hso_dc_ctrl.strDcInfo.bufAddr[1];
			g_hso_dc_ctrl.strDcInfo.bufSize[1] = 0;
			g_hso_dc_ctrl.strDcInfo.bufAddr[1] = 0;
		}


		if (ulSendedCount >= HI_DRV_UPLOAD_DATA_ACK_SIZE)
		{
			break;
		}

	}
	if (0 == ulTotalLeft)
	{
		hso_dc_free_mem(0);
	}

	return HI_ERR_SUCCESS;
}



HI_U32 upload_data_config_prv(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_DIAG_UPLOAD_START_REQ_S  *pRx = HI_NULL;
    HI_DIAG_UPLOAD_START_IND_S ret_len = {0};
    DC_INPUT_PARA_STRU strDcPara = {0};
    HI_PVOID ulAddr = HI_NULL;

    (hi_void)memset_s((HI_VOID *)&g_hso_dc_ctrl.strDcInfo, sizeof(g_hso_dc_ctrl.strDcInfo), 0, sizeof(HI_BUF_S));
    if (usId == ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP)
    {
       hso_dc_free_mem(0);
       return HI_ERR_SUCCESS;
    }


    HI_UNREF_PARAM(ulOption);
	pRx = (HI_DIAG_UPLOAD_START_REQ_S*)pCmdParam;

    if (pRx->EndAddr < pRx->StartAddr)
    {
        ret_len.ulReturn = HI_ERR_INVALID_PARAMETER;
        goto fail;
    }

    ulAddr = hso_dc_get_mem(pRx->EndAddr - pRx->StartAddr);
    if (HI_NULL == ulAddr)
	{
		ret_len.ulReturn = HI_ERR_MALLOC_FAILUE;
		goto fail;
	}


    strDcPara.enDataSrc = (DC_TRIG_DATA_ENUM)pRx->usDataSrc;
    strDcPara.enTrigerType = (DC_TRIG_TYPE_ENUM)pRx->usTrigerType;
    strDcPara.lAgcGain = (HI_S32)pRx->AgcGain;
    strDcPara.ulAfterPointlen =  pRx->ulPointNum;
    strDcPara.ulEndAddr = (HI_U32)ulAddr + (pRx->EndAddr - pRx->StartAddr);
    strDcPara.ulStartAddr = (HI_U32)ulAddr;
    strDcPara.ulWaitResultTimeOut = pRx->ulTimeout;
    strDcPara.ulWaitEnvOkTime=1000;
    

    if (HI_ERR_SUCCESS != (ulRet = HI_DC_StartNormalDataCollect(&strDcPara,&g_hso_dc_ctrl.strDcInfo)))
    {
        ret_len.ulReturn = ulRet;
        goto fail;
	}

    ret_len.ulReturn = HI_ERR_SUCCESS;
    ret_len.ulTotalLength = g_hso_dc_ctrl.strDcInfo.bufSize[0] + g_hso_dc_ctrl.strDcInfo.bufSize[1];

    if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&ret_len,
        (HI_U16)sizeof(HI_DIAG_UPLOAD_START_IND_S), HI_TRUE) != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s((HI_VOID *)&g_hso_dc_ctrl.strDcInfo, sizeof(g_hso_dc_ctrl.strDcInfo), 0, sizeof(HI_BUF_S));
		hso_dc_free_mem(0);
		return  HI_ERR_INVALID_PARAMETER;
	}

    HI_MDM_Sleep(10);

    if(HI_ERR_SUCCESS == upload_data(ID_DIAG_CMD_DRV_UPLOAD_DATA, pCmdParam, usCmdParamSize, ulOption))
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        ret_len.ulReturn = HI_ERR_INVALID_RAM_ADDR;
    }
fail:
    /* TO DO*/
	hso_dc_free_mem(0);
    (hi_void)memset_s((HI_VOID *)&g_hso_dc_ctrl.strDcInfo, sizeof(g_hso_dc_ctrl.strDcInfo), 0, sizeof(HI_BUF_S));
    if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&ret_len,
        (HI_U16)sizeof(HI_DIAG_UPLOAD_START_IND_S), HI_TRUE) != HI_ERR_SUCCESS)
    {
		return  HI_ERR_INVALID_PARAMETER;
	}
    
    return ret_len.ulReturn;
}

HI_U32 upload_data_config(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    return HI_DIAG_AsyncCmd(usId,pCmdParam,usCmdParamSize,ulOption,upload_data_config_prv);
}

#elif defined (MAC_COLLECT_DATA_DEBUG) || defined(STA_COLLECT_200KB_DATA)

#include "drv_dfx_cmn_inc.h"
#include <drv_collectdata.h>

#if defined (PRODUCT_CFG_SUPPORT_SHARE_MEMORY)
extern HI_PVOID g_shareMemoryAddr;
#endif


HI_PRV HI_BUF_S strDcInfo;

//对用户传来的地址进行判断后，发送给上传处理。
HI_U32 upload_data(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulTransmitedLen = 0;  /* 不足1K数据翻转，ulTransmitedLen记录第一部分传输长度，不翻转情况下为0 */
    HI_U32 ulSendLen = 0;
    HI_U32 ulTotalLeft = 0;
    HI_U32 ulSendedCount = 0;
    ulTotalLeft = strDcInfo.bufSize[0] + strDcInfo.bufSize[1];
    while (ulTotalLeft > 0)
    {
    	ulSendLen = (ulTotalLeft >= HI_DRV_UPLOAD_DATA_PACKET_SIZE - ulTransmitedLen)?(HI_DRV_UPLOAD_DATA_PACKET_SIZE - ulTransmitedLen):ulTotalLeft;
		ulTransmitedLen = 0;

        /* 数据翻转 记录第一部分长度ulTransmitedLen */
        if (ulSendLen > strDcInfo.bufSize[0])
        {
        	ulSendLen = strDcInfo.bufSize[0];
            ulTransmitedLen = ulSendLen;
        }
	    if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)strDcInfo.bufAddr[0], (HI_U16)ulSendLen, HI_TRUE) != HI_ERR_SUCCESS)
	    {
	        HI_DIAG_LOG_MSG_E2(upload_data_012,HI_DIAG_MT("<-- SEND ERR!send_addr=0x%8x, len1 = %d \n"), strDcInfo.bufAddr[0], ulSendLen);
	        return  HI_ERR_INVALID_PARAMETER;
	    }

		ulTotalLeft -= ulSendLen;
        ulSendedCount += ulSendLen;
		strDcInfo.bufSize[0] -= ulSendLen;
		strDcInfo.bufAddr[0] += ulSendLen;


        /* 更新buf内的内容 */
        if (0 == strDcInfo.bufSize[0])
        {
            strDcInfo.bufSize[0] = strDcInfo.bufSize[1];
            strDcInfo.bufAddr[0] = strDcInfo.bufAddr[1];
            strDcInfo.bufSize[1] = 0;
            strDcInfo.bufAddr[1] = 0;
        }


        if (ulSendedCount >= HI_DRV_UPLOAD_DATA_ACK_SIZE)
        {
            break;
        }

    }

    return HI_ERR_SUCCESS;
}
HI_U32 upload_data_config(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_UPLOAD_START_IND_S ret_len = {0};
    HI_U32 ulAddr = 0;
    HI_U32 ulSize = 0;
    HI_U32 ulRet = 0;

    HI_DIAG_UPLOAD_START_REQ_S  *pRx = pCmdParam;
    DC_INPUT_PARA_STRU strDcPara = {0};
    DIAG_CMD_CONFIG_MONITER_CONFIG_STRU NV_MAC_LOWPOWER = {0};
    
    (hi_void)memset_s((HI_VOID *)&strDcInfo, sizeof(strDcInfo), 0, sizeof(HI_BUF_S));
    if (usId == ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP)
    {
       return HI_ERR_SUCCESS;
    }
#if defined (MAC_COLLECT_DATA_DEBUG)//若同时两个都打开了，则mac上传数据优先
    HI_MAC_GetNtbCollectAddr(&ulAddr,&ulSize);
    if (HI_NULL == (HI_PVOID)ulAddr)
	{
		ret_len.ulReturn = HI_ERR_MALLOC_FAILUE;
		goto fail;
	}
#endif

#if defined(MAC_COLLECT_DATA_DEBUG)

    strDcInfo.bufAddr[0] = ulAddr;
    strDcInfo.bufSize[0] = ulSize;
    strDcInfo.bufAddr[1] = 0;
    strDcInfo.bufSize[1] = 0;

#elif defined (STA_COLLECT_200KB_DATA)

    strDcPara.enDataSrc = (DC_TRIG_DATA_ENUM)pRx->usDataSrc;
    strDcPara.enTrigerType = (DC_TRIG_TYPE_ENUM)pRx->usTrigerType;
    strDcPara.lAgcGain = (HI_S32)pRx->AgcGain;
    strDcPara.ulAfterPointlen =  pRx->ulPointNum;
    strDcPara.ulEndAddr = (HI_U32)g_shareMemoryAddr + PRODUCT_CFG_SHARE_MEMORY_SIZE;//(pRx->EndAddr - pRx->StartAddr);
    strDcPara.ulStartAddr = (HI_U32)g_shareMemoryAddr;
    strDcPara.ulWaitResultTimeOut = pRx->ulTimeout;
    strDcPara.ulWaitEnvOkTime=1000;
    
    if (HI_ERR_SUCCESS != (ulRet = DC_Data_Collect(&strDcPara,&strDcInfo,HI_DRV_DATA_COLLECT_STEP_ALL)))
    {
        ret_len.ulReturn = ulRet;
        goto fail;
	}
#endif

    ret_len.ulReturn = HI_ERR_SUCCESS;
    ret_len.ulTotalLength = strDcInfo.bufSize[0] + strDcInfo.bufSize[1];

    if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&ret_len,
        (HI_U16)sizeof(HI_DIAG_UPLOAD_START_IND_S), HI_TRUE) != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s((HI_VOID *)&strDcInfo, sizeof(strDcInfo), 0, sizeof(HI_BUF_S));
		return  HI_ERR_INVALID_PARAMETER;
	}

    HI_MDM_Sleep(10);

    if(HI_ERR_SUCCESS == upload_data(ID_DIAG_CMD_DRV_UPLOAD_DATA, pCmdParam, usCmdParamSize, ulOption))
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        ret_len.ulReturn = HI_ERR_INVALID_RAM_ADDR;
    }
fail:
    /* TO DO*/
    (hi_void)memset_s((HI_VOID *)&strDcInfo, sizeof(strDcInfo), 0, sizeof(HI_BUF_S));
    if (HI_DIAG_ReportPacket(usId, (HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&ret_len,
        (HI_U16)sizeof(HI_DIAG_UPLOAD_START_IND_S), HI_TRUE) != HI_ERR_SUCCESS)
    {
		return  HI_ERR_INVALID_PARAMETER;
	}
    
    return ret_len.ulReturn;
}




#endif