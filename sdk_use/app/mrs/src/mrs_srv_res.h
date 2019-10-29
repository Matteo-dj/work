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
#define MRS_CCO_TIMER_MAP                       (EN_APP_TASK_TIMER_MRS_1)   // 映射表维护定时器ID
#define MRS_CCO_TIMER_STA                       (EN_APP_TASK_TIMER_MRS_2)   // 与STA通讯定时器ID
#define MRS_CCO_TIMER_MASTER                    (EN_APP_TASK_TIMER_MRS_3)   // 与集中器通讯定时器ID
#define MRS_CCO_TIMER_READMETER_ACTIVE          (EN_APP_TASK_TIMER_MRS_4)   // 读表模块激活定时器
#define MRS_CCO_TIMER_SERVICE_END               (EN_APP_TASK_TIMER_MRS_5)   // 服务结束定时器(用于搜表或随抄等服务的结束)
#define MRS_CCO_TIMER_FLASHWRITE_ACTIVE         (EN_APP_TASK_TIMER_MRS_6)   // 表档案存盘激活定时器ID
#define MRS_CCO_TIMER_GET_TIME                  (EN_APP_TASK_TIMER_MRS_7)   // 定时获取集中器时间定时器
#define MRS_CCO_TIMER_SYSTEM_RESET              (EN_APP_TASK_TIMER_MRS_8)   // AFN=01H-F1执行硬件重启定时器ID
#define MRS_CCO_TIMER_PARALLEL_CHECK            (EN_APP_TASK_TIMER_MRS_9)   // 并发读表PLC超时检查定时器
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE    (EN_APP_TASK_TIMER_MRS_10)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_TIMER_BROADCAST                 (EN_APP_TASK_TIMER_MRS_11)
#define MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE     (EN_APP_TASK_TIMER_MRS_12)
#define MRS_CCO_TIMER_EVENT_ACTIVE              (EN_APP_TASK_TIMER_MRS_13)
#define MRS_CCO_TIMER_MASTER_ACTIVE             (EN_APP_TASK_TIMER_MRS_14)
#define MRS_CCO_TIMER_MASTER_REPORT             (EN_APP_TASK_TIMER_MRS_15)
#define MRS_CCO_TIMER_RESET_PIN                 (EN_APP_TASK_TIMER_MRS_16)
#define MRS_CCO_TIMER_SIMU_CHL                  (EN_APP_TASK_TIMER_MRS_17)
#define MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME    (EN_APP_TASK_TIMER_MRS_18)
#define MRS_CCO_TIMER_GET_METERLIST             (EN_APP_TASK_TIMER_MRS_19)
#define MRS_CCO_TIMER_TOPO_QUERY                (EN_APP_TASK_TIMER_MRS_20)
#define MRS_CCO_TIMER_CHECK_MAC_MAP_TTL         (EN_APP_TASK_TIMER_MRS_21)
#define MRS_CCO_TIMER_MAP_REFRESH               (EN_APP_TASK_TIMER_MRS_22)
#define MRS_CCO_TIMER_SEARCH_METER_END          (EN_APP_TASK_TIMER_MRS_23)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/13   问题单号号:DTS2014012607684 */
#define MRS_CCO_TIMER_UPG_FILE_RECEIVE          (EN_APP_TASK_TIMER_MRS_26)  //升级下装阶段接收到升级数据超时定时器
#define MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE     (EN_APP_TASK_TIMER_MRS_27)  //停止升级重发定时器
#define MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE   (EN_APP_TASK_TIMER_MRS_28)  //升级状态查询定时器
#define MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT  (EN_APP_TASK_TIMER_MRS_29)  //升级状态查询超时定时器
/* END:   Added by fengxiaomin/00209182, 2014/1/13 */
#define MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE (EN_APP_TASK_TIMER_MRS_30)
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIMER_ACQUIRE_CLOCK             (EN_APP_TASK_TIMER_MRS_31)
#define MRS_CCO_TIMER_MRS_LOG                   (EN_APP_TASK_TIMER_MRS_32)
#endif
#define MRS_CCO_TIMER_SEARCH_METER_END_TF       (EN_APP_TASK_TIMER_MRS_33)
#define MRS_CCO_TIMER_SEARCH_METER_WHITELIST    (EN_APP_TASK_TIMER_MRS_34)
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
#define MRS_CCO_TIMER_SEARCH_REPORT             (EN_APP_TASK_TIMER_MRS_35)
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */
#define MRS_CCO_TIMER_TEST_MODE_FRAME           (EN_APP_TASK_TIMER_MRS_36)
#define MRS_CCO_TIMER_ZERO_CROSS                (EN_APP_TASK_TIMER_MRS_37)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
#define MRS_CCO_TIMER_RUN_LED_BBU               (EN_APP_TASK_TIMER_MRS_38)
#endif

#define MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP    (EN_APP_TASK_TIMER_MRS_39) //将以mac地址入网的采集器加入白名单后，如果30分钟内没有加入白名单的采集器站点入网,删除添加的白名单
#define MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE   (EN_APP_TASK_TIMER_MRS_40) //探测抄表机制中缓存拒绝站点信息定时器
#define MRS_CCO_TIMER_DETECT_WAIT_TOPO          (EN_APP_TASK_TIMER_MRS_41) //探测机制中等待拓扑组网,一定时间后拓扑中仍只有CCO,后续将拒绝的站点加入白名单
#define MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE     (EN_APP_TASK_TIMER_MRS_42) //探测机制中缓存在追加白名单中的站点入网定时器,超时后启动探测抄表
#define MRS_CCO_TIMER_DETECT_BLACK_LIST         (EN_APP_TASK_TIMER_MRS_43) //黑名单超时处理定时器
#define MRS_CCO_TIMER_DETECT_REFRESH_WHITE      (EN_APP_TASK_TIMER_MRS_44) //探测抄表机制中通知刷新白名单定时器
#define MRS_CCO_TIMER_RESUME_DETECT_RM          (EN_APP_TASK_TIMER_MRS_45) //探测抄表机制中通知刷新白名单定时器
#define MRS_CCO_TIMER_ENTER_TESTMODE            (EN_APP_TASK_TIMER_MRS_46) //进入测试模式
#define MRS_CCO_TIMER_TESTMODE                  (EN_APP_TASK_TIMER_MRS_47) //测试模式
#define MRS_CCO_TIMER_PHYSICAL_TESTMODE         (EN_APP_TASK_TIMER_MRS_48) //ccy 业务串口测试模式 定时器
#define MRS_CCO_TIMER_MSDU_TESTMODE             (EN_APP_TASK_TIMER_MRS_49) //cco msdu上报业务串口测试定时器
#define MRS_CCO_TIMER_POWERFAILURE              (EN_APP_TASK_TIMER_MRS_50)
#define MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA   (EN_APP_TASK_TIMER_MRS_51) //cco擦除上报数据定时器
#define MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK   (EN_APP_TASK_TIMER_MRS_52) //通知站点绑定网络
#define MRS_CCO_TIMER_CHECK_MAIN_NODE_ADDR      (EN_APP_TASK_TIMER_MRS_53) //检查主节点地址
#define MRS_CCO_TIMER_TF_RESULT_QUERY_NEXT      (EN_APP_TASK_TIMER_MRS_54) //台区识别结果查询: 下一次查询开始定时器
#define MRS_CCO_TIMER_TF_RESULT_QUERY_ROUND     (EN_APP_TASK_TIMER_MRS_55) //台区识别结果查询: 新一轮查询开始定时器
#define MRS_CCO_TIMER_TF_RESULT_QUERY_BEGIN     (EN_APP_TASK_TIMER_MRS_56) //台区识别结果查询: 开始定时器
#define MRS_CCO_TIMER_POWEROFF_DETECT           (EN_APP_TASK_TIMER_MRS_57) //停电探测定时器
#define MRS_CCO_TIMER_POWEROFF_RECHECK          (EN_APP_TASK_TIMER_MRS_58) //停电复核定时器
#define MRS_CCO_TIMER_POWEROFF_STABLE_CHECK     (EN_APP_TASK_TIMER_MRS_59) //停电模块站点稳定性检查
#define MRS_CCO_TIMER_POWEROFF_MAC_CACHE        (EN_APP_TASK_TIMER_MRS_60) //停电模块MAC缓存定时器
#define MRS_CCO_TIMER_POWEROFF_DEAL_SCALE_OFF   (EN_APP_TASK_TIMER_MRS_61) //停电模块处理大规模停电(探测站点数目到达10个的时候启动该定时器)
#define MRS_CCO_TIMER_NDM_PLC_READ_METER        (EN_APP_TASK_TIMER_MRS_63) //抄控器和集中器同时连接cco点抄读表时，抄控器缓冲区定时器
#define MRS_CCO_TIMER_STA_PLC_READ_DEV_ID       (EN_APP_TASK_TIMER_MRS_64) //通过cco读取sta、I采，II采设备各类ID信息，等待plc帧返回定时器
#define MRS_CCO_TIMER_CLT_POWER_REPORT          (EN_APP_TASK_TIMER_MRS_65) //采集器停电上电上报
#define MRS_CCO_TIMER_CLT_POWEOFF_ERASE_DATA    (EN_APP_TASK_TIMER_MRS_66) //cco擦除采集器下接表上报数据定时器
#define MRS_CCO_TIMER_POWERON                   (EN_APP_TASK_TIMER_MRS_67) // cco复电上报定时器
#define MRS_CCO_TIMER_POWERON_ERASE_DATA   		(EN_APP_TASK_TIMER_MRS_68) //cco擦除上报数据定时器
#define MRS_CCO_TIMER_RF_UART_TIME      		(EN_APP_TASK_TIMER_MRS_69) //STA RF 定时器
#define MRS_CCO_TIMER_RF_TIME      				(EN_APP_TASK_TIMER_MRS_70) //CCO RF 定时器
#define MRS_CCO_TIMER_RF_READ_WATERMETER    	(EN_APP_TASK_TIMER_MRS_71) //启动抄读水表定时器
#define MRS_CCO_TIMER_GET_WATER_METERLIST       (EN_APP_TASK_TIMER_MRS_72) // 水表搜表
#define MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP  (EN_APP_TASK_TIMER_MRS_73) // 设置水表关系
#define MRS_CCO_TIMER_SGD_RF_TIME               (EN_APP_TASK_TIMER_MRS_74) //CCO 国网 RF 定时器
//Matteo
#define MRS_CCO_TIMER_ARCHIVE_CHANGE_CHECK      (EN_APP_TASK_TIMER_MRS_81)

