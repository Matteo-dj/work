
#include <hi_types.h>
#include <hi_platform.h>
#include <hi_mdm_types.h>
#include <hi_mdm_spi.h>
#include <hi_mdm_time.h>
#include <hi_mdm.h>
#include <hi_stdlib.h>
#include <hi_mdm_clock.h>

#define HI_DEBUG_SPI_Print(fmt, ...)
#define HI_SPI_UNUSE_DATA 0xFFFF//半双工模式下无效数据对应电平
#define HI_SPI_SLAVE_DEVICE_COUNT 1//支持的SPI逻辑设备个数


#define REG_SPI0_BASE_ADDR 0x101E5000//SPI控制器基地址
#define REG_SPI1_BASE_ADDR 0x101EB000//SPI控制器基地址
#define REG_SPI_CR0 0x0//控制寄存器0偏移地址
#define REG_SPI_CR1 0x4//控制寄存器1偏移地址
#define REG_SPI_DR  0x8//发送(接收)数据寄存器偏移地址
#define REG_SPI_SR  0xc//状态寄存器偏移地址
#define REG_SPI_CPSR 0x10//时钟分频寄存器偏移地址
#define REG_SPI_IMSC 0x14//中断屏蔽寄存器偏移地址
#define REG_SPI_RIS 0x18//原始中断状态寄存器偏移地址
#define REG_SPI_MIS 0x1c//屏蔽后中断状态寄存器偏移地址
#define REG_SPI_CR 0x20//中断清楚寄存器偏移地址
#define REG_SPI_TXFIFOCR 0x28//发送FIFO控制寄存器偏移地址
#define REG_SPI_RXFIFOCR 0x2c//接收FIFO控制寄存器偏移地址

#define MASK_SPI_SR_TFE    (1<<0)//TX FIFO是否已空 0:未空 1:已空
#define MASK_SPI_SR_TNF    (1<<1)//TX FIFO是否未满 0:已满 1:未满
#define MASK_SPI_SR_RNE    (1<<2)//RX FIFO未空标志 0:已空 1:未空
#define MASK_SPI_SR_RFF    (1<<3)//RX FIFO已满标志 0:未满 1:已满
#define MASK_SPI_SR_BSY    (1<<4)//SPI忙标志 0:空闲 1:忙

#define SPI_CR0_ST_BIT_DSS  0//起始bit:数据位宽
#define SPI_CR0_ST_BIT_FRF  4//起始bit:帧格式
#define SPI_CR0_ST_BIT_SPO  6//起始bit:极性
#define SPI_CR0_ST_BIT_SPH  7//起始bit:相位
#define SPI_CR0_ST_BIT_SCR  8//起始bit:串行时钟率

#define SPI_CR0_BIT_WIDTH_DSS 4//bit宽:数据位宽
#define SPI_CR0_BIT_WIDTH_FRF 2//bit宽:帧格式
#define SPI_CR0_BIT_WIDTH_SPO 1//bit宽:极性
#define SPI_CR0_BIT_WIDTH_SPH 1//bit宽:相位
#define SPI_CR0_BIT_WIDTH_SCR 8//bit宽:串行时钟率


#define SPI_CR1_ST_BIT_LBM 0//起始bit:回环模式
#define SPI_CR1_ST_BIT_SSE 1//起始bit:SPI使能
#define SPI_CR1_ST_BIT_MS 2//起始bit:MASTER SLAVE
#define SPI_CR1_ST_BIT_BIGEND 4//起始bit: 大小端
#define SPI_CR1_ST_BIT_WAITVAL 8//起始bit: microwire写和读之间等待拍数
#define SPI_CR1_ST_BIT_WAITEN 15//起始bit:microwire写和读之间等待拍数使能

#define SPI_CR1_BIT_WIDTH_LBM 1//bit宽:回环模式
#define SPI_CR1_BIT_WIDTH_SSE 1//bit宽:SPI使能
#define SPI_CR1_BIT_WIDTH_MS 1//bit宽:MASTER SLAVE
#define SPI_CR1_BIT_WIDTH_BIGEND 1//bit宽: 大小端
#define SPI_CR1_BIT_WIDTH_WAITVAL 7//bit宽:microwire写和读之间等待拍数
#define SPI_CR1_BIT_WIDTH_WAITEN 1//bit宽:microwire写和读之间等待拍数使能

