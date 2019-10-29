/**
 ******************************************************************************
 * @file       mrs_bso_reboot.c
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
    BSO_REBOOT_NONE = 0, 
    BSO_REBOOT_RUN, 
    BSO_REBOOT,
} mrs_bso_rbt_state_e;

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
/**
 ******************************************************************************
 * @brief      ����ҵ�����ݷ��ʹ���
 * @param[in]  fd �����
 * @param[in]  mrs_bso ��ҵ�����
 * @retval     STATUS  ��OK-�ɹ�, ERROR-ʧ��
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32  
mrs_bsoreboot_run(int fd, mrs_bso_t *mrs_bso)
{
    switch (mrs_bso->run_state)
    {
        case BSO_REBOOT:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_REBOOT), BSO_CMD_REBOOT, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                // ��λ
                Mrs_Misc_Reboot();
                mrs_bso_clr(mrs_bso);
            }
            break;
            
        default:
            break;
    }
    
    return OK;
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����ݽ��մ���
 * @param[in]  fd �����
 * @param[in]  buf ���������ݻ�����
 * @param[in]  len ���������ݳ���
 * @param[in]  mrs_bso ��ҵ�����
 * @retval     STATUS  ��OK-�ɹ�, ERROR-ʧ��
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsoreboot_proc(int fd, HI_U8 *buf, int len, 
    mrs_bso_t *mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];
    
    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);
    
    if (BSO_CMD_REBOOT == cmdId)
    {
        mrs_bso->run_state = BSO_REBOOT;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����ʵ����
 * @param[in]  mrs_bso_t *mrs_bso  ��ҵ�����
 * @retval     mrs_bso_t*  �����ص�ҵ�����
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
mrs_bso_t* 
mrs_bsoreboot_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_REBOOT;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;

    mrs_bso->run_state = BSO_REBOOT_RUN;
    mrs_bso->proc_state = BSO_REBOOT_RUN;
    
    mrs_bso->run = mrs_bsoreboot_run;
    mrs_bso->proc = mrs_bsoreboot_proc;
    
    return mrs_bso;
}

/*-------------------------------End of mrs_bso_reboot.c--------------------------------*/
#endif