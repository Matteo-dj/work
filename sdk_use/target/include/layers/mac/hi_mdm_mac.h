/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_mac.h
* Description：数据链路层二次开发接口。
*
* History:
* Version              Date                      DefectNum           Description
* V200R001C00SPC010    2018-03-31                NA                  第一次版本发布。
* V200R001C00SPC011    2018-05-23                NA                  新增V200能源二次开发接口。
* V200R001C00SPC012    2018-05-24--2018-07-20    DTS2018071704905    新增:电压跌落相关接口、判断模块是否为精准过零电路、获取/设置管理ID接口、抓包相关宏定义及接口、本站点是否入网接口、二次开发接口注释、
                                                 DTS2018070406515         南网协议宏定义、采集器地址和主节点地址宏定义、南网测试模式定义、南网协议设置工作频段接口、加载国网和V100协议、加载南网和V100协议。
                                                 DTS2018062205983    修改:注册抓包回调函数接口的参数、PLC通道可用时拓扑变更状态4及对应参数结构体中增加mac地址信息。
                                                 DTS2018061501525
                       2018-08-02                DTS2018080209741    新增:设置nv中的snid为0、设置当前工作频段的发射功率、设置相位模式接口
                                                 DTS2018072807719    修改:PLC通道可用时拓扑变更状态中的变更状态改为离线和入网状态，南网设置工作频段说明注释
* V200R001C00SPC020    2018-08-04                DTS2018080213757    新增锁定主节点地址状态，修改白名单变化原因码，修改设置无扰识别开关接口为设置无扰识别模式接口，新增查询SNR台区识别结果接口，新增设置主节点地址接口，
* V200R001C00SPC030    2018-08-22                DTS2018082011809    新增查询能源模式接口，新增能源用户启动停止协议探测接口，新增控制信标发送接口，新增获取探测芯片版本信息接口，新增清除探测芯片信息接口，
                                                                     新增设置STA站点判断离线时间接口，新增解析CCO报文作为心跳接口，新增能源用户判断满载未锁定离线接口，新增台区识别寄存器开关接口，新增上电自检功能注册接口，
                                                                     新增物理相位识别功能注册接口，新增无扰NTB识别功能注册接口，新增大电容停电上报功能注册接口，修改CCO端入网鉴权注册说明注释，修改设置组网超时时间接口。
                                                 DTS2018081401152    修改南网设置频段接口中的入参判断及接口说明注释
                       2018-09-10         DTS2018090702209    修改设置恢复V200协议开关接口注释，修改获取V200协议开关接口注释，修改部分接口注释。
                       2018-09-12         DTS2018091210477    删除HI_MDM_SetAppLedSwitch接口，修改收发指示灯配置模式接口注释，修改指示灯状态接口注释。
                       2018-09-18         DTS2018091810261    新增设置工作模式立刻生效接口。
                       2018-09-21         DTS2018091710031    新增MAC层通道注册和发送接口、协议栈可维可测统计量获取接口、配置离线检测周期个数接口、拓扑类型配置接口、客户自定义信标条目接口、配置网络参数等接口。

* V200R001C00SPC050    2018-11-10         DTS2018110703483    新增加载无扰台区识别SNR组件接口、加载无扰台区识别NTB分布式组件接口、清除无扰台区识别NTB识别结果接口、设置站点归属网络接口、设置无扰台区识别NTB发送流程控制接口。
                                                              新增获取相位识别结果接口、设置站点的相位识别结果接口。

* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    增加接口支持信号发送。
* V200R001C00SPC050B012    2018-12-20   DTS2018122005101    增加远程通讯终端交换模块注册接口、是否设置最大站点
* V200R001C00SPC050B012    2018-12-26   DTS2018122107640    修改采集过零NTB接口、新增指示灯闪烁控制接口
* V200R001C00SPC050B012    2018-12-26   DTS2018121707454    增加芯片ID探测功能。
* V200R001C00SPC050B013    2019-01-09   DTS2019010904749    新增获取首次组网完成的时长接口
                                                            新增获取启动组网的时长接口
                                                            新增获取拓扑变更次数接口
                                                            新增获取路由周期、代理站点发现列表报文周期、发现站点发现列表报文周期接口。
                                                            新增根据MAC地址获取站点管理ID信息接口
                                                            修改获取拓扑结构体字段
* V200R001C00SPC050B014    2019-01-23   DTS2019012104349    新增配置过零消失工频周期接口
                                                            新增获取过零消失工频周期接口
                                                            新增模块拔出回调函数
                                                            新增CCO 断相上报、断相恢复事件回调
                                        DTS2019012410663    新增工频过零边沿个数宏定义
                                                            修改无扰台区识别结果输出结构字段
                                                            新增台区识别模块回调函数的类型及回调数据结构
                                                            新增注册台区识别回调函数接口
                                                            新增通知STA采集NTB数据接口
                                                            新增通知处理NTB数据接口
                                                            新增设置接收邻台区广播APP报文控制接口
                                                            修改PLC通道发送参数字段
                                         2019-02-01  DTS2019012104349
                                                            新增配置单相三相模块接口
                                                            新增配置单相三相模块电压跌落检测结果接口
                                                            新增过零消失电压未跌落回调事件上报
                                         2019-02-01  DTS2019010900227  新增拓扑中物理相位B,C识别结果
                                                            新增判断三相表模块支持过零类型接口
                                                            新增物理相位识别完成通知接口
                                                            修改工装采集NTB接口
* V200R001C00SPC050B019    2019-02-21   DTS2019010504405    新增抄控宝是否接入强电接口
                                                            新增CCO三相过零电路检测结果接口
* V200R001C00SPC051B012    2019-03-11   DTS2019031104604    新增CCO设置上报站点离线周期
                                                            新增CCO获取上报站点离线周期
                                                            
******************************************************************************/

/**@defgroup hct_mac 数据链路层
 * @ingroup mdm
 */

#ifndef __HI_MDM_MAC_H__
#define __HI_MDM_MAC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER

/**
 *@ingroup hct_mac
 * IO管脚定义
 */
typedef enum
{
    HI_LED_ID0 = 0,     /**< CCO的B相位发送灯,整表检测串口接收指示灯             */
    HI_LED_ID1,           /**< CCO的C相位发送灯,整表检测串口发送指示灯             */
    HI_LED_ID2,          /**< CCO的A相位发送灯             */
    HI_LED_ID3,          /**< 接收指示灯                          */
    HI_LED_ID4,          /**< 发送灯，(II采为485灯)      */
    HI_LED_ID5,          /**<发射机告警指示灯           */
    HI_LED_ID6,          /**<发射机A相位过零显示    */
    HI_LED_ID7,         /**<发射机B相位过零显示     */
    HI_LED_ID8,         /**<发射机C相位过零显示     */
    HI_LED_MAX_ID,  /**<表示非法ID              */
}HI_LED_ID_E;

/**
 *@ingroup hct_mac
 * TONEMASK的size
 */
#define HI_MAX_TONEMASK_NUM                    16
/**
 *@ingroup hct_mac
 * TONEMASK的模式，对应国网的全频下使用
 */
#define TONE_MASK_MODE_1                          0x00
/**
 *@ingroup hct_mac
 * TONEMASK的模式，对应国网的2.5M~5.7M使用
 */
#define TONE_MASK_MODE_2                          0x01
/**
 *@ingroup hct_mac
 * TONEMASK的模式，对应国网的0.7M~3M下使用
 */
#define TONE_MASK_MODE_3                          0x02
/**
 *@ingroup hct_mac
 * TONEMASK的模式，对应国网的1.7M~3M使用
 */
#define TONE_MASK_MODE_4                          0x03
/**
 *@ingroup hct_mac
 * 表示当前模块角色未知
 */
#define HI_MDM_PLC_ROLE_UNKNOWN        (0)
/**
 *@ingroup hct_mac
 * 表示当前站点为STA模块
 */
#define HI_MDM_PLC_ROLE_STA                  (1)
/**
 *@ingroup hct_mac
 * 表示当前站点为代理模块
 */
#define HI_MDM_PLC_ROLE_PROXY              (2)
/**
 *@ingroup hct_mac
 * 表示当前站点为CCO模块
 */
#define HI_MDM_PLC_ROLE_CCO                  (4)

/**
 *@ingroup hct_mac
 * 表示当前工作模式0
 */
#define HI_MDM_PLC_FREQ_0                       (0)
/**
 *@ingroup hct_mac
 * 表示当前工作模式1
 */
#define HI_MDM_PLC_FREQ_1                       (1)
/**
 *@ingroup hct_mac
 * 表示当前工作模式2
 */
#define HI_MDM_PLC_FREQ_2                       (2)
/**
 *@ingroup hct_mac
 * 表示当前工作模式3
 */
#define HI_MDM_PLC_FREQ_3                       (3)
/**
 *@ingroup hct_mac
 * 表示当前工作模式4
 */
#define HI_MDM_PLC_FREQ_4                       (4)

/**
 *@ingroup hct_mac
 * 表示A相位
 */
#define HI_MDM_PLC_PHASE_A                      (1)
/**
 *@ingroup hct_mac
 * 表示B相位
 */
#define HI_MDM_PLC_PHASE_B                      (2)
/**
 *@ingroup hct_mac
 * 表示C相位
 */
#define HI_MDM_PLC_PHASE_C                      (3)

/**
 *@ingroup hct_mac
 * 表示未知状态，比如初始化后的默认状态
 */
#define HI_MDM_CHL_STATUS_UNKNOWN         (0)
/**
 *@ingroup hct_mac
 * 表示PLC通道已经 连接，APP可以使用
 */
#define HI_MDM_CHL_STATUS_CONNECT          (1)
/**
 *@ingroup hct_mac
 * 表示PLC通道已经断开，APP不可以使用（如果使用可能会失败）
 */
#define HI_MDM_CHL_STATUS_DISCONNECT     (2)
/**
 *@ingroup hct_mac
 * 表示PLC通道正在建立连接，APP不可以使用（如果使用可能会失败）
 */
#define HI_MDM_CHL_STATUS_CONNECTING     (3)

/**
 *@ingroup hct_mac
 * 表示搜网状态
 */
#define HI_MDM_CHL_STATUS_SEARCHING        (4)

/**
 *@ingroup hct_mac
 * 设置白名单选项: MDM层不需要回复白名单信息astItem，但在设置结束后回复白名单设置的成功和失败总个数
 */
#define HI_MDM_NM_WHITE_LIST_SET_OPTION_NO_RSP          (0)
/**
 *@ingroup hct_mac
 * 设置静态代理选项: 不存储FLASH,重启失效
 */
#define HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE               (1)
#if 0
/**
 *@ingroup hct_mac
 * 设置静态代理选项: 存储FLASH,重启有效
 */
//#define HI_MDM_NM_STATIC_PROXY_OPT_SAVE                        (2)
#endif
/**
 *@ingroup hct_mac
 * 用户NA的ID定义
 */
#define HI_MDM_OBJ_ID_NA        0x0
/**
 *@ingroup hct_mac
 * 用户一的ID定义
 */
#define HI_MDM_OBJ_ID_USR1      0x1
/**
 *@ingroup hct_mac
 * 用户二的ID定义
 */
#define HI_MDM_OBJ_ID_USR2      0x2
/**
 *@ingroup hct_mac
 * 用户三的ID定义
 */
#define HI_MDM_OBJ_ID_USR3      0x3
/**
 *@ingroup hct_mac
 * 抄表用户的ID定义
 */
#define HI_MDM_OBJ_ID_MRS       0x4
/**
 *@ingroup hct_mac
 * 用户数量
 */
#define HI_MDM_OBJ_ID_NUM       4
/**
 *@ingroup hct_mac
 * 用户对象缓存的最大值
 */
#define HI_MDM_OBJ_BUFF_NUM_MAX (HI_MDM_OBJ_ID_NUM + 1)

/**
 *@ingroup hct_mac
 * 台区名单刷新的状态，未知
 */
#define HI_MDM_TRANSFORMER_LIST_REFRESH_STATE_UNKNOWN    0
/**
 *@ingroup hct_mac
 * 相位名单刷新的状态，未知
 */
#define HI_MDM_PHASE_LIST_REFRESH_STATE_UNKNOWN                 0

/**
 *@ingroup hct_mac
 * 台区识别结果，未识别
 */
#define HI_MDM_TRANSFORMER_RESULT_UNKNOWN                       0x00
/**
 *@ingroup hct_mac
 * 台区识别结果，属于本台区
 */
#define HI_MDM_TRANSFORMER_RESULT_BELONG                           0x01
/**
 *@ingroup hct_mac
 * 台区识别结果，不属于本台区
 */
#define HI_MDM_TRANSFORMER_RESULT_NOT_BELONG                   0x02
/**
 *@ingroup hct_mac
 * 台区识别结果，不支持识别
 */
#define HI_MDM_TRANSFORMER_RESULT_NOT_SUPPORT                 0x03
/**
 *@ingroup hct_mac
 * 台区识别结果，识别失败
 */
#define HI_MDM_TRANSFORMER_RESULT_FAIL                                 0x04

/**
 *@ingroup hct_mac
 * 相位识别状态，未识别状态
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN                         0
/**
 *@ingroup hct_mac
 * 相位识别状态，不支持相位识别
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT                  1
/**
 *@ingroup hct_mac
 * 相位识别状态，正在识别
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_DOING                               2
/**
 *@ingroup hct_mac
 * 相位识别状态，识别结束
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_OVER                                 3
/**
 *@ingroup hct_mac
 * 零火线识别状态，未识别状态
 */
#define HI_MAC_PN_IDENTIFY_STATE_UNKNOWN      HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN
/**
 *@ingroup hct_mac
 * 零火线识别状态，不支持识别
 */
#define HI_MAC_PN_IDENTIFY_STATE_NOT_SUPPORT  HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT
/**
 *@ingroup hct_mac
 * 零火线识别状态，正在识别
 */
#define HI_MAC_PN_IDENTIFY_STATE_DOING        HI_MAC_PHASE_IDENTIFY_STATE_DOING
/**
 *@ingroup hct_mac
 * 零火线识别状态，识别结束
 */
#define HI_MAC_PN_IDENTIFY_STATE_OVER         HI_MAC_PHASE_IDENTIFY_STATE_OVER
/**
 *@ingroup hct_mac
 * 黑名单的站点个数最大值
 */
#define HI_MDM_BLACK_LIST_NUM_MAX                                           2048
/**
 *@ingroup hct_mac
 * 台区识别名单的站点个数最大值
 */
#define HI_MDM_TRANSFORMER_IDENTIFY_LIST_NUM_MAX              2048

 /**
 * @ingroup hct_mac
 * 协议版本号定义: V100协议版本号
 */
#define HI_MDM_PTCL_VER_100         0x0001
 /**
 * @ingroup hct_mac
 * 协议版本号定义: 国网协议版本号
 * 0x0040 - 0x007f:国网版本
 */
#define HI_MDM_PTCL_VER_SG           0x0040
 /**
 * @ingroup hct_mac
 * 协议版本号定义: V200协议版本号
 * 0x0080 - 0x00bf:V200版本
 */
#define HI_MDM_PTCL_VER_200        0x0080

 /**
 * @ingroup hct_mac
 * 协议版本号定义: 南网协议版本号
 * 0x00c0 - 0x00ff:CSG版本
 */
#define HI_MDM_PTCL_VER_CSG        0x0002

/**
 *@ingroup hct_mac
 * V100未锁定状态
 */
#define HI_MDM_UNLOCK_NETWORK_STATUS    (0)
/**
 *@ingroup hct_mac
 * 锁定MAC状态
 */
#define HI_MDM_LOCK_CCOMAC_STATUS           (1)
/**
 *@ingroup hct_mac
 * 锁定频段SNID状态
 */
#define HI_MDM_LOCK_FREQSNID_STATUS         (2)
/**
 *@ingroup hct_mac
 * 锁定主节点地址状态
 */
#define HI_MDM_LOCK_MAIN_NODE_ADDR_TATUS    (3)

/**
 *@ingroup hct_mac
 * DIAG 命令通知默认ID号
 */
#define HI_MDM_DIAG_CMD_NOTIFY_DEFAULT_ID 0

/**
 *@ingroup hct_mac
 * 设备名称标示字节大小
 */
#define HI_MDM_DEV_ID_LEN       6

/**
* @ingroup  hct_mac
* @brief  台区识别工装检测注册函数
*
* @par  描述:
* 台区识别工装检测注册函数
* @attention 无。
* @param  ret [IN] 类型 #HI_U32，#HI_ERR_SUCCESS:查询成功，#HI_ERR_FAILURE:查询失败。
* @param  avg_ntb [IN] 类型 #HI_U32，过零检测平均周期，单位us。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see EQUIP_ZERO_NTB_NUM 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*QueryZeroCrossCallback)(HI_U32 ret, HI_U32 avg_ntb);

/**
 * @ingroup hct_mac
 *  工装检测设置的接收档位
 */
#define EQUIP_ZERO_CROSS_LEVEL_240K                   0
/**
 * @ingroup hct_mac
 *  工装检测设置的接收档位
 */
#define EQUIP_ZERO_CROSS_LEVEL_1M                      1
/**
 * @ingroup hct_mac
 *  工装检测采集数据个数
 */
#define EQUIP_ZERO_NTB_NUM                                  20
/**
 * @ingroup hct_mac
 *  工装检测上升沿模式
 */
#define EQUIP_ZERO_CROSS_CRICUIT_RISE                  0
/**
 * @ingroup hct_mac
 *  工装检测下降沿模式
 */
#define EQUIP_ZERO_CROSS_CRICUIT_FALL                  1
/**
 * @ingroup hct_mac
 *  功率检测门限最大值
 */
#define MAX_POWER_DETECT_THRESHOLD                    127

/**
* @ingroup  hct_mac
* @brief  工装检测收集CCO,STA产品的过零点数据的回调函数类型
*
* @par  描述:
* 工装检测收集CCO,STA产品的过零点数据的回调函数类型
* @attention 无。
* @param  ulRet [IN] 类型 #HI_U32，#HI_ERR_SUCCESS:查询成功，#HI_ERR_FAILURE:查询失败。
* @param  ucLevel [IN] 类型 #HI_U8，STA的接收档位，其中0表示240K，1表示1M。
* @param  ulNtb [EQUIP_ZERO_NTB_NUM] [IN] 类型 #HI_U32，过零点数据，数组大小参见#EQUIP_ZERO_NTB_NUM定义。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see EQUIP_ZERO_NTB_NUM 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*CollectNtbCallback)(HI_U32 ulRet, HI_U8 ucLevel, HI_U32 ulNtb[EQUIP_ZERO_NTB_NUM]);

/**
* @ingroup  hct_mac
* @brief  定义点对点通信接收数据函数的类型
*
* @par  描述:
* 定义点对点通信接收数据函数的类型
* @attention 作用产品类型为STA。
* @param  data [IN] 类型 #HI_PVOID*，报文数据指针。
* @param  data_size [IN] 类型 #HI_U16，报文数据大小。
* @param  mac_frame_ctrl [IN] 类型  #DMS_MAC_FRAME_CTRL_S*，报文控制信息。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_type.h文件中#DMS_MAC_FRAME_CTRL_S 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PTPCHL_RX_F)(HI_IN HI_PVOID *, HI_U16 , HI_IN DMS_MAC_FRAME_CTRL_S* );
/**
* @ingroup  hct_mac
 *  供电方式枚举定义
 */
typedef enum
{
    HI_MAC_STRONG_POOL_POWER_SUPPLY = 0,        /**<强电供电  */
    HI_MAC_BATTERY_POWER_SUPPLY                        /**<电池供电  */
}HI_MAC_POWER_SUPPLY_MODE_ENUM;

/**
* @ingroup  hct_mac
* @brief 通道点对点通信回调函数
*
* @par  描述:
* @li 通道点对点通信回调函数
* @attention 无。
* @param  pCmdParam [IN] 类型 #HI_PVOID，填入发送buff 参数内容。
* @param  usCmdParamSize [IN] 类型 #HI_U16，参数长度。
* @param  ulOption [IN] 类型 #HI_U32，选项字，当前保留填写0。
* @retval   ret 类型 #HI_U32，#HI_ERR_INVALID_PARAMETER 入参不合法，#HI_ERR_NOT_ENOUGH_MEMORY    内存申请失败，\n
                   #HI_ERR_SUCCESS 报文发送成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_MDM_NDM_PING_F)(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/**
 * @ingroup  hct_mac
 *  白名单模式配置，仅基础信息库作为白名单
 */
#define HI_MDM_WHITE_LIST_MODE_BASE_LIB               0
/**
 * @ingroup  hct_mac
 *  白名单模式配置，表档案和基础信息库作为白名单
 */
#define HI_MDM_WHITE_LIST_MODE_METER_BASE_LIB   1
/**
 * @ingroup  hct_mac
 *  白名单模式配置，仅表档案作为白名单
 */
#define HI_MDM_WHITE_LIST_MODE_METER                    2
/**
 * @ingroup  hct_mac
 *  白名单模式配置，无效模式
 */
#define HI_MDM_WHITE_LIST_MODE_INVALID              0xff

/**
 * @ingroup  hct_mac
 *  DIAG命令处理映射表最小命令ID
 */
#define HI_MDM_NM_REQ_ID_MIN                        0x3610

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取网络拓扑结构功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取网络拓扑结构
* @attention 作用产品类型为CCO。
* @param  topo_req [IN] 类型 #HI_MDM_NM_TOPO_REQ_S，拓扑查询请求。
* @param  topo_ind [OUT] 类型 #HI_MDM_NM_TOPO_IND_S，拓扑查询应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_TOPO_REQ_S和#HI_MDM_NM_TOPO_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_TOPO_ID                   0x3610

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取站点当前频段功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点当前频段
* @attention 无。
* @param  freq_qry_ind [OUT] 类型 #HI_MDM_NM_FREQ_QRY_IND_S，频段查询应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_FREQ_QRY_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_FREQ_QRY_ID               0x3611

/**
* @ingroup  hct_mac
* @brief 该消息ID实现通知全网切换频功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，通知全网切换频段(只可设置CCO)
* @attention 作用产品类型为CCO。
* @param  freq_set_req [IN] 类型 #HI_MDM_NM_FREQ_SET_REQ_S，频段设置请求 。
* @param  freq_set_ind [OUT] 类型 #HI_MDM_NM_FREQ_SET_IND_S，频段设置应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_FREQ_SET_REQ_S和#HI_MDM_NM_FREQ_SET_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_FREQ_SET_ID               0x3612

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取站点版本信息功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点版本信息
* @attention 无。
* @param  device_ver_ind [OUT] 类型 #HI_MDM_NM_DEVICE_VER_IND_S，站点版本信息应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_DEVICE_VER_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_DEVICE_VER_ID             0x3613

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取站点路由表功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点路由表
* @attention 无。
* @param  route_info_req [IN] 类型 #HI_MDM_NM_ROUTER_INFO_REQ_S，查询路由信息请求 。
* @param  route_info_ind [OUT] 类型 #HI_MDM_NM_ROUTER_INFO_IND_S，查询路由信息应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_ROUTER_INFO_REQ_S和#HI_MDM_NM_ROUTER_INFO_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_ROUTER_INFO_ID            0x3614

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取获取站点信息功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点信息
* @attention 无。
* @param  node_qry_ind [OUT] 类型 #HI_MDM_NM_NODE_IND_S，查询站点信息应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_NODE_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_NODE_ID                   0x3615

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取获取站点发现列表功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点发现列表
* @attention 无。
* @param  discover_list_req [IN] 类型 #HI_MDM_NM_DISCOVER_LIST_REQ_S，查询发现列表请求 。
* @param  discover_list_ind [OUT] 类型 #HI_MDM_NM_DISCOVER_LIST_IND_S，查询发现列表应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_DISCOVER_LIST_REQ_S和#HI_MDM_NM_DISCOVER_LIST_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_DISCOVER_LIST_ID          0x3616

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取网络白名单功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取网络白名单(只可CCO使用)
* @attention  作用产品类型为CCO。
* @param  white_list_qry_req [IN] 类型 #HI_MDM_NM_WHITE_LIST_QRY_REQ_S，查询白名单请求 。
* @param  white_list_qry_ind [OUT] 类型 #HI_MDM_NM_WHITE_LIST_QRY_IND_S，查询白名单应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_WHITE_LIST_QRY_REQ_S和#HI_MDM_NM_WHITE_LIST_QRY_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_WHITE_LIST_QRY_ID         0x3617

/**
* @ingroup  hct_mac
* @brief 该消息ID实现设置网络白名单功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，设置网络白名单(只可CCO使用,白名单为基础信息库)
* @attention  作用产品类型为CCO。
* @param  whitelist_set_req [IN] 类型 #HI_MDM_NM_WHITE_LIST_SET_REQ_S，设置白名单请求 。
* @param  whitelist_set_ind [OUT] 类型 #HI_MDM_NM_WHITE_LIST_SET_IND_S，设置白名单应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_WHITE_LIST_SET_REQ_S和#HI_MDM_NM_WHITE_LIST_SET_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_WHITE_LIST_SET_ID         0x3618

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取站点SNID功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点SNID
* @attention 无。
* @param  snid_qry_ind [OUT] 类型 #HI_MDM_NM_SNID_QRY_IND_S，查询站点SNID应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_SNID_QRY_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SNID_QRY_ID               0x3619

/**
* @ingroup  hct_mac
* @brief 该消息ID实现设置站点SNID功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，设置站点SNID
* @attention 作用产品类型为CCO。
* @param  snid_set_req [IN] 类型 #HI_MDM_NM_SNID_SET_REQ_S，设置站点SNID请求 。
* @param  snid_set_ind [OUT] 类型 #HI_MDM_NM_SNID_SET_IND_S，设置站点SNID应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_SNID_SET_REQ_S和#HI_MDM_NM_SNID_SET_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SNID_SET_ID               0x361A

/**
* @ingroup  hct_mac
* @brief 该消息ID实现获取站点静态代理功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，获取站点静态代理
* @attention 作用产品类型为STA、NDM（非CCO）。
* @param  static_proxy_qry_ind [OUT] 类型 #HI_MDM_NM_STATIC_PROXY_QRY_IND_S，获取站点静态代理请求 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_STATIC_PROXY_QRY_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_STATIC_PROXY_QRY_ID       0x361B

/**
* @ingroup  hct_mac
* @brief 该消息ID实现设置站点静态代理功能
*
* @par   描述:
* 通过函数 HI_MDM_MsgSend发送该消息到MDM层，设置站点静态代理
* @attention 作用产品类型为STA、NDM（非CCO）。
* @param  static_proxy_set_req [IN] 类型 #HI_MDM_NM_STATIC_PROXY_SET_REQ_S，设置站点静态代理请求 。
* @param  static_proxy_set_ind [OUT] 类型 #HI_MDM_NM_STATIC_PROXY_SET_IND_S，设置站点静态代理应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_STATIC_PROXY_SET_REQ_S和#HI_MDM_NM_STATIC_PROXY_SET_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_STATIC_PROXY_SET_ID       0x361C

/**
* @ingroup  hct_mac
* @brief  该消息ID实现获取白名单开关
*
* @par   描述:
* 通过HI_MDM_MsgSend函数发送该消息给抄控器的MDM层，获取白名单开关
* @attention 作用产品类型为CCO。
* @param  whitelist_switch_qry_ind [OUT] 类型 #HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S，获取白名单开关应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_QRY_WHITE_LIST_SWITCH_ID  0x361F

/**
* @ingroup  hct_mac
* @brief  该消息ID实现设置白名单开关
*
* @par   描述:
* 通过HI_MDM_MsgSend函数发送该消息给抄控器的MDM层，设置白名单开关
* @attention 作用产品类型为CCO。
* @param  set_req [IN] 类型 #HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S，设置白名单开关请求 。
* @param  set_ind [OUT] 类型 #HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S，设置白名单开关应答 。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 在hi_mdm_mac.h文件中#HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S和#HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S的定义。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SET_WHITE_LIST_SWITCH_ID  0x3620


/**
 * @ingroup  hct_mac
 *  DIAG命令处理映射表最小命令ID
 */
#define HI_MDM_NM_REQ_ID_MAX                0x3620

/**
 * @ingroup  hct_mac
 *  信标最大发送次数的最大值
 */
#define HI_MDM_NM_MAX_BEACON_MAX_SEND_CNT       3
/**
 *@ingroup hct_mac
 * 获取拓扑结构请求结构
 */
typedef struct
{
    HI_U16 usNodeNum;       /**<表示发送请求拓扑查询后，MDM层每次上报给APP层的拓扑节点的最大个数\n
                                                    如果该参数为0，则MDM层每次上报给APP层的拓扑节点个数为1 。     */
    HI_U16 usOffsetNum;     /**<指定获取第几个节点，取值为0表示不指定。目前版本不支持。  */
}HI_MDM_NM_TOPO_REQ_S;

/**
 *@ingroup hct_mac
 * 获取拓扑中的站点信息
 */
typedef struct
{
    HI_U32 uTei:12;                  /**< 本站点TEI                 */
    HI_U32 uProxyTei:12;         /**<  站点父代理TEI      */
    HI_U32 uLevel:4;                /**<  本站点层级(0-15)     */
    HI_U32 uRole:3;                 /**<  本站点角色(参考 #HI_NDM_PLC_ROLE_STA等)   */
    HI_U32 ulOnLine:1;            /**<  当前站点是否在线      */
    HI_U8  ucUlCommRate;      /**<  本站点上行通信成功率 */
    HI_U8  ucDlCommRate;      /**<  本站点下行通信成功率   */
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];     /**<  本站点MAC地址    */
    HI_U32 ulJoinTime;             /**<  本站点加入网络时间    */
    HI_U8  ucBootVer;              /**<  boot版本号           */
    HI_U8  aucVer[3];               /**<  VRC版本号，取值说明如下。\n
                                                     aucVer[0]表示V版本号，如取值100则表示V100\n
                                                      aucVer[1]表示R版本号，如取值1则表示R001\n
                                                       aucVer[2]表示C版本号，如取值0则表示C00       */
    HI_U16 ausVer[2];              /**< B和SPC版本号，取值说明如下。  */
                                            /**<  ausVer[0]表示B版本号，最大为256，如取值102则表示B102\n
                                                     ausVer[1]表示SPC版本号，最大为256，如取值1则表示SPC001。*/
    HI_U32 uLeaveTime;         /**<  站点离线时长，单位s。*/
    HI_U32 uLeaveCnt;           /**<  站点的离线次数。     */
    HI_U32 uProxyCnt;           /**<  站点代理变更次数。   */
    HI_U8  ucProxyType;        /**<  站点的代理类型。 */
    HI_U8  reserved[3];
}HI_MDM_NM_TOPO_NODE_S;

/**
 *@ingroup hct_mac
 * 获取拓扑信息结构
 */
typedef struct
{
    HI_U16 usTotalNodeNum;      /**< Topo中所有站点个数(包含CCO)       */
    HI_U16 usNodeNum;             /**<每个数据包中有效站点个数       */
    HI_MDM_NM_TOPO_NODE_S astNode[0];   /**<站点信息                               */
}HI_MDM_NM_TOPO_IND_S;

/**
 *@ingroup hct_mac
 * 设置全网切频段命令结构
 */
typedef struct
{
    HI_U16 usFreqSn;        /**<设置的频段(参考#HI_MDM_PLC_FREQ_1等)        */
    HI_U16 usStartTime;   /**< 切换频段的时间；单位s               */
}HI_MDM_NM_FREQ_SET_REQ_S;

/**
 *@ingroup hct_mac
 * 频段控制信息结构
 */
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**< 本站点的MAC地址    */
    HI_U16 usFreqSn;    /**< 要切换频段    */
    HI_U32 ulErr;          /**< 是否设置成功    */
}HI_MDM_NM_FREQ_SET_IND_S;

/**
 *@ingroup hct_mac
 * 获取频段信息结构
 */
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];      /**< 本站点的MAC地址    */
    HI_U16 usFreqSn;                                            /**< 本站点当前频段    */
}HI_MDM_NM_FREQ_QRY_IND_S;

/**
 *@ingroup hct_mac
 * 获取版本信息结构
 */
typedef struct
{
    HI_U8  ucBootVer;      /**< boot版本号           */
    HI_U8  aucVer[3];       /**<VRC版本号，取值说明如下。\n
                                        aucVer[0]表示V版本号，如取值100则表示V100\n
                                        aucVer[1]表示R版本号，如取值1则表示R001\n
                                        aucVer[2]表示C版本号，如取值0则表示C00           */
    HI_U16 ausVer[2];       /**< B和SPC版本号，取值说明如下。\n
                                        ausVer[0]表示B版本号，最大为256，如取值102则表示B102\n
                                        ausVer[1]表示SPC版本号，最大为256，如取值1则表示SPC001      */
    HI_U8  ucNvCVer;        /**< NV的C版本号,NV的V为aucVer[0],R版本为aucVer[1]          */
    HI_U8  reserved;
    HI_U16 ausNvVer[2];     /**< NV的B和SPC版本号，取值说明如下。\n
                                            ausVer[0]表示B版本号，最大为256，如取值102则表示B102\n
                                            ausVer[1]表示SPC版本号，最大为256，如取值1则表示SPC001      */
    HI_U16 pad;                   /**< 保留    */
}HI_MDM_NM_DEVICE_VER_IND_S;

/**
 *@ingroup hct_mac
 * 获取路由表请求结构
 */
 typedef struct
{
    HI_U16 usNodeNum;       /**< 表示发送请求路由信息查询后，MDM层每次上报给APP层的节点的最大个数\n
                                                 如果该参数为0，则MDM层每次上报给APP层的节点个数为1   */
    HI_U16 usOffsetNum;     /**< 指定获取第几个节点，取值为0表示不指定。目前版本不支持 */
}HI_MDM_NM_ROUTER_INFO_REQ_S;

/**
 *@ingroup hct_mac
 * 站点路由表信息
 */
typedef struct
{
    HI_U16 usOdTei;     /**<目的站点*/
    HI_U16 reserved;   /**<保留*/
    HI_U16 ausNextHopTei[4];    /**<下一跳数组*/
}HI_MDM_NM_ROUTER_INFO_NODE_S;

/**
 *@ingroup hct_mac
 * 获取路由表信息结构
 */
typedef struct
{
    HI_U16 usTotalNodeNum;      /**<网络所有站点个数(包含CCO)       */
    HI_U16 usNodeNum;           /**<每个数据包中有效站点个数        */
    HI_MDM_NM_ROUTER_INFO_NODE_S astNode[0];    /** <站点路由表信息  */
}HI_MDM_NM_ROUTER_INFO_IND_S;

/**
 *@ingroup hct_mac
 * 获取站点信息结构
 */
typedef struct
{
    HI_U32 uTei:12;             /**<本站点TEI          */
    HI_U32 uProxyTei:12;    /**<本站点父代理TEI            */
    HI_U32 uLevel:4;            /**< 本站点层级(0-15)          */
    HI_U32 uRole:3;             /**<本站点角色(参考 #HI_NDM_PLC_ROLE_STA等)         */
    HI_U32 bOnline:1;           /**< 当前站点是否在线       */
    HI_U8  ucUlCommRate;    /**< 本站点上行通信成功率      */
    HI_U8  ucDlCommRate;    /**< 本站点下行通信成功率      */
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];     /**< 本站点MAC地址     */
    HI_U8  aucIpAddr[HI_IPV4_ADDR_LEN];             /**< 本站点IP地址      */
    HI_U32 ulRunTime;            /**< 本站点运行时间，单位ms        */
}HI_MDM_NM_NODE_IND_S;

/**
 *@ingroup hct_mac
 * 发现列表请求控制字段
 */
typedef struct
{
    HI_U16 usItemNum;       /**<MDM层每次上报给APP层的发现列表项的最大个数\n
                                                如果该参数为0，则MDM层每次上报给APP层的发现\n
                                                列表项个数为1        */
    HI_U16 reserved;   /**<保留*/
}HI_MDM_NM_DISCOVER_LIST_REQ_S;

/**
 *@ingroup hct_mac
 * 获取发现列表信息结构体
 */
typedef struct
{
    HI_U32 uTei:12;             /**<本站点TEI         */
    HI_U32 uProxyTei:12;     /**< 本站点父代理TEI      */
    HI_U32 uLevel:4;            /**< 本站点层级(0-15)         */
    HI_U32 uRole:3;             /**< 本站点角色(参考 #HI_NDM_PLC_ROLE_STA等)       */
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**< 本站点MAC地址     */
    HI_U8 ucUlCommRate;      /**< 本站点上行通信成功率     */
    HI_U8 ucDlCommRate;     /**< 本站点下行通信成功率      */
}HI_MDM_NM_DISCOVER_LIST_ITEM_S;
/**
 *@ingroup hct_mac
 * 获取发现列表信息结构体
 */
typedef struct
{
    HI_U16 usTotalItemNum;      /**<   发现列表总个数       */
    HI_U16 usItemNum;             /**<  astItem的有效个数           */
    HI_MDM_NM_DISCOVER_LIST_ITEM_S astItem[0];   /**< 发现列表列表项 */
}HI_MDM_NM_DISCOVER_LIST_IND_S;

/**
 *@ingroup hct_mac
 *获取 白名单请求信息结构体
 */
typedef struct
{
    HI_U16 usItemNum;   /**< 发送请求发现查询后，MDM层每次上报给APP层的白名单的最大个数\n
                                 如果该参数为0，则MDM层每次上报给APP层的白名单个数为1       */
    HI_U16 reserved;    /**<保留*/
}HI_MDM_NM_WHITE_LIST_QRY_REQ_S;

/**
 *@ingroup hct_mac
 * 白名单项
 */
typedef struct
{
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**<  站点MAC地址   */
    HI_U16 reserved;    /**<保留*/
}HI_MDM_NM_WHITE_LIST_ITEM_S;

/**
 *@ingroup hct_mac
 * 设置白名单信息结构体
 */
typedef struct
{
    HI_U16 usTotalItemNum;      /**<  白名单的总个数   */
    HI_U16 usItemNum;               /**<stItem的有效个数     */
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];   /**<  白名单项    */
}HI_MDM_NM_WHITE_LIST_QRY_IND_S;

/**
 *@ingroup hct_mac
 * 设置白名单请求信息结构体
 */
typedef struct
{
    HI_U16 usOption;        /**< 白名单回复选项，取值如下：\n
                                            #HI_MDM_NM_BLACK_WHITE_LIST_SET_OPTION_NO_RSP表示MDM层不需要回复白名单信息astItem，但在设置结束后回复设置的成功和失败总个数\n
                                            以上选项以最后一次请求的选项为准 */
    HI_U16 usId;                    /**<白名单的ID号。如果MDM层接收到多个该请求消息且ID不一致，MDM层则仅保存最后一个ID的白名单项*/
    HI_U16 usTotalItemNum;  /**< 设置白名单的总个数，为0时清空基础信息库 */
    HI_U16 usItemNum;          /**< astItem的有效个数    */
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];  /**<  白名单项   */
}HI_MDM_NM_WHITE_LIST_SET_REQ_S;

/**
 *@ingroup hct_mac
 * 设置白名单接口输出的设置结果信息
 */
typedef struct
{
    HI_U32 ulErr;                             /**< 设置是否成功; #HI_ERR_SUCCESS表示成功*/
    HI_U16 usTotalItemNum;            /**< 设置白名单的总个数*/
    HI_U16 usTotalItemNumOk;        /**<设置白名单成功的总个数*/
    HI_U16 usTotalItemNumErr;       /**<设置白名单失败的总个数*/
    HI_U16 usItemNum;                   /**<白名单项个数（astItem的个数）*/
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];   /**<黑白名单项*/
}HI_MDM_NM_WHITE_LIST_SET_IND_S;

/**
 *@ingroup hct_mac
 * 获取本站点SNID结构体
 */
typedef struct
{
    HI_U32 ulSnid;      /**<本站点当前SNID      */
}HI_MDM_NM_SNID_QRY_IND_S;

/**
 *@ingroup hct_mac
 * 设置SNID请求结构体
 */
typedef struct
{
    HI_U32 ulSnid;      /**< 要设置的SNID  */
}HI_MDM_NM_SNID_SET_REQ_S;

/**
 *@ingroup hct_mac
 * 设置SNID信息结构体
 */
typedef struct
{
    HI_U32 ulSnid;      /**< 网络的SNID       */
    HI_U32 usRc:1;      /**<  本消息的返回码，取值如下：\n
                                 #HI_FALSE表示指定的SNID已经存在，不需要重新设置\n
                                 #HI_TRUE表示设置成功         */
    HI_U32 reserved:31;    /**<保留*/
}HI_MDM_NM_SNID_SET_IND_S;

/**
 *@ingroup hct_mac
 * 获取站点静态代理
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**<    指定静态中继MAC地址\n
                                                                                        全部为0表示没有指定静态中继地址   */
    HI_U8 reserved[2];          /**<保留*/
}HI_MDM_NM_STATIC_PROXY_QRY_IND_S;

/**
 *@ingroup hct_mac
 * 设置站点静态代理控制结构体
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**< 指定静态中继MAC地址\n
                                                                                 全部为0表示没有指定静态中继地址 */
    HI_U8 ucOption;     /**< 选项设置，取值如下：\n
                                         #HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE(1)表示静态中继不存储到FLASH，断电重启失效（默认值）\n
                                         注意：断电重启后仍然生效，若中继模块位置被移动、替换等，可能造成该站点无法入网 */
    HI_U8 reserved;    /**<保留*/
}HI_MDM_NM_STATIC_PROXY_SET_REQ_S;

/**
 *@ingroup hct_mac
 * 设置站点静态代理信息结构体
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**< 指定静态中继MAC地址\n
                                                 全部为0表示解锁静态代理 */
    HI_U8 ucOption;     /**<  选项设置，取值如下：\n
                         #HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE(1)表示静态中继不存储到FLASH，断电重启失效（默认值）\n
                         注意：断电重启后仍然生效，若中继模块位置被移动、替换等，可能造成该站点无法入网 */
    HI_U8 reserved;
    HI_U32 ulErr;       /**<  设置是否成功，HI_ERR_SUCCESS表示成功。 */
}HI_MDM_NM_STATIC_PROXY_SET_IND_S;

/**
 *@ingroup hct_mac
 * 获取白名单开关结构体
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch;     /**< 当前白名单开关状态 */
    HI_U8 mode;                      /**< 白名单的模式 */
    HI_U8 reserved[2];              /**<保留*/
}HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S;

/**
 *@ingroup hct_mac
 * 设置白名单请求结构体
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch;     /**<设置的白名单开关状态    */
    HI_U8 mode;                             /**<白名单的模式        */
    HI_U8 reserved[2];             /**<保留*/
}HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S;

/**
 *@ingroup hct_mac
 * 设置白名单信息结构体
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch; /**<当前白名单开关状态 */
    HI_U8 mode;                 /**<白名单的模式 */
    HI_U8 reserved[2];          /**<保留*/

    HI_U32 ulErr;               /**<设置是否成功，HI_ERR_SUCCESS表示成功。 */
}HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S;

/**
 *@ingroup hct_mac
 * 搜网模式定义
 */
typedef enum
{
    HI_NDC_CHL_ACCESS_MODE_CCO = 0,             /**<表示CCO搜网接入模式，即仅搜索并连接CCO（上电后抄控器默认连接网络的模式）*/
    HI_NDC_CHL_ACCESS_MODE_STA,                   /**<表示STA搜网接入模式，仅搜索并连接STA*/
    HI_NDC_CHL_ACCESS_MODE_AUTO                 /**<表示不指定模式*/
}HI_NDC_CHL_ACCESS_MODE_E;

/**
 *@ingroup hct_mac
 * 搜网的命令定义
 */
typedef enum
{
    HI_NDC_CHL_ACTION_SEARCH = 0,           /**< 表示NDC仅搜索网络，但不连接网络（CCO模式下上电后NDC会自动搜网）*/
    HI_NDC_CHL_ACTION_SEARCH_QRY,         /**< 表示搜网过程中查询网络信息（包含搜网模式、MAC地址、IP地址和设备名称等）*/
    HI_NDC_CHL_ACTION_CONNECT,               /**< 表示选择网络*/
    HI_NDC_CHL_ACTION_DISCONNECT,          /**< 表示断开网络*/
    HI_NDC_CHL_ACTION_CLOSE                     /**< 表示关闭搜网*/
}HI_NDC_CHL_CONNECT_ACTION_E;

/**
 *@ingroup hct_mac
 * 搜网请求参数
 */
typedef struct
{
    HI_NDC_CHL_ACCESS_MODE_E enAccessMode;    /**< 搜网模式*/

    HI_U8       ucFreq;                                            /**< 搜网频段*/
    HI_U8       ucAction;                                          /**<参见HI_NDC_CHL_CONNECT_ACTION_E定义*/
    HI_U8       ucMac[HI_PLC_MAC_ADDR_LEN];         /**< MAC地址*/

    HI_U32     ulSnid;                                               /**< 网络标识符*/
} HI_ND_NETWORK_SEARCH_CONN_REQ_S;

/**
 *@ingroup hct_mac
 * 搜网请求参数
 */
typedef struct
{
    HI_ND_NETWORK_SEARCH_CONN_REQ_S stReq;  /**<参见#HI_ND_NETWORK_SEARCH_CONN_REQ_S定义*/
} HI_MDM_NET_SEARCH_CONN_REQ_S;

/**
 *@ingroup hct_mac
 * 搜网结果上报中的单个站点信息
 */
typedef struct
{
    HI_U8       bConnect:1;
    HI_U8       ucFreq:5;                           /**< 频段*/
    HI_U8       ucProdTypeStaType:1;        /**< #HI_FTM_PRODUCT_TYPE_STA_MI_M     #HI_FTM_PRODUCT_TYPE_STA_MI_I  */
    HI_U8       pad:1;                                       /**<保留*/
    HI_U8       rsvd;                      /**<保留*/
    HI_U8       aucMac[HI_PLC_MAC_ADDR_LEN];        /**<站点的MAC地址*/

    HI_U32     ulSnid;                             /**<网络表示符 */

    HI_U32      ulDiscoveryCnt;                    /**<搜查到该通信模块的次数*/

    HI_U8       aucIpv4[HI_IPV4_ADDR_LEN];          /**<网络节点的PLC的IP地址*/
    HI_U8       aucDevId[HI_MDM_DEV_ID_LEN];        /**<网络节点的设备标识*/
    HI_U8       ucChannelInfo;                      /**<信道质量信息*/
    HI_U8       ucNdcVer;                           /**<NDC通道版本号*/
    HI_U8       ucProdType;                         /**<产品版本 -- #HI_FTM_PRODUCT_TYPE_E*/
    HI_U8       reserved[3];                          /**<保留*/
} HI_ND_MULTINETWORK_INFO_S;

/**
 *@ingroup hct_mac
 * 搜网结果上报
 */
typedef struct
{
    HI_NDC_CHL_ACCESS_MODE_E enAccessMode;      /**<表示抄控器通信模块接入PLC网络的模式*/

    HI_U8  ucStatus :3;                             /**<表示NDC当前状态*/
    HI_U8  ucIndType:3;                             /**<上报类型，部分上报还是全部上报，或者是状态更新*/
    HI_U8  reserved0:2;                             /**<保留*/
    HI_U8  ucCurrentFreq;                           /**<表示当前工作的网络频段*/
    HI_U8  ucNodeNum;                              /**<表示搜索到的多网络个数*/
    HI_U8  pad;                                    /**<保留*/

    HI_U32  ulCurrentSnid;                          /**<表示当前工作的网络号*/

    HI_U32 ulErrCode;                               /**<表示搜网/连网的错误码*/
    HI_U32 ulDuration;                              /**<搜索网络持续时长，单位秒。*/

    HI_ND_MULTINETWORK_INFO_S stNode[0];            /**<网络参数，仅当ucNodeNum不为0有效*/
} HI_ND_NETWORK_SEARCH_CONN_IND_S;

/**
 *@ingroup hct_mac
 * 搜网结果上报参数
 */
typedef struct
{
   HI_ND_NETWORK_SEARCH_CONN_IND_S stInd;   /**<参见#HI_ND_NETWORK_SEARCH_CONN_IND_S定义*/
} HI_MDM_NET_SEARCH_CONN_IND_S;

/**
 *@ingroup hct_mac
 * 指示灯周期闪烁的最大指示灯个数
 */
#define LED_MAX_FLASH_NUM             3

/**
 *@ingroup hct_mac
 * 指示灯闪烁入参
 */
typedef struct
{
    HI_U8 light_on_cnt;                 /**<周期闪烁在多少个duration后，点亮指示灯*/
    HI_U8 light_off_cnt;                /**< 周期闪烁在多少个duration后，点灭指示灯*/
    HI_U16 duration;                    /**< 周期闪烁最小单位时间，ms*/

    HI_U8 led_num;                      /**< 周期闪烁的指示灯个数，最大为#LED_MAX_FLASH_NUM个*/
    HI_U8 led_id[LED_MAX_FLASH_NUM];    /**< 周期闪烁的指示灯ID,参见#HI_LED_ID_E*/
}HI_MDM_LED_FLASH_PARA_S;


/**
* @ingroup  hct_mac
* @brief  通过宽带PLC快速发送完成后通知APP的回调函数类型
*
* @par  描述:
* 通过宽带PLC快速发送完成后通知APP的回调函数类型
* @attention 作用产品类型为CCO、STA，仅适用于V100频段。
* @param  ulResult [OUT] 类型 #HI_U32，快速发送的结果，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID(*HI_MAC_BBPLCSENDRESULTFUN)(HI_U32 ulResult);

/**
 *@ingroup hct_mac
 * 快速发送模式定义
 */

typedef enum
{
    HI_MAC_FAST_SEND_BACKOFF,
    HI_MAC_FAST_SEND_PREEMPT,
    HI_MAC_FAST_SEND_MAX
}HI_MDM_FAST_SEND_MODE_E;

/**
 *@ingroup hct_mac
 * 基于宽带PLC快速发送的入参
 */
typedef struct
{
    HI_U8   SendPhase;              /**<发送相位*/
    HI_U8   SendFreq;               /**<发送频段*/
    HI_U8   Tmi;                    /**<分集拷贝模式*/
    HI_U8   ExtTmi;                 /**<扩展分集拷贝模式*/

    HI_U8   ResendCnt;              /**<重传次数*/
    HI_BOOL AllPhaseSendFlag;       /**<三相同时发送标志,如果为TRUE表示同时在三相发进行发送，FLASE则只在单相发送*/
    HI_U8   rsvd[2];                /**<保留*/

    HI_U32  Timeout;                /**<超时时间,单位ms*/

    HI_U16  DataLength;             /**<发送数据度*/
    HI_U8   Odmac[6];               /**<指定源始目的mac*/

    HI_BOOL IsMacEncode;            /**<数据是否已经封装成mac层格式，如果为TRUE，数据中包括FC、pb*/
    HI_U8   PbNum;                  /**<PB块个数*/
    HI_U16  PbLen;                  /**<PB块长度*/

    HI_U8   SendMode;               /**<快速发送模式，1表示抢占发送，0表示退避发送*/
    HI_U8   BroadcastFlag;          /**<广播标志，1表示广播，0表示非广播*/
    HI_U16  DataIndex;              /**<发送报文的索引*/


    HI_MAC_BBPLCSENDRESULTFUN BbPlcSendResultFun;  /**<发送结果上报回调函数*/

    HI_U8  Data[0];              /**<发送数据*/
}HI_MDM_BBPLC_SEND_PARA_S;


/**
* @ingroup  hct_mac
* @brief  通过单载波快速发送完成后通知APP的回调函数类型
*
* @par  描述:
* 通过单载波快速发送完成后通知APP的回调函数类型
* @attention 作用产品类型为CCO、STA，仅适用于V100频段。
* @param  ulResult [OUT] 类型 #HI_U32，快速发送的结果，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID(*HI_MAC_SCSENDRESULTFUN)(HI_U32 ulResult);

/**
 *@ingroup hct_mac
 * 基于单载波快速发送的入参
 */
typedef struct
{
    HI_U8 SendPhase;                               /**<发送相位*/
    HI_U8 DataLength;                              /**<报文长度*/
    HI_U8 SendMode;                                /**<快速发送模式，1表示抢占发送，0表示退避发送*/
    HI_U8 ResendNum;                               /**<重复发送次数*/

    HI_U16 DataIndex;                              /**<发送报文索引*/
    HI_U8 Odmac[6];                                /**<指定源始目的mac*/

    HI_U32 Timeout;                                /**<超时时间,单位ms*/

    HI_BOOL AllPhaseSendFlag;                      /**<三相同时发送标志,如果为TRUE表示同时在三相发进行发送，FLASE则只在单相发送*/
    HI_U8   rsvd[3];

    HI_MAC_SCSENDRESULTFUN ScSendResultFun;        /**<发送结果回调函数*/
    HI_U8 Data[0];                                 /**<报文缓存地址*/
}HI_MAC_SC_SEND_PARA_S;

/**
 *@ingroup hct_mac
 * 上报接收到的快速发送数据入参
 */
typedef struct
{
    HI_U8 *DataAddr;                /**<接收到报文指针*/

    HI_U16 DataIndex;               /**<接收到报文索引*/
    HI_U16 DataLength;              /**<接收到报文长度*/

    HI_U8 Odmac[6];                 /**<目的mac 地址*/
    HI_BOOL IsMacDecode;            /**<上送数据是否已经剥去mac层封装只上报数据，TRUE表示剥去*/
    HI_U8 Rsvd;                     /**<保留*/
}HI_MAC_REPORT_RCV_FAST_SEND_PKT_S;

/**
 *@ingroup hct_mac
 * 设置单载波发送速率的入参
 */
typedef struct
{
    HI_U8  FecRate;             /**<码率*/
    HI_U8  ModulationMode;      /**<调制模式*/
    HI_U16 FsFreq;              /**<符号率*/
    HI_U16 MaxSendLength;       /**<充许发送最大的报文长度*/
    HI_BOOL ScEnable;            /**<单载波使能标志，HI_TRUE表示使能，HI_FALSE表示去使能*/
    HI_U8  rsvd;
}HI_MAC_SC_PARA_S;

/**
* @ingroup  hct_mac
* @brief  接收快速发送的数据后通知APP的回调函数类型
*
* @par  描述:
* 通过单载波快速发送完成后通知APP的回调函数类型
* @attention 作用产品类型为CCO、STA，仅适用于V100频段。
* @param  pReportData [OUT] 类型 #HI_MAC_REPORT_RCV_FAST_SEND_PKT_S，返回数据信息的指针。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnFastSendCallback)(HI_MAC_REPORT_RCV_FAST_SEND_PKT_S * pReportData);


 /**
 * @ingroup hct_mac
 * 站点属性信息
 */
 typedef struct
 {
     HI_U8    MyLevel;                       /**<本站点层级*/
     HI_U8    MyRole;                        /**<本站点角色*/
     HI_U8    ProxyCommRate;                 /**<与代理站点的通信成功率*/
     HI_U8    rsvd;                          /**<保留*/

     HI_U16   MyTei;                         /**<本站点tei*/
     HI_U16   MyProxyTei;                    /**<本站点父代理*/

     HI_U32   MySnid;                        /**<本站点SNID*/

     HI_U8    CcoMac[HI_PLC_MAC_ADDR_LEN];   /**<CCO MAC 地址*/
     HI_U8    MyMac[HI_PLC_MAC_ADDR_LEN];    /**<本站点MAC地址*/

     HI_U8    ProductType;                   /**<APP设置的产品类型*/
     HI_U8    FinalSelectFreq;               /**<最后关联的频段*/
     HI_U16   ProtocolVer;                   /**<当前执行的协议版本*/

 } HI_MDM_NETWORK_ATTRIBUTE_S;


 /**
 * @ingroup hct_mac
 * 快速关闭通知当前状态
 */
typedef enum
{
    HI_MDM_FAST_NOTICE_FINISHED,    /**<处理完成/未启动*/
    HI_MDM_FAST_NOTICE_PROCESSING,  /**<正在处理*/
} HI_MDM_FAST_NOTICE_STATUS_E;

typedef struct
{
    HI_U16 usTotal;     /**<STA站点总数*/
    HI_U16 usSuccess;   /**<收到应答的STA站点数量*/
    HI_U16 usFailed;    /**<多次查询未应答，判定为失败的STA站点数量*/
    HI_U16 usProcess;   /**<正在处理的站点数量*/
    HI_U8  ucStatus;    /**<当前状态#HI_MDM_FAST_NOTICE_STATUS_E*/
    HI_U8  pad[3];      /**<预留*/
} HI_MDM_FAST_NOTICE_INFO_S;

/**
* @ingroup  hct_mac
* @brief  把快速关闭通知的结果通知APP的回调函数类型
*
* @par  描述:
* 把快速关闭通知的结果通知APP的回调函数类型
* @attention 作用产品类型为CCO，仅适用于V100频段。
* @param  pstInf [OUT] 类型 #HI_MDM_FAST_NOTICE_INFO_S，返回数据信息的指针。
* @param  pParam [IN] 类型 #HI_PVOID，用户自定义参数，注册时传入，回调时回传。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnCcoFastNoticeCallback)(HI_MDM_FAST_NOTICE_INFO_S *pstInf, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  接收快速关闭通知的数据后通知APP的回调函数类型
*
* @par  描述:
* 接收快速关闭通知的数据后通知APP的回调函数类型
* @attention 作用产品类型为STA，仅适用于V100频段。
* @param  ucData [OUT] 类型 #HI_U8，上报快速关闭指令
* @param  pParam [IN] 类型 #HI_PVOID，用户自定义参数，注册时传入，回调时回传。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnStaFastNoticeCallback)(HI_U8 ucData, HI_PVOID pParam);

/**
 *@ingroup hct_mac
 * 全相位
 */
#define PHASE_ALL     (0)
/**
 *@ingroup hct_mac
 * A相位
 */
#define PHASE_A       (1)
/**
 *@ingroup hct_mac
 * B相位
 */
#define PHASE_B       (2)
/**
 *@ingroup hct_mac
 * C相位
 */
#define PHASE_C       (3)
/**
 *@ingroup hct_mac
 * 空相位
 */
#define INVALID_PHASE   (4)
/**
 *@ingroup hct_mac
 * 相位个数
 */
#define PHASE_CNT      3
/**
 *@ingroup hct_mac
 * 零火线接线正确
 */
#define PN_CONNECTION_RIGHT     (0)
/**
 *@ingroup hct_mac
 * 零火线接线反接
 */
#define PN_CONNECTION_ERROR     (1)
/**
 *@ingroup hct_mac
 * 零火线接线情况未知
 */
#define INVALID_PN_CONNECTION   (2)
/**
 *@ingroup hct_mac
 * 工频过零边沿: 双沿
 */
#define POWER_EDGE_DOUBLE       (0)
 /**
  *@ingroup hct_mac
  * 工频过零边沿: 上升沿
  */
#define POWER_EDGE_RISE         (1)
 /**
  *@ingroup hct_mac
  * 工频过零边沿: 下降沿
  */
#define POWER_EDGE_FALL         (2)
 /**
  *@ingroup hct_mac
  * 工频过零边沿: 边沿未知
  */
#define POWER_EDGE_UNKNOWN      (3)
 /**
  *@ingroup hct_mac
  * 工频过零边沿个数
  */
#define POWER_EDGE_CNT          (2)

/**
* @ingroup hct_mac
 * STA产品类型: 非法
 */
#define STA_TYPE_INVALID (0)
/**
* @ingroup hct_mac
 * STA类型为电表
 */
#define STA_TYPE_METER   (1)
/**
* @ingroup hct_mac
 * STA类型为I型采集器
 */
#define STA_TYPE_CLTI    (2)

/**
* @ingroup hct_mac
 * 表示搜表结束
 */
#define LED_SEARCH_METER_END        1
/**
* @ingroup hct_mac
 * 表示搜表开始
 */
#define LED_SEARCH_METER_BEGIN      0
/**
* @ingroup hct_mac
 * 表示读表地址失败
 */
#define LED_READ_METER_FAIL         1
/**
* @ingroup hct_mac
 * 表示读表地址成功
 */
#define LED_READ_METER_SUCCESS      0

/**
* @ingroup hct_mac
 * 接收机扩展LED灯IONum 告警灯
 */
#define NDM_LED_RECVIVER_ALARM          (IO_EXP_IO_NUM_05)
/**
* @ingroup hct_mac
 * 发射机扩展LED灯IONum A相识别灯
 */
#define NDM_LED_TRANSMITTER_A           (IO_EXP_IO_NUM_00)
/**
* @ingroup hct_mac
 * 发射机扩展LED灯IONum B相识别灯
 */
#define NDM_LED_TRANSMITTER_B           (IO_EXP_IO_NUM_01)
/**
* @ingroup hct_mac
 * 发射机扩展LED灯IONum C相识别灯
 */
#define NDM_LED_TRANSMITTER_C           (IO_EXP_IO_NUM_05)
/**
* @ingroup hct_mac
 * 发射机扩展LED灯IONum 告警灯
 */
#define NDM_LED_TRANSMITTER_ALARM       (IO_EXP_IO_NUM_12)
/**
* @ingroup hct_mac
 * 接收机/发射机 公共扩展IO Wifi Enable IO
 */
#define NDM_IO_WIFI_ENABLE              (IO_EXP_IO_NUM_11)
/**
* @ingroup hct_mac
 * 接收机/发射机 公共扩展IO Beep IO
 */
#define NDM_IO_BEEP_ENABLE              (IO_EXP_IO_NUM_14)

/**
* @ingroup hct_mac
 * 接收机/发射机 内部IO 数据收发灯
 */
#define NDM_LED_DATA_RECV_SEND          (HI_LED_IDX_3)

/**
* @ingroup hct_mac
 * 网络参数
 */
typedef struct
{
    HI_U8 bProxy:1;       /**<当前站点是否为代理*/
    HI_U8 bCco:1;         /**<当前站点是否为头端*/
    HI_U8 ucLayer;        /**< 当前站点所在层级数*/
    HI_U8 ucChildLayer;   /**< 当前站点带子节点的层级数*/
    HI_U16 reserved;        /**< 保留*/
    HI_U16 usNodeNum;    /**< 当前站点的一级子节点个数，0表示没有子节点*/
    HI_U16 usTei;        /**< 当前模块的TEI*/
} HI_MAC_NET_PARAM_QRY_S;

/**
* @ingroup hct_mac
* 定义MAC地址大小最大条目数
*/
#define HI_MAC_VALID_MAC_LIST_MAX 2048

/**
* @ingroup hct_mac
* MAC地址条目结构
*/
typedef struct
{
    HI_U8  mac[6];                  /**<MAC地址*/
    HI_U8  pad[2];                   /**<保留*/
} HI_MAC_ENTRY_STRU;

/**
*@ingroup hct_mac
* 增加MAC类型
*/
typedef struct
{
    HI_U8 list_num;          /**<mac_list个数，取值范围1-32*/
    HI_U8 clear_flag;        /**<取值: #HI_TRUE 表示清除备份MAC地址列表*/
    HI_U8 type;               /**<mac类型，来自表地址:#HI_MDM_METER_MAC，来自NV:#HI_MDM_NV_MAC,来自采集器地址:#HI_MDM_CLT_MAC*/
    HI_U8 reserved;          /**<保留*/
    HI_MAC_ENTRY_STRU mac_list[0];      /**< MAC地址条目结构体*/
}HI_MAC_ATTR_STRU;

/**
* @ingroup hct_mac
* MAC地址条目
*/
typedef struct
{
   HI_U16 list_num;              /**<MAC地址条目*/
   HI_U16 pad;                   /**<保留*/
   HI_MAC_ENTRY_STRU  mac_list[HI_MAC_VALID_MAC_LIST_MAX];  /**<MAC地址条目结构*/
}HI_MAC_VALID_MAC_LIST_STRU;

/**
* @ingroup hct_mac
* 抄控器衰减模式
*/
typedef enum
{
    NORMAL_STATUS = 0,     /**<正常模式*/
    ATTENUATE_STATUS,      /**<衰减模式*/
}en_ndm_status;

 /**
 * @ingroup hct_mac
 * 白名单变化的原因码(HSO也在使用)
 */
typedef enum
{
    HI_MAC_WHIST_LIST_CHANGE_REASON_UNKNOWN = 0x00,                   /**< 原因未知       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_NV = 0x5,                                 /**< 修改NV项的NV       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_BASE_LIB,                       /**< 基础信息库发生变化     */
    HI_MAC_WHIST_LIST_TF_IDENTIFY,                                                      /**< 台区识别       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_SWITCH_MODE,              /**< HSO界面修改白名单的模式     */

    HI_MAC_WHIST_LIST_CHANGE_REASON_MAC_REQ = 0x1f,                    /**< MAC地址请求       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_INIT,                               /**< App初始化表档案       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_START_SEARCH,              /**<启动搜表 */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_STOP_SEARCH,                /**<停止搜表 */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_CON,                                /**<集中器变更表档案 */
    HI_MAC_WHIST_LIST_CHANGE_REASON_CLT_JOIN_NET,                       /**<未以表地址入网的采集器站点被拒绝入网后增加到白名单 */
    HI_MAC_WHIST_LIST_CHANGE_REASON_TEST_MODE,                          /**<送检版本，关闭白名单*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_TF_BEGIN,                   /**< 启动台区识别*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_TF_END,                     /**< 停止台区识别*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_MAX = 0x3f,                              /**<原因码最大值*/
}HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM;

/**
* @ingroup hct_mac
* APP业务状态定义
*/
typedef enum
{
    HI_MAC_BUSINESS_FREE = 0,                       /**<业务空闲状态*/
    HI_MAC_BUSINESS_BUSY,                           /**<业务繁忙状态*/
}HI_MAC_BUSINESS_STATE_ENUM;