#define BIT_MASK(bit_st,bit_width) (((1<<(bit_width))-1)<<(bit_st))
#define DIV_BAUD 2//默认时钟分频值

#define S_TO_US(x) ((x)*1000000)

//CR0支持用户配置的BIT
#define HI_SPI_SUPPORT_USR_CFG_BIT_CR0 ((BIT_MASK(SPI_CR0_ST_BIT_DSS,SPI_CR0_BIT_WIDTH_DSS))\
    |(BIT_MASK(SPI_CR0_ST_BIT_FRF,SPI_CR0_BIT_WIDTH_FRF))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SPO,SPI_CR0_BIT_WIDTH_SPO))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SPH,SPI_CR0_BIT_WIDTH_SPH))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SCR,SPI_CR0_BIT_WIDTH_SCR)))

//CR1支持用户配置的BIT
#define HI_SPI_SUPPORT_USR_CFG_BIT_CR1 ((BIT_MASK(SPI_CR1_ST_BIT_BIGEND,SPI_CR1_BIT_WIDTH_BIGEND))\
    |(BIT_MASK(SPI_CR1_ST_BIT_WAITVAL,SPI_CR1_BIT_WIDTH_WAITVAL))\
    |(BIT_MASK(SPI_CR1_ST_BIT_WAITEN,SPI_CR1_BIT_WIDTH_WAITEN)))

//CR0支持内部配置的BIT
#define HI_SPI_SUPPORT_INNER_BIT_CFG0 0

//CR1支持内部配置的BIT
#define HI_SPI_SUPPORT_INNER_BIT_CFG1 ((BIT_MASK(SPI_CR1_ST_BIT_LBM,SPI_CR1_BIT_WIDTH_LBM))\
    |(BIT_MASK(SPI_CR1_ST_BIT_MS,SPI_CR1_BIT_WIDTH_MS)))

//CR0内部配置默认值
#define HI_SPI_INNER_DEFAULT_VAL_CFG0 0

//CR1内部配置默认值
#define HI_SPI_INNER_DEFAULT_VAL_CFG1 ((HI_SPI_CFG_LBM_DISABLE<<SPI_CR1_ST_BIT_LBM)|(HI_SPI_CFG_ROLE_MASTER<<SPI_CR1_ST_BIT_MS))


//数组下标与FD转换关系
#define HI_SPI_FD_TO_ARRY_ID(h) (h-100)
#define HI_SPI_ARRY_ID_TO_FD(a_id) (a_id+100)



#define HI_SPI_WRITE_REG(reg_base, reg_off,value)\
    HI_REG_WRITE32((reg_base +(reg_off)), value) 
    
#define HI_SPI_READ_REG(reg_base, reg_off)\
    HI_REG_READ_VAL32((reg_base +(reg_off))) 
typedef HI_VOID (*spi_trans_data_func)(HI_U32 reg_base, HI_VOID * pData,HI_U32 offset);

#define SPI_FIFO_LEN 256//物理收发FIFO长度

#define HI_SPI_USE_FIFO_LEN 64//使用收发FIFO长度

#if(HI_SPI_USE_FIFO_LEN>SPI_FIFO_LEN)
#err "HI_SPI_USE_FIFO_LEN>SPI_FIFO_LEN"
#endif

typedef struct
{
    HI_PVOID pSendData;
    HI_PVOID pRecvData;
    HI_U32 dataLen;
    HI_U32 usrData;
}HI_SPI_TRANSTMIT_DATA_S;

typedef struct
{
    HI_U16 cr0;
    HI_U16 cr1;
    
    HI_U32 baudRate;
    spi_usr_func prepareFunc;
    spi_usr_func restoreFunc;
}HI_SPI_DEVICE_USR_CFG_IN_S;//等价结构HI_SSI_DEVICE_USR_CFG_S

