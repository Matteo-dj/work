#include "app_common.h"
#include "cts_mdm_upg_main.h"
#include "cts_cfg_interface.h"

//*****************************************************************************

HI_U32 cts_HI_MDM_CREATE_MSG_SEND_CTRL(HI_MDM_MSG_CTRL_S *stCtrl);
HI_U32 cts_UPG_Send_MdmUpgStart(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgStop(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_UPG_ObtainMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_UPG_ReleaseMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgFileCheck(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);

HI_U32 cts_HI_MDM_CREATE_MSG_SEND_CTRL(HI_MDM_MSG_CTRL_S *stCtrl)
{
    if( HI_NULL == stCtrl)
    {
        CTS_PRINT0("cts_HI_MDM_CREATE_MSG_SEND_CTRL err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(stCtrl, sizeof(HI_MDM_MSG_CTRL_S), 0, sizeof(HI_MDM_MSG_CTRL_S));
    stCtrl->usTransId = HI_MDM_OBJ_ID_USR1;

    return HI_ERR_SUCCESS;
}

//У���������ļ���������ʼ����
HI_U32 cts_UPG_Send_MdmUpgStart(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    HI_U32 ret = HI_ERR_SUCCESS;    
    HI_U32 ulUpgId         = 0;
    HI_U32 ulOption        = 0;
    HI_U32 ulUpgTimeWindow = 0;
    HI_U32 ulUpgFileSize   = 0;
    HI_MDM_MSG_CTRL_S stCtrl;
    HI_MDM_UPG_START_REQ_S *pstReq              = HI_NULL; 
    UPG_Send_MdmUpgStart_PARAM_S *pStartReqPara = HI_NULL;
    CTS_UPG_OBTAIN_MEM_CTX  *ctsObtainMemCtx    = upgGetMemObtainCtx();

    if((cts_HI_MDM_CREATE_MSG_SEND_CTRL(&stCtrl) != HI_ERR_SUCCESS)
        || (pucParam == HI_NULL)
        || (upgCtsGetLoadFileFinishFlag() == HI_FALSE)
        || (ctsObtainMemCtx == HI_NULL)
        || (ctsObtainMemCtx->pUpgObtainBuf == HI_NULL))
    {
        CTS_PRINT0("cts_UPG_Send_MdmUpgStart err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }

    //ͨ����λ�ȡ������ʼ������Ϣ
    pStartReqPara   = (UPG_Send_MdmUpgStart_PARAM_S *) pucParam;
    ulUpgId         = pStartReqPara->ulUpgId;
    ulOption        = pStartReqPara->ulOption | pStartReqPara->ulUpdateOption;
    ulUpgTimeWindow = pStartReqPara->ulUpgTimeWindow;
    ulUpgFileSize   = ctsObtainMemCtx->ulObtainFileSize;

    //���������ļ�ǰ�ѻ�ȡ��������
    pstReq = (HI_MDM_UPG_START_REQ_S*)ctsObtainMemCtx->pUpgObtainBuf;
    (hi_void)memset_s(pstReq, sizeof(HI_MDM_UPG_START_REQ_S), 0, sizeof(HI_MDM_UPG_START_REQ_S));

    //������ʼ��������
    pstReq->stUpgCfg.ulOption        = ulOption;
    pstReq->stUpgCfg.ulUpgId         = ulUpgId;
    //pstReq->stUpgCfg.ulFileCrc       = HI_MDM_Crc32(0, (HI_CHAR*)((HI_PBYTE)pstReq + sizeof(HI_MDM_UPG_START_REQ_S)),ctsObtainMemCtx->ulRealFileSize);
    pstReq->stUpgCfg.ulFileCrc       = HI_MDM_Crc32(0, (HI_CHAR*)((HI_PBYTE)pstReq + sizeof(HI_MDM_UPG_START_REQ_S)), ulUpgFileSize);
    CTS_PRINT1("CTS_UPG_File_CRC = %x", pstReq->stUpgCfg.ulFileCrc);
    pstReq->stUpgCfg.ulFileLen       = ulUpgFileSize;
    pstReq->stUpgCfg.ulBlkNum        = 1;
    pstReq->stUpgCfg.ulUpgTimeWindow = ulUpgTimeWindow;

    //������������
    pstReq->stUpgFile.ulUpgId        = ulUpgId;
    pstReq->stUpgFile.usFileBlkSize  = 0;
    pstReq->stUpgFile.usIdx          = 1;
    
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    pstReq->stUpgFile.usChecksum     = upgCtsCalcCheckSum((HI_PBYTE)pstReq + sizeof(HI_MDM_UPG_START_REQ_S),(HI_U16)ulUpgFileSize);
    CTS_PRINT1("usChecksum = %x", pstReq->stUpgFile.usChecksum);
// TODO:ɾ��(���޸�)
#else
    pstReq->stUpgFile.usChecksum     = 0;

#endif
    ret = HI_MDM_MsgSend(HI_MDM_UPG_START, (HI_U8*)pstReq, sizeof(HI_MDM_UPG_START_REQ_S) + ulUpgFileSize, &stCtrl);

    CTS_PRINT1("cts_UPG_Send_MdmUpgStart:HI_MDM_UPG_START ret %d", ret);
    printRet(ret);
    return ret;     
}

//�������ļ�����������ʼ����ǰУ�������ļ�
//������Ŀǰ��������STA�����ļ�ͷ���(֧��CRCУ��)
HI_U32 cts_UPG_Send_MdmUpgFileCheck(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    return HI_ERR_SUCCESS; 
}

HI_U32 cts_UPG_Send_MdmUpgStop(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    HI_U32 usMsgId;
    HI_PBYTE pucMsgPaload;
    HI_MDM_MSG_CTRL_S stCtrl;    
    HI_U32 uspucMsgPaloadSize;
    HI_U32 ret = HI_ERR_SUCCESS;

    if((cts_HI_MDM_CREATE_MSG_SEND_CTRL(&stCtrl) != HI_ERR_SUCCESS) || (pucParam == HI_NULL))
    {
        CTS_PRINT0("cts_UPG_Send_MdmUpgStop err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }

    usMsgId            = HI_MDM_UPG_STOP;
    uspucMsgPaloadSize = sizeof(HI_MDM_UPG_STOP_REQ_S);
    pucMsgPaload       = pucParam;

    ret = HI_MDM_MsgSend(usMsgId, pucMsgPaload, uspucMsgPaloadSize, &stCtrl);
    
    CTS_PRINT1("cts_UPG_Send_MdmUpgStop:HI_MDM_MsgSend ret %d",ret);
    printRet(ret);
    return ret; 
    
}

//����״̬��ѯ:������������״̬�ͻ�������״̬
//HI_MDM_UPG_STATUS_REQ_S.stReqStatus.ulOption ȡֵΪ0����ѯ��������״̬��Ϊ1��ѯ��������״̬
HI_U32 cts_UPG_Send_MdmUpgStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    HI_U32 usMsgId;
    HI_PBYTE pucMsgPaload;
    HI_MDM_MSG_CTRL_S stCtrl;    
    HI_U32 uspucMsgPaloadSize;
    HI_U32 ret = HI_ERR_SUCCESS;

    if((cts_HI_MDM_CREATE_MSG_SEND_CTRL(&stCtrl) != HI_ERR_SUCCESS) || (pucParam == HI_NULL))
    {
        CTS_PRINT0("cts_UPG_Send_MdmUpgStatus err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }

    usMsgId            = HI_MDM_UPG_STATUS;
    uspucMsgPaloadSize = sizeof(HI_MDM_UPG_STATUS_REQ_S);
    pucMsgPaload       = pucParam;

    ret = HI_MDM_MsgSend(usMsgId, pucMsgPaload, uspucMsgPaloadSize, &stCtrl);
    
    CTS_PRINT1("cts_UPG_Send_MdmUpgStatus:HI_MDM_MsgSend ret %d",ret);
    printRet(ret);
    return ret; 
}

//�����ڵ�������HI_MDM_UPG_ObtainMem�ӿڣ���������������
//��ȡ���������ֹͣ�����������쳣�˳��������cts_HI_MDM_UPG_ReleaseMem�ͷ�
HI_U32 cts_HI_MDM_UPG_ObtainMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    HI_U32 ulUpgId    = 0;        
    HI_U32 ulFileSize = 0;
    HI_U32 ret        = HI_ERR_SUCCESS;    
    HI_MDM_UPG_START_REQ_S *pstReq = HI_NULL;
    HI_MDM_UPG_ObtainMem_PARAM_S * pobtainMemPara = HI_NULL;
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();

    if ((pucParam == HI_NULL) || (ctsObtainMemCtx == HI_NULL))
    {
        CTS_PRINT0("cts_HI_MDM_UPG_ObtainMem err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }
    if (ctsObtainMemCtx->pUpgObtainBuf != HI_NULL)
    {
        HI_MDM_Free(ctsObtainMemCtx->ulUpgId, ctsObtainMemCtx->pUpgObtainBuf);
    }
    pobtainMemPara = (HI_MDM_UPG_ObtainMem_PARAM_S *)pucParam;
    ulUpgId        = pobtainMemPara->ulUpgId;
    ulFileSize     = pobtainMemPara->ulFileSize;

    CTS_PRINT1("cts_HI_MDM_UPG_ObtainMem:HI_MDM_UPG_ObtainMem  ulUpgId %d", ulUpgId);
        
    ret = HI_MDM_UPG_ObtainMem(ulUpgId, ulFileSize , &pstReq, 0);
    
    CTS_PRINT1("cts_HI_MDM_UPG_ObtainMem:HI_MDM_UPG_ObtainMem  ret %d", ret);
    if (pstReq != HI_NULL)
    {
        (hi_void)memset_s(pstReq, ulFileSize+sizeof(HI_MDM_UPG_START_REQ_S), 0, ulFileSize+sizeof(HI_MDM_UPG_START_REQ_S));
        ctsObtainMemCtx->pUpgObtainBuf    = (HI_PVOID)pstReq;
        ctsObtainMemCtx->ulUpgId          = pobtainMemPara->ulUpgId;
        ctsObtainMemCtx->ulObtainFileSize = ulFileSize;
        ctsObtainMemCtx->ulRealFileSize   = 0;
        CTS_PRINT3("cts_HI_MDM_UPG_ObtainMem success:HI_MDM_UPG_ObtainMem pUpgObtainBuf %x,ulUpgId %d,ulObtainFileSize %d ", 
            (HI_U32)ctsObtainMemCtx->pUpgObtainBuf, ctsObtainMemCtx->ulUpgId, ctsObtainMemCtx->ulObtainFileSize);
    }
    else
    {
        CTS_PRINT3("cts_HI_MDM_UPG_ObtainMem fail:HI_MDM_UPG_ObtainMem pUpgObtainBuf %x,ulUpgId %d,ulObtainFileSize %d ",
            (HI_U32)ctsObtainMemCtx->pUpgObtainBuf, ctsObtainMemCtx->ulUpgId, ctsObtainMemCtx->ulObtainFileSize);
    }
    printRet(ret);
    return ret; 

}

//ֹͣ������������쳣�˳�������øýӿ��ͷ���������
//�ͷ��ڴ�ʱ������ID���������ڴ�ʱ��IDƥ�䣬�����޷��ͷ�
HI_U32 cts_HI_MDM_UPG_ReleaseMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MDM_UPG_START_REQ_S* pstReq = HI_NULL;
    HI_MDM_UPG_ReleaseMem_PARAM_S *preleaseMemPara = HI_NULL; 
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();
    
    if ((pucParam == HI_NULL))
//        || (ctsObtainMemCtx == HI_NULL)
        // TODO: ����������(���޸�)
//        || (ctsObtainMemCtx->pUpgObtainBuf == HI_NULL))
    {
        CTS_PRINT0("cts_HI_MDM_UPG_ReleaseMem err parameter.");        
        return HI_ERR_INVALID_PARAMETER;
    }
    CTS_PRINT0("cts_HI_MDM_UPG_ReleaseMem enter.");
    preleaseMemPara = (HI_MDM_UPG_ReleaseMem_PARAM_S *)pucParam;
    pstReq          = (HI_MDM_UPG_START_REQ_S *)ctsObtainMemCtx->pUpgObtainBuf;

//    if (preleaseMemPara->ulUpgId != ctsObtainMemCtx->ulUpgId)
//    {
//        CTS_PRINT0("cts_HI_MDM_UPG_ReleaseMem: upgId not match, release mem fail.");
//        return HI_ERR_INVALID_ID;
//    }
    ret = HI_MDM_UPG_ReleaseMem(preleaseMemPara->ulUpgId, pstReq,0);
    
    CTS_PRINT3("cts_HI_MDM_UPG_ReleaseMem:HI_MDM_UPG_ReleaseMem ret %d, preleaseMemPara->ulUpgId %d, pstReq %x",
        ret, preleaseMemPara->ulUpgId, (HI_U32)pstReq);  
    if(HI_ERR_SUCCESS == ret)
    //�ͷ��ڴ�����������
    {
        (hi_void)memset_s(ctsObtainMemCtx, sizeof(CTS_UPG_OBTAIN_MEM_CTX), 0, sizeof(CTS_UPG_OBTAIN_MEM_CTX));
        CTS_PRINT0("cts_HI_MDM_UPG_ReleaseMem: release mem success, set ctsObtainMemCtx to default.");
    }
    else
    {
        CTS_PRINT0("cts_HI_MDM_UPG_ReleaseMem: release mem fail.");
    }
    printRet(ret);
    return ret; 
}

////��ʼ������
HI_U32 cts_Uart0_init()
{
    HI_U32 ret = 0;
    HI_SYS_UART_CFG_S Uart_Cfg  = {0};
    /* Uart default config */
    Uart_Cfg.enRate = 115200;
    Uart_Cfg.ucDataBit = 8;
    Uart_Cfg.ucFlowCtrl = 0;
    Uart_Cfg.ucParity = 1;
    Uart_Cfg.ucStop = 1;

    HI_MDM_UART_Close(UART_PORT_NUM0);
    ret = HI_MDM_UART_Open(UART_PORT_NUM0,&Uart_Cfg);

    if (ret != HI_ERR_SUCCESS)
    {
        CTS_PRINT0("���ڴ�ʧ��");
        printRet(ret);
        return ret;
    }
    printRet(ret);
    return HI_ERR_SUCCESS;
}
//�Դ�Ŵ������ݵĻ���������д����
HI_VOID ctsReadBufferPro(HI_U8 *buffer, HI_U16 real_read_size)
{
    HI_U16 i = 0;
    HI_U32 ulrx_data_len = 0;
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();
    HI_MDM_UPG_START_REQ_S *pstUPGStartReq = HI_NULL;
    pstUPGStartReq = ctsObtainMemCtx->pUpgObtainBuf;
    while (real_read_size)
    {
        //���������ȱ���
        if(ctsObtainMemCtx->ulRealFileSize >= ctsObtainMemCtx->ulObtainFileSize)
        {
            CTS_PRINT0("���ڽ��ճ��ȴ����ļ����� !");
            break;
        }
        ulrx_data_len = ctsObtainMemCtx->ulRealFileSize;
        pstUPGStartReq->stUpgFile.ucFileBlk[ulrx_data_len] = buffer[i++];//�����ڶ���������д�뻺����
        ctsObtainMemCtx->ulRealFileSize++;//���������е����ݳ��ȼ�1
        real_read_size--;
        //CTS_PRINT2("cts_UPG_load = %x ; readSize = %d", pstUPGStartReq->stUpgFile.ucFileBlk[ulrx_data_len],ctsObtainMemCtx->ulRealFileSize);
    }
}
HI_VOID cts_Uart0_ReadBody(void)
{
#define UartRxSize 1
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 buffer[UartRxSize] = {0};
    HI_U16 rx_buf_size = 1, real_read_size = 0;
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();
    HI_MDM_UPG_START_REQ_S *pstUPGStartReq = HI_NULL;
    pstUPGStartReq = ctsObtainMemCtx->pUpgObtainBuf;
    HI_MDM_Sleep(1*1000);
    //HI_DIAG_LOG_MSG_I0(SOCKET_FILE_LOG_FLAG_047, HI_DIAG_MT("������������"));
    for (;;)
    {
        //�Ӵ��ڶ�����
        ret = HI_MDM_UART_Read(UART_PORT_NUM0, buffer, rx_buf_size, &real_read_size);
        if (ret == HI_ERR_SUCCESS)
        {
            //�Դ�Ŵ������ݵĻ���������д����
            ctsReadBufferPro(buffer, real_read_size);
        }
        if(ctsObtainMemCtx->ulRealFileSize >= ctsObtainMemCtx->ulObtainFileSize)
        {
            //���� ����������
            upgCtsSetLdFileFinishFlag(HI_TRUE);
            CTS_PRINT4("File[0] = %x ; File[1] = %x;File[end-1] = %x;File[end] = %x", 
                pstUPGStartReq->stUpgFile.ucFileBlk[0],pstUPGStartReq->stUpgFile.ucFileBlk[1],
                pstUPGStartReq->stUpgFile.ucFileBlk[ctsObtainMemCtx->ulRealFileSize-2],pstUPGStartReq->stUpgFile.ucFileBlk[ctsObtainMemCtx->ulRealFileSize-1]);
            break;
        }
    }
    CTS_PRINT0("cts_Uart0_ReadBody read finish !");
}
HI_VOID cts_Uart0_ReceiveFileLen(void)
{
#define UartRxSize 1
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 buffer[UartRxSize] = {0};
    HI_U16 rx_buf_size = 1, real_read_size = 0;
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();
    HI_MDM_UPG_START_REQ_S *pstUPGStartReq = HI_NULL;
    pstUPGStartReq = ctsObtainMemCtx->pUpgObtainBuf;
    CTS_PRINT0("cts_Uart0_ReadBody reading !");
    HI_MDM_Sleep(1*1000);
    //HI_DIAG_LOG_MSG_I0(SOCKET_FILE_LOG_FLAG_047, HI_DIAG_MT("������������"));
    for (;;)
    {
        //�Ӵ��ڶ�����
        ret = HI_MDM_UART_Read(UART_PORT_NUM0, buffer, rx_buf_size, &real_read_size);
        if (ret == HI_ERR_SUCCESS)
        {
            //�Դ�Ŵ������ݵĻ���������д����
            ctsReadBufferPro(buffer, real_read_size);
        }
        if((ctsObtainMemCtx->ulRealFileSize-1) >= pstUPGStartReq->stUpgFile.ucFileBlk[0])
        {
            CTS_PRINT6("File[0] = %x ; File[1] = %x;File[2] = %x;File[3] = %x;File[4] = %x;File[5] = %x", 
                pstUPGStartReq->stUpgFile.ucFileBlk[0],pstUPGStartReq->stUpgFile.ucFileBlk[1],
                pstUPGStartReq->stUpgFile.ucFileBlk[2],pstUPGStartReq->stUpgFile.ucFileBlk[3],
                pstUPGStartReq->stUpgFile.ucFileBlk[4],pstUPGStartReq->stUpgFile.ucFileBlk[5]);
            ctsObtainMemCtx->ulObtainFileSize = 
            (HI_U32)(((HI_U32)(pstUPGStartReq->stUpgFile.ucFileBlk[1]&0x0f)<<16)|((HI_U32)(pstUPGStartReq->stUpgFile.ucFileBlk[2]&0x0f)<<12)
            |((HI_U32)(pstUPGStartReq->stUpgFile.ucFileBlk[3]&0x0f)<<8)|((HI_U32)(pstUPGStartReq->stUpgFile.ucFileBlk[4]&0x0f)<<4)
            |((HI_U32)(pstUPGStartReq->stUpgFile.ucFileBlk[5]&0x0f)));
            ctsObtainMemCtx->ulRealFileSize = 0;
            (hi_void)memset_s(pstUPGStartReq->stUpgFile.ucFileBlk, ctsObtainMemCtx->ulObtainFileSize+sizeof(HI_MDM_UPG_START_REQ_S), 0, ctsObtainMemCtx->ulObtainFileSize+sizeof(HI_MDM_UPG_START_REQ_S));
            CTS_PRINT1("ctsObtainMemCtx->ulObtainFileSize =  %d", ctsObtainMemCtx->ulObtainFileSize);
            break;
        }
    }
    CTS_PRINT0("cts_Uart0_ReadBody read finish !");
}
HI_U32 cts_Uart0_Close()
{
    return HI_MDM_UART_Close(UART_PORT_NUM0);
}
HI_VOID cts_TimerStart()
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ret = HI_MDM_TIMER_Start(&g_stCtsOsRes.astTimerHandle[EN_APP_TASK_TIMER_CTS_1],timer_test_isr,4000, HI_SYS_TIMER_PERIODIC,ID_CTS_MSG_MR_TIMER);

    HI_MDM_GetMilliSeconds();
    printRet(ret);
}

HI_VOID cts_TimerStop()
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ret = HI_MDM_TIMER_Stop(g_stCtsOsRes.astTimerHandle[EN_APP_TASK_TIMER_CTS_1]);
    printRet(ret);
}
HI_U32 cts_UPG_File_Load(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    CTS_PRINT0("enter cts_UPG_File_Load."); 
    UPG_File_Load_PARAM_S * pstFileLoadParam = HI_NULL;
    pstFileLoadParam = (UPG_File_Load_PARAM_S *)pucParam;
    if(pstFileLoadParam->ulUpgId)
    {
        CTS_PRINT0("������������!");
        cts_HI_MDM_UPG_ObtainMem(pucParam, ulParamSize, ulOpt, uObjId);
        CTS_PRINT0("��ʼ������׼������!");
        cts_Uart0_init();
        cts_Uart0_ReceiveFileLen();
        CTS_PRINT0("���������ļ�!");
        cts_Uart0_ReadBody();
    }
    else
    {
        cts_Uart0_Close();
        CTS_PRINT0("uart0 closed.");
    }
}
//*****************************************************************************

