#ifndef __HI_DRIVER_INTERNAL_H__
#define __HI_DRIVER_INTERNAL_H__
#include <hi_types.h>
HI_START_HEADER
#include <hi_driver.h>
#include <nv_interface.h>
//*****************************************************************************
// ��������: HI_HRTIMER_Udelay
//     ��λ: us
//*****************************************************************************
HI_U32 HI_HRTIMER_Udelay(HI_U32 count);

//*****************************************************************************
// ��������: HI_UART_Printf
// ��������: ���Դ��ڣ�UART1����ӡ���
//
// ����˵��:
//           ���մ�ӡ��ʽ���룻
//
// �� �� ֵ:
//           �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE��
//
// ����Ҫ��: ���нӿ�ʵ����Ҫ"\r\n"���ܽ��лس����С�
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/* Only for the debug UART ( UART1 for HI3911 ) */
HI_U32  HI_UART_Printf(HI_CONST HI_CHAR *fmt, ...);



//************************************************************************************************************************
// SPI Interface
//************************************************************************************************************************
/* SPI1 = master device ; SPI2 = slave device*/




//*****************************************************************************
// ��������: HI_SPI_Init
// ��������: ��ʼ��SPI
//
// ����˵��:pi_id��SPI id��[SPI1/SPI2]��
//
// �� �� ֵ: �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE��
// ����Ҫ��:
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Init
** RETURN:
**   HI_SUCCESS��
**   HI_FAILURE��
*/
//*****************************************************************************
// ��������: HI_SPI_Deinit
// ��������: ȥ��ʼ��SPI
//
// ����˵��:pi_id��SPI id��[SPI1/SPI2]��
//
// �� �� ֵ: �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE��
// ����Ҫ��:
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Deinit
** RETURN:
**   HI_SUCCESS��
**   HI_FAILURE��
*/
//*****************************************************************************
// ��������: HI_SPI_Set_Attr
// ��������: ����֡��ʽ�����ʵ���Ϣ
//
// ����˵��:pi_id��SPI id��[SPI1/SPI2]��
//
// �� �� ֵ: HI_ERR_SUCCESS��ʾ�ɹ���
//           HI_ERR_SPI_PARAMETER_WRONG��ʾ�������ô���
// ����Ҫ��:
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Set_Attr
**          set the parameters of SPI
** RETURN:
**   HI_SUCCESS;
**   HI_ERR_SPI_PARAMETER_WRONG;
*/
//*****************************************************************************
// ��������: HI_SPI_Write
// ��������: д����
//
// ����˵��:pi_id��SPI id��[SPI1/SPI2]��
//          *buff��д����buffer���ָ��,
//           count��д���ݸ�����
//           timeout��д���ݳ�ʱֵ����10msΪ��λ��
// �� �� ֵ: HI_ERR_SUCCESS��ʾ�ɹ���
//           HI_ERR_SPI_WRITE_TIMEOUTд���ݳ�ʱ��
//           HI_ERR_SPI_PARAMETER_WRONG��������
// ����Ҫ��:ֻ֧�ְ�˫��ģʽ
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/*
** FUNCTION: HI_SPI_Write
** PARA:
**   buff: buffer to write;
**   count: count to write by the type of buff
**   timeout: time by 10ms;
** RETURN:
**   HI_SUCCESS��
**   HI_ERR_SPI_WRITE_TIMEOUT//д���ݳ�ʱ;
**   HI_ERR_SPI_PARAMETER_WRONG//��������;
*/
//*****************************************************************************
// ��������: HI_SPI_Read
// ��������: ��ȡ����
//
// ����˵��:pi_id��SPI id��[SPI1/SPI2]��
//          *buff��д����buffer���ָ��,
//           count���������ݸ�����
//           timeout�������ݳ�ʱֵ����10msΪ��λ����
// �� �� ֵ: HI_ERR_SUCCESS��ʾ�ɹ���
//           HI_ERR_SPI_WRITE_TIMEOUT�������ݳ�ʱ��
//           HI_ERR_SPI_PARAMETER_WRONG��������
// ����Ҫ��:ֻ֧�ְ�˫��ģʽ
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
/*
** FUNCTION: HI_SPI_Read
** PARA:
**   buff: buffer to read;
**   count: count to read by the type of buff
**   timeout: time by 10ms;
** RETURN:
**   HI_SUCCESS;
**   HI_ERR_SPI_READ_TIMEOUT//�������ݳ�ʱ;
**   HI_ERR_SPI_PARAMETER_WRONG//��������;
*/


