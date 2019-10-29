#ifndef __HI_DRIVER_INTERNAL_H__
#define __HI_DRIVER_INTERNAL_H__
#include <hi_types.h>
HI_START_HEADER
#include <hi_driver.h>
#include <nv_interface.h>
//*****************************************************************************
// 函数名称: HI_HRTIMER_Udelay
//     单位: us
//*****************************************************************************
HI_U32 HI_HRTIMER_Udelay(HI_U32 count);

//*****************************************************************************
// 函数名称: HI_UART_Printf
// 功能描述: 调试串口（UART1）打印输出
//
// 参数说明:
//           按照打印格式输入；
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求: 现有接口实现需要"\r\n"才能进行回车换行。
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/* Only for the debug UART ( UART1 for HI3911 ) */
HI_U32  HI_UART_Printf(HI_CONST HI_CHAR *fmt, ...);



//************************************************************************************************************************
// SPI Interface
//************************************************************************************************************************
/* SPI1 = master device ; SPI2 = slave device*/




//*****************************************************************************
// 函数名称: HI_SPI_Init
// 功能描述: 初始化SPI
//
// 参数说明:pi_id：SPI id号[SPI1/SPI2]；
//
// 返 回 值: 成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Init
** RETURN:
**   HI_SUCCESS；
**   HI_FAILURE；
*/
//*****************************************************************************
// 函数名称: HI_SPI_Deinit
// 功能描述: 去初始化SPI
//
// 参数说明:pi_id：SPI id号[SPI1/SPI2]；
//
// 返 回 值: 成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Deinit
** RETURN:
**   HI_SUCCESS；
**   HI_FAILURE；
*/
//*****************************************************************************
// 函数名称: HI_SPI_Set_Attr
// 功能描述: 设置帧格式，速率等信息
//
// 参数说明:pi_id：SPI id号[SPI1/SPI2]；
//
// 返 回 值: HI_ERR_SUCCESS表示成功；
//           HI_ERR_SPI_PARAMETER_WRONG表示参数设置错误；
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/*
** FUCTION: HI_SPI_Set_Attr
**          set the parameters of SPI
** RETURN:
**   HI_SUCCESS;
**   HI_ERR_SPI_PARAMETER_WRONG;
*/
//*****************************************************************************
// 函数名称: HI_SPI_Write
// 功能描述: 写数据
//
// 参数说明:pi_id：SPI id号[SPI1/SPI2]；
//          *buff：写数据buffer存放指针,
//           count：写数据个数；
//           timeout：写数据超时值（以10ms为单位）
// 返 回 值: HI_ERR_SUCCESS表示成功；
//           HI_ERR_SPI_WRITE_TIMEOUT写数据超时；
//           HI_ERR_SPI_PARAMETER_WRONG参数错误；
// 调用要求:只支持半双工模式
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/*
** FUNCTION: HI_SPI_Write
** PARA:
**   buff: buffer to write;
**   count: count to write by the type of buff
**   timeout: time by 10ms;
** RETURN:
**   HI_SUCCESS；
**   HI_ERR_SPI_WRITE_TIMEOUT//写数据超时;
**   HI_ERR_SPI_PARAMETER_WRONG//参数错误;
*/
//*****************************************************************************
// 函数名称: HI_SPI_Read
// 功能描述: 读取数据
//
// 参数说明:pi_id：SPI id号[SPI1/SPI2]；
//          *buff：写数据buffer存放指针,
//           count：读回数据个数；
//           timeout：读数据超时值（以10ms为单位）；
// 返 回 值: HI_ERR_SUCCESS表示成功；
//           HI_ERR_SPI_WRITE_TIMEOUT读回数据超时；
//           HI_ERR_SPI_PARAMETER_WRONG参数错误；
// 调用要求:只支持半双工模式
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
/*
** FUNCTION: HI_SPI_Read
** PARA:
**   buff: buffer to read;
**   count: count to read by the type of buff
**   timeout: time by 10ms;
** RETURN:
**   HI_SUCCESS;
**   HI_ERR_SPI_READ_TIMEOUT//读回数据超时;
**   HI_ERR_SPI_PARAMETER_WRONG//参数错误;
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

#if 1 /*todo liteos头文件不应对外暴露，后续应定义一个中间目录，进一步整改*/
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
// 函数名称: HI_DMAC_Transmit
// 功能描述: 数据传输
//
// 参数说明:
// 返 回 值: 失败 HI_ERR_FAILURE
//           成功 HI_ERR_SUCCESS
// 调用要求: 可用HI_DMAC_Is_Finished代替回调函数，
//           即令dmac_callback func, HI_VOID *pCallbackArgs两参数为NULL
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32  HI_DMAC_Transmit(HI_DMAC_CONFIG_PARA_TYPE_S *ptrDmacConfigPara,
                        dmac_callback func, HI_VOID *pCallbackArgs, HI_VOID **handle);

