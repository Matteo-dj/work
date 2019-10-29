/**
 ******************************************************************************
 * @file       mrs_bso.h
 * @version  
 * @brief      ҵ��������б�Ĳ�������
 * @details    ����ҵ�����Ĳ�����Ҫ�������¼���
 * 			   1��ҵ�����Ĵ���
 * 			   2��ҵ�����Ĳ���
 * 			   3��ҵ������ɾ��
 * 			   4��ҵ�����Ļ�ȡ
 * @copyright 
 ******************************************************************************
 */

#ifndef _MRS_BSO_H_
#define _MRS_BSO_H_

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_types.h"
#include "mrs_misc.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define BSO_MAX_NUM             BSO_TYPE_MAX
#define BSO_MAX_WRCNT           3
#define BSO_WRBUF_LEN           64

#define PACK_MALLOC1        HI_S32 wrlen = 0;  \
                            HI_U8 wrbuf[BSO_WRBUF_LEN] = {0};

#define PACK_MALLOC2        PACK_MALLOC1 \
                            HI_U16 clen = 0; \
                            HI_U8* ctxt = wrbuf + PTL_TXT_OFT_HEAD;

/*̨��ʶ������*/
#define BSO_CMD_TFQUERY_START   0x0001
#define BSO_CMD_TFQUERY_STOP    0x0002

/*��������*/
#define BSO_CMD_CLT_REAL_START  0x0001
#define BSO_CMD_CLT_REAL_STOP   0x0002
#define BSO_CMD_CLT_SAVE_START  0x0003
#define BSO_CMD_CLT_SAVE_STOP   0x0004

#define BSO_CMD_CLT_TRANS_RUN   0x0005
#define BSO_CMD_CLT_TRANS_TODO  0x0006
#define BSO_CMD_CLT_TRANS_STATE 0x0007

/*�źŷ�������*/
#define BSO_CMD_SG0             0x0001
#define BSO_CMD_SG1             0x0002
#define BSO_CMD_SG2             0x0003
#define BSO_CMD_SG1_STOP        0x0004

/*������������*/
#define BSO_CMD_PARAM_RD_ALL    0x0001
#define BSO_CMD_PARAM_WR_IP     0x0002
#define BSO_CMD_PARAM_WR_TEI    0x0003
#define BSO_CMD_PARAM_RD_IP     0x0004
#define BSO_CMD_PARAM_RD_TEI    0x0005
#define BSO_CMD_PARAM_RD_IP3911 0x0006

/*��λ����*/
#define BSO_CMD_REBOOT          0x0001

/*�ϱ�����*/
#define BSO_CMD_REPORT_REBOOT   0x0001
#define BSO_CMD_REPORT_TF       0x0002
#define BSO_CMD_REPORT_WRTIME   0x0003
#define BSO_CMD_REPORT_RDTIME   0x0004
#define BSO_CMD_REPORT_BATUPG   0x0005
#define BSO_CMD_REPORT_BATSTAT  0x0006
#define BSO_CMD_REPORT_NTB      0x0007

/*��������*/
#define BSO_CMD_MISC_HEARTBEAT  0x0001
#define BSO_CMD_MISC_TIME       0x0002
#define BSO_CMD_MISC_NTB        0x0003

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/**
 * ҵ���������
 */
typedef enum
{
    BSO_TYPE_TFQUERY = 0, 
    BSO_TYPE_COLLECT, 
    BSO_TYPE_SG,
    BSO_TYPE_PARAM,
    BSO_TYPE_REBOOT, 
    BSO_TYPE_REPORT, 
    BSO_TYPE_MISC,
    BSO_TYPE_BDCOLLECT, 
    BSO_TYPE_MAX,  
} mrs_bso_type_e;

typedef enum
{
    BSO_RESP_OK = 0,            /**< Ӧ�� OK*/
    BSO_RESP_REAL_TRANS,        /**< ʵʱ������� */
    BSO_RESP_SAVE_TRANS,        /**< �洢������� */
    BSO_RESP_SD_NOSPACE,        /**< SD���ռ䲻�� */
    BSO_RESP_NO_SD,             /**< SDδ��*/
    BSO_RESP_TIMEOUT,           /**< ��ʱ */
    BSO_RESP_ERR,               /**< ���� */
    BSO_RESP_NONE,              /**< ��Ӧ�� */
} mrs_bso_resp_t;

typedef enum
{
    CLT_TYPE_REAL = 0, 
    CLT_TYPE_SAVE, 
    CLT_TYPE_REBOOT,
} mrs_clt_type_e;

typedef struct
{
    HI_U8 clt_type;      /*0:ʵʱ1:�洢2:�ϵ�����*/
    HI_U8 file_type;
    HI_U8 res[2];
    void* hi_buf;
} mrs_clt_info_t;

typedef struct _mrs_bso_t
{
    HI_U8 chantype;            /**< ͨ������ */
    HI_U8 bsotype;             /**< ҵ������ */
    HI_U8 misc_use;            /**< ����ͬ���ù��ܲ�ͬ*/
    HI_U8 wcnt;                /*д����*/
    
    HI_U32 t;
    
    HI_U8 *pbuf;               /*�����ڴ�*/
    void *param;               /*���ⲿ����*/

    HI_U8 run_state;
    HI_U8 proc_state;
    
    //HI_U8 (*getstate)(struct _mrs_bso_t *mrs_bso);        /**< ��ȡҵ������״̬ */
    //void (*setstate)(struct _mrs_bso_t *mrs_bso, HI_U8);  /**< ����ҵ������״̬ */
    
    HI_S32 (*run)(HI_S32 fd, struct _mrs_bso_t *mrs_bso);         
    HI_S32 (*proc)(HI_S32 fd, HI_U8 *buf, HI_S32 len, 
        struct _mrs_bso_t *mrs_bso);
} mrs_bso_t;

typedef mrs_bso_t* (*bso_instance)(mrs_bso_t*);

/*-----------------------------------------------------------------------------
 Section: Globals
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
HI_S32 
mrs_bso_wr(int fd, HI_U8 *buf, int len, 
    HI_U8 wcnt, 
    HI_U32 tiemout, 
    mrs_bso_t *bso);

void mrs_bso_clr(mrs_bso_t *mrs_bso);

mrs_bso_t*
mrs_bso_instance(mrs_bso_type_e type, 
	mrs_bso_t *bos);

HI_S32
mrs_bsolist_ins(mrs_bso_t *mrs_bsolist[], 
	mrs_bso_t* mrs_bso);

void
mrs_bsolist_del(mrs_bso_t *mrs_bsolist[], 
	mrs_bso_t* mrs_bso);

mrs_bso_t*
mrs_bsolist_get(mrs_bso_t *mrs_bsolist[], 
	HI_U8 i);


#endif /* _MRS_BSO_H_ */

/*-------------------------------End of mrs_bso.h--------------------------------*/