//************************************************************************************************************************
// Dmac Interface
//************************************************************************************************************************
typedef enum hidw_dmac_transfer_flow_TYPE_E {
    HI_Dmac_mem2mem_dma    = 0x0, /* mem to mem - DMAC   flow ctlr */
    HI_Dmac_mem2prf_dma    = 0x1, /* mem to prf - DMAC   flow ctlr */
    HI_Dmac_prf2mem_dma    = 0x2, /* prf to mem - DMAC   flow ctlr */
    HI_Dmac_prf2prf_dma    = 0x3, /* prf to prf - DMAC   flow ctlr */
    HI_Dmac_prf2mem_prf    = 0x4, /* prf to mem - periph flow ctlr */
    HI_Dmac_prf2prf_srcprf = 0x5, /* prf to prf - source flow ctlr */
    HI_Dmac_mem2prf_prf    = 0x6, /* mem to prf - periph flow ctlr */
    HI_Dmac_prf2prf_dstprf = 0x7,  /* prf to prf - dest   flow ctlr */
    HI_Dmac_trans_flow_err = -0x1
}HI_DW_DMAC_TRANSFER_FLOW_TYPE_E;
/****d* dmac.data/dw_dmac_transfer_type
 * DESCRIPTION
 *  This data type is used for selecting the transfer type for a
 *  specified DMA channel when using the specified driver API
 *  functions. See the DW_ahb_dmac databook for a detailed
 *  description on these transfer types.
 * SEE ALSO
 *  dw_dmac_setTransferType(), dw_dmac_getTransferType()
 * SOURCE
 */
typedef enum hidw_dmac_transfer_type_TYPE_E {
    HI_Dmac_transfer_row1  = 0x1, /* single block or last multi-block */
                               /*  no write back                   */
    HI_Dmac_transfer_row2  = 0x2, /* multi-block auto-reload DAR      */
                               /*  contiguous SAR no write back    */
    HI_Dmac_transfer_row3  = 0x3, /* multi-block auto reload SAR      */
                               /*  contiguous DAR no write back    */
    HI_Dmac_transfer_row4  = 0x4, /* multi-block auto-reload SAR DAR  */
                               /*  no write back                   */
    HI_Dmac_transfer_row5  = 0x5, /* single block or last multi-block */
                               /*  with write back                 */
    HI_Dmac_transfer_row6  = 0x6, /* multi-block linked list DAR      */
                               /*  contiguous SAR with write back  */
    HI_Dmac_transfer_row7  = 0x7, /* multi-block linked list DAR auto */
                               /*  reload SAR  with write back     */
    HI_Dmac_transfer_row8  = 0x8, /* multi-block linked list SAR      */
                               /*  contiguous DAR with write back  */
    HI_Dmac_transfer_row9  = 0x9, /* multi-block linked list SAR auto */
                               /*  reload DAR with write back      */
    HI_Dmac_transfer_row10 = 0xa, /* multi-block linked list SAR DAR  */
                               /*  with write back                 */
    HI_Dmac_transfer_err = -0x1
}HI_DW_DMAC_TRANSFER_TYPE_E;
/*****/
/****d* dmac.data/dw_dmac_burst_trans_length
 * DESCRIPTION
 *  This data type is used for selecting the burst transfer length
 *  on the source and/or destination of a DMA channel when using the
 *  specified driver API functions. These transfer length values do
 *  not relate to the AMBA HBURST parameter.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit field(s): (x = channel number)
 *    - CTLx.SRC_MSIZE, CTLx.DEST_MSIZE
 * SEE ALSO
 *  dw_dmac_setBurstTransLength(), dw_dmac_getBurstTransLength()
 * SOURCE
 */
