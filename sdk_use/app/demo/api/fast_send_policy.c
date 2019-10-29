#include <hi_driver.h>
#include <hi_mdm.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_mdm_event.h>
#include <hi_mdm_isr.h>
#include <hi_mdm_mem.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_sem.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_task.h>
#include <hi_mdm_timer.h>
#include <hi_mem.h>
#include <hi_stdlib.h>
#include "hi_mdm_mac.h"
#include "fast_send_policy.h"


DEMO_FAST_SEND_POLICY_CB_S g_test_fast_send_cb = {0};
HI_U8 g_rvc_fast_send_data[16] = {0};
HI_U16 g_fast_send_pkt_index = 0;

extern	HI_U32 HI_MDM_FastSendInit(HI_VOID);

//些函数只是为了调试时用于记录接收报文个数，对功能没有影响，使用时可以不调用。
extern 	HI_VOID mac_fast_send_app_rcv_cnt(HI_VOID);

HI_VOID demo_data_proc(HI_U8 *data, HI_U16 data_length)
{
    mac_fast_send_app_rcv_cnt();

    //进行数据处理
}

HI_VOID demo_send_pkt_index_add(HI_VOID)
{
    if (g_fast_send_pkt_index == 0)
    {
        g_fast_send_pkt_index = 1;
    }
    else
    {
        g_fast_send_pkt_index++;
    }
}

HI_BOOL demo_fast_send_fliter(HI_U16 packet_index)
{

    HI_U32 index = 0;

    if (g_test_fast_send_cb.fliter_index >= 32)
    {
        g_test_fast_send_cb.fliter_index = 0;
    }

    for (index = 0; index < 32; index ++)
    {
        if (packet_index == g_test_fast_send_cb.rcv_pkt_index[index])
        {
            // 重复报文
            return HI_FALSE;
        }
    }

    g_test_fast_send_cb.rcv_pkt_index[g_test_fast_send_cb.fliter_index] = packet_index;

    g_test_fast_send_cb.fliter_index++;

    return HI_TRUE;
}

HI_VOID demo_fast_send_cfg_sc(HI_VOID)
{
    HI_MAC_SC_PARA_S sc_para = {0};

    sc_para.FecRate = 0;
    sc_para.FsFreq = 0xa0;
    sc_para.MaxSendLength = 0x10;
    sc_para.ModulationMode = 0;
    sc_para.ScEnable = HI_TRUE;

    HI_MDM_ConfigScPara(&sc_para);
}

HI_VOID demo_sc_send_end_proc(HI_U32 result)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_MDM_BBPLC_SEND_PARA_S *plc_para = HI_NULL;
    MEMO_FAST_SEND_BB_HEAD_S bb_head = {0};
    HI_U32 send_size = 0;

    /* 提高发送成功率的同时，也要给信标留下发送时间*/

    bb_head.fast_type = 0x5aa5;

    HI_MDM_EnableSending();
    send_size = sizeof(HI_MDM_BBPLC_SEND_PARA_S) + g_test_fast_send_cb.data_len + sizeof(bb_head);
    plc_para = (HI_MDM_BBPLC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, send_size);
    if (HI_NULL == plc_para)
    {
        return;
    }
    
    (hi_void)memset_s(plc_para,send_size, 0,send_size);
    (hi_void)memcpy_s(plc_para->Data,sizeof(bb_head),&bb_head,sizeof(bb_head));
    (hi_void)memcpy_s(plc_para->Data + sizeof(bb_head), g_test_fast_send_cb.data_len, (HI_VOID *)(&(g_test_fast_send_cb.data)), g_test_fast_send_cb.data_len);
    plc_para->DataLength = g_test_fast_send_cb.data_len + sizeof(bb_head);
    plc_para->DataIndex = g_test_fast_send_cb.pkt_index;
    plc_para->SendMode = 0;
    plc_para->BroadcastFlag = 1;
    plc_para->ResendCnt = 1;
    plc_para->Timeout = 300;
    plc_para->SendFreq = 3;

    (hi_void)memcpy_s(plc_para->Odmac, sizeof(plc_para->Odmac), g_test_fast_send_cb.odmac, sizeof(g_test_fast_send_cb.odmac));
    HI_MDM_FastSendOnBbPlc(plc_para);

    HI_MDM_Free(HI_MOD_ID_USR,plc_para);
