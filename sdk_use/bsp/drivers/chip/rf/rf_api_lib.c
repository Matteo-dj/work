//*****************************************************************************
//
//                  ��Ȩ���� (C),  ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : si4438_api_lib.c
//  �� �� ��   : V1.0
//  ��    ��   : wanglei/00279578
//  ��������   : 2014-10-10
//  ��������   : RF������ƽӿ�
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2014-10-10
//    ��    �� : wanglei/00279578
//    �޸����� : �����ļ�
//
//*****************************************************************************

/*includes*/
#include <stdarg.h>
#include <sal_common.h>
#include <radio_config_Si4438_b1.h>
#include <radio_config_Si4438_c2.h>
#include <rf_api_lib.h>
#include <radio_comm.h>
#include <gpio_spi.h>
#include <hi_driver.h>
#include <hi_drv_com.h>
#include <dfx_sys.h>
#include <hi_mdm_spi.h>
#include "rf_api.h"



#define RF_CONTROL_GPIO_NUM 6//RF����ռ��6��GPIO
#define RF_RESET_DELAY_TIME 20
#define TX_CONDITION_CHANGE2READY           0x30
#define RX_CONDITION_START_RX_IMMEDIATELY   0x0
#define RF_DIF_DATARATE_NUM 4

#define RF_PWR_LVL_DEFAULT  0x14
#define RF_PWR_LVL_MAX      0x7F

typedef struct
{
    HI_GPIO_DIRECTION_E io_dir[RF_CONTROL_GPIO_NUM];                        //IO����
    HI_GPIO_VALUE_E io_output_val[RF_CONTROL_GPIO_NUM];                 //IO���ֵ
    HI_U8 io_func[RF_CONTROL_GPIO_NUM];              //IO����ѡ��
	HI_U8 rsv[2];
}HI_DEFAULT_GPIO_CONFIG;

typedef struct
{
    HI_HW_IO_IDX_E ioFunNum;
    HI_U32 ioInitFun;
	HI_U8 ioNum;
    HI_BOOL ioInitDir;
	HI_U8 reserved[2];
}HI_RF_IO_CFG_ST;

/* Ĭ��IO���ã�����rf�ܽ����ã������жϲ���rfģ��ʱ�ָ�ԭ��IO���� */
HI_PRV HI_DEFAULT_GPIO_CONFIG s_default_io_cfg;
/* rf��ͬ����,��ͬоƬ�汾���� */
HI_PRV HI_U8 rf_chip_cfg_C2[] = RADIO_C2_CONFIGURATION_DATA_ARRAY_40K;
HI_PRV HI_U8 rf_chip_cfg_B1[] = RADIO_B1_CONFIGURATION_DATA_ARRAY_40K;
HI_PRV HI_U8 * rfconfig_40k = HI_NULL;
/*���ò�ͬ�ĵ���ģʽ*/
HI_U8 g_modem_type = 0;
/*DEBUG ����ģʽ�Ƿ���ȷ����*/
//HI_U8 g_modem_define = 0;//0��ʾδ���ã�1��ʾB�汾FSK,2��ʾB�汾GFSK
                         //3��ʾC�汾FSK,4��ʾC�汾GFSK
/*b1оƬ��2GFSK����*/
static HI_U8 rf_b1_modem_type_1[] = {RF_B1_MODEM_MOD_TYPE_12_2GFSK};
static HI_U8 rf_b1_modem_type_2[] = {RF_B1_MODEM_OOK_CNT1_11_2GFSK};

/*C2оƬ��2GFSK��*/
static HI_U8 rf_c2_modem_type_1[] = {RF_C2_MODEM_MOD_TYPE_12_2GFSK};
static HI_U8 rf_c2_modem_type_2[] = {RF_C2_MODEM_RAW_CONTROL_8_2GFSK};
static HI_U8 rf_c2_modem_type_3[] = {RF_C2_MODEM_SPIKE_DET_2_2GFSK};
static HI_U8 rf_c2_modem_type_4[] = {RF_C2_MODEM_DSA_CTRL1_5_2GFSK};

/* rf��ͬоƬ�汾��־λ */
HI_U8 chip_cfg_file = 0;

/* ��������rfоƬ���书�� */
HI_U8 g_rf_pa_pwr_lvl = RF_PWR_LVL_MAX;


/*SDNʹ��*/
HI_VOID SdnEnable(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(RF_SDN_GPIO_IDX, HI_GPIO_VALUE0);
}

/*SDN�ر�*/
HI_VOID SdnDisable(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(RF_SDN_GPIO_IDX, HI_GPIO_VALUE1);
}

