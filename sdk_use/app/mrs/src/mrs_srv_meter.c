//*****************************************************************************
// 
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
// 
//*****************************************************************************
//  �� �� ��   : meter.h
//  �� �� ��   : V1.0 
//  ��    ��   : niesongsong
//  ��������   : 2012-08-16
//  ��������   : ���
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-16
//    ��    �� : niesongsong/kf62735
//    �޸����� : �����ļ� 
// 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_list.h"
#include "mrs_srv_meter.h"
#include "mrs_srv_sta_meter.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_fw_proto698_45.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) 
#define simu_check_hbit(x,y) (((x&0xf0) == (y&0xf0)) || ((x&0xf0) == 0xa0))
#define simu_check_lbit(x,y)  (((x&0x0f) == (y&0x0f)) || ((x&0x0f) == 0x0a))

typedef HI_U32 (*simu_afn_proc)(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);

typedef struct {
    HI_U32 afn_str_min;
    HI_U32 afn_str_max;
    simu_afn_proc aproc;
}afn_struct;

HI_PRV simu_alloc_fun simu_mem_alloc;
HI_PRV simu_free_fun simu_mem_free;

HI_PRV HI_U32 g_mrs_using_vmeter = 0;   /*ʹ��ģ����ı�־*/
HI_PRV HI_U32 g_ulVMEvtActiveDelay = 0; /*ģ����¼�������ʱ�¼�*/

static HI_VOID mrsStaListFreeNd(HI_VOID * lk,HI_VOID * m);