typedef struct
{
    spi_usr_func prepareFunc;//通信开始前，用户回调函数
    spi_usr_func restoreFunc;//通信结束后, 用户回调函数
    HI_U16 cr0;//配置寄存器0配置值
    HI_U16 cr1;//配置寄存器1配置值
    HI_U32 timeOutPerFram;//单帧发送超时时间(us)
    HI_BOOL isUsed;//该设备描述符是否被open了。
    HI_U8 ucSpiId;//SPI编号,即当前从设备使用的是哪一个硬件SPI（V200有两个SPI控制器）
    HI_U8 reserver[2];
    HI_U32 ulRegBase;//spi基地址  
}HI_SPI_DEVICE_PRV_CFG_S;

typedef struct
{
    HI_U16 lastId;//上次访问的设备，即硬件中当前使用的配置
    HI_BOOL isInit;//模块初始化标志
    HI_U8 reserver1;
    HI_U32 regBase;//SPI寄存器基地址
}HI_SPI_INFOR_S;

typedef struct
{
    HI_SPI_INFOR_S spiInfor[HI_SPI_ID_MAX];//SPI信息
    HI_SPI_DEVICE_PRV_CFG_S devCfg[HI_SPI_SLAVE_DEVICE_COUNT];//代表各个从设备对应的master配置
}HI_SPI_CTRL_S;//SSI模块全局控制变量
HI_SPI_CTRL_S g_spi_ctrl;


//*****************************************************************************
// 函数名称: spi_xx_data_xbit
// 功能描述: 发送接收一桢数据
//
// 参数说明:
//      pData[in]:数据buf指针
//      offset[in]:指针偏移
//
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_VOID spi_send_data_16bit(HI_U32 reg_base, HI_U16 * pData,HI_U32 offset)
{
    HI_SPI_WRITE_REG(reg_base, REG_SPI_DR, *(pData+offset));
    HI_DEBUG_SPI_Print("send16 0x%x\r\n",*(pData+offset));
}

HI_PRV HI_VOID spi_send_data_8bit(HI_U32 reg_base, HI_U8 * pData,HI_U32 offset)
{
    HI_SPI_WRITE_REG(reg_base, REG_SPI_DR, *(pData+offset));
    HI_DEBUG_SPI_Print("send8 0x%x\r\n",*(pData+offset));
}

HI_PRV HI_VOID spi_rcv_data_16bit(HI_U32 reg_base, HI_U16 * pData,HI_U32 offset)
{
    *(pData+offset) =  (HI_U16)HI_SPI_READ_REG(reg_base, REG_SPI_DR);
    HI_DEBUG_SPI_Print("rcv16 0x%x\r\n",*(pData+offset));
}

HI_PRV HI_VOID spi_rcv_data_8bit(HI_U32 reg_base, HI_U8 * pData,HI_U32 offset)
{
    *(pData+offset) =  (HI_U8)HI_SPI_READ_REG(reg_base, REG_SPI_DR);
    HI_DEBUG_SPI_Print("rcv8 0x%x\r\n",*(pData+offset));
}

//*****************************************************************************
// 函数名称: spi_xxable_cs
// 功能描述: 使能去使能CS
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_VOID spi_disable(HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg)
{
    HI_U32 reg_val = 0;

    reg_val = (HI_U32)HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1);
    reg_val &= ~(1<<SPI_CR1_ST_BIT_SSE);
    HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1, reg_val);
}

HI_PRV HI_VOID spi_enable(HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg)
{
    HI_U32 reg_val = 0;
    
    reg_val = HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1);
    reg_val |= (1<<SPI_CR1_ST_BIT_SSE);
    HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1, reg_val); 
}

