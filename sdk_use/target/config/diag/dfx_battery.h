
#ifndef __DFX_BATTERY_H__
#define __DFX_BATTERY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef enum
{
    BQ_UPG_FAIL_REASON_NO,
    BQ_UPG_FAIL_REASON_ID_FILE_SIZE,            //升级文件大小检测失败
    BQ_UPG_FAIL_REASON_ID_FILE_CONTENT,         //升级文件内容检测失败
    BQ_UPG_FAIL_REASON_ID_CRC,                  //CRC校验错误
    BQ_UPG_FAIL_REASON_ID_OVER_TIME,            //超时
    BQ_UPG_FAIL_REASON_ID_POWER_OFF,            //异常断电
    BQ_UPG_FAIL_REASON_ID_CHANGE_MOD_FAIL,      //切换到升级模式失败
    BQ_UPG_FAIL_REASON_ID_I2C,                  //与BQ芯片交互问题
    BQ_UPG_FAIL_REASON_WORK_QUEUE_BUSY,
}BQ_UPG_FAIL_REASON_ID_E;//升级失败具体原因，统计用


typedef struct
{
    //升级相关参数
    HI_U16 mainVer;     //BQ芯片主版本号
    HI_U16 subVer;      //电池芯片firmware芯片升级的版本序号
    HI_U32 ulCheckSum;  //电池芯片firmware升级的CRC32
    HI_U16 usUpgStatus; //电池芯片firmware升级状态  HI_NDM_BAT_UPG_STATUS_E
    HI_U16 usErrCode;   //电池芯片当前是否允许升级  HI_NDM_BAT_UPG_ERR_CODE_E

    //电池实时状态
    HI_U16 voltValue;//电池电压(mv)
    HI_U16 temperature;//电池温度(k)
    HI_S16 current;//电池电流(mA)放电为负数，充电为正数
    HI_BOOL isBatExist;
    HI_U8 ucBatteryPercent;//等价于电池在位&&电池版本正确;与电池电量<=100%同时满足时上报给HSO电池电量

    //电池初始化状态
    HI_U16 initVoltValue;
    HI_U16 initTemperature;
    HI_S16 initCurrent;
    HI_BOOL initIsBatExist;
    HI_U8 initBatPercent;

    //其它控制信息
    HI_BOOL isInit;//是否初始化完成 HI_TRUE初始化完成
    HI_BOOL isResetting;// HI_TURE:正在重启中
    HI_U8 pad_1[2];

    //调试参数统计量
    HI_U32 currentExeLine;
    HI_U8 bqUpgFailReason;//BQ_UPG_FAIL_REASON_ID_E
    HI_U8 bqUpgNotAllowReason;//BQ_UPG_NOT_ALLOW_ID_E
    HI_U8 writeNvFailTimes;
    HI_U8 readNvFailTimes;
    HI_U8 readFirmWareVersionFail;
    HI_U8 readTemperatureFailTimes;
    HI_U8 readVoltValFailTimes;
    HI_U8 readCurrentFailTimes;

    HI_U8 readBatPercentFailTimes;
    HI_U8 isSupportBq;
    HI_U8 pad_2[2];
}BQ27510_STATUS_S;

#endif