#define MRS_CCO_TIMER_MAX                       (EN_APP_TIMER_MAX)

#define EN_MRS_SRV_CCO_MAP_VIN_TIMER            (MRS_CCO_TIMER_MAP)
#define EN_MRS_SRV_CCO_STA_TIMER                (MRS_CCO_TIMER_STA)
#define EN_MRS_SRV_CCO_MUSTER_TIMER             (MRS_CCO_TIMER_MASTER)
#define EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER    (MRS_CCO_TIMER_READMETER_ACTIVE)
#define EN_MRS_SRV_CCO_SERVICE_END_TIMER        (MRS_CCO_TIMER_SERVICE_END)
#define EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER  (MRS_CCO_TIMER_FLASHWRITE_ACTIVE)
#define EN_MRS_SRV_CCO_GET_TIME_TIMER           (MRS_CCO_TIMER_GET_TIME)
#define EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER       (MRS_CCO_TIMER_SYSTEM_RESET)
#define EN_MRS_SRV_CCO_RM_CHECK_TIMER           (MRS_CCO_TIMER_PARALLEL_CHECK)
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
//#define EN_MRS_SRV_CCO_SEARCHMETER_TIMER        (MRS_CCO_TIMER_SEARCHMETER)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
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

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define MS_PER_SECOND                           (1000)      //定时器基本单位  1s
#define MRS_CCO_TIME_FLASHWRITE_ACTIVE          (20)     // 表档案存盘激活时间  默认20s
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

#define MRS_CCO_TIME_SYSTEM_RESET               (500)       // AFN=01H-F1执行硬件重启超时时间
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
#define MRS_CCO_TIME_PARALLEL_CHECK             (50)       // 并发读表超时检查定时器默认值(每间隔此时间做一次超时检查)
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
#define MRS_CCO_TIME_SEARCHMETER                (2000)
#define MRS_CCO_TIME_RESET_PIN                  (150)

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIME_ACQUIRE_CLOCK              (1500)
#define MRS_CCO_TIME_CHECK_CLOCK                (1000*60)
#define MRS_CCO_TIME_STARTUP_CLOCK              (1000*60*20) //CCO上电20分钟后开始问询集中器时钟的时间
#define MRS_CCO_TIME_ACTIVE_ACQUIRE_CLOCK       (200) //CCO收到路由重启或路由恢复命令后，激活问询集中器时钟的时间
//#define MRS_CCO_TIME_GET_CLOCK                  (1000 * 8)//CCO上电后阶段问询时钟与集中器之间的超时时间
#endif

#define MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT      (2)             // 集中器通讯默认最大重试次数
#define MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT        (1000 * 10)     // 集中器通讯默认超时时间(模拟通道关闭),单位:ms

#define MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT  (1000 * 60)   // 集中器通讯默认超时时间(模拟通道打开),单位:ms

#define MRS_SRV_CCO_TIMER_MUSTER_VALUE                  (MRS_CCO_TIME_MASTER)
#define MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE         (MRS_CCO_TIME_READMETER_ACTIVE)
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
#define MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE            (MRS_CCO_TIME_READMETER_ACTIVE)
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */
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
#define MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT           (3 * 1000) //集中器透抄一帧结束后恢复探测抄表的时间
#define MRS_CCO_TIME_CLT_POWER_REPORT_VALUE             (5 *1000)  //CCO收到采集器停上电报文后,汇聚等待上报集中器的时间
//*****************************************************************************
// CCO运行状态
//*****************************************************************************
#define MRS_CCO_STATE_CREATE_MAP                (0)     // 初始化状态，未探测波特率
#define MRS_CCO_STATE_NORMAL                    (1)     // 抄表态，正常态，STA可以正常处理任何请求


//*****************************************************************************
// STA的定时器
//*****************************************************************************
#define MRS_STA_TIMER_TEST                      (EN_APP_TASK_TIMER_MRS_1)
#define MRS_STA_TIMER_INIT                      MRS_STA_TIMER_TEST
#define MRS_STA_TIMER_HEART                     (EN_APP_TASK_TIMER_MRS_2)   // 心跳定时器(与地址获取失败LED闪烁定时器复用:MRS_STA_TIMER_TEST状态下为LED定时器)
#define MRS_STA_TIMER_QUEUE                     (EN_APP_TASK_TIMER_MRS_3)
#define MRS_STA_TIMER_TTL                       (EN_APP_TASK_TIMER_MRS_4)
#define MRS_STA_TIMER_IO_EVENT_OUT              (EN_APP_TASK_TIMER_MRS_5)   // IO控制模块中EVENT_OUT管脚查看状态定时器ID
#define MRS_STA_TIMER_RUN_LED                   (EN_APP_TASK_TIMER_MRS_6)   // II型采集器中运行灯亮/灭定时器
#define MRS_STA_TIMER_UART_LED                  (EN_APP_TASK_TIMER_MRS_7)   // II型采集器中UART收发灯亮/灭定时器
#define MRS_STA_TIMER_IRDA_RESET                (EN_APP_TASK_TIMER_MRS_8)   // II型采集器红外回应数据帧硬件初始化定时器
#define MRS_STA_TIMER_EVENT_REPORT              (EN_APP_TASK_TIMER_MRS_9)
#define MRS_STA_TIMER_ZERO_CROSS_ADC            (EN_APP_TASK_TIMER_MRS_10)
#define MRS_STA_TIMER_SM_FRAME                  (EN_APP_TASK_TIMER_MRS_11)  // 搜表等待表应答首字节定时器
#define MRS_STA_TIMER_SM_BYTE                   (EN_APP_TASK_TIMER_MRS_12)  // 表应答数据字节间隔定时器
#define MRS_STA_TIMER_SM_VM                     (EN_APP_TASK_TIMER_MRS_13)  // 虚拟表被搜功能定时器
#define MRS_STA_TIMER_SM_PERIOD                 (EN_APP_TASK_TIMER_MRS_14)  // 周期性搜表定时器
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIMER_DAYFREEZE_REC             (EN_APP_TASK_TIMER_MRS_15)  // 周期性记录日冻结统计定时器
#endif
#define MRS_STA_TIMER_RESET_PIN                 (MRS_CCO_TIMER_RESET_PIN)   // EN_APP_TASK_TIMER_MRS_16
#define MRS_STA_TIMER_SIMU_CHL                  (MRS_CCO_TIMER_SIMU_CHL)    // EN_APP_TASK_TIMER_MRS_17
#define MRS_STA_TIMER_CLTI_POSITIVE_A           (EN_APP_TASK_TIMER_MRS_18)  // I采: STA上电读上次入网电表(+A)定时器
#define MRS_STA_TIMER_CLTI_SM_QUERY             (EN_APP_TASK_TIMER_MRS_19)  // I采: 查询搜表结果定时器
#define MRS_STA_TIMER_CLTI_SM_START             (EN_APP_TASK_TIMER_MRS_20)  // I采: 搜表开始帧定时器
#define MRS_STA_TIMER_CLTI_SM_STOP              (EN_APP_TASK_TIMER_MRS_21)  // I采: 搜表停止帧定时器
#define MRS_STA_TIMER_RM_FRAME_INTERVAL         (EN_APP_TASK_TIMER_MRS_22)  // Frame interval timer
#define MRS_STA_TIMER_VM_EVT_ACTIVE             (EN_APP_TASK_TIMER_MRS_23)  // 模拟表事件激活定时器
#define MRS_STA_TIMER_ZERO_CROSS                (EN_APP_TASK_TIMER_MRS_24)  // 过零检测
#define MRS_STA_TIMER_CLTII_EVENT               (EN_APP_TASK_TIMER_MRS_25)  // II采事件监控
#define MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL   (EN_APP_TASK_TIMER_MRS_26)  // II采本地抄表帧间隔
#define MRS_STA_TIMER_LOCAL_MONITOR             (EN_APP_TASK_TIMER_MRS_27)  // STA本地监控主动上报状态字
#define MRS_STA_TIMER_SM_PERIOD_NO_METER        (EN_APP_TASK_TIMER_MRS_28)  // 上电未搜到表，间隔10分钟搜表
#define MRS_STA_TIMER_RM_METER_JOIN_NET         (EN_APP_TASK_TIMER_MRS_29)  // 抄表成功后等待5秒入网定时器
#define MRS_STA_TIMER_CLTI_UPG                  (EN_APP_TASK_TIMER_MRS_30)
#define MRS_STA_TIMER_ENTER_TESTMODE            (EN_APP_TASK_TIMER_MRS_31)  // 进入测试模式
#define MRS_STA_TIMER_TESTMODE                  (EN_APP_TASK_TIMER_MRS_32)  // 测试模式
#define MRS_STA_TIMER_PHYSICAL_TESTMODE         (EN_APP_TASK_TIMER_MRS_33)  // 物理层转发
#define MRS_STA_TIMER_MSDU_TESTMODE             (EN_APP_TASK_TIMER_MRS_34)  // MSDU转发
#define MRS_STA_TIMER_POWERFAILURE              (EN_APP_TASK_TIMER_MRS_35)
#define MRS_STA_TIMER_NETWORK_LOCK_CHECK        (EN_APP_TASK_TIMER_MRS_36)  // 锁定后，定时检查锁定时间和离网时间是否达到门限
#define MRS_STA_TIMER_REJOIN_AFTER_UNLOCK       (EN_APP_TASK_TIMER_MRS_37)  // 定时检查站点是否解锁网络，解锁后重新入网
#define MRS_STA_TIMER_TEST_MODE_FRAME           (EN_APP_TASK_TIMER_MRS_38)  // 发送测试模式报文
#define MRS_STA_TIMER_RM_QUEUE_LOCK             (EN_APP_TASK_TIMER_MRS_39)  // STA队列锁30分钟启动自重启机制定时器
#define MRS_STA_TIMER_WAIT_POWERFAILURE_ACK     (EN_APP_TASK_TIMER_MRS_40)  // 等待停电ack定时器
#define MRS_STA_TIMER_POWERON_PLC_SEND          (EN_APP_TASK_TIMER_MRS_41)
#define MRS_STA_TIMER_CLT_POWEROFF_WAIT_METER   (EN_APP_TASK_TIMER_MRS_42)
#define MRS_STA_TIMER_CLT_SEND_PLC              (EN_APP_TASK_TIMER_MRS_43)
#define MRS_STA_TIMER_COLLECT_VOLTAGE_BEGIN     (EN_APP_TASK_TIMER_MRS_44)  // 工频电压采集开始定时器
#define MRS_STA_TIMER_COLLECT_VOLTAGE           (EN_APP_TASK_TIMER_MRS_45)  // 工频电压采集定时器
#define MRS_STA_TIMER_COLLECT_FREQENCY_BEGIN    (EN_APP_TASK_TIMER_MRS_46)  // 工频频率采集开始定时器
#define MRS_STA_TIMER_COLLECT_FREQENCY          (EN_APP_TASK_TIMER_MRS_47)  // 工频频率采集定时器
#define MRS_STA_TIMER_RF_TIME      				(EN_APP_TASK_TIMER_MRS_57) //STA RF 定时器
#define MRS_STA_TIMER_POWERON              		(EN_APP_TASK_TIMER_MRS_69)
#define MRS_STA_PULL_RST_DELAY_TIMER	 		 EN_APP_TASK_TIMER_MRS_70	// STA延迟响应拉动rst管脚定时器
#define MRS_STA_DISABLE_POWER_FAILURE_TIMER		 EN_APP_TASK_TIMER_MRS_71	// TSTA 关闭停电检测定时器
//CJJ-HPLC:2018-9-7
#define MRS_STA_TIMER_RF_OVER_TIME               (EN_APP_TASK_TIMER_MRS_80)
//CJJ-HPLC:2018-9-7 
#define MRS_STA_TIMER_MAX                       (EN_APP_TIMER_MAX)

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
#define MRS_STA_TIME_UART_LED_TEST              (20)            // II型采集器中UART收发灯亮/灭定时器超时时间
#define MRS_STA_TIME_UART_LED_NORMAL            (100)           // II型采集器中UART收发灯亮/灭定时器超时时间
#define MRS_STA_TIME_GETMETER_ERR_LED           (1000)          // 表地址获取失败后LED闪烁定时器时间
#define MRS_STA_TIME_RESET_PIN                  (150)
#define MRS_COLLECTOR_TIME_IRDA_RESET           (500)           // II型采集器红外回应数据帧硬件初始化定时器超时时间
#define MRS_STA_TIME_SM_PERIOD_NO_METER         (1000*60*10)    // 周期性搜表定时器，周期10分钟

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIME_DAYFREEZE_REC              (1000 * 60 * 5)  // 周期性记录日冻结统计定时周期(每5分钟刷新)
#endif