typedef enum hidw_dmac_burst_trans_length_TYPE_E {
    HI_Dmac_msize_1   = 0x0,
    HI_Dmac_msize_4   = 0x1,
    HI_Dmac_msize_8   = 0x2,
    HI_Dmac_msize_16  = 0x3,
    HI_Dmac_msize_32  = 0x4,
    HI_Dmac_msize_64  = 0x5,
    HI_Dmac_msize_128 = 0x6,
    HI_Dmac_msize_256 = 0x7,
    HI_Dmac_msize_err = -0x1
}HI_DW_DMAC_BURST_TRANS_LENGTH_TYPE_E;
/*****/
/****d* dmac.data/dw_dmac_address_increment
 * DESCRIPTION
 *  This data type is used for selecting the address increment
 *  type for the source and/or destination on a DMA channel when using
 *  the specified driver API functions.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit-field(s): (x = channel number)
 *    - CTLx.SINC, CTLx.DINC
 * SEE ALSO
 *  dw_dmac_setAddressInc(), dw_dmac_getAddressInc()
 * SOURCE
 */
typedef enum hidw_dmac_address_increment_TYPE_E {
    HI_Dmac_addr_increment = 0x0,
    HI_Dmac_addr_decrement = 0x1,
    HI_Dmac_addr_nochange  = 0x2
}HI_DW_DMAC_ADDRESS_INCREMENT_TYPE_E;
/*****/

/****d* dmac.data/dw_dmac_transfer_width
 * DESCRIPTION
 *  This data type is used for selecting the transfer width for the
 *  source and/or destination on a DMA channel when using the specified
 *  driver API functions. This data type maps directly to the AMBA AHB
 *  HSIZE parameter.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit field(s): (x = channel number)
 *    - CTLx.SRC_TR_WIDTH, CTLx.DST_TR_WIDTH
 * SEE ALSO
 *  dw_dmac_setTransWidth(), dw_dmac_getTransWidth()
 * SOURCE
 */
typedef enum hidw_dmac_transfer_width_TYPE_E {
    HI_Dmac_trans_width_8   = 0x0,
    HI_Dmac_trans_width_16  = 0x1,
    HI_Dmac_trans_width_32  = 0x2,
    HI_Dmac_trans_width_64  = 0x3,
    HI_Dmac_trans_width_128 = 0x4,
    HI_Dmac_trans_width_256 = 0x5,
    HI_Dmac_trans_width_err = -0x1,
}HI_DW_DMAC_TRANSFER_WIDTH_TYPE_E;
/*****/
/****d* dmac.data/dw_dmac_master_number
 * DESCRIPTION
 *  This data type is used to select the master interface number
 *  on the DMA Controller when using the specified driver API
 *  functions.
 * NOTES
 *  This data type relates directly to the following DMA Controller
 *  register(s) / bit-field(s): (x = channel number)
 *    - CTLx.SMS, CTLx.DMS, LLPx.LMS
 * SEE ALSO
 *  dw_dmac_setMstSelect(), dw_dmac_getMstSelect(),
 *  dw_dmac_setListMstSelect(), dw_dmac_getListMstSelect()
 * SOURCE
 */
typedef enum hidw_dmac_master_number_TYPE_E {
    HI_Dmac_master1 = 0x0,
    HI_Dmac_master2 = 0x1,
    HI_Dmac_master3 = 0x2,
    HI_Dmac_master4 = 0x3
}HI_DW_DMAC_MASTER_NUMBER_TYPE_E;


