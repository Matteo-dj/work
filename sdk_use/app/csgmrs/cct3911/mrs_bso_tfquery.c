/**
 ******************************************************************************
 * @file       mrs_bso_tfquery.c
 * @version   
 * @brief
 * @details 
 *
 * @copyright  
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_mdm_time.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_TFQUERY_NONE = 0,
    BSO_TFQUERY_RUN, 
    BSO_TFQUERY_START, 
    BSO_TFQUERY_STOP, 
} mrs_bso_tfquery_state_e;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static void bsotfquery_start(int fd, mrs_bso_t* mrs_bso)
{
    PACK_MALLOC2;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp = BSO_RESP_OK;
    HI_U8 flag = 0;

    ret = Mrs_Misc_QueryTfStatusEnable();
    if (ret != HI_ERR_SUCCESS)
    {
        resp = BSO_RESP_ERR;
        flag = 1;
    }
    
    load_uint8(ctxt, &clen, resp);
    wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, flag, 
        BSO_TYPE_TFQUERY), BSO_CMD_TFQUERY_START, 
        ctxt, clen, wrbuf, BSO_WRBUF_LEN);
    mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
    
    if (ret == HI_ERR_SUCCESS)
    {
        printf("tf_enable OK\r\n");
    }
    else
    {
        printf("tf_enable Err[%d]\r\n", ret);
    }
    
    mrs_bso_clr(mrs_bso);
}

static void bsotfquery_stop(int fd, mrs_bso_t* mrs_bso)
{
    PACK_MALLOC2;
    HI_U8 resp = BSO_RESP_OK;
    HI_U8 flag = 0;
    HI_U32 ret;

    ret = Mrs_Misc_QueryTfStatusDisable();
    if (ret != HI_ERR_SUCCESS)
    {
        resp = BSO_RESP_ERR;
        flag = 1;
    }
    
    load_uint8(ctxt, &clen, resp);
    wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, flag, 
        BSO_TYPE_TFQUERY), BSO_CMD_TFQUERY_STOP, 
        ctxt, clen, wrbuf, BSO_WRBUF_LEN);
    mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);

    if (ret == HI_ERR_SUCCESS)
    {
        printf("tf_Disable OK\r\n");
    }
    else
    {
        printf("tf_Disable Err[%d]\r\n", ret);
    }

    mrs_bso_clr(mrs_bso);
}

static HI_S32 
mrs_bsotfquery_run(int fd, mrs_bso_t* mrs_bso)
{
    switch (mrs_bso->run_state)
    {
        case BSO_TFQUERY_START:
            bsotfquery_start(fd, mrs_bso);
            break;

        case BSO_TFQUERY_STOP:
            bsotfquery_stop(fd, mrs_bso);
            break;
            
        default:
            break;
    }
    
    return OK;
}

static HI_S32  
mrs_bsotfquery_proc(int fd, HI_U8 *buf, int len, 
    mrs_bso_t* mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];
    
    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);
    
    if (BSO_CMD_TFQUERY_START == cmdId)
    {
        mrs_bso->run_state = BSO_TFQUERY_START;
    }
    else if (BSO_CMD_TFQUERY_STOP == cmdId)
    {
        mrs_bso->run_state = BSO_TFQUERY_STOP;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

mrs_bso_t* 
mrs_bsotfquery_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_TFQUERY;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;
    
    mrs_bso->run = mrs_bsotfquery_run;
    mrs_bso->proc = mrs_bsotfquery_proc;
    
    mrs_bso->run_state = BSO_TFQUERY_RUN;
    mrs_bso->proc_state = BSO_TFQUERY_RUN;

    return mrs_bso;
}

/*-------------------------------End of mrs_bso_tfquery.c--------------------------------*/
#endif