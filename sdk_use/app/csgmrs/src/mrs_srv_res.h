//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_res.h
//  版 本 号   : V1.0
//  作    者   : niesongsong/kf62735
//  生成日期   : 2012-07-25
//  功能描述   : SRV定时器与公共ID部分
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2012-07-25
//    作    者 : niesongsong/kf62735
//    修改内容 : 创建文件
//
//  2.日    期 : 2013-07-24
//    修 改 人 : cuiate/00233580
//    修改内容 : 重新整理timer定义
//
//  备注：
//      仅做内部资源配置使用，外部不可见
//*****************************************************************************
#ifndef __MRS_SRV_RES_H__
#define __MRS_SRV_RES_H__
HI_START_HEADER


//命名方式
//#define MRS_CCO_TIMER_XXX
//#define MRS_CCO_TIME_XXX
//#define MRS_CCO_STATE_XXX
//#define MRS_STA_TIMER_XXX
//#define MRS_STA_TIME_XXX
//#define MRS_STA_STATE_XXX

//*****************************************************************************
// CCO的定时器
//*****************************************************************************

typedef enum
{
    MRS_CCO_TIMER_MAP,                      // 映射表维护定时器ID
    MRS_CCO_TIMER_STA,                      // 与STA通讯定时器ID
    MRS_CCO_TIMER_MASTER,                   // 与集中器通讯定时器ID
    MRS_CCO_TIMER_READMETER_ACTIVE,         // 读表模块激活定时器
    MRS_CCO_TIMER_SERVICE_END,              // 服务结束定时器(用于搜表或随抄等服务的结束)

    MRS_CCO_TIMER_FLASHWRITE_ACTIVE,        // 表档案存盘激活定时器ID
    MRS_CCO_TIMER_GET_TIME,                 // 定时获取集中器时间定时器
    MRS_CCO_TIMER_SYSTEM_RESET,             // AFN=01H-F1执行硬件重启定时器ID
    MRS_CCO_TIMER_PARALLEL_CHECK,           // 并发读表PLC超时检查定时器
    MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE,

    MRS_CCO_TIMER_BROADCAST,
    MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE,
    MRS_CCO_TIMER_EVENT_ACTIVE,
    MRS_CCO_TIMER_MASTER_ACTIVE,
    MRS_CCO_TIMER_MASTER_REPORT,

    MRS_CCO_TIMER_RESET_PIN,
    MRS_CCO_TIMER_SIMU_CHL,
    MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME,
    MRS_CCO_TIMER_GET_METERLIST,
    MRS_CCO_TIMER_TOPO_QUERY,
    MRS_CCO_TIMER_SEND_BIND_CMD,            // 发送锁定报文

    MRS_CCO_TIMER_CHECK_MAC_MAP_TTL,
    MRS_CCO_TIMER_MAP_REFRESH,
    MRS_CCO_TIMER_SEARCH_METER_END,
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/13   问题单号号:DTS2014012607684 */
    MRS_CCO_TIMER_UPG_FILE_RECEIVE,         // 升级下装阶段接收到升级数据超时定时器
    MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE,    // 停止升级重发定时器
    MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE,  // 升级状态查询定时器
    MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT, // 升级状态查询超时定时器
    MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE,

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_CCO_TIMER_ACQUIRE_CLOCK,
    MRS_CCO_TIMER_MRS_LOG,
#endif
    MRS_CCO_TIMER_SEARCH_METER_END_TF,
    MRS_CCO_TIMER_SEARCH_METER_WHITELIST,
    MRS_CCO_TIMER_SEARCH_REPORT,
    MRS_CCO_TIMER_TEST_MODE_FRAME,

    MRS_CCO_TIMER_ZERO_CROSS,

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    MRS_CCO_TIMER_RUN_LED_BBU,
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CCO_TIMER_EVT_FLAG_NOTICE,
    MRS_CCO_TIMER_UPG_QUERY_INFO_TIMEOUT,   // 查询升级全部信息
    MRS_CCO_TIMER_MR_TASK_ACTIVE,           // 抄表任务激活，在任务暂停状态或未组网时使用
    MRS_CCO_TIMER_MODE_PHYSICAL_TEST,       //物理层测试模式
    MRS_CCO_TIMER_ENTER_TESTMODE,           //进入测试模式
#endif

    MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP,   // 将以mac地址入网的采集器加入白名单后，如果30分钟内没有加入白名单的采集器站点入网,删除添加的白名单
    MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE,  // 探测抄表机制中缓存拒绝站点信息定时器
    MRS_CCO_TIMER_DETECT_WAIT_TOPO,         // 探测机制中等待拓扑组网,一定时间后拓扑中仍只有CCO,后续将拒绝的站点加入白名单
    MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE,    // 探测机制中缓存在追加白名单中的站点入网定时器,超时后启动探测抄表
    MRS_CCO_TIMER_DETECT_BLACK_LIST,        // 黑名单超时处理定时器
    MRS_CCO_TIMER_DETECT_REFRESH_WHITE,     // 探测抄表机制中通知刷新白名单定时器
    MRS_CCO_TIMER_RESUME_DETECT_RM,         // 探测抄表机制中恢复探测抄表定时器
    MRS_CCO_TIMER_SEND_METER_LIST_PLC,      // 探测抄表机制中发送表地址列表PLC帧定时器

    MRS_CCO_TIMER_RECOVER_POWER_AFE,        // 恢复发射功率定时器
    MRS_CCO_TIMER_POWERFAILURE,             //CCO上报停电事件
    MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA,  //CCO擦除数据定时器

    MRS_CCO_TIMER_MAX
} MRS_CCO_LOGIC_TIMER_ID_E;


#define EN_MRS_SRV_CCO_MAP_VIN_TIMER            (MRS_CCO_TIMER_MAP)
#define EN_MRS_SRV_CCO_STA_TIMER                (MRS_CCO_TIMER_STA)
#define EN_MRS_SRV_CCO_MUSTER_TIMER             (MRS_CCO_TIMER_MASTER)
#define EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER    (MRS_CCO_TIMER_READMETER_ACTIVE)
#define EN_MRS_SRV_CCO_SERVICE_END_TIMER        (MRS_CCO_TIMER_SERVICE_END)
#define EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER  (MRS_CCO_TIMER_FLASHWRITE_ACTIVE)
#define EN_MRS_SRV_CCO_GET_TIME_TIMER           (MRS_CCO_TIMER_GET_TIME)
#define EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER       (MRS_CCO_TIMER_SYSTEM_RESET)
#define EN_MRS_SRV_CCO_RM_CHECK_TIMER           (MRS_CCO_TIMER_PARALLEL_CHECK)
#define EN_MRS_SRV_CCO_BROADCAST_TIMER          (MRS_CCO_TIMER_BROADCAST)
#define EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER     (MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE)
#define EN_MRS_SRV_CCO_EVENT_ACTIVE_TIMER       (MRS_CCO_TIMER_EVENT_ACTIVE)
#define EN_MRS_SRV_CCO_RESET_PIN_TIMER          (MRS_CCO_TIMER_RESET_PIN)
#define EN_MRS_SRV_CCO_SIMU_CHL_NOTIFY_MONITOR  (MRS_CCO_TIMER_SIMU_CHL)
#define EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER  (MRS_CCO_TIMER_CHECK_MAC_MAP_TTL)
#define EN_MRS_SRV_CCO_MAP_REFRESH_TIMER        (MRS_CCO_TIMER_MAP_REFRESH)

//*****************************************************************************
// CCO定时器的定时周期
//*****************************************************************************
#define MRS_CCO_TIME_MASTER                     (1000 * 60) // 适配抄控器ip通道变更为1min
#define MRS_CCO_TIME_READMETER_ACTIVE           (200)       // 读表模块激活时间
#define MRS_CCO_TIME_SERVICE_END                (300)     // 服务结束定时器时间(用于随抄等服务的结束)

#define MS_PER_SECOND                           (1000)      //定时器基本单位  1s
#define MRS_CCO_TIME_FLASHWRITE_ACTIVE          (20)     // 表档案存盘激活时间  默认20s

