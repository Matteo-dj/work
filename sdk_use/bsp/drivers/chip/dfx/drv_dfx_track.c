#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_sal.h>
#include <drv_dfx.h>
#include <dfx_track.h>
#include <hi_mdm_cpu_stat.h>
#include "drv_dfx_track.h"
#include <los_trace.h>




HI_PRV HI_U32 report_track_info(HI_BOOL bLock,HI_U32 ulOption)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_DFX_TRACK_INFO_TOTAL_IND_S stTotalInd={0};
    HI_DFX_TRACK_INFO_ITEM_IND_S stItemInd={0};
    HI_CPU_STAT_TRACK_INFO_S stTrackInfo={0};
    HI_CPU_STAT_TRACK_ITEM_S stTrackItem={0};
    HI_U32 i=0;
    HI_U32 ulCurIdx=0;

    if(bLock)
    {
        ret=HI_MDM_CPU_TrackLock();
        if(ret!=HI_ERR_SUCCESS)
        {
            
            stTotalInd.ulRet=ret;
            HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(stTotalInd),HI_TRUE);
            return ret;
        }

    }

    ret=HI_MDM_CPU_TrackGetInfo(&stTrackInfo);
    if(ret!=HI_ERR_SUCCESS)
    {
        stTotalInd.ulRet=ret;
        HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(stTotalInd),HI_TRUE);
        return ret;
    }

    stTotalInd.ulRet=HI_ERR_SUCCESS;
    stTotalInd.ulArrayCount=stTrackInfo.ulMaxRecordCount;
    stTotalInd.ulOption=HI_MDM_CPU_TrackGetType();
    stTotalInd.ulLockPos=stTrackInfo.ulLockIdx;
    HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(stTotalInd),HI_TRUE);

    for(i=stTotalInd.ulLockPos;i<stTotalInd.ulArrayCount;i++,ulCurIdx++)
    {
        ret=HI_MDM_CPU_TrackGetLockInfoByIdx(i,&stTrackItem);
        if(ret!=HI_ERR_SUCCESS)
        {
            stItemInd.ulRet=ret;
            stItemInd.ulSeq=ulCurIdx;
            HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
            return ret;
        }
        stItemInd.ulRet=ret;
        stItemInd.ulSeq=ulCurIdx;
#if 1        
        stItemInd.ulTraceType=stTrackItem.ulTraceType; 
        stItemInd.ulCurTime=stTrackItem.ulCurTime;
        stItemInd.ulEntry=stTrackItem.ulEntry;
        stItemInd.ulData0=stTrackItem.ulData0;
        stItemInd.ulData1=stTrackItem.ulData1;
#endif        
        HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);

    }

    for(i=0;i<stTotalInd.ulLockPos;i++,ulCurIdx++)
    {
        ret=HI_MDM_CPU_TrackGetLockInfoByIdx(i,&stTrackItem);
        if(ret!=HI_ERR_SUCCESS)
        {
            stItemInd.ulRet=ret;
            stItemInd.ulSeq=ulCurIdx;
            HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
            return ret;   
        }
        
        stItemInd.ulRet=ret;
        stItemInd.ulSeq=ulCurIdx;
#if 1        
        stItemInd.ulTraceType=stTrackItem.ulTraceType; 
        stItemInd.ulCurTime=stTrackItem.ulCurTime;
        stItemInd.ulEntry=stTrackItem.ulEntry;
        stItemInd.ulData0=stTrackItem.ulData0;
        stItemInd.ulData1=stTrackItem.ulData1;
#endif        
        HI_DIAG_ReportPacket(ID_DIAG_TRACK_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
    }

    return ret;
}


HI_U32 dfx_track_start(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_DFX_TRACK_ST_CMD_S *pReq = (HI_DFX_TRACK_ST_CMD_S*) pCmdParam;
    HI_DFX_TRACK_ST_IND_S stInd={0};

    
    ret=HI_MDM_CPU_TrackStart((HI_U16)pReq->ulArrayCount);
    if(ret==HI_ERR_SUCCESS)
    {
        HI_MDM_CPU_TrackSetType(pReq->ulOption);
    }
    stInd.ulRet=ret;
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DFX_TRACK_ST_IND_S),HI_TRUE);

    return HI_ERR_SUCCESS;
}

HI_U32 dfx_track_end(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DFX_TRACK_STOP_IND_S stInd={0};
    stInd.ulRet = HI_MDM_CPU_TrackStop();
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DFX_TRACK_STOP_IND_S),HI_TRUE);
    return HI_ERR_SUCCESS;
}



HI_U32 dfx_track_get_info(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DFX_TRACK_GET_INFO_CMD_S * pReq = (HI_DFX_TRACK_GET_INFO_CMD_S*) pCmdParam;
    report_track_info((HI_BOOL)pReq->bLock,ulOption);
    return HI_ERR_SUCCESS;
}