#endif
}


HI_VOID demo_sc_rcv_data_proc(HI_MAC_REPORT_RCV_FAST_SEND_PKT_S * pReportData)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

    HI_MDM_NETWORK_ATTRIBUTE_S attr_info = {0};
    HI_MAC_SC_SEND_PARA_S *sc_para = HI_NULL;
    HI_MDM_BBPLC_SEND_PARA_S *plc_para = HI_NULL;
    MEMO_FAST_SEND_BB_HEAD_S bb_head = {0};
    DEMO_FASTA_SEND_DATA_S * send_data = HI_NULL;

    HI_MDM_GetNetworkAttribute(&attr_info);

	if (HI_NULL == pReportData)
	{
		return;
	}

	if (HI_NULL == pReportData->DataAddr)
	{
		return;
	}

	send_data = (DEMO_FASTA_SEND_DATA_S *)(pReportData->DataAddr);

	if (demo_fast_send_fliter(pReportData->DataIndex) == HI_FALSE)
	{
		return;
	}

	if(send_data->odtei == attr_info.MyTei)
	{
		sc_para = (HI_MAC_SC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, sizeof(HI_MAC_SC_SEND_PARA_S) + pReportData->DataLength);
		if (HI_NULL == sc_para)
		{
			return;
		}

	    (hi_void)memset_s(sc_para, sizeof(HI_MAC_SC_SEND_PARA_S) + pReportData->DataLength, 0, sizeof(HI_MAC_SC_SEND_PARA_S) + pReportData->DataLength);
		(hi_void)memcpy_s(sc_para->Data, pReportData->DataLength, pReportData->DataAddr, pReportData->DataLength);
		sc_para->DataLength = (HI_U8)(pReportData->DataLength);
		sc_para->SendMode = 1;
		sc_para->DataIndex = pReportData->DataIndex;
		sc_para->ScSendResultFun = demo_sc_send_end_proc;
		sc_para->Timeout = 50;
	    (hi_void)memcpy_s(sc_para->Odmac, sizeof(sc_para->Odmac), pReportData->Odmac, sizeof(pReportData->Odmac));
		HI_MDM_FastSendOnScPlc(sc_para);
		HI_MDM_Free(HI_MOD_ID_USR, sc_para);
	}
	else if (HI_ND_TYPE_RELAY == attr_info.MyRole)
	{

		bb_head.fast_type = 0x5aa5;
		plc_para = (HI_MDM_BBPLC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + pReportData->DataLength + sizeof(bb_head));
		if (HI_NULL == plc_para)
		{
			return;
		}
	    (hi_void)memset_s(plc_para, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + pReportData->DataLength + sizeof(bb_head), 0, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + pReportData->DataLength + sizeof(bb_head));
	    (hi_void)memcpy_s(plc_para->Data, sizeof(bb_head), &bb_head, sizeof(bb_head));
	    (hi_void)memcpy_s(plc_para->Data + sizeof(bb_head), pReportData->DataLength, pReportData->DataAddr, pReportData->DataLength);
		plc_para->DataLength = pReportData->DataLength + sizeof(bb_head);
		plc_para->DataIndex = pReportData->DataIndex;
		plc_para->SendMode = 0;
		plc_para->BroadcastFlag = 1;
		plc_para->Timeout = 100;
		plc_para->ResendCnt = 1;
	    (hi_void)memcpy_s(plc_para->Odmac, sizeof(plc_para->Odmac), pReportData->Odmac, sizeof(pReportData->Odmac));
		HI_MDM_FastSendOnBbPlc(plc_para);

		HI_MDM_Free(HI_MOD_ID_USR, plc_para);
	}

	demo_data_proc(pReportData->DataAddr,  pReportData->DataLength);