#define MRS_CCO_TIME_SYSTEM_RESET               (5000)       // AFN=01H-F1执行硬件重启超时时间
#define MRS_CCO_TIME_PARALLEL_CHECK             (50)       // 并发读表超时检查定时器默认值(每间隔此时间做一次超时检查)
#define MRS_CCO_TIME_SEARCHMETER                (2000)
#define MRS_CCO_TIME_RESET_PIN                  (150)

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIME_ACQUIRE_CLOCK              (1500)
#define MRS_CCO_TIME_CHECK_CLOCK                (1000*60)
#define MRS_CCO_TIME_STARTUP_CLOCK              (1000*60*20) //CCO上电20分钟后开始问询集中器时钟的时间
#define MRS_CCO_TIME_ACTIVE_ACQUIRE_CLOCK       (200) //CCO收到路由重启或路由恢复命令后，激活问询集中器时钟的时间
#endif

#define MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT      (2)             // 集中器通讯默认最大重试次数
#define MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT        (1000 * 10)     // 集中器通讯默认超时时间(模拟通道关闭),单位:ms

#define MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT  (1000 * 60)   // 集中器通讯默认超时时间(模拟通道打开),单位:ms

#define MRS_SRV_CCO_TIMER_MUSTER_VALUE                  (MRS_CCO_TIME_MASTER)
#define MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE         (MRS_CCO_TIME_READMETER_ACTIVE)
#define MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE            (MRS_CCO_TIME_READMETER_ACTIVE)
#define MRS_SRV_CCO_TIMER_READMETER_REACTIV_VALUE       (1000 * 30)
#define MRS_SRV_CCO_TIMER_SERVICE_END_VALUE             (10)     // 服务结束定时器时间(用于随抄结束后恢复轮抄等服务的结束)
#define MRS_SRV_CCO_TIMER_SAVEMETERS_FLASH_ACTIVE_VALUE (MRS_CCO_TIME_FLASHWRITE_ACTIVE)
#define MRS_SRV_CCO_SYSTEM_RESET_TIMER_VALUE            (MRS_CCO_TIME_SYSTEM_RESET)
#define MRS_SRV_CCO_SEARCHMETER_TIME_VALUE              (MRS_CCO_TIME_SEARCHMETER)
#define MRS_SRV_CCO_TIMER_READMETER_INIT_ACTIV_VALUE    (10000)     // 读表模块初始激活时间(上电后延时5min)
#define MRS_SRV_CCO_TIMER_DEFAULT_TIMEOUT_VALUE         (90000)     // 随抄等服务默认超时时间
#define MRS_SRV_CCO_TIMER_SEARCH_REPORT_VALUE           (8000)      // 搜表结果上报给集中器的超时时间,单位:ms

#define MRS_CCO_UPG_CLEAR_FILE_TIMEOUT_VALUE            (800)  //收到清除下装命令后超时时间,单位:ms
#define MRS_CCO_DETECT_MAC_CACHE_TIME_VALUE             (30 * 1000)  //APP收到MAC拒绝入网信息后最大缓存的时间
#define MRS_CCO_DETECT_REFRESH_WHITE_TIME_VALUE         (10 * 1000)  //探测机制中通知刷新白名单检查时间

#define MRS_CCO_TIME_SEARCH_METER_WHITELIST_DEFAULT     (0)
#define MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT           (3 * 1000)  //集中器透抄一帧结束后恢复探测抄表的时间

#define MRS_CCO_TIME_MR_TASK_ACTIVE                     (1000)      // 抄表任务激活，在任务暂停状态或未组网时使用

#define MRS_SRV_STA_SYSTEM_RESET_TIMER_VALUE            (MRS_CCO_TIME_SYSTEM_RESET) // 被检版重启定时器  单位:ms

#define MRS_CCO_RECOVER_POWER_AFE_TIME_MIN              (15) //15分钟
#define MRS_CCO_RECOVER_POWER_AFE_TIME_DEFAULT          (15) //15分钟
#define MRS_CCO_RECOVER_POWER_AFE_TIME_MAX              (30) //30分钟