/**
* @ingroup  hct_mac
* @brief  台区识别名单有刷新时通知APP的回调函数类型
*
* @par  描述:
* 台区识别名单有刷新时通知APP的回调函数类型
* @attention 作用产品类型为CCO。
* @param  state [IN] 类型 #HI_U32，台区名单刷新的状态，默认为#HI_MDM_TRANSFORMER_LIST_REFRESH_STATE_UNKNOWN。
* @retval   ret 类型HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN)(HI_U32 state);

/**
* @ingroup  hct_mac
* @brief  相位识别名单有刷新时通知APP的回调函数类型
*
* @par  描述:
* 相位识别名单有刷新时通知APP的回调函数类型
* @attention 作用产品类型为CCO。
* @param  state [IN] 类型 HI_U32，相位识别名单刷新的状态，默认为#HI_MDM_PHASE_LIST_REFRESH_STATE_UNKNOWN。
* @retval   ret 类型HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_REPORT_PHASE_LIST_REFRESH_FUN)(HI_U32 state);

/**
* @ingroup hct_mac
 * 台区识别结果中站点信息
 */
typedef struct
{
    HI_U16 tei;                         /**< 站点的TEI, 不在线的站点该值为0        */
    HI_U8  is_online    : 1;         /**< 站点是否在线，0: 不在线，1: 在线       */
    HI_U8  pad1         : 3;         /**< 保留       */
    HI_U8  result       : 4;          /**< 台区识别结果，0: 未知，1: 本台区站点，2: 非本台区站点，3: 该站点不支持台区识别     */
    HI_U8  pad[3];                    /**< 保留       */

    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];     /**< 站点的MAC地址       */
}HI_MDM_TRANSFORMER_LIST_ENTRY_S;

/**
* @ingroup hct_mac
 * 台区识别结果
 */
typedef struct
{
    HI_U16 num;                                      /**< 上报结果中包含的站点数        */
    HI_U16 pad;                                      /**< 保留         */
    HI_MDM_TRANSFORMER_LIST_ENTRY_S *pEntry;        /**< 上报站点信息条目     */
}HI_MDM_TRANSFORMER_LIST_S;

/**
* @ingroup hct_mac
 * 黑名单中某个站点的信息
 */
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];               /**< 站点的mac 地址       */
    HI_U8  mode;                                                /**< 存储类型，保留     */
    HI_U8  pad;                                                  /**< 保留     */
}HI_MDM_BLACK_LIST_ENTRY_S;

/**
* @ingroup hct_mac
 * 用于定义黑名单结果
 */
typedef struct
{
    HI_U16 num;                                     /**< 上报站点的mac 地址条目 数        */
    HI_U16 pad;                                    /**< 保留         */
    HI_MDM_BLACK_LIST_ENTRY_S *pEntry;         /**< 上报站点的mac 地址条目         */
}HI_MDM_BLACK_LIST_S;

/**
* @ingroup hct_mac
 * 启动搜表的接口
 */
typedef struct
{
    HI_U32 time;            /**<台区识别的持续时间，单位:分钟*/
}HI_MDM_TF_START_SEARCH_METER_S;

/**
* @ingroup hct_mac
 * 定义报文的解析状态，当前用于测试系统
 */
typedef enum
{
    PBB_CORRECT_TYPE,          /**<接收到的PB块正确*/
    PBB_ERROR_TYPE,             /**<接收PB块错误*/
    PBB_NOTHING_TYPE,         /**<报文只有FC没有PB块*/
    PBB_MAX_TYPE              /**<最大值*/
}HI_MDM_CAPTURE_PKT_PBB_TYPE_EN;

/**
* @ingroup hct_mac
 * 用于定义使能
 */
#define HI_MDM_ENABLE  1
/**
* @ingroup hct_mac
 * 用于定义去使能
 */
#define HI_MDM_DISABLE 0
/**
* @ingroup hct_mac
 * 用于定义组网状态
 */
typedef enum
{
    FORMING_STATUS = 1,  /**< 组网中        */
    EVALUATE_STAUS,         /**< 优化中        */
    EVALUATE_END,             /**< 组网及优化完成        */
}HI_MDM_NETWORK_STATUS_E;

/**
* @ingroup  hct_mac
* @brief  抓包函数回调函数
* @par  描述:
* 用于捕捉模块接收到的报文
* @attention 无。
* @param  ucPayloadType [IN] 类型 #HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ,上报payload的类型，具体参见#HI_MDM_CAPTURE_PKT_PBB_TYPE_EN定义。
* @param  pucFcData [IN] 类型 #HI_U8 *，用于存放上报FC，要求指针指向的空间不小于16字节。
* @param  pucPayload [IN] 类型 #HI_U8 *，用于存放上报的payload，要求指针指向的空间不小520字节。
*
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 表示加入app队列成功, #HI_ERR_QUEUE_FULL表示队里已满。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_POWER_FAILURE_REPORT_ST 定义。
* @since HiMDL_V200R001C00
*/
typedef  HI_U32 (*HI_MDM_CAPTURE_FUNCTION)(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData);

/**
* @ingroup hct_mac
* 超级电容STA模块插拔检测管脚
*/
#define POWER_FAILURE_PLUG_MODULE_GPIO  HI_HW_GPIO26_BACKUP
/**
* @ingroup hct_mac
* 超级电容STA模块升压电路控制管脚
*/
#define POWER_FAILURE_ELEVATE_POWER_GPIO  HI_HW_GPIO28_BACKUP

/**
* @ingroup hct_mac
* 超级电容STA模块电压跌落中断检测管脚
*/
#define POWER_FAILURE_POWER_DOWN_GPIO   HI_HW_GPIO29_BACKUP

/**
 *@ingroup hct_mac
 * 停电上报事件mac上报至app的内容
 */
typedef struct
{
    HI_U16 my_tei;                          /**< 本站点TEI                 */
    HI_U8 device_power_failure  : 1;           /**< 设备停电标记, 设备停电为HI_TRUE,未停电为HI_FALSE */
    HI_U8 device_not_pull_reset : 1;          /**< 设备拉动管脚标记,设备停电为HI_TRUE,未停电为HI_FALSE */
    HI_U8 is_plug_module : 1;          /**< 模块拔出标记拔出为HI_TRUE,未拔出为HI_FALSE */
	HI_U8 pad			 : 5;
}HI_MDM_POWER_FAILURE_REPORT_ST;
/**
 *@ingroup hct_mac
 * 相位过零消失上报事件mac上报至app的内容
 */
typedef struct
{
    HI_U8 phase[PHASE_CNT];             /**< 过零信号变化的相位, 值为HI_TRUE说明状态发生变化     */
    HI_U8 pad;

    HI_U32 time_stamp;                      /**< 过零信号变化的时间                 */
}HI_MDM_PHASE_POWER_FAILURE_REPORT_ST;

/**
 *@ingroup hct_mac
 * 停电上报功能开启
 */
#define IS_DETECT_POWER_FAILURE         1

/**
 *@ingroup hct_mac
 * STA端停电上报事件类型
 */
typedef enum
{
	HI_MDM_POWER_FAILURE_EVENT_LOCAL = 0,          /**< STA本地停电上报事件             */
	HI_MDM_POWER_FAILURE_EVNET_POWERON,            /**< STA停电恢复重启事件             */
    HI_MDM_POWER_ON_EVNET_REPORT,                  /**< STA本地复电上报事件             */
	HI_MDM_POWER_FAILURE_EVNET_PLUG_MODULE,        /**< STA模块拔出事件             */
	HI_MDM_POWER_FAILURE_EVNET_NOT_POWER_DOWN,     /**< STA模块过零信号消失电压未跌落事件             */
} HI_MDM_POWER_FAILURE_EVENT_TYPE_E;


/**
 *@ingroup hct_mac
 * CCO端停电上报事件类型
 */
typedef enum
{
	HI_MDM_CCO_POWER_FAILURE_EVENT_LOCAL = 0,               /**< CCO本地停电上报事件             */
	HI_MDM_CCO_POWER_FAILURE_EVNET_POWERON,                 /**< CCO停电恢复重启事件             */
	HI_MDM_CCO_PHASE_POWER_FAILURE_EVNET,                   /**< CCO相位过零信号消失事件     */
    HI_MDM_CCO_PHASE_POWER_ON_EVNET,                        /**< CCO相位过零信号恢复事件     */
} HI_MDM_CCO_POWER_FAILURE_EVENT_TYPE_E;

/**
 *@ingroup hct_mac
 * 单三相模块电压跌落查询结果
 */
typedef enum
{
	HI_MDM_TSTA_POWER_DOWN_CHECK_INIT = 0 ,						/**< 单三相模块电压跌落查询初始状态 */
	HI_MDM_TSTA_POWER_DOWN_CHECK_TIMEOUT ,                  /**< 单三相模块电压跌落查询超时            */
	HI_MDM_TSTA_POWER_DOWN_CHECK_POWER_DOWN,                /**<单三相模块电压跌落查询电压跌落  */
	HI_MDM_TSTA_POWER_DOWN_CHECK_NORMAL,                   /**< 单三相模块电压跌落查询电压正常   */
} HI_MDM_TSTA_POWER_DOWN_EVENT_TYPE_E;

/**
* @ingroup  hct_mac
* @brief  停电上报功能的回调函数
* @par  描述:
* 本API接口使用场景：连续三个工频周期没有检测到过零信号(强电环境下)且电压跌落到9.5V以下，判断为非插拔模块时，上报停电事件给APP
* @attention 无。
* @param  ucType [IN] 类型 #HI_U8，事件类型。STA模块 参看 #HI_MDM_POWER_FAILURE_EVENT_TYPE_E。\n
*													CCO模块参看 #HI_MDM_CCO_POWER_FAILURE_EVENT_TYPE_E
* @param  pCmdData [IN] 类型 #HI_MDM_POWER_FAILURE_REPORT_ST *，数据指针。
* @param  ulDataSize [IN] 类型 #HI_U32，数据大小。
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_POWER_FAILURE_REPORT_ST 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PowerFailureEventCallback)(HI_U8 ucType,HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData,HI_U32 ulDataSize);
/**
* @ingroup  hct_mac
* @brief  相位过零消失的回调函数
* @par  描述:
* 本API接口使用场景：连续n工频周期没有检测到过零信号，判断为相位过零消失，上报到app，n可以配置，默认三个。
* @attention 无。
* @param  ucType [IN] 类型 #HI_U8，事件类型。参看 #HI_MDM_POWER_FAILURE_EVENT_TYPE_E。
* @param  pCmdData [IN] 类型 #HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *，数据指针。
* @param  ulDataSize [IN] 类型 #HI_U32，数据大小。
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_PHASE_POWER_FAILURE_REPORT_ST 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PhasePowerFailureEventCallback)(HI_U8 ucType,HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *pCmdData,HI_U32 ulDataSize);

/**
 *@ingroup hct_mac
 * 停电上报事件回调函数封装结构体
 */
typedef struct
{
    HI_MDM_PowerFailureEventCallback powerfailure_callback;              /**< 停电上报事件回调函数             */
    HI_MDM_PhasePowerFailureEventCallback phasePowerFailureCallback;     /**< 相位停电上报事件回调函数         */
}HI_MDM_MAC_POWER_FAILURE_ST;

/**
 *@ingroup hct_mac
 * 接收位图大小
 */
#define POWER_FAILURE_BITMAP_LEN    256

/**
* @ingroup hct_mac
 * 停电上报事件维测信息
 */
typedef struct
{
    HI_U32 rcv_pwf_frame_time;                                           /**< 接收到停电上报帧的时间       */

    HI_U8 cco_statistic_pwf_bitmap[POWER_FAILURE_BITMAP_LEN];           /**< cco统计的bitmap      */

    HI_U16 pwf_tei_cnt;                                                  /**< 停电tei数目      */
    HI_U16 pad;
}CCO_POWER_FAILURE_STATISTIC_S;

/**
* @ingroup hct_mac
 * 网络锁定信息
 */
typedef struct
{
    HI_U8  lock_status;                             /**< 锁定状态 \n
                                                        V100未能锁定状态: #HI_MDM_UNLOCK_NETWORK_STATUS \n
                                                        锁定MAC状态:  #HI_MDM_UNLOCK_NETWORK_STATUS \n
                                                        锁定频段snid状态: #HI_MDM_LOCK_FREQSNID_STATUS \n
                                                        */
    HI_U8  pad1;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    /**< 锁定网络的CCO MAC地址        */

    HI_U32 lock_freq : 8;                           /**< 锁定频段        */
    HI_U32 lock_snid : 24;                          /**< 锁定SNID        */

    HI_U32 lock_begin_time;                      /**< 锁定开始时间，单位s        */
    HI_U32 lock_duration;                          /**< 锁定时长，单位s        */
    HI_U32 leave_time_len;                       /**< 从锁定开始的离线时长，单位s        */
} HI_MDM_LOCK_INFO_STRU;

/**
 *@ingroup hct_mac
 * 邻居网络信息结构
 */
typedef struct
{
    HI_U32   ulSnid;                /**< 邻居网络网络号             */

    HI_U8   ucFreq;                 /**< 邻居网络频段              */
    HI_BOOL bIsSingleComm;          /**< 是否为单播通信            */
    HI_U8  pad;                     /**< 保留位                    */

    HI_U32  ulBandWidth;            /**< 邻居网络带宽              */
} HI_MDM_NEIGHBOR_NET_ITEM_S;

/**
 *@ingroup hct_mac
 * 邻居网络最大网络数
 */
#define HI_MAX_NEIGHBOR_NET_TOTAL_NUM    30


/**
 *@ingroup hct_mac
 * 邻居网络查询结构
 */
typedef struct
{
    HI_U16 usNum;                                                   /**< 邻居网络数目             */
    HI_U16 pad;                                                     /**< 保留位                   */
    HI_MDM_NEIGHBOR_NET_ITEM_S astItem[HI_MAX_NEIGHBOR_NET_TOTAL_NUM];    /**< 邻居网络信息数组         */
} HI_MDM_NEIGHBOR_NET_INFO_S;

/**
* @ingroup  hct_mac
* @brief  查询STA不入网原因的回调函数类型
*
* @par  描述:
* 查询STA不入网原因的回调函数类型
* @attention 作用产品类型为STA。
* @param  staNotJoinReason [IN] 类型 HI_U8，STA不入网原因，参见#HI_MDM_STA_JOIN_NET_STATUS_E。
* @retval   ret 类型HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_StaNotJoinReasonCallback)(HI_U8 staNotJoinReason);

/**
 *@ingroup hct_mac
 * 设置离线STA结构
 */
typedef struct
{
    HI_U16 usCount;                 /**< 设置离线STA数目，个数小于等于1016             */
    HI_U8 reserved[2];              /**< 保留                    */
    HI_U16 ausTeiList[0];           /**< 设置离线STA信息             */
}HI_MDM_TEI_LIST_S;

/**
 *@ingroup hct_mac
 * STA状态枚举
 */
typedef enum
{
    HI_MDM_STA_JOIN_NET_STATUS_NOT_FIND_CCO = 0, /**< STA未检测到CCO不入网,该状态不主动上报*/
    HI_MDM_STA_JOIN_NET_STATUS_NOT_JOIN_NET,     /**< STA未加入网络                */
    HI_MDM_STA_JOIN_NET_STATUS_REFUSED,          /**< STA被拒绝加入网络            */
    HI_MDM_STA_JOIN_NET_STATUS_ONLINE = 3,       /**< STA加入网络                  */
}HI_MDM_STA_JOIN_NET_STATUS_E;

/**
*@ingroup hct_mac
* STA管理ID信息
*/
typedef struct
{
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];   /**< STA的MAC地址                  */
    HI_U8  pad[2];                            /**< 保留                  */
    HI_U8  aucStaId[24];                      /**< STA的管理ID                  */
}HI_MDM_STA_ID_S;

/**
*@ingroup hct_mac
* 查询STA管理ID记录
*/
typedef struct
{
    HI_U32 num;                                                /**< STA的个数                  */
    HI_MDM_STA_ID_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];      /**< STA的管理ID列表                  */
}HI_MDM_NETWORK_STA_ID_S;

/**
*@ingroup hct_mac
* 收发指示灯配置模式
*/

typedef enum
{
    HI_MDM_SET_LED_DISABLE_MODE = 0,                /**< 收发报文时，不配置收发指示灯*/
    HI_MDM_SET_MAC_LED_MODE,                        /**< 收发MAC报文时，配置收发指示灯*/
    HI_MDM_SET_APP_LED_MODE,                        /**< 收发APP报文时，配置收发指示灯*/
    HI_MDM_SET_IP_LED_MODE,                         /**< 收发IP报文时，配置收发指示灯*/
    HI_MDM_SET_APP_IP_LED_MODE,                     /**< 收发APP，IP报文时，配置收发指示灯*/
}HI_MDM_SET_TX_RX_LED_MODE_E;