//*****************************************************************************
// 函数名称: spi_wait_hw_ok
// 功能描述: spi设备通信前后等待spi设备空闲|队列中数据处理完毕
//
// 参数说明:
//      ulTimeOut[in]:超时等待时间
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_U32 spi_wait_hw_ok(HI_IN HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg, HI_IN HI_U32 ulTimeOut)
{
    //有效数据
    HI_U32 use_time = 0;
    HI_U32 reg_sr_val = 0;
    HI_U16 tmp_data = 0;
    for(;;)
    {
        if(use_time>ulTimeOut)
        {
            return HI_ERR_TIMEOUT;
        }
        
        reg_sr_val = HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_SR);
        if((reg_sr_val&MASK_SPI_SR_BSY)!=0)//BUSY
        {
            HI_MDM_Udelay(1);
            use_time++;
            continue;
        }
        
        if((reg_sr_val&MASK_SPI_SR_TFE)==0)//发送队列不为空
        {
            HI_MDM_Udelay(1);
            use_time++;
            continue;
        }

        if((reg_sr_val&MASK_SPI_SR_RNE)!=0)//接收队列未空
        {
            HI_MDM_Udelay(1);
            use_time++;
            spi_rcv_data_16bit(pDevPrvCfg->ulRegBase, (HI_U16 *)&tmp_data,0);
            continue;
        }        
        break;
    }
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: spi_trans_data_only
// 功能描述: spi发送(接收)数据流程(不含预配置流程)
//
// 参数说明:
//      send_func[in]:发送一桢回调函数
//      rcv_func[in]:接收一桢回调函数
//      pData[in]:发送(接收)数据内容和长度
//      ulTimeOutUs[in]:发送(接收)超时时间
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_U32 spi_trans_data_only(HI_IN HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg, HI_IN spi_trans_data_func send_func,HI_IN spi_trans_data_func rcv_func,
                                      HI_IN HI_SPI_TRANSTMIT_DATA_S *pData,HI_IN HI_U32 ulTimeOutUs)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 sent_len = 0;
    HI_U32 send_len_this_time = 0;
    HI_U32 i = 0;
    HI_U16 tempData = 0;
    HI_U32 cur_time = 0;
    spi_enable(pDevPrvCfg);

    //等待器件就绪
    ret = spi_wait_hw_ok(pDevPrvCfg, ulTimeOutUs);
    if(ret!=HI_ERR_SUCCESS)
    {
        ret = HI_ERR_TIMEOUT;
        goto restore;
    }

    while(pData->dataLen>sent_len)
    {
         send_len_this_time =  ((pData->dataLen-sent_len)>=HI_SPI_USE_FIFO_LEN)?HI_SPI_USE_FIFO_LEN:(pData->dataLen-sent_len);
         for(i=0;i<send_len_this_time;)
         {
            if(pData->pSendData!=HI_NULL)
            {
                send_func(pDevPrvCfg->ulRegBase, pData->pSendData, sent_len+i);
            }
            else
            {
                tempData = HI_SPI_UNUSE_DATA;
                send_func(pDevPrvCfg->ulRegBase, &tempData,0);
            }
            i++;
         }

         cur_time = 0;
         for(i=0;i<send_len_this_time;)
         {
            if(HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_SR)&(MASK_SPI_SR_RNE))
            {
                if(pData->pRecvData!=HI_NULL)
                {
                    rcv_func(pDevPrvCfg->ulRegBase, pData->pRecvData, sent_len+i);//sent_len+i一定会受到send_len_this_time这个多数据才推出循环
                }
                else
                {
                    rcv_func(pDevPrvCfg->ulRegBase, &tempData,0);
                }
                i++;
            }
            else
            {
                if(cur_time>ulTimeOutUs)
                {
                    ret = HI_ERR_TIMEOUT;
                    goto restore;
                }
                HI_MDM_Udelay(1);
                cur_time++;  
            } 
         }
         
         sent_len+=send_len_this_time;
    }

    //等待器件操作完成
    ret = spi_wait_hw_ok(pDevPrvCfg, ulTimeOutUs);
restore: 
    spi_disable(pDevPrvCfg);
    return ret;

}

