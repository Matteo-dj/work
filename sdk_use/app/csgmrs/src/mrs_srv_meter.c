//*****************************************************************************
// 
//                  版权所有 (C), 1998-2011, 华为技术有限公司
// 
//*****************************************************************************
//  文 件 名   : meter.h
//  版 本 号   : V1.0 
//  作    者   : niesongsong
//  生成日期   : 2012-08-16
//  功能描述   : 电表
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2012-08-16
//    作    者 : niesongsong/kf62735
//    修改内容 : 创建文件 
// 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_list.h"
#include "mrs_srv_meter.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) 
#define simu_check_hbit(x,y) (((x&0xf0) == (y&0xf0)) || ((x&0xf0) == 0xa0))
#define simu_check_lbit(x,y)  (((x&0x0f) == (y&0x0f)) || ((x&0x0f) == 0x0a))

#define VER_2007 MRS_STA_PROTOCOL_2007 
#define VER_1997 MRS_STA_PROTOCOL_1997 

typedef HI_U32 (*simu_afn_proc)(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);

typedef struct {
    HI_U32 afn_str_min;
    HI_U32 afn_str_max;
    simu_afn_proc aproc;
}afn_struct;

HI_PRV simu_alloc_fun simu_mem_alloc;
HI_PRV simu_free_fun simu_mem_free;

HI_PRV HI_U32 g_mrs_using_vmeter = 0;   /*使用模拟电表的标志*/
HI_PRV HI_U32 g_ulVMEvtActiveDelay = 0; /*模拟表事件激活延时事件*/

static HI_VOID mrsStaListFreeNd(HI_VOID * lk,HI_VOID * m);

/*处理申明*/
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

/*辅助函数申明*/
/*check类函数(读函数)非0表示成功，其它为错误码*/
HI_PRV HI_U32 simu_check_mac(HI_U8 src[6],HI_U8 dest[6]);

/*do类返回类型为HI_U32的函数(写函数)0表示成功，其它是错误码*/
HI_PRV HI_U32 simu_do_decode(HI_U8 *buf,HI_U32 len);
HI_PRV HI_U32 simu_do_encode(HI_U8 *buf,HI_U32 len);
HI_PRV HI_U32 simu_do_reply(HI_U8 *,HI_U32 ,MRS_SIMU_METER *,MRS_MEM_BUF *);

/*组织回复帧，正确返回0,其它为错误码*/
HI_U32 simu_pack_frame(MRS_SIMU_METER *m, HI_U8 *content,HI_U16 len,MRS_MEM_BUF *out_buf);
/*表响应地址帧*/
HI_U32 simu_ack_mframe(MRS_SIMU_METER *m, MRS_MEM_BUF *out_buf);
/*确认应答*/
HI_U32 simu_resp_ack_frame(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);
/*表响应否认帧*/
HI_U32 simu_ack_dframe(MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf);

/*构造与析构*/
HI_PRV MRS_SIMU_METER * simu_do_create(HI_U8 ,HI_U8 []);
HI_PRV HI_VOID simu_do_destroy(MRS_SIMU_METER * m);
HI_PRV HI_VOID sim_get_all_rate(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *ucContent, HI_U32 ulModulus);
HI_PRV HI_U32 sim_get_total_rate(HI_U8 ucDILow, HI_U32 ulModulus);
HI_PRV HI_U32 sim_get_Wire(MRS_SIMU_METER *m, HI_U32 ulWire);
HI_PRV HI_U32 sim_count_wire(HI_U32 ulwire, HI_U32 ulwireBound, HI_U32 ulLimit);
HI_PRV HI_BOOL sim_ten_times_add(HI_U32 ulCurrtimes);
HI_PRV HI_VOID mrsStaConverVmAddr(HI_U8 *pucMAddr);

