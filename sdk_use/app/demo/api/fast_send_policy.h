#ifndef __FAST_SEND_POLICY_H__
#define __FAST_SEND_POLICY_H__


#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    HI_U16 odtei;
    HI_U16 pkt_index;
    HI_U8  send_data[4];
}DEMO_FASTA_SEND_DATA_S;

typedef struct
{
    HI_U16 odtei;
    HI_U16 pkt_index;

    HI_U8 data_len;
    HI_U8 fliter_index;
    HI_U8 odmac[6];

    HI_U16 rcv_pkt_index[32];

    DEMO_FASTA_SEND_DATA_S data;
    HI_U32 send_cnt;
}DEMO_FAST_SEND_POLICY_CB_S;


typedef struct
{
    HI_U32 fast_type;
}MEMO_FAST_SEND_BB_HEAD_S;

#ifdef __cplusplus
}
#endif


#endif