#define MRS_STA_TIME_ZERO_CROSS                 (800)
#define MRS_STA_TIME_RM_METER_JOIN_NET          (5 * 1000)          // 抄表成功后等待入网的时间
#define MRS_STA_TIME_RM_METER_JOIN_NET_DUT      (1 * 1000)          // 测试I采出厂工装时抄表成功后等待入网的时间
#define MRS_STA_TIME_CLTI_UPG                   (1000)
#define MRS_STA_TIME_NETWORK_LOCK_CHECK         (3 * 60 * 1000)     // 锁定离网时间门限检查周期，3分钟
#define MRS_STA_TIME_REJOIN_AFTER_UNLOCK        (1 * 1000)          // 解锁成功后，重新入网

#define MRS_STA_TIME_RM_QUEUE_LOCK              (30 * 60 * 1000)    // STA队列锁30分钟启动自重启机制


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
#define MRS_STA_QUEUE_FROM_RF                   (6)     // 无线

#define MRS_STA_TRANSFER_TIMING_NORMAL          (1)     // 透抄校时命令(08校时)
#define MRS_STA_TRANSFER_TIMING_CLTII_EX        (2)     // 透抄校时命令(扩展校时)
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_IR_SUBID_SPECIAL_TRANSMIT           (3)     //II采红外特殊DI透抄命令
#define MRS_IR_SUBID_TRANSMIT                   (4)     //II采红外普通透抄命令
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_CLTI_EVT_QUERY_DISCARD              (5)     //废弃的I采事件查询命令

#define MRS_RF_SUBID_TRANSMIT           (6)     // 无线
#define MRS_RF_SUBID_TRANSMIT_TOOL           (7)     // 无线 工装命令

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
#define MRS_STA_ITEM_ID_DETECT                  (HI_U16)(-11)   // STA上电检测,获取表地址
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 debug */
#define MRS_STA_ITEM_ID_TRAN_DATA_CLT_II        (HI_U16)(-12)   // II采485口联调
#define MRS_STA_ITEM_ID_RM_CLT_II               (HI_U16)(-13)   // II采try搜表结果是否可以抄通

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


HI_END_HEADER
#endif //__MRS_SRV_RES_H__