//全局功能码
afn_struct afn_data[] = {
    {0x05060001,0x05060001,simu_afn_f0},  /*日冻结时钟*/
    {0x05060101,0x0506013E,simu_afn_f1},  /*日冻结正向有功电能数据*/ 
    {0x00010000,0x00010000,simu_afn_f2},  /*正向有功总电能*/
    {0x04000501,0x04000501,simu_afn_f3},  /*电表运行状态字1*/
    {0x04000503,0x04000503,simu_afn_f4},  /*电表运行状态字3*/
    {0x00000100,0x00000100,simu_afn_f5},  /*电表地址*/
    {0x03300d00,0x03300d00,simu_afn_f6},  /*开盖总次数*/
    {0x03110000,0x03110000,simu_afn_f7},  /*掉电总次数*/
    {0x02800001,0x02800001,simu_afn_f8},  /*零线电流*/
    {0x02020100,0x02020100,simu_afn_f9},  /*火线电流*/
    {0x0001ff01,0x0001ff3E,simu_afn_f10}, /*上n结算日正向有功电能数据块*/
    {0x04001501,0x04001504,simu_afn_evt_proc}, /*事件处理*/ 
    {0x3f3f3f3f,0x3f3f3f3f,simu_afn_fx},  /*未实现*/
};


/*组织回复帧,正确返回0,其它为错误码*/
HI_U32 simu_pack_frame(MRS_SIMU_METER *m, 
                       HI_U8 *content,HI_U16 len,
                       MRS_MEM_BUF *out_buf)
{
    HI_U8 *buf;
    HI_U16 tlen;

    if (!m || !out_buf) {
        return 0;
    }
    /*清空发送区*/
    (hi_void)memset_s(out_buf, sizeof(MRS_MEM_BUF), 0, sizeof(MRS_MEM_BUF));

    /*总长度*/
    tlen = 12 + len;
    buf = simu_mem_alloc(tlen);
    if (!buf) {
        return 0;
    }
    /*组装*/
    buf[0] = 0x68;                              
    (hi_void)memcpy_s(buf + 1, tlen-1, m->ucMeterAddr, HI_METER_ADDR_LEN); 
    buf[7] = 0x68;
    buf[8] = (HI_U8)0x91; 
    buf[9] = (HI_U8)len;
    
    /*应用域内容*/
    simu_do_encode(content,len);
    (hi_void)memcpy_s(buf+10, tlen-10, content, len);

    /*校验和*/
    buf[tlen-2] = mrsToolsCalcCheckSum(buf,tlen-2);
    buf[tlen-1] = 0x16;

    out_buf->uiLen = (HI_U32)tlen;
    out_buf->ucBuf = buf;

    return 1;
}