/**
* @ingroup  hct_mac
* @brief  停电时获取电表管脚拉动标记位
* @par  描述:
* 停电时获取电表管脚拉动标记位，模块停电后不拉动reset管脚，避免模块重启
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U32，\n
                                      @li  #HI_FALSE   电表拉动reset管脚
                                      @li  #HI_TRUE    电表不拉动reset管脚
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_BOOL HI_MDM_GetDevicePullFlag(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  配置连续n个工频周期无过零信号判定停电，n的值。
* @par  描述:
* 停电时n个工频周期无过零信号判定停电
* @attention 无。
* @param  no_zero_cross_cnt [IN] 类型 #HI_U8
* @retval   ret 类型 #HI_U32 #HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerFailureNoZeroCrossCnt(HI_U8 no_zero_cross_cnt);

/**
* @ingroup  hct_mac
* @brief  获取连续n个工频周期无过零信号判定停电，n的值。
* @par  描述:
* 停电时n个工频周期无过零信号判定停电
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U8
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPowerFailureNoZeroCrossCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置停电检测功能是否为单三相模块。系统默认为非单三相模块
* @par  描述:
*  设置停电检测功能是否为单三相模块
* @attention  无
* @param  is_single_tsta [IN] 类型 #HI_BOOL，设置是否为单三相模块\n
* #HI_TRUE 是单三相模块, # HI_FALSE 非单三相模块 。
* @retval  无
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsSingleTsta(hi_bool is_single_tsta);

/**
* @ingroup  hct_mac
* @brief  APP 设置停电检测功电压跌落检测结果。在单三相模块中调用。
* @par  描述:
* APP 设置停电检测功电压跌落检测结果。
* @attention  不支持非单相过零的三相表 模块使用
* @param  power_down_check_result [IN] 类型 #power_down_check_result，APP 判定电压跌落结果\n
* 参考#HI_MDM_TSTA_POWER_DOWN_EVENT_TYPE_E。
* @retval  无
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetSingleTstaPowerDownCheckResult(hi_u8 power_down_check_result);

/**
* @ingroup  hct_mac
* @brief  停电上报事件app注册接口
* @par  描述:
* 停电上报事件app注册接口
* @attention 无。
* @param    pfnCallback [IN] 类型 #HI_MDM_PowerFailureEventCallback， 停电上报功能的回调函数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_pfnPowerFailureEventCallback 回调函数。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterPowerFailureEvent(HI_MDM_PowerFailureEventCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  相位过零信号变化事件app注册接口
* @par  描述:
* 相位过零信号变化事件app注册接口
* @attention 无。
* @param    phasePfCallback [IN] 类型 #HI_MDM_PhasePowerFailureEventCallback，  相位过零信号变化的回调函数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_PhasePowerFailureEventCallback 回调函数。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterPhasePowerFailureEvent(HI_MDM_PhasePowerFailureEventCallback phasePfCallback);

typedef struct
{
    HI_U8 ucStaNum;         /**< 待探测站点个数*/
    HI_U8 pad[3];

    HI_U8 aucMacList[0];    /**< MAC地址列表*/
} HI_MDM_POWEROFF_DETECT_MAC_LIST_S;
/**
* @ingroup  hct_mac
* @brief  帮助邻居节点应答停电探测
* @par  描述:
* 帮助邻居节点应答停电探测
* @attention 无。
* @param    pstMacList [IN] 类型 #HI_MDM_POWEROFF_DETECT_MAC_LIST_S， 待探测的站点MAC地址列表。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示执行成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_POWEROFF_DETECT_MAC_LIST_S 待探测站点MAC地址列表
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_HelpNeighborAnswerDetect(HI_IN HI_MDM_POWEROFF_DETECT_MAC_LIST_S * pstMacList);
/**
* @ingroup  hct_mac
* @brief 获取电压跌落中断计数接口
* @par  描述:
* 获取电压跌落中断计数接口
* @attention 超级电容电未完全释放完时，此时上电，会产生一次跌落中断。
* @param   无
* @retval  HI_U8
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U8 HI_MDM_VolDeclineCnt(HI_VOID);


/**
* @ingroup  hct_mac
* @brief  停电事件上报功能是否开启
* @par  描述:
* 停电事件上报功能是否开启
* @attention 无。
* @param    PowerFailureReportEnable [IN] 类型 #HI_U8 *，停电事件上报功能开关，#IS_DETECT_POWER_FAILURE 功能打开，其他值功能关闭。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示停电上报功能开启。#HI_ERR_INVALID_PARAMETER 表示入参错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_PowerFailurReportEnable(HI_U8 *PowerFailureReportEnable);

/**
* @ingroup  hct_mac
* @brief  停电事件上报获取CCO发送到集中器的信息
*
* @par  描述:
* 停电事件上报获取CCO发送到集中器的信息
* @attention 作用产品类型为CCO。
* @param    PowerFailureDfxInfo [IN] 类型 #CCO_POWER_FAILURE_STATISTIC_S *，停电上报事件维测信息。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示获取信息成功。#HI_ERR_INVALID_PARAMETER 表示入参错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see CCO_POWER_FAILURE_STATISTIC_S
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_PowerFailurCcoDfxInfo(CCO_POWER_FAILURE_STATISTIC_S *PowerFailureDfxInfo);

/**
* @ingroup  hct_mac
* @brief  查询通信模块的PLC的MAC地址和IP地址
 * @par  描述:
* 该函数负责查询通信模块的PLC的MAC地址和IP地址
* @attention 无。
* @param    aucMacAddr[HI_PLC_MAC_ADDR_LEN] [OUT] 类型 #HI_U8 通信模块的PLC MAC地址 长度为 #HI_PLC_MAC_ADDR_LEN。
* @param    aucIpAddr[HI_IPV4_ADDR_LEN] [OUT] 类型 #HI_U8 通信模块的PLC IP地址 长度为 #HI_IPV4_ADDR_LEN。
* @retval   ret 类型 #HI_U32，HI_ERR_SUCCESS 表示接口执行成功，其他为错误码。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_GetAddr(HI_OUT HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  查询通信模块的PLC的MAC地址和IP地址和子网掩码
 * @par  描述:
* 该函数负责查询通信模块的PLC的MAC地址和IP地址和子网掩码
* @attention 无。
* @param    aucIpAddr[HI_IPV4_ADDR_LEN] [OUT] 类型 #HI_U8，通信模块的站点IP地址 长度为 #HI_IPV4_ADDR_LEN。
* @param    aucNetMask[HI_IPV4_ADDR_LEN] [OUT] 类型 #HI_U8，通信模块的站点子网掩码 长度为 #HI_IPV4_ADDR_LEN。
* @param    aucMacAddr[HI_PLC_MAC_ADDR_LEN] [OUT] 类型 #HI_U8，通信模块的站点MAC地址 长度为 #HI_PLC_MAC_ADDR_LEN。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示接口执行成功，其他为错误码。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_GetPlcEth(HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN], HI_U8 aucNetMask[HI_IPV4_ADDR_LEN], HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  供外部查询网络拓扑图，为同步调用接口，内存需外部传入。
 * @par  描述:
* 共外部查询网络拓扑图，为同步调用接口，内存需外部传入。
* @attention 作用产品类型为CCO。
* @param  pstQueryInfo [IN]/[OUT] 类型 #HI_MAC_CONFIG_INFO_S * ，输出MAC地址为pMac的站点属性信息，\n
                                                        详细内容参见#HI_MAC_CONFIG_INFO_S定义。
* @retval   ret 类型 #HI_U32，\n
                                      @li  #HI_ERR_SUCCESS 获取成功，\n
                                      @li  #HI_ERR_INVALID_PARAMETER 输入指针参数为空，\n
                                      更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MAC_CONFIG_INFO_S定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryInfo(HI_INOUT HI_MAC_CONFIG_INFO_S* pstQueryInfo);
/**
* @ingroup  hct_mac
* @brief  查询网络中某站点的属性
 * @par  描述:
* 该功能只对CCO产品形态生效
* @attention  作用产品类型为CCO。
* @param  pMac [IN] 类型 #HI_U8 * ，查询目标站点的MAC地址。
* @param  pstTopoEntry [OUT] 类型 #HI_MAC_NETWORK_TOPO_ENTRY_S * ，输出MAC地址为pMac的站点属性信息，\n
                                                        详细内容参见#HI_MAC_NETWORK_TOPO_ENTRY_S定义。
* @retval   ret 类型 #HI_U32，\n
                                    @li    #HI_ERR_SUCCESS 获取成功，\n
                                    @li    #HI_ERR_INVALID_PARAMETER 输入指针参数为空，\n
                                    @li    #HI_ERR_NOT_EXIST 当前产品形态不支持，\n
                                        更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MAC_NETWORK_TOPO_ENTRY_S定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryMacAttr(HI_IN HI_U8 *pMac, HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopoEntry);
/**
* @ingroup  hct_mac
* @brief  设置网络白名单
 * @par  描述:
* 设置网络白名单
* @attention  作用产品类型为CCO。
* @param  pList [IN] 类型 #HI_MAC_VALID_MAC_LIST_STRU * ，设置的白名单MAC地址列表。
* @param  reason [IN] 类型 HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM ，白名单改变原因。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MAC_NETWORK_TOPO_ENTRY_S定义。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief  添加网络白名单
 * @par  描述:
* 添加网络白名单
* @attention  作用产品类型为CCO。
* @param  pList [IN] 类型 #HI_MAC_VALID_MAC_LIST_STRU * ，添加的白名单MAC地址列表。
* @param  reason [IN] 类型 HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM ，白名单添加原因。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MAC_VALID_MAC_LIST_STRU定义。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_AddValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief  删除网络白名单
 * @par  描述:
* 删除网络白名单
* @attention  作用产品类型为CCO。
* @param  pList [IN] 类型 #HI_MAC_VALID_MAC_LIST_STRU * ，删除的白名单MAC地址列表。
* @param  reason [IN] 类型 HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM ，白名单删除原因。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MAC_VALID_MAC_LIST_STRU定义。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_DelValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief   查询当前是否可以发送报文。
 * @par  描述:
* 查询当前是否可以发送报文。
* @attention 无。
* @param  macbuffer_lid [IN] 类型 #HI_U8 ，mac_buffer报文队列的序列号。
* @retval   ret 类型 #HI_U32，#HI_TRUE 可以发送，#HI_FALSE 不可以发送。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/

HI_BOOL HI_MDM_QureySendStatus(HI_IN HI_U8 macbuffer_lid);

/**
* @ingroup  hct_mac
* @brief   设置搜表状态。
*
* @par  描述:
* 设置搜表状态，用来控制指示灯按照搜表状态闪烁，设置指示灯闪烁参见#HI_MDM_LED_Set485Led
* @attention 作用产品类型为STA(II采)。
* @param  status [IN] 类型 #HI_U8 ，搜表状态，#HI_FALSE:未搜表，#HI_TRUE:搜表中。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LED_SetSearchMeterStatus(HI_IN HI_U8 status);

/**
* @ingroup  hct_mac
* @brief   设置读取表号状态。
*
* @par  描述:
* 设置读取表号状态，用来控制指示灯闪烁
* @attention 作用产品类型为STA(II采)。
* @param  status [IN] 类型 #HI_U8 读取表号状态，#HI_FALSE:读取失败，入网时红灯绿灯亮500ms，灭500ms，周期循环显示。未入网时红灯绿灯亮9.5s，灭0.5s。\n
                                              #HI_TRUE:读取成功，入网时按照业务收发报文点灯，未入网时红灯亮3s，灭1s，绿灯灭。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LED_SetReadMeterAddrStatus(HI_IN HI_U8 status);

/**
* @ingroup  hct_mac
* @brief   设置485状态灯。
*
* @par  描述:
* 给485状态灯提供点灯接口，当485通信时，点亮485灯
* @attention 作用产品类型为STA。
* @param  on_time [IN] 类型 #HI_U8 点亮时间长短，单位ms。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 点亮失败 #HI_ERR_SUCCESS 点亮成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LED_Set485Led(HI_IN HI_U8 on_time);

/**
* @ingroup  hct_mac
* @brief   点亮指示灯
*
* @par  描述:
* 点亮指示灯
* @attention 无。
* @param  LedId [IN] 类型 #HI_U8 LED ID 取值范围参考HI_LED_ID_E。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 点亮失败 #HI_ERR_SUCCESS 点亮成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOn(HI_U8 LedId);

/**
* @ingroup  hct_mac
* @brief   点灭指示灯
*
* @par  描述:
* 点灭指示灯
* @attention 无。
* @param  LedId [IN] 类型 #HI_U8 LED ID 取值范围参考#HI_LED_ID_E。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 点灭失败 #HI_ERR_SUCCESS 点灭成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOff(HI_U8 LedId);
/**
* @ingroup  hct_mac
* @brief   设置LED不使用即空闲
*
* @par  描述:
* 设置LED不使用即空闲
* @attention 无。
* @param  LedId [IN] 类型 #HI_U8 LED ID 取值范围参考HI_LED_ID_E。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UnLockLed(HI_U8 LedId);
/**
* @ingroup  hct_mac
* @brief   设置LED使用即占用
*
* @par  描述:
* 设置LED使用即占用
* @attention 无。
* @param  LedId [IN] 类型 #HI_U8 LED ID 取值范围参考HI_LED_ID_E。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LockLed(HI_U8 LedId);

/**
* @ingroup  hct_mac
* @brief   获取LED是否占用
*
* @par  描述:
* 获取LED是否占用
* @attention 无。
* @param  LedId [IN] 类型 #HI_U8 LED ID 取值范围参考HI_LED_ID_E。
* @param  IsLock [OUT] 类型 #HI_BOOL,#HI_TRUE 表示LED被占用，#HI_FALSE 表示LED没有占用。
* @retval   ret 类型 #HI_U32，#HI_ERR_INVALID_PARAMETER 参数无效 #HI_ERR_SUCCESS 获取成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetLedIsLock(HI_U8 LedId,HI_BOOL *IsLock);

/**
* @ingroup  hct_mac
* @brief   启动指示灯闪烁
*
* @par  描述:
* 启动指示灯闪烁
* @attention 无。
* @param  led_flash_para [IN] 类型 #HI_MDM_LED_FLASH_PARA_S* LED闪烁参数。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartLedFlash(HI_MDM_LED_FLASH_PARA_S *led_flash_para);

/**
* @ingroup  hct_mac
* @brief   停止指示灯闪烁
*
* @par  描述:
* 停止指示灯闪烁
* @attention 无。
* @param  无
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StopLedFlash(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   设置发送报文指示灯
*
* @par  描述:
* 设置发送报文指示灯
* @attention 无。
* @param  LightDuration [IN] 类型 #HI_U32 LED点亮时长。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOnTx(HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   设置接收报文指示灯
*
* @par  描述:
* 设置接收报文指示灯
* @attention 无。
* @param  LightDuration [IN] 类型 #HI_U32 LED点亮时长。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOnRx(HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   设置发送报文的相位指示灯
*
* @par  描述:
* 设置发送报文的相位指示灯
* @attention 作用产品类型为CCO。
* @param  phase [IN] 类型 #HI_U8 相位（#PHASE_A、#PHASE_B、#PHASE_C、#PHASE_ALL）。
* @param  LightDuration [IN] 类型 #HI_U32 LED点亮时长。
* @retval   ret 类型 #HI_U32，#HI_ERR_FAILURE 设置失败 #HI_ERR_SUCCESS 设置成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightPhaseOnTx(HI_U8 phase,HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   设置LED模块进入工装模式
*
* @par  描述:
* APP提供接口，设置LED模块进入工装模式
* @attention 作用产品类型为STA、NDM。
* @param  无。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetDutMode(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  该函数实现配置收发指示灯。
*
* @par   描述: 根据入参值配置对应指示灯状态。
* @attention
* @param  mode [IN] 类型 #HI_U8，值参见#HI_MDM_SET_TX_RX_LED_MODE_E。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_INVALID_PARAMETER下发参数错误，#HI_ERR_NOT_SUPPORT功能未启用不支持，
   #HI_ERR_FAILURE 设置失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTxRxLedMode(HI_U8 mode);

/**
* @ingroup  hct_mac
* @brief  该函数实现配置指示灯状态。
*
* @par   描述:
* HI_TRUE: STA启用状态指示灯，HI_FALSE: STA不启用状态指示灯
* @attention
* @param  setflag [IN] 类型 #HI_BOOL HI_TRUE:STA启用状态控制指示灯，HI_FALSE: STA不启用状态控制指示灯。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_NOT_SUPPORT功能未启用不支持，
   #HI_ERR_FAILURE 设置失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see setflag控制的模块闪灯状态, 参见<Hi3911V200 API 开发指南>"指示灯控制"小节
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedStateSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief   该函数实现配置读取电表地址失败并且未入网状态下指示灯闪烁，初始化阶段调用，STA产品有效。
*
* @par  描述:
*闪烁策略:将指示灯进行点亮light_on_cnt个durtion_ms，然后点灭light_off_cnt个durtion_ms，作为一个
*闪烁周期，然后按照该策略循环显示。
* @attention 无。
* @param  enable_led [IN] 类型 #HI_BOOL 表示是否使用本次配置参数进行闪烁，HI_TRUE表示启用，HI_FALSE表示不启用。
* @param  durtion_ms [IN] 类型 #HI_U16 表示指示灯周期闪烁的周期，单位ms，最小为10ms，否则返回参数无效。
* @param  light_on_cnt [IN] 类型 #HI_U8 表示指示灯点亮的周期个数，有效范围(0,100]，否则返回参数无效。
* @param  light_off_cnt [IN] 类型 #HI_U8 表示指示灯点灭的周期个数，有效范围(0,100]，否则返回参数无效。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_INVALID_PARAMETER 下发参数无效情况下返回失败，
*#HI_ERR_BUSY已经调用接口设置了启用参数配置，不允许再次下发启用参数配置。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see LedStateSwitch控制的模块闪灯状态，参见<Hi3911V200 API 开发指南>"指示灯控制"小节
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedNotJoinNetAndReadMeterFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   该函数实现配置读取电表地址失败状态下指示灯闪烁，初始化阶段调用，STA产品有效。
*
* @par  描述:
*闪烁策略:将指示灯进行点亮light_on_cnt个durtion_ms，然后点灭light_off_cnt个durtion_ms，作为一个
*闪烁周期，然后按照该策略循环显示。
* @attention 无。
* @param  enable_led [IN] 类型 #HI_BOOL 表示是否使用本次配置参数进行闪烁，HI_TRUE表示启用，HI_FALSE表示不启用。
* @param  durtion_ms [IN] 类型 #HI_U16 表示指示灯周期闪烁的周期，单位ms，最小为10ms，否则返回参数无效。
* @param  light_on_cnt [IN] 类型 #HI_U8 表示指示灯点亮的周期个数，有效范围(0,100]，否则返回参数无效。
* @param  light_off_cnt [IN] 类型 #HI_U8 表示指示灯点灭的周期个数，有效范围(0,100]，否则返回参数无效。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_INVALID_PARAMETER 下发参数无效情况下返回失败，
*#HI_ERR_BUSY已经调用接口设置了启用参数配置，不允许再次下发启用参数配置。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see LedStateSwitch控制的模块闪灯状态，参见<Hi3911V200 API 开发指南>"指示灯控制"小节
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedReadMeterFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   该函数实现配置多网络冲突状态下指示灯闪烁，初始化阶段调用，STA产品有效。
*
* @par  描述:
*闪烁策略:将指示灯进行点亮light_on_cnt个durtion_ms，然后点灭light_off_cnt个durtion_ms，作为一个
*闪烁周期，然后按照该策略循环显示。
* @attention 无。
* @param  enable_led [IN] 类型 #HI_BOOL 表示是否使用本次配置参数进行闪烁，HI_TRUE表示启用，HI_FALSE表示不启用。
* @param  durtion_ms [IN] 类型 #HI_U16 表示指示灯周期闪烁的周期，单位ms，最小为10ms，否则返回参数无效。
* @param  light_on_cnt [IN] 类型 #HI_U8 表示指示灯点亮的周期个数，有效范围(0,100]，否则返回参数无效。
* @param  light_off_cnt [IN] 类型 #HI_U8 表示指示灯点灭的周期个数，有效范围(0,100]，否则返回参数无效。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_INVALID_PARAMETER 下发参数无效情况下返回失败，
*#HI_ERR_BUSY已经调用接口设置了启用参数配置，不允许再次下发启用参数配置。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see LedStateSwitch控制的模块闪灯状态，参见<Hi3911V200 API 开发指南>"指示灯控制"小节
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedMultiExpFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   该函数实现配置未入网状态下指示灯闪烁，初始化阶段调用，STA产品有效。
*
* @par  描述:
*闪烁策略:将指示灯进行点亮light_on_cnt个durtion_ms，然后点灭light_off_cnt个durtion_ms，作为一个
*闪烁周期，然后按照该策略循环显示。
* @attention 无。
* @param  enable_led [IN] 类型 #HI_BOOL 表示是否使用本次配置参数进行闪烁，HI_TRUE表示启用，HI_FALSE表示不启用。
* @param  durtion_ms [IN] 类型 #HI_U16 表示指示灯周期闪烁的周期，单位ms，最小为10ms，否则返回参数无效。
* @param  light_on_cnt [IN] 类型 #HI_U8 表示指示灯点亮的周期个数，有效范围(0,100]，否则返回参数无效。
* @param  light_off_cnt [IN] 类型 #HI_U8 表示指示灯点灭的周期个数，有效范围(0,100]，否则返回参数无效。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_INVALID_PARAMETER 下发参数无效情况下返回失败，
*#HI_ERR_BUSY已经调用接口设置了启用参数配置，不允许再次下发启用参数配置。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see LedStateSwitch控制的模块闪灯状态，参见<Hi3911V200 API 开发指南>"指示灯控制"小节
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedNotJoinNet(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief  该函数实现通过抄控器发送用户帧
*
* @par  描述:
* 无
* @attention 无。
* @param  usPacketId [IN] 类型 #HI_U16，报文ID。
* @param  pucPacket [IN] 类型 #HI_PBYTE，报文内容指针。
* @param  usPacketSize [IN] 类型 #HI_U16，报文大小。
* @param  aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN] [IN] 类型 #HI_U8，MAC地址。
* @param  ulSn [IN] 类型 #HI_U32，报文序号。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 成功，#HI_ERR_FAILURE 失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 报文接收处理APP注册接口参见#mrsStaTransmitRemoteCmd。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SendNdmReqFrame(HI_U16 usPacketId, HI_PBYTE pucPacket, HI_U16 usPacketSize,
                HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
#if 0
/**
* @ingroup  hct_mac
* @brief  设置发送功率模拟功率。
*
* @par  描述:
* 该函数设置的是模拟功率的实际值。一个单位衰减2dB，最大值为10，对应衰减20dB 。
* @attention
* <ul><li></li>改值会写入FLASH中，具有记忆功能，掉电后仍保持</ul>
* @param  TxAfePgaGainStep [IN] 类型 #HI_U8，最大值为10，最小值为0。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_WriteAfePgaGain(HI_U8 TxAfePgaGain);
#endif
/**
* @ingroup  hct_mac
* @brief  设置发送功率模拟增益。
*
* @par  描述:
* 该函数设置的是模拟功率的增益，并不直接配置模拟功率，\n
  实际的模拟增功率为最大模拟功率与TxAfePgaGainStep的差。\n
  一个单位衰减2dB，最大值为10，对应衰减20dB 。
* @attention
* <ul><li></li>实时生效，掉电后恢复模拟功率默认值</ul>
* @param  TxAfePgaGainStep [IN] 类型 #HI_U8，最大值为10，最小值为0。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 设置失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ReduceAfePower(HI_IN HI_U8 TxAfePgaGainStep);
/**
* @ingroup  hct_mac
* @brief  设置当前模块模拟发送功率。
*
* @par  描述:
* 设置当前模块模拟发送功率。
* @attention
* <ul><li></li> 实时生效，掉电恢复默认值</ul>
* @param TxAfePgaStep [IN] 类型 #HI_U8，最大值为10，最小值为0，单位为2dB。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 设置失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_SetAfePgaGain
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetAfePgaGain(HI_IN HI_U8 TxAfePgaStep);
/**
* @ingroup  hct_mac
* @brief  获取当前模块模拟发送功率。
*
* @par  描述:
* 获取当前模块模拟发送功率。
* @attention
* <ul><li></li> 与#HI_MDM_SetAfePgaGain相关</ul>
* @param  无。
* @retval   ret 类型 #HI_U8，最大值为10，最小值为0。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_SetAfePgaGain
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetAfePgaGain(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  设置发送功率数字增益。
*
* @par  描述:
* 四个入参分别为四种子载波数对应的数字功率值。一个时间只有一个生效。\n
  一个单位对应衰减1dB。该函数设置的是数字功率的增量，并不直接配置数字功率，\n
  当前各个频段的数字功率与PowerStep的差。
* @attention
* <ul><li></li>实时生效，掉电恢复NV 0x67中的默认值</ul>
* @param  PowerStep1 [IN] 类型 #HI_U8，频段载波为411子载波时候的功率值，取值范围[0,20]。
* @param  PowerStep2 [IN] 类型 #HI_U8，频段载波为131子载波时候的功率值，取值范围[0,20]。
* @param  PowerStep3 [IN] 类型 #HI_U8，频段载波为64子载波时候的功率值，取值范围[0,20]。
* @param  PowerStep4 [IN] 类型 #HI_U8，频段载波为32子载波时候的功率值，取值范围[0,20]。
* @param  PowerStep5 [IN] 类型 #HI_U8，保留。
* @retval   ret 类型 #HI_U32，HI_ERR_INVALID_PARAMETER表示减小后的发射功率超过发射功率下界; HI_ERR_SUCCESS成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ReducePowerStep(HI_IN HI_U8 PowerStep1,HI_IN HI_U8 PowerStep2,HI_IN HI_U8 PowerStep3,HI_IN HI_U8 PowerStep4,HI_IN HI_U8 PowerStep5);

/**
* @ingroup  hct_mac
* @brief  设置数字发送功率
*
* @par  描述:
* 四个入参分别为四种子载波数对应的数字功率值。入参均合法时，同时修改子载波的功率值。一个单位对应衰减1dB。\n
  该函数设置的是数字功率
* @attention
* <ul><li></li>实时生效，掉电恢复数字功率的默认值</ul>
* @param  PowerStep1 [IN] 类型 #HI_S8，频段载波为411子载波时候的功率值，取值范围[-11,9]。
* @param  PowerStep2 [IN] 类型 #HI_S8，频段载波为131子载波时候的功率值，取值范围-6,14]。
* @param  PowerStep3 [IN] 类型 #HI_S8，频段载波为64子载波时候的功率值，取值范围[-3,17]。
* @param  PowerStep4 [IN] 类型 #HI_S8，频段载波为32子载波时候的功率值，取值范围[0,20]。
* @retval   ret 类型 #HI_32，HI_ERR_INVALID_PARAMETER表示入参发射功率不在规定范围内;HI_ERR_SUCCESS表示成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerStep(HI_IN HI_S8 PowerStep1,HI_IN HI_S8 PowerStep2,HI_IN HI_S8 PowerStep3,HI_IN HI_S8 PowerStep4);

/**
* @ingroup  hct_mac
* @brief  获取模块的数字功率。
*
* @par  描述:
* 获取模块的数字功率。
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_S8，该值为当前子载波数下对应的数字功率。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_SetPowerStep
* @since HiMDL_V200R001C00
*/
HI_S8 HI_MDM_GetPowerStep(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置当前工作频段对应的数字和模拟发射功率，仅内存生效
*
* @par  描述:
* 设置当前工作频段对应的数字和模拟发射功率
* @attention
* <ul><li></li> 实时生效，掉电恢复默认值</ul>
* @param  PowerStep [IN] 类型 #HI_S8，当前工作频段对应的功率值，取值范围应满足不同子载波对应的功率范围。
          频段载波为411子载波时候的功率值，取值范围[-11,9]。
          频段载波为131子载波时候的功率值，取值范围[-6,14]。
          频段载波为64子载波时候的功率值，取值范围[-3,17]。
          频段载波为32子载波时候的功率值，取值范围[0,20]。
          频段载波为49子载波时候的功率值，取值范围[-2,18]。
          频段载波为89子载波时候的功率值，取值范围[-4,16]。
* @param TxAfePgaStep [IN] 类型 #HI_U8，最大值为10，最小值为0，单位为2dB。
* @retval   ret 类型 #HI_32，HI_ERR_INVALID_PARAMETER表示入参发射功率不在规定范围内;HI_ERR_SUCCESS表示成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCurrWorkAfePower(HI_IN HI_S8 PowerStep, HI_IN HI_U8 TxAfePgaStep);

/**
* @ingroup  hct_mac
* @brief  是否存在V100芯片的STA站点
*
* @par  描述:
* 是否存在V100芯片的STA站点
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_BOOL #HI_TRUE 存在， #HI_FALSE 不存在。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsV100ChipExist(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置备用mac地址
*
* @par  描述:
* II采设置备用mac地址
* @attention  入参 #HI_MAC_ATTR_STRU* 中的#HI_MAC_ENTRY_STRU 包含的地址，要求不能为全0x00或全0xff。
* @param  mac_attr [IN] 类型 #HI_MAC_ATTR_STRU* mac地址信息。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 配置成功，#HI_ERR_FAILURE 配置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetBackupMacAddr (HI_IN HI_MAC_ATTR_STRU *mac_attr);

/**
* @ingroup  hct_mac
* @brief   设置STA产品类型。
*
* @par  描述:
* 设置STA产品类型。
* @attention 作用产品类型为STA。
* @param  sta_type [IN] 类型 #HI_U8 ，STA_TYPE_METER(1)表示电表类型，STA_TYPE_CLTI(2)表示I型采集器。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetSTAProductType(HI_U8 sta_type);
/**
* @ingroup  hct_mac
* @brief   设置白名单开关接口
*
* @par  描述:
* 设置白名单开关接口
* @attention 作用产品类型为CCO。
* @param  isOn [IN] 类型 #HI_BOOL ，白名单是否开启，1表示开启，0表示关闭。
* @param  isPermanent [IN] 类型 #HI_BOOL，下次重启是否生效。1生效，0不生效。
* @param  reason [IN] 类型 #HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM，白名单变化原因码,详见定义。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetWhiteListSwitch(HI_BOOL isOn, HI_BOOL isPermanent, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);
/**
* @ingroup  hct_mac
* @brief   获取白名单开关接口
*
* @par  描述:
* 获取白名单开关接口
* @attention 作用产品类型为CCO
* @param  isOn [OUT] 类型 #HI_BOOL*
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetWhiteListSwitch(HI_BOOL* isOn);

/**
* @ingroup  hct_mac
* @brief   设置相位模式
*
* @par  描述:
* 设置相位模式
* @attention 无。
* @param  phase_cnt [IN] 类型 #HI_U8 相位个数，CCO端有效，1 <= phase_cnt <= 3，设置该参数后，需要重启设备生效。
* @param  default_phase [IN] 类型 #HI_U8 默认相位，CCO处于单相位模式时，该值有效\n
          default_phase有效值为:#HI_MDM_PLC_PHASE_A:A相位 #HI_MDM_PLC_PHASE_B:B相位 #HI_MDM_PLC_PHASE_C:C相位。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_WritePhaseMode(HI_IN HI_U8 phase_cnt,HI_IN HI_U8 default_phase);

/**
* @ingroup  hct_mac
* @brief   设置相位模式
*
* @par  描述:
* 设置相位模式，仅修改内存
* @attention 仅供CCO工装测试时的被检CCO使用。
* @param  phase_cnt [IN] 类型 #HI_U8 相位个数，CCO端有效，1 <= phase_cnt <= 3，设置该参数后，不需要重启。
* @param  default_phase [IN] 类型 #HI_U8 默认相位，CCO处于单相位模式时，该值有效\n
          default_phase有效值为:#HI_MDM_PLC_PHASE_A:A相位 #HI_MDM_PLC_PHASE_B:B相位 #HI_MDM_PLC_PHASE_C:C相位。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#其他 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetPhaseMode(HI_IN HI_U8 phase_cnt,HI_IN HI_U8 default_phase);

/**
* @ingroup  hct_mac
* @brief   整表测试模式确认
*
* @par  描述:
* 在抄控器、STA或者CCO上调用该接口，确认当前是否为抄控器的整表测试模式
* @attention 无。
* @param 无。
* @retval   ret 类型 #HI_BOOL 取值 #HI_FALSE 不是整表测试模式。#HI_TRUE  是整表测试模式。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_BOOL HI_MDM_IsNdmMeterAccess(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  查询STA的台区识别过零检测频率的平均值
*
* @par  描述:
* 查询STA的台区识别过零检测频率的平均值
* @attention 作用产品类型为STA、NDM。
* @param  pfnCallback [IN] 类型 QueryZeroCrossCallback，参见#QueryZeroCrossCallback定义。
* @retval   ret 类型 #HI_U32 取值 @li #HI_ERR_SUCCESS 获取成功\n
                                       @li #HI_ERR_BUSY    正在查询中\n
                                       @li #HI_ERR_FAILURE 返回失败\n
                                       @li #HI_ERR_NOT_SUPPORT 未组网。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryZeroCross(QueryZeroCrossCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  工装检测收集CCO,STA产品的过零点数据
*
* @par  描述:
* 工装检测收集CCO,STA产品的过零点数据
* @attention 无。
* @param  enable [IN] 类型 #HI_BOOL，取值: #HI_TRUE 表示启动采集，#HI_FALSE  表示停止采集。
* @param  ucLevel [IN] 类型 #HI_U8，取值: #EQUIP_ZERO_CROSS_LEVEL_240K 表示240k，#EQUIP_ZERO_CROSS_LEVEL_1M  表示1M。
* @param  ucPhaseMode [IN] 类型 #HI_U8, 取值: #PHASE_A 表示A相，#PHASE_B 表示B相，#PHASE_C 表示C相
* @param  edge_mdoe[IN]类型 #HI_U8，取值: #EQUIP_ZERO_CROSS_CRICUIT_RISE表示上升沿，#EQUIP_ZERO_CROSS_CRICUIT_FALL表示下降沿，其他边沿无效，针对CCO产品有效，其他产品无效。
* @param  pfnCallback [IN] 类型 CollectNtbCallback，注册回调函数。
* @retval   ret 类型 #HI_U32，\n
                                       @li #HI_ERR_SUCCESS 获取成功\n
                                       @li #HI_ERR_BUSY    正在数据采集中\n
                                       @li #HI_ERR_FAILURE 返回失败\n
                                       @li #HI_ERR_NOT_SUPPORT STA产品未组网场景下返回该错误，CCO产品不支持下降沿情况下返回该错误\n
                                       @li #HI_ERR_INVALID_PARAMETER 参数错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_CollectNtb(HI_BOOL enable, HI_U8 ucLevel, HI_U8 ucPhaseMode, HI_U8 edge_mode, CollectNtbCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  下发配置STA,NDM产品的过零档位
*
* @par  描述:
* 下发配置STA,NDM产品的过零档位
* @attention 作用产品类型为STA、NDM。
* @param  ucLevel [IN] 类型 #HI_U8，取值:\n @li #EQUIP_ZERO_CROSS_LEVEL_240K 表示240k\n
                                            @li #EQUIP_ZERO_CROSS_LEVEL_1M  表示1M。
* @retval   ret 类型 #HI_U32，\n
                                      @li  #HI_ERR_SUCCESS 设置成功\n
                                      @li  #HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetZeroCrossLevel(HI_U8 ucLevel);
/**
* @ingroup  hct_mac
* @brief  下发配置产品的过零校正电压
*
* @par  描述:
* 下发配置产品的过零校正电压
* @attention 无。
* @param  vlotage [IN] 类型 #HI_S16，取值: vlotage 有效值范围[-511, 511]
* @retval   ret 类型 #HI_U32，\n
                                       @li #HI_ERR_SUCCESS 设置成功\n
                                       @li #HI_ERR_FAILURE 设置失败\n
                                       @li #HI_ERR_INVALID_PARAMETER 参数下发错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_WriteCorrectionVoltage(HI_S16 vlotage);

/**
* @ingroup  hct_mac
* @brief  获取产品的过零校正电压
*
* @par   描述:
* 获取产品的过零校正电压
* @attention
* @param  vlotage [IN] 类型 #HI_S16 *
* @retval   ret [IN] 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 获取失败
* @par Dependency:
* <ul><li>hi_mdm.h: 文件用于描述链路层对外接口.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCorrectionVoltage(HI_S16 *vlotage);

/**
* @ingroup  hct_mac
* @brief  配置台区识别功能寄存器使能开关
*
* @par   描述:
* 配置台区识别功能寄存器使能开关
* @attention
* @param  vlotage [IN] 类型 #HI_BOOL,#HI_TRUE表示启用功能，#HI_FALSE表示关闭功能
* @retval   ret [IN] 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE设置失败
* @par Dependency:
* <ul><li>hi_mdm.h: 文件用于描述链路层对外接口.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTfRegEnSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief  设置抄控宝同步对象，只在抄控宝工装下使用。
*
* @par  描述:
* @li 设置抄控宝同步对象，只在抄控宝工装下使用。
* @attention 作用产品类型为NDM。
* @param  freq_index [IN] 类型 #HI_U8，同步对象的工作模式0~5，注意工作模块的最大值取决于版本配置的盲检频段个数。
* @param  snid [IN] 类型 #HI_U32， 同步对象的snid，取值1~15对应V100的协议，16~16777215对应V200协议。
* @param  cco_mac [IN] 类型 #HI_U8，CCO的Mac地址。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 同步对象参数设置成功。#HI_ERR_FAILURE 同步对象参数设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmNtbSynchStation(HI_U8 freq_index,HI_U32 snid,HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  判断NTB同步是否成功
*
* @par  描述:
* 判断NTB同步是否成功
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_BOOL 取值HI_ETRUE 同步成功。 HI_EFALSE 同步失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_CheckNtbIsSynchSuccess(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  注册台区识别信息有刷新时的上报回调函数
*
* @par  描述:
* 注册台区识别信息有刷新时的上报回调函数
* @attention
* <ul><li> 作用产品类型为CCO。不支持阻塞性的回调函数注册到本接口中。</li></ul>
* @param  func [IN] 类型 #HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN 。
* @retval   ret 类型HI_U32，取值:\n @li #HI_ERR_INVALID_PARAMETER 表示入参错误\n
                                    @li #HI_ERR_SUCCESS 表示成功\n
                                    @li #HI_ERR_NOT_SUPPORT 表示该产品形态不支持台区识别白名单查询\n
                                    @li 其他值参照hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterReportTransformerListRefresh(HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN func);
/**
* @ingroup  hct_mac
* @brief  注册相位识别信息有刷新时的上报回调函数
*
* @par  描述:
* 注册相位识别信息有刷新时的上报回调函数
* @attention 作用产品类型为CCO。
* @param  func [IN] 类型 #HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN 。
* @retval   ret 类型HI_U32，取值:\n @li #HI_ERR_INVALID_PARAMETER 表示入参错误\n
                                    @li #HI_ERR_SUCCESS 表示成功\n
                                    @li #HI_ERR_NOT_SUPPORT 表示该产品形态不支持\n
                                    @li 其他值参照hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterReportPhaseListRefresh(HI_MDM_REPORT_PHASE_LIST_REFRESH_FUN func);
/**
* @ingroup  hct_mac
* @brief  查询台区识别名单
*
* @par  描述:
* 查询台区识别名单
* @attention 作用产品类型为CCO。
* @param  pstList [IN] 类型 #HI_MDM_TRANSFORMER_LIST_S *，详细参见#HI_MDM_TRANSFORMER_LIST_S 的定义。
* @param  entry_num [IN] 类型 HI_U16，用户输入的pstList->pEntry指向的空间能存储的站点数。
* @retval   ret 类型HI_U32，取值: \n @li #HI_ERR_FAILURE 表示数据错误\n
                                     @li #HI_ERR_INVALID_PARAMETER 表示入参错误\n
                                     @li #HI_ERR_SUCCESS 表示成功\n
                                     @li #HI_ERR_NOT_SUPPORT 表示该产品形态不支持台区识别白名单查询\n
                                     @li 其他值参照hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see  HI_MDM_TRANSFORMER_LIST_S
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryTransformerList(HI_MDM_TRANSFORMER_LIST_S *pstList, HI_U16 entry_num);
/**
* @ingroup  hct_mac
* @brief  查询黑名单（非本台区的站点）
*
* @par  描述:
* 查询黑名单（非本台区的站点）
* @attention 作用产品类型为CCO。
* @param  pstList [IN] 类型 #HI_MDM_BLACK_LIST_S *，详细参见#HI_MDM_BLACK_LIST_S 的定义。
* @param  entry_num [IN] 类型 #HI_U16，用户输入的pstList->pEntry指向的空间能存储的站点数。
* @retval   ret 类型HI_U32，取值: \n @li #HI_ERR_FAILURE 表示数据错误\n
                                     @li #HI_ERR_INVALID_PARAMETER 表示入参错误\n
                                     @li #HI_ERR_SUCCESS 表示成功\n
                                     @li #HI_ERR_NOT_SUPPORT 表示该产品形态不支持台区识别白名单查询\n
                                     @li 其他值参照hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryBlackList(HI_MDM_BLACK_LIST_S *pstList, HI_U16 entry_num);
/**
* @ingroup  hct_mac
* @brief   点对点通信接收报文回调函数注册
*
* @par  描述:
* 点对点通信接收报文回调函数注册
* @attention 无。
* @param  id [IN] 类型 #HI_U8，点对点通信命令ID，取值范围1~5。
* @param  func [IN] 类型 #HI_MDM_NDM_PING_F。
* @retval   ret 类型 #HI_U32，#HI_ERR_INVALID_PARAMETER 入参不合法，#HI_ERR_SUCCESS 报文发送成功，\n
                           更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterNdmPingFunc(HI_U8 id, HI_MDM_NDM_PING_F func);

/**
* @ingroup  hct_mac
* @brief  点对点通信报文发送
*
* @par  描述:
* 点对点通信报文发送
* @attention 无。
* @param  usId [IN] 类型 #HI_U8，NDM点对点通信命令ID，取值范围 #HI_MDM_NDM_PING_ID_0 至 #HI_MDM_NDM_PING_ID_5。
* @param  pucMsgPaload [IN] 类型 #HI_PBYTE，报文的内容缓存地址。
* @param  uspucMsgPaloadSize [IN] 类型 #HI_U32，报文的长度会自动四字节对齐，最大348单位字节。
* @param  pstCtrl [IN] 类型 #HI_MDM_MSG_CTRL_S* ，报文控制字结构，详细见#HI_MDM_MSG_CTRL_S定义。
* @retval   ret 类型 #HI_U32，\n
                                     @li  #HI_ERR_INVALID_PARAMETER 入参不合法\n
                                     @li  #HI_ERR_NOT_ENOUGH_MEMORY    内存申请失败\n
                                     @li  #HI_ERR_SUCCESS 报文发送成功\n
                                     @li  更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SendNdmPing(HI_U8 usId, HI_PBYTE pucMsgPaload, HI_U32 uspucMsgPaloadSize,
                                    HI_MDM_MSG_CTRL_S* pstCtrl);

/**
* @ingroup  hct_mac
* @brief  配置CCO/STA模块设备标识符。
*
* @par 描述:
* 配置CCO/STA模块设备标识符。
* @attention 无。
*
* @param  aucDevIdBcd[HI_MDM_DEV_ID_LEN] [IN] 6个字节的BCD码，即数字。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_mac.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_DevIdConfig(HI_U8 aucDevIdBcd[HI_MDM_DEV_ID_LEN]);

/**
* @ingroup  hct_mac
* @brief 配置STA的占空比策略
*
* @par  描述:
* 配置STA的占空比策略
* @attention 作用产品类型为STA。
* @param  duty_enable [IN] 类型 #HI_BOOL，是否启用占空策略。
* @param  duty_percent [IN] 类型 #HI_U8，占空比。
* @param  max_sendlength [IN] 类型 #HI_U32，最大可连续发送 时长，单位ms。
* @retval   ret 类型 #HI_U32，\n
                                     @li #HI_ERR_SUCCESS 配置成功\n
                                     @li #HI_ERR_FAILURE  配置失败\n
                                     @li 更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMacDutyCycleCtrlParam(HI_IN HI_BOOL duty_enable,HI_IN HI_U8 duty_percent,HI_IN HI_U32 max_sendlength);

/**
* @ingroup  hct_mac
* @brief 配置信标周期
*
* @par  描述:
* 配置信标周期
* @attention 作用产品类型为CCO。
* @param  MinPeriod [IN] 类型 #HI_U32，最小信标周期，单位ms，取值范围:1000ms-10000ms。
* @param  MaxPeriod [IN] 类型 #HI_U32，最大信标周期，单位ms，取值范围:1000ms-10000ms。
* @retval   ret 类型 #HI_U32，\n
                              @li #HI_ERR_SUCCESS 配置成功\n
                              @li #HI_ERR_FAILURE 返回失败\n
                              @li 更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTdmaPeriod(HI_IN HI_U32 MinPeriod,HI_IN HI_U32 MaxPeriod);

/**
* @ingroup  hct_mac
* @brief 获取信标周期
*
* @par  描述:
* 配置信标周期
* @attention 作用产品类型为CCO。
* @param  MinPeriod [OUT] 类型 #HI_U32，最小信标周期，单位ms，取值范围:1000ms-10000ms。
* @param  MaxPeriod [OUT] 类型 #HI_U32，最大信标周期，单位ms，取值范围:1000ms-10000ms。
* @retval   ret 类型 #HI_U32，\n
                              @li #HI_ERR_SUCCESS 获取成功\n
                              @li #HI_ERR_FAILURE 返回失败\n
                              @li 更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTdmaPeriod(HI_OUT HI_U32 *MinPeriod, HI_OUT HI_U32 *MaxPeriod);

/**
* @ingroup  hct_mac
* @brief 获取当前信标周期的长度
*
* @par  描述:
* 获取当前信标周期
* @attention 作用产品类型为CCO、NDM。
* @param  无
* @retval   ret 类型 #HI_U32，当前信标周期的长度,单位是100us。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTdmaPeriodLen(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  配置信标时隙时长
*
* @par  描述:
* 配置信标时隙时长
* @attention 作用产品类型为CCO。
* @param  BeaDuration [IN] 类型 #HI_U8，一个信标时长，单位ms。 取值: [0,25]。
* @retval   ret 类型 #HI_U32，\n
                              @li #HI_ERR_SUCCESS 设置成功\n
                              @li #HI_ERR_FAILURE 返回失败\n
                              @li 更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconDuration(HI_IN HI_U8 BeaDuration);
/**
* @ingroup  hct_mac
* @brief  配置多网络协调开关
*
* @par  描述:
* 配置多网络协调开关
* @attention 作用产品类型为CCO。
* @param  MultiNetworkCoordinateSwitch [IN] 类型 #HI_BOOL，取值#HI_TRUE, 表示打开，#HI_FALSE 表示关闭。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMultiNetworkCoordinateSwitch(HI_IN HI_BOOL MultiNetworkCoordinateSwitch);

/**
* @ingroup  hct_mac
* @brief  获取多网络协调开关
*
* @par  描述:
* 获取多网络协调开关
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_BOOL，取值#HI_TRUE, 表示打开，#HI_FALSE 表示关闭。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetMultiNetworkCoordinateSwitch(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  配置多网络监听时间
*
* @par  描述:
* 配置多网络监听时间
* @attention 作用产品类型为CCO。
* @param  MultiNetworkListenTime [IN] 类型 #HI_U32, 多网络监听时间单位ms。取值: [100,10000]。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 配置成功，#HI_ERR_INVALID_PARAMETER 无效的入参，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMultiNetWorkListenTime(HI_IN HI_U32 MultiNetworkListenTime);

/**
* @ingroup  hct_mac
* @brief  配置送检模式下频段为全频，仅支持国网协议使用。
*
* @par  描述:
* 配置送检模式下频段为全频
* @attention 无。
* @param  无。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetTestFreq(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  抄控器串口发送时，对led控制．
*
* @par  描述:
* 抄控器串口发送时，点亮led 20ms
* @attention 作用产品类型为NDM。
* @param  无。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NdmSerialInterfaceTxLedProc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  抄控器串口接收时，对led控制．
*
* @par  描述:
* 抄控器串口接收时，点亮led 20ms
* @attention 作用产品类型为NDM。
* @param  无。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NdmSerialInterfaceRxLedProc(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  无扰台区识别模式
*/
typedef enum
{
    SOFT_TF_OFF = 0,                        /**< 关闭无扰台区识别*/
    SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT,  /**< 打开无扰台区识别，站点根据识别结果加入网络*/
    SOFT_TF_ON_WITH_STA_JOIN_BY_WHITE,      /**< 打开无扰台区识别，站点根据白名单加入网络*/
    SOFT_TF_MODE_INVALID,                   /**< 非法值*/
}HI_MDM_SOFT_TF_MODE_EN;

/**
* @ingroup  hct_mac
* @brief  设置无扰台区识别模式。
*
* @par  描述:
* 设置无扰台区识别模式。
* @attention 作用产品类型为CCO和STA。
* @param  eSoftTfMode [IN] 类型 #HI_MDM_SOFT_TF_MODE_EN，无扰台区识别模式。
* @retval   ret 类型 #HI_U32，取值HI_ERR_SUCCESS :发送设置消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSoftTfMode(HI_MDM_SOFT_TF_MODE_EN eSoftTfMode);

/**
 *@ingroup hct_mac
 * 无扰台区识别结果输出结构
 */
typedef struct
{
    HI_BOOL bIsTfComplete;                          /**< 是否识别完成 */
    HI_BOOL bIsBelongJoinNet;                       /**< 是否属于加入台区 */
    HI_U16  usRequeryTime;                          /**< 建议等待多久后再查询，单位s */

    HI_U8  aucMainNodeAddr[HI_PLC_MAC_ADDR_LEN];    /**< 站点归属的主节点地址   */
    HI_U8  bIsMainNodeAddr;                         /**< 是否获取到主节点地址，未获取到主节点地址时，填充MAC地址 */
    HI_U8  ucPad;
} HI_MDM_SNR_TF_RESULT_S;

/**
* @ingroup  hct_mac
* @brief  查询无扰台区识别结果
*
* @par  描述:
* 查询无扰台区识别结果
* @attention 作用产品类型为STA，。
* @param  pstResult [OUT] 类型 #HI_MDM_SNR_TF_RESULT_S *，输出台区识别结果。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS :查询成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 参见hi_mdm.h中#HI_MDM_SNR_TF_RESULT_S定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSnrTfResult(HI_MDM_SNR_TF_RESULT_S *pstResult);

/**
* @ingroup  hct_mac
* @brief  清除无扰台区识别NTB识别结果
*
* @par  描述:
* 清除无扰台区识别NTB识别结果
* @attention 作用产品类型为STA，。
* @param is_clear_history [IN] 类型 #HI_BOOL，是否清除识别历史记录。
* @param is_restart_identify [IN] 类型 #HI_BOOL，是否重新开始识别。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS :发送清除消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearSoftTfNtbResult(HI_BOOL is_clear_history, HI_BOOL is_restart_identify);

/**
* @ingroup  hct_mac
* @brief  锁定网络功能
*
* @par  描述:
* 锁定网络功能
* @attention 作用产品类型为STA。
* @param  lock_duration [HI_IN] 类型 #HI_U32，锁定时长，单位:S。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS :设置成功，#HI_ERR_INVALID_PARAMETER 入参的指针为空。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LockNetwork(HI_IN HI_U32 lock_duration);
/**
* @ingroup  hct_mac
* @brief  查询当前锁定信息。
*
* @par  描述:
* 查询当前锁定信息。
* @attention 作用产品类型为STA。
* @param  pstLockInfo [HI_OUT] 类型 #HI_MDM_LOCK_INFO_STRU *，输出锁定信息。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 查询成功。#HI_ERR_INVALID_PARAMETER 入参的指针为空。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 参见hi_mdm.h中#HI_MDM_LOCK_INFO_STRU定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetLockInfo(HI_OUT HI_MDM_LOCK_INFO_STRU * pstLockInfo);
/**
* @ingroup  hct_mac
* @brief  解锁网络。
*
* @par  描述:
* 解锁网络。
* @attention 作用产品类型为STA。
* @param  无。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 发送解锁消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UnlockNetwork(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  重新入网。
*
* @par  描述:
* 重新入网。
* @attention 作用产品类型为STA。
* @param  无。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 通知重新入网成功(2s左右重新离网重新入网，\n
                  其他: 通知失败，可能当前未入网，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RejoinNetwork(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置站点归属网络
*
* @par  描述:
* 设置站点归属网络。
* @attention 作用产品类型为STA，设置归属网络后，若站点能接收到该网络，无扰台区识别结果以设置的网络为准。
* @param cco_addr[HI_PLC_MAC_ADDR_LEN] [IN] 类型 #HI_U8 归属网络的MAC地址或者主节点地址(默认为MAC地址)。
* @param is_main_node_addr [IN] 类型 #HI_BOOL cco_addr是否为主节点地址。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 发送设置归属网络消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBelongNetwork(HI_U8 cco_addr[HI_PLC_MAC_ADDR_LEN], HI_BOOL is_main_node_addr);

/**
* @ingroup  hct_mac
* @brief  清除归属网络信息
*
* @par  描述:
* 清除归属网络信息。
* @attention 作用产品类型为STA。
* @param  无。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 发送清除消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearBelongNetwork(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  无扰台区识别NTB发送流程控制类型
*/
typedef enum
{
    HI_MDM_NTB_SEND_PROCESS_RESUME = 0,         /**< 恢复发送流程*/
    HI_MDM_NTB_SEND_FREQUENCY_REDUCE,           /**< 降低发送频率*/
    HI_MDM_NTB_SEND_PROCESS_PAUSE,              /**< 暂停发送流程*/
    HI_MDM_NTB_SEND_CTRL_MAX,
}HI_MDM_NTB_SEND_PROCESS_CTRL_EN;

/**
 * @ingroup hct_mac
 * @brief 控制台区识别NTB发送流程的原因: 抄表
 */
#define HI_MDM_NTB_SEND_CTRL_REASON_READ_METER      (1 << 0)
/**
 * @ingroup hct_mac
 * @brief 控制台区识别NTB发送流程的原因: 升级
 */
#define HI_MDM_NTB_SEND_CTRL_REASON_UPGADE          (1 << 1)

/**
* @ingroup  hct_mac
* @brief  设置无扰台区识别NTB发送流程控制
*
* @par  描述:
* 设置无扰台区识别NTB发送流程控制
* @attention 作用产品类型为CCO。需要保证业务通信性能时，使用此接口降低无扰台区识别信道占用。
* @param  ctrl_type [IN] 类型 #HI_MDM_NTB_SEND_PROCESS_CTRL_EN 无扰台区识别NTB发送控制类型。
* @param  reason [IN] 无扰台区识别NTB发送控制原因，参见HI_MDM_NTB_SEND_CTRL_REASON_XX。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 消息发送成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_CTRL_EN ctrl_type, HI_U32 reason);

/**
*@ingroup hct_mac
* 台区识别回调传输数据类型枚举
*/
typedef enum
{
    TF_CALLBACK_OPTION_NOTIFY_START_COLLECT_NTB,    /**< 通知启动NTB数据采集，数据结构为TF_NOTIFY_START_COLLECT_NTB_INFO_S */
    TF_CALLBACK_OPTION_NOTIFY_NTB_DATA,             /**< 告知采集的NTB数据，数据结构为TF_NOTIFY_NTB_DATA_INFO_S */
    TF_CALLBACK_OPTION_QUERY_NTB_DATA,              /**< 查询采集的NTB数据，数据结构为TF_QUERY_NTB_DATA_INFO_S */
    TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA,         /**< STA上报NTB数据给APP，数据结构为TF_STA_REPORT_NTB_DATA_INFO_S */
} TF_CALLBACK_OPTION_E;

/**
*@ingroup hct_mac
* 通知启动NTB数据采集信息，对应回调类型TF_CALLBACK_OPTION_NOTIFY_START_COLLECT_NTB
*/
typedef struct
{
    HI_U32  start_ntb;                      /**< 起始NTB */

    HI_U8   collect_num;                    /**< 采集数量 */
    HI_BOOL is_incarease_collect_seq;       /**< 是否增加采集序列号 */
    HI_U8   dest_mac[HI_PLC_MAC_ADDR_LEN];  /**< 目标站点MAC地址（全FF表示广播） */
} TF_NOTIFY_START_COLLECT_NTB_INFO_S;

/**
*@ingroup hct_mac
* 告知NTB数据信息，对应回调类型TF_CALLBACK_OPTION_NOTIFY_NTB_DATA
*/
typedef struct
{
    HI_U32  freq : 8;                                /**< 数据来源的频段号 */
    HI_U32  nid  : 24;                               /**< 数据来源的网络号 */

    HI_U8   addr[HI_PLC_MAC_ADDR_LEN];               /**< 数据来源的地址，CCO为主节点地址，STA为表地址 */
    HI_U16  tei;                                     /**< 数据来源的TEI */

    HI_U8   is_increase_collect_seq;                 /**< 是否增加采集序列号，发送时有效 */
    HI_U8   collect_seq;                             /**< 采集序列号，接收时有效 */
    HI_BOOL is_collect_seq_valid;                    /**< 采集序列号是否合法(与CCO端采集序列号一致为合法)，接收时有效 */
    HI_U8   collect_edge;                            /**< 采集边沿，定义参见POWER_EDGE_XX */

    HI_U16 total_ntb_num;                           /**< 总的NTB个数 */
    HI_U8  phase_ntb_num[PHASE_CNT*POWER_EDGE_CNT]; /**< 各相位NTB个数，顺序: 单沿时为ABCXXX，双沿时下降沿ABC上升沿ABC */

    HI_U32 ntb_buf[0];                              /**< NTB值，与phase_diff_num顺序一致 */
} TF_NOTIFY_NTB_DATA_INFO_S;

/**
*@ingroup hct_mac
* 查询NTB数据信息，对应回调类型TF_CALLBACK_OPTION_QUERY_NTB_DATA
*/
typedef struct
{
    HI_U8  dest_mac[HI_PLC_MAC_ADDR_LEN];           /**< 查询目标MAC地址（全FF表示广播） */
    HI_U16 pad;
} TF_QUERY_NTB_DATA_INFO_S;

/**
*@ingroup hct_mac
* STA上报给APP层的NTB数据信息，对应回调类型TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA
*/
typedef struct
{
    HI_U8  collect_num;             /**< 采集数量(即NTB序列的长度) */
    HI_U8  pad[3];

    HI_U32 ntb_buf[0];             /**< NTB序列 */
} TF_STA_REPORT_NTB_DATA_INFO_S;

/**
* @ingroup  hct_mac
* @brief   定义台区识别模块回调函数的类型
*
* @par  描述:
* 定义台区识别模块回调函数的类型
* @attention 作用产品类型为CCO，STA。回调函数返回后，程序会释放数据内存，若需要继续使用数据，务必在回调返回前进行拷贝。
* @param  option [IN] 类型 #TF_CALLBACK_OPTION_E，触发回调的类型。
* @param  data [IN] 类型 #HI_U8*，调用回调传递的数据。
* @param  data_size [IN] 类型 #HI_U16，调用回调传递的数据大小。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #TF_NOTIFY_STA_COLLECT_NTB_INFO_S，#TF_NOTIFY_NTB_DATA_INFO_S。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*TF_CALLBACK_FUN)(TF_CALLBACK_OPTION_E option, HI_U8 *data, HI_U16 data_size);

/**
* @ingroup  hct_mac
* @brief   注册台区识别回调函数
*
* @par  描述:
* 注册台区识别回调函数
* @attention 作用产品类型为CCO，STA。
* @param  fun [IN] 类型 #TF_CALLBACK_FUN，台区识别回调函数类型。
* @retval ret 类型 #HI_U32，取值#HI_ERR_SUCCESS表示查询成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_registerTfCallbackFun(TF_CALLBACK_FUN fun);

/**
* @ingroup  hct_mac
* @brief   通知STA采集NTB数据
*
* @par  描述:
* 通知STA采集NTB数据，采集完成后，会通过HI_MDM_registerTfCallbackFun注册的回调将数据上报到APP层，
* 上报回调类型为TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA。
* @attention 作用产品类型为STA。
* @param  start_ntb [IN] 类型 #HI_U32，起始NTB。
* @param  collect_num [IN] 类型 #HI_U8，采集NTB数量。
* @retval ret 类型 #HI_U32，取值#HI_ERR_SUCCESS表示发送消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_notifyStaCollectNtbData(HI_U8 collect_seq, HI_U32 start_ntb, HI_U8 collect_num);

/**
* @ingroup  hct_mac
* @brief   通知处理NTB数据
*
* @par  描述:
* 通知处理NTB数据
* @attention 作用产品类型为CCO和STA。
* @param  ntb_data [IN] 类型 #TF_NOTIFY_NTB_DATA_INFO_S，接收到的NTB数据。
* @param  data_size [IN] 类型 #HI_U16，NTB数据所占内存空间。
* @retval ret 类型 #HI_U32，取值#HI_ERR_SUCCESS表示发送消息成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_notifyHandleRcvNtbData(TF_NOTIFY_NTB_DATA_INFO_S *ntb_data, HI_U16 data_size);

/**
*@ingroup hct_mac
* 获取相位识别信息
*/
typedef struct
{
    HI_U32 is_identify_complete : 1;       /**< 是否识别完成 */
    HI_U32 phase_result : 3;               /**< 相线1相位识别结果 */
    HI_U32 phase_result_b : 3;             /**< 相线2相位识别结果 */
    HI_U32 phase_result_c : 3;             /**< 相线3相位识别结果 */
    HI_U32 pn_result : 2;                  /**< 相线1零火反接识别结果 */
    HI_U32 pn_result_b : 2;                /**< 相线2零火反接识别结果 */
    HI_U32 pn_result_c : 2;                /**< 相线3零火反接识别结果 */
    HI_U32 pad : 16;
} HI_MDM_PHASE_IDENTIFY_RESULT_STRU;

/**
* @ingroup  hct_mac
* @brief  获取相位识别结果
*
* @par  描述:
* 获取相位识别结果(包含零火反接结果)
* @attention 仅作用于STA端。
* @param  phase_identify_info [OUT] 类型 #HI_MDM_PHASE_IDENTIFY_RESULT_STRU，相位识别信息。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS表示查询成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetPhaseIdentifyResult(HI_OUT HI_MDM_PHASE_IDENTIFY_RESULT_STRU *phase_identify_info);

/**
* @ingroup  hct_mac
* @brief  设置站点的相位识别结果
*
* @par  描述:
* 设置站点的相位识别结果(包含零火反接结果)
* @attention 仅作用于CCO端。
* @param  tei [IN] 类型 #HI_U16，被设置站点的TEI。
* @param  phase_identify_info [IN] 类型 #HI_MDM_PHASE_IDENTIFY_RESULT_STRU，相位识别结果。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS表示设置消息发送成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaPhaseIdentifyResut(HI_U16 tei, HI_MDM_PHASE_IDENTIFY_RESULT_STRU *phase_identify_info);

/**
* @ingroup  hct_mac
* @brief  获取CCO Mac地址函数
*
* @par  描述:
* 获取CCO Mac地址函数
* @attention 作用产品类型为STA。
* @param  pucMac [IN]/[OUT] 类型 #HI_U8 * 获取cco mac地址。
* @retval   ret 类型 #HI_U32，取值：\n
                                @li #HI_ERR_SUCCESS 获取cco mac地址成功\n
                                @li #HI_ERR_FAILURE 获取cco mac地址失败\n
                                @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCCOMacAddr(HI_U8 *pucMac);

/**
* @ingroup  hct_mac
* @brief  获取代理节点的 Mac地址函数
*
* @par  描述:
* 获取代理节点的 Mac地址函数
* @attention 作用产品类型为STA。
* @param  pucMac [IN]/[OUT] 类型 #HI_U8 * 获取proxy mac地址。
* @retval   ret 类型 #HI_U32，取值：\n
                                @li #HI_ERR_SUCCESS 获取proxy mac地址成功\n
                                @li #HI_ERR_FAILURE 获取proxy mac地址失败\n
                                @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetProxyMacAddr(HI_U8 *pucMac);

/**
* @ingroup  hct_mac
* @brief  获取STA不入网原因注册函数
*
* @par  描述:
* 获取STA不入网原因注册函数
* @attention 作用产品类型为STA。
* @param  staNotJoinCallback [IN] 类型 #HI_MDM_StaNotJoinReasonCallback 不入网原因回调函数。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 注册成功 #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Register_NotJoinReason(HI_MDM_StaNotJoinReasonCallback staNotJoinCallback);

/**
* @ingroup  hct_mac
* @brief  使能MAC层进入网络测试模式
*
* @par  描述:
* 使能MAC层进入网络测试模式
* @attention 作用产品类型为CCO。
* @param  snid [IN] 类型 #HI_U32 网络号，V100协议取值范围 1-15。国网协议取值范围为 1- 16777215
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 使能成功，#HI_ERR_FAILURE 使能失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EnterTestMode(HI_IN HI_U32 snid);
/**
* @ingroup  hct_mac
* @brief  使能MAC层退出网络测试模式
*
* @par  描述:
* CCO退出网络测试模式时，模块硬复位
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ExitTestMode(HI_VOID);

/**
* @ingroup hct_mac
* @brief 设置业务状态
*
* @par 描述:
* 在APP当前的业务不允许MAC中断时，调用该接口通知MAC，禁止MAC的启动影响业务的操作(例如台区识别功能)；\n
在APP结束该业务时，调用该接口通知MAC，取消对MAC的启动影响业务的操作的限制。\n
目前已预知的APP业务只有升级业务。
* @attention 作用产品类型为CCO。
* @param businessState [IN] 类型 #HI_MAC_BUSINESS_STATE_ENUM businessState。
* @retval ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MAC_InformBusinessState(HI_MAC_BUSINESS_STATE_ENUM businessState);
/**
* @ingroup  hct_mac
* @brief  获取协议版本号
*
* @par  描述:
* 获取协议版本号
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   protocol_ver [IN] 类型 #HI_U16，V100协议版本号:#HI_MDM_PTCL_VER_100，国网协议版本号: #HI_MDM_PTCL_VER_SG，V200协议版本号: #HI_MDM_PTCL_VER_200。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm.h文件中#HI_MDM_PTCL_VER_100等定义。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetProtocolVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  通知已入网的STA站点立即离线
*
* @par  描述:
* 通知已入网的STA站点立即离线
* @attention 作用产品类型为CCO。
* @param  aucStaMac[HI_PLC_MAC_ADDR_LEN] [IN] 类型 #HI_U8。
* @param  usStaTei [IN] 类型 #HI_U16。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_INVALID_PARAMETER 入参无效 #HI_ERR_SUCCESS 设置STA离线成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaOffline(HI_U8 aucStaMac[HI_PLC_MAC_ADDR_LEN], HI_U16 usStaTei);

/**
* @ingroup  hct_mac
* @brief  设置CCO端组网完成超时功能
*
* @par  描述:
* 超时时间到期，有站点在线，强制将组网中设置为组网完成状态
* @attention 作用产品类型为CCO。
* @param  NetStateFormEnable[IN] 类型 #HI_BOOL，下发启用停止开关，#HI_TRUE，表示开启该功能，#HI_FASLE，表示停止该功能。
* @param  NetStateFormTimeOut [IN] 类型 #HI_U32，设置组网超时时间，最小必须大于2分钟，单位ms。
* @retval   ret 类型 #HI_U32，取值：\n
                               @li #HI_ERR_SUCCESS 下发设置功能成功\n
                               @li #HI_ERR_BUSY 表示已经启动，不允许重复启动\n
                               @li #HI_ERR_FAILURE 下发设置失败\n
                               @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNetWorkStateFormSwitch(HI_BOOL NetStateFormEnable,HI_U32 NetStateFormTimeOut);

/**
* @ingroup  hct_mac
* @brief  设置STA离线接口
*
* @par  描述:
* 设置STA离线接口
* @attention 作用产品类型为CCO。
* @param  pstList [IN] 类型 #HI_MDM_TEI_LIST_S *。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_INVALID_PARAMETER 入参无效 #HI_ERR_SUCCESS 踢除STA成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_TEI_LIST_S 结构体。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaOfflineByTeiList(HI_MDM_TEI_LIST_S *pstList);

/**
* @ingroup  hct_mac
* @brief  获取邻居网络信息函数
*
* @par  描述:
* 获取邻居网络信息函数。
* @attention 适用于国网版本，作用产品类型为CCO。
* @param  pstInfo [IN]/[OUT] 类型 #HI_MDM_NEIGHBOR_NET_INFO_S *，存储邻居网络信息。
* @retval   ret 类型 #HI_U32，取值：\n
                               @li #HI_ERR_SUCCESS 获取邻居网络信息成功\n
                               @li #HI_ERR_FAILURE 获取邻居网络信息失败\n
                               @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中关于 #HI_MDM_NEIGHBOR_NET_INFO_S结构 的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNeighborNetInfo(HI_MDM_NEIGHBOR_NET_INFO_S *pstInfo);
/**
* @ingroup  hct_mac
* @brief  获取邻居网络个数
*
* @par  描述:
* 获取邻居网络信息函数。
* @attention 作用产品类型为CCO,STA。
* @param  pstInfo [IN]/[OUT] 类型 #HI_U32 *，获取邻居网络个数。
* @retval   ret 类型 #HI_U32，取值：\n
                               @li #HI_ERR_SUCCESS 获取邻居网络个数成功\n
                               @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNeighBorNetNum(HI_U32 *pneighborNetNum);
/**
* @ingroup  hct_mac
* @brief  查询网络中站点的芯片管理ID
*
* @par  描述:
* 查询网络中站点的芯片管理ID
* @attention 作用产品类型为CCO。
* @param  pstQueryIdInfo [OUT] 类型 #HI_MDM_NETWORK_STA_ID_S *，站点的芯片管理ID信息。
* @retval   ret 类型 #HI_U32，取值：\n
                               @li #HI_ERR_SUCCESS 获取站点的芯片管理ID信息成功\n
                               @li #HI_ERR_FAILURE 获取站点的芯片管理ID信息失败\n
                               @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryStaID(HI_OUT HI_MDM_NETWORK_STA_ID_S *pstQueryIdInfo);

/**
 * @ingroup hct_mac
* @brief  模块管理ID信息的长度
*/
#define HI_MDM_MANAGEMENT_ID_SIZE            (24)

/**
* @ingroup  hct_mac
* @brief  根据站点MAC地址查询站点的管理ID信息
*
* @par  描述:
* 查询网络中站点的芯片管理ID
* @attention 作用产品类型为CCO。
* @param  mac [IN] 类型 #HI_U8 *，站点的MAC地址。
* @param  management_id [OUT] 类型 #HI_U8 *，站点的芯片管理ID信息。
* @retval   ret 类型 #HI_U32，取值：\n
                               @li #HI_ERR_SUCCESS 获取站点的芯片管理ID信息成功\n
                               @li #HI_ERR_FAILURE 获取站点的芯片管理ID信息失败\n
                               @li #HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryStaIDByMac(HI_U8 mac[HI_PLC_MAC_ADDR_LEN],HI_U8 management_id[HI_MDM_MANAGEMENT_ID_SIZE]);


/**
* @ingroup  hct_mac
* @brief   注册认证MAC地址回调函数
*
* @par  描述:
* 注册认证MAC地址回调函数
* @attention 作用产品类型为CCO。
* @param  pfnAuthCallback [IN] 类型 #HI_BOOL，认证MAC地址回调函数，为HI_NULL时表示取消注册\n
          @li pucMacAddr: MAC地址\n
          @li ucProductType: 产品类型 #HI_ND_TYPE_CCO/#HI_ND_TYPE_STA等\n
          @li ucMacType: MAC地址类型 #HI_MDM_METER_MAC/#HI_MDM_NV_MAC\n
          @li pParam: 用户参数 回调函数返回#HI_TRUE表示认证成功，返回#HI_FALSE表示认证失败。
* @param  pParam [IN] 类型 #HI_VOID，传入认证参数，#HI_NULL表示参数非法。
* @retval   ret 类型 #HI_U32，取值:\n
                                @li #HI_ERR_SUCCESS 表示注册成功\n
                                @li #HI_ERR_INVALID_PARAMETER 入参不可用。。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see pfnAuthCallback回调函数指针的定义。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterStaAuthCallback(HI_IN HI_BOOL (*pfnAuthCallback)(HI_U8 *pucMacAddr, HI_U8 ucProductType, HI_U8 ucMacType, HI_VOID *pParam), HI_IN HI_VOID *pParam);

/**
* @ingroup  hct_mac
* @brief   设置主节点地址
*
* @par  描述:
* 设置集中器下发的主节点地址到MAC层
* @attention 作用产品类型为CCO。
* @param  main_node_addr [IN] 类型 #HI_U8，主节点地址数组
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 执行成功，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see pfnAuthCallback回调函数指针的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMainNodeAddr(HI_IN HI_U8 main_node_addr[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief   设置使用上次的台区识别状态
*
* @par  描述:
* 设置使用上次的台区识别状态
* @attention 作用产品类型为CCO。
* @param  bIsUseLastTfStatus [IN] 类型 #HI_BOOL，是否使用上次的台区识别状态，#HI_TRUE将使用掉电前保存的台区识别状态，#HI_FALSE将置为识别未完成，重新开始识别。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 执行成功，#HI_ERR_FAILURE 设置失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see pfnAuthCallback回调函数指针的定义。
* @since HiMDL_V200R001C00
*/
HI_U32  HI_MDM_SetUseLastTfStatus(HI_IN HI_BOOL bIsUseLastTfStatus);

/**
* @ingroup  hct_mac
* @brief   检查SNR台区识别是否完成
*
* @par  描述:
* 检查SNR台区识别是否完成
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_BOOL #HI_TRUE 台区识别是完成，#HI_FALSE 台区识别没有完成。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsTfComplete(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  载波节点离线原因 未知
*/
#define HI_ND_OFFLINE_REASON_UNKNOWN            (0x00)
/**
 * @ingroup hct_mac
* @brief  载波节点离线原因 电表掉电
*/
#define HI_ND_OFFLINE_REASON_METER_POWEROFF     (0x01)
/**
 * @ingroup hct_mac
* @brief  载波节点离线原因 信道变化
*/
#define HI_ND_OFFLINE_REASON_PLC_CHANNEL        (0x02)
/**
 * @ingroup hct_mac
* @brief  载波节点在线状态变化 离线变为在线
*/
#define HI_ND_STATE_CHANGE_OFFLINE_TO_ONLINE    (0x00)
/**
 * @ingroup hct_mac
* @brief  载波节点在线状态变化 在线变为离线
*/
#define HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE    (0x01)

/**
 * @ingroup hct_mac
* @brief  发生变化节点参数结构体
*/
typedef struct
{
    HI_U8  aucMac[6];               /**<  发生状态改变的从节点MAC地址         */
    HI_U8  ucStateChange;        /**<  该节点的状态变化,取值参考HI_ND_STATE_CHANGE_XXX       */
    HI_U8  ucOfflineReason;      /**<  若为离线变为在线，离线原因，未知: #HI_ND_OFFLINE_REASON_UNKNOWN，\n
                                                             电表掉电: HI_ND_OFFLINE_REASON_METER_POWEROFF，信道变化: HI_ND_OFFLINE_REASON_PLC_CHANNEL  */
    HI_U32 ulOfflineDuration;    /**<  若为离线变为在线，离线时长，单位:秒  */

    HI_BOOL is_init_poweroff;   /**<  停电上报功能是否初始化，HI_TRUE表示初始化该功能，HI_FALSE表示未初始化该功能   */
    HI_U8   pad;
    HI_U16  pad1;
} HI_MDM_NETWORK_TOPOCHG_ENTRY_S;
/**
* @ingroup  hct_mac
* @brief   定义网络拓扑中节点状态变化函数的类型
*
* @par  描述:
* 定义网络拓扑中节点状态变化函数的类型
* @attention 作用产品类型为CCO。
* @param  entry [IN]/[OUT] 类型 #HI_MDM_NETWORK_TOPOCHG_ENTRY_S*，发生变化节点参数，详见#HI_MDM_NETWORK_TOPOCHG_ENTRY_S定义。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_Report_Topo_Change_Node_Fun 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Topo_Change_Node_Fun)(HI_IN HI_MDM_NETWORK_TOPOCHG_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief   注册网络拓扑中节点状态变化回调函数
*
* @par  描述:
* 注册网络拓扑中节点状态变化回调函数
* @attention 作用产品类型为CCO。
* @param  fun [IN] 类型 #HI_MDM_Report_Topo_Change_Node_Fun。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_Report_Topo_Change_Node_Fun 定义。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportTopoChangeNode(HI_IN HI_MDM_Report_Topo_Change_Node_Fun fun);

/**
* @ingroup hct_mac
* @brief  MAC地址来源：来自表地址
*/
#define HI_MDM_METER_MAC    (0)
/**
* @ingroup hct_mac
* @brief  MAC地址来源：来自NV
*/
#define HI_MDM_NV_MAC       (1)
/**
* @ingroup hct_mac
* @brief  MAC地址来源：来自采集器地址
*/
#define HI_MDM_CLT_MAC      (2) //来自采集器地址
/**
* @ingroup hct_mac
* @brief  MAC地址来源：来自主节点地址
*/
#define HI_MDM_CCO_MAIN_MAC (3)

/**
 * @ingroup hct_mac
* @brief  入网被拒绝节点的参数结构体
*/
typedef struct
{
    HI_U8  aucMac[6];       /**<  入网被拒节点的MAC地址         */
    HI_U8  ucMacType;     /**<  入网被拒节点的MAC地址 类型，来自表地址: #HI_MDM_METER_MAC，来自NV: #HI_MDM_NV_MAC，来自采集器地址: HI_MDM_CLT_MAC */
    HI_U8  ucProduct;       /**<  入网被拒节点的产品形态，参见CCO: #HI_ND_TYPE_CCO等         */
} HI_MDM_NETWORK_REJECT_ENTRY_S;

/**
* @ingroup  hct_mac
* @brief   定义网络拓扑中节点入网被拒绝函数的类型
*
* @par  描述:
* 定义网络拓扑中节点入网被拒绝函数的类型
* @attention 作用产品类型为CCO。
* @param  fun [IN] 类型 #HI_MDM_NETWORK_REJECT_ENTRY_S*，入网被拒绝节点的参数，详见#HI_MDM_NETWORK_REJECT_ENTRY_S定义。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_NETWORK_REJECT_ENTRY_S 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Reject_Node_Fun)(HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief   注册网络拓扑中节点入网被拒绝回调函数
*
* @par  描述:
* 注册网络拓扑中节点入网被拒绝回调函数
* @attention 作用产品类型为CCO。
* @param  fun [IN] 类型 #HI_MDM_Report_Reject_Node_Fun。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 获取成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_Report_Reject_Node_Fun 定义。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportRejectNode(HI_IN HI_MDM_Report_Reject_Node_Fun fun);

/**
 * @ingroup hct_mac
* @brief  PLC通道可用时拓扑变更状态 组网完成
*/
#define HI_MDM_MAC_NETWORK_FORMED         (1)
/**
 * @ingroup hct_mac
* @brief  PLC通道可用时拓扑变更状态 优化完成
*/
#define HI_MDM_MAC_NETWORK_EVALUATE_END   (2)
/**
 * @ingroup hct_mac
* @brief  PLC通道可用时拓扑变更状态 CCO优化中后,站点离线
*/
#define HI_MDM_MAC_STA_LEAVE              (3)
/**
 * @ingroup hct_mac
* @brief  PLC通道可用时拓扑变更状态 CCO优化中后,站点加入网络
*/
#define HI_MDM_MAC_STA_JOIN_NET           (4)
/**
 * @ingroup hct_mac
* @brief  PLC通道可用时拓扑变更状态结构体
*/
typedef struct
{
    HI_U8 ucStatus;            /**<  PLC通道可用时拓扑变更状态，组网完成: #HI_MDM_MAC_NETWORK_FORMED，\n
                                                     优化完成: #HI_MDM_MAC_NETWORK_EVALUATE_END，\n
                                                     CCO优化中后站点离线: #*HI_MDM_MAC_STA_LEAVE，CCO优化中后站点加入: #HI_MDM_MAC_STA_JOIN_NET*/
    HI_U8 pad;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]; //仅当ucStatus=HI_MDM_MAC_STA_JOIN_NET或 HI_MDM_MAC_STA_LEAVE时，aucMac才有效，加入网络站点的mac地址

}HI_MDM_TOPO_STATUS_CHG_ENTRY_S;