//*****************************************************************************
// 函数名称: HI_DMAC_Is_Finished
// 功能描述: DMA传输是否完成判断
//
// 参数说明:
// 返 回 值: 失败 HI_ERR_DMAC_INVALID_PARA
//           成功 HI_ERR_SUCCESS
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_DMAC_Is_Finished(HI_VOID* handle, HI_DMAC_TASK_STATE_TYPE_E *state,
	HI_DMAC_TASK_TRANS_RESULT_TYPE_E  *err);
//*****************************************************************************
// 函数名称: HI_DMAC_Free_Handle
// 功能描述: 销毁DMA实例
//
// 参数说明:
// 返 回 值: 失败 HI_ERR_DMAC_INVALID_PARA
//                HI_ERR_FAILURE
//           成功 HI_ERR_SUCCESS
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_DMAC_Free_Handle(HI_VOID *handle);
//*****************************************************************************
// 函数名称: HI_DMAC_Init
// 功能描述: 初始化dmac
//
// 参数说明:
// 返 回 值: 失败 HI_ERR_FAILURE
//           成功 HI_ERR_SUCCESS
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_DMAC_Init(HI_VOID);
//*****************************************************************************
// 函数名称: HI_DMAC_Exit
// 功能描述: 去初始化dmac
//
// 参数说明:
// 返 回 值:
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID  HI_DMAC_Exit(HI_VOID);


enum hi_wdg_mode {
    WDG_NORMAL = 0,  //普通模式，时间到期则直接重启
    WDG_INTER = 1,  //中断模式，时间到期时会产生中断，由用户重新设置。
    WDG_ERR,
};
typedef struct hiWdg_TYPE_S
{
       HI_U32 ulOverTime;
	enum hi_wdg_mode    ulMode;
	HI_VOID (*func)(HI_VOID);

}HI_WDG_TYPE_S;


//*****************************************************************************
// 函数名称: HI_COM_Sys_Reset
// 功能描述: 系统全局软复位
// 参数说明: 无
// 返 回 值: 无
// 调用要求:无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_COM_Sys_Reset(HI_VOID);



//*****************************************************************************
// 函数名称: HI_TIME_CNT_Enable
// 功能描述: time_cnt 使能计时功能，开始计时
// 参数说明: 无 
//
// 返 回 值: 无
//
// 作    者: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_VOID HI_TIME_CNT_Enable(HI_VOID);

//*****************************************************************************
// 函数名称: HI_TIME_CNT_Disable
// 功能描述: time_cnt 关闭计时功能，关闭后，时间清零
// 参数说明: 无 
//
// 返 回 值: 无
//
// 作    者: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_VOID HI_TIME_CNT_Disable(HI_VOID);

//*****************************************************************************
// 函数名称: HI_TIME_CNT_GetTime
// 功能描述: 读取当前时间,包含秒、毫秒、微秒单位
// 参数说明:  
// 			*pulUs 微秒
// 			*pulMs 毫秒
// 			*pulSec  秒
// 返 回 值: 
//          HI_ERR_SUCCESS            成功
//          其它值为错误码,参见hi_errno.h
//
// 作    者: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetTime(HI_U32 *pulUs, HI_U32 *pulMs, HI_U32 *pulSec);

