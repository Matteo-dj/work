
#include <hi_types.h>
#include <hi_platform.h>
#include <hi_mdm_types.h>
#include <hi_mdm_spi.h>
#include <hi_mdm_time.h>
#include <hi_mdm.h>
#include <hi_stdlib.h>
#include <hi_mdm_clock.h>

#define HI_DEBUG_SPI_Print(fmt, ...)
#define HI_SPI_UNUSE_DATA 0xFFFF//��˫��ģʽ����Ч���ݶ�Ӧ��ƽ
#define HI_SPI_SLAVE_DEVICE_COUNT 1//֧�ֵ�SPI�߼��豸����


#define REG_SPI0_BASE_ADDR 0x101E5000//SPI����������ַ
#define REG_SPI1_BASE_ADDR 0x101EB000//SPI����������ַ
#define REG_SPI_CR0 0x0//���ƼĴ���0ƫ�Ƶ�ַ
#define REG_SPI_CR1 0x4//���ƼĴ���1ƫ�Ƶ�ַ
#define REG_SPI_DR  0x8//����(����)���ݼĴ���ƫ�Ƶ�ַ
#define REG_SPI_SR  0xc//״̬�Ĵ���ƫ�Ƶ�ַ
#define REG_SPI_CPSR 0x10//ʱ�ӷ�Ƶ�Ĵ���ƫ�Ƶ�ַ
#define REG_SPI_IMSC 0x14//�ж����μĴ���ƫ�Ƶ�ַ
#define REG_SPI_RIS 0x18//ԭʼ�ж�״̬�Ĵ���ƫ�Ƶ�ַ
#define REG_SPI_MIS 0x1c//���κ��ж�״̬�Ĵ���ƫ�Ƶ�ַ
#define REG_SPI_CR 0x20//�ж�����Ĵ���ƫ�Ƶ�ַ
#define REG_SPI_TXFIFOCR 0x28//����FIFO���ƼĴ���ƫ�Ƶ�ַ
#define REG_SPI_RXFIFOCR 0x2c//����FIFO���ƼĴ���ƫ�Ƶ�ַ

#define MASK_SPI_SR_TFE    (1<<0)//TX FIFO�Ƿ��ѿ� 0:δ�� 1:�ѿ�
#define MASK_SPI_SR_TNF    (1<<1)//TX FIFO�Ƿ�δ�� 0:���� 1:δ��
#define MASK_SPI_SR_RNE    (1<<2)//RX FIFOδ�ձ�־ 0:�ѿ� 1:δ��
#define MASK_SPI_SR_RFF    (1<<3)//RX FIFO������־ 0:δ�� 1:����
#define MASK_SPI_SR_BSY    (1<<4)//SPIæ��־ 0:���� 1:æ

#define SPI_CR0_ST_BIT_DSS  0//��ʼbit:����λ��
#define SPI_CR0_ST_BIT_FRF  4//��ʼbit:֡��ʽ
#define SPI_CR0_ST_BIT_SPO  6//��ʼbit:����
#define SPI_CR0_ST_BIT_SPH  7//��ʼbit:��λ
#define SPI_CR0_ST_BIT_SCR  8//��ʼbit:����ʱ����

#define SPI_CR0_BIT_WIDTH_DSS 4//bit��:����λ��
#define SPI_CR0_BIT_WIDTH_FRF 2//bit��:֡��ʽ
#define SPI_CR0_BIT_WIDTH_SPO 1//bit��:����
#define SPI_CR0_BIT_WIDTH_SPH 1//bit��:��λ
#define SPI_CR0_BIT_WIDTH_SCR 8//bit��:����ʱ����


#define SPI_CR1_ST_BIT_LBM 0//��ʼbit:�ػ�ģʽ
#define SPI_CR1_ST_BIT_SSE 1//��ʼbit:SPIʹ��
#define SPI_CR1_ST_BIT_MS 2//��ʼbit:MASTER SLAVE
#define SPI_CR1_ST_BIT_BIGEND 4//��ʼbit: ��С��
#define SPI_CR1_ST_BIT_WAITVAL 8//��ʼbit: microwireд�Ͷ�֮��ȴ�����
#define SPI_CR1_ST_BIT_WAITEN 15//��ʼbit:microwireд�Ͷ�֮��ȴ�����ʹ��

#define SPI_CR1_BIT_WIDTH_LBM 1//bit��:�ػ�ģʽ
#define SPI_CR1_BIT_WIDTH_SSE 1//bit��:SPIʹ��
#define SPI_CR1_BIT_WIDTH_MS 1//bit��:MASTER SLAVE
#define SPI_CR1_BIT_WIDTH_BIGEND 1//bit��: ��С��
#define SPI_CR1_BIT_WIDTH_WAITVAL 7//bit��:microwireд�Ͷ�֮��ȴ�����
#define SPI_CR1_BIT_WIDTH_WAITEN 1//bit��:microwireд�Ͷ�֮��ȴ�����ʹ��

#define BIT_MASK(bit_st,bit_width) (((1<<(bit_width))-1)<<(bit_st))
#define DIV_BAUD 2//Ĭ��ʱ�ӷ�Ƶֵ

#define S_TO_US(x) ((x)*1000000)

//CR0֧���û����õ�BIT
#define HI_SPI_SUPPORT_USR_CFG_BIT_CR0 ((BIT_MASK(SPI_CR0_ST_BIT_DSS,SPI_CR0_BIT_WIDTH_DSS))\
    |(BIT_MASK(SPI_CR0_ST_BIT_FRF,SPI_CR0_BIT_WIDTH_FRF))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SPO,SPI_CR0_BIT_WIDTH_SPO))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SPH,SPI_CR0_BIT_WIDTH_SPH))\
    |(BIT_MASK(SPI_CR0_ST_BIT_SCR,SPI_CR0_BIT_WIDTH_SCR)))

//CR1֧���û����õ�BIT
#define HI_SPI_SUPPORT_USR_CFG_BIT_CR1 ((BIT_MASK(SPI_CR1_ST_BIT_BIGEND,SPI_CR1_BIT_WIDTH_BIGEND))\
    |(BIT_MASK(SPI_CR1_ST_BIT_WAITVAL,SPI_CR1_BIT_WIDTH_WAITVAL))\
    |(BIT_MASK(SPI_CR1_ST_BIT_WAITEN,SPI_CR1_BIT_WIDTH_WAITEN)))

//CR0֧���ڲ����õ�BIT
#define HI_SPI_SUPPORT_INNER_BIT_CFG0 0

//CR1֧���ڲ����õ�BIT
#define HI_SPI_SUPPORT_INNER_BIT_CFG1 ((BIT_MASK(SPI_CR1_ST_BIT_LBM,SPI_CR1_BIT_WIDTH_LBM))\
    |(BIT_MASK(SPI_CR1_ST_BIT_MS,SPI_CR1_BIT_WIDTH_MS)))

//CR0�ڲ�����Ĭ��ֵ
#define HI_SPI_INNER_DEFAULT_VAL_CFG0 0

//CR1�ڲ�����Ĭ��ֵ
#define HI_SPI_INNER_DEFAULT_VAL_CFG1 ((HI_SPI_CFG_LBM_DISABLE<<SPI_CR1_ST_BIT_LBM)|(HI_SPI_CFG_ROLE_MASTER<<SPI_CR1_ST_BIT_MS))


//�����±���FDת����ϵ
#define HI_SPI_FD_TO_ARRY_ID(h) (h-100)
#define HI_SPI_ARRY_ID_TO_FD(a_id) (a_id+100)



#define HI_SPI_WRITE_REG(reg_base, reg_off,value)\
    HI_REG_WRITE32((reg_base +(reg_off)), value) 
    
#define HI_SPI_READ_REG(reg_base, reg_off)\
    HI_REG_READ_VAL32((reg_base +(reg_off))) 
typedef HI_VOID (*spi_trans_data_func)(HI_U32 reg_base, HI_VOID * pData,HI_U32 offset);

#define SPI_FIFO_LEN 256//�����շ�FIFO����

#define HI_SPI_USE_FIFO_LEN 64//ʹ���շ�FIFO����

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
}HI_SPI_DEVICE_USR_CFG_IN_S;//�ȼ۽ṹHI_SSI_DEVICE_USR_CFG_S

typedef struct
{
    spi_usr_func prepareFunc;//ͨ�ſ�ʼǰ���û��ص�����
    spi_usr_func restoreFunc;//ͨ�Ž�����, �û��ص�����
    HI_U16 cr0;//���üĴ���0����ֵ
    HI_U16 cr1;//���üĴ���1����ֵ
    HI_U32 timeOutPerFram;//��֡���ͳ�ʱʱ��(us)
    HI_BOOL isUsed;//���豸�������Ƿ�open�ˡ�
    HI_U8 ucSpiId;//SPI���,����ǰ���豸ʹ�õ�����һ��Ӳ��SPI��V200������SPI��������
    HI_U8 reserver[2];
    HI_U32 ulRegBase;//spi����ַ  
}HI_SPI_DEVICE_PRV_CFG_S;

typedef struct
{
    HI_U16 lastId;//�ϴη��ʵ��豸����Ӳ���е�ǰʹ�õ�����
    HI_BOOL isInit;//ģ���ʼ����־
    HI_U8 reserver1;
    HI_U32 regBase;//SPI�Ĵ�������ַ
}HI_SPI_INFOR_S;

typedef struct
{
    HI_SPI_INFOR_S spiInfor[HI_SPI_ID_MAX];//SPI��Ϣ
    HI_SPI_DEVICE_PRV_CFG_S devCfg[HI_SPI_SLAVE_DEVICE_COUNT];//����������豸��Ӧ��master����
}HI_SPI_CTRL_S;//SSIģ��ȫ�ֿ��Ʊ���
HI_SPI_CTRL_S g_spi_ctrl;


//*****************************************************************************
// ��������: spi_xx_data_xbit
// ��������: ���ͽ���һ������
//
// ����˵��:
//      pData[in]:����bufָ��
//      offset[in]:ָ��ƫ��
//
// �� �� ֵ:
//
// ����Ҫ��:
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
// ��������: spi_xxable_cs
// ��������: ʹ��ȥʹ��CS
//
// ����˵��:
//
// �� �� ֵ:
//
// ����Ҫ��:
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
// ��������: spi_wait_hw_ok
// ��������: spi�豸ͨ��ǰ��ȴ�spi�豸����|���������ݴ������
//
// ����˵��:
//      ulTimeOut[in]:��ʱ�ȴ�ʱ��
// �� �� ֵ:
//
// ����Ҫ��:
//
//*****************************************************************************
HI_PRV HI_U32 spi_wait_hw_ok(HI_IN HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg, HI_IN HI_U32 ulTimeOut)
{
    //��Ч����
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
        
        if((reg_sr_val&MASK_SPI_SR_TFE)==0)//���Ͷ��в�Ϊ��
        {
            HI_MDM_Udelay(1);
            use_time++;
            continue;
        }

        if((reg_sr_val&MASK_SPI_SR_RNE)!=0)//���ն���δ��
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
// ��������: spi_trans_data_only
// ��������: spi����(����)��������(����Ԥ��������)
//
// ����˵��:
//      send_func[in]:����һ��ص�����
//      rcv_func[in]:����һ��ص�����
//      pData[in]:����(����)�������ݺͳ���
//      ulTimeOutUs[in]:����(����)��ʱʱ��
// �� �� ֵ:
//
// ����Ҫ��:
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

    //�ȴ���������
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
                    rcv_func(pDevPrvCfg->ulRegBase, pData->pRecvData, sent_len+i);//sent_len+iһ�����ܵ�send_len_this_time��������ݲ��Ƴ�ѭ��
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

    //�ȴ������������
    ret = spi_wait_hw_ok(pDevPrvCfg, ulTimeOutUs);
restore: 
    spi_disable(pDevPrvCfg);
    return ret;

}

//*****************************************************************************
// ��������: spi_modify_control_cfg
// ��������: ����(����)����ǰ������д���Ĵ�����
//
// ����˵��:
//      array_id[in]:�豸�±�
//      cfg0[in]:���üĴ���0ֵ
//      cfg1[in]:���üĴ���1ֵ
// �� �� ֵ:
//
// ����Ҫ��:
//
//*****************************************************************************
HI_PRV HI_VOID spi_modify_control_cfg(HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg, HI_U32 array_id, HI_U16 cfg0,HI_U16 cfg1)
{
    //�����豸�����޸ļĴ�������
    HI_U32 reg0 = 0,reg1 = 0;
    if(array_id != g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId)
    {
        g_spi_ctrl.spiInfor[pDevPrvCfg->ucSpiId].lastId = (HI_U16)array_id;        //д���õ�SPI��
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
        HI_SPI_WRITE_REG(pDevPrvCfg->ulRegBase, REG_SPI_CPSR, DIV_BAUD);//����Ƶ 
    }

}

//*****************************************************************************
// ��������: spi_modify_control_cfg
// ��������: �����û���������λ���ȡ����(����)һ�庯��ָ��
//
// ����˵��:
//      pSendDataFunc[out]:����һ�平�Ӻ���
//      pRcvDataFunc[in]:����һ�平�Ӻ���
//      pDevPrvCfg[in]:�û�����
// �� �� ֵ:
//
// ����Ҫ��:
//
//*****************************************************************************
HI_PRV HI_U32 spi_get_trans_data_func(HI_OUT spi_trans_data_func *pSendDataFunc,HI_OUT spi_trans_data_func * pRcvDataFunc,HI_IN HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg)
{
    //ʡ�� pSendDataFunc pRcvDataFunc pDevPrvCfg�ǿ��ж�
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
// ��������: spi_transmit_data_prv  
// ��������: SSI����(����)���ݴ���ȫ����
//
// ����˵��:
//      fd [in]:�豸������
//      pData[in]:���ͽ�������
//      ulTimeOutUs[in]:��ʱʱ��
// �� �� ֵ:
//
// ����Ҫ��:
//
//*****************************************************************************
HI_PRV HI_U32 spi_transmit_data_prv(HI_IN HI_U32 fd ,HI_IN HI_SPI_TRANSTMIT_DATA_S *pData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SPI_DEVICE_PRV_CFG_S *pDevPrvCfg = HI_NULL;
    spi_trans_data_func send_func = HI_NULL;
    spi_trans_data_func rcv_func = HI_NULL;
    HI_U32 array_id = HI_SPI_FD_TO_ARRY_ID(fd);

    //����ж�
    if(array_id>=HI_SPI_SLAVE_DEVICE_COUNT)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    pDevPrvCfg = &g_spi_ctrl.devCfg[array_id];
    if(pDevPrvCfg->isUsed==HI_FALSE)
    {
        return HI_ERR_ACCESS_DENIED;
    }

    //��ȡ���ͺͽ������ݺ���(16bit or 8bit)
    (HI_VOID)spi_get_trans_data_func(&send_func, &rcv_func,pDevPrvCfg);



    //����SPI�Ĵ���
    spi_modify_control_cfg(pDevPrvCfg, array_id, pDevPrvCfg->cr0,pDevPrvCfg->cr1);

    //�����û�׼������
    if(pDevPrvCfg->prepareFunc != HI_NULL)
    {
        pDevPrvCfg->prepareFunc(pData->usrData);
    }
        
    ret = spi_trans_data_only(pDevPrvCfg, send_func,rcv_func, pData, pDevPrvCfg->timeOutPerFram*HI_SPI_USE_FIFO_LEN);//ÿ�εȴ�ʱ�䳤��  

    //�����û��ָ�����
    if(pDevPrvCfg->restoreFunc!=HI_NULL)
    {
        pDevPrvCfg->restoreFunc(pData->usrData);
    }
    return ret;
    
}

//*****************************************************************************
// ��������: HI_MDM_SPI_SendData
// ��������: ��������,���ڰ�˫������
//
// ����˵��:
//      fd[in]:�豸������
//      pSendData[in]:��������ָ��
//      ulDataLen[in]:�������ݳ���
//      usrData[in]:�û��ص��������
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
// ����Ҫ��:
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
// ��������: HI_MDM_SPI_RecvData
// ��������: ��������,���ڰ�˫������
//
// ����˵��:
//      fd[in]:�豸������
//      pRcvData[in]:��������ָ��
//      ulDataLen[in]:�������ݳ���
//      usrData[in]:�û��ص��������
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
// ����Ҫ��:
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
// ��������: HI_MDM_SPI_SendRecvData
// ��������: ���ͽ�������,����ȫ˫������
//
// ����˵��:
//      fd[in]:�豸������
//      pSendData[in]:��������ָ��
//      pRcvData[in]:��������ָ��
//      ulDataLen[in]:���ݳ���
//      usrData[in]:�û��ص��������
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
// ����Ҫ��:
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
    HI_U32 real_in_clk = g_cfg_apb_clock/DIV_BAUD;//SPI����������ʱ��
    HI_U32 spi_clk = 0;
    HI_U32 bit_per_fram = 0;//ÿ֡������Ҫ���ٸ�ʱ������
    HI_U32 time_per_fram = 0;//ÿ֡������Ҫ�೤ʱ��(���䳬ʱʱ��)
    HI_U32 reg_clk = 0;//д���Ĵ����е�CLK VAL
    
    if(HI_NULL == pUsrArg)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_INVALID_PARAMETER\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }
    pUsrCfg = (HI_SPI_DEVICE_USR_CFG_IN_S*)pUsrArg;
    spi_clk = pUsrCfg->baudRate;//SPIͨ��ʱ��Ƶ��
    //1.�����ж�
    if(array_id>=HI_SPI_SLAVE_DEVICE_COUNT||spi_clk>real_in_clk||spi_clk*256<real_in_clk)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_INVALID_PARAMETER\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    
    //2.����ʱ���ʼĴ���д��ֵ;real_in_clk/spi_clk:����ļĴ���ֵ;+(spi_clk-1)ʱ������Ӧ�����¶������reg_clk��Ҫ���϶���
    reg_clk = (real_in_clk+(spi_clk-1))/spi_clk;// 1~256

    //3.����ÿ֡���䳬ʱʱ��
    //���ݿ��ΪpUsrArg->dataWidth+1;��+1����������(������������);micro wire֡��ʽ����һ��֡����ʱ��(��˫��)������д��ת�����waitVal
    bit_per_fram = pUsrArg->dataWidth+1+1;
    if((pUsrArg->framMode==HI_SPI_CFG_FRAM_MODE_MICROWIRE)&&(pUsrArg->waitEn==HI_SPI_CFG_WAIT_EN_ENABLE))
    {
        bit_per_fram = bit_per_fram+pUsrArg->dataWidth+pUsrArg->waitVal;
    }
    //����ÿ֡���䳬ʱʱ�� bit_per_fram*S_TO_US(1)/(spi_clk)Ϊһ�崫�䳬ʱʱ��;+spi_clk��ʱʱ�����϶���;*5:ʵ�����õ�SPI����С���·�ֵ,������Ӳ���ֵ��
    time_per_fram = ((bit_per_fram*S_TO_US(1)+spi_clk)/spi_clk)*5;//
    
    pDevPrvCfg = &g_spi_ctrl.devCfg[array_id];
    if(pDevPrvCfg->isUsed==HI_FALSE)
    {
        HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config FAIL] HI_ERR_ACCESS_DENIED\r\n");
        return HI_ERR_ACCESS_DENIED;
    }

    HI_DEBUG_SPI_Print("[HI_MDM_SPI_Config SUC][set baudRate=%u][real baudRate=%u][us_per_fram=%u][reg_clk=%u][dataWidth=%u]\r\n"
        ,pUsrCfg->baudRate,real_in_clk/reg_clk,time_per_fram,reg_clk,pUsrArg->dataWidth+1);

    //�޸Ĳ����еĲ����ʲ���
    reg_clk = reg_clk-1;
    pUsrCfg->cr0 &= (~(BIT_MASK(SPI_CR0_ST_BIT_SCR, SPI_CR0_BIT_WIDTH_SCR)));
    pUsrCfg->cr0 |= ((reg_clk<<SPI_CR0_ST_BIT_SCR)&(BIT_MASK(SPI_CR0_ST_BIT_SCR, SPI_CR0_BIT_WIDTH_SCR)));

    pUsrCfg->cr0 = (pUsrCfg->cr0&HI_SPI_SUPPORT_USR_CFG_BIT_CR0)|(HI_SPI_INNER_DEFAULT_VAL_CFG0&HI_SPI_SUPPORT_INNER_BIT_CFG0);
    pUsrCfg->cr1 = (pUsrCfg->cr1&HI_SPI_SUPPORT_USR_CFG_BIT_CR1)|(HI_SPI_INNER_DEFAULT_VAL_CFG1&HI_SPI_SUPPORT_INNER_BIT_CFG1);   

    //�û����ø�ֵ���ڲ�����ȫ�ֱ���
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
    //��ʼ��SPIӲ�����:��IO����
    if(enSpiId >= HI_SPI_ID_MAX)
    {
        return HI_ERR_INVALID_ID;
    }
    if(g_spi_ctrl.spiInfor[enSpiId].isInit==HI_FALSE)
    {
        //���÷�Ƶ�Ĵ���
        //HI_SSI_WRITE_REG(REG_SSI_CPSR, DIV_BAUD);//����Ƶ
        
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