/**
* @ingroup  hct_mac
* @brief   定义通道可用时网络拓扑变化通知函数的类型
*
* @par  描述:
* 定义通道可用时网络拓扑变化通知函数的类型
* @attention 作用产品类型为CCO。
* @param  entry [IN]/[OUT] 类型 #HI_MDM_TOPO_STATUS_CHG_ENTRY_S*，通道可用参数，详见#HI_MDM_TOPO_STATUS_CHG_ENTRY_S定义。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_TOPO_STATUS_CHG_ENTRY_S 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Topo_Status_Chg_Fun)(HI_IN HI_MDM_TOPO_STATUS_CHG_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief 注册通道可用时网络拓扑变化通知
*
* @par  描述:
* 注册通道可用时网络拓扑变化通知
* @attention 作用产品类型为CCO。
* @param  fun [IN] 类型 #HI_MDM_Report_Topo_Status_Chg_Fun。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 同步对象参数设置成功。#HI_ERR_FAILURE 同步对象参数设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_Report_Topo_Status_Chg_Fun的定义。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportTopoStatusChg(HI_IN HI_MDM_Report_Topo_Status_Chg_Fun fun);

/**
* @ingroup  hct_mac
* @brief 设置组网类型可配置使能标记
*
* @par  描述:
* 设置组网类型可配置使能标记
* @attention 无。
* @param  fun [IN] 类型 #HI_BOOL，#HI_TRUE表示允许配置组网类型，#HI_FALSE表示不允许配置组网类型。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetTopoTypeFlag(HI_BOOL flag);

/**
* @ingroup  hct_mac
* @brief 获取组网类型可配置使能标记
*
* @par  描述:
* 获取组网类型可配置使能标记
* @attention 无。
* @param  fun [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_BOOL，取值#HI_TRUE表示允许配置组网类型，#HI_FALSE表示不允许配置组网类型。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetTopoTypeFlag(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  网络拓扑类型枚举
*/
typedef enum
{
    HI_MDM_TOPO_TREE = 0,
    HI_MDM_TOPO_STAR,
    HI_MDM_TOPO_MAX
} HI_MDM_TOPO_TYPE_E;