typedef struct hiDmac_Config_Para_TYPE_S
{
    HI_U32  u32ChannelNum;
    HI_U32  pu32Para;
    HI_U32  *pu32SrcAddr;
    HI_U32  *pu32DstAddr;
    HI_DW_DMAC_TRANSFER_FLOW_TYPE_E      enumFlowCtrl;
    HI_U32                               transLen;
    HI_DW_DMAC_TRANSFER_TYPE_E           transType;
    HI_U32                               blockSize;
    HI_U32                               numBlocks;
    HI_DW_DMAC_BURST_TRANS_LENGTH_TYPE_E src_msize;
    HI_DW_DMAC_BURST_TRANS_LENGTH_TYPE_E dst_msize;
    HI_DW_DMAC_ADDRESS_INCREMENT_TYPE_E  sinc;
    HI_DW_DMAC_ADDRESS_INCREMENT_TYPE_E  dinc;
    HI_DW_DMAC_TRANSFER_WIDTH_TYPE_E     src_tr_width;
    HI_DW_DMAC_TRANSFER_WIDTH_TYPE_E     dst_tr_width;
    HI_DW_DMAC_MASTER_NUMBER_TYPE_E      ctl_sms;
    HI_DW_DMAC_MASTER_NUMBER_TYPE_E      ctl_dms;
}HI_DMAC_CONFIG_PARA_TYPE_S;

typedef enum hidmac_task_state_TYPE_E
{
	HI_DMAC_TASK_STATE_FINISH 		=	0x100,
	HI_DMAC_TASK_STATE_UNFINISH 	=	0x101
}HI_DMAC_TASK_STATE_TYPE_E;

typedef enum hidmac_task_trans_result_TYPE_E
{
	HI_DMAC_TASK_TRANS_SUCCESS 	=	0x200,
	HI_DMAC_TASK_TRANS_FAIL		=	0x201
}HI_DMAC_TASK_TRANS_RESULT_TYPE_E;

typedef HI_VOID (*dmac_callback)(HI_VOID *dev, HI_VOID* args, HI_S32 err);

#if 1 /*todo liteosͷ�ļ���Ӧ���Ⱪ¶������Ӧ����һ���м�Ŀ¼����һ������*/
typedef enum
{
    HI_TASK_SCHEDULE_TYPE_INT,
    HI_TASK_SCHEDULE_TYPE_CREATE_TASK,
    HI_TASK_SCHEDULE_TYPE_SHEDULE_TASK,
    HI_TASK_SCHEDULE_TYPE_DELETE_TASK,
}HI_TASK_SCHEDULE_TYPE_E;

typedef HI_VOID (*TASK_SCHEDULE_FUNC)(HI_TASK_SCHEDULE_TYPE_E enType, HI_U32 ulIdx);
#endif

//*****************************************************************************
// ��������: HI_DMAC_Transmit
// ��������: ���ݴ���
//
// ����˵��:
// �� �� ֵ: ʧ�� HI_ERR_FAILURE
//           �ɹ� HI_ERR_SUCCESS
// ����Ҫ��: ����HI_DMAC_Is_Finished����ص�������
//           ����dmac_callback func, HI_VOID *pCallbackArgs������ΪNULL
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32  HI_DMAC_Transmit(HI_DMAC_CONFIG_PARA_TYPE_S *ptrDmacConfigPara,
                        dmac_callback func, HI_VOID *pCallbackArgs, HI_VOID **handle);

//*****************************************************************************
// ��������: HI_DMAC_Is_Finished
// ��������: DMA�����Ƿ�����ж�
//
// ����˵��:
// �� �� ֵ: ʧ�� HI_ERR_DMAC_INVALID_PARA
//           �ɹ� HI_ERR_SUCCESS
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32 HI_DMAC_Is_Finished(HI_VOID* handle, HI_DMAC_TASK_STATE_TYPE_E *state,
	HI_DMAC_TASK_TRANS_RESULT_TYPE_E  *err);
