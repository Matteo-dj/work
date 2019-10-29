#include <hi_types.h>
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_pwd.h"
#include <dfx_sal.h>
#include <hi_auth.h>

#define HI_PWD_CHALLENGE_USEFUL_TIME 100//随机序列有效期(单位 s)
typedef struct
{
    HI_U32 challengeCreateTime;//序列生成时间(单位 s)
    HI_U8 challenge[CHANLLENGE_COMMON_SIZE];//随机序列
    HI_U8 ucNewSalt[CHANLLENGE_SALT_SIZE]; //盐值128bit
    HI_U16 usNewCnt;
    HI_U16 pad;
}HI_PWD_MODIFY_CTRL_S;

HI_PWD_MODIFY_CTRL_S g_pwd_modify_ctrl;

HI_U32 dfx_get_challenge(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_PWD_MODIFY_GET_CHALLENGE_IND_S stInd = {0};
    HI_TCP_AUTH_CHALLENGE_S * pstChallenge = HI_NULL;
    HI_U32 ret=HI_ERR_SUCCESS;

    pstChallenge = (HI_TCP_AUTH_CHALLENGE_S*)HI_MDM_Malloc(HI_MOD_ID_SAL_SYS, sizeof(HI_TCP_AUTH_CHALLENGE_S));  
    if (pstChallenge == HI_NULL)
    {
        stInd.ret = HI_ERR_MALLOC_FAILUE;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstChallenge, sizeof(HI_TCP_AUTH_CHALLENGE_S), 0, sizeof(HI_TCP_AUTH_CHALLENGE_S));

    ret=HI_AUTH_CreateChallenge(pstChallenge);
    if(HI_ERR_SUCCESS!=ret)
    {
        stInd.ret = ret;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        HI_MDM_Free(HI_MOD_ID_SAL_SYS, pstChallenge);
        return ret;
    }

    
    (hi_void)memcpy_s (stInd.challenge, CHANLLENGE_COMMON_SIZE, pstChallenge->aucRand, CHANLLENGE_COMMON_SIZE);
    (hi_void)memcpy_s (stInd.salt,CHANLLENGE_SALT_SIZE,pstChallenge->aucSalt,CHANLLENGE_SALT_SIZE);
    stInd.ver = pstChallenge->verNum;
    stInd.cnt = pstChallenge->usCnt;


    ret=HI_AUTH_CreateNewCntAndSalt(&stInd.usNewCnt,stInd.ucNewSalt);
    if(HI_ERR_SUCCESS!=ret)
    {
        stInd.ret = ret;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        HI_MDM_Free(HI_MOD_ID_SAL_SYS, pstChallenge);
        return ret;
    }
   
    
    /*存放随机数到全局变量中，用于校验*/    
    (hi_void)memcpy_s(g_pwd_modify_ctrl.challenge, CHANLLENGE_COMMON_SIZE, stInd.challenge, CHANLLENGE_COMMON_SIZE);
    (hi_void)memcpy_s(g_pwd_modify_ctrl.ucNewSalt, CHANLLENGE_SALT_SIZE, stInd.ucNewSalt, CHANLLENGE_SALT_SIZE);
    g_pwd_modify_ctrl.usNewCnt=stInd.usNewCnt;
    g_pwd_modify_ctrl.challengeCreateTime = HI_MDM_GetSeconds();
    stInd.ret = HI_ERR_SUCCESS;
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
    HI_MDM_Free(HI_MOD_ID_SAL_SYS, pstChallenge);
    return HI_ERR_SUCCESS;
}

HI_U32 dfx_change_pass_word(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 retData = 0;
    HI_PWD_MODIFY_REQ_S *pReq = (HI_PWD_MODIFY_REQ_S *)pCmdParam;
    HI_PWD_MODIFY_IND_S  stInd ;
    HI_U32 cur_sec = HI_MDM_GetSeconds();
    HI_U8 aucNVSTK[CHANLLENGE_COMMON_SIZE] = {0};
    HI_U8 aucHSOSTK[CHANLLENGE_COMMON_SIZE] = {0};
    (hi_void)memset_s(&stInd,sizeof(HI_PWD_MODIFY_IND_S), 0,sizeof(HI_PWD_MODIFY_IND_S));


    //是否支持密码修改功能
    if(HI_FALSE == HI_AUTH_GetChagePassWordEnable())
    {
        stInd.errId = HI_PWD_MODIFY_ERR_NOT_SUPPORT;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    //校验challenge是否合法
    if(g_pwd_modify_ctrl.challengeCreateTime==0
        ||((cur_sec-g_pwd_modify_ctrl.challengeCreateTime)>HI_PWD_CHALLENGE_USEFUL_TIME)
        ||memcmp(g_pwd_modify_ctrl.challenge, pReq->challenge, CHANLLENGE_COMMON_SIZE))
    {
        stInd.errId = HI_PWD_MODIFY_ERR_CHALLENGE_WRONG;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    //获取单板端存储STK
    ret = HI_AUTH_GetNvSTK(aucNVSTK);
    if(ret!=HI_ERR_SUCCESS)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_SYS_CALL;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }
    
    //根据随机数，HSO端发送过来的PC和单板端的STK，计算出HSO端的STK'
    ret = HI_AUTH_CreateSTK(aucNVSTK,&(pReq->challenge[0]),&(pReq->usOldPwdCP[0]),aucHSOSTK);
    if(ret != HI_ERR_SUCCESS)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_SYS_CALL;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }

    //密码校验
    ret = HI_AUTH_PassWordCheck(aucHSOSTK,aucNVSTK,&retData);
    if(ret == HI_ERR_PASS_WORD_WRONG)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_PASS_WORD_WRONG;
        stInd.data[0] = retData;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }
    else if(ret == HI_ERR_ACCOUNT_LOCKED)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_ACCESS_ACCOUNT_LOCK;
        stInd.data[0] = retData;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }
    else if(ret!=HI_ERR_SUCCESS)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_SYS_CALL;
        stInd.data[0] = retData;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }

    //修改密码
    ret = HI_AUTH_SetSTK2NV(&(pReq->usNewPwdSTK[0]),g_pwd_modify_ctrl.usNewCnt,g_pwd_modify_ctrl.ucNewSalt);
    if(ret!=HI_ERR_SUCCESS)
    {
        stInd.errId = HI_PWD_MODIFY_ERR_SYS_CALL;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
        return ret;
    }

    //成功
    stInd.errId = HI_PWD_MODIFY_ERR_OK;
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);
    return HI_ERR_SUCCESS;

}