//*****************************************************************************
// 函数名称: spi_modify_control_cfg
// 功能描述: 发送(接收)数据前将配置写到寄存器中
//
// 参数说明:
//      array_id[in]:设备下标
//      cfg0[in]:配置寄存器0值
//      cfg1[in]:配置寄存器1值
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_VOID spi_modify_control_cfg(HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg, HI_U32 array_id, HI_U16 cfg0,HI_U16 cfg1)
{
    //按照设备配置修改寄存器配置
    HI_U32 reg0 = 0,reg1 = 0;
    if(array_id != g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId)
    {
        g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId = (HI_U16)array_id;        //写配置到SPI中
        reg0 = HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR0);
        reg1 = HI_SPI_READ_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1);
        HI_DEBUG_SPI_Print("spi_modify_control_cfg1[r0=0x%x][r1=0x%x]\r\n",reg0,reg1);
        reg0&=(~(HI_SPI_SUPPORT_USR_CFG_BIT_CR0|HI_SPI_SUPPORT_INNER_BIT_CFG0));
        reg1&=(~(HI_SPI_SUPPORT_USR_CFG_BIT_CR1|HI_SPI_SUPPORT_INNER_BIT_CFG1));
        HI_DEBUG_SPI_Print("spi_modify_control_cfg2[r0=0x%x][r1=0x%x]\r\n",reg0,reg1);
        reg0|=(cfg0&(HI_SPI_SUPPORT_USR_CFG_BIT_CR0|HI_SPI_SUPPORT_INNER_BIT_CFG0));
        reg1|=(cfg1&(HI_SPI_SUPPORT_USR_CFG_BIT_CR1|HI_SPI_SUPPORT_INNER_BIT_CFG1));
        HI_DEBUG_SPI_Print("spi_modify_control_cfg3[r0=0x%x][r1=0x%x]\r\n",reg0,reg1);
        HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR0, reg0);
        HI_DEBUG_SPI_Print("spi_modify_control_cfg cr0 = 0x%x\r\n",cfg0&(HI_SPI_SUPPORT_USR_CFG_BIT_CR0|HI_SPI_SUPPORT_INNER_BIT_CFG0));
        HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CR1, reg1); 
        HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CPSR, DIV_BAUD);//二分频 
    }

}

//*****************************************************************************
// 函数名称: spi_modify_control_cfg
// 功能描述: 根据用户配置数据位宽获取发送(接收)一桢函数指针
//
// 参数说明:
//      pSendDataFunc[out]:发送一桢钩子函数
//      pRcvDataFunc[in]:接收一桢钩子函数
//      pDevPrvCfg[in]:用户配置
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_U32 spi_get_trans_data_func(HI_OUT spi_trans_data_func *pSendDataFunc,HI_OUT spi_trans_data_func * pRcvDataFunc,HI_IN HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg)
{
    //省略 pSendDataFunc pRcvDataFunc pDevPrvCfg非空判断
    if((pDevPrvCfg->cr0&BIT_MASK(SPI_CR0_ST_BIT_DSS, SPI_CR0_BIT_WIDTH_DSS))>(HI_SPI_CFG_DATA_WIDTH_E_8BIT<<SPI_CR0_ST_BIT_DSS))
    {
        *pSendDataFunc = (spi_trans_data_func)spi_send_data_16bit;
        *pRcvDataFunc = (spi_trans_data_func)spi_rcv_data_16bit;
    }
    else
    {
        *pSendDataFunc = (spi_trans_data_func)spi_send_data_8bit;
        *pRcvDataFunc = (spi_trans_data_func)spi_rcv_data_8bit;
    }
    return HI_ERR_SUCCESS;
} 

