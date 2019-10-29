#ifndef __HI_EQT_H__
#define __HI_EQT_H__
#include <hi_mdm.h>
#include <hi_config.h>
HI_START_HEADER

//*****************************************************************************
//*****************************************************************************

typedef enum 
{
    HI_EQT_VVV,
}HI_EQT_ID_VVV_E;

//*****************************************************************************
// EQT和HSO报文定义, 使用默认枚举方式定义, HI_EQT_ID_MAX 为消息资源占用的最大值
// 取值约束: 从0开始定义, 最大为 HI_EQT_ID_MAX, 不超过 256 
// 消息在主控任务中执行;
// 执行入口定义为 Msg1ExecuteProc(HI_U32 ulMsgId, HI_U8* pucPayload, HI_U16 usPayloadSize, HI_U8 ucTransId, HI_U32 ulOption)
//*****************************************************************************
typedef enum 
{
    HI_EQT_ID_ERR = 0x00,
    HI_EQT_ID_PK_645, // REQ: eqt_HsoReqPk645; 
    
    // TODO:
    
    HI_EQT_ID_MAX = 256 // 该值必须大于 eqt_io.cfg配置文件中的 HSO REQ 个数, 否则无法启动
}HI_EQT_ID_E;
//*****************************************************************************




//*****************************************************************************
//*****************************************************************************
HI_END_HEADER
#endif // __HI_EQT_H__