/**
* @ingroup  hct_mac
* @brief 设置组网类型
*
* @par  描述:
* 设置组网类型
* @attention 作用产品类型为CCO。
* @param  fun [IN] 类型 #HI_MDM_TOPO_TYPE_E，组网类型。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_TOPO_TYPE_E的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTopoType(HI_MDM_TOPO_TYPE_E enTopoType);

/**
* @ingroup  hct_mac
* @brief 获取组网类型
*
* @par  描述:
* 获取组网类型
* @attention 无。
* @param  fun [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_MDM_TOPO_TYPE_E，组网类型。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_TOPO_TYPE_E的定义。
* @since HiMDL_V200R001C00
*/
HI_MDM_TOPO_TYPE_E HI_MDM_GetTopoType(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   定义信标帧用户自定义条目编码函数的类型
*
* @par  描述:
* 定义信标帧用户自定义条目编码函数的类型
* @attention 无。
* @param  entry [IN] 类型 #HI_U8，缓冲区地址。
* @param  entry [IN] 类型 #HI_U16，缓冲区长度。
* @param  entry [OUT] 类型 #HI_U16，实际编码长度。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_Encode_Beacon_User_Fun)(HI_U8 *buf, HI_U16 buf_len, HI_U16 *offset);

/**
* @ingroup  hct_mac
* @brief   定义信标帧用户自定义条目解码函数的类型
*
* @par  描述:
* 定义信标帧用户自定义条目解码函数的类型
* @attention 无。
* @param  entry [IN] 类型 #HI_U8，缓冲区地址。
* @param  entry [IN] 类型 #HI_U16，缓冲区长度。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_Decode_Beacon_User_Fun)(HI_U8 *addr, HI_U16 len);

/**
 * @ingroup hct_mac
* @brief  信标帧用户自定义条目编解码函数注册结构体
*/
typedef struct
{
    HI_MDM_Encode_Beacon_User_Fun pfEncode;
    HI_MDM_Decode_Beacon_User_Fun pfDecode;
}HI_MDM_BEACON_USER_FUN_S;

/**
* @ingroup  hct_mac
* @brief 注册信标帧用户自定义条目编解码函数
*
* @par  描述:
* 注册信标帧用户自定义条目编解码函数
* @attention 无。
* @param  fun [IN] 类型 #HI_MDM_BEACON_USER_FUN_S。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_BEACON_USER_FUN_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterBeaconUserDef(HI_MDM_BEACON_USER_FUN_S *fun);

/**
* @ingroup  hct_mac
* @brief  设置发送静默状态和取消发送静默状态。
*
* @par  描述:
* 设置发送静默状态和取消发送静默状态。
* @attention 无。
* @param  ucSilenceStatus [IN] 类型 #HI_U8，#HI_MDM_ENABLE(1)表示静默状态，#HI_MDM_DISABLE(0)表示取消静默状态。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_INVALID_PARAMETER 入参不合法。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_ENABLE 及#HI_MDM_DISABLE的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSendSilenceStatus(HI_U8 ucSilenceStatus);

/**
* @ingroup  hct_mac
* @brief  获取发送静默状态。
*
* @par  描述:
* 获取发送静默状态。
* @attention 无。
* @param  pu8SilenceStatus [IN] 类型 #HI_U8，#HI_MDM_ENABLE(1)表示静默状态，#HI_MDM_DISABLE(0)表示取消静默状态。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取成功。#HI_ERR_INVALID_PARAMETER 入参不合法。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_ENABLE 及#HI_MDM_DISABLE的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSendSilenceStatus(HI_U8 *pu8SilenceStatus);

/**
* @ingroup  hct_mac
* @brief 获取当前网络状态。
*
* @par  描述:
* 获取当前网络状态。
* @attention 作用产品类型为CCO。
* @param  eNetStatus [IN] 类型 #HI_MDM_NETWORK_STATUS_E*，#FORMING_STATUS(1)表示组网中。#EVALUATE_STAUS(2)优化中。#EVALUATE_END(3)组网及优化完成。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取当前网络状态成功。#HI_ERR_INVALID_PARAMETER 指针入参为空。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_NETWORK_STATUS_E定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCurrentNetworkStatus(HI_MDM_NETWORK_STATUS_E *eNetStatus);

/**
* @ingroup  hct_mac
* @brief 注册抓包回调函数，要求注册的抓包处理函数执行时间尽可能短，避免影响中断响应的实时性。
* @par  描述:
* 注册抄控器抓包回调函数，要求注册的抓包处理函数执行时间尽可能短，避免对下中断处理
* @attention 无。
* @param  Function [IN] 类型 #HI_MDM_CAPTURE_FUNCTION 注册的回调函数，回调函数有3个参数,具体参数说明参看类型定义。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 同步对象参数设置成功。#HI_ERR_INVALID_PARAMETER 指针入参为空。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCapturePktFunction(HI_MDM_CAPTURE_FUNCTION Function);


/**
* @ingroup  hct_mac
* @brief 注销抓包回调函数
* @par  描述:
* 注销抓包回调函数
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 注销成功，其他值为错误码。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_CancelCapturePktFunction(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  控制抄控器搜网功能开关，当关闭搜网功能时，还可以设置抄控器的频段。
*
* @par  描述:
* 控制抄控器搜网功能开关，当关闭搜网功能时，还可以设置抄控器的频段。
* @attention 作用产品类型为NDM。
* @param  ucCtrlFlag [IN] 类型 #HI_U8，用于控制抄控器搜网功能的开关，0表示关闭搜网功能，1表示打开。
* @param  ucSetFreq [IN] 类型 #HI_U8， 用于设置工作模式0-5。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 同步对象参数设置成功，#HI_ERR_FAILURE 同步对象参数设置失败，#HI_ERR_INVALID_PARAMETER 入参不可用。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SearchNetworkCtrl(HI_U8 ucCtrlFlag, HI_U8 ucSetFreq);
/**
 * @ingroup hct_mac
* @brief  物理层测试模式
*/
typedef enum
{
    PHYSICAL_TEST_MODE_NONE = 0,
    PHYSICAL_TEST_MODE_APP_TO_COM = 1,  /**<转发当前帧应用层报文至应用层串口通道*/
    PHYSICAL_TEST_MODE_APP_TO_PLC = 2,  /**<转发当前帧应用层报文到PLC通道*/
    PHYSICAL_TEST_MODE_APP_TO_COM_LOOP = 3, /**<进入载波透传应用层串口自动测试模式，保持测试模式到"测试模式持续时间"后退出*/
    PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP = 4, /**<进入载波报文自动回复自动测试模式，保持测试模式到"测试模式持续时间"后退出*/
    PHYSICAL_TEST_MODE_EXIT = 5,            /**<进入MAC层透传模式，透传接收到的报文的MSDU到串口信道，保持测试模式持续时间"后退出*/
    PHYSICAL_TEST_MODE_SET_FREQ = 6,            /**<设置频段,当前只支持设置国网频段*/
    PHYSICAL_TEST_MODE_SET_TONE_MASK_MODE = 7,            /**<设置TONEMAP表，当前只支持设置国网频段*/
    PHYSICAL_TEST_MODE_LOOPBACK = 8,    /**<南网回环测试模式*/
    PHYSICAL_TEST_MODE_TRANSIMIT = 9,   /**<南网透明转发测试模式*/
}HI_MDM_PHYSICAL_TEST_MODE_EN;

/**
* @ingroup  hct_mac
* @brief  获取NTB以及同步状态
*
* @par  描述:
* 获取NTB及同步状态
* @attention 无。
* @param  isSync [IN]/[OUT] 类型 #HI_BOOL *，是否有同步对象。
* @retval   ret 类型 #HI_U32，站点有同步对象时，返回NTB值;没有同步对象时，返回0。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNtb(HI_BOOL *isSync);
/**
* @ingroup  hct_mac
* @brief  设置工作频段
*
* @par  描述:
* 设置工作频段,当前支持国网4个频段
* @attention 仅支持测试系统中使用，需要功能宏#PRODUCT_CFG_TTCN打开。\n
                设置频段不修改nv值，重启会失效。
* @param  freq_para [IN] 类型 #HI_U8，频段参数 0时为设置为国网全频，1时为设置为国网2.5-5.7m频段，\n
                                                2时为设置为国网0.7-3m频段，3时为设置为国网1.7-3m频段。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetWorkFreq(HI_U8 freq_para);

/**
* @ingroup  hct_mac
* @brief  设置工作频段(南网协议)
*
* @par  描述:
* 设置工作频段,当前支持设置频段:2-12m、2.5-5.7m、500k-3.7m
* @attention 仅支持测试系统中使用，需要功能宏#PRODUCT_CFG_TTCN打开。\n
                设置频段不修改nv值，重启会失效。
* @param  freq_para [IN] 类型 #HI_U8，频段参数 0时为设置为南网全频，1时为设置为南网2.5-5.7m频段，3时为设置为南网500k-3.7m频段。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetWorkFreqCsg(HI_U8 Freq);


/**
* @ingroup  hct_mac
* @brief  CCO设置全网工作频段
*
* @par  描述:
* CCO设置全网工作频段
* @attention 作用产品类型为CCO。
* @param  FreqMode [IN] 类型 #HI_U8，频段参数 0表示设置为国网全频，1表示设置为国网2.5M~5.7M频段，2表示设置为国网0.7M~3M频段，3表示设置为国网1.7M~3M频段
* @param  ChangeTime [IN] 类型 #HI_U32，全网切换频段延时时间，单位s，小于10s时，会采用0x22aNV中whole_network_change_freq_time项定义的默认值。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_FAILURE 返回失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetWholeNetWorkFreq(HI_IN HI_U8 FreqMode, HI_IN HI_U32 ChangeTime);

/**
* @ingroup  hct_mac
* @brief  CCO查询全网工作频段
*
* @par  描述:
*  CCO查询全网工作频段
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_U8，频段参数 0表示设置为国网全频，1表示设置为国网2.5M~5.7M频段，2表示设置为国网0.7M~3M频段，3表示设置为国网1.7M~3M频段
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetWholeNetWorkFreq(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  CCO查询全网NID
*
* @par  描述:
*  CCO查询全网NID
* @attention 作用产品类型为CCO。
* @param  无。
* @retval   ret 类型 #HI_U32，国网协议下NID，取值范围16~16777215
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetWholeNetNid(HI_VOID);

/**
* @ingroup hct_mac
* @brief  搜网模式设定
*/
typedef enum
{
    HI_SEARCH_NET_ACCESS_MODE_CCO = 0,             /**<表示CCO搜网接入模式，即仅搜索并连接CCO*/
    HI_SEARCH_NET_ACCESS_MODE_STA,                   /**<表示STA搜网接入模式，仅搜索并连接STA*/
}HI_SEARCH_NET_ACCESS_MODE_E;

/**
* @ingroup hct_mac
* @brief  搜网接收频段数组大小
*/

#define RCV_SEARCH_FREQ_NUM         (6)
/**
 * @ingroup hct_mac
* @brief  搜网查询条目结构体
*/
typedef struct
{
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];          /**<查询站点的mac 地址 */
    HI_U8 ucFreq;                               /**<查询站点的频段  */
    HI_U8 pad;                                  /**<保留位  */

    HI_U32 ucSnid;                              /**<查询站点网络号  */
}MAC_NDC_QUERY_INFO_S;

/**
 * @ingroup hct_mac
* @brief  搜网结果结构体
*/

typedef struct
{

    HI_U32   ulSnid;                                        /**<网络ID*/

    HI_U32   ulDiscoveryCnt;                               /**<搜查到该通信模块的次数*/

    HI_U8    ucIp[HI_IPV4_ADDR_LEN];                       /**<站点IP地址*/

    HI_U8    ucMac[HI_PLC_MAC_ADDR_LEN];                   /**<站点MAC地址*/
    HI_U8    ucDevId[HI_MDM_DEV_ID_LEN];                   /**<网络节点的设备标识*/

    HI_U8    ucChannelInfo;                                /**<信道质量，只记录信标帧的信道质量*/
    HI_U8    ucConnect;                                    /**<表示是否连接上*/
    HI_U8    ucProdType;                                   /**<标识产品类型: \n
                                                               CCO: #HI_FTM_PRODUCT_TYPE_C01       \n
                                                               STA: #HI_FTM_PRODUCT_TYPE_M101  */
    HI_U8    ucCurrentFreq;                                  /**<表示当前工作的网络频段*/

}MAC_NDC_MDL_RESULT_INFO_S;


/**
 * @ingroup hct_mac
* @brief  搜网状态结果转换
*/

typedef enum
{
    SEARCH_NET_STATUS_IDLE,             /**<未进入搜网前的状态*/
    SEARCH_NET_STATUS_SEARCH,           /**<搜网状态*/
    SEARCH_NET_STATUS_READY,            /**<连接后的状态*/
} SEARCH_NET_STATUS_E;


/**
* @ingroup  hct_mac
* @brief  启动搜网
*
* @par  描述:
* 启动搜网
* @attention 无
* @param  ucSetFreq [IN] 类型 #HI_U8，需要搜网的工作模式数组，其余值填充255
* @param  access_mode [IN] 类型 #HI_SEARCH_NET_ACCESS_MODE_E，搜网模式
* @retval   ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_SEARCH_NET_ACCESS_MODE_E 定义
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartNetSearch(HI_U8 ucSetFreq[RCV_SEARCH_FREQ_NUM],HI_SEARCH_NET_ACCESS_MODE_E access_mode);

/**
* @ingroup  hct_mac
* @brief  停止搜网
*
* @par  描述:
* 停止搜网,停止所有工作模式搜网，需传入有效的工作模式
* @attention 无
* @param  ucSetFreq [IN] 类型 #HI_U8，工作模式。
* @retval   ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StopNetSearch(HI_U8 ucSetFreq);

/**
* @ingroup  hct_mac
* @brief  获取搜网结果
*
* @par  描述:
* 获取搜网结果
* @attention 无
* @param  pst_query_info [IN] 类型 #MAC_NDC_QUERY_INFO_S *，搜网查询条目信息。
* @param  pst_query_info [OUT] 类型 #MAC_NDC_MDL_RESULT_INFO_S *，搜网查询条目信息。
* @retval  ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 参看MAC_NDC_QUERY_INFO_S，MAC_NDC_MDL_RESULT_INFO_S结构体定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetSearchResult(HI_IN MAC_NDC_QUERY_INFO_S *pst_query_info,
                                            HI_OUT MAC_NDC_MDL_RESULT_INFO_S *pst_query_result);

/**
* @ingroup  hct_mac
* @brief  搜网状态上报回调函数
* @par  描述:
* 本API接口使用场景：搜网状态发生变化时上报
* @attention 无。
* @param  search_net_state [IN] 类型 #HI_U8，搜网状态
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterNetSearchStateCallBack)(HI_U8 search_net_state);
/**
* @ingroup  hct_mac
* @brief  搜网状态上报注册接口
* @par  描述:
* 搜网状态上报注册接口
* @attention 无。
* @param    pfnCallback [IN] 类型 #HI_MDM_RegisterNetSearchStateCallBack， 搜网状态上报的回调函数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_RegisterNetSearchStateCallBack 回调函数。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterNetSearchStateInterface(HI_MDM_RegisterNetSearchStateCallBack);

/**
* @ingroup  hct_mac
* @brief  获取物理层测试模式
*
* @par  描述:
*  获取物理层测试模式
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U8，物理层测试模式，含义参考#HI_MDM_PHYSICAL_TEST_MODE_EN。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPhysicalTestMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置物理层测试模式
*
* @par  描述:
* 设置物理层测试模式
* @attention 无。
* @param   mode [IN] 类型 #HI_MDM_PHYSICAL_TEST_MODE_EN， 物理层测试模式。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnterPhyTestMode(HI_MDM_PHYSICAL_TEST_MODE_EN mode);
/**
* @ingroup  hct_mac
* @brief  退出物理层测试模式
*
* @par  描述:
* 退出物理层测试模式
* @attention 无。
* @param   无。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ExitPlcTest(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  获取帧Payload长度
*
* @par  描述:
*  获取帧Payload长度
* @attention 无。
* @param   ucType [IN] 类型 #HI_U8， 帧类型定义，信标帧: #BEACON_DT，SOF帧: #SOF_DT。
* @retval   ret 类型 #HI_U16，帧Payload长度。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_Get_PayloadLen(HI_U8 ucType);
/**
* @ingroup  hct_mac
* @brief  设置ToneMask模式
*
* @par  描述:
* 设置ToneMask模式
* @attention 无。
* @param  mode [IN] 类型 #HI_U8， ToneMask模式，国网全频模式: #TONE_MASK_MODE_1，国网2.5M~5.7M模式: #TONE_MASK_MODE_2，国网0.7M~3.0M模式: #TONE_MASK_MODE_3，国网1.7M~3.0M模式: #TONE_MASK_MODE_4。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetToneMaskMode(HI_U8 mode);

/**
* @ingroup  hct_mac
* @brief  设置是否将00结尾的mac地址转化为FE
*
* @par  描述:
* STA端设置MAC地址时是否将00结尾的mac地址转化为FE，CCO端MAC认证时是否将请求入网站点00结尾的mac地址转化为FE
* @attention 作用产品类型为CCO、STA，只可以在APP初始化时设置。
* @param  IsConvertMacAddr [IN] 类型 #HI_BOOL， 是否将00结尾的mac地址转化为FE，#HI_TRUE为将00结尾的mac地址转化为FE， #HI_FALSE 为不转换
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsConvertMacAddr(HI_BOOL IsConvertMacAddr);

/**
* @ingroup  hct_mac
* @brief  设置是否启用白名单为空标志位
*
* @par  描述:
* 设置是否启用白名单为空标志位
* @attention 作用产品类型为CCO。
* @param  WhiteListAssocSwitch [IN] 类型 #HI_BOOL， 是否启用白名单为空标志位，#HI_TRUE为开启， #HI_FALSE 为关闭
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetWhiteListAssocSwitch(HI_BOOL WhiteListAssocSwitch);
/**
* @ingroup  hct_mac
* @brief  电压跌落触发中断回调函数
* @par  描述:
* 本API接口使用场景：电压跌落时触发中断
* @attention 作用产品类型为STA。
* @param   #HI_U32，电压跌落
* @retval ret [OUT] 类型 #HI_S32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_S32 (*HI_MDM_VoltageDecFunc)(HI_U32);
/**
* @ingroup  hct_mac
* @brief  电压跌落触发中断注册接口
* @par  描述:
* 电压跌落触发中断注册接口
* @attention  作用产品类型为STA。
* @param    pfnCallback [IN] 类型 #HI_MDM_VoltageDecFunc， 电压跌落触发中断回调函数。
* @retval   ret 类型 #HI_U32，是否注册成功，#HI_ERR_SUCCESS:注册成功，#HI_ERR_NOT_SUPPORT:产品不支持，#HI_ERR_INVALID_PARAMETER:注册参数错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_VoltageDecFunc 回调函数。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_VoltageDecFuncReg(HI_MDM_VoltageDecFunc GpioCallBackFunc);

/**
* @ingroup  hct_mac
* @brief  设置是否启用网络满载标志位
*
* @par  描述:
* 设置是否启用网络满载标志位
* @attention 作用产品类型为CCO。
* @param  FullLoadAssocSwitch [IN] 类型 #HI_BOOL，是否启用网络满载标志位，#HI_TRUE为开启， #HI_FALSE 为关闭
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetFullLoadAssocSwitch(HI_BOOL FullLoadAssocSwitch);

/**
* @ingroup  hct_mac
* @brief  设置应用层认证功能
*
* @par  描述:
* 设置应用层认证功能
* @attention 作用产品类型为CCO。
* @param  IsAppAuth [IN] 类型 #HI_BOOL，是否启用应用层认证功能，#HI_TRUE为开启， #HI_FALSE 为关闭
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAppAuth(HI_BOOL IsAppAuth);

/**
* @ingroup  hct_mac
* @brief  是否允许关联
*
* @par  描述:
* 是否允许关联
* @attention 作用产品类型为CCO。
* @param  IsAllowAssoc [IN] 类型 #HI_BOOL，是否允许关联，#HI_TRUE为允许， #HI_FALSE 为不允许
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAllowAssoc(HI_BOOL IsAllowAssoc);

/**
* @ingroup  hct_mac
* @brief  获取是否允许加入网络
*
* @par  描述:
* 获取是否允许加入网络
* @attention 作用产品类型为CCO。
* @param  无
* @retval   ret 类型 #HI_BOOL，是否允许加入网络，#HI_TRUE为允许， #HI_FALSE 为不允许。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetAllowAssoc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置白名单个数
*
* @par  描述:
* 设置白名单个数
* @attention 作用产品类型为CCO。
* @param  WhiteListNum [IN] 类型 #HI_U32，白名单个数，取值范围0x0 ~ 0xFFFFFFFF。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAppWhiteListNum(HI_U32 WhiteListNum);

/**
* @ingroup  hct_mac
* @brief  获取CCO端根据网间协调帧监听到的邻居网络个数
*
* @par  描述:
* 获取CCO端根据网间协调帧监听到的邻居网络个数
* @attention 作用产品类型为CCO。
* @param  无
* @retval   ret 类型 #HI_U8，CCO端根据网间协调帧监听到的邻居网络个数，范围0 ~ 15。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNum(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取CCO由于STA上报网络冲突协调时，收集到的邻居网络个数
*
* @par  描述:
*  获取CCO由于STA上报网络冲突协调时，收集到的邻居网络个数
* @attention 作用产品类型为CCO，仅在CCO发送STA上报冲突协调时，会计算一次邻居网络个数。
* @param  无
* @retval   ret 类型 #HI_U8，CCO由于STA上报网络冲突协调时，收集到的邻居网络个数，范围0 ~ 15。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNumInConflict(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  站点根据FC收集到的邻居网络个数,CCO,STA均不包括本网络
*
* @par  描述:
*  站点根据FC收集到的邻居网络个数,CCO,STA均不包括本网络
* @attention 作用产品类型为CCO、STA。
* @param  无
* @retval   ret 类型 #HI_U8，站点根据FC收集到的邻居网络个数，范围0 ~ 15。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNumByFc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief    STA端注册检查是否允许对该CCO_MAC进行关联请求的函数
*
* @par  描述:
*  STA端注册检查是否允许对该CCO_MAC进行关联请求的函数
* @attention 作用产品类型为STA。
* @param  cco_mac [IN] 类型 #HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]，CCO的MAC地址。
* @retval   ret 类型 #HI_BOOL，是否允许对该CCO_MAC进行关联请求，#HI_TRUE为允许， #HI_FALSE 为不允许。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN)(HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  STA端注册检查是否允许对CCO_MAC进行关联
*
* @par  描述:
*  STA端注册检查是否允许对CCO_MAC进行关联
* @attention 作用产品类型为STA。
* @param   func [IN] 类型 #HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN，STA端注册检查是否允许对该CCO_MAC进行关联请求的函数。
* @retval   ret 类型 #HI_U32，是否注册成功，#HI_ERR_SUCCESS:注册成功，#HI_ERR_NOT_SUPPORT:产品不支持，#HI_ERR_INVALID_PARAMETER:注册参数错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterLockAssocCcoMacFunc(HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN func);

/**
* @ingroup  hct_mac
* @brief  是否启用STA端判断CCO_MAC是否进行关联限制功能
*
* @par  描述:
*  是否启用STA端判断CCO_MAC是否进行关联限制功能
* @attention 作用产品类型为STA。
* @param   SetSwitch [IN] 类型 #HI_BOOL，是否启用STA端判断CCO_MAC是否进行关联限制功能，#HI_TRUE表示启用，#HI_FALSE表示不启用
* @retval   ret 类型 #HI_U32，是否设置成功，#HI_ERR_SUCCESS表示设置成功，其他表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLockAssocCcoMacSwitch(HI_BOOL SetSwitch);

/**
* @ingroup  hct_mac
* @brief  通知开始快速关闭通知处理函数
*
* @par  描述:
* 通知开始快速关闭通知处理函数
* @attention 只在CCO上运行。
* @param  usMaxLoop [IN] 类型 #HI_U16，最大循环次数。
* @param  ucNotifyData [OUT] 类型 #HI_U8，通知时携带数据内容，范围0~127。
* @param  pfnCallback [OUT] 类型 #HI_MDM_pfnCcoFastNoticeCallback，通知结果回调上报函数。
* @param  pParam [IN] 类型 #HI_PVOID，用户自定义参数，注册时传入，回调时回传，不需要时可以为空。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 开始关闭通知成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartWholeNetFastNotice(HI_U16 usMaxLoop, HI_U8 ucNotifyData, HI_MDM_pfnCcoFastNoticeCallback pfnCallback, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  注册处理快速关闭通知数据回调函数
*
* @par  描述:
* 注册处理快速关闭通知数据回调函数
* @attention 只在STA上运行
* @param  pfnCallback [IN] 类型 #HI_MDM_pfnStaFastNoticeCallback，注册的回调函数。
* @param  pParam [IN] 类型 #HI_PVOID，用户自定义参数，注册时传入，回调时回传，不需要时可以为空。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 注册成功。#HI_ERR_INVALID_PARAMETER 指针入参为空。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterStaFastNoticeCallback(HI_MDM_pfnStaFastNoticeCallback pfnCallback, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  基于宽带快速发送数据的处理函数
*
* @par  描述:
* 基于宽带快速发送数据的处理函数,一般用于发送长度超过11字节的数据。
* @attention
* 发送参数中使用说明如下:
* 1、在发送广播报文时指定相位才是有效的，发送广播报文时BroadcastFlag标志置为1。
* 2、如果发送数据是已经封成MPDU格式发送的报文，IsMacEncode标志要置成TRUE,并且发送时要指定发频段、TMI、PB块数和PB块长度这些信息，否则置成FLASE不需填写这些字段。
* 3、如果期望接收到报文后抢占发发送，SendMode设置成1，否则设置成0。抢占发送需要用户自己确保链路空闲，否则会因为冲突导致发送失败。
* 4、其它参数可以指定也可以不指定，不指定时使用默认的值
* 5、对端接收报文后需要通过HI_MDM_ChlSetup注册的MDM通道的PLC业务端口上送，因此使用此接口快速发送前需要先注册上报通道处理函数。
* @param  MacBbPlcSendPara [IN] 类型 #HI_MDM_BBPLC_SEND_PARA_S *，快速发送传入的入参指针。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 宽带发送成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendOnBbPlc(HI_MDM_BBPLC_SEND_PARA_S *MacBbPlcSendPara);

/**
* @ingroup  hct_mac
* @brief  基于单载波快速发送处理函数
*
* @par  描述:
* 基于单载波快速发送处理函数
* @attention
* 1、如果期望接收到报文后抢占发发送，SendMode设置成1，否则设置成0。抢占发送需要用户自己确保链路空闲，否则会因为冲突导致发送失败。
* 2、单载波一次最大发送报文长度有限制不能超过64字节。
* 3、调用函数前确保单载波参数配置正确，设置单载波参数参看HI_MDM_ConfigScPara。
* @param  MacBbPlcSendPara [IN] 类型 #HI_MDM_BBPLC_SEND_PARA_S *，快速发送传入的入参指针。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 单载波快速发送成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendOnScPlc(HI_MAC_SC_SEND_PARA_S *MacScPlcSendPara);


/**
* @ingroup  hct_mac
* @brief  基于宽带快速发送不带PB块的扩展sack报文处理函数
*
* @par  描述:
* 基于宽带快速发送不带PB块处理函数。
* @attention
* 1、在发送广播报文时指定相位才是有效的，发送广播报文时BroadcastFlag标志置为1。
* 2、如果发送数据是已经封成MPDU格式发送的报文，IsMacEncode标志要置成TRUE,只发送长度为16字节的报文，否则置成FLASE需要指定发送数据长度，但不能超过11字节。
* 3、如果期望接收到报文后抢占发发送，SendMode设置成1，否则设置成0。抢占发送需要用户自己确保链路空闲，否则会因为冲突导致发送失败。
* 4、此函数的入参需要指定DataLength、Timeout、SendPhase、IsMacEncode，其它参数不关注。
* @param  MacBbPlcSendPara [IN] 类型 #HI_MDM_BBPLC_SEND_PARA_S *，快速发送传入的入参指针。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 发送成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendSackOnBbPlc(HI_MDM_BBPLC_SEND_PARA_S *MacBbPlcSendPara);

/**
* @ingroup  hct_mac
* @brief  注册单载波接收到快速发送数据后上报APP的回调函数
*
* @par  描述:
* 注册接收到快速发送数据后上报APP的回调函数
* @attention 无。
* @param  pfnCallback [IN] 类型 #HI_MDM_pfnFastSendCallback，注册的回调函数。
* @param  pData [IN] 类型 #HI_U8 *，用于存放上报数据的地址指针。
* @param  ucMaxDataLength [IN] 类型 #HI_U8，可存放数据的最大长度。
* @param  ucRportMode [IN] 类型 #HI_U8,期望上报数据模式，0表示剥去mac封装只上报应用层数据，1表示带mac层封装上报
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 注册成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterFastCallbackSc(HI_MDM_pfnFastSendCallback pfnCallback, HI_U8 *pData, HI_U8 ucMaxDataLength, HI_U8 ucRportMode);

/**
* @ingroup  hct_mac
* @brief  注册接收到快速发送扩展sack数据后上报APP的回调函数
*
* @par  描述:
* 注册接收到快速发送数据后上报APP的回调函数
* @attention
* 1、上报数据长度根据ucRportMode有差别，如果上报数据带MAC层封装则数长度为16，如果上报数据不带MAC层封装则数据长度固定为11，因此需要接收者判断数据的有效性。
* 2、上报参数中IsMacDecode和DataAddr是有效的，其它参数不用关注。
* 3、ucMaxDataLength长度要能保证存放下上报数，否则报文上报会失败。
* @param  pfnCallback [IN] 类型 #HI_MDM_pfnFastSendCallback，注册的回调函数。
* @param  pData [IN] 类型 #HI_U8 *，用于存放上报数据的地址指针。
* @param  ucMaxDataLength [IN] 类型 #HI_U8，可存放数据的最大长度。
* @param  ucRportMode [IN] 类型 #HI_U8,期望上报数据模式，0表示剥去mac封装只上报应用层数据，1表示带mac层封装上报
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 注册成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterFastCallbackSack(HI_MDM_pfnFastSendCallback pfnCallback, HI_U8 *pData, HI_U8 ucMaxDataLength, HI_U8 ucRportMode);

/**
* @ingroup  hct_mac
* @brief  控制MAC层停止数据发送指定时间。
*
* @par  描述:
* 控制MAC层停止数据发送指定时间。
* @attention
* 1、要求指定停止的时间不能超过1分钟，否则可能导致站点离线。
* 2、超过1分钟，按最大1分钟进行设置。
* @param  DisableTime [IN] 类型 #HI_U32，指定MAC层停止发送的时间单位ms，取值范围0-60000ms。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置停止发送成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_DisableSending(HI_IN HI_U32 DisableTime);

/**
* @ingroup  hct_mac
* @brief  恢复MAC层数据发送功能。
*
* @par  描述:
* 恢复MAC层数据发送功能。
* @attention 无。
* @param  无
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnableSending(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置单载波使能和发送速率。
*
* @par  描述:
* 设置单载波发送速率,具体配置参数需要同芯片开发人员确认。
* @attention 无。
* @param  ScPara [IN] 类型 #HI_MAC_SC_PARA_S *，设置单载波速率参结构指针。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置单载波参数成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ConfigScPara(HI_IN HI_MAC_SC_PARA_S *ScPara);

/**
* @ingroup  hct_mac
* @brief  获取本站点网络属性信息
*
* @par  描述:
* 获取本站点网络属性信息
* @attention
* @param  AttributeInfo [IN] 类型 #HI_MDM_NETWORK_ATTRIBUTE_S *，存放获取网络属性信息的指针。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取网络属性成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetworkAttribute(HI_MDM_NETWORK_ATTRIBUTE_S *AttributeInfo);


/**
* @ingroup  hct_mac
* @brief  查询TOPO中子站点最多的代理站点的tei和mac
*
* @par  描述:
* 查询TOPO中子站点最多的代理站点tei和mac,此函数只运行在CCO.
* @attention
* @param  Tei [OUT] 类型 #HI_U16 *，查询到的代理TEI。
* @param  Mac [OUT] 类型 #HI_U8，查询到的代理MAC。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 查询子站点最多代理成功。#HI_ERR_INVALID_PARAMETER 入参无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryMostChildProxy(HI_U16 *Tei, HI_U8 Mac[6]);

/**
* @ingroup  hct_mac
* @brief  设置是否恢复V200协议标识，非能源版本使用。
*
* @par  描述:
* 设置是否恢复V200协议标识
* @attention 作用产品类型为CCO。
* @param  IsRecoverPtcl [IN] 类型 #HI_BOOL， 是否恢复V200协议标识，#HI_TRUE表示恢复，#HI_FALSE表示不恢复。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsRecoverPtcl(HI_BOOL IsRecoverPtcl);

/**
* @ingroup  hct_mac
* @brief  获取是否恢复V200协议标识，非能源版本使用。
*
* @par  描述:
*  获取是否恢复V200协议标识
* @attention 作用产品类型为CCO。
* @param  无
* @retval   ret 类型 #HI_BOOL，是否恢复V200协议标识，#HI_TRUE表示恢复，#HI_FALSE表示不恢复。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetIsRecoverPtcl(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置PHY低功耗开关
*
* @par  描述:
* 设置PHY低功耗开关
* @attention 作用产品类型为STA，站点仅在未入网状态下低功耗生效，不支持动态打开仅支持动态关闭。
* @param  PhyLowPowerSwtich [IN] 类型 #HI_BOOL， PHY低功耗开关，#HI_FALSE表示关闭。
* @retval   ret 类型 #HI_U32，发消息设置PHY低功耗开关的返回值，#HI_ERR_SUCCESS表示设置成功，更多返回值参见hi_errno.h。。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPhyLowPowerSwtich(HI_BOOL PhyLowPowerSwtich);

/**
* @ingroup  hct_mac
* @brief  获取PHY低功耗开关
*
* @par  描述:
*  获取PHY低功耗开关
* @attention 作用产品类型为STA。
* @param  无
* @retval   ret 类型 #HI_BOOL，PHY低功耗开关，#HI_TRUE表示打开，#HI_FALSE表示关闭。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetPhyLowPowerSwitch(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  无抓包模式
*/
#define HI_MAC_NO_CAPTURE_MODE              (0)
/**
 * @ingroup hct_mac
* @brief  单PB块抓包模式
*/
#define HI_MAC_CAPTURE_SINGLE_PB_MODE       (1)
/**
 * @ingroup hct_mac
* @brief  多PB块抓包模式
*/
#define HI_MAC_CAPTURE_MULTI_PB_MODE        (2)
/**
 * @ingroup hct_mac
* @brief  多PB块截包模式
*/
#define HI_MAC_INTERCEPT_MULTI_PB_MODE      (3)
/**
 * @ingroup hct_mac
* @brief  多PB块回环模式
*/
#define HI_MAC_INTERCEPT_LOOP_MODE          (4)
/**
 * @ingroup hct_mac
* @brief  翻转环回测试模式，南网协议测试模式使用。
*/
#define HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  (5)