/****************************************************************************
// ��������:  rf_chip_reset
// ��������:  rfоƬ��λ(�ڲ��Ĵ���״̬���)��
// ����˵��:  ��
// �� �� ֵ:  ��
// ����Ҫ��:  �ڲ�(��δ����)����ֹ�ж��е���
****************************************************************************/
HI_PRV HI_VOID rf_chip_reset(HI_VOID)
{
    SdnDisable();
    HI_MDM_Udelay(RF_RESET_DELAY_TIME);
    SdnEnable();
    HI_MDM_Udelay(RF_RESET_DELAY_TIME);
}

/****************************************************************************
// ��������:  rf_chip_powerup
// ��������:  4438оƬ�ϵ�
// ����˵��:  ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ڲ�(��δ����)����ֹ�ж��е���
****************************************************************************/
HI_PRV HI_U32 rf_chip_powerup(HI_VOID)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_POWER_UP] = {RF_B1_POWER_UP};
    return radio_comm_SendCmd(SI446X_CMD_ARG_COUNT_POWER_UP,tx_cmd); 
}

/****************************************************************************
// ��������:  rf_interrupt_enable
// ��������:  4438оƬ�ж�ʹ�ܿ���
// ����˵��:  enable:   �Ƿ�ʹ��
//            callback: �жϻص�������ȥʹ��ʱ��ֵ����ΪHI_NULL
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ڲ�(��δ����)
****************************************************************************/
HI_PRV HI_U32 rf_interrupt_enable(HI_RF_INT_ENABLE enable, HI_RF_INT_CALLBACK callback)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if (RF_INT_ENABLE == enable)
    {
        ret =  HI_MDM_IO_IntConnect(RF_IRQ_GPIO_IDX,HI_INT_EDGE,HI_EDGE_FALL_LEVEL_LOW,callback,0, HI_FALSE);//�½��ش����ж�
    }
    else
    {
        ret =  HI_MDM_IO_IntDisconnect(RF_IRQ_GPIO_IDX);
    }

    return ret;
}

/****************************************************************************
// ��������:  rf_configuration_init
// ��������:  rfģ���ʼ��
// ����˵��:  rf_rate:���ʣ��ݲ�ʹ��
//            callback:�жϻص�������Ҫ��ǿ�
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_configuration_init(HI_RF_RATE_E rf_rate, HI_RF_INT_CALLBACK callback)
{
    HI_U8 col = 0;
    HI_U8 numOfBytes = 0;
    HI_U8 tx_cmd[SEND_CMD_COUNT] = {0};
    HI_U32 ret = HI_ERR_FAILURE;
    REPLY_GET_INT_STATUS_MAP int_status = {0};
    HI_U8 *set_prop = HI_NULL;

    if((rf_rate >= RF_CFG_RATE_MAX)||(rf_rate <= RF_CFG_RATE_MIN)\
    ||(HI_NULL == callback)||(HI_NULL == rfconfig_40k))
    {
        return HI_ERR_FAILURE;
    }
    if(chip_cfg_file == SI4438_B1)
    {
        rfconfig_40k = rf_chip_cfg_B1;
    }
    else
    {
        rfconfig_40k = rf_chip_cfg_C2;
    }

    set_prop = rfconfig_40k;

    (HI_VOID)rf_interrupt_enable(RF_INIT_DISABLE, HI_NULL);
    rf_chip_reset();
    while (*set_prop != 0x00)
    {
        /* �����е�����ṹ����:����� + �������� */
        numOfBytes = *set_prop++;

        if (numOfBytes > SEND_CMD_COUNT)
        {
            return HI_ERR_FAILURE;
        }

        for (col = 0; col < numOfBytes; col++)
        {
            tx_cmd[col] = *set_prop;
            set_prop ++;
        }
        ret = radio_comm_SendCmd(numOfBytes, tx_cmd);
        if (HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
        if (radio_nirq_level() == HI_GPIO_VALUE0)
        {
            rf_get_int_status(0, 0, 0, &int_status);
            if (int_status.chip_pend & SI446X_CMD_GET_INT_STATUS_REP_CMD_ERROR_BIT)
            {
#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
                radio_init_cmd_err();
#endif
                return HI_ERR_RF_CMD_ERR;
            }
        }
    }

    /* ����RFоƬ���书�� */
    ret = rf_set_property(SI446X_PROP_GRP_ID_PA, 1,SI446X_PROP_GRP_INDEX_PA_PWR_LVL,&g_rf_pa_pwr_lvl);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    /*���õ���ģʽ2FSK,2GFSK*/
    ret = rf_switch_modem_type_to_gfsk();
    if(HI_ERR_SUCCESS != ret)
    {
       return HI_ERR_FAILURE;
    }

    /* ���óɹ���ʹ���жϣ����������ʹ���жϣ�ͨ�������뷵���ϲ㣬���ϲ������������ */
    (HI_VOID)rf_interrupt_enable(RF_INT_ENABLE, callback);
        
    return HI_ERR_SUCCESS;
}