//*****************************************************************************
// 函数名称: HI_TIME_CNT_GetSec
// 功能描述: 读取当前时间,仅包含秒单位
// 参数说明: 
// 			*pulSec  秒
// 返 回 值: 
//          HI_ERR_SUCCESS            成功
//          其它值为错误码,参见hi_errno.h
//
// 作    者: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetSec(HI_U32 *pulSec);

//*****************************************************************************
// 函数名称: HI_TIME_CNT_GetMs
// 功能描述: 读取当前时间,仅包含毫秒单位
// 参数说明: 
// 			*pulMs 毫秒
// 返 回 值: 
//          HI_ERR_SUCCESS            成功
//          其它值为错误码,参见hi_errno.h
//
// 作    者: zhangna/z00193780 [2017-03-2]
//*****************************************************************************
HI_EXTERN HI_U32 HI_TIME_CNT_GetMs(HI_U32 *pulMs);

//*****************************************************************************
// 函数名称: HI_TIME_CNT_GetUs
// 功能描述: 读取当前时间,仅包含微秒单位
// 参数说明: 
// 			*pulUs 微秒
// 返 回 值: 
//          HI_ERR_SUCCESS            成功
//          其它值为错误码,参见hi_errno.h
//
// 作    者: zhangna/z00193780 [2017-03-2]
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
// 函数名称: HwCheckInit
// 功能描述: 硬件检测初始化
// 参数说明: 无
//
//
// 返 回 值: 无
//
// 作    者: zhangna/z00193780 [2016-02-23]
//*****************************************************************************
HI_EXTERN HI_VOID HwCheckInit(HI_VOID);

