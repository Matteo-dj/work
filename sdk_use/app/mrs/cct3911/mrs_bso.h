/**
 ******************************************************************************
 * @file       mrs_bso.h
 * @version  
 * @brief      业务对象处理列表的操作集合
 * @details    各个业务对象的操作主要包含以下几个
 * 			   1、业务对象的创建
 * 			   2、业务对象的插入
 * 			   3、业务对象的删除
 * 			   4、业务对象的获取
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

/*台区识别命令*/
#define BSO_CMD_TFQUERY_START   0x0001
#define BSO_CMD_TFQUERY_STOP    0x0002

/*采数命令*/
#define BSO_CMD_CLT_REAL_START  0x0001
#define BSO_CMD_CLT_REAL_STOP   0x0002
#define BSO_CMD_CLT_SAVE_START  0x0003
#define BSO_CMD_CLT_SAVE_STOP   0x0004

#define BSO_CMD_CLT_TRANS_RUN   0x0005
#define BSO_CMD_CLT_TRANS_TODO  0x0006
#define BSO_CMD_CLT_TRANS_STATE 0x0007

/*信号发送命令*/
#define BSO_CMD_SG0             0x0001
#define BSO_CMD_SG1             0x0002
#define BSO_CMD_SG2             0x0003
#define BSO_CMD_SG1_STOP        0x0004

/*参数设置命令*/
#define BSO_CMD_PARAM_RD_ALL    0x0001
#define BSO_CMD_PARAM_WR_IP     0x0002
#define BSO_CMD_PARAM_WR_TEI    0x0003
#define BSO_CMD_PARAM_RD_IP     0x0004
#define BSO_CMD_PARAM_RD_TEI    0x0005
#define BSO_CMD_PARAM_RD_IP3911 0x0006

/*复位命令*/
#define BSO_CMD_REBOOT          0x0001

/*上报命令*/
#define BSO_CMD_REPORT_REBOOT   0x0001
#define BSO_CMD_REPORT_TF       0x0002
#define BSO_CMD_REPORT_WRTIME   0x0003
#define BSO_CMD_REPORT_RDTIME   0x0004
#define BSO_CMD_REPORT_BATUPG   0x0005
#define BSO_CMD_REPORT_BATSTAT  0x0006
#define BSO_CMD_REPORT_NTB      0x0007

/*混杂命令*/
#define BSO_CMD_MISC_HEARTBEAT  0x0001
#define BSO_CMD_MISC_TIME       0x0002
#define BSO_CMD_MISC_NTB        0x0003

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/**
 * 业务对象类型
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
    BSO_RESP_OK = 0,            /**< 应答 OK*/
    BSO_RESP_REAL_TRANS,        /**< 实时采数完成 */
    BSO_RESP_SAVE_TRANS,        /**< 存储采数完成 */
    BSO_RESP_SD_NOSPACE,        /**< SD卡空间不够 */
    BSO_RESP_NO_SD,             /**< SD未插*/
    BSO_RESP_TIMEOUT,           /**< 超时 */
    BSO_RESP_ERR,               /**< 错误 */
    BSO_RESP_NONE,              /**< 无应答 */
} mrs_bso_resp_t;

typedef enum
{
    CLT_TYPE_REAL = 0, 
    CLT_TYPE_SAVE, 
    CLT_TYPE_REBOOT,
} mrs_clt_type_e;

typedef struct
{
    HI_U8 clt_type;      /*0:实时1:存储2:断电续采*/
    HI_U8 file_type;
    HI_U8 res[2];
    void* hi_buf;
} mrs_clt_info_t;

typedef struct _mrs_bso_t
{
    HI_U8 chantype;            /**< 通道类型 */
    HI_U8 bsotype;             /**< 业务类型 */
    HI_U8 misc_use;            /**< 对象不同复用功能不同*/
    HI_U8 wcnt;                /*写次数*/
    
    HI_U32 t;
    
    HI_U8 *pbuf;               /*申请内存*/
    void *param;               /*由外部传入*/

    HI_U8 run_state;
    HI_U8 proc_state;
    
    //HI_U8 (*getstate)(struct _mrs_bso_t *mrs_bso);        /**< 获取业务对象的状态 */
    //void (*setstate)(struct _mrs_bso_t *mrs_bso, HI_U8);  /**< 设置业务对象的状态 */
    
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