#endif
}


HI_U32 demo_connect_state_bb(HI_U16 port, HI_U8 option)
{

	if (option != HI_MDM_CHL_STATUS_CONNECT)
	{
	    (hi_void)memset_s(g_test_fast_send_cb.rcv_pkt_index, sizeof(g_test_fast_send_cb.rcv_pkt_index), 0, sizeof(g_test_fast_send_cb.rcv_pkt_index));
		g_test_fast_send_cb.fliter_index = 0;
	}

	return HI_ERR_SUCCESS;
}


HI_U32 demo_rcv_data_proc_bb(HI_U16 index, HI_DMS_CHL_RX_S * rx_data)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_NETWORK_ATTRIBUTE_S attr_info = {0};
    HI_MAC_SC_SEND_PARA_S *sc_para = HI_NULL;
    HI_MDM_BBPLC_SEND_PARA_S *plc_para = HI_NULL;
    MEMO_FAST_SEND_BB_HEAD_S bb_head = {0};
    DEMO_FASTA_SEND_DATA_S * send_data = HI_NULL;


    if (HI_NULL == rx_data)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (HI_NULL == rx_data->pPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memcpy_s(&bb_head, sizeof(MEMO_FAST_SEND_BB_HEAD_S), rx_data->pPayload, 4);

    send_data = (DEMO_FASTA_SEND_DATA_S *)(rx_data->pPayload + sizeof(MEMO_FAST_SEND_BB_HEAD_S));

    if(0x5aa5 != bb_head.fast_type || (demo_fast_send_fliter(send_data->pkt_index) == HI_FALSE))
    {
        return HI_ERR_SUCCESS;
    }

    HI_MDM_GetNetworkAttribute(&attr_info);

    if(send_data->odtei == attr_info.MyTei)
    {

        sc_para = (HI_MAC_SC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, sizeof(HI_MAC_SC_SEND_PARA_S) + rx_data->usPayloadLen);
        if (HI_NULL == sc_para)
        {
            return HI_ERR_FAILURE;
        }
        (hi_void)memset_s(sc_para, sizeof(HI_MAC_SC_SEND_PARA_S) + rx_data->usPayloadLen, 0, sizeof(HI_MAC_SC_SEND_PARA_S) + rx_data->usPayloadLen);
        (hi_void)memcpy_s(sc_para->Data, rx_data->usPayloadLen - sizeof(bb_head),
                 rx_data->pPayload + sizeof(bb_head), rx_data->usPayloadLen - sizeof(bb_head));
        sc_para->DataLength = (HI_U8)(rx_data->usPayloadLen - sizeof(bb_head));
        sc_para->SendMode = 1;
        sc_para->DataIndex = send_data->pkt_index;
        sc_para->ScSendResultFun = demo_sc_send_end_proc;
        sc_para->Timeout = 50;
        (hi_void)memcpy_s(sc_para->Odmac, sizeof(sc_para->Odmac), rx_data->ucDestMac, sizeof(rx_data->ucDestMac));
        HI_MDM_FastSendOnScPlc(sc_para);
        HI_MDM_Free(HI_MOD_ID_USR, sc_para);
    }
    else if (HI_ND_TYPE_RELAY == attr_info.MyRole)
    {
        plc_para = (HI_MDM_BBPLC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + rx_data->usPayloadLen);
        if (HI_NULL == plc_para)
        {
            return HI_ERR_FAILURE;
        }
        (hi_void)memset_s(plc_para, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + rx_data->usPayloadLen, 0, sizeof(HI_MDM_BBPLC_SEND_PARA_S) + rx_data->usPayloadLen);

        (hi_void)memcpy_s(plc_para->Data, rx_data->usPayloadLen, rx_data->pPayload, rx_data->usPayloadLen);
        plc_para->DataLength = rx_data->usPayloadLen;
        plc_para->DataIndex = send_data->pkt_index;
        plc_para->SendMode = 0;
        plc_para->BroadcastFlag = 1;
        plc_para->Timeout = 100;
        plc_para->ResendCnt = 1;
        (hi_void)memcpy_s(plc_para->Odmac, sizeof(plc_para->Odmac), rx_data->ucDestMac, sizeof(rx_data->ucDestMac));
        HI_MDM_FastSendOnBbPlc(plc_para);
        HI_MDM_Free(HI_MOD_ID_USR, plc_para);
    }
    demo_data_proc(rx_data->pPayload + sizeof(bb_head),rx_data->usPayloadLen - sizeof(bb_head));
    #endif
    return HI_ERR_SUCCESS;
}