//*****************************************************************************
// 函数名称: PowerOnHwCheck
// 功能描述: 任务启动前硬件检测
// 参数说明: 无
//
//
// 返 回 值: 无
//
// 作    者: zhangna/z00193780 [2016-02-23]
//*****************************************************************************
HI_EXTERN HI_VOID PowerOnHwCheck(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_FpgaInit(HI_U32 address,HI_U32 size);
#endif
HI_EXTERN HI_CHAR *spif_get_name(HI_VOID);

//*****************************************************************************
// 函数名称: HI_MDM_EVENT_Init
// 功能描述: 事件初始化
//
// 参数说明:
//      ulMaxEventCount: 事件个数最大值
//
// 返 回 值:
//      HI_ERR_SUCCESS :仅代表消息成功发送到可维可测任务
//      其它为错误码,详见hi_errno.h
//
// 调用要求:
//          1) 在系统初始化之前不能调用读写事件接口。如果调用，则系统运行会不正常。
//          2) 在中断中，可以对事件对象进行写操作，但不能读操作。
//          3) 在锁任务调度状态下，禁止任务阻塞与读事件。
//
// 调用举例: TODO: ...
//*****************************************************************************
HI_EXTERN HI_U32 HI_MDM_EVENT_Init(HI_VOID);

HI_EXTERN HI_U32 HI_MDM_TASK_RegisterSchedulingTrack(TASK_SCHEDULE_FUNC pfnHook);

HI_EXTERN HI_U32 HI_MDM_SelfRepair_Init(HI_VOID);


/**
* @ingroup  hct_mdm
* @brief  接收交换模块转发的以太网数据函数指针定义
*
* @par   描述:
             接收交换模块转发的以太网数据函数指针定义
* @attention
* @param  [IN] 类型 HI_U32。
* @param  [IN] 类型 HI_U32。
* @retval   [OUT] 类型 HI_U32。
* @par Dependency:
* <ul><li>hi_mdm.h: 文件用于描述链路层对外接口.</li></ul>
* @see EQUIP_ZERO_NTB_NUM 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_SWITCH_RX_F)(HI_U8 *, HI_U16);

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

//*****************************************************************************
// 函数名称: HI_MDM_SocketSend
// 功能描述: 适用于已连接的数据包或流式套接口发送数据。
//      对于数据报类套接口，必需注意发送数据长度不
//      应超过通讯子网的IP包最大长度
//
// 参数说明:
//      s [in] : 一个用于标识已连接套接口的描述字
//      buf [in] : 包含待发送数据的缓冲区
//      len [in] : 缓冲区中数据的长度
//
// 返 回 值:
//      若无错误发生,返回所发送数据的总数。
//      否则的话，返回ERROR(-1)错误
//  说明:
//      本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// 函数名称: HI_MDM_SocketRecv
// 功能描述: 本函数用于已连接的数据报或流式套接口进行数据的接收。
//
// 参数说明:
//      s [in] : 一个用于标识已连接套接口的描述字
//      buf [out] : 用于接收数据的缓冲区
//      len [in] : 缓冲区的长度
//
// 返 回 值:
//          若无错误发生，HI_MDM_SocketRecv()返回读入的字节数。
//          如果连接已中止，返回0。如果发生错误，返回-1
//  说明:
//       本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketRecv (HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// 函数名称: HI_MDM_SocketSendTo
// 功能描述: 用来将数据由指定的socket传给对方主机
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [in] : 用于发送数据的缓冲区
//   len [in] : 缓冲区的长度
//   pucIp [in]: 指向目的套接字的地址如"192.168.1.111"
//   usPortNum [in] : 端口号
//
// 返 回 值:
//          成功则返回实际传送出去的字符数，失败返回－1
//  说明:
//         本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_IN HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// 函数名称: HI_MDM_SocketRecvFrom
// 功能描述:用来接收远程主机经指定的socket传来的数据
//          并把数据传到由参数buf指向的内存空间
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [in] : 用于接收数据的缓冲区
//   len [in] : 缓冲区的长度
//
// 返 回 值:
//          成功则返回接收到的字符数,失败返回-1
//  说明:
//          本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);

//*****************************************************************************
// 函数名称: HI_MDM_SocketConnect
// 功能描述:本函数用于创建与指定外部端口的连接
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   pucIp [in] : 目的IP地址如"192.168.1.1"
//   usPortNum [in] : 端口号
//
// 返 回 值:
//          成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// 函数名称: HI_MDM_SocketShutdown
// 功能描述:用于任何类型的套接口禁止接收、禁止发送或禁止收发
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   how：标志，用于描述禁止哪些操作。
//              how的方式有三种分别是
//              SHUT_RD（0）：关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。
//              SHUT_WR（1）：关闭sockfd的写功能，此选项将不允许sockfd进行写操作。
//              SHUT_RDWR（2）：关闭sockfd的读写功能。
// 返 回 值:
//           成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketShutdown(HI_SOCKET s, HI_S32 how);

//*****************************************************************************
// 函数名称: HI_MDM_SocketClose
// 功能描述:本函数关闭一个套接口
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//
// 返 回 值:
//          成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketClose(HI_SOCKET s);

//*****************************************************************************
// 函数名称: HI_SYS_SocketAccept
// 功能描述:在一个套接口接受一个连接
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//
// 返 回 值:
//           成功返回一个新的套接字描述符，失败返回-1。
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketAccept (HI_SOCKET s);

//*****************************************************************************
// 函数名称: HI_MDM_SocketTcpCreate
// 功能描述: 创建TCP socket。
//      默认会设置以下几个参数,可通过onebuiler来修改配置:
//      发送BUFFER: PRODUCT_CFG_TCP_TX_BUFFER
//      接收BUFFER: PRODUCT_CFG_TCP_RCV_BUFFER
//      KEEPALIVE配置:
//          PRODUCT_CFG_TCP_KEEPINTVL  #设置发送keepalive的间隔时间（s）
//          PRODUCT_CFG_TCP_KEEPALIVE_R2 #设置不通信则开始启动keepalive的时间（s）
//          PRODUCT_CFG_KEEPALIVE_WAIT  #设置keepalive探测次数(次)
//    #设置发送keepalive的间隔时间（s）
// 参数说明:无
//
// 返 回 值: 正确:SOCKET_ID.
//           失败:0xFFFFFFFF
//
// 调用要求: 任务中调用, 不能适用于中断
// 作    者: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketTcpCreate(HI_VOID);

//*****************************************************************************
// 函数名称: HI_MDM_SocketUdpCreate
// 功能描述: 创建UDP socket
//
// 参数说明:无
//
// 返 回 值: 正确:SOCKET_ID.
//           失败:0xFFFFFFFF
//
// 调用要求: 任务中调用, 不能适用于中断
// 作    者: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_MDM_SocketUdpCreate(HI_VOID);

HI_EAPI HI_U32 HI_MDM_SocketSetLinger(HI_SOCKET s);

//*****************************************************************************
// 函数名称: HI_MDM_SocketBind
// 功能描述:将一本地地址与一套接口捆绑。
//                       本函数适用于未连接的数据报或流类套接口
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   pucIp [in] : 目的IP地址如"192.168.1.1"
//   usPortNum [in] : 端口号
//
// 返 回 值:
//                   成功则返回HI_ERR_SUCCESS，失败返回HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);

//*****************************************************************************
// 函数名称: HI_MDM_SocketListen
// 功能描述:创建一个套接口并监听申请的连接.
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   backlog [in] : 等待连接队列的最大长度
//
// 返 回 值:
//                   成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32 HI_MDM_SocketListen(HI_SOCKET s, HI_S16 backlog);

//*****************************************************************************
// 函数名称: HI_MDM_SocketSetFd
// 功能描述:设置Socket位图中的位域
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   fd [in] : socket 位域内容指针
//
// 返 回 值:
//                   无
//*****************************************************************************
HI_EAPI HI_VOID HI_MDM_SocketSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
HI_EAPI HI_BOOL HI_MDM_SocketIsSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);

//*****************************************************************************
// 函数名称: HI_MDM_SocketSelect
// 功能描述:确定一个或多个套接口的状态
//
// 参数说明:
//   nfds [in] : 是一个整数值，是指集合中所有文件描述符的范围
//   readfds [in] : 指向一组等待可读性检查的套接口
//   writefds [in] : 指向一组等待可写性检查的套接口
//   ulTimeOut [in] : 最多等待时间
// 返 回 值:
//                   成功返回HI_ERR_SUCCESS
//                   超时返回HI_ERR_TIMEOUT
//                   错误返回HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_U32 HI_MDM_SocketSelect(HI_S32 nfds, HI_OUT HI_SYS_SOCKET_FD* readfds, HI_OUT HI_SYS_SOCKET_FD*writefds, HI_U32 ulTimeOut);
HI_EAPI HI_U32 HI_MDM_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt);
HI_EAPI HI_U32 HI_MDM_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_PVOID optval,  HI_U32 optlen);


//*****************************************************************************
// 函数名称: HI_MDM_DfxSetVerInfo
// 功能描述: 设置APP模块版本号和编译时间。
//
// 参数说明:
//      enVerType [in]:模块ID,仅支持HI_DFX_APP_VER
//      szVer [in]:版本号
//      szBuiltDataTime [in]:编译时间
//
// 返 回 值:
//      HI_ERR_SUCCESS:执行成功
//      其它值错误
//
// 调用要求:用户之间在APP的.c文件中调用宏HI_MDM_CONFIG_APP_VER 即可
// 调用举例:HI_MDM_CONFIG_APP_VER(VERSION_MAIN,VERSION_FEATURE,VERSION_BUG)
//*****************************************************************************
HI_EAPI HI_U32 HI_MDM_DfxSetVerInfo(HI_SYS_DFX_VER_E enVerType, HI_CONST HI_CHAR szVer[12], HI_CONST HI_CHAR szBuiltDataTime[20]);


#endif

HI_EAPI HI_U32 HI_MDM_INT_LockTmp(HI_U32 lr);
HI_EAPI HI_VOID HI_MDM_INT_RestoreTmp(HI_U32 lr,HI_U32 ulIntSave);

HI_EAPI HI_U32 HI_MDM_MEM_Init(HI_U32 ulPoolCount);
HI_EAPI HI_U32 HI_MDM_MEM_InitDynStaticPool(HI_U32 ulDynPoolSize,HI_U32 ulStaticPoolSize);

HI_END_HEADER
#endif