/**
* @ingroup  hct_mac
* @brief  设置抓包模式
*
* @par  描述:
* 设置抓包模式
* @attention 无。
* @param  RxCaptureMode [IN] 类型 #HI_U8，取值:\n @li #HI_MAC_NO_CAPTURE_MODE 表示无抓包模式\n
                                                  @li #HI_MAC_CAPTURE_SINGLE_PB_MODE 表示单PB块抓包模式\n
                                                  @li #HI_MAC_CAPTURE_MULTI_PB_MODE 表示多PB块抓包模式\n
                                                  @li #HI_MAC_INTERCEPT_MULTI_PB_MODE 表示多PB块抓包，并且抓包后丢弃该报文模式\n
                                                  @li #HI_MAC_INTERCEPT_LOOP_MODE 表示多PB块环回测试模式\n
                                                  @li #HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  表示翻转环回测试模式。
* @retval   ret 类型 #HI_U32，\n
                                      @li  #HI_ERR_SUCCESS 设置成功\n
                                      @li  #HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCaptureMode(HI_U8 RxCaptureMode);

/**
* @ingroup  hct_mac
* @brief  获取抓包模式
*
* @par  描述:
* 获取抓包模式
* @attention 无。
* @param  无。
* @retval   RxCaptureMode [IN] 类型 #HI_U8，取值:\n @li #HI_MAC_NO_CAPTURE_MODE 表示无抓包模式\n
                                                  @li #HI_MAC_CAPTURE_SINGLE_PB_MODE 表示单PB块抓包模式\n
                                                  @li #HI_MAC_CAPTURE_MULTI_PB_MODE 表示多PB块抓包模式\n
                                                  @li #HI_MAC_INTERCEPT_MULTI_PB_MODE 表示多PB块抓包，并且抓包后丢弃该报文模式\n
                                                  @li #HI_MAC_INTERCEPT_LOOP_MODE 表示多PB块环回测试模式\n
                                                  @li #HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  表示翻转环回测试模式。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm.h文件中#HI_MDM_PTCL_VER_100等定义。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetCaptureMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置拦截MSDU报文开关
*
* @par  描述:
* 设置拦截MSDU报文开关
* @attention 无。
* @param  InterceptMsduSwitch [IN] 类型 #HI_BOOL。
* @retval   无
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetInterceptMsdu(HI_BOOL InterceptMsduSwitch);

/**
 *@ingroup hct_mac
 * 抓取MSDU内容信息
 */
typedef struct
{
    HI_U16 protocol_ver;        /**< MSDU协议类型                 */
    HI_U16 msdu_size;           /**< MSDU长度                 */

    HI_U8  *msdu_addr;          /**< MSDU内容缓存地址                 */
}HI_MDM_CAPTURE_MSDU_INFO_ST;

/**
* @ingroup  hct_mac
* @brief  抓取MSDU内容注册函数
*
* @par  描述:
* 抓取MSDU内容注册函数
* @attention 无。
* @param  CaptureMsduInfo [IN] 类型 #HI_MDM_CAPTURE_MSDU_INFO_ST，MSDU信息信息
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see EQUIP_ZERO_NTB_NUM 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*MAC_CAPTURE_MSDU_FUN)(HI_MDM_CAPTURE_MSDU_INFO_ST *CaptureMsduInfo);

/**
* @ingroup  hct_mac
* @brief  注册抓取MSDU内容的接口
*
* @par  描述:
* 注册抓取MSDU内容的接口
* @attention 无。
* @param  CaptureMsudInterface [IN] 类型 #MAC_CAPTURE_MSDU_FUN，MSDU抓包接口。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 注册成功，#HI_ERR_FAILURE 注册失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCaptureMsduInterface(MAC_CAPTURE_MSDU_FUN CaptureMsudInterface);
/**
* @ingroup  hct_mac
* @brief  去注册抓取MSDU内容的接口
*
* @par  描述:
* 去注册抓取MSDU内容的接口
* @attention 无。
* @param  CaptureMsudInterface [IN] 类型 #MAC_CAPTURE_MSDU_FUN，MSDU抓包接口。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 去注册成功，#HI_ERR_FAILURE 去注册失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_UnregisterCaptureMsduInterface(MAC_CAPTURE_MSDU_FUN CaptureMsudInterface);

/**
* @ingroup  hct_mac
* @brief  获取站点是否加入网络
*
* @par  描述:
* 获取站点是否加入网络
* @attention 作用产品类型为STA。
* @param  无
* @retval   ret 类型 #HI_BOOL，是否加入网络，#HI_TRUE表示加入网络，#HI_FALSE表示未加入网络。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsStaJoinNet(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  判断模块是否为精准过零电路
*
* @par  描述:
*  判断模块是否为精准过零电路
* @attention 作用产品类型为CCO,STA。
* @param  无
* @retval   ret 类型 #HI_BOOL，是否为精准过零电路，#HI_TRUE表示为精准过零电路，#HI_FALSE表示为非精准过零电路。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsPreciseZeroCross(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取管理ID
*
* @par  描述:
* 获取管理ID
* @attention
* @param  my_management_id [OUT] 类型 #HI_U8， 管理ID
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示获取成功，#其他表示获取失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetMyManagementId(HI_U8 *my_management_id);
/**
* @ingroup  hct_mac
* @brief  设置管理ID
*
* @par  描述:
* 设置管理ID
* @attention 工装使用，配置管理ID
* @param  my_management_id [IN] 类型 #HI_U8， 管理ID
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMyManagementId(HI_U8 *my_management_id);

/**
* @ingroup  hct_mac
* @brief  加载国网和V100协议
*
* @par  描述:
* 加载国网和V100协议
* @attention 加载国网和V100协议
* @param  无
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 加载成功，#HI_ERR_FAILURE 加载失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  加载南网和V100协议
*
* @par  描述:
* 加载国网和V100协议
* @attention 加载南网网和V100协议
* @param  无
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 加载成功，#HI_ERR_FAILURE 加载失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadCsgAndV100Protocol(HI_VOID);


/**
* @ingroup  hct_mac
* @brief  加载快速广播功能
*
* @par  描述:
* 加载快速广播功能，对快速广播中的单载波、扩展sack报文收发功能进行初始化。
* @attention
* 使用快速广播功能，首先在初始化中调用此接口，否则快速广播功能不可用。
* * @param  无
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 加载成功，#HI_ERR_FAILURE 加载失败，更多返回值定义参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendInit(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  将nv中的snid置为0
*
* @par  描述:
* 将nv中的snid置为0
* @attention CCO工装使用，将nv中的snid（包含nid）置为0
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ResetSnidNv(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  注册停电上报功能(非电容模块)相关函数
*
* @par  描述:
*       注册停电上报功能(非电容模块)相关函数
* @attention 停电上报功能(非电容模块)注册接口，需要配置相应的nv开关\n
*            将CCO 0x408 nv中ucReportEnable 的值设置为1\n
*              STA 0x418 nv中ucEnable 的值设置为1.
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PowerOffRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置站点状态为保护状态
*
* @par  描述:
*       设置站点状态为保护状态
* @attention 处理大批量掉线站点时，探测成功个数超过50%时 设置站点进入保护状态
* @param   mac_addr [IN] 类型 #HI_U8， 站点mac地址
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NM_PowerOffSetNodeProtectingState(HI_U8 *mac_addr);

/**
* @ingroup  hct_mac
* @brief  模块进行上电过零检测
*
* @par  描述:
* 模块进行上电过零检测
* @attention CCO,STA,NDM均在上电阶段进行过零检测
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示启用该检测功能成功，#HI_ERR_BUSY表示已经启动，不允许再次下发，
*#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ZeroCrossAutoCheck(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  国网和V100协议下物理相位识别功能配置
*
* @par  描述:
* 国网和V100协议下物理相位识别功能配置
* @attention CCO,STA产品物理相位识别功能配置
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功,#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PhaseIdentifyRegSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  南网和V100协议下物理相位识别功能配置
*
* @par  描述:
* 南网和V100协议下物理相位识别功能配置
* @attention CCO,STA产品物理相位识别功能配置
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功,#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PhaseIdentifyRegCsgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  国网和V100协议下无扰NTB识别功能配置
*
* @par  描述:
* 国网和V100协议下无扰NTB识别功能配置
* @attention CCO,STA产品无扰NTB识别功能配置
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功,#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TfpRegSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  南网和V100协议下无扰NTB识别功能配置
*
* @par  描述:
* 南网和V100协议下无扰NTB识别功能配置
* @attention CCO,STA产品无扰NTB识别功能配置
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功,#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TfpRegCsgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  加载无扰台区识别SNR组件
*
* @par  描述:
* 加载无扰台区识别SNR组件
* @attention CCO,STA产品无扰台区识别SNR功能组件加载
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示加载成功,#其他表示加载失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfSnrComponent(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  加载无扰台区识别NTB分布式组件
*
* @par  描述:
* 加载无扰台区识别NTB分布式组件
* @attention CCO,STA产品无扰台区识别NTB分布式组件加载
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示加载成功,#其他表示加载失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfNtbDistComponent(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  加载无扰台区识别北京NTB协议
*
* @par  描述:
* 加载无扰台区识别北京NTB协议
* @attention CCO,STA产品无扰台区识别北京NTB协议加载
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示加载成功,#其他表示加载失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfNtbProtocolBeijing(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  大电容停电上报功能配置
*
* @par  描述:
* 大电容停电上报功能配置
* @attention STA产品下停电上报功能配置
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功,#其他表示启用失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PowerFailureCheckRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  指示灯收发，异常状态功能配置
*
* @par  描述:
* 指示灯收发，异常状态功能配置
* @attention指示灯收发，异常状态功能配置
* @param   无
* @retval 无
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LedCtrlRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取是否为能源模式
*
* @par  描述:
* 获取是否为能源模式
* @attention 判断是否为能源模式
* @param   无
* @retval  ret 类型 #HI_BOOL，#HI_TRUE 表示为能源模式,#其他表示非能源模式
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetIsEnergyMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 能源应用场景下，用于CCO端启动停止芯片探测开关
*
* @par  描述:
* 能源应用场景下，用于CCO端启动停止芯片探测开关
* @attention 无。
* @param  setflag[IN] 类型 #HI_TRUE，表示启动探测,#HI_FALSE，表示停止探测。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 设置成功，#HI_ERR_NOT_SUPPORT 非能源场景下不支持，#HI_ERR_BUSY已经启动，不允许再次启动
* #其他返回值表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetChipDetectSwitch(HI_BOOL setflag);

/**
 *@ingroup hct_mac
 * 芯片探测结果
 */
typedef enum
{
    HI_MDM_CHIP_VER_UNKOWN = 0,         /**<表示没有探测到站点*/
    HI_MDM_HI3911V100_CHIP_VER = 1,     /**<bit0有值，表示探测到Hi3911V100芯片*/
    HI_MDM_HI3911V200_CHIP_VER = 2,     /**<bit1有值，表示探测到Hi3911V200芯片*/
    HI_MDM_HI3921V100_CHIP_VER = 4,     /**<bit2有值，表示探测到Hi3921V100芯片*/
}HI_MDM_CHIP_VER_BIT_OFFSET_E;

/**
* @ingroup  hct_mac
* @brief 能源应用场景下，用于CCO端获取芯片探测结果
*
* @par  描述:
* 能源应用场景下，用于CCO端获取芯片探测结果
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U32，参见#HI_MDM_CHIP_VER_BIT_OFFSET_E枚举值中定义。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetChipDetectVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 能源应用场景下，CCO端清除探测到芯片版本信息
*
* @par  描述:
* 能源应用场景下，CCO端清除探测到芯片版本信息
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示清除成功,#其他表示清除失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearChipDetectVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 能源应用场景下，用于设置是否允许CCO信标发送开关
*
* @par  描述:
* 能源应用场景下，用于CCO产品形态下设置信标发送开关
* @attention 无。
* @param  setflag[IN] 类型 #HI_TRUE，表示不允许发送信标,#HI_FALSE，表示允许发送信标。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功,#其他表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSendBeaconSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief 能源应用场景下，用于设置STA站点离线判断时间，用于设置接收信标，以及接收代理信息进行离线判断
*
* @par  描述:
* 能源应用场景下，用于STA站点设置判断站点离线时间，设置check_bcn_time,check_proxy_time最小值分别为60s,120s,
* 超过NV配置参数值后，按照NV中值进行配置。
* @attention 无。
* @param  setflag[IN] 类型 #HI_TRUE，表示设置生效,#HI_FALSE,表示设置不生效。
*@param  check_bcn_time[IN] 类型#HI_U32, 表示根据接收信标检测站点离线时间，单位ms，最小为60s。
*@param  check_proxy_time[IN]类型 #HI_U32, 表示根据接收代理信息检测站点离线时间，单位ms，最小为120s。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功,#HI_ERR_INVALID_PARAMETER表示参数错误,#HI_ERR_NOT_SUPPORT表示不支持,其他表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCheckOfflineTime(HI_BOOL setflag,HI_U32 check_bcn_time,HI_U32 check_proxy_time);

/**
* @ingroup  hct_mac
* @brief 获取收集到来自CCO的报文个数
*
* @par  描述:
* 在STA端查看收集到来自CCO的报文个数
* @attention 无。
* @param  无。
* @retval   ret 类型 #HI_U32，获取到CCO的报文个数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCCOPacketCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 设置能源版本是否启用在满载并且没有被锁定情况下，站点主动离线功能。
*
* @par  描述:
* 在STA端设置能源版本是否启用在满载并且没有被锁定情况下，站点主动离线功能。
* @attention 无。
* @param  setflag[IN] 类型 #HI_TRUE，表示设置生效,#HI_FALSE,表示设置不生效。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功，#HI_ERR_NOT_SUPPORT表示产品 不支持
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetOfflineByUnlockInEnergyMode(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief  设置信标最大发送次数
*
* @par  描述:
* 设置信标最大发送次数
* @attention 无。
* @param  count [IN] 类型 #HI_U8，信标最大发送次数，取值范围[1,3]。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconMaxSendCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  设置信标时隙自适应标记
*
* @par  描述:
* 设置信标时隙自适应标记
* @attention 作用产品类型为CCO，该接口仅在国网频段下生效。
* @param  count [IN] 类型 #HI_BOOL，取值HI_TRUE表示使能信标时隙自适应，取值HI_FALSE表示去使能信标时隙自适应。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAutoBeaconDurationFlag(HI_BOOL flag);

/**
* @ingroup  hct_mac
* @brief  获取信标时隙自适应标记
*
* @par  描述:
* 获取信标时隙自适应标记
* @attention 作用产品类型为CCO，该接口仅在国网频段下可以获取自适应值。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_BOOL，取值HI_TRUE表示使能信标时隙自适应，取值HI_FALSE表示去使能信标时隙自适应。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetAutoBeaconDurationFlag(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  路由参数集的个数
*/
#define HI_MDM_ROUTE_PARAM_GROUP_NUM (6)

/**
 * @ingroup hct_mac
* @brief  路由参数定义结构体
*/
typedef struct
{
    HI_U16 station_cnt;                    /**<站点个数(包括CCO)           */
    HI_U16 evaluate_cycle_len;             /**<评估周期长度 ，单位:秒           */
    HI_U8 proxy_send_dk_cycle_len;         /**<代理站点发送发现列表报文周期， 单位:秒           */
    HI_U8 sta_send_dk_cycle_len;           /**<发现站点发送发现列表报文周期， 单位:秒           */
    HI_U8 pad[2];
}HI_MDM_ROUTE_PARAM_S;

/**
* @ingroup  hct_mac
* @brief  设置路由参数
*
* @par  描述:
* 设置路由参数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_MDM_ROUTE_PARAM_S*，路由参数数组。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_ROUTE_PARAM_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRouteParam(HI_MDM_ROUTE_PARAM_S param[HI_MDM_ROUTE_PARAM_GROUP_NUM]);

/**
* @ingroup  hct_mac
* @brief  获取路由参数
*
* @par  描述:
* 获取路由参数
* @attention 作用产品类型为CCO。
* @param  count [OUT] 类型 #HI_MDM_ROUTE_PARAM_S*，路由参数数组。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取成功。#HI_ERR_FAILURE 获取失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_ROUTE_PARAM_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRouteParam(HI_MDM_ROUTE_PARAM_S param[HI_MDM_ROUTE_PARAM_GROUP_NUM]);

/**
* @ingroup  hct_mac
* @brief  设置每个路由周期发送的心跳个数
*
* @par  描述:
* 设置每个路由周期发送的心跳个数
* @attention 无。
* @param  count [IN] 类型 #HI_U8，每个路由周期发送的心跳个数，取值范围[4,8]。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功，其他值表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetHeartbeatCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  获取每个路由周期发送的心跳个数
*
* @par  描述:
* 获取每个路由周期发送的心跳个数
* @attention 无。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U8，每个路由周期发送的心跳个数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetHeartbeatCnt(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  网管报文优先级定义结构体
*/
typedef struct
{
    HI_U8 assoc_priority;           /**<关联请求报文优先级           */
    HI_U8 discovery_priority;       /**<发现列表报文优先级           */
}HI_MDM_NM_PKT_PRIORITY_S;

/**
* @ingroup  hct_mac
* @brief  设置网管报文优先级
*
* @par  描述:
* 设置网管报文优先级
* @attention 无。
* @param  count [IN] 类型 #HI_MDM_NM_PKT_PRIORITY_S*，网管报文优先级。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_NM_PKT_PRIORITY_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNmPktPriority(HI_MDM_NM_PKT_PRIORITY_S *param);

/**
* @ingroup  hct_mac
* @brief  获取网管报文优先级
*
* @par  描述:
* 获取网管报文优先级
* @attention 无。
* @param  count [OUT] 类型 #HI_MDM_NM_PKT_PRIORITY_S*，路由参数数组。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取成功。#HI_ERR_FAILURE 获取失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_NM_PKT_PRIORITY_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNmPktPriority(HI_MDM_NM_PKT_PRIORITY_S *param);

/**
* @ingroup  hct_mac
* @brief  设置网间协调开关
*
* @par  描述:
* 设置网间协调开关
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_BOOL，取值HI_TRUE表示打开网间协调，取值HI_FALSE表示关闭网间协调。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetNetCoordinateSwitch(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  获取网间协调开关
*
* @par  描述:
* 获取网间协调开关
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_BOOL，取值HI_TRUE表示打开网间协调，取值HI_FALSE表示关闭网间协调。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetNetCoordinateSwitch(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置协议探测开关
*
* @par  描述:
* 设置协议探测开关
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_BOOL，取值HI_TRUE表示打开协议探测，取值HI_FALSE表示关闭协议探测。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetPtclDetectSwtich(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  获取协议探测开关
*
* @par  描述:
* 获取协议探测开关
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_BOOL，取值HI_TRUE表示打开协议探测，取值HI_FALSE表示关闭协议探测。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetPtclDetectSwtich(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置扩展帧间隔时间
*
* @par  描述:
* 设置扩展帧间隔时间
* @attention 无。
* @param  count [IN] 类型 #HI_U16，扩展帧间隔时间，单位是us。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetEifsUS(HI_U16 eifs_us);

/**
* @ingroup  hct_mac
* @brief  获取扩展帧间隔时间
*
* @par  描述:
* 获取扩展帧间隔时间
* @attention 无。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U16，扩展帧间隔时间，单位是us。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetEifsUS(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  调制参数定义结构体
*/
typedef struct
{
    HI_U8 PB_72_TMI;        /**<PB72           */
    HI_U8 PB_136_TMI;       /**<PB136           */
    HI_U8 PB_264_TMI;       /**<PB264           */
    HI_U8 PB_520_TMI;       /**<PB520           */
}HI_MDM_BEACON_TMI_S;

/**
* @ingroup  hct_mac
* @brief  设置调制参数
*
* @par  描述:
* 设置调制参数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_MDM_BEACON_TMI_S*，调制参数。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_BEACON_TMI_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconTMI(HI_MDM_BEACON_TMI_S *tmi);

/**
* @ingroup  hct_mac
* @brief  获取调制参数
*
* @par  描述:
* 获取调制参数
* @attention 作用产品类型为CCO。
* @param  count [OUT] 类型 #HI_MDM_BEACON_TMI_S*，调制参数。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取成功。#HI_ERR_FAILURE 获取失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_BEACON_TMI_S的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetBeaconTMI(HI_MDM_BEACON_TMI_S *tmi);

/**
 * @ingroup hct_mac
* @brief  发送发现列表使用的TMI模式
*/
typedef enum
{
    HI_MDM_DISCOVERY_TMI_CHANGE = 0,            /**<动态TMI策略           */
    HI_MDM_DISCOVERY_TMI_LOW_LATENCY,           /**<低时延TMI策略           */
    HI_MDM_DISCOVERY_TMI_HIGH_DEPENDENCE,       /**<高可靠性TMI策略           */
    HI_MDM_DISCOVERY_TMI_MAX
}HI_MDM_DISCOVERY_TMI_MODE;

/**
* @ingroup  hct_mac
* @brief  设置发送发现列表使用的TMI模式
*
* @par  描述:
* 设置发送发现列表使用的TMI模式
* @attention 无。
* @param  count [IN] 类型 #HI_MDM_DISCOVERY_TMI_MODE*，发送发现列表使用的TMI模式。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_DISCOVERY_TMI_MODE的定义。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNmDiscoveryTMIMode(HI_MDM_DISCOVERY_TMI_MODE mode);

/**
* @ingroup  hct_mac
* @brief  获取发送发现列表使用的TMI模式
*
* @par  描述:
* 获取发送发现列表使用的TMI模式
* @attention 无。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_MDM_DISCOVERY_TMI_MODE，发送发现列表使用的TMI模式。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_DISCOVERY_TMI_MODE的定义。
* @since HiMDL_V200R001C00
*/
HI_MDM_DISCOVERY_TMI_MODE HI_MDM_GetNmDiscoveryTMIMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置相位数
*
* @par  描述:
* 设置相位数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_U8，相位数，取值范围为1或3
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功，其他值表示设置失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPhaseCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  获取相位数
*
* @par  描述:
* 获取相位数
* @attention 无。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U8，相位数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPhaseCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置功率检测门限
*
* @par  描述:
* 设置功率检测门限
* @attention 无。
* @param  count [IN] 类型 #HI_U8，功率检测门限。
* @retval    ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 设置成功。#HI_ERR_FAILURE 设置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerDetectThreshold(HI_U8 threshold);

/**
* @ingroup  hct_mac
* @brief  获取功率检测门限
*
* @par  描述:
* 获取功率检测门限
* @attention 无。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U8，功率检测门限。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPowerDetectThreshold(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置并行检测开关
*
* @par  描述:
* 设置并行检测开关
* @attention 无。
* @param  count [IN] 类型 #HI_BOOL，取值HI_TRUE表示打开并行检测，取值HI_FALSE表示关闭并行检测。
* @retval   ret 类型 #HI_VOID。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetParallelDetectSwitch(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  获取并行检测开关
*
* @par  描述: 获取并行检测开关
* @attention 无。
* @param  on_off [OUT] 类型 #HI_BOOL*，并行检测开关。
* @retval   ret 类型 #HI_U32，取值#HI_ERR_SUCCESS 获取成功。#HI_ERR_FAILURE 获取失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetParallelDetectSwitch(HI_BOOL *on_off);


/**
*@ingroup hct_mac
* 获取收发机发送报文统计
*/
typedef struct
{
    HI_U32 unicast_count;                       /**<单播报文个数           */
    HI_U32 broadcast_count;                     /**<广播报文个数           */

    HI_U32 short_packet_count;                  /**<发送短包个数           */
    HI_U32 long_packet_count;                   /**<发送长包个数           */
    HI_U32 pb72_packet_count;                   /**<发送pb72个数           */
    HI_U32 pb264_packet_count;                  /**<发送pb264个数           */

    HI_U32 tx_send_msdu_success;                /**<发送机成功发送msdu的总数           */
    HI_U32 tx_send_msdu_timeout;                /**<发送机发送超时msdu的总数           */
    HI_U32 tx_send_msdu_fail;                   /**<发送机发送失败msdu的总数           */
    HI_U32 tx_send_msdu_resend;                 /**<发送机重发msdu的次数           */

    HI_U32 tx_send_app_success;                 /**<发送机成功发送app报文的个数           */
    HI_U32 tx_send_app_timeout;                 /**<发送机发送超时的app报文的个数           */
    HI_U32 tx_send_app_fail;                    /**<发送机发送失败的app报文的个数           */
    HI_U32 tx_send_app_resend;                  /**<发送机重发app报文的个数           */
}HI_MDM_TX_STATICS_S;

/**
*@ingroup hct_mac
* 获取收发机接收报文统计
*/
typedef struct
{
    HI_U32 fc_count;                        /**<FC中断次数             */
    HI_U32 fc_crc_success_count;            /**<FC校验正确次数            */
    HI_U32 fc_crc_err_count;                /**<FC校验错误次数           */
    HI_U32 fc_local_count;                  /**<本地FC总数           */

    HI_U32 sof_count;                       /**<本地sof帧个数           */
    HI_U32 beacon_count;                    /**<本地beacon帧个数           */

    HI_U32 beacon_pb_crc_err_count;         /**<beacon pb校验失败总数           */
    HI_U32 beacon_pb_crc_success_count;     /**<beacon pb校验成功总数           */

    HI_U32 broadcast_count;                 /**<本地广播报文个数            */
    HI_U32 unicast_count;                   /**<本地单播报文个数             */

    HI_U32 resend_count;                    /**<本地重传报文个数           */

    HI_U32 short_pb_crc_err_count;          /**<136pb块crc校验错误数           */
    HI_U32 long_pb_crc_err_count;           /**<520pb块crc校验错误数           */
    HI_U32 pb72_crc_err_count;              /**<72pb块crc校验错误数           */
    HI_U32 pb264_crc_err_count;             /**<264pb块crc校验错误数           */

    HI_U32 short_pb_count;                  /**<136pb块个数           */
    HI_U32 long_pb_count;                   /**<520pb块个数           */
    HI_U32 pb72_count;                      /**<72pb块个数           */
    HI_U32 pb264_count;                     /**<264pb块个数            */

    HI_U32 discard_packet_count;            /**<丢弃报文           */
}HI_MDM_RX_STATICS_S;

/**
*@ingroup hct_mac
* 获取汇聚器报文统计
*/
typedef struct
{
    HI_U32 cvg_send_app_success;            /**<汇聚器发送app报文到发送机成功的个数           */
    HI_U32 cvg_send_app_fail;               /**<汇聚器发送app报文到发送机失败的个数           */
    HI_U32 cvg_recv_app_success;            /**<汇聚器从发送机接收app报文成功的个数           */
    HI_U32 cvg_recv_app_fail;               /**<汇聚器从发送机接收app报文失败的个数           */
}HI_MDM_CVG_STATICS_S;

/**
* @ingroup  hct_mac
* @brief 获取发送报文统计
*
* @par  描述:
* 在STA/CCO 获取发送报文统计
* @attention 无。
* @param  无。
* @retval   ret 类型#HI_U32，#HI_ERR_SUCCESS表示查询成功,#HI_ERR_INVALID_PARAMETER表示参数错误,#HI_ERR_FAILURE 表示失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTxStatistics(HI_MDM_TX_STATICS_S  *tx_stats);

/**
* @ingroup  hct_mac
* @brief 获取接收报文统计
*
* @par  描述:
* 在STA/CCO 获取接收报文统计
* @attention 无。
* @param  无。
* @retval   ret 类型#HI_U32，#HI_ERR_SUCCESS表示查询成功,#HI_ERR_INVALID_PARAMETER表示参数错误,#HI_ERR_FAILURE 表示失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRxStatistics(HI_MDM_RX_STATICS_S *rx_stats);

/**
* @ingroup  hct_mac
* @brief 获取汇聚器报文统计
*
* @par  描述:
* 在STA/CCO 获取汇聚器报文统计
* @attention 无。
* @param  无。
* @retval   ret 类型#HI_U32，#HI_ERR_SUCCESS表示查询成功,#HI_ERR_INVALID_PARAMETER表示参数错误,#HI_ERR_FAILURE 表示失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCvgStatistics(HI_MDM_CVG_STATICS_S  *cvg_stats);

/**
* @ingroup  hct_mac
* @brief 清除发送报文统计
*
* @par  描述:
* 在STA/CCO 清除发送报文统计
* @attention 无。
* @param  无。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearTxStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 清除接收报文统计
*
* @par  描述:
* 在STA/CCO 清除接收报文统计
* @attention 无。
* @param  无。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearRxStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief 清除汇聚器报文统计
*
* @par  描述:
* 在STA/CCO 清除汇聚器报文统计
* @attention 无。
* @param  无。
* @retval  无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearCvgStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置离网检测周期的个数
*
* @par  描述:
* 设置离网检测周期的个数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_U8，离网检测周期的个数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功,#HI_ERR_INVALID_PARAMETER表示参数错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLeaveCycle(HI_U8 leave_cycle);

/**
* @ingroup  hct_mac
* @brief  获取离线上报周期的个数
*
* @par  描述:
* 获取离线上报周期的个数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U8，站点离线上报周期的个数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetOfflineReportCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  设置离线上报周期的个数
*
* @par  描述:
* 设置离线上报周期的个数
* @attention 作用产品类型为CCO。
* @param  offline_cycle [IN] 类型 #HI_U8，离线上报周期的个数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功,#HI_ERR_INVALID_PARAMETER表示参数错误。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetOfflineReportCycle(HI_U8 offline_cycle);

/**
* @ingroup  hct_mac
* @brief  获取离网检测周期的个数
*
* @par  描述:
* 获取离网检测周期的个数
* @attention 作用产品类型为CCO。
* @param  count [IN] 类型 #HI_VOID。
* @retval   ret 类型 #HI_U8，离网检测周期的个数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetLeaveCycle(HI_VOID);


/**
* @ingroup  hct_mac
* @brief 设置工作模式立刻生效接口。
*
* @par  描述:
* 设置工作模式立刻生效。
* @attention 在CCO/STA端使用。
* @param  freq_mode[IN] 类型 #HI_U8表示设置工作模式。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功，#HI_ERR_INVALID_PARAMETER表示工作模式无效。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetFreqWithoutDelay(HI_U8 freq_mode);

/**
 * @ingroup hct_mac
 * @brief 接收邻台区APP广播报文过滤函数
 */
