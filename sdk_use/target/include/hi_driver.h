//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_driver.h
//  版 本 号   : V1.0
//  作    者   : 张娜/00193780
//  生成日期   : 2011年4月17日
//  功能描述   : 
//
//               驱动适配(Cipher、DMA、Gpio、I2C、Watchdog、LED、SPI、uart、flash、CRC、IR...);
//  函数列表   :
//****************************************************************************

#ifndef __HI_DRIVER_H__
#define __HI_DRIVER_H__
#include <hi_types.h>
HI_START_HEADER
#include <hi_errno.h>
#include <dfx_sys.h>
#include <hi_mdm.h>
#include <hi_product_config.h>
#include <hi_mdm_io.h>
#include <hi_mdm_led.h>
#include <hi_mdm_switch.h>
#include <hi_mdm_i2c.h>

//************************************************************************************************************************
// I2C Interface
//************************************************************************************************************************




//************************************************************************************************************************
// GPIO Interface
//************************************************************************************************************************
typedef enum
{
	HI_GPIO_0 = 0,
	HI_GPIO_1,
	HI_GPIO_2,
	HI_GPIO_3,
}HI_GPIO_GROUP_NUM_E;

typedef enum
{
	HI_GPIO_BIT0 = 0,
	HI_GPIO_BIT1,
	HI_GPIO_BIT2,
	HI_GPIO_BIT3,
	HI_GPIO_BIT4,
	HI_GPIO_BIT5,
	HI_GPIO_BIT6,
	HI_GPIO_BIT7,
}HI_GPIO_BIT_NUM_E;

typedef struct
{
    HI_GPIO_GROUP_NUM_E enGpioIndex;
    HI_GPIO_BIT_NUM_E enGpioBit;
    union hiGPIO_VALUE_DIRECTION_U
    {
    	HI_GPIO_VALUE_E enGpioValue;
    	HI_GPIO_DIRECTION_E enGpioDirec;
    }stGpioValueDirec;
} HI_GPIO_DATA_S;

typedef struct
{
    HI_GPIO_GROUP_NUM_E enGpioIndexInt;
    HI_GPIO_BIT_NUM_E enGpioBitInt;
    HI_GPIO_INT_E enLevelnEdge;
    HI_GPIO_INT_POLARITY_E enRisingnFalling;
    HI_S32 (*gpio_callback)(HI_U32 ulCalbacArgv);
    HI_U32 ulCallbackArgv;
}HI_GPIO_INT_CTRL_S;



//*****************************************************************************
// 函数名称: hieth_isinit
// 功能描述: 抄控器自动工装检测，网口初始化完成标识
//          
//
// 参数说明:
//          无
//
// 返 回 值:
//          网口初始化完成结果
//
// 调用要求: 无
// 调用举例:
// 作    者: zhangyang/z00218821 [2014-06-16]
//*****************************************************************************
int hieth_isinit(void);

//*****************************************************************************

//************************************************************************************************************************
// Hrtimer Interface
//************************************************************************************************************************

#include <hi_mdm_hrtimer.h>
//*****************************************************************************
// 函数名称: HI_HRTIMER_Init
// 功能描述: 初始化Htr
//
// 参数说明:
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求: 如果第一次成功调用后再调用，则计数后直接返回HI_ERR_SUCCESS。
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_HRTIMER_Init(HI_VOID);
//*****************************************************************************
// 函数名称: HI_HRTIMER_Destroy
// 功能描述: 去初始化 Hrt.
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求: 需要与HI_HRTIMER_Init成对使用。
//           如果active链表和expired链表不为空，返回HI_ERR_HRT_BUSY。
//           计数值不为0时，计数值自减1后返回HI_ERR_SUCCESS。
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_HRTIMER_Destroy(HI_VOID);
//*****************************************************************************
// 函数名称: HI_HRTIMER_Start
// 功能描述: 增加一个定时器。定时器个数最大个数限制为100个
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求: 当Timer为空,expires为0，或者func为空时，返回HI_ERR_HRT_BAD_PARA
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_HRTIMER_Start(HI_HRTIMER_TYPE_S *timer);
//*****************************************************************************
// 函数名称: HI_HRTIMER_Stop
// 功能描述: 删除一个定时器。
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求: 当Timer为空,expires为0，或者func为空时，返回HI_ERR_HRT_BAD_PARA
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_HRTIMER_Stop(HI_HRTIMER_TYPE_S *timer);
//*****************************************************************************
// 函数名称: HI_HRTIMER_Modify
// 功能描述: 使用用户输入的expires,timout,data等值替换timer的初始设置值。
//
// 参数说明:HI_HRTIMER_TYPE_S*timer  //要修改的定时器指针，不能为空。
//          Expires     //修改后的定时长度，不能为0
//          Func       //修改后的用户回调函数，不能为空
//          Data      //回调函数传参，可为空。
//
// 返 回 值:
//
// 调用要求: 实现方法为删除之前的timer,重新add.
//           当Timer为空,expires为0，或者func为空时，返回HI_ERR_HRT_BAD_PARA
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_HRTIMER_Modify(HI_HRTIMER_TYPE_S *timer, HI_U32 expires, HI_VOID (*timeout)(HI_U32 ARG), HI_U32 data);