#define MRS_CCO_LOW_POWER_RUN_TIME                      (5 * 60) //5分钟 CCO运行超过5分钟不允许再降低发射功率

//*****************************************************************************
// CCO运行状态
//*****************************************************************************
#define MRS_CCO_STATE_CREATE_MAP                (0)     // 初始化状态，未探测波特率
#define MRS_CCO_STATE_NORMAL                    (1)     // 抄表态，正常态，STA可以正常处理任何请求


//*****************************************************************************
// STA的定时器
//*****************************************************************************

typedef enum
{
    MRS_STA_TIMER_TEST,
    MRS_STA_TIMER_HEART,                    // 心跳定时器(与地址获取失败LED闪烁定时器复用:MRS_STA_TIMER_TEST状态下为LED定时器)
    MRS_STA_TIMER_QUEUE,
    MRS_STA_TIMER_TTL,
    MRS_STA_TIMER_IO_EVENT_OUT,             // IO控制模块中EVENT_OUT管脚查看状态定时器ID
    MRS_STA_TIMER_RUN_LED,                  // II型采集器中运行灯亮/灭定时器
    MRS_STA_TIMER_UART_LED,                 // II型采集器中UART收发灯亮/灭定时器
    MRS_STA_TIMER_IRDA_RESET,               // II型采集器红外回应数据帧硬件初始化定时器
    MRS_STA_TIMER_EVENT_REPORT,
    MRS_STA_TIMER_SM_FRAME,                 // 搜表等待表应答首字节定时器
    MRS_STA_TIMER_SM_BYTE,                  // 表应答数据字节间隔定时器
    MRS_STA_TIMER_SM_VM,                    // 虚拟表被搜功能定时器
    MRS_STA_TIMER_SM_PERIOD,                // 周期性搜表定时器
    MRS_STA_TIMER_SM_PERIOD_NO_METER,       // 上电未搜到表，间隔10分钟搜表
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_STA_TIMER_DAYFREEZE_REC,            // 周期性记录日冻结统计定时器
#endif
    MRS_STA_TIMER_RESET_PIN,
    MRS_STA_TIMER_SIMU_CHL,
    MRS_STA_TIMER_CLTI_POSITIVE_A,          // I采: STA上电读上次入网电表(+A)定时器
    MRS_STA_TIMER_CLTI_SM_QUERY,            // I采: 查询搜表结果定时器
    MRS_STA_TIMER_CLTI_SM_START,            // I采: 搜表开始帧定时器
    MRS_STA_TIMER_CLTI_SM_STOP,             // I采: 搜表停止帧定时器
    MRS_STA_TIMER_VM_EVT_ACTIVE,            // 模拟表事件激活定时器
    MRS_STA_TIMER_ZERO_CROSS,               // 过零检测
    MRS_STA_TIMER_CLTII_EVENT,              // II采事件监控
    MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL,  // II采本地抄表帧间隔
    MRS_STA_TIMER_LOCAL_MONITOR,            // STA本地监控主动上报状态字
    MRS_STA_TIMER_RM_METER_JOIN_NET,        // 抄表成功后等待5秒入网定时器
    MRS_STA_TIMER_CSG_CLTI_SM_QUERY,        // 搜表查询
    MRS_STA_TIMER_CSG_CLTI_QUEUE,           // 队列定时器

    MRS_STA_TIMER_CSG_QRY_DETECT_STATUS,    // 查询探测搜表状态

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_STA_TIMER_SET_CLT_MAC,
    MRS_STA_TIMER_CSG_CLTI_UPG,
    MRS_STA_TIMER_MODE_PHYSICAL_TEST,       //物理层测试模式
    MRS_STA_TIMER_ENTER_TESTMODE,           //进入测试模式
    //MRS_STA_TIMER_MODE_FREQ_FELLOW,         //频段跟随命令
    MRS_STA_TIMER_CSG_REBOOT,               //重启定时器
#endif

    MRS_STA_TIMER_RM_FRAME_INTERVAL,        /* Frame interval timer. CNcomment: 帧间隔定时器。CNend */
    MRS_STA_TIMER_RM_QUEUE_LOCK,            // STA队列锁5分钟启动自重启机制定时器
    MRS_STA_TIMER_POWERFAILURE,             //停电上报sta定时器

    MRS_STA_TIMER_NETWORK_LOCK_CHECK,       // 锁定后，定时检查锁定时间和离网时间是否达到门限
    MRS_STA_TIMER_REJOIN_AFTER_UNLOCK,      // 定时检查站点是否解锁网络，解锁后重新入网

    MRS_STA_TIMER_MAX
} MRS_STA_LOGIC_TIMER_ID_E;