typedef HI_BOOL (*MAC_NEIGHBOR_BROADCAST_FRAME_FILTER_FUN)(HI_CONST HI_PVOID frame, HI_U16 size);

/**
 * @ingroup hct_mac
 * @brief 接收邻台区APP广播报文控制结构
 */
typedef struct
{
    HI_BOOL rcv_switch;     /**< 接收开关: HI_TRUE表示接收邻网络广播APP报文，HI_FALSE表示不接收 >*/
    HI_U8   pad[3];

    MAC_NEIGHBOR_BROADCAST_FRAME_FILTER_FUN filter_fun; /**< 接收之后的过滤函数 >*/
} HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU;

/**
* @ingroup  hct_mac
* @brief 设置接收邻台区广播APP报文控制
*
* @par  描述:
* 设置接收邻台区广播APP报文控制。
* @attention 在CCO/STA端使用。
* @param  rcv_ctrl [IN] 类型 #HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU 设置的控制模式。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示设置成功，其他表示失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRcvNeighborBrodcastFrameCtrl(HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU *rcv_ctrl);

/**
 * @ingroup hct_mac
* @brief  通道类型，IPV6通道类型。
*/
#define HI_MDM_IPV6_TYPE                  (7)

/**
 * @ingroup hct_mac
* @brief  MAC帧头类型，短帧头。
*/
#define HI_MDM_MAC_TYPE_SHORT_PK          (0)

/**
 * @ingroup hct_mac
 * @brief  MAC帧头类型，长帧头。
*/
#define HI_MDM_MAC_TYPE_LONG_PK           (1)


/**
 * @ingroup hct_mac
 * @brief  PLC通道发送参数
*/
typedef struct
{
    HI_U8   ucSrcMac[HI_PLC_MAC_ADDR_LEN];      /**<报文最终源地址           */
    HI_U8   ucDestMac[HI_PLC_MAC_ADDR_LEN];     /**<报文最终目的地址           */

    HI_U8   Mtype;                              /**<报文类型           */
    HI_U8   MacHeadType;                        /**<帧头类型           */
    HI_BOOL IsDefaultLid;                       /**<是否使用默认优先           */
    HI_U8   Lid;                                /**<报文优先级           */

    HI_U32  Timeout;                            /**<报文过期时间，0表示采用默认的超时时间           */

    HI_U32  IsMyNetFrame    : 1;                /**<是否本网络报文 */
    HI_U32  RcvFreq         : 7;                /**<接收报文的频段号 */
    HI_U32  RcvNid          : 24;               /**<接收报文的网络号 */
}HI_PLC_FRAME_CTRL_STRU;

/**
* @ingroup  hct_mac
* @brief   应用层注册到协议栈的接收接口
*
* @par  描述:
* 应用层注册到协议栈的接收接口
* @attention 无。
* @param  Frame [IN] 类型 #HI_PVOID，应用层数据缓存。
* @param  FrameSize [IN] 类型 #HI_U16，应用层数据大小。
* @param  pFrameCtrl [IN] 类型 #HI_PLC_FRAME_CTRL_STRU，应用层数据参数。
* @param  pBuf [IN] 类型 #HI_PVOID，协议栈接收数据的缓存，零拷贝下使用，非零拷贝为空。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_Report_Topo_Change_Node_Fun 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*MAC_PACKET_INPUT_FUN)(HI_PVOID Frame, HI_U16 FrameSize,HI_PLC_FRAME_CTRL_STRU* pFrameCtrl,HI_PVOID pBuf);


/**
 * @ingroup hct_mac
 * @brief  应用层通道注册参数
*/
typedef struct
{
    HI_U8   DefalutLid;         /**<应用层报文默认优先级           */
    HI_U8   Pad[3];

    MAC_PACKET_INPUT_FUN InputFun;  /**<应用层接收接口           */
}HI_MDM_PLC_CHANNEL_INFO_STRU;

/**
* @ingroup  hct_mac
* @brief 应用层向协议栈注册接收通道
*
* @par  描述:
* 应用层向协议栈注册接收通道
* @attention IPV6类型仅支持在国网协议上使用。
* @param  pMacChannelInfo[IN] 类型 #HI_MDM_PLC_CHANNEL_INFO_STRU，通道参数。
* @param  ChannelType[IN] 类型 #HI_U8，通道类型，#HI_MDM_IPV6_TYPE，IPV6通道
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示注册成功，#HI_ERR_INVALID_PARAMETER表示注册失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterPlcChannel(HI_IN HI_MDM_PLC_CHANNEL_INFO_STRU *pMacChannelInfo,HI_IN HI_U8 ChannelType);

/**
* @ingroup  hct_mac
* @brief PLC数据发送接口
*
* @par  描述:
* 应用层调用该接口发送数据
* @attention 在CCO/STA端使用。
* @param  Frame[IN] 类型 #HI_PVOID，应用层数据缓存
* @param  FrameSize[IN] 类型 #HI_U16，应用层数据大小
* @param  pFrameCtrl[IN] 类型 #HI_PLC_FRAME_CTRL_STRU，报文发送参数
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示报文发送到协议栈成功，#HI_ERR_INVALID_PARAMETER表示发送到协议栈失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PlcSendFrame(HI_PVOID Frame, HI_U16 FrameSize,HI_PLC_FRAME_CTRL_STRU *pFrameCtrl);
/**
* @ingroup  hct_mac
* @brief   应用层注册到协议栈的多领域认证接口类型定义
* @par  描述:
* 应用层注册到协议栈的多领域认证接口
* @attention 无。
* @param  network_type [IN] 类型 #HI_U32 CCO领域信息
* @param  is_open_auth_cco [IN] 类型 #HI_BOOL  CCO是否开启领域认证
* @retval   HI_U8 认证结果:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL(*STA_AUTH_CCO_BY_NETWORK_TYPE)(HI_BOOL is_open_auth_cco,HI_U32 network_type);
/**
* @ingroup  hct_mac
* @brief 国网协议下多领域认证初始化接口
* @param  sta_auth_coo_network_func [IN] 类型 #STA_AUTH_CCO_BY_NETWORK_TYPE 多领域认证回调函数
* @par  描述:
* 应用层调用该接口初始多领域认证资源
* @attention 在STA端使用。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示初始化成功，#HI_ERR_INVALID_PARAMETER表示初始化失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterStaAuthCco(STA_AUTH_CCO_BY_NETWORK_TYPE sta_auth_coo_network_func);
/**
* @ingroup  hct_mac
* @brief 领域认证开关
* @par  描述:
* 应用层调用该接口来开启或者关闭领域认证
* @attention 在STA端使用。
* @param  Switch [IN] 类型 #HI_BOOL 领域认证开关
* @retval   ret 类型 #HI_VOID
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetNetworkSwitch(HI_BOOL Switch);
/**
* @ingroup  hct_mac
* @brief 设置本领域信息
* @par  描述:
* 应用层调用该接口初始化领域信息
* @attention 在CCO/STA端使用。入参是一个不为零的整数。
* @param  field_type [IN] 类型 #HI_U32，领域类型。
* @retval   ret 类型 HI_U32#HI_ERR_SUCCESS :设置成功，#HI_ERR_INVALID_PARAMETER:设置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetProductFieldType(HI_U32 field_type);
/**
* @ingroup  hct_mac
* @brief 获取本产品的领域信息
* @par  描述:
* 应用层调用该接口获取本产品的领域信息
* @attention 在CCO/STA端使用。
* @retval   ret 类型 HI_U32#产品领域类型
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetProductFieldType(HI_VOID);
/**
 * @ingroup hct_mac
 * @brief  管理ID长度
*/
#define MANAGEMENT_ID_SIZE  (24)
/**
 * @ingroup hct_mac
 * @brief  PLC通道发送参数
*/
typedef struct
{
     HI_U8  sta_mac[HI_PLC_MAC_ADDR_LEN]; /**<STA mac地址          */
     HI_U8  product_type;                 /**<产品类型             */
     HI_U8  mac_type;
     HI_U8  id[MANAGEMENT_ID_SIZE];       /**<管理ID信息           */
}HI_MDM_CCO_AHTU_STA_INFO_STRU;

/**
 * @ingroup hct_mac
 * @brief  上报抄控器连接功能搜索结果
*/
#define NDM_CONNECT_REPORT_SEARCH_RESULT                0

/**
 * @ingroup hct_mac
 * @brief  上报抄控器连接功能接入的mac地址
*/
#define NDM_CONNECT_REPORT_ACCESS_MAC                   1

/**
 *@ingroup hct_mac
 * 抄控器连接功能上报信息内容
 */
typedef struct
{
    HI_BOOL is_search_success;      /**<是否搜索成功           */
    HI_U8 pad;
    HI_U16 stei;                    /**<连接tei                */

    HI_U32 nid;                     /**<连接nid                */

    HI_U8 mac[HI_PLC_MAC_ADDR_LEN]; /**<接入站点的mac地址      */
    HI_U16 pad1;
}HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST;

/**
 *@ingroup hct_mac
 * 抄控器连接功能CCO/STA模块状态枚举
 */
typedef enum
{
    NDM_CONNECT_PASSIVE_SEARCH_STATE = 0,           /**< 被动搜索状态 */
    NDM_CONNECT_FAST_SYNC_STATE,                    /**<  快速同步状态 */
    NDM_CONNECT_LISTEN_STATE,                       /**<  监听状态 */
    NDM_CONNECT_DATA_TRANSMISSION_STATE,            /**<  数据传输状态 */
}ndm_connect_state_enum;

/**
 *@ingroup hct_mac
 * 抄控器连接功能抄控器模块状态枚举
 */
typedef enum
{
    NDM_CONNECT_NDM_INIT_STATE = 0,                     /**< 初始化状态 */
    NDM_CONNECT_NDM_SEND_SEARCH_FRAME_STATE,            /**< 发送搜索帧状态 */
    NDM_CONNECT_NDM_CONNECTED_STATE                      /**<  抄控器连接状态 */
}ndm_connect_ndm_state_enum;

/**
 * @ingroup hct_mac
 * @brief  抄控器所使用版本类型
*/
typedef enum
{
    NDM_BJ_VERSION = 1,                                 /**< 北京抄控器版本编号 */
    NDM_CQ_VERSION,                                     /**< 重庆抄控器版本编号 */
    NDM_MAX_VERSION                                     /**< 最大抄控器版本编号 */
}ndm_special_version_enum;

/**
* @ingroup  hct_mac
* @brief  抄控器连接功能的回调函数
* @par  描述:
* 抄控器在接收到同步帧后，完成ntb 同步 上报事件至app
* @attention 无。
* @param  ucType [IN] 类型 #HI_U8，事件类型。参看 #NDM_CONNECT_REPORT_SEARCH_RESULT。\n
                                                  #NDM_CONNECT_REPORT_ACCESS_MAC。
* @param  pCmdData [IN] 类型 #HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST *，数据指针。
* @param  ulDataSize [IN] 类型 #HI_U32，数据大小。
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_NdmConnectSearchReportCallback)(HI_U8 ucType,HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST *pCmdData,HI_U32 ulDataSize);

/**
 *@ingroup hct_mac
 * 抄控器连接功能回调函数封装结构体
 */
typedef struct
{
    HI_MDM_NdmConnectSearchReportCallback ndm_connect_search_report_callback;  /**<  抄控器连接搜索结果回调函数 */
}HI_MDM_MAC_NDM_CONNECT_SEARCH_REPORT_ST;

/**
* @ingroup  hct_mac
* @brief  注册抄控器连接功能相关函数
*
* @par  描述:
*       注册抄控器连接功能相关函数
* @attention 抄控器连接功能注册接口\n
*
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmSgConnectRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  抄控器连接状态上报app注册接口
* @par  描述:
* 抄控器连接状态上报app注册接口
* @attention 无。
* @param    SearchCallback [IN] 类型 #HI_MDM_NdmConnectSearchReportCallback， 抄控器连接功能的回调函数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_NdmConnectSearchReportCallback 回调函数。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmConnectSearchResultReport(HI_MDM_NdmConnectSearchReportCallback SearchCallback);

/**
* @ingroup  hct_mac
* @brief 获取使用抄控器连接功能下的模块状态
* @par  描述:
* 获取使用抄控器连接功能下的模块状态
* @attention 抄控器参看ndm_connect_ndm_state_enum结构体，CCO/STA 参看ndm_connect_state_enum结构体
* @retval   ret 类型 #HI_U8 抄控器模块状态 #ndm_connect_ndm_state_enum，\n
                            CCO/STA 模块状态 #ndm_connect_state_enum
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_NdmConnectState(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  启动抄控器连接接口
* @par  描述:
* 抄控器连接指定mac地址站点
* @attention 无。
* @param    oda [IN] 类型 #HI_U8， 抄控器需要连接站点的mac地址。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示启动成功。其他表示失败。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmConnectStartSearchNet(HI_U8 oda[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief   应用层注册到协议栈的管理ID认证接口
* @par  描述:
* 应用层注册到协议栈的管理ID认证接口
* @attention 无。
* @param  ManagementId [IN] 类型 #HI_U8 *，管理ID信息。
* @retval   HI_BOOL 认证结果:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*CCO_AUTH_MANAGEMENTID_FUN)(HI_MDM_CCO_AHTU_STA_INFO_STRU *auth_info);
/**
* @ingroup  hct_mac
* @brief 国网协议下CCO认证sta管理ID接口
* @param  cco_auth_id_func [IN] 类型 #CCO_AUTH_MANAGEMENTID_FUN 认证管理ID接口回调函数
* @par  描述:
* 国网协议下CCO认证sta管理ID接口
* @attention 在STA端使用。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS表示注册成功，#HI_ERR_FAILURE表示注册失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCcoAuthManagementId(CCO_AUTH_MANAGEMENTID_FUN cco_auth_id_func);
/**
* @ingroup  hct_mac
* @brief 在站点正常运行时设置管理ID，本次设置只在本次运行时有效,不会影响芯片中保存的ID信息，掉电后需要重新设置。
* @param  runtime_management_id [IN] 类型 #HI_U8* 管理ID信息,is_use_default_id[IN] #HI_BOOL
* @param  is_use_default_id [IN] 类型 #HI_BOOL 是否使用默认的管理ID
* @par  描述:
* 在站点正常运行时设置管理ID
* @attention 在CCO/STA端使用。
* @attention is_use_default_id:HI_TRUE时，管理ID会使用芯片自带ID信息，将不会使用runtime_management_id的值
                              :HI_FALSE时，将runtime_management_id设为设备的管理ID信息
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设置成功，#HI_ERR_INVALID_PARAMETER:参数错误
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRunTimeManagementId(HI_U8 runtime_management_id[MANAGEMENT_ID_SIZE],HI_BOOL is_use_default_id);
/**
* @ingroup  hct_mac
* @brief 获取站点正常运行时设置的管理ID。如果未设置管理ID信息，则返回的是芯片里的自带管理ID信息。
* @param  runtime_management_id [OUT] 类型 #HI_U8* 管理ID信息
* @par  描述:
* 获取站点正常运行时设置管理ID
* @attention 在CCO/STA端使用。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示获取成功，#HI_ERR_INVALID_PARAMETER:参数错误
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRunTimeManagementId(HI_U8 runtime_management_id[MANAGEMENT_ID_SIZE]);

/**
* @ingroup  hct_mac
* @brief 启用STA端白名单标识符和满载标识符(白名单标识和满载标识默认是关闭状态)
* @param  is_set_disable [IN] 类型 #HI_BOOL HI_TRUE :启用  HI_FALSE:停用
* @par  描述:
* 启用白名单标识符和满载标识符
* @attention 该接口仅供集抄领域使用
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设置成功，#HI_ERR_INVALID_PARAMETER:参数错误
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetFullLoadAndWhiteListEnable(HI_BOOL is_set_enable);
/**
* @ingroup  hct_mac
* @brief 获取STA端当前白名单标识符和满载标识的使用状态
* @par  描述:
* 获取当前白名单标识符和满载标识的使用状态
* @attention 该接口仅供集抄领域使用
* @retval   ret 类型 #HI_BOOL，#HI_TRUE :表示正在使用，#HI_FALSE :未使用
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetFullLoadAndWhiteListState(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  获取接收正确的PB块的累计值
* @par  描述:
*  获取接收正确的PB块的累计值
* @retval   ret 类型 #HI_U32，接收正确的PB块计数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MAC_GetRxPbStatistic(HI_VOID);

/**
* @ingroup hct_mac
* @brief  ID探测功能 cco/sta 状态结构体
*/
typedef enum
{
    NDM_DETECTID_INIT_STATE = 5,                      /**< 初始化状态 */
    NDM_DETECTID_ACCESS_NDM_STATE,                    /**<  接入抄控器状态 */
    NDM_DETECTID_DATA_TRANSMISSION_STATE,            /**<  数据传输状态 */
}ndm_detectid_state_enum;

/**
* @ingroup hct_mac
* @brief  ID探测功能 抄控器状态结构体
*/
typedef enum
{
    NDM_DETECTID_NDM_INIT_STATE = 5,                    /**< 初始化状态 */
    NDM_DETECTID_NDM_IDLE_STATE,                        /**< 空闲状态 */
    NDM_DETECTID_CREATE_BCN_STATE,                      /**<  创建信标状态 */
    NDM_DETECTID_NDM_LISTEN_DATA_STATE,                 /**<  监听数据状态 */
    NDM_DETECTID_NDM_DATA_TRANSMISSION_STATE,           /**<  数据传输状态 */
    NDM_DETECTID_NDM_FINISH_DETECT_STATE,               /**<  探测结束状态 */
}ndm_detectid_ndm_state_enum;

/**
* @ingroup hct_mac
* @brief  抄控器探测id功能状态上报
*/
#define NDM_DETECTID_STATE_REPORT       1

/**
* @ingroup hct_mac
* @brief  抄控器探测id功能上报信息
*/

typedef struct
{
    HI_U8 ndm_state;
    HI_U8 freq;
    HI_U16 pad1;
}HI_MDM_NDM_DETECTID_REPORT_INFO_ST;

/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能状态变化回调函数
* @par  描述:
* 抄控器发生状态变化时，调用该函数，上报状态变化到app
* @attention 无。
* @param  ucType [IN] 类型 #HI_U8，事件类型。参看 #NDM_DETECTID_STATE_REPORT。
* @param  pCmdData [IN] 类型 #HI_MDM_NDM_DETECTID_REPORT_INFO_ST *，数据指针。
* @param  ulDataSize [IN] 类型 #HI_U32，数据大小。
* @retval ret [OUT] 类型 #HI_U32，#HI_ERR_SUCCESS 成功，更多返回值参见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see HI_MDM_NDM_DETECTID_REPORT_INFO_ST 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_NdmDetectIdStateReportCallback)(HI_U8 ucType,HI_MDM_NDM_DETECTID_REPORT_INFO_ST *pCmdData,HI_U32 ulDataSize);

/**
* @ingroup hct_mac
* 抄控器探测id功能回调函数封装结构体
*/
typedef struct
{
    HI_MDM_NdmDetectIdStateReportCallback ndm_detectid_state_report_callback;
}HI_MDM_MAC_NDM_DETECTID_STATE_REPORT_ST;

/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能启动探测接口，需要完成功能注册
* @param  oda [IN] 类型 #HI_U8 探测站点地址， freq [IN] 类型 #HI_U8 探测频段
* @par  描述:
*  完成功能注册后，调用该接口进行模块id探测
* @attention 该接口在功能注册完成后调用生效
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS :启动探测成功，#HI_ERR_FAILURE :启动探测失败(未进行注册)
*                              更多返回值参见hi_errno.h。
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmStartDetectId(HI_U8 oda[HI_PLC_MAC_ADDR_LEN], HI_U8 freq);

/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能注册接口
* @param  无
* @par  描述:
*  完成抄控器注册
* @attention 无
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS :注册成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能时设置频段接口，默认协议为国网协议， snid为0
* @param  freq [IN] 类型 #HI_U8 频段
* @par  描述:
*  抄控器探测id功能时设置频段接口，默认协议为国网协议， snid为0
* @attention 该接口只在抄控器模块使用
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS :设置成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetFreq(HI_U8 freq);
/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能设置模块状态
* @param  state [IN] 类型 #HI_U8 取值参考 #ndm_detectid_ndm_state_enum
* @par  描述:
*  app层接收到 cco/sta 响应的探测id报文时，可调用该接口设置抄控器模块的状态
* @attention 该接口只在抄控器模块使用
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS :设置成功，#HI_ERR_FAILURE :设置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetState(HI_U8 state);
/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能状态上报app注册接口
* @par  描述:
* 抄控器探测id功能状态上报app注册接口
* @attention 无。
* @param    DetectidStateCallback [IN] 类型 #HI_MDM_NdmDetectIdStateReportCallback， 抄控器探测id功能的回调函数。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see #HI_MDM_NdmDetectIdStateReportCallback 回调函数。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectidStateReport(HI_MDM_NdmDetectIdStateReportCallback DetectidStateCallback);

/**
* @ingroup  hct_mac
* @brief  抄控器探测id功能设置接收数据标志位
* @param  is_rcv_data [IN] 类型 #HI_BOOL HI_TRUE :接收到探测响应的数据  HI_FALSE:未接收到探测响应的数据
* @par  描述: 无
* @attention 无
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS :设置成功
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetRcvData(HI_BOOL is_rcv_data);

/**
* @ingroup hct_mac
* @brief  关闭衰减模式
*/
#define HI_MDM_NDM_ATTENUATE_DISABLE    0

/**
* @ingroup hct_mac
* @brief  启用衰减模式
*/
#define HI_MDM_NDM_ATTENUATE_ENABLE    1

/**
* @ingroup hct_mac
* @brief  抄控宝台区识别中
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_STATUS_DOING      1

/**
* @ingroup hct_mac
* @brief  抄控宝台区识别结束
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_STATUS_OVER       0

/**
* @ingroup hct_mac
* @brief  抄控宝台区识别没有识别结果，处于识别中
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_DOING       0


/**
* @ingroup hct_mac
* @brief  抄控宝台区识别没有识别成功
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_SUCCESS     1

/**
* @ingroup hct_mac
* @brief  抄控宝台区识别没有识别失败
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_FAIL         2

/**
* @ingroup hct_mac
* @brief  接收机档位为240K
*/
#define NDM_TF_REPORT_LEVEL_240K                0


/**
* @ingroup hct_mac
* @brief  接收机档位为1M
*/

#define NDM_TF_REPORT_LEVEL_1M                1
/**
 * @ingroup hct_mac
* @brief  台区识别结果上报结构体
*/
typedef struct
{
    HI_U8  ucStatus;                   /**<识别状态         */
    HI_U8  ucTfResult;                 /**<识别结果         */
    HI_U8  ucSerialNum;                /**<识别到的台区号         */
    HI_U8  ucPhase;                    /**<识别到的相位         */

    HI_U8  ucLevel;                    /**<接收机档位  \n
                                        取值: #NDM_TF_REPORT_LEVEL_240K 表示240k，#NDM_TF_REPORT_LEVEL_1M  表示1M。
                                        */
    HI_U8  ulPad[3];                   /**<保留位         */

    HI_U16 wobble_scope;               /**<扰动均值，单位us         */
    HI_U16 noise_scope;                /**<实时噪声均值，单位us         */
} HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU;

/**
* @ingroup  hct_mac
* @brief   定义接收抄控器上报台区识别数据函数类型。
*
* @par  描述:
*定义接收抄控器上报台区识别数据函数类型。
* @attention 无。
* @param  ndm2ndmtfstatuscallback [IN] 类型 #HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU*，抄控宝上报的台区识别数据。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack)(HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU *ndm2ndmtfstatuscallback);

/**
* @ingroup  hct_mac
* @brief   定义处理搜网数据上报函数类型。
*
* @par  描述:
* 定义处理搜网数据上报函数类型。
* @attention 无。
* @param  HI_MDM_NET_SEARCH_CONN_IND_S [IN] 类型 #HI_MDM_NET_SEARCH_CONN_IND_S*，搜网结果上报数据，详见#HI_MDM_NET_SEARCH_CONN_IND_S定义。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see hi_mdm_mac.h 文件中#HI_MDM_NET_SEARCH_CONN_IND_S 定义。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterNdmNetSearchStateCallBack)(HI_MDM_NET_SEARCH_CONN_IND_S *search_data);

/**
* @ingroup  hct_mac
* @brief  设置抄控宝tei
* @par  描述:
* 设置抄控宝tei接口
* @attention 无。
* @param    tei [IN] 类型 #HI_U32， 设置的抄控宝tei值。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设定成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误。 \n
                              #HI_ERR_NOT_SUPPORT表示不是抄控宝产品。 \n
                              #HI_ERR_FAILURE表示其他错误。
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmTei(HI_IN HI_U32 tei);

/**
* @ingroup  hct_mac
* @brief  获取抄控宝tei
* @par  描述:
* 获取抄控宝tei接口
* @attention 无。
* @param    tei [OUT] 类型 #HI_U32 *， 抄控宝的tei值。
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设定成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误。 \n
                              #HI_ERR_NOT_SUPPORT表示不是抄控宝产品。 \n
                              #HI_ERR_FAILURE表示其他错误。
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNdmTei(HI_IN HI_U32 *tei);
/**
* @ingroup  hct_mac
* @brief  设置抄控宝衰减模式
* @par  描述:
* 设置抄控宝衰减模式
* @attention 无。
* @param    attenuate_mode [IN] 类型 #HI_U8，设置衰减模式值
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设定成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误 \n
                              #HI_ERR_NOT_SUPPORT表示不是抄控宝产品 \n
                              #HI_ERR_FAILURE表示其他错误
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmAttenuate (HI_IN HI_U8 attenuate_mode);
/**
* @ingroup  hct_mac
* @brief  获取抄控宝衰减模式
* @par  描述:
* 获取抄控宝衰减模式
* @attention 无。
* @param    attenuate_mode [OUT] 类型 #HI_U8 *，获取的衰减模式
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示获取成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误 \n
                              #HI_ERR_NOT_SUPPORT表示不是抄控宝产品 \n
                              #HI_ERR_FAILURE表示其他错误
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNdmAttenuate (HI_IN HI_U8 *attenuate_mode);
/**
* @ingroup  hct_mac
* @brief  上报台区识别数据注册函数接口
* @par  描述:
* 上报台区识别数据注册函数接口
* @attention 无。
* @param    func [IN] 类型 #HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack，注册的回调函数
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误 \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterQueryNdm2NdmTfStatus(HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack func);
/**
* @ingroup  hct_mac
* @brief  上报搜网站点注册回调函数接口
* @par  描述:
* 上报搜网站点注册回调函数接口
* @attention 无。
* @param    func [IN] 类型 #HI_MDM_RegisterNdmNetSearchStateCallBack，注册回调函数
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示获取成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误 \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterQueryNdmSearchData(HI_MDM_RegisterNdmNetSearchStateCallBack func);

/**
* @ingroup  hct_mac
* @brief  设置抄控器接入的频段，网络号，协议
* @par  描述:
* 设置抄控器接入的频段，网络号，协议
* @attention 无。
* @param    freq_index [IN] 类型 #HI_U8，频段索引
* @param    nid [IN] 类型 #HI_U32，国网协议下NID，取值范围16~16777215， 其他协议下取值范围1-15
* @param    ptcl_ver [IN] 类型 #HI_U16，协议版本号
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示设置成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示设置失败 \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmAccessInfo(HI_U8 freq_index, HI_U32 nid, HI_U16 ptcl_ver);

/**
* @ingroup  hct_mac
* @brief  是否设置最大站点
*
* @par  描述:
* 是否设置最大站点
* @attention 是否设置最大站点
* @param   无
* @retval  ret 类型 #HI_BOOL，#HI_TRUE 表示为,#其他表示
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMaxTei(HI_U16 max_tei);
/**
* @ingroup  hct_mac
* @brief  注册远程通讯终端交换模块接口
*
* @par  描述:
* 注册远程通讯终端交换模块接口
* @attention 作用产品类型为CCO,STA。
* @param   无
* @retval  ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示配置成功，#其他表示配置失败
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSwitchRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取首次组网完成的时长
*
* @par  描述:
* 获取第一次CCO判定组网完成的时长
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#组网完成的时长，0xFFFFFFFF表示组网还未完成。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetworkFormTime(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取启动组网的时长
*
* @par  描述:
* 获取启动组网的时长
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#启动组网的时长，0xFFFFFFFF表示未启动组网。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetStartNetworkFormTime(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取拓扑变更的次数
*
* @par  描述:
* 获取拓扑表更的次数
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#拓扑变更的次数。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTopoChangeCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取路由的周期
*
* @par  描述:
* 获取路由评估的周期。
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#路由评估周期的长度，单位秒。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetRouteCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取代理站点发送发现列表报文的周期
*
* @par  描述:
* 获取代理站点发送发现列表报文的周期
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#代理站点发送发现列表报文的周期，单位秒。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetPcoDiscoverPkCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  获取发现站点发送发现列表报文的周期
*
* @par  描述:
* 获取发现站点发送发现列表报文的周期
* @attention 作用产品类型为CCO
* @param   无
* @retval  ret 类型 #HI_U32，#发现站点发送发现列表报文的周期，单位秒。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetStaDiscoverPkCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   定义三相表物理相位识别完成上报函数类型。
*
* @par  描述:
*定义三相表物理相位识别完成上报函数类型。
* @attention 无。
* @param  无。
* @retval   无。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_RegisterThreeMeterPhaseCheckCallBack)(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  上报三相表相位检测完成注册函数
* @par  描述:
* 上报三相表物理相位检测完成注册函数。
* @attention  作用产品类型为CCO。
* @param    func [IN] 类型 #HI_MDM_RegisterThreeMeterPhaseCheckCallBack，注册的回调函数
* @retval   ret 类型 #HI_U32，#HI_ERR_SUCCESS 表示注册成功。\n
                              #HI_ERR_INVALID_PARAMETER 表示下发参数错误 \n
                              #HI_ERR_NOT_SUPPORT 表示产品不支持\n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterThreeMeterPhaseCheckResultReportFunc(HI_MDM_RegisterThreeMeterPhaseCheckCallBack func);

/**
* @ingroup hct_mac
* @brief  三相表检测类型模式
*/
typedef enum
{
    HI_MDM_CHECK_UNKOWN_THREE_METER_TYPE = 0,         /**< 未检测到三相表过零类型 */
    HI_MDM_CHECK_NO_THREE_METER_TYPE,                 /**< 不是三相表产品类型 */
    HI_MDM_CHECK_SUPPORT_ONE_LINE_ZERO_CROSS,         /**<支持一路过零的三相表产品类型 */
    HI_MDM_CHECK_SUPPORT_THREE_LINE_ZERO_CROSS,       /**<支持三路过零的三相表产品 类型*/
}HI_MDM_CHECK_THREE_METER_TYPE_MODE;

/**
* @ingroup  hct_mac
* @brief  获取三相表产品支持过零的类型。
*
* @par  描述:
* 获取三相表产品支持过零的类型。
* @attention 作用产品类型为三相表产品
* @param   无
* @retval  ret 类型 #HI_U8，#HI_MDM_CHECK_THREE_METER_TYPE_MODE获取的支持过零类型
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetThreeMeterSupportZeroCrossType(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  判断抄控宝产品的过零电路是否正常。
*
* @par  描述:
* 判断抄控宝产品的过零电路是否正常。
* @attention 作用于抄控宝产品形态。
* @param   无
* @retval  ret 类型 #HI_BOOL，#HI_TRUE过零电路正常，#HI_FALSE过零电路异常。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetZeroCrossCirCuitIsNormal(HI_VOID);

/**
* @ingroup hct_mac
* @brief  CCO三相过零检测结果
*/
typedef enum
{
    HI_MDM_PHASE_CHECK_UNKOWN = 0,                /**< 未检测到相位结果 */
    HI_MDM_PHASE_CHECK_ABC,                       /**< 检测到三相相序为ABC */
    HI_MDM_PHASE_CHECK_ACB,                       /**< 检测到三相相序为ACB */
    HI_MDM_PHASE_CHECK_LOSS,                      /**< 检测到存在断相 */
    HI_MDM_PHASE_CHECK_SAME,                      /**< 检测到存在相同相位 */
    HI_MDM_PHASE_CHECK_ERR,                       /**< 检测到三相相序错误 */
}HI_MDM_CCO_ZERO_CROSS_PHASE_CHECK_RESULT_MODE;

/**
* @ingroup  hct_mac
* @brief  获取CCO产品形态下三相过零电路检测结果。
*
* @par  描述:
* 获取CCO产品形态下三相过零电路检测结果。
* @attention 作用于CCO产品形态。
* @param   无
* @retval  ret 类型 #HI_U8，参见HI_MDM_CCO_ZERO_CROSS_PHASE_CHECK_RESULT_MODE定义。
* @par Dependency:
* <ul><li>hi_mdm_mac.h: 文件用于描述链路层对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetCcoZeroCrossCirCuitCheckResult(HI_VOID);

HI_END_HEADER
#endif // __HI_MDM_MAC_H__