//*************************************************************************************************************************
// FLASH Interface
//*************************************************************************************************************************
/**
 * @ingroup hct_FLASH
 * HI_MDM_FLASH_Ioctl命令选项
 */
#define HI_GET_FLASH_INFO 0/**< 获取FLASH信息*/ 

/**
 * @ingroup hct_FLASH
 * FLASH信息
 */
typedef struct
{
    HI_U32 ulTotalSize;     /**< FLASH容量，单位:字节*/ 
    HI_U32 ulSectorSize;    /**< sector大小，单位:字节*/ 
    HI_U32 ulPageSize;      /**< 页大小,单位:字节*/ 
}HI_FLASH_INFO_S;

//*****************************************************************************
// 函数名称: HI_FLASH_Init
// 功能描述: 初始化flash设备
//
// 参数说明:
//           无
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_Init(HI_VOID);

//*****************************************************************************
// 函数名称: HI_FLASH_DeInit
// 功能描述: 去初始化flash设备
//
// 参数说明:
//           无
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_DeInit(HI_VOID);





//*****************************************************************************
// 函数名称: HI_FLASH_Write
// 功能描述: 写入flash操作
//
// 参数说明:
//           byte_offset：操作地址相对于FLASH 0地址的偏移；
//           buffer：写入buffer起始地址；
//           byte_size：写入字节数；
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败:
//           地址越界返回HI_ERR_FLASH_ADDR_BEYOND
//           写入boot区域（暂定1M）返回HI_ERR_FLASH_WRITE_IN_BOOT
//           其他错误返回HI_ERR_FAILURE；
//
// 调用要求: 内部包含擦除Flash内容操作；实现方式为DMA方式。
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_Write(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U8 *pBuf, HI_IN HI_U32 ulByteSize, HI_IN HI_BOOL bDoErase);
//*****************************************************************************
// 函数名称: HI_FLASH_Read
// 功能描述: 读flash操作
//
// 参数说明:
//           byte_offset：操作地址相对于FLASH 0地址的偏移；
//           buffer：写入buffer起始地址；
//           byte_size：写入字节数；
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_Read(HI_IN HI_U32 ulByteOffAddr, HI_OUT HI_U8 *pBuf, HI_IN HI_U32 ulByteSize);

//*****************************************************************************
// 函数名称: HI_FLASH_Erase
// 功能描述: 擦写flash操作
//
// 参数说明:
//           byte_offset：操作地址相对于FLASH 0地址的偏移；
//           byte_size：写入字节数；
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_Erase(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U32 ulByteSize);

//*****************************************************************************
// 函数名称: HI_FLASH_Ioctl
// 功能描述: Flash的信息获取
//
// 参数说明:
//           usCommand：输入命令；
//           pstFlashInfo：flash信息；
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_Ioctl(HI_IN HI_U16 usCommand, HI_OUT HI_FLASH_INFO_S *pstFlashInfo);

#ifdef PRODUCT_CFG_FLASH_PROTECT
//*****************************************************************************
// 函数名称: HI_FLASH_PortectInit
// 功能描述: flash保护初始化
//
// 参数说明:
//           无
//
// 返 回 值:
//           HI_ERR_SUCCESS初始化成功，其他值失败
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_PortectInit(HI_VOID);
//*****************************************************************************
// 函数名称: HI_FLASH_RemoveProtect
// 功能描述: 关闭flash保护功能，且将整片flash解保护，重新上电后恢复保护功能。
//
// 参数说明:
//
// 返 回 值:HI_ERR_SUCCESS 执行成功
//          其他值:获取信号量失败
//
// 调用要求:flash维持功能调用
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_RemoveProtect(HI_VOID);
//*****************************************************************************
// 函数名称: HI_FLASH_DfxGetProtectInfo
// 功能描述: 获取flash保护维测信息:magic, 状态寄存器1的值
//
// 参数说明:
//
// 返 回 值:无
//
// 调用要求:flash维持功能调用
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_FLASH_DfxGetProtectInfo(HI_BOOL *pbSupportProtect,HI_U8 *pucStatusVal);


#endif

typedef HI_VOID (*UPG_FAILED_IGNORE_PROCESS_HOOK)(HI_VOID);
HI_VOID HI_Enable_Upgrade_Failed_Process(UPG_FAILED_IGNORE_PROCESS_HOOK pfunc);

HI_END_HEADER

#endif // __HI_DRIVER_H__