//*****************************************************************************
// 函数名称: spi_transmit_data_prv  
// 功能描述: SSI发送(接收)数据处理全流程
//
// 参数说明:
//      fd [in]:设备描述符
//      pData[in]:发送接收数据
//      ulTimeOutUs[in]:超时时间
// 返 回 值:
//
// 调用要求:
//
//*****************************************************************************
HI_PRV HI_U32 spi_transmit_data_prv(HI_IN HI_U32 fd ,HI_IN HI_SPI_TRANSTMIT_DATA_S *pData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg = HI_NULL;
    spi_trans_data_func send_func = HI_NULL;
    spi_trans_data_func rcv_func = HI_NULL;
    HI_U32 array_id = HI_SPI_FD_TO_ARRY_ID(fd);

    //入参判断
    if(array_id>=HI_SPI_SLAVE_DEVICE_COUNT)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    pDevPrvCfg = &g_spi_ctrl.devCfg[array_id];
    if(pDevPrvCfg->isUsed==HI_FALSE)
    {
        return HI_ERR_ACCESS_DENIED;
    }

    //获取发送和接收数据函数(16bit or 8bit)
    (HI_VOID)spi_get_trans_data_func(&send_func, &rcv_func,pDevPrvCfg);



    //配置SPI寄存器
    spi_modify_control_cfg(pDevPrvCfg, array_id, pDevPrvCfg->cr0,pDevPrvCfg->cr1);

    //调用用户准备函数
    if(pDevPrvCfg->prepareFunc != HI_NULL)
    {
        pDevPrvCfg->prepareFunc(pData->usrData);
    }
        
    ret = spi_trans_data_only(pDevPrvCfg, send_func,rcv_func, pData, pDevPrvCfg->timeOutPerFram*HI_SPI_USE_FIFO_LEN);//每次等待时间长度  

    //调用用户恢复函数
    if(pDevPrvCfg->restoreFunc!=HI_NULL)
    {
        pDevPrvCfg->restoreFunc(pData->usrData);
    }
    return ret;
    
}

//*****************************************************************************
// 函数名称: HI_MDM_SPI_SendData
// 功能描述: 发送数据,用于半双工器件
//
// 参数说明:
//      fd[in]:设备描述符
//      pSendData[in]:发送数据指针
//      ulDataLen[in]:发送数据长度
//      usrData[in]:用户回调函数入参
// 返 回 值:
//      HI_ERR_SUCCESS:成功
// 调用要求:
//
//*****************************************************************************
HI_U32 HI_MDM_SPI_SendData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData)
{
    HI_SPI_TRANSTMIT_DATA_S stTransData = {0};
    if(pSendData==HI_NULL||ulDataLen==0)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    stTransData.pSendData = pSendData;
    stTransData.dataLen = ulDataLen;
    stTransData.pRecvData = HI_NULL;
    stTransData.usrData = usrData;
    return spi_transmit_data_prv(fd, &stTransData);
}

//*****************************************************************************
// 函数名称: HI_MDM_SPI_RecvData
// 功能描述: 接收数据,用于半双工器件
//
// 参数说明:
//      fd[in]:设备描述符
//      pRcvData[in]:接收数据指针
//      ulDataLen[in]:接收数据长度
//      usrData[in]:用户回调函数入参
// 返 回 值:
//      HI_ERR_SUCCESS:成功
// 调用要求:
//
//*****************************************************************************
HI_U32 HI_MDM_SPI_RecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen,HI_IN HI_U32 usrData)
{
    HI_SPI_TRANSTMIT_DATA_S stTransData = {0};
    if(pRcvData==HI_NULL||ulDataLen==0)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    stTransData.pSendData = HI_NULL;
    stTransData.dataLen = ulDataLen;
    stTransData.pRecvData = pRcvData;
    stTransData.usrData = usrData;
    return spi_transmit_data_prv(fd, &stTransData);
}

//*****************************************************************************
// 函数名称: HI_MDM_SPI_SendRecvData
// 功能描述: 发送接收数据,用于全双工器件
//
// 参数说明:
//      fd[in]:设备描述符
//      pSendData[in]:发送数据指针
//      pRcvData[in]:接收数据指针
//      ulDataLen[in]:数据长度
//      usrData[in]:用户回调函数入参
// 返 回 值:
//      HI_ERR_SUCCESS:成功
// 调用要求:
//
//*****************************************************************************
HI_U32 HI_MDM_SPI_SendRecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData)
{
    HI_SPI_TRANSTMIT_DATA_S stTransData = {0};
    if(pSendData==HI_NULL||pRcvData==HI_NULL||ulDataLen==0)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    stTransData.pSendData = pSendData;
    stTransData.dataLen = ulDataLen;
    stTransData.pRecvData = pRcvData;
    stTransData.usrData = usrData;
    return spi_transmit_data_prv(fd, &stTransData);
}


