#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_SUPPORT_SHARE_TASK
#include <hi_share_task.h>
#include <hi_stdlib.h>
#include <hi_mdm_task.h>

#define HI_SHARE_TASK_MAX_MODULE_NUM 5
#define HI_SHARE_TASK_Q_ST_BIT 16
#define HI_SHARE_TASK_MSG_ID_MASK ((1<<HI_SHARE_TASK_Q_ST_BIT)-1)
#define HI_SHARE_TASK_MAX_MSG_ID  ((1<<HI_SHARE_TASK_Q_ST_BIT)-1)

#define HI_SHARE_TASK_TASK_ID_2_ARRAY_ID(x) (x)
#define HI_SHARE_TASK_ARRAY_ID_2_TASK_ID(x) (x)

#define HI_SHARE_TASK_HANDLE_2_ARRAY_ID(x) (x)
#define HI_SHARE_TASK_ARRAY_ID_2_HANDLE(x) (x)

typedef struct
{
    HI_BOOL bInit;
    HI_U8 ucModuleArrayId;
    HI_U16 pad;
    
    HI_U32 ulOsTaskId;//����ID    
    HI_U32 ulOsQId;//��Ϣ����ID

    
    HI_U32 ulStackSize;//ջ��С
    HI_CHAR *pszQueueName;//��Ϣ��������
    HI_CHAR *pszTaskName;//��������
    HI_U16 usQLen;//MSG���г���
    HI_U16 usTaskPri;//�������ȼ�

    SHARE_TASK_FUNC pfn[HI_SHARE_TASK_MAX_MODULE_NUM];//�ص�����
}HI_SHARE_TASK_S;


HI_SHARE_TASK_S g_stSharetask[HI_SHARE_TASK_ID_MAX];

HI_U32 HI_SHARE_TASKQ_Init(HI_VOID)
{

    HI_SHARE_TASK_S *pstShareTask=HI_NULL;

    //��ʼ������task
    pstShareTask=&g_stSharetask[HI_SHARE_TASK_ID_0];
    pstShareTask->ulStackSize=HI_SHARE_TASK_0_STACK_SIZE;
    pstShareTask->usTaskPri=HI_SHARE_TASK_0_TASK_PRI;
    pstShareTask->usQLen=HI_SHARE_TASK_0_MSGQ_LEN;
    pstShareTask->pszTaskName=HI_SHARE_TASK_0_TASK_NAME;
    pstShareTask->pszQueueName=HI_SHARE_TASK_0_MSGQ_NAME;
    return HI_ERR_SUCCESS;
 }

HI_VOID share_task_body(HI_U32 ulTaskArrayId)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg={0};
    HI_SHARE_TASK_S *pstShareTask=HI_NULL;
    HI_U32 ulMoudleId=0;
    SHARE_TASK_FUNC pFn=HI_NULL;
    pstShareTask=&g_stSharetask[ulTaskArrayId];

    for(;;)
    {
        ret = HI_MDM_MSG_Wait(pstShareTask->ulOsQId,&stMsg,HI_SYS_WAIT_FOREVER);
        if(ret!=HI_ERR_SUCCESS)
        {
            HI_MDM_Sleep(1000);
        }
        
        ulMoudleId = (stMsg.ulMsgId>>HI_SHARE_TASK_Q_ST_BIT);
        stMsg.ulMsgId =(stMsg.ulMsgId&HI_SHARE_TASK_MSG_ID_MASK);
        pFn=pstShareTask->pfn[ulMoudleId];
        if(pFn!=HI_NULL)
        {
            pFn(&stMsg);
        }
    }

}



HI_U32 HI_SHARE_TASKQ_Create(HI_SHARE_TASK_ID_E ulShareTaskId,HI_U32 *pHandle,SHARE_TASK_FUNC pfn)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_SHARE_TASK_S *pstShareTask=HI_NULL;
    HI_U32 ulTaskArrayId=HI_SHARE_TASK_TASK_ID_2_ARRAY_ID(ulShareTaskId);

    if(pHandle==HI_NULL || pfn==HI_NULL ||ulTaskArrayId>=HI_SHARE_TASK_ID_MAX)
    {
        return HI_ERR_SHARE_TASK_INVALID_PARAM;
    }

    pstShareTask=&g_stSharetask[ulTaskArrayId];

    //��һ�δ����������Ϣ����
    if(!pstShareTask->bInit)
    {
        ret=HI_MDM_MSG_CreateQueue(&pstShareTask->ulOsQId,pstShareTask->pszQueueName,pstShareTask->usQLen);
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }
       
        ret=HI_MDM_TASK_Create(&pstShareTask->ulOsTaskId,pstShareTask->pszTaskName,share_task_body,ulTaskArrayId,pstShareTask->ulStackSize,pstShareTask->usTaskPri);
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }
        pstShareTask->bInit=HI_TRUE;
    }

    
    if(pstShareTask->ucModuleArrayId<HI_SHARE_TASK_MAX_MODULE_NUM)
    {
        *pHandle=HI_SHARE_TASK_ARRAY_ID_2_HANDLE(pstShareTask->ucModuleArrayId);
        pstShareTask->pfn[pstShareTask->ucModuleArrayId]=pfn;
        pstShareTask->ucModuleArrayId++;
    }
    else
    {
        return HI_ERR_SHARE_TASK_NOT_ENOUGH_MODULE_ID;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_SHARE_TASKQ_SendMsg(HI_SHARE_TASK_ID_E ulShareTaskId,HI_U32 ulHandle,HI_SYS_QUEUE_MSG_S *pMsg)
{
    HI_SHARE_TASK_S *pstShareTask=HI_NULL;
    HI_U32 ulTaskArrayId=HI_SHARE_TASK_TASK_ID_2_ARRAY_ID(ulShareTaskId);
    HI_U32 ucModuleArrayId=HI_SHARE_TASK_HANDLE_2_ARRAY_ID(ulHandle);

    if(pMsg==HI_NULL || (ulTaskArrayId>=HI_SHARE_TASK_ID_MAX) || (ucModuleArrayId>=HI_SHARE_TASK_MAX_MODULE_NUM))
    {
        return HI_ERR_SHARE_TASK_INVALID_PARAM;
    }
    
    if(pMsg->ulMsgId>HI_SHARE_TASK_MAX_MSG_ID)
    {
        return HI_ERR_SHARE_TASK_INVALID_MSG_ID;
    }

    pstShareTask=&g_stSharetask[ulTaskArrayId];

    
    pMsg->ulMsgId = (pMsg->ulMsgId&HI_SHARE_TASK_MSG_ID_MASK);
    pMsg->ulMsgId += (ucModuleArrayId<<HI_SHARE_TASK_Q_ST_BIT);

    return HI_MDM_MSG_Send(pstShareTask->ulOsQId,pMsg,0);
}
#endif//PRODUCT_CFG_SUPPORT_SHARE_TASK