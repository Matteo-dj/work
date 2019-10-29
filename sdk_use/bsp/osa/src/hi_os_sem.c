#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_sem.h>
#include <hi_mdm_sem.h>
#include "hi_os_stat.h"
#include "hi_os_sys.h"





HI_U32 HI_MDM_SEM_BCreate (HI_U32 * pSemId, HI_CHAR szName[8], HI_U8 ucInit)
{
    HI_U32  ulSemId;
    HI_U32 ret = LOS_OK;

    if (HI_NULL == pSemId)
    {
        g_stOsaErr.ulCreateSem++;
        return HI_ERR_SEM_INVALID_PARAM;
    }
	 //edb_put_str_p0(szName);
	
    ret = LOS_SemBCreate (ucInit,&ulSemId);

    if(LOS_OK == ret)
    {
        *pSemId = ulSemId;

        return HI_ERR_SUCCESS;
    }
	
		//edb_put_str_p2("HI_MDM_SEM_BCreate nok ",ulSemId,ret);
    g_stOsaErr.ulCreateSem++;
    return HI_ERR_SEM_CREATE_FAIL;
}


HI_U32 HI_MDM_SEM_Delete(HI_U32 ulSemId)
{
    HI_U32 ret = 0;
     
    ret =  LOS_SemDelete(ulSemId);
    if(LOS_OK == ret )
    {
        return HI_ERR_SUCCESS;
    }
    //printf("[%s fail][%u][%u]\n",__func__,ulSemId,ret);
    g_stOsaErr.ulDelSem++;
    return HI_ERR_SEM_DELETE_FAIL;
}


HI_U32 get_sem_count(HI_U32 ulSemId)
{

    HI_U32      uwRet;
    HI_U32      uwValue = 0;


    uwRet = LOS_SemGetValue(ulSemId, &uwValue); 
    if (uwRet != LOS_OK)
    {
        return HI_ERR_FAILURE;
        
    }
	  
    return uwValue;
}


HI_U32 HI_MDM_SEM_Wait(HI_U32 ulSemId, HI_U32 ulTimeOut)
{
    HI_U32 ulTick = HI_SYS_WAIT_FOREVER;
    HI_U32 ret = 0;

    if (HI_SYS_WAIT_FOREVER != ulTimeOut)
    {
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/        
        ulTick = ms2systick(ulTimeOut, HI_TRUE);
    }

    ret = LOS_SemPend(ulSemId, ulTick);
    if (ret==LOS_ERRNO_SEM_TIMEOUT)
    {
        return HI_ERR_SEM_WAIT_TIME_OUT;
    }
    else if (HI_ERR_SUCCESS == ret)
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        g_stOsaErr.ulWaitSem++;
        return HI_ERR_SEM_WAIT_FAIL;
    }
}


HI_U32 HI_MDM_SEM_Signal (HI_U32 ulSemId)
{
    HI_U32 ret = 0;
     
    ret =  LOS_SemPost(ulSemId);
    if(LOS_OK == ret)
    {
        return HI_ERR_SUCCESS;
    }
    //printf("[%s fail][%u][%u]\n",__func__,ulSemId,ret);
    g_stOsaErr.ulReleaseSem++;
    return HI_ERR_SEM_SIG_FAIL;
}