HI_VOID demo_fast_send_init(HI_VOID)
{
    HI_DMS_CHL_PORT_OBJ_S chi_para = {0};
    chi_para.pfnChlDataRx = demo_rcv_data_proc_bb;
    chi_para.pfnChlConnect = demo_connect_state_bb;

    demo_fast_send_cfg_sc();
    HI_MDM_RegisterFastCallbackSc(demo_sc_rcv_data_proc, g_rvc_fast_send_data, 16, 0);
    
    HI_MDM_ChlSetup(&chi_para, HI_DMS_CHL_MAC_PORT_APP);
	HI_MDM_FastSendInit();

}

HI_VOID demo_fast_send_policy_test(HI_VOID)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_MAC_SC_SEND_PARA_S *sc_para = HI_NULL;
    HI_U8 data[4] = {1,2,3,4};
    HI_U8 brodcast_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

    HI_MDM_QueryMostChildProxy(&g_test_fast_send_cb.odtei, g_test_fast_send_cb.odmac);

    sc_para = (HI_MAC_SC_SEND_PARA_S *)HI_MDM_Malloc(HI_MOD_ID_USR, sizeof(HI_MAC_SC_SEND_PARA_S) + sizeof(g_test_fast_send_cb.data));
    if (HI_NULL == sc_para)
    {
        return;
    }
    
    (hi_void)memset_s(sc_para, sizeof(HI_MAC_SC_SEND_PARA_S) + sizeof(g_test_fast_send_cb.data), 0, sizeof(HI_MAC_SC_SEND_PARA_S) + sizeof(g_test_fast_send_cb.data));
    if(0 == g_test_fast_send_cb.odtei)
    {
        (hi_void)memcpy_s(g_test_fast_send_cb.odmac, sizeof(g_test_fast_send_cb.odmac), brodcast_mac,sizeof(brodcast_mac));
        g_test_fast_send_cb.odtei = 0xfff;
    }

    demo_send_pkt_index_add();

    g_test_fast_send_cb.data_len = sizeof(g_test_fast_send_cb.data);
    (hi_void)memcpy_s(g_test_fast_send_cb.data.send_data, sizeof(g_test_fast_send_cb.data.send_data), data, 4);

    g_test_fast_send_cb.data.odtei = g_test_fast_send_cb.odtei;
    g_test_fast_send_cb.data.pkt_index = g_fast_send_pkt_index;
    g_test_fast_send_cb.pkt_index = g_fast_send_pkt_index;

    (hi_void)memcpy_s(sc_para->Data, g_test_fast_send_cb.data_len, (HI_VOID *)(&(g_test_fast_send_cb.data)), g_test_fast_send_cb.data_len);
    (hi_void)memcpy_s(sc_para->Odmac, sizeof(sc_para->Odmac), g_test_fast_send_cb.odmac, sizeof(g_test_fast_send_cb.odmac));
    sc_para->DataLength = sizeof(g_test_fast_send_cb.data);
    sc_para->SendMode = 1;
    sc_para->ResendNum = 1;
    sc_para->DataIndex = g_fast_send_pkt_index;
    sc_para->ScSendResultFun = demo_sc_send_end_proc;
    sc_para->SendPhase = PHASE_ALL;
    sc_para->Timeout = 50;
    
    HI_MDM_DisableSending(500);
    HI_MDM_FastSendOnScPlc(sc_para);
    HI_MDM_Free(HI_MOD_ID_USR, sc_para);
    g_test_fast_send_cb.send_cnt++;
    #endif
}



