
#ifndef __HI_MDM_DATACOLLECT__
#define __HI_MDM_DATACOLLECT__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**@defgroup hct_data_collect 采数
 * @ingroup mdm
*/

/**
 * @ingroup hct_data_collect
 * 采集数据存储的区域
 */
typedef struct
{
    HI_U32 bufAddr[2]; /**< 因为翻转而有可能有两段数据 ，两段区域首地址*/
    HI_U32 bufSize[2]; /**< 两段区域大小*/
}HI_BUF_S;

/**
 * @ingroup hct_data_collect
 * 采数触发点
 */
typedef enum
{
    EN_TRIG_TYPE_FST_PB = 0,            /**< 0 ：第一个PB块解码完成*/
    EN_TRIG_TYPE_SND_PB,                /**< 1 ：第二个PB块解码完成*/
    EN_TRIG_TYPE_TRD_PB,                /**< 2 ：第三个PB块解码完成*/
    EN_TRIG_TYPE_FTH_PB,                /**< 3 ：第四个PB块解码完成*/
    EN_TRIG_TYPE_FIV_PB,                /**< 4 ：第五个PB块解码完成*/
    EN_TRIG_TYPE_FC_START,              /**< 5 ：FC开始*/
    EN_TRIG_TYPE_FC_ERR,                /**< 6 ：时域帧尾，包括FC解码错*/
    EN_TRIG_TYPE_NOISE,                 /**< 7 : 盲采*/
    EN_TRIG_TYPE_STAT_FOUR,             /**< 8 : 载波侦听状态4*/
    EN_TRIG_TYPE_TX_ENABLE_FFT,         /**< 9 ：tx的FFT使能*/
    EN_TRIG_TYPE_TX_START_FFT,          /**< 10：tx的FFT开始*/
    EN_TRIG_TYPE_ENABLE_IFFT,           /**< 11：符号对齐的IFFT使能*/
    EN_TRIG_TYPE_START_IFFT,            /**< 12：符号对齐的IFFT开始*/
    EN_TRIG_TYPE_ENABLE_FFT,            /**< 13：频域均衡的FFT使能*/
    EN_TRIG_TYPE_START_FFT,             /**< 14：频域均衡的FFT开始*/
    EN_TRIG_TYPE_PREM_FFT_OUTPUT_START, /**< 15：Preamble的FFT输出开始*/
    EN_TRIG_TYPE_RREM_FFT_OUTPUT_END,   /**< 16：Preamble的FFT输出结束*/
    EN_TRIG_TYPE_PAYLOAD_START,         /**< 17：PayLoad解码开始*/
    EN_TRIG_TYPE_CTR12FFT_DIN_EN,       /**< 18：ctrl2fft_din_en*/
    EN_TRIG_TYPE_CTR12FFT_DOUT_EN,      /**< 19：ctrl2fft_dout_en*/
    EN_TRIG_TYPE_NBI_FFT_ENABLE,        /**< 20：NBI的FFT使能*/
    EN_TRIG_TYPE_NBI_FFT_START,         /**< 21：NBI的FFT开始*/
    EN_TRIG_TYPE_PL_DECODE_FAIL,        /**< 22：pl译码失败*/
    EN_TRIG_TYPE_FC_DECODE_FAIL,        /**< 23：fc译码失败*/
    EN_TRIG_TYPE_FC_0PPM_DECODE_FAIL,   /**< 24：fc译码失败（0ppm）*/
    EN_TRIG_TYPE_PARA_DETECT_FAIL,      /**<25：并行检测失败*/
    EN_TRIG_TYPE_FRAME_END,             /**< 26：frame_end*/
    EN_TRIG_TYPE_FC_START_COUNT,        /**< 27：fc_start间隔计数*/
    EN_TRIG_TYPE_MAX,
    EN_TRIG_TYPE_MASK_DATA=256,         /**< 256：数据mask触发*/
}DC_TRIG_TYPE_ENUM;

/**
 * @ingroup hct_data_collect
 * 采数数据：（时域到频域，由前往后）
 */