#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
/****************************************************************************
// ��������:  rf_register_cmd
// ��������:  ά�⣬���ڼ�¼rf�������˳��
// ����˵��:  ��
// �� �� ֵ:  ��
// ����Ҫ��:  ��
****************************************************************************/
HI_VOID rf_register_cmd(HI_VOID)
{
    radio_backup_cmd_order();
}
#endif

/****************************************************************************
// ��������:  rf_get_part_info
// ��������:  ��ȡrfģ��汾��Ϣ
// ����˵��:  part_info:����ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/

HI_U32 rf_get_part_info(REPLY_PART_INFO_MAP * part_info)
{
    HI_U8 tx_cmd = SI446X_CMD_ID_PART_INFO;
    HI_U32 ret;

    if(HI_NULL == part_info)
    {
        return HI_ERR_FAILURE;
    }
    ret = radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,
                              &tx_cmd,
                              SI446X_CMD_REPLY_COUNT_PART_INFO,
                              (HI_U8*)part_info);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }


    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_start_tx
// ��������:  ����rfģ��Ϊ����ģʽ
// ����˵��:  channel:  �ŵ�
//            tx_len:   ������Ч���ݳ��ȣ�������pHֵ����CRC��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_start_tx(HI_U8 channel, HI_U16 tx_len)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_START_TX] = {0};

    tx_cmd[0] = SI446X_CMD_ID_START_TX;
    tx_cmd[1] = channel;
    tx_cmd[2] = TX_CONDITION_CHANGE2READY;//������ɺ����Ready״̬
    tx_cmd[3] = (HI_U8)(tx_len >> 8);
    tx_cmd[4] = (HI_U8)(tx_len);
    tx_cmd[5] = 0x00;

    return radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_TX, tx_cmd);
}

/****************************************************************************
// ��������:  rf_start_rx
// ��������:  ����rfģ��Ϊ����ģʽ
// ����˵��:  channel:  �ŵ�
//            tx_len:   ������Ч���ݳ��ȣ�������pHֵ����CRC��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_start_rx(HI_U8 channel, HI_U16 rx_len)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_START_RX] = {0};

    tx_cmd[0] = SI446X_CMD_ID_START_RX;
    tx_cmd[1] = channel;
    tx_cmd[2] = RX_CONDITION_START_RX_IMMEDIATELY;
    tx_cmd[3] = (HI_U8)(rx_len >> 8);
    tx_cmd[4] = (HI_U8)(rx_len);
    tx_cmd[5] = SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE;
    tx_cmd[6] = SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX;
    tx_cmd[7] = SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX;

    return radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, tx_cmd);
}

/****************************************************************************
// ��������:  rf_get_int_status
// ��������:  ��ȡrf�ж�״̬���������Ӧpend״̬
// ����˵��:  ph_clr_pend: ��ӦbitΪ0�����pend
//            modem_clr_pend: ��ӦbitΪ0�����pend
//            chip_clr_pend: ��ӦbitΪ0�����pend
//            int_status: ��ȡ�ж�״ָ̬��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_get_int_status(HI_U8 ph_clr_pend, HI_U8 modem_clr_pend, HI_U8 chip_clr_pend,REPLY_GET_INT_STATUS_MAP *int_status)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_GET_INT_STATUS] = {0};
    HI_U32 ret;

    if(HI_NULL == int_status)
    {
        return HI_ERR_FAILURE;
    }
    tx_cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
    tx_cmd[1] = ph_clr_pend;
    tx_cmd[2] = modem_clr_pend;
    tx_cmd[3] = chip_clr_pend;
    ret = radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
                              tx_cmd,
                              SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
                              (HI_U8*)int_status);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_set_property
// ��������:  ����rfоƬ����
// ����˵��:  group: ���
//            num_props:��������
//            start_prop:��ʼ����
//            property_cfg:  �������ݻ���ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_set_property(HI_U8 group, HI_U8 num_props, HI_U8 start_prop, HI_U8 *property_cfg)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_SET_PROPERTY] = {0};
    HI_U8 i = 0;

    if((HI_NULL == property_cfg)||(num_props > SI446X_CMD_SET_PROPERTY_ARG_NUM_PROPS_MAX))
    {
        return HI_ERR_FAILURE;
    }

    tx_cmd[0] = SI446X_CMD_ID_SET_PROPERTY;
    tx_cmd[1] = group;
    tx_cmd[2] = num_props;
    tx_cmd[3] = start_prop;
    for (i=0; i<num_props; i++)
    {
/*
                    �������ˡ�z00193780
                    ������ˡ�w00190206
                    �����������i�����ֵΪ10��tx_cmd��property_cfg����������󱨡�IOģ��SPI,����޸Ŀ��ܶ�����Ӱ�죬��V200Ӳ����ʱ��֧����֤�����Բ����޸ġ�
                    ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/	
        tx_cmd[i+SI446X_CMD_SET_PROPERTY_ARG_DATA0_INDEX] = property_cfg[i];
    }
    return radio_comm_SendCmd(SI446X_CMD_SET_PROPERTY_ARG_DATA0_INDEX + num_props, tx_cmd);
}

/****************************************************************************
// ��������:  rf_change_state
// ��������:  �л�rfоƬ״̬
// ����˵��:  new_state: ����״̬
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_change_state(HI_U8 new_state)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_CHANGE_STATE] = {0};

    tx_cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
    tx_cmd[1] = new_state;

    return radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_CHANGE_STATE, tx_cmd);
}

/****************************************************************************
// ��������:  rf_change_state
// ��������:  �л�rfоƬ״̬
// ����˵��:  fifo_reset: SI446X_CMD_FIFO_INFO_ARG_TX_BIT
//                        ��SI446X_CMD_FIFO_INFO_ARG_RX_BIT���������
//            fifo_info:fifo��Ϣ������fifo�����ݸ����ͷ���fifo�п������
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_fifo_info(HI_U8 fifo_reset, REPLY_FIFO_INFO_MAP *fifo_info)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_FIFO_INFO] = {0};
    HI_U32 ret;

    if(HI_NULL == fifo_info)
    {
        return HI_ERR_FAILURE;
    }
    tx_cmd[0] = SI446X_CMD_ID_FIFO_INFO;
    tx_cmd[1] = fifo_reset;
    ret = radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FIFO_INFO,
                              tx_cmd,
                              SI446X_CMD_REPLY_COUNT_FIFO_INFO,
                              (HI_U8*)fifo_info);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_write_tx_fifo
// ��������:  ������д��rfоƬfifo
// ����˵��:  numBytes: д�����ݳ���
//            pTxData:  д�����ݻ���ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_write_tx_fifo(HI_U8 byte_num, HI_U8* data)
{
    HI_U32 ret = HI_ERR_FAILURE;
    
    if(HI_NULL == data)
    {
        return HI_ERR_FAILURE;
    }

    ret = radio_comm_WriteDataNoCts(SI446X_CMD_ID_WRITE_TX_FIFO, byte_num, data);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_read_rx_fifo
// ��������:  ��rfоƬ����fifo��������
// ����˵��:  numBytes: ���������ݳ���
//            pRxData:  �������ݻ���ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_read_rx_fifo(HI_U8 byte_num, HI_U8* data)
{
    HI_U32 ret;
    if(HI_NULL == data)
    {
        return HI_ERR_FAILURE;
    }

    ret = radio_comm_ReadDataNoCts(SI446X_CMD_ID_READ_RX_FIFO, byte_num, data);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_get_property
// ��������:  ��rfоƬ����ĳ�����ԣ��緢�书�ʵ�
// ����˵��:  group: ���
//            num_props:��������
//            start_prop:��ʼ����
//            property_cfg:  �������ݻ���ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_get_property(HI_U8 group, HI_U8 num_props, HI_U8 start_prop,
    HI_U8 *property_cfg)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_GET_PROPERTY] = {0};
    HI_U32 ret;

    if((HI_NULL == property_cfg)||(num_props > SI446X_CMD_GET_PROPERTY_ARG_NUM_PROPS_MAX))
    {
        return HI_ERR_FAILURE;
    }

    tx_cmd[0] = SI446X_CMD_ID_GET_PROPERTY;
    tx_cmd[1] = group;
    tx_cmd[2] = num_props;
    tx_cmd[3] = start_prop;

    ret = radio_comm_SendCmdGetResp(SI446X_CMD_ARG_COUNT_GET_PROPERTY, tx_cmd, num_props,
        property_cfg);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

/****************************************************************************
// ��������:  rf_config_gpio_pin
// ��������:  ����rfоƬ����IO
//            ��������ԭ��ͼ��3���ܽ�(GPIO0/2/3)��������PAоƬ
// ����˵��:  ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_config_gpio_pin(HI_RF_GPIO_CFG_S *gpio_cfg)
{
    HI_U8 tx_cmd[SI446X_CMD_ARG_COUNT_GPIO_PIN_CFG] = {0};

    tx_cmd[0] = SI446X_CMD_ID_GPIO_PIN_CFG;
    (hi_void)memcpy_s(&tx_cmd[1], sizeof(HI_RF_GPIO_CFG_S), gpio_cfg, sizeof(HI_RF_GPIO_CFG_S));
    return radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_GPIO_PIN_CFG,tx_cmd);
}

/****************************************************************************
// ��������:  rf_frr_a_read
// ��������:  ���ٶ�A�Ĵ���
// ����˵��:  respByteCount: ���ݳ���
//            data:���ݻ���ָ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_PRV HI_U32 rf_frr_a_read(HI_U8 respByteCount,HI_U8 *data)
{
    if(HI_NULL == data)
    {
        return HI_ERR_FAILURE;
    }

    return radio_comm_ReadDataNoCts(SI446X_CMD_ID_FRR_A_READ, respByteCount,data);
}

/****************************************************************************
// ��������:  rf_get_rssi
// ��������:  ���ٶ�A�Ĵ������˴�������Ϊ�ź�ǿ��
// ����˵��:  val: �ź�ǿ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  �ж��н�ֹ����
****************************************************************************/
HI_U32 rf_get_rssi(HI_U8 *val)
{
    return rf_frr_a_read(1, val);
}


/* �ڲ����ܺ��� */

/***********************************/
/* RF���ܶ�ӦGPIO���ù�ϵ
SCLK --- GPIO31
NSEL --- GPIO25
MISO --- GPIO13
MOSI --- GPIO14
STA��Ʒ��̬
SDN  --- GPIO16(STA)
NIRQ --- GPIO4 (STA)
II�ɲ�Ʒ��̬
SDN  --- GPIO24(II��)
NIRQ --- GPIO8 (II��) */
/***********************************/
HI_VOID rf_sta_gpio_config(HI_VOID)
{
    HI_U8 i = 0;
    HI_RF_IO_CFG_ST rfIODefault[RF_CONTROL_GPIO_NUM] = {{HI_HW_UART1_RXD_IO13,HI_GPIO13_SEL_GPIO13,HI_GPIO_IDX_13,HI_GPIO_DIRECTION_IN,{0}},\
                                                        {HI_HW_SFC_HOLDN_IO25,HI_GPIO25_SEL_GPIO25,HI_GPIO_IDX_25,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_PERI_RSTN_IO31,HI_GPIO31_SEL_GPIO31,HI_GPIO_IDX_31,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14,HI_GPIO_IDX_14,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART3_TXD_IO16,HI_GPIO16_SEL_GPIO16,HI_GPIO_IDX_16,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_STATUS_LED_IO4,HI_GPIO4_SEL_GPIO4,HI_GPIO_IDX_4,HI_GPIO_DIRECTION_IN,{0}}};

    for(i = 0; i < RF_CONTROL_GPIO_NUM; i++)
    {
        HI_MDM_IO_GetMux(rfIODefault[i].ioFunNum , &s_default_io_cfg.io_func[i]);
        HI_MDM_IO_GetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, &s_default_io_cfg.io_dir[i]);
        HI_MDM_IO_GetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, &s_default_io_cfg.io_output_val[i]);

        HI_MDM_IO_SetMux(rfIODefault[i].ioFunNum, (HI_U8)(rfIODefault[i].ioInitFun));
        HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, (HI_GPIO_DIRECTION_E)rfIODefault[i].ioInitDir);
    }
    return;
}
HI_VOID rf_staii_gpio_config(HI_VOID)
{
    HI_U8 i = 0;
    HI_RF_IO_CFG_ST rfIODefault[RF_CONTROL_GPIO_NUM] = {{HI_HW_UART1_RXD_IO13,HI_GPIO13_SEL_GPIO13,HI_GPIO_IDX_13,HI_GPIO_DIRECTION_IN,{0}},\
                                                        {HI_HW_SFC_HOLDN_IO25,HI_GPIO25_SEL_GPIO25,HI_GPIO_IDX_25,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_PERI_RSTN_IO31,HI_GPIO31_SEL_GPIO31,HI_GPIO_IDX_31,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14,HI_GPIO_IDX_14,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART3_TXD_IO16,HI_GPIO16_SEL_GPIO16,HI_GPIO_IDX_16,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_I2C_SDA_IO8,HI_GPIO8_SEL_GPIO8,HI_GPIO_IDX_8,HI_GPIO_DIRECTION_IN,{0}}};

    for(i = 0; i < RF_CONTROL_GPIO_NUM; i++)
    {
        HI_MDM_IO_GetMux(rfIODefault[i].ioFunNum, &s_default_io_cfg.io_func[i]);
        HI_MDM_IO_GetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, &s_default_io_cfg.io_dir[i]);
        HI_MDM_IO_GetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, &s_default_io_cfg.io_output_val[i]);

        HI_MDM_IO_SetMux(rfIODefault[i].ioFunNum, (HI_U8)(rfIODefault[i].ioInitFun));
        HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, (HI_GPIO_DIRECTION_E)rfIODefault[i].ioInitDir);
    }
    return;
}
/*��GPIO�ָ���ԭ��������*/
HI_VOID rf_sta_gpio_resume_default(HI_VOID)
{
    HI_U8 i = 0;
    HI_RF_IO_CFG_ST rfIODefault[RF_CONTROL_GPIO_NUM] = {{HI_HW_UART1_RXD_IO13,HI_GPIO13_SEL_GPIO13,HI_GPIO_IDX_13,HI_GPIO_DIRECTION_IN,{0}},\
                                                        {HI_HW_SFC_HOLDN_IO25,HI_GPIO25_SEL_GPIO25,HI_GPIO_IDX_25,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_PERI_RSTN_IO31,HI_GPIO31_SEL_GPIO31,HI_GPIO_IDX_31,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14,HI_GPIO_IDX_14,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART3_TXD_IO16,HI_GPIO16_SEL_GPIO16,HI_GPIO_IDX_16,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_STATUS_LED_IO4,HI_GPIO4_SEL_GPIO4,HI_GPIO_IDX_4,HI_GPIO_DIRECTION_IN,{0}}};

    for(i = 0; i < RF_CONTROL_GPIO_NUM; i++)
    {
        if(rfIODefault[i].ioFunNum != HI_HW_STATUS_LED_IO4)
        {
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, (HI_GPIO_DIRECTION_E)s_default_io_cfg.io_dir[i]);
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, s_default_io_cfg.io_output_val[i]);
        }
        else
        {
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, HI_GPIO_DIRECTION_OUT);
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, HI_GPIO_VALUE0);
        }
        
        HI_MDM_IO_SetMux(rfIODefault[i].ioFunNum, s_default_io_cfg.io_func[i]);
    }
}
HI_VOID rf_staii_gpio_resume_default(HI_VOID)
{
    HI_U8 i = 0;
    HI_RF_IO_CFG_ST rfIODefault[RF_CONTROL_GPIO_NUM] = {{HI_HW_UART1_RXD_IO13,HI_GPIO13_SEL_GPIO13,HI_GPIO_IDX_13,HI_GPIO_DIRECTION_IN,{0}},\
                                                        {HI_HW_SFC_HOLDN_IO25,HI_GPIO25_SEL_GPIO25,HI_GPIO_IDX_25,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_PERI_RSTN_IO31,HI_GPIO31_SEL_GPIO31,HI_GPIO_IDX_31,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14,HI_GPIO_IDX_14,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_UART3_TXD_IO16,HI_GPIO16_SEL_GPIO16,HI_GPIO_IDX_16,HI_GPIO_DIRECTION_OUT,{0}},\
                                                        {HI_HW_I2C_SDA_IO8,HI_GPIO8_SEL_GPIO8,HI_GPIO_IDX_8,HI_GPIO_DIRECTION_IN,{0}}};

    for(i = 0; i < RF_CONTROL_GPIO_NUM; i++)
    {
        if(rfIODefault[i].ioFunNum != HI_HW_I2C_SDA_IO8)
        {
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, (HI_GPIO_DIRECTION_E)s_default_io_cfg.io_dir[i]);
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, s_default_io_cfg.io_output_val[i]);
        }
        else
        {
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)rfIODefault[i].ioNum, HI_GPIO_DIRECTION_OUT);
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)rfIODefault[i].ioNum, HI_GPIO_VALUE0);
        }
        
        HI_MDM_IO_SetMux(rfIODefault[i].ioFunNum, s_default_io_cfg.io_func[i]);
    }
}

HI_PRV HI_U32 rf_check_chip_version(HI_VOID)
{
	HI_U32 ret = HI_ERR_FAILURE;
	REPLY_PART_INFO_MAP RfVersion = {0};
        
	ret = rf_get_part_info(&RfVersion);
	if(HI_ERR_SUCCESS != ret)
	{
		return HI_ERR_FAILURE;
	}
    
	if((RfVersion.part_msb == RF_CHIP_VERSION_MSB)&&(RfVersion.part_lsb == RF_CHIP_VERSION_LSB))
	{
		if(RfVersion.chiprev == SI4438_CHIPREV_B1)
		{
			chip_cfg_file = SI4438_B1;

		}
		else //(RfVersion.chiprev == SI4438_CHIPREV_C2)��ֹ�Ժ���ּȲ���B�ֲ���C��оƬ
		{
			chip_cfg_file = SI4438_C2;
		}
		
		return HI_ERR_SUCCESS;
	}
	
	return HI_ERR_FAILURE;
}


HI_VOID rf_gpio_spi_init_io(HI_VOID)//ģ��SPI
{
    HI_MDM_IO_SetMux(HI_HW_UP_RX_LED_IO0, HI_GPIO0_SEL_GPIO0);//SDO
    HI_MDM_IO_SetDir(HI_GPIO_IDX_0, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_0, HI_GPIO_VALUE0);
    
    HI_MDM_IO_SetMux(HI_HW_UP_TX_LED_IO1, HI_GPIO1_SEL_GPIO1);//SDI
    HI_MDM_IO_SetDir(HI_GPIO_IDX_1, HI_GPIO_DIRECTION_IN);
    
    HI_MDM_IO_SetMux(HI_HW_CRSZ_DTC_IO6,HI_GPIO6_SEL_CRSZ_DTC);//CLK
    HI_MDM_IO_SetDir(HI_GPIO_IDX_6, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_6, HI_GPIO_VALUE0);
    
    HI_MDM_IO_SetMux(HI_HW_GPIO28_BACKUP,HI_GPIO28_BACKUP_SEL_GPIO28);//CS
    HI_MDM_IO_SetDir(HI_GPIO_IDX_28, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);

#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
    HI_MDM_IO_SetMux(HI_HW_GPIO29_BACKUP, HI_GPIO29_BACKUP_SEL_GPIO29);//SDN ��λ�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_29, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_29, HI_GPIO_VALUE0);//SDN��ʼֵΪ�͵�ƽ

    HI_MDM_IO_SetMux(HI_HW_GPIO27_BACKUP, HI_GPIO27_BACKUP_SEL_GPIO27);//irq�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_27, HI_GPIO_DIRECTION_IN);

    HI_MDM_IO_SetMux(HI_HW_I2C_SCL_IO7,HI_GPIO7_SEL_GPIO7);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_7, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_7, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��

    HI_MDM_IO_SetMux(HI_HW_I2C_SDA_IO8,HI_GPIO8_SEL_GPIO8);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_8, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_8, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��

#else//sta
    HI_MDM_IO_SetMux(HI_HW_SFC_HOLDN_IO25, HI_GPIO25_SEL_GPIO25);//SDN ��λ�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_25, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_25, HI_GPIO_VALUE0);//SDN��ʼֵΪ�͵�ƽ

    HI_MDM_IO_SetMux(HI_HW_PERI_RSTN_IO31, HI_GPIO31_SEL_GPIO31);//irq�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_31, HI_GPIO_DIRECTION_IN);

    HI_MDM_IO_SetMux(HI_HW_UART1_RXD_IO13, HI_GPIO13_SEL_GPIO13);//ָʾ��
    HI_MDM_IO_SetMux(HI_HW_UART1_TXD_IO14, HI_GPIO14_SEL_GPIO14);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_13, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_14, HI_GPIO_DIRECTION_OUT);

    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_13, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_14, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��
    HI_MDM_Sleep(500);
#endif  
}

HI_VOID rf_init_io(HI_VOID)//spi ������
{

    HI_MDM_IO_SetMux(HI_HW_UP_RX_LED_IO0, HI_GPIO0_SEL_SSP_DO);//SDO
    HI_MDM_IO_SetMux(HI_HW_UP_TX_LED_IO1, HI_GPIO1_SEL_SSP_DI);//SDI
    HI_MDM_IO_SetMux(HI_HW_CRSZ_DTC_IO6,HI_GPIO6_SEL_SSP_CK);//CLK
    
    HI_MDM_IO_SetMux(HI_HW_GPIO28_BACKUP,HI_GPIO28_BACKUP_SEL_GPIO28);//CS
    HI_MDM_IO_SetDir(HI_GPIO_IDX_28, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);
   
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
  
    HI_MDM_IO_SetMux(HI_HW_GPIO29_BACKUP, HI_GPIO29_BACKUP_SEL_GPIO29);//SDN ��λ�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_29, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_29, HI_GPIO_VALUE0);//SDN��ʼֵΪ�͵�ƽ

    HI_MDM_IO_SetMux(HI_HW_GPIO27_BACKUP, HI_GPIO27_BACKUP_SEL_GPIO27);//irq�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_27, HI_GPIO_DIRECTION_IN);

    HI_MDM_IO_SetMux(HI_HW_I2C_SCL_IO7,HI_GPIO7_SEL_GPIO7);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_7, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_7, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��

    HI_MDM_IO_SetMux(HI_HW_I2C_SDA_IO8,HI_GPIO8_SEL_GPIO8);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_8, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_8, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��


#else//sta
    HI_MDM_IO_SetMux(HI_HW_SFC_HOLDN_IO25, HI_GPIO25_SEL_GPIO25);//SDN ��λ�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_25, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_25, HI_GPIO_VALUE0);//SDN��ʼֵΪ�͵�ƽ

    HI_MDM_IO_SetMux(HI_HW_PERI_RSTN_IO31, HI_GPIO31_SEL_GPIO31);//irq�ܽ�
    HI_MDM_IO_SetDir(HI_GPIO_IDX_31, HI_GPIO_DIRECTION_IN);

    HI_MDM_IO_SetMux(HI_HW_UART1_RXD_IO13, HI_GPIO13_SEL_GPIO13);//ָʾ��
    HI_MDM_IO_SetMux(HI_HW_UART1_TXD_IO14, HI_GPIO14_SEL_GPIO14);//ָʾ��
    HI_MDM_IO_SetDir(HI_GPIO_IDX_13, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_14, HI_GPIO_DIRECTION_OUT);

    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_13, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_14, HI_GPIO_VALUE1);//�ߵ�ƽ -> ��
    HI_MDM_Sleep(500);