//*****************************************************************************
// ��������: HI_DMAC_Free_Handle
// ��������: ����DMAʵ��
//
// ����˵��:
// �� �� ֵ: ʧ�� HI_ERR_DMAC_INVALID_PARA
//                HI_ERR_FAILURE
//           �ɹ� HI_ERR_SUCCESS
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32 HI_DMAC_Free_Handle(HI_VOID *handle);
//*****************************************************************************
// ��������: HI_DMAC_Init
// ��������: ��ʼ��dmac
//
// ����˵��:
// �� �� ֵ: ʧ�� HI_ERR_FAILURE
//           �ɹ� HI_ERR_SUCCESS
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32 HI_DMAC_Init(HI_VOID);
//*****************************************************************************
// ��������: HI_DMAC_Exit
// ��������: ȥ��ʼ��dmac
//
// ����˵��:
// �� �� ֵ:
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_VOID  HI_DMAC_Exit(HI_VOID);


enum hi_wdg_mode {
    WDG_NORMAL = 0,  //��ͨģʽ��ʱ�䵽����ֱ������
    WDG_INTER = 1,  //�ж�ģʽ��ʱ�䵽��ʱ������жϣ����û��������á�
    WDG_ERR,
};
typedef struct hiWdg_TYPE_S
{
       HI_U32 ulOverTime;
	enum hi_wdg_mode    ulMode;
	HI_VOID (*func)(HI_VOID);

}HI_WDG_TYPE_S;


//*****************************************************************************
// ��������: HI_COM_Sys_Reset
// ��������: ϵͳȫ����λ
// ����˵��: ��
// �� �� ֵ: ��
// ����Ҫ��:��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_VOID HI_COM_Sys_Reset(HI_VOID);



//*****************************************************************************
// ��������: HI_TIME_CNT_Enable
// ��������: time_cnt ʹ�ܼ�ʱ���ܣ���ʼ��ʱ
// ����˵��: �� 
//
// �� �� ֵ: ��
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_VOID HI_TIME_CNT_Enable(HI_VOID);

//*****************************************************************************
// ��������: HI_TIME_CNT_Disable
// ��������: time_cnt �رռ�ʱ���ܣ��رպ�ʱ������
// ����˵��: �� 
//
// �� �� ֵ: ��
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_VOID HI_TIME_CNT_Disable(HI_VOID);

//*****************************************************************************
// ��������: HI_TIME_CNT_GetTime
// ��������: ��ȡ��ǰʱ��,�����롢���롢΢�뵥λ
// ����˵��:  
// 			*pulUs ΢��
// 			*pulMs ����
// 			*pulSec  ��
// �� �� ֵ: 
//          HI_ERR_SUCCESS            �ɹ�
//          ����ֵΪ������,�μ�hi_errno.h
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetTime(HI_U32 *pulUs, HI_U32 *pulMs, HI_U32 *pulSec);

//*****************************************************************************
// ��������: HI_TIME_CNT_GetSec
// ��������: ��ȡ��ǰʱ��,�������뵥λ
// ����˵��: 
// 			*pulSec  ��
// �� �� ֵ: 
//          HI_ERR_SUCCESS            �ɹ�
//          ����ֵΪ������,�μ�hi_errno.h
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetSec(HI_U32 *pulSec);

//*****************************************************************************
// ��������: HI_TIME_CNT_GetMs
// ��������: ��ȡ��ǰʱ��,���������뵥λ
// ����˵��: 
// 			*pulMs ����
// �� �� ֵ: 
//          HI_ERR_SUCCESS            �ɹ�
//          ����ֵΪ������,�μ�hi_errno.h
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetMs(HI_U32 *pulMs);

//*****************************************************************************
// ��������: HI_TIME_CNT_GetUs
// ��������: ��ȡ��ǰʱ��,������΢�뵥λ
// ����˵��: 
// 			*pulUs ΢��
// �� �� ֵ: 
//          HI_ERR_SUCCESS            �ɹ�
//          ����ֵΪ������,�μ�hi_errno.h
//
// ��    ��: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetUs(HI_U32 *pulUs);

