#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_event.h>
#include <los_hwi.h>
#include "hi_os_stat.h"
#include "hi_os_sys.h"
#include <hi_mdm_event.h>
#include "string.h"
#include <hi_stdlib.h>
#include "sal_os_cfg.h"
#include <stdlib.h>

#define HI_EVENT_SUPPORT_BITS (0X00FFFFFF)//注释:LITEOS本身限制EVENT不支持25bit,VXWORKS仅支持低24bit。

typedef struct
{   
    HI_BOOL bUsed;
    HI_U8 ucRsv[3];
    EVENT_CB_S stOsEvent;
}HI_EVENT_ITEM_S;

typedef struct
{
    HI_U8 bInit;//模块是否初始化
    HI_U8 ulMaxEventCout;//模块最多支持多少个event
    HI_U16 pad;
    HI_EVENT_ITEM_S *pstEventItem;//指向item数组
}HI_EVENT_CTRL_S;

HI_EVENT_CTRL_S g_stEventCtrl = {0,};

HI_U32 HI_MDM_EVENT_Init(HI_VOID)
{
    HI_U8 ulMaxEventCount = (HI_U8)g_stOsaResource.ulEventNum;
    
    if(!g_stEventCtrl.bInit)
    {
        if (!ulMaxEventCount)
        {
            return HI_ERR_INVALID_PARAMETER;
        }
        
        g_stEventCtrl.pstEventItem = (HI_EVENT_ITEM_S *)malloc(sizeof(HI_EVENT_ITEM_S) * ulMaxEventCount);
        if(!g_stEventCtrl.pstEventItem)
        {
            return HI_ERR_EVENT_NOT_ENOUGH_MEMORY;
        }
        
        memset_s((HI_VOID *)g_stEventCtrl.pstEventItem, sizeof(HI_EVENT_ITEM_S) * ulMaxEventCount, 0, sizeof(HI_EVENT_ITEM_S) * ulMaxEventCount);
        g_stEventCtrl.ulMaxEventCout=ulMaxEventCount;
        g_stEventCtrl.bInit=HI_TRUE;
        return HI_ERR_SUCCESS;
    }
    
    return HI_ERR_EVENT_RE_INIT;
}

HI_U32 HI_MDM_EVENT_Create(HI_OUT HI_U32 * pEventID,HI_CHAR szName[8])
{ 
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 i =0;
    HI_U32 uwIntSave=0;
    HI_EVENT_ITEM_S *pstEventItem=HI_NULL;
    HI_U32 ulTempEventId=HI_INVALID_EVENT_ID;
    
    if(!g_stEventCtrl.bInit)
    {
        return HI_ERR_EVENT_NOT_INIT;
    }

    if(pEventID==HI_NULL||szName==HI_NULL)
    {
        return HI_ERR_EVENT_INVALID_PARAM;
    }


    uwIntSave = LOS_IntLock();
    for (i=0;i<g_stEventCtrl.ulMaxEventCout;i++)
    {
        pstEventItem=&g_stEventCtrl.pstEventItem[i];
        if (pstEventItem->bUsed==HI_FALSE)
        {
            pstEventItem->bUsed=HI_TRUE;
            ulTempEventId=i;
            break;
        }
    }   
    LOS_IntRestore(uwIntSave); 
    
    if (ulTempEventId==HI_INVALID_EVENT_ID)
    {
        return HI_ERR_EVENT_CREATE_NO_HADNLE;
    }

    if (pstEventItem)
    {
        ret = LOS_EventInit(&(pstEventItem->stOsEvent));
        if(ret != LOS_OK)
        {
            return HI_ERR_EVENT_CREATE_SYS_FAIL;
        }
    }
    
    *pEventID=ulTempEventId;
    return HI_ERR_SUCCESS;
        
}


HI_U32 HI_MDM_EVENT_Send(HI_U32 ulEventID ,HI_U32 ulEventBits)
{
	HI_U32 ret=LOS_OK;
	HI_EVENT_ITEM_S *pstEventItem=HI_NULL;

	if(!g_stEventCtrl.bInit)
	{
		return HI_ERR_EVENT_NOT_INIT;
	}

	if(ulEventID>=g_stEventCtrl.ulMaxEventCout)
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}

	if (ulEventBits & (~HI_EVENT_SUPPORT_BITS))
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}
	
	pstEventItem=&g_stEventCtrl.pstEventItem[ulEventID];
	if(pstEventItem->bUsed==HI_FALSE)
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}

	
	ulEventBits=(ulEventBits&HI_EVENT_SUPPORT_BITS);
	ret = LOS_EventWrite(&(pstEventItem->stOsEvent), ulEventBits);
	if(ret!=LOS_OK)
	{
		return HI_ERR_EVENT_SEND_FAIL;
	}
	return HI_ERR_SUCCESS;
	
}