/*表响应地址帧*/
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
    /*填充MAC*/
    (hi_void)memcpy_s(mframe+1, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(mframe+10, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    simu_do_encode(mframe+10,6);
    /*计算较验和*/
    mframe[16] = mrsToolsCalcCheckSum(mframe,16);
    /*总长度*/
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

/*表响应确认帧*/
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
    /*填充MAC*/
    (hi_void)memcpy_s(mframe+1, HI_METER_ADDR_LEN, m->ucMeterAddr, HI_METER_ADDR_LEN);
    /*控制码*/
    mframe[8] |= (cb[0] & 0x1f); /*1f: 000 11111  c0: 110 00000*/
    /*计算较验和*/
    mframe[10] = mrsToolsCalcCheckSum(mframe,10);
    /*总长度*/
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

/*表响应否认帧*/
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
    /*填充MAC*/
    (hi_void)memcpy_s(mframe+1, sizeof(mframe)-1, m->ucMeterAddr, HI_METER_ADDR_LEN);
    simu_do_encode(mframe+10,1);
    /*控制码*/
    mframe[8] = ((cb[0] & 0x1f) | 0xc0); /*1f: 000 11111  c0: 110 00000*/
    /*计算较验和*/
    mframe[11] = mrsToolsCalcCheckSum(mframe,11);
    /*总长度*/
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

/*日冻结时钟*/
HI_U32 simu_afn_f0(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[9] = {0};
    HI_U32 ret;

    HI_UNREF_PARAM(cb);

    /*前四位为功能码*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*日冻结时钟*/
    mrsIntToBcd(m->uiTime,content+4,5);
    ret = simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);

    return ret;
}

/*日冻结正向有功电能数据*/ 
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

    /*前四位为功能码*/
    (hi_void)memcpy_s(ucContent, sizeof(di), &di, sizeof(di));
    ucDILow = ucContent[0];

    ulTotalRate = sim_get_total_rate(ucDILow, ulModulus);
    m->uiRate[0] = ulTotalRate/ucNum;
    m->uiRate[3] = ulTotalRate/ucNum + ulTotalRate % ucNum;
    
    /*正向有功电能四个费率*/
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

/*正向有功总电能*/
HI_U32 simu_afn_f2(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[8] = {0};

    HI_UNREF_PARAM(cb);

    /*前四位为功能码*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*正向有功总电能*/
    mrsIntToBcd(m->uiKWH,content+4,4);
    m->uiKWH++;
    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

/*电表运行状态字1*/
HI_U32 simu_afn_f3(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(cb);
    
    /*前四位为功能码*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*状态字1*/
    (hi_void)memcpy_s(content+4,sizeof(content)-4,&m->usWord1,2);

    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

/*电表运行状态字3*/
HI_U32 simu_afn_f4(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(cb);
    
    /*前四位为功能码*/
    (hi_void)memcpy_s(content, sizeof(content), &di, sizeof(di));

    /*状态字3*/
    (hi_void)memcpy_s(content+4,sizeof(content)-4,&m->usWord1,2);
    
    return simu_pack_frame(m,content,MRS_TOOLS_ALEN(content),out_buf);
}

HI_U32 simu_afn_f5(HI_U32 di, MRS_SIMU_METER *m, HI_U8 *cb, MRS_MEM_BUF *out_buf)
{
    HI_U8 content[6] = {0};

    HI_UNREF_PARAM(di);
    HI_UNREF_PARAM(cb);

    (hi_void)memcpy_s(content,sizeof(content), m->ucMeterAddr, sizeof(m->ucMeterAddr));

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
        // 开表盖置位
        g_MeterEvtStatus[1] |= 0x04;
    }
    return;
}

HI_U32 simu_reset_evtout_pin(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MV_APP_SIMU_METER_INFO minfo = {0};
    
    //读取NV项
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
    case 0x04001501:    // 主动上报状态字
        ret = simu_read_evt_status(di, m, cb, out_buf);
        break;
        
    case 0x04001503:    // 复位主动上报状态字
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

/*未实现的功能,回复否认*/
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
        if (ulCurrtimes % 32 == 0)
        {
            return HI_TRUE;
        }
        
        if (HI_MDM_GetRandNum16(0, 6) == 1)
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
        (hi_void)memset_s(buf,sizeof(MRS_MEM_BUF), 0,sizeof(MRS_MEM_BUF));
        ret = aproc(afn, m, &p[8], buf);
        if (ret == 0) {
            simu_mem_free(buf->ucBuf);
            buf->uiLen = 0;
        }
        return ret;
    }
    return (HI_U32)-1;
}


/*dest必须是bcd码的,src可能有通配符*/
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
    if (((ver != VER_2007) && (ver != VER_1997)) 
     || !maddr || !mrsToolsCheckBCD(maddr,6)) {
        return m;
    }
    m = simu_mem_alloc(sizeof(MRS_SIMU_METER));
    if (m) {
        (hi_void)memset_s(m, sizeof(MRS_SIMU_METER), 0, sizeof(MRS_SIMU_METER));
        m->ucVer = VER_2007;        /*版本为1997*/

        m->uiTime = 1208160000;     /*冻结时间,2012-08-16 00:00*/
        m->uiRate[0] = 10;          /*日冻结+A 费率1*/
        m->uiRate[1] = 20;          /*日冻结+A 费率2*/
        m->uiRate[2] = 40;          /*日冻结+A 费率3*/
        m->uiRate[3] = 30;           /*日冻结+A 费率4*/

        m->uiKWH = 200;              /*当前正向有功总电能*/

        m->usWord1 = 0x0200;        /*电表状态字1*/
        m->usWord3 = 0x0400;        /*电表状态字3*/

        /*电表地址*/
        (hi_void)memcpy_s(m->ucMeterAddr,sizeof(m->ucMeterAddr),maddr,HI_METER_ADDR_LEN);
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
// 函数名称: mrsSrvMeterInit
// 功能描述: 初始化电表模块     
// 参数说明:
//   MRS_MET_MODULE * [IN] 电表模块
//   HI_U16           [IN] 电表数量
// 返 回 值: 
//    成功返回HI_ERR_SUCCESS
// 调用要求: 
// 调用举例: 
// 作    者: niesongsong/kf62735 [2011-12-13]
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
    
    //初始化
    (hi_void)memset_s(mmodule,sizeof(MRS_MET_MODULE), 0,sizeof(MRS_MET_MODULE));
    simu_mem_alloc = afun;
    simu_mem_free = ffun;

    mmodule->pfnMalloc = afun;
    mmodule->pfnFree = ffun;

    //添加电表
    for (index = 0;index < rnum; index++) 
    {
        
        HI_U8 maddr[6] = {0};
#ifndef SAL_HAVE_FEATURE_UT            
        mrsStaConverVmAddr(maddr);
#else
        mrsIntToBcd(index,maddr,6);  
#endif
        //添加电表
        ret = mrsSrvMeterAdd(mmodule,maddr);        
        if (ret != HI_ERR_SUCCESS) 
        {
            break;
        }
    }
    return ret;
}

//*****************************************************************************
// 函数名称: mrsSrvMeterAdd
// 功能描述: 添加电表模块     
// 参数说明:
//   MRS_MET_MODULE * [IN] 电表模块
//   HI_U8          * [IN] 电表地址
// 返 回 值: 
//    成功返回HI_ERR_SUCCESS
// 调用要求: 
// 调用举例: 
// 作    者: niesongsong/kf62735 [2011-12-13]
// 备    注: 除地址外，其它项均为默认值
//*****************************************************************************
HI_U32 mrsSrvMeterAdd(MRS_MET_MODULE * mmodule,HI_U8 * maddr)
{
    MRS_SIMU_METER *meter;
    HI_U16 index;
    
    //参数判断
    if (!mmodule || !maddr) 
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //创建电表
    meter = simu_do_create(MRS_STA_PROTOCOL_1997,maddr);
    if (!meter) 
    {
        return HI_ERR_FAILURE;
    }

    //表号不能重复
    for (index = 0; index < mmodule->usNum;index++)
    {
        if (mrsToolsMemEq(maddr,mmodule->pstMList[index]->ucMeterAddr,6))
        {
            return HI_ERR_FAILURE;
        }
    }

    //添加电表(不能超过64个)
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
// 函数名称: mrsSrvMeterReply
// 功能描述: 电表回复     
// 参数说明:
//   MRS_MET_MODULE * [IN]  电表模块
//   MRS_MEM_BUF    * [IN]  输入
//   MRS_SRV_LIST   * [IN/OUT]  输出缓存列表(多个电表回复)
//   HI_VOID        * [IN] 块内存申请函数
// 返 回 值: 
// 调用要求: 
// 调用举例: 
// 作    者: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrsSrvMetersReply(MRS_MET_MODULE * mmodule,MRS_MEM_BUF * inbuf,MRS_SRV_LIST *list)
{
    HI_U16 mindex;
    MRS_SIMU_METER *meter;
    HI_U8 *temp;
    HI_U8 * rdata;
    HI_U32 rlen;
    HI_BOOL fond = HI_FALSE;
    
    //参数判断
    if (!mmodule || !inbuf || !inbuf->ucBuf || !list || !simu_mem_alloc) 
    {
        return HI_ERR_FAILURE;
    }

    rdata = mrsToolsStreamFindByte(inbuf->ucBuf,(HI_U16)inbuf->uiLen,0x68);
    if (!rdata) 
    {
        return HI_ERR_FAILURE;
    }
    rlen = (HI_U32)(inbuf->uiLen + (HI_PBYTE)inbuf->ucBuf - (HI_PBYTE)rdata);

    //检查帧
    if (mrsToolsCheck645Frame(rdata,(HI_U16)rlen)) 
    {
        return HI_ERR_FAILURE;
    }

    temp = simu_mem_alloc(rlen);
    if (!temp) 
    {
        return HI_ERR_FAILURE;
    }
    //解帧
    (hi_void)memcpy_s(temp, rlen, rdata, rlen);
    simu_do_decode((HI_U8 *)(temp+10),rdata[9]);

    for (mindex = 0;mindex < mmodule->usNum;mindex++) 
    {
        MRS_MEM_BUF outbuf = {0};
        meter = mmodule->pstMList[mindex];
        if (meter) 
        {
            //检查MAC
            if (!simu_check_mac(rdata + 1,meter->ucMeterAddr))
            {
                continue;
            }

            fond = HI_TRUE;
            
            //如果电表回复成功
            if (simu_do_reply(temp,rlen,meter,&outbuf))
            {
                MRS_MEM_LINK * block = simu_mem_alloc(sizeof(MRS_MEM_LINK));
                if (block) 
                {
                    (hi_void)memset_s(block,sizeof(MRS_MEM_LINK), 0,sizeof(MRS_MEM_LINK));
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
    //先释放数据，再释放结点
    MRS_MET_MODULE * md;
    MRS_MEM_LINK * nd;

    md = (MRS_MET_MODULE *)m;
    nd = mrs_list_entry(lk,MRS_MEM_LINK,link);

    md->pfnFree(nd->buf.ucBuf);
    md->pfnFree(nd);
}

//*****************************************************************************
// 函数名称: mrsStaSend2Meter
// 功能描述: 将数据发送给电表    
// 参数说明:
//   MRS_MET_MODULE       [IN] 模块电表模块
//   HI_PBYTE             [IN] 数据
//   HI_U16               [IN] 数据长度
// 返 回 值: 
//   成功返回  HI_ERR_SUCCESS
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaSend2Meter(MRS_MET_MODULE * md,HI_PBYTE pucData, HI_U16 usDataSize)
{
    MRS_SRV_LIST mlist;
    MRS_SRV_LINK *link;
    MRS_MEM_LINK *outbuf;
    HI_U32 ret = 0;

    MRS_MEM_BUF inbuf = {0};
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[VM-MR]-TX"), pucData, usDataSize); 
    
    //初始化回复列表
    mrsSrvInitList(&mlist);

    //初始化输入
    inbuf.ucBuf = pucData;
    inbuf.uiLen = usDataSize;

    //获取电表回复
    mrsSrvMetersReply(md,&inbuf,&mlist);
    
    link = mlist.next;
    for (link = mlist.next;link != &mlist;link = link->next)
    {
        outbuf = mrs_list_entry(link,MRS_MEM_LINK,link);
        
        // 获取到完整的帧数据
        ret = mrs645ProtoStreamInput(outbuf->buf.ucBuf, (HI_U16)outbuf->buf.uiLen, &stFrame, ID_MRS_UART_645BUF);
        if(HI_ERR_SUCCESS == ret)
        {
            HI_SYS_QUEUE_MSG_S stMsg = { 0 }; 
            MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&stMsg.ulParam[0];

            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[VM-MR]-RX"), stFrame.pucDatagram, stFrame.usDatagramSize); 
            
            (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
            stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

            // 发送消息给 MRS队列, 进行消息分发调度 
            ret = mrsSendMessage2Queue(&stMsg);
            if(ret != HI_ERR_SUCCESS)
            {
                mrsToolsFree(stFrame.pucDatagram);
            }
            
            break;
        }
    }
    
    //释放
    mrsSrvListClear(&mlist,mrsStaListFreeNd,md);
    return ret;
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

// 同步虚拟表地址到Mac层
HI_VOID mrsSyncVMAddrToMac(HI_VOID)
{
    HI_U8 maddr[6] = {0};

    mrsStaConverVmAddr(maddr);
    mrsStaUpdateAddr(maddr, MRS_PROTO645_VER_2007);  //更新表地址
    mrsStaVMAddr2MeterList(maddr, MRS_PROTO645_VER_2007);
    mrsStaUpdateTopAddrInf(maddr, MRS_PROTO645_VER_2007);
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