HI_U32 HI_MDM_SPI_Config(HI_IN HI_U32 fd ,HI_IN HI_SPI_DEVICE_USR_CFG_S *pUsrArg)
{
    HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg = HI_NULL;
    HI_SPI_DEVICE_USR_CFG_IN_S* pUsrCfg = HI_NULL;
    HI_U32 array_id = HI_SPI_FD_TO_ARRY_ID(fd);
    HI_U32 real_in_clk = g_cfg_apb_clock/DIV_BAUD;//SPI控制器工作时钟
    HI_U32 spi_clk = 0;
    HI_U32 bit_per_fram = 0;//每帧传输需要多少个时钟周期
    HI_U32 time_per_fram = 0;//每帧传输需要多长时间(传输超时时间)
    HI_U32 reg_clk = 0;//写到寄存器中的CLK VAL
    
    if(HI_NULL == pUsrArg)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_INVALID_PARAMETER\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }
    pUsrCfg = (HI_SPI_DEVICE_USR_CFG_IN_S*)pUsrArg;
    spi_clk = pUsrCfg->baudRate;//SPI通信时钟频率
    //1.参数判断
    if(array_id>=HI_SPI_SLAVE_DEVICE_COUNT||spi_clk>real_in_clk||spi_clk*256<real_in_clk)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_INVALID_PARAMETER\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    
    //2.计算时钟率寄存器写入值;real_in_clk/spi_clk:计算的寄存器值;+(spi_clk-1)时钟速率应该向下对齐因此reg_clk需要向上对齐
    reg_clk = (real_in_clk+(spi_clk-1))/spi_clk;// 1~256

    //3.计算每帧传输超时时间
    //数据宽度为pUsrArg->dataWidth+1;再+1连续传输间隔(部分配置适用);micro wire帧格式增加一个帧传输时间(半双工)在增加写读转换间隔waitVal
    bit_per_fram = pUsrArg->dataWidth+1+1;
    if((pUsrArg->framMode==HI_SPI_CFG_FRAM_MODE_MICROWIRE)&&(pUsrArg->waitEn==HI_SPI_CFG_WAIT_EN_ENABLE))
    {
        bit_per_fram = bit_per_fram+pUsrArg->dataWidth+pUsrArg->waitVal;
    }
    //计算每帧传输超时时间 bit_per_fram*S_TO_US(1)/(spi_clk)为一桢传输超时时间;+spi_clk超时时间向上对齐;*5:实际配置的SPI速率小于下发值,因此增加补偿值。
    time_per_fram = ((bit_per_fram*S_TO_US(1)+spi_clk)/spi_clk)*5;//
    
    pDevPrvCfg = &g_spi_ctrl.devCfg[array_id];
    if(pDevPrvCfg->isUsed==HI_FALSE)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_ACCESS_DENIED\r\n");
        return HI_ERR_ACCESS_DENIED;
    }

    HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config SUC][set baudRate=%u][real baudRate=%u][us_per_fram=%u][reg_clk=%u][dataWidth=%u]\r\n"
        ,pUsrCfg->baudRate,real_in_clk/reg_clk,time_per_fram,reg_clk,pUsrArg->dataWidth+1);

    //修改参数中的波特率部分
    reg_clk = reg_clk-1;
    pUsrCfg->cr0 &= (~(BIT_MASK(SPI_CR0_ST_BIT_SCR, SPI_CR0_BIT_WIDTH_SCR)));
    pUsrCfg->cr0 |= ((reg_clk<<SPI_CR0_ST_BIT_SCR)&(BIT_MASK(SPI_CR0_ST_BIT_SCR, SPI_CR0_BIT_WIDTH_SCR)));

    pUsrCfg->cr0 = (pUsrCfg->cr0&HI_SPI_SUPPORT_USR_CFG_BIT_CR0)|(HI_SPI_INNER_DEFAULT_VAL_CFG0&HI_SPI_SUPPORT_INNER_BIT_CFG0);
    pUsrCfg->cr1 = (pUsrCfg->cr1&HI_SPI_SUPPORT_USR_CFG_BIT_CR1)|(HI_SPI_INNER_DEFAULT_VAL_CFG1&HI_SPI_SUPPORT_INNER_BIT_CFG1);   

    //用户配置赋值给内部配置全局变量
    pDevPrvCfg->cr0 = pUsrCfg->cr0;
    pDevPrvCfg->cr1 = pUsrCfg->cr1;
    pDevPrvCfg->prepareFunc = pUsrCfg->prepareFunc;
    pDevPrvCfg->restoreFunc = pUsrCfg->restoreFunc;
    pDevPrvCfg->timeOutPerFram  = time_per_fram;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_SPI_DevOpen(HI_IN HI_SPI_ID_E enSpiId, HI_OUT HI_U32 *pFd)
{
    HI_U32 i = 0;
    
    if(pFd==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if(enSpiId >= HI_SPI_ID_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    if(!g_spi_ctrl.spiInfor[enSpiId].isInit)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    for(i=0;i<HI_SPI_SLAVE_DEVICE_COUNT;i++)
    {
        if(g_spi_ctrl.devCfg[i].isUsed == HI_FALSE)
        {
            g_spi_ctrl.devCfg[i].isUsed = HI_TRUE;
            g_spi_ctrl.devCfg[i].ulRegBase = g_spi_ctrl.spiInfor[enSpiId].regBase;
            g_spi_ctrl.devCfg[i].ucSpiId = enSpiId;
            *pFd = HI_SPI_ARRY_ID_TO_FD(i);
            return HI_ERR_SUCCESS;
        }
    }
    
    return HI_ERR_NOT_ENOUGH_FD; 
}

HI_U32 HI_MDM_SPI_DevClose(HI_IN HI_U32 fd)
{
    HI_U32 array_id = HI_SPI_FD_TO_ARRY_ID(fd);
    HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg = HI_NULL;

    if(array_id >= HI_SPI_SLAVE_DEVICE_COUNT)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    pDevPrvCfg = &g_spi_ctrl.devCfg[array_id];
    if(pDevPrvCfg->isUsed==HI_FALSE)
    {
        return HI_ERR_ACCESS_DENIED;
    }
    (hi_void)memset_s(pDevPrvCfg, sizeof(HI_SPI_DEVICE_PRV_CFG_S), 0x0, sizeof(HI_SPI_DEVICE_PRV_CFG_S));
    pDevPrvCfg->isUsed = HI_FALSE;
    if(g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId==array_id)
    {
        g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId = 0xFFFF;   
    }
    return HI_ERR_SUCCESS;
    
}

HI_U32 HI_MDM_SPI_Init(HI_IN HI_SPI_ID_E enSpiId)
{
    //初始化SPI硬件相关:如IO复用
    if(enSpiId >= HI_SPI_ID_MAX)
    {
        return HI_ERR_INVALID_ID;
    }
    if(g_spi_ctrl.spiInfor[enSpiId].isInit==HI_FALSE)
    {
        //配置分频寄存器
        //HI_SSI_WRITE_REG(REG_SSI_CPSR, DIV_BAUD);//二分频
        
        g_spi_ctrl.spiInfor[enSpiId].lastId= 0xFFFF;
        g_spi_ctrl.spiInfor[enSpiId].isInit = HI_TRUE;
        if(HI_SPI_ID_0 == enSpiId)
        {
            HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SSP);
            g_spi_ctrl.spiInfor[enSpiId].regBase = REG_SPI0_BASE_ADDR;
        }
        else if(HI_SPI_ID_1 == enSpiId)
        {
        
            HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_PHY_WRAP_CLK_EN_REG,HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_SSP2);
            g_spi_ctrl.spiInfor[enSpiId].regBase = REG_SPI1_BASE_ADDR;
        }  
    }
    return HI_ERR_SUCCESS;
}