HI_U32 HI_MDM_EVENT_Wait (HI_U32 ulEventID, HI_U32 ulEventMask,HI_OUT HI_U32 * pEventBits,HI_U32 ulTimeOut,HI_U32 ulFlag)
{
	HI_EVENT_ITEM_S *pstEventItem=HI_NULL;
	HI_U32 ulTick=HI_SYS_WAIT_FOREVER;
	HI_U32 ulEventBits=0;
	
	if(!g_stEventCtrl.bInit)
	{
		return HI_ERR_EVENT_NOT_INIT;
	}

	if(ulEventID>=g_stEventCtrl.ulMaxEventCout||pEventBits==HI_NULL)
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}
	
	pstEventItem=&g_stEventCtrl.pstEventItem[ulEventID];
	if(pstEventItem->bUsed==HI_FALSE)
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}

	if (ulEventMask & (~HI_EVENT_SUPPORT_BITS))
	{
		return HI_ERR_EVENT_INVALID_PARAM;
	}
	
	if (HI_SYS_WAIT_FOREVER != ulTimeOut)
	{
		ulTick = ms2systick (ulTimeOut, HI_TRUE);
	}
	
	ulEventBits = LOS_EventRead(&(pstEventItem->stOsEvent),ulEventMask,ulFlag,ulTick);

	if(ulEventBits==LOS_ERRNO_EVENT_READ_TIMEOUT)
	{
		return HI_ERR_EVENT_WAIT_TIME_OUT;
	}


	if (!ulEventBits || (ulEventBits & LOS_ERRTYPE_ERROR))
	{
		g_stOsaErr.ulWaitEvent++;
		return HI_ERR_EVENT_WAIT_FAIL;
	}
	else
	{
		*pEventBits = ulEventBits;
		return HI_ERR_SUCCESS;
	}

}

HI_U32 HI_MDM_EVENT_Clear (HI_U32 ulEventID,HI_U32 ulEventBits)// TODO:参数修改
{
    HI_U32 ret=LOS_OK;
    HI_EVENT_ITEM_S *pstEventItem=HI_NULL;

    if(!g_stEventCtrl.bInit)
    {
        return HI_ERR_EVENT_NOT_INIT;
    }

    if(ulEventID >= g_stEventCtrl.ulMaxEventCout)
    {
        return HI_ERR_EVENT_INVALID_PARAM;
    }

    
    pstEventItem=&g_stEventCtrl.pstEventItem[ulEventID];
    if(pstEventItem->bUsed==HI_FALSE)
    {
        return HI_ERR_EVENT_INVALID_PARAM;
    }

    ulEventBits=(ulEventBits&HI_EVENT_SUPPORT_BITS);


    ret = LOS_EventClear(&(pstEventItem->stOsEvent),~ulEventBits);// TODO:正好反了
    if(ret!=LOS_OK)
    {
        return HI_ERR_EVENT_CLEAR_FAIL;
    }
    return HI_ERR_SUCCESS;

}

HI_U32 HI_MDM_EVENT_Delete (HI_U32 ulEventID)
{
    HI_U32 ret=LOS_OK;
    HI_EVENT_ITEM_S *pstEventItem=HI_NULL;

    if(!g_stEventCtrl.bInit)
    {
        return HI_ERR_EVENT_NOT_INIT;
    }

    if(ulEventID >= g_stEventCtrl.ulMaxEventCout)
    {
        return HI_ERR_EVENT_INVALID_PARAM;
    }

    pstEventItem=&g_stEventCtrl.pstEventItem[ulEventID];
    if(pstEventItem->bUsed==HI_FALSE)
    {
        return HI_ERR_EVENT_INVALID_PARAM;
    }

    ret=LOS_EventDestroy(&(pstEventItem->stOsEvent));
    if(ret!=LOS_OK)
    {
        return HI_ERR_EVENT_DELETE_FAIL;
    }

    memset_s((HI_VOID *)pstEventItem, sizeof(HI_EVENT_ITEM_S), 0x0, sizeof(HI_EVENT_ITEM_S));
    return HI_ERR_SUCCESS;

}

