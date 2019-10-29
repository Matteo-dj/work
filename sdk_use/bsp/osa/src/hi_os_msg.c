#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_queue.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_time.h>
#include "hi_os_stat.h"
#include "hi_os_sys.h"
#include <hi_asm_func.h>


HI_U32 HI_MDM_MSG_CreateQueue(HI_OUT HI_U32 * pQueueId,HI_CHAR szName[8],HI_U16 usMsgArrayCnt)
{
    HI_U32 msgQID;
    HI_U32 ret = LOS_OK;


    if ((HI_NULL == pQueueId) || (0 == usMsgArrayCnt)||(szName == HI_NULL))
    {
        g_stOsaErr.ulNewQu++;
        return HI_ERR_MSG_INVALID_PARAM;
    }

    ret = LOS_QueueCreate(szName,usMsgArrayCnt,&msgQID,0,sizeof(HI_SYS_QUEUE_MSG_S));
    PRINTK("%s,%s,ret = %x msgQID= %d\n",__func__,szName,ret,msgQID);

    if(LOS_OK != ret)
    {        
        //printf("%s,ret = %x \n",__func__,ret);
        g_stOsaErr.ulNewQu++;
        return HI_ERR_MSG_CREATE_Q_FAIL;
    }

    
    *pQueueId = msgQID;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_MSG_DeleteQueue(HI_U32 ulQueueId)
{
    
    HI_U32 ret = LOS_OK;
    ret = LOS_QueueDelete(ulQueueId);
    if(ret!=LOS_OK)
    {
        return HI_ERR_MSG_Q_DELETE_FAIL;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_MSG_Send(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S* pstMsg,HI_U32 ulTimeOut)
{
    HI_U32 lr=HI_ASM_GetLr();
    HI_U32 ulTick = HI_SYS_WAIT_FOREVER;
    HI_U32 ret = 0;


    if (HI_NULL == pstMsg)
    {
        return HI_ERR_MSG_INVALID_PARAM;
    }    
    // printf("[%s in][ulQueueId: %u][lr:0x%x]\n",__func__,ulQueueId,get_lr);
	
    if (HI_SYS_WAIT_FOREVER != ulTimeOut)
    {
        ulTick = ms2systick(ulTimeOut, HI_TRUE);
    }

    pstMsg->ulTimeStamp = HI_MDM_GetMilliSeconds();    
    ret = LOS_QueueWriteCopy(ulQueueId, (HI_PVOID)pstMsg, sizeof(HI_SYS_QUEUE_MSG_S), ulTick);
    if (HI_ERR_SUCCESS == ret) 
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        printf("[%s fail %u][%u][%u]%d\n",__func__,lr,pstMsg->ulMsgId,ret,ulQueueId);
        g_stOsaErr.ulSendQu++;
        return HI_ERR_MSG_SEND_FAIL;
    }
}


HI_U32 HI_MDM_MSG_Wait(HI_U32 ulQueueId, HI_OUT HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut)
{
    HI_U32 ulTick = HI_SYS_WAIT_FOREVER;
    HI_U32 ret = 0;
    HI_U32 size = sizeof(HI_SYS_QUEUE_MSG_S);

    if (HI_NULL == pstMsg)
    {
        return HI_ERR_MSG_INVALID_PARAM;
    }

    if (HI_SYS_WAIT_FOREVER != ulTimeOut)
    {
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/
        ulTick = ms2systick (ulTimeOut, HI_TRUE);
    }
    ret = LOS_QueueReadCopy(ulQueueId, pstMsg, &size, ulTick);

    if (LOS_ERRNO_QUEUE_TIMEOUT == ret)
    {
        g_stOsaErr.ulRcvQuLo++;
        return HI_ERR_MSG_WAIT_TIME_OUT;
    }
    else if(ret!=HI_ERR_SUCCESS)
    {
        g_stOsaErr.ulRcvQuLo++;
        return HI_ERR_MSG_WAIT_FAIL;
    }
    return HI_ERR_SUCCESS;
}


HI_BOOL HI_MDM_MSG_IsQueueFull(HI_U32 ulQueueId)
{
    QUEUE_INFO_S  stQinfo = {0,};
    HI_U32 ret = LOS_OK;

    ret = LOS_QueueInfoGet(ulQueueId,&stQinfo);
    if(HI_ERR_SUCCESS != ret)
    {

        //printf("[%s fail][%u][%u]\n",__func__,ulQueueId,ret);
        return HI_TRUE;
    }
    
    if(stQinfo.usReadableCnt==stQinfo.usQueueLen)
    {
        return HI_TRUE;
    }
    
    return HI_FALSE;

}

HI_U32 HI_MDM_MSG_GetCurrentMsgNum(HI_U32 ulQueueId)
{
    QUEUE_INFO_S  stQinfo = {0};
    HI_U32 ret = LOS_OK;

    ret = LOS_QueueInfoGet(ulQueueId,&stQinfo);
    if(HI_ERR_SUCCESS != ret)
    {

        //printf("[%s fail][%u][%u]\n",__func__,ulQueueId,ret);
        return HI_ERR_MSG_INVALID_MSG_NUM;
    }
    
    return stQinfo.usReadableCnt;
}



HI_U32 HI_MDM_MSG_GetTotalMsgNum(HI_U32 ulQueueId)
{

    QUEUE_INFO_S  stQinfo = {0,};
    HI_U32 ret = LOS_OK;

    ret = LOS_QueueInfoGet(ulQueueId,&stQinfo);
    if(HI_ERR_SUCCESS != ret)
    {

        //printf("[%s fail][%u][%u]\n",__func__,ulQueueId,ret);
        return HI_ERR_MSG_INVALID_MSG_NUM;
    }
    
    return stQinfo.usQueueLen;
}