#endif
}


HI_VOID rf_spi_prepare(HI_U32 data)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE0);
    HI_MDM_Udelay(1);/*CS���ͺ����RFоƬ�ֲ�Ҫ�󣬵ȴ�һ��ʱ�䣬��������������������*/
}

HI_VOID rf_spi_restore(HI_U32 data)
{
    if(!data)
    {
        HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);
    }
    HI_MDM_Udelay(1);/*�����������ݽ����󣬸���RFоƬ�ֲ�Ҫ�����һ��ʱ��������CS��*/
}

HI_U32 g_ulSpiFd = 0;
HI_U32 rf_spi_init(HI_VOID)
{
    HI_U32 ulRet;
    HI_SPI_DEVICE_USR_CFG_S UsrArg;

    ulRet = HI_MDM_SPI_Init(HI_SPI_ID_0);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    ulRet = HI_MDM_SPI_DevOpen(HI_SPI_ID_0, &g_ulSpiFd);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    UsrArg.baudRate = SI4438_SPI_BAUD_RATE;
    UsrArg.dataWidth =HI_SPI_CFG_DATA_WIDTH_E_8BIT;
    UsrArg.framMode =HI_SPI_CFG_FRAM_MODE_MOTOROLA;
    UsrArg.sph = HI_SPI_CFG_CLOCK_CPHA_0;
    UsrArg.spo = HI_SPI_CFG_CLOCK_CPOL_0;
    UsrArg.endian = HI_SPI_CFG_ENDIAN_LITTLE;
    UsrArg.prepareFunc = rf_spi_prepare;
    UsrArg.restoreFunc = rf_spi_restore;
    
    ulRet = HI_MDM_SPI_Config(g_ulSpiFd, &UsrArg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    return HI_ERR_SUCCESS;

}


/****************************************************************************
// ��������:  rf_chip_ready
// ��������:  ��ʼ��spi��rfģ�飬���rfģ���Ƿ���λ���汾���Ƿ�ƥ��
// ����˵��:  ��
// �� �� ֵ:  HI_ERR_SUCCESS:�ɹ�
//            ���������룬���hi_errno.h
// ����Ҫ��:  ����ʼ������һ�Σ��ж��н�ֹ����
****************************************************************************/
HI_U32 rf_chip_ready(HI_VOID)
{
	HI_U32 ret;

#ifdef PRODUCT_CFG_RF_USE_SPI_CONTROLLER 
    rf_init_io();
    rf_spi_init();
#else
    rf_gpio_spi_init_io();
#endif  

	rf_chip_reset();
	ret = rf_chip_powerup();
	HI_MDM_Sleep(10);//�ȴ�10ms
	if(HI_ERR_SUCCESS != ret)
	{
		return ret;
	}
    
	ret = rf_check_chip_version();
	if(HI_ERR_SUCCESS != ret)
	{
		return HI_ERR_FAILURE;
	}
	return HI_ERR_SUCCESS;

}

HI_VOID rf_set_power_value(HI_U8 power_value)
{
    /* �������Чֵ�����жϣ�ͬʱ��֤��R2�汾������ */
    if ((power_value == 0)||(power_value > RF_PWR_LVL_MAX))
    {
        power_value = RF_PWR_LVL_MAX;
    }
    g_rf_pa_pwr_lvl = power_value;
}

HI_U32 rf_switch_modem_type_to_gfsk(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;

    if(SI4438_B1 == chip_cfg_file)
    {
        ret = rf_set_property(rf_b1_modem_type_1[0], rf_b1_modem_type_1[1], rf_b1_modem_type_1[2], &rf_b1_modem_type_1[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
        ret = rf_set_property(rf_b1_modem_type_2[0], rf_b1_modem_type_2[1], rf_b1_modem_type_2[2], &rf_b1_modem_type_2[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
        return HI_ERR_SUCCESS;
    }
    else
    {
        ret = rf_set_property(rf_c2_modem_type_1[0], rf_c2_modem_type_1[1], rf_c2_modem_type_1[2], &rf_c2_modem_type_1[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }

        ret = rf_set_property(rf_c2_modem_type_2[0], rf_c2_modem_type_2[1], rf_c2_modem_type_2[2], &rf_c2_modem_type_2[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }

        ret = rf_set_property(rf_c2_modem_type_3[0], rf_c2_modem_type_3[1], rf_c2_modem_type_3[2], &rf_c2_modem_type_3[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }

        ret = rf_set_property(rf_c2_modem_type_4[0], rf_c2_modem_type_4[1], rf_c2_modem_type_4[2], &rf_c2_modem_type_4[3]);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
        return HI_ERR_SUCCESS;
    }
}