/*��������*/
HI_PRV HI_U32 simu_afn_f0(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f1(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f2(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f3(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f4(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f5(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f6(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f7(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f8(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f9(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_f10(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_reset_evtout_pin(HI_VOID);
HI_PRV HI_U32 simu_clean_evt_status(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_read_evt_status(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_evt_proc(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
HI_PRV HI_U32 simu_afn_fx(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);

HI_VOID mrsSimuSetOpenCoverEvt(HI_U8 ucEvtFlg);

HI_PRV simu_afn_proc simu_get_afn_index(HI_U32 afn);

/*������������*/
/*check�ຯ��(������)��0��ʾ�ɹ�������Ϊ������*/
HI_PRV HI_U32 simu_check_mac(HI_U8 src[6],HI_U8 dest[6]);

/*do�෵������ΪHI_U32�ĺ���(д����)0��ʾ�ɹ��������Ǵ�����*/
HI_PRV HI_U32 simu_do_decode(HI_U8 *buf,HI_U32 len);
HI_PRV HI_U32 simu_do_encode(HI_U8 *buf,HI_U32 len);
HI_PRV HI_U32 simu_do_reply(HI_U8 *,HI_U32 ,MRS_SIMU_METER *,MRS_MEM_BUF *);

/*��֯�ظ�֡����ȷ����0,����Ϊ������*/
HI_U32 simu_pack_frame(MRS_SIMU_METER *m, HI_U8 *content,HI_U16 len,MRS_MEM_BUF *out_buf);
/*����Ӧ��ַ֡*/
HI_U32 simu_ack_mframe(MRS_SIMU_METER *m, MRS_MEM_BUF *out_buf);
/*ȷ��Ӧ��*/
HI_U32 simu_resp_ack_frame(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
/*����Ӧ����֡*/
HI_U32 simu_ack_dframe(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);

/*����������*/
HI_PRV MRS_SIMU_METER * simu_do_create(HI_U8 ,HI_U8 []);
HI_PRV HI_VOID simu_do_destroy(MRS_SIMU_METER * m);
HI_PRV HI_VOID sim_get_all_rate(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *ucContent, HI_U32 ulModulus);
HI_PRV HI_U32 sim_get_total_rate(HI_U8 ucDILow, HI_U32 ulModulus);
HI_PRV HI_U32 sim_get_Wire(MRS_SIMU_METER *m, HI_U32 ulWire);
HI_PRV HI_U32 sim_count_wire(HI_U32 ulwire, HI_U32 ulwireBound, HI_U32 ulLimit);
HI_PRV HI_BOOL sim_ten_times_add(HI_U32 ulCurrtimes);
HI_PRV HI_VOID mrsStaConverVmAddr(HI_U8 *pucMAddr);

//ȫ�ֹ�����
afn_struct afn_data[] = {
    {0x05060001,0x05060001,simu_afn_f0},  /*�ն���ʱ��*/
    {0x05060101,0x0506013E,simu_afn_f1},  /*�ն��������й���������*/ 
    {0x00010000,0x00010000,simu_afn_f2},  /*�����й��ܵ���*/
    {0x04000501,0x04000501,simu_afn_f3},  /*�������״̬��1*/
    {0x04000503,0x04000503,simu_afn_f4},  /*�������״̬��3*/
    {0x00000100,0x00000100,simu_afn_f5},  /*����ַ*/
    {0x03300d00,0x03300d00,simu_afn_f6},  /*�����ܴ���*/
    {0x03110000,0x03110000,simu_afn_f7},  /*�����ܴ���*/
    {0x02800001,0x02800001,simu_afn_f8},  /*���ߵ���*/
    {0x02020100,0x02020100,simu_afn_f9},  /*���ߵ���*/
    {0x0001ff01,0x0001ff3E,simu_afn_f10}, /*��n�����������й��������ݿ�*/
    {0x04001501,0x04001504,simu_afn_evt_proc}, /*�¼�����*/ 
    {0x3f3f3f3f,0x3f3f3f3f,simu_afn_fx},  /*δʵ��*/
};


/*��֯�ظ�֡,��ȷ����0,����Ϊ������*/
HI_U32 simu_pack_frame(MRS_SIMU_METER *m, 
                       HI_U8 *content,HI_U16 len,
                       MRS_MEM_BUF *out_buf)
{
    HI_U8 *buf;
    HI_U16 tlen;

    if (!m || !out_buf) {
        return 0;
    }
    /*��շ�����*/
    (hi_void)memset_s(out_buf, sizeof(MRS_MEM_BUF), 0, sizeof(MRS_MEM_BUF));

    /*�ܳ���*/
    tlen = 12 + len;
    buf = simu_mem_alloc(tlen);
    if (!buf) {
        return 0;
    }
    /*��װ*/
    buf[0] = 0x68;                              
    (hi_void)memcpy_s(buf + 1, tlen-1, m->ucMeterAddr, HI_METER_ADDR_LEN); 
    buf[7] = 0x68;
    buf[8] = (HI_U8)0x91; 
    buf[9] = (HI_U8)len;
    
    /*Ӧ��������*/
    simu_do_encode(content,len);
    (hi_void)memcpy_s(buf+10, tlen-10, content, len);

    /*У���*/
    buf[tlen-2] = mrsToolsCalcCheckSum(buf,tlen-2);
    buf[tlen-1] = 0x16;

    out_buf->uiLen = (HI_U32)tlen;
    out_buf->ucBuf = buf;

    return 1;
}

/*����Ӧ��ַ֡*/
HI_U32 simu_ack_mframe(MRS_SIMU_METER *m, MRS_MEM_BUF *out_buf)
{
    HI_U8 *buf;
    HI_U16 tlen;
    HI_U8 mframe[] = {
        0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x68,
        0x93,0x06,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x16
    };   
    if (!m || !out_buf) {
        return 0;
    }
    /*���MAC*/
    (hi_void)memcpy_s(mframe+1, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(mframe+10, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    simu_do_encode(mframe+10,6);
    /*��������*/
    mframe[16] = mrsToolsCalcCheckSum(mframe,16);
    /*�ܳ���*/
    tlen = 18;
    buf = simu_mem_alloc(tlen);
    if (!buf) {
        return 0;
    }
    (hi_void)memcpy_s(buf, tlen, mframe, tlen);
    out_buf->ucBuf = buf;
    out_buf->uiLen = tlen;

    return 1;
}

/*����Ӧȷ��֡*/
HI_U32 simu_resp_ack_frame(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 *buf;
    HI_U16 tlen;
    HI_U8 mframe[] = {
        0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x68,
        0x80,0x00,0xFF,0x16
    };   
    if (!m || !out_buf) {
        return 0;
    }
    /*���MAC*/
    (hi_void)memcpy_s(mframe+1, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    /*������*/
    mframe[8] |= (cb[0] & 0x1f); /*1f: 000 11111  c0: 110 00000*/
    /*��������*/
    mframe[10] = mrsToolsCalcCheckSum(mframe,10);
    /*�ܳ���*/
    tlen = 12;
    buf = simu_mem_alloc(tlen);
    if (!buf) {
        return 0;
    }
    (hi_void)memcpy_s(buf, tlen, mframe, tlen);
    out_buf->ucBuf = buf;
    out_buf->uiLen = tlen;
    
    return 1;
}

/*����Ӧ����֡*/
HI_U32 simu_ack_dframe(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 *buf;
    HI_U16 tlen;
    HI_U8 mframe[] = {
        0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x68,
        0xC2,0x01,0x02,0xFF,0x16
    };   
    if (!m || !out_buf) {
        return 0;
    }
    /*���MAC*/
    (hi_void)memcpy_s(mframe+1, sizeof(mframe)-1, m->ucMeterAddr, HI_METER_ADDR_LEN);
    simu_do_encode(mframe+10,1);
    /*������*/
    mframe[8] = ((cb[0] & 0x1f) | 0xc0); /*1f: 000 11111  c0: 110 00000*/
    /*��������*/
    mframe[11] = mrsToolsCalcCheckSum(mframe,11);
    /*�ܳ���*/
    tlen = 13;
    buf = simu_mem_alloc(tlen);
    if (!buf) {
        return 0;
    }
    (hi_void)memcpy_s(buf, tlen, mframe, tlen);
    out_buf->ucBuf = buf;
    out_buf->uiLen = tlen;
    
    return 1;
}

/*�ն���ʱ��*/
HI_U32 simu_afn_f0(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[9] = {0};
    HI_U32 ret;

    HI_UNREF_PARAM(cb);

    /*ǰ��λΪ������*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*�ն���ʱ��*/
    mrsIntToBcd(m->uiTime,content+4,5);
    ret = simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);

    return ret;
}

/*�ն��������й���������*/ 
HI_U32 simu_afn_f1(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[24] = {0};

    HI_UNREF_PARAM(cb);

    sim_get_all_rate(di, m, ucContent, MRS_STA_SIM_F1_RATE_MODULUS);
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_U32 simu_afn_f10(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[24] = {0};

    HI_UNREF_PARAM(cb);

    sim_get_all_rate(di, m, ucContent, MRS_STA_SIM_F10_RATE_MODULUS);
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_VOID sim_get_all_rate(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *ucContent, HI_U32 ulModulus)
{
    HI_U32 ulTotalRate = 0;
    HI_U8 ucDILow = 0;
    HI_U8 ucOffset = 0;
    HI_U8 ucNum = sizeof(m->uiRate)/sizeof(HI_U32);

    /*ǰ��λΪ������*/
    (hi_void)memcpy_s(ucContent, sizeof(di), &di, sizeof(di));
    ucDILow = ucContent[0];

    ulTotalRate = sim_get_total_rate(ucDILow, ulModulus);
    m->uiRate[0] = ulTotalRate/ucNum;
    m->uiRate[3] = ulTotalRate/ucNum + ulTotalRate % ucNum;
    
    /*�����й������ĸ�����*/
    ucOffset += sizeof(di);
    mrsIntToBcd(ulTotalRate, ucContent + ucOffset, sizeof(ulTotalRate));
    ucOffset += sizeof(ulTotalRate);
    mrsIntToBcd(m->uiRate[0], ucContent + ucOffset, sizeof(m->uiRate[0]));
    ucOffset += sizeof(m->uiRate[0]);
    mrsIntToBcd(m->uiRate[0], ucContent + ucOffset, sizeof(m->uiRate[0]));
    ucOffset += sizeof(m->uiRate[0]);
    mrsIntToBcd(m->uiRate[0], ucContent + ucOffset, sizeof(m->uiRate[0]));
    ucOffset += sizeof(m->uiRate[0]);
    mrsIntToBcd(m->uiRate[3], ucContent + ucOffset, sizeof(m->uiRate[3]));
}

HI_U32 sim_get_total_rate(HI_U8 ucDILow, HI_U32 ulModulus)
{
    HI_PRV HI_U32 ulRandRate = 0;
    HI_U32 ulRate = 0;

    if(ucDILow > 62 || ucDILow < 1)
    {
        return 0;
    }

    if (!ulRandRate)
    {
        ulRandRate = HI_MDM_GetRandNum32(100, 0xFFFFFFFF);
    }
    
    ulRate = (ulRandRate << (ucDILow & 0x1F)) | (ulRandRate >> (32 - (ucDILow & 0x1F))) ;
    if (ucDILow & 0x20)
    {
        ulRate = ~ulRate;
    }

    return ulModulus * (62 - ucDILow) + ulRate % ulModulus;
}

/*�����й��ܵ���*/
HI_U32 simu_afn_f2(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[8] = {0};

    HI_UNREF_PARAM(cb);

    /*ǰ��λΪ������*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*�����й��ܵ���*/
    mrsIntToBcd(m->uiKWH,content+4,4);
    m->uiKWH++;
    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

/*�������״̬��1*/
HI_U32 simu_afn_f3(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(cb);
    
    /*ǰ��λΪ������*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*״̬��1*/
    (hi_void)memcpy_s(content+4, sizeof(content) - 4, &m->usWord1, 2);

    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

/*�������״̬��3*/
HI_U32 simu_afn_f4(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(cb);
    
    /*ǰ��λΪ������*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*״̬��3*/
    (hi_void)memcpy_s(content+4, sizeof(content) - 4, &m->usWord1, 2);
    
    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

HI_U32 simu_afn_f5(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(di);
    HI_UNREF_PARAM(cb);

    (hi_void)memcpy_s(content, sizeof(content), m->ucMeterAddr, sizeof(m->ucMeterAddr));

    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

HI_U32 simu_afn_f6(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[7] = {0};
    HI_PRV HI_U32 ulCurrtimes = 0;

    HI_UNREF_PARAM(cb);
    
    (hi_void)memcpy_s(ucContent, sizeof(ucContent), &di, sizeof(di));
    
    if (HI_TRUE == sim_ten_times_add(ulCurrtimes))
    {
        m->ulOpenTimes++;
    }

    mrsIntToBcd(m->ulOpenTimes, ucContent + sizeof(di), 3);
    ulCurrtimes++;
    
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_U32 simu_afn_f7(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[7] = {0};
    HI_PRV HI_U32 ulCurrtimes = 0;

    HI_UNREF_PARAM(cb);
    
    (hi_void)memcpy_s(ucContent, sizeof(ucContent), &di, sizeof(di));
    
    if (HI_TRUE == sim_ten_times_add(ulCurrtimes))
    {
        m->ulPowerOffTimes++;
    }

    mrsIntToBcd(m->ulPowerOffTimes, ucContent + sizeof(di), 3);
    ulCurrtimes++;
    
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_U32 simu_afn_f8(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[7] = {0};

    HI_UNREF_PARAM(cb);
    
    (hi_void)memcpy_s(ucContent, sizeof(ucContent), &di, sizeof(di));
    m->ulNeutralWire = sim_get_Wire(m, m->ulLiveWire);
    mrsIntToBcd(m->ulNeutralWire, ucContent + sizeof(di), 3);
    
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_U32 simu_afn_f9(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 ucContent[7] = {0};

    HI_UNREF_PARAM(cb);
    
    (hi_void)memcpy_s(ucContent, sizeof(ucContent), &di, sizeof(di));
    m->ulLiveWire = sim_get_Wire(m, m->ulNeutralWire);
    mrsIntToBcd(m->ulLiveWire, ucContent + sizeof(di), 3);
    
    return simu_pack_frame(m, ucContent, MRS_TOOLS_ALEN(ucContent), out_buf);
}

HI_PRV HI_U8 g_MeterEvtStatus[] = {0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

HI_VOID mrsSimuSetOpenCoverEvt(HI_U8 ucEvtFlg)
{
    if(ucEvtFlg)
    {
        // �������λ
        g_MeterEvtStatus[1] |= 0x04;
    }
    return;
}

HI_U32 simu_reset_evtout_pin(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MV_APP_SIMU_METER_INFO minfo = {0};
    
    //��ȡNV��
    ret = HI_MDM_NV_Read(ID_MV_APP_SIMU_METER_INFO, &minfo, sizeof(minfo));
    if(ret == HI_ERR_SUCCESS)
    {
        minfo.ucEvtOutFlg = 0;
        ret = HI_MDM_NV_Write(ID_MV_APP_SIMU_METER_INFO, &minfo, sizeof(minfo));
    }

    return ret;
}

HI_U32 simu_clean_evt_status(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U16 usIndex = 0;
    HI_U8 *pucReset = HI_NULL;

    MRS_NOT_USED(di);

    if(cb[1] < 4 + 4 + 4 + 12)    /*DI + PSW + OptCode + ResetWord*/
    {
        return 0;
    }

    pucReset = &cb[1 + 1 + 4 + 4 + 4]; /*AFN + LEN + DI + PSW + OptCode*/
    
    for(usIndex = 0; usIndex < MRS_TOOLS_ARRAY_COUNT(g_MeterEvtStatus); usIndex++)
    {
        g_MeterEvtStatus[usIndex] &= pucReset[usIndex];
    }

    simu_reset_evtout_pin();
    
    return simu_resp_ack_frame(m, cb, out_buf);
}

HI_U32 simu_read_evt_status(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U32 ret = 1;
    HI_U16 usIndex = 0;
    HI_U8 *pucContent = HI_NULL;
    HI_U8 ucEvtNum = 0;
    HI_U16 usDataLen = 0;
    HI_U16 usOffset = 0;

    HI_UNREF_PARAM(cb);

    for(usIndex = 0; usIndex < 12 * 8; usIndex++)
    {
        if(g_MeterEvtStatus[usIndex / 8] & (0x01 << (usIndex % 8)))
        {
            ucEvtNum++;
        }
    }

    usDataLen = 4 + 12 + 1 + ucEvtNum + 1;  /* DI + EvtStatus + AA + EvtNum + AA */
    pucContent = mrsToolsMalloc(usDataLen);
    if(!pucContent)
    {
        return 0;
    }
    
    (hi_void)memcpy_s(pucContent, usDataLen, &di, sizeof(di));
    usOffset += sizeof(di);
    
    (hi_void)memcpy_s(pucContent + usOffset, usDataLen - usOffset, g_MeterEvtStatus, 12);
    usOffset += 12;

    pucContent[usOffset++] = 0xAA;

    (hi_void)memset_s(pucContent + usOffset, usDataLen - usOffset, 0x01, ucEvtNum);
    usOffset += ucEvtNum;
    
    pucContent[usOffset++] = 0xAA;
    
    ret = simu_pack_frame(m, pucContent, usDataLen, out_buf);

    mrsToolsFree(pucContent);

    return ret;

}


HI_U32 simu_afn_evt_proc(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    switch(di)
    {
    case 0x04001501:    // �����ϱ�״̬��
        ret = simu_read_evt_status(di, m, cb, out_buf);
        break;
        
    case 0x04001503:    // ��λ�����ϱ�״̬��
        ret = simu_clean_evt_status(di, m, cb, out_buf);
        break;
        
    default:
        return simu_ack_dframe(m, cb, out_buf);
    }

    return ret;
}


HI_U32 sim_get_Wire(MRS_SIMU_METER *m, HI_U32 ulWire)
{
    HI_U8 ucRunTimes = (HI_U8)m->ulRunTimes;
    HI_U32 ulSimWire = 0;

    ucRunTimes++;
    ucRunTimes = ((ucRunTimes == 60) ? 0 : ucRunTimes);
    m->ulRunTimes = ucRunTimes;

    if(ucRunTimes < 30)
    {
        ulSimWire = sim_count_wire(ulWire, 300, 0);
    }
    else if(ucRunTimes < 50)
    {
        ulSimWire = sim_count_wire(ulWire, 500, 300);
    }
    else
    {
        ulSimWire = sim_count_wire(ulWire, 4000, 500);
    }
    
    return ulSimWire;
}

/*δʵ�ֵĹ���,�ظ�����*/
HI_U32 simu_afn_fx(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    MRS_NOT_USED(di);
    return simu_ack_dframe(m,cb,out_buf);
}

HI_U32 sim_count_wire(HI_U32 ulwire, HI_U32 ulwireBound, HI_U32 ulLimit)
{
    HI_U16 usRandWire = 0;
    HI_U32 ulSimWire = 0;

    usRandWire = HI_MDM_GetRandNum16(0, 200);
    if((usRandWire % 2 == 0) && (ulwire > ulwireBound))
    {
        ulSimWire = ulwire - ulLimit - (usRandWire % 200);
    }
    else
    {
        ulSimWire = ulwire + ulLimit + (usRandWire % 200);
    }

    return ulSimWire;
}

HI_BOOL sim_ten_times_add(HI_U32 ulCurrtimes)
{
    if(ulCurrtimes < 20)
    {
        if(HI_MDM_GetRandNum16((HI_U16)0, (HI_U16)(81-ulCurrtimes)) == 1)
        {
            return HI_TRUE;
        }
    }
    else
    {
        if(ulCurrtimes % 32 == 0)
        {
            return HI_TRUE;
        }
        
        if(HI_MDM_GetRandNum16(0, 6) == 1)
        {
            return HI_TRUE;
        }
    }
    
    return HI_FALSE;
}

HI_U32 simu_do_decode(HI_U8 *buf,HI_U32 len)
{
    while(len--) {
        buf[len] -= 0x33;
    }
    return 0;    
}

HI_U32 simu_do_encode(HI_U8 *buf,HI_U32 len)
{
    while(len--) {
        buf[len] += 0x33;
    }
    return 0;  
}

simu_afn_proc simu_get_afn_index(HI_U32 di)
{
    HI_U32 i = 0;
    
    for (i = 0;i < MRS_TOOLS_ALEN(afn_data);i++) {
        if ((di >= afn_data[i].afn_str_min) && (di <= afn_data[i].afn_str_max))
        {
            return afn_data[i].aproc;
        }
    }
    
    return afn_data[i-1].aproc;
}

HI_U32 simu_do_reply(HI_U8 *p,HI_U32 len,MRS_SIMU_METER *m, MRS_MEM_BUF *buf)
{
    HI_U32 afn = 0;
    simu_afn_proc aproc = 0;

    HI_UNREF_PARAM(len);

    if (!p || !buf || !m) {
        return (HI_U32)-1;
    }
    
    /*control*/
    if (p[8] == 0x13) {
        return simu_ack_mframe(m,buf);
    } else if ((p[8] != 0x11) && (p[8] != 0x14)){
        return simu_ack_dframe(m, &p[8], buf);
    }

    (hi_void)memcpy_s(&afn, sizeof(afn), p + 10, sizeof(afn));
    aproc = simu_get_afn_index(afn);
    if (aproc) {
        HI_U32 ret = 0;
        (hi_void)memset_s(buf, sizeof(MRS_MEM_BUF), 0, sizeof(MRS_MEM_BUF));
        ret = aproc(afn, m, &p[8], buf);
        if (ret == 0) {
            simu_mem_free(buf->ucBuf);
            buf->uiLen = 0;
        }
        return ret;
    }
    return (HI_U32)-1;
}


/*dest������bcd���,src������ͨ���*/
HI_U32 simu_check_mac(HI_U8 src[6],HI_U8 dest[6])
{
    HI_U32 i = 0;
    for (i = 0;i < 6 ;i++) {
        if (!simu_check_hbit(src[i],dest[i])) {
            return 0;
        }
        if (!simu_check_lbit(src[i],dest[i])) {
            return 0;
        }
    }
    return 1;
}

MRS_SIMU_METER * simu_do_create(HI_U8 ver,HI_U8 maddr[6])
{
    MRS_SIMU_METER * m = 0;
    if ((!mrsCheck645Proto(ver)) 
     || !maddr || !mrsToolsCheckBCD(maddr,6)) {
        return m;
    }
    m = simu_mem_alloc(sizeof(MRS_SIMU_METER));
    if (m) {
        (hi_void)memset_s(m, sizeof(MRS_SIMU_METER), 0, sizeof(MRS_SIMU_METER));
        m->ucVer = METER_PROTO_645_2007;        /*�汾Ϊ2007*/

        m->uiTime = 1208160000;     /*����ʱ��,2012-08-16 00:00*/
        m->uiRate[0] = 10;          /*�ն���+A ����1*/
        m->uiRate[1] = 20;          /*�ն���+A ����2*/
        m->uiRate[2] = 40;          /*�ն���+A ����3*/
        m->uiRate[3] = 30;           /*�ն���+A ����4*/

        m->uiKWH = 200;              /*��ǰ�����й��ܵ���*/

        m->usWord1 = 0x0200;        /*���״̬��1*/
        m->usWord3 = 0x0400;        /*���״̬��3*/

        /*����ַ*/
        (hi_void)memcpy_s(m->ucMeterAddr, sizeof(m->ucMeterAddr), maddr, HI_METER_ADDR_LEN);
    }
    return m;
}

HI_VOID simu_do_destroy(MRS_SIMU_METER * meter)
{
    if (meter) 
    {
        simu_mem_free(meter);
    }
} 

//*****************************************************************************
// ��������: mrsSrvMeterInit
// ��������: ��ʼ�����ģ��     
// ����˵��:
//   MRS_MET_MODULE * [IN] ���ģ��
//   HI_U16           [IN] �������
// �� �� ֵ: 
//    �ɹ�����HI_ERR_SUCCESS
// ����Ҫ��: 
// ���þ���: 
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrsSrvMeterInit(MRS_MET_MODULE * mmodule,HI_U16 rnum,
                       simu_alloc_fun afun,simu_free_fun ffun)
{
    HI_U8 index = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!mmodule||!afun||!ffun||rnum > PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    //��ʼ��
    (hi_void)memset_s(mmodule, sizeof(MRS_MET_MODULE), 0, sizeof(MRS_MET_MODULE));
    simu_mem_alloc = afun;
    simu_mem_free = ffun;

    mmodule->pfnMalloc = afun;
    mmodule->pfnFree = ffun;

    //��ӵ��
    for (index = 0;index < rnum; index++) 
    {
        
        HI_U8 maddr[6] = {0};
#ifndef SAL_HAVE_FEATURE_UT            
        mrsStaConverVmAddr(maddr);
#else
        mrsIntToBcd(index,maddr,6);  
#endif
        //��ӵ��
        ret = mrsSrvMeterAdd(mmodule,maddr);        
        if (ret != HI_ERR_SUCCESS) 
        {
            break;
        }
    }
    return ret;
}

//*****************************************************************************
// ��������: mrsSrvMeterAdd
// ��������: ��ӵ��ģ��     
// ����˵��:
//   MRS_MET_MODULE * [IN] ���ģ��
//   HI_U8          * [IN] ����ַ
// �� �� ֵ: 
//    �ɹ�����HI_ERR_SUCCESS
// ����Ҫ��: 
// ���þ���: 
// ��    ��: niesongsong/kf62735 [2011-12-13]
// ��    ע: ����ַ�⣬�������ΪĬ��ֵ
//*****************************************************************************
HI_U32 mrsSrvMeterAdd(MRS_MET_MODULE * mmodule,HI_U8 * maddr)
{
    MRS_SIMU_METER *meter;
    HI_U16 index;
    
    //�����ж�
    if (!mmodule || !maddr) 
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //�������
    meter = simu_do_create(METER_PROTO_645_1997, maddr);
    if (!meter) 
    {
        return HI_ERR_FAILURE;
    }

    //��Ų����ظ�
    for (index = 0; index < mmodule->usNum;index++)
    {
        if (mrsToolsMemEq(maddr,mmodule->pstMList[index]->ucMeterAddr,6))
        {
            return HI_ERR_FAILURE;
        }
    }

    //��ӵ��(���ܳ���64��)
    if (mmodule->usNum >= PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM) 
    {
        simu_do_destroy(meter);
        return HI_ERR_FULL;
    }

    mmodule->pstMList[mmodule->usNum] = meter;
    mmodule->usNum++;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsSrvMeterReply
// ��������: ���ظ�     
// ����˵��:
//   MRS_MET_MODULE * [IN]  ���ģ��
//   MRS_MEM_BUF    * [IN]  ����
//   MRS_SRV_LIST   * [IN/OUT]  ��������б�(������ظ�)
//   HI_VOID        * [IN] ���ڴ����뺯��
// �� �� ֵ: 
// ����Ҫ��: 
// ���þ���: 
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrsSrvMetersReply(MRS_MET_MODULE * mmodule,MRS_MEM_BUF * inbuf,MRS_SRV_LIST *list)
{
    HI_U16 mindex;
    MRS_SIMU_METER *meter;
    HI_U8 *temp;
    HI_U8 * rdata;
    HI_U32 rlen;
    HI_BOOL fond = HI_FALSE;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usStart = 0;
    HI_U16 usLength = 0;
    
    //�����ж�
    if (!mmodule || !inbuf || !inbuf->ucBuf || !list || !simu_mem_alloc) 
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsFind645Frame(inbuf->ucBuf, (HI_S16)inbuf->uiLen, &usStart, &usLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    rdata = inbuf->ucBuf + usStart;
    rlen = usLength;

    temp = simu_mem_alloc(rlen);
    if (!temp) 
    {
        return HI_ERR_FAILURE;
    }
    //��֡
    (hi_void)memcpy_s(temp, rlen, rdata, rlen);
    simu_do_decode((HI_U8 *)(temp+10),rdata[9]);

    for (mindex = 0;mindex < mmodule->usNum;mindex++) 
    {
        MRS_MEM_BUF outbuf = {0};
        meter = mmodule->pstMList[mindex];
        if (meter) 
        {
            //���MAC
            if (!simu_check_mac(rdata + 1,meter->ucMeterAddr))
            {
                continue;
            }

            fond = HI_TRUE;
            
            //������ظ��ɹ�
            if (simu_do_reply(temp,rlen,meter,&outbuf))
            {
                MRS_MEM_LINK * block = simu_mem_alloc(sizeof(MRS_MEM_LINK));
                if (block) 
                {
                    (hi_void)memset_s(block, sizeof(MRS_MEM_LINK), 0, sizeof(MRS_MEM_LINK));
                    block->buf.ucBuf = outbuf.ucBuf;
                    block->buf.uiLen = outbuf.uiLen;
                    mrsSrvListInsTail(list,&block->link);

                    break;
                }
            }
        }
    }

    if(fond == HI_FALSE)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("sim meter addr not exist"),rdata + 1, 6);

        for(mindex = 0; mindex < mmodule->usNum; mindex++)
        {
            MRS_SIMU_METER *meter_t;

            meter_t = mmodule->pstMList[mindex];

            if(meter_t)
            {
                HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("sim meter addr"), meter_t->ucMeterAddr, 6);
            }
        }
    }

    simu_mem_free(temp);
    return 0;
}


HI_VOID mrsStaListFreeNd(HI_VOID * lk,HI_VOID * m)
{
    //���ͷ����ݣ����ͷŽ��
    MRS_MET_MODULE * md;
    MRS_MEM_LINK * nd;

    md = (MRS_MET_MODULE *)m;
    nd = mrs_list_entry(lk,MRS_MEM_LINK,link);

    md->pfnFree(nd->buf.ucBuf);
    md->pfnFree(nd);
}

//*****************************************************************************
// ��������: mrsStaSend2Meter
// ��������: �����ݷ��͸����    
// ����˵��:
//   MRS_MET_MODULE       [IN] ģ����ģ��
//   HI_PBYTE             [IN] ����
//   HI_U16               [IN] ���ݳ���
// �� �� ֵ: 
//   �ɹ�����  HI_ERR_SUCCESS
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaSend2Meter(MRS_MET_MODULE * md, HI_PBYTE pucData, HI_U16 usDataSize, HI_U8 ucProtocol)
{
    if (METER_PROTO_698_45 == ucProtocol)
    {
        return mrsStaSend2Meter698(md, pucData, usDataSize);
    }
    else
    {
        return mrsStaSend2Meter645(md, pucData, usDataSize);
    }
}


HI_U32 mrsStaSend2Meter645(MRS_MET_MODULE * md, HI_PBYTE pucData, HI_U16 usDataSize)
{
    MRS_SRV_LIST mlist;
    MRS_SRV_LINK *link;
    MRS_MEM_LINK *outbuf;
    HI_U32 ret = 0;

    MRS_MEM_BUF inbuf = {0};
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[VM-MR]-TX"), pucData, usDataSize); 
    
    //��ʼ���ظ��б�
    mrsSrvInitList(&mlist);

    //��ʼ������
    inbuf.ucBuf = pucData;
    inbuf.uiLen = usDataSize;

    //��ȡ���ظ�
    mrsSrvMetersReply(md,&inbuf,&mlist);
    
    link = mlist.next;
    for (link = mlist.next;link != &mlist;link = link->next)
    {
        outbuf = mrs_list_entry(link,MRS_MEM_LINK,link);
        
        // ��ȡ��������֡����
        ret = mrs645ProtoStreamInput(outbuf->buf.ucBuf, (HI_U16)outbuf->buf.uiLen, &stFrame, ID_MRS_UART_645BUF);
        if(HI_ERR_SUCCESS == ret)
        {
            HI_SYS_QUEUE_MSG_S stMsg = { 0 }; 
            MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&stMsg.ulParam[0];

            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[VM-MR]-RX"), stFrame.pucDatagram, stFrame.usDatagramSize); 
            
            (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
            stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

            // ������Ϣ�� MRS����, ������Ϣ�ַ����� 
            ret = mrsSendMessage2Queue(&stMsg);
            if(ret != HI_ERR_SUCCESS)
            {
                mrsToolsFree(stFrame.pucDatagram);
            }
            
            break;
        }
    }
    
    //�ͷ�
    mrsSrvListClear(&mlist,mrsStaListFreeNd,md);
    return ret;
}


// �����й����ܣ��ܼ����ʵ���������
HI_U8 g_aucReplyFrame698[] = 
{
    0x68,
    0x34,0x00,
    0xC3,
    0x05,0x07,0x00,0x26,0x04,0x17,0x20,0x00,
    0xC2,0x65,
    0x85,0x01,0x00,0x00,0x10,0x02,0x00,0x01,0x01,0x05,0x06,0x00,0x00,0x00,0x00,0x06,
    0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0xA8,0x58,
    0x16
};

HI_U32 mrsStaSend2Meter698(MRS_MET_MODULE * md, HI_PBYTE pucData, HI_U16 usDataSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U16 i = 0;
    HI_BOOL bFound = HI_FALSE;

    if (!md || !pucData)
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[VM-MR]-TX"), pucData, usDataSize);

    ulRet = mrsFind698MeterAddr(pucData, usDataSize, aucMeterAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    for (i = 0; i < md->usNum; i++)
    {
        MRS_SIMU_METER *pstMeter = md->pstMList[i];
        if (!pstMeter)
        {
            continue;
        }

        if (!mrsToolsMemEq(aucMeterAddr, pstMeter->ucMeterAddr, HI_METER_ADDR_LEN))
        {
            continue;
        }

        bFound = HI_TRUE;
    }

    if (bFound)
    {
        HI_SYS_QUEUE_MSG_S stMsg;
        MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;
        HI_U8 *pucPayload = g_aucReplyFrame698;
        HI_U16 usPayloadLen = sizeof(g_aucReplyFrame698);

        MRS_698_HEAD_STRU *pHead = (MRS_698_HEAD_STRU *)pucPayload;
        HI_U16 usFCS = 0;

        (hi_void)memcpy_s(pHead->aucAddr, HI_METER_ADDR_LEN, aucMeterAddr, HI_METER_ADDR_LEN);
        pHead->usHCS = mrs698Crc16(MRS_698_CRC_INIT, pucPayload + 1, sizeof(MRS_698_HEAD_STRU) - 3);
        usFCS = mrs698Crc16(MRS_698_CRC_INIT, pucPayload + 1, usPayloadLen - 4);
        (hi_void)memcpy_s(pucPayload + usPayloadLen - 3, sizeof(HI_U16), &usFCS, sizeof(usFCS));

        (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));

        pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&stMsg.ulParam[0];
        pstFrame->pucDatagram = (HI_PBYTE)mrsToolsMalloc(usPayloadLen);
        if (!pstFrame->pucDatagram)
        {
            return HI_ERR_FAILURE;
        }

        (hi_void)memcpy_s(pstFrame->pucDatagram, usPayloadLen, pucPayload, usPayloadLen);
        pstFrame->usDatagramSize = usPayloadLen;

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("[VM-MR]-RX"), pucPayload, usPayloadLen); 

        stMsg.ulMsgId = EN_MRS_FW_MSG_698_FRAME_INPUT;

        ulRet = mrsSendMessage2Queue(&stMsg);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstFrame->pucDatagram);
        }

        return ulRet;
    }

    return HI_ERR_FAILURE;
}


HI_VOID mrsStaConverVmAddr(HI_U8 *pucMAddr)
{
    HI_U32 l = 0;
    HI_U32 h = 0;
    
    mrsToolsGetLocalMac(pucMAddr);
    h = ((HI_U32)pucMAddr[2] * 1000) + pucMAddr[3];
    l = ((HI_U32)pucMAddr[4] * 1000) + pucMAddr[5];
    
    mrsIntToBcd(h, pucMAddr + 3, 3);
    mrsIntToBcd(l, pucMAddr, 3);
    
    return;
}

// ͬ��������ַ��Mac��
HI_VOID mrsSyncVMAddrToMac(HI_U8 ucProtocol)
{
    HI_U8 maddr[6] = {0};
        
    mrsStaConverVmAddr(maddr);
    mrsStaUpdateAddr(maddr, ucProtocol);  //���±��ַ
    mrsStaVMAddr2MeterList(maddr, ucProtocol);
    mrsStaUpdateTopAddrInf(maddr, ucProtocol);
    
    mrsStaSetMeterAddr(maddr);
    mrsStaSetMeterProtocol(ucProtocol);
    mrsStaSetMeterBaudrate((HI_U16)mrsGetCurBaudRate());
    HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
}


HI_U32 mrsStaGetSupportVM()
{
    if(g_mrs_using_vmeter == NV_SIMU_METER_ENABLE)
    {
        return (HI_U32)NV_SIMU_METER_ENABLE;
    }
    else
    {
        return (HI_U32)NV_SIMU_METER_DISABLE;
    }
}
HI_U32 mrsStaSetSupportVM(HI_U32 flag)
{
    g_mrs_using_vmeter = flag;
    
    mrsDfxVmMeterStatus((HI_U8)flag, HI_FALSE);
    
    return g_mrs_using_vmeter;
}

HI_VOID mrsStaSetVMActiveDelay(HI_U32 ulTime)
{
    g_ulVMEvtActiveDelay = ulTime;
    return;
}

HI_U32 mrsStaGetVMActiveDelay(HI_VOID)
{
    return g_ulVMEvtActiveDelay;
}

#endif