typedef enum
{
    EN_TRIG_AD_12_BIT=0,               /**< 0 ：12bit AD输出*/
    EN_TRIG_ASYNC_PRO_10_BIT=11,       /**< 11：10bit Async_Pro输出*/
    EN_TRIG_CUBIC_10_BIT=1,            /**< 1 ：10bit Cubic输出*/
    EN_TRIG_IMP_10_BIT=2,              /**< 2 ：10bit Imp输出*/
    EN_TRIG_DCOC_10_BIT=3,             /**< 3 ：10bit DCOC输出*/
    EN_TRIG_NF_10_BIT=4,               /**< 4 :     10bit NF滤波器输出*/
    EN_TRIG_DAGC_10_BIT=5,             /**< 5 ：10bit Dagc输出*/
    EN_TRIG_CSMA_4_BIT=6,              /**< 6 ： 4bit 载波侦听状态*/
    EN_TRIG_FFT_ORDER_32_BIT=7,        /**< 7 ：32bit FFT顺序输出(q，i)*/
    EN_TRIG_FFT_DISORDER_32_BIT=8,     /**< 8 ：32bit FFT乱序输出(q，i)*/
    EN_TRIG_FC_12_BIT=9,               /**< 9 ：12bit FC解调输出（q，i）*/
    EN_TRIG_PAYLOAD_12_BIT=10,         /**< 10：12bit PayLoad解调输出（q，i）*/
    EN_TRIG_TF_10_BIT_AD_DATA=12,      /**< 12：10bit 台区识别AD数据*/
    EN_TRIG_DA_12_BIT=13,              /**< 13：12bit DA数据 */
    EN_TRIG_IMP_18_BIT=14,             /**< 14：18bit IMP输出 */
    EN_TRIG_FFT_16_BIT=15,             /**< 15：16bit FFT数据 */
    EN_TRIG_CS_16_BIT=16,              /**< 16：16bit CS数据*/
    EN_TRIG_LOGIC_16_LBIT_LOGIC=17,    /**< 17：低16bit 逻分数据*/

    // 芯片DC_CAPTURE_CTRL enTrigerType位宽为8bit，而软件的可使用位宽为16bit，所以从256开始表示测试序列模式
    EN_TRIG_INCREMENTAL_SEQUENCE=256,  /**< 256：递增序列 偏移地址0x80对应成员self_test_mode配置为1*/
    EN_TRIG_DESCENDING_SEQUENCE=257,   /**< 257：递减序列 偏移地址0x80对应成员self_test_mode配置为2*/
    EN_TRIG_LOGIC_32_BIT_LOGIC=258,    /**< 258:      32bit逻分数据*/
    EN_TRIG_LOGIC_16_HBIT_LOGIC=259,   /**< 259:高16bit逻分数据*/

    EN_TRIG_DATA_MAX
}DC_TRIG_DATA_ENUM;

/**
 * @ingroup hct_data_collect
 * 采数配置参数
 */
typedef struct
{
    HI_U32 ulWaitEnvOkTime;          /**< 等待采数环境稳定时间(ms)*/
    HI_U32 ulWaitResultTimeOut;      /**<等待采数结果超时时间(ms)*/
    HI_U32  ulStartAddr;             /**< 分配内存起始地址,需要64Byte对齐*/
    HI_U32  ulEndAddr;               /**< 分配内存结束地址,需要64Byte对齐*/
    HI_S32  lAgcGain;                /**< 配置增益*/
    HI_U32  ulAfterPointlen;         /**< 触发采集时，触发点后再采集的数据长度*/
    DC_TRIG_TYPE_ENUM  enTrigerType; /**< 采数触发点*/
    DC_TRIG_DATA_ENUM  enDataSrc;    /**< 采数数据：（时域到频域，由前往后）*/
}DC_INPUT_PARA_STRU;


/**
* @ingroup  hct_data_collect
* @brief  采数功能初始化。
*
* @par 描述:
* 采数功能初始化。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_datacollect.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_DC_Init(HI_VOID);


/**
* @ingroup  hct_data_collect
* @brief  普通采数。
*
* @par 描述:
* 普通采数。
* @attention 采数内存空间起始和结束地址需要64Byte对齐。
*
* @param  pDcInput [IN] 采数设置的参数。
* @param  sBuf [OUT] 存储数据区域。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_datacollect.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_DC_StartNormalDataCollect(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf);

#endif