#define MRS_STA_TIMER_INIT                      (MRS_STA_TIMER_TEST)
#define MRS_COLLECTOR_TIMER_IRDA_RESET          (MRS_STA_TIMER_IRDA_RESET)

//*****************************************************************************
// STA定时器的定时周期
//*****************************************************************************
#define MRS_STA_TIME_TEST                       (1500)          // 1.5秒
#define MRS_STA_TIME_INIT                       (1000*60*30)    // 30分钟
#define MRS_STA_TIME_HEART                      (1000*60*5)     // 变更为5分钟 (liuxipeng KF54842)
#define MRS_STA_TIME_IO_EVENTOUT_VALUE          (50)
#define MRS_STA_TIME_RUN_LED_0_25HZ             (2000)          // II型采集器中运行灯亮/灭定时器超时时间(0.25Hz)
#define MRS_STA_TIME_RUN_LED_0_50HZ             (1000)          // II型采集器中运行灯亮/灭定时器超时时间(0.50Hz)
#define MRS_STA_TIME_RUN_LED_5_00HZ             (100)           // II型采集器中运行灯亮/灭定时器超时时间(5.00Hz)
#define MRS_STA_TIME_UART_LED                   (100)           // II型采集器中UART收发灯亮/灭定时器超时时间
#define MRS_STA_TIME_GETMETER_ERR_LED           (1000)          // 表地址获取失败后LED闪烁定时器时间
#define MRS_STA_TIME_RESET_PIN                  (150)
#define MRS_COLLECTOR_TIME_IRDA_RESET           (500)           // II型采集器红外回应数据帧硬件初始化定时器超时时间
#define MRS_STA_TIME_SM_PERIOD_NO_METER         (1000*60*10)    // 周期性搜表定时器，周期10分钟

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIME_DAYFREEZE_REC              (1000 * 60 * 5)  // 周期性记录日冻结统计定时周期(每5分钟刷新)
#endif

#define MRS_STA_TIME_ZERO_CROSS                 (800)
#define MRS_STA_TIME_RM_METER_JOIN_NET          (5 * 1000)  // 抄表成功后等待入网的时间
#define MRS_STA_TIME_RM_METER_JOIN_NET_DUT      (1 * 1000)  // 测试I采出厂工装时抄表成功后等待入网的时间

#define MRS_STA_TIME_CSG_QRY_DETECT_STATUS      (15 * 1000)
#define MRS_STA_TIME_CSG_CLTI_UPG               (3000)

#define MRS_STA_TIME_RM_QUEUE_LOCK              (30 * 60 * 1000) //STA队列锁30分钟启动自重启机制


#define MRS_STA_TIME_NETWORK_LOCK_CHECK         (3 * 60 * 1000)     // 锁定离网时间门限检查周期，3分钟
#define MRS_STA_TIME_REJOIN_AFTER_UNLOCK        (1 * 1000)          // 解锁成功后，重新入网

//*****************************************************************************
// STA运行状态
//*****************************************************************************
#define MRS_STA_STATE_INIT                      (0)     // 初始化状态，未探测波特率
#define MRS_STA_STATE_MR                        (1)     // 抄表态，正常态，STA可以正常处理任何请求

//*****************************************************************************
// STA队列资源配置
//*****************************************************************************