#ifdef PRODUCT_CFG_SUPPORT_UPG
#define CHK_VALID_FLASH_WRITE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_NV_FLASH_ADDR)     { return HI_ERR_ACCESS_DENIED; }
#else
#define CHK_VALID_FLASH_WRITE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_FLASH_BLOCK_SIZE)  { return HI_ERR_ACCESS_DENIED; }
#endif

#ifdef PRODUCT_CFG_SUPPORT_UPG
 #define CHK_VALID_FLASH_ERASE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_NV_FLASH_ADDR)     { return HI_ERR_ACCESS_DENIED; }
#else
#define CHK_VALID_FLASH_ERASE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_FLASH_BLOCK_SIZE)  { return HI_ERR_ACCESS_DENIED; }
#endif

#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))

//*****************************************************************************
// ��������: HwCheckInit
// ��������: Ӳ������ʼ��
// ����˵��: ��
//
//
// �� �� ֵ: ��
//
// ��    ��: zhangna/z00193780 [2016-02-23]
//*****************************************************************************
HI_EXTERN HI_VOID HwCheckInit(HI_VOID);

//*****************************************************************************
// ��������: PowerOnHwCheck
// ��������: ��������ǰӲ�����
// ����˵��: ��
//
//
// �� �� ֵ: ��
//
// ��    ��: zhangna/z00193780 [2016-02-23]
//*****************************************************************************
HI_EXTERN HI_VOID PowerOnHwCheck(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_FpgaInit(HI_U32 address,HI_U32 size);
#endif
HI_EXTERN HI_CHAR *spif_get_name(HI_VOID);

//*****************************************************************************
// ��������: HI_MDM_EVENT_Init
// ��������: �¼���ʼ��
//
// ����˵��:
//      ulMaxEventCount: �¼��������ֵ
//
// �� �� ֵ:
//      HI_ERR_SUCCESS :��������Ϣ�ɹ����͵���ά�ɲ�����
//      ����Ϊ������,���hi_errno.h
//
// ����Ҫ��:
//          1) ��ϵͳ��ʼ��֮ǰ���ܵ��ö�д�¼��ӿڡ�������ã���ϵͳ���л᲻������
//          2) ���ж��У����Զ��¼��������д�����������ܶ�������
//          3) �����������״̬�£���ֹ������������¼���
//
// ���þ���: TODO: ...
//*****************************************************************************
HI_EXTERN HI_U32 HI_MDM_EVENT_Init(HI_VOID);

HI_EXTERN HI_U32 HI_MDM_TASK_RegisterSchedulingTrack(TASK_SCHEDULE_FUNC pfnHook);

HI_EXTERN HI_U32 HI_MDM_SelfRepair_Init(HI_VOID);


/**
* @ingroup  hct_mdm
* @brief  ���ս���ģ��ת������̫�����ݺ���ָ�붨��
*
* @par   ����:
             ���ս���ģ��ת������̫�����ݺ���ָ�붨��
* @attention
* @param  [IN] ���� HI_U32��
* @param  [IN] ���� HI_U32��
* @retval   [OUT] ���� HI_U32��
* @par Dependency:
* <ul><li>hi_mdm.h: �ļ�����������·�����ӿ�.</li></ul>
* @see EQUIP_ZERO_NTB_NUM ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_SWITCH_RX_F)(HI_U8 *, HI_U16);

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

//*****************************************************************************
// ��������: HI_MDM_SocketSend
// ��������: �����������ӵ����ݰ�����ʽ�׽ӿڷ������ݡ�
//      �������ݱ����׽ӿڣ�����ע�ⷢ�����ݳ��Ȳ�
//      Ӧ����ͨѶ������IP����󳤶�
//
// ����˵��:
//      s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//      buf [in] : �������������ݵĻ�����
//      len [in] : �����������ݵĳ���
//
// �� �� ֵ:
//      ���޴�����,�������������ݵ�������
//      ����Ļ�������ERROR(-1)����
//  ˵��:
//      ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// ��������: HI_MDM_SocketRecv
// ��������: ���������������ӵ����ݱ�����ʽ�׽ӿڽ������ݵĽ��ա�
//
// ����˵��:
//      s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//      buf [out] : ���ڽ������ݵĻ�����
//      len [in] : �������ĳ���
//
// �� �� ֵ:
//          ���޴�������HI_MDM_SocketRecv()���ض�����ֽ�����
//          �����������ֹ������0������������󣬷���-1
//  ˵��:
//       ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketRecv (HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// ��������: HI_MDM_SocketSendTo
// ��������: ������������ָ����socket�����Է�����
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [in] : ���ڷ������ݵĻ�����
//   len [in] : �������ĳ���
//   pucIp [in]: ָ��Ŀ���׽��ֵĵ�ַ��"192.168.1.111"
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//          �ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ��أ�1
//  ˵��:
//         ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_IN HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// ��������: HI_MDM_SocketRecvFrom
// ��������:��������Զ��������ָ����socket����������
//          �������ݴ����ɲ���bufָ����ڴ�ռ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [in] : ���ڽ������ݵĻ�����
//   len [in] : �������ĳ���
//
// �� �� ֵ:
//          �ɹ��򷵻ؽ��յ����ַ���,ʧ�ܷ���-1
//  ˵��:
//          ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// ��������: HI_MDM_SocketConnect
// ��������:���������ڴ�����ָ���ⲿ�˿ڵ�����
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   pucIp [in] : Ŀ��IP��ַ��"192.168.1.1"
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//          �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// ��������: HI_MDM_SocketShutdown
// ��������:�����κ����͵��׽ӿڽ�ֹ���ա���ֹ���ͻ��ֹ�շ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   how����־������������ֹ��Щ������
//              how�ķ�ʽ�����ֱַ���
//              SHUT_RD��0�����ر�sockfd�ϵĶ����ܣ���ѡ�������sockfd���ж�������
//              SHUT_WR��1�����ر�sockfd��д���ܣ���ѡ�������sockfd����д������
//              SHUT_RDWR��2�����ر�sockfd�Ķ�д���ܡ�
// �� �� ֵ:
//           �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketShutdown(HI_SOCKET s, HI_S32 how);

//*****************************************************************************
// ��������: HI_MDM_SocketClose
// ��������:�������ر�һ���׽ӿ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//
// �� �� ֵ:
//          �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketClose(HI_SOCKET s);

//*****************************************************************************
// ��������: HI_SYS_SocketAccept
// ��������:��һ���׽ӿڽ���һ������
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//
// �� �� ֵ:
//           �ɹ�����һ���µ��׽�����������ʧ�ܷ���-1��
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketAccept (HI_SOCKET s);

//*****************************************************************************
// ��������: HI_MDM_SocketTcpCreate
// ��������: ����TCP socket��
//      Ĭ�ϻ��������¼�������,��ͨ��onebuiler���޸�����:
//      ����BUFFER: PRODUCT_CFG_TCP_TX_BUFFER
//      ����BUFFER: PRODUCT_CFG_TCP_RCV_BUFFER
//      KEEPALIVE����:
//          PRODUCT_CFG_TCP_KEEPINTVL  #���÷���keepalive�ļ��ʱ�䣨s��
//          PRODUCT_CFG_TCP_KEEPALIVE_R2 #���ò�ͨ����ʼ����keepalive��ʱ�䣨s��
//          PRODUCT_CFG_KEEPALIVE_WAIT  #����keepalive̽�����(��)
//    #���÷���keepalive�ļ��ʱ�䣨s��
// ����˵��:��
//
// �� �� ֵ: ��ȷ:SOCKET_ID.
//           ʧ��:0xFFFFFFFF
//
// ����Ҫ��: �����е���, �����������ж�
// ��    ��: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketTcpCreate(HI_VOID);

//*****************************************************************************
// ��������: HI_MDM_SocketUdpCreate
// ��������: ����UDP socket
//
// ����˵��:��
//
// �� �� ֵ: ��ȷ:SOCKET_ID.
//           ʧ��:0xFFFFFFFF
//
// ����Ҫ��: �����е���, �����������ж�
// ��    ��: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketUdpCreate(HI_VOID);

HI_EAPI HI_U32 HI_MDM_SocketSetLinger(HI_SOCKET s);

//*****************************************************************************
// ��������: HI_MDM_SocketBind
// ��������:��һ���ص�ַ��һ�׽ӿ�����
//                       ������������δ���ӵ����ݱ��������׽ӿ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   pucIp [in] : Ŀ��IP��ַ��"192.168.1.1"
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//                   �ɹ��򷵻�HI_ERR_SUCCESS��ʧ�ܷ���HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// ��������: HI_MDM_SocketListen
// ��������:����һ���׽ӿڲ��������������.
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   backlog [in] : �ȴ����Ӷ��е���󳤶�
//
// �� �� ֵ:
//                   �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketListen(HI_SOCKET s, HI_S16 backlog);

//*****************************************************************************
// ��������: HI_MDM_SocketSetFd
// ��������:����Socketλͼ�е�λ��
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   fd [in] : socket λ������ָ��
//
// �� �� ֵ:
//                   ��
//*****************************************************************************
HI_EAPI HI_VOID HI_MDM_SocketSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
HI_EAPI HI_BOOL HI_MDM_SocketIsSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);

//*****************************************************************************
// ��������: HI_MDM_SocketSelect
// ��������:ȷ��һ�������׽ӿڵ�״̬
//
// ����˵��:
//   nfds [in] : ��һ������ֵ����ָ�����������ļ��������ķ�Χ
//   readfds [in] : ָ��һ��ȴ��ɶ��Լ����׽ӿ�
//   writefds [in] : ָ��һ��ȴ���д�Լ����׽ӿ�
//   ulTimeOut [in] : ���ȴ�ʱ��
// �� �� ֵ:
//                   �ɹ�����HI_ERR_SUCCESS
//                   ��ʱ����HI_ERR_TIMEOUT
//                   ���󷵻�HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_U32 HI_MDM_SocketSelect(HI_S32 nfds, HI_OUT HI_SYS_SOCKET_FD* readfds, HI_OUT HI_SYS_SOCKET_FD*writefds, HI_U32 ulTimeOut);
HI_EAPI HI_U32 HI_MDM_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt);
HI_EAPI HI_U32 HI_MDM_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_PVOID optval,  HI_U32 optlen);


//*****************************************************************************
// ��������: HI_MDM_DfxSetVerInfo
// ��������: ����APPģ��汾�źͱ���ʱ�䡣
//
// ����˵��:
//      enVerType [in]:ģ��ID,��֧��HI_DFX_APP_VER
//      szVer [in]:�汾��
//      szBuiltDataTime [in]:����ʱ��
//
// �� �� ֵ:
//      HI_ERR_SUCCESS:ִ�гɹ�
//      ����ֵ����
//
// ����Ҫ��:�û�֮����APP��.c�ļ��е��ú�HI_MDM_CONFIG_APP_VER ����
// ���þ���:HI_MDM_CONFIG_APP_VER(VERSION_MAIN,VERSION_FEATURE,VERSION_BUG)
//*****************************************************************************
HI_EAPI HI_U32 HI_MDM_DfxSetVerInfo(HI_SYS_DFX_VER_E enVerType, HI_CONST HI_CHAR szVer[12], HI_CONST HI_CHAR szBuiltDataTime[20]);


#endif

HI_EAPI HI_U32 HI_MDM_INT_LockTmp(HI_U32 lr);
HI_EAPI HI_VOID HI_MDM_INT_RestoreTmp(HI_U32 lr,HI_U32 ulIntSave);

HI_EAPI HI_U32 HI_MDM_MEM_Init(HI_U32 ulPoolCount);
HI_EAPI HI_U32 HI_MDM_MEM_InitDynStaticPool(HI_U32 ulDynPoolSize,HI_U32 ulStaticPoolSize);

HI_END_HEADER
#endif