#define MRS_STA_QUEUE_FROM_STA                  (0)     // 本地
#define MRS_STA_QUEUE_FROM_PLC                  (1)     // PLC
#define MRS_STA_QUEUE_FROM_LOCAL                (2)     // HSO
#define MRS_STA_QUEUE_FROM_REMOTE               (3)     // 抄控器(远端串口)
#define MRS_STA_QUEUE_FROM_REMOTE_HSO           (4)     // 抄控器(远端HSO)
#define MRS_STA_QUEUE_FROM_IR                   (5)     // 红外

#define MRS_STA_TRANSFER_TIMING_NORMAL          (1)     // 透抄校时命令(08校时)
#define MRS_STA_TRANSFER_TIMING_CLTII_EX        (2)     // 透抄校时命令(扩展校时)
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_IR_SUBID_SPECIAL_TRANSMIT           (3)     //II采红外特殊DI透抄命令
#define MRS_IR_SUBID_TRANSMIT                   (4)     //II采红外普通透抄命令
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_CLTI_EVT_QUERY_DISCARD              (5)     //废弃的I采事件查询命令

//*****************************************************************************
// STA其它定义
//*****************************************************************************
#define MRS_STA_ITEM_ID_HEART                   (HI_U16)(-1)    // 心跳ID
#define MRS_STA_ITEM_ID_TRANSMIT                (HI_U16)(-2)    // 转发数据
#define MRS_STA_ITEM_ID_SEARCH_METER_CLT_II     (HI_U16)(-3)    // 搜表命令
#define MRS_STA_ITEM_ID_SEARCH_METER_CLT_I      (HI_U16)(-4)    // I采搜表命令
#define MRS_STA_ITEM_ID_SEND_WORK_MODE_CLT_II   (HI_U16)(-5)    // II采发送工作模式(测试模式)
#define MRS_STA_ITEM_ID_EVENT                   (HI_U16)(-6)    // 事件查询
#define MRS_STA_ITEM_ID_CLTII_EVENT             (HI_U16)(-7)    // II采事件监控
#define MRS_STA_ITEM_ID_DUTY_CYCLE              (HI_U16)(-8)    // STA低功耗读取电表时间
#define MRS_STA_ITEM_ID_SET_VER_CLT_I           (HI_U16)(-9)    // I采模块设置版本号给I采
#define MRS_STA_ITEM_ID_CLTI_EVENT              (HI_U16)(-10)   // I采事件查询

#define MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER   (HI_U16)(-11)
#define MRS_STA_ITEM_ID_CSG_TRANS_XR            (HI_U16)(-12)
#define MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE     (HI_U16)(-13)

#define MRS_STA_SEND_ADDR_FRAME_TIMES_MAX       (150)   // 150次
#define MRS_STA_SEND_ADDR_FRAME_TIMES_DEFAULT   (7)     // 7次
#define MRS_STA_SEND_ADDR_FRAME_TIMES_MIN       (0)     // 0次

#define MRS_STA_WAIT_SEND_TIME_MAX              (24 * 60 * 60)  // 24小时
#define MRS_STA_WAIT_SEND_TIME_DEFAULT          (60)    // 60秒
#define MRS_STA_WAIT_SEND_TIME_MIN              (1)     // 1秒


//*****************************************************************************
// 产品类型
//*****************************************************************************
#define MRS_SRV_PRODUCT_STA                     (0)
#define MRS_SRV_PRODUCT_II_COLLECTOR            (1)


//*****************************************************************************
// CCO运行状态相关函数
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoSetState(HI_U32 state);
HI_PUBLIC HI_U32 mrsCcoGetState(HI_VOID);
HI_PUBLIC HI_VOID mrsCcoForcePlcConnect(HI_VOID);


//*****************************************************************************
// STA运行状态相关函数
//*****************************************************************************
HI_PUBLIC HI_U8 mrsStaSetState(HI_U8 state);
HI_PUBLIC HI_U8 mrsStaGetState(HI_VOID);


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_SRV_TIMER_MAX   MRS_CCO_TIMER_MAX
#else
#define MRS_SRV_TIMER_MAX   MRS_STA_TIMER_MAX
#endif

HI_END_HEADER
#endif //__MRS_SRV_RES_H__

