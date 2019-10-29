//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : drv_dfx.c
//  �� �� ��   : V1.0
//  ��    ��   :  chenghuanhuan/00132816
//  ��������   : 2012-05-09
//  ��������   : ��ά�ɲ����Ӧ��
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2012-05-09
//    ��    �� : chenghuanhuan/00132816
//    �޸����� : �����ļ�
//               2012-06-19 upload_data ���������ɼ�
//
//*****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>


#if defined(PRODUCT_CFG_COLLECT_DATA)
#include <dfx_phy.h>
#include <drv_collectdata.h>
#include <mac_inf.h>
#include <hi_drv_com.h>
#include <os_vx_sal_api.h>
#include <hi_ioexp.h>
#include <sal_common.h>
#include <hi_low_power.h>
#include "dc_debug.h"

#define IO_EXP_NUM_BPF_SHORT                IO_EXP_IO_NUM_03


//#define REG_DC_AGC_SET      0x101a0560
#define REG_DC_AGC_SET      0x101a0554
//#define REG_DC_AGC_VAL      0x101a0564
#define REG_DC_AGC_VAL      0x101a0558

#define DC_BASE_ADDR           0x101a0e00
#define DC_CONTROL            (DC_BASE_ADDR+0x80)
#define DC_RX_STATUS          (DC_BASE_ADDR+0x84)
#define DC_RX_START_ADDR      (DC_BASE_ADDR+0x88)
#define DC_RX_END_ADDR        (DC_BASE_ADDR+0x8c)
#define DC_RX_CURR_ADDR       (DC_BASE_ADDR+0x90)
#define DC_TX_STATUS          (DC_BASE_ADDR+0x94)
#define DC_TX_START_ADDR      (DC_BASE_ADDR+0x98)
#define DC_TX_END_ADDR        (DC_BASE_ADDR+0x9c)
#define DC_TX_CURR_ADDR       (DC_BASE_ADDR+0xa0)
#define DC_DELAY_COUNT        (DC_BASE_ADDR+0xac)
#define DC_CAPTURE_CTRL       (DC_BASE_ADDR+0xb0)
#define DC_TRIGGER_MASK       (DC_BASE_ADDR+0xb4)
#define DC_TRIGGER_DATA       (DC_BASE_ADDR+0xb8)


#define CAPTURE_CTRL_REG_LADDR_BIT_ST 0

#define CAPTURE_CTRL_REG_TRI_MODE_BIT_ST 16

#define CAPTURE_CTRL_REG_TRI_TYPE_BIT_ST 18

#define CAPTURE_CTRL_REG_VALID_CTRL_BIT_ST 21

#define CAPTURE_CTRL_REG_TRI_SEL_BIT_ST 24 

#define CAPTURE_CTRL_TRI_MODE_NOT_TRI    0
#define CAPTURE_CTRL_TRI_MODE_SOFT_TRI   1
#define CAPTURE_CTRL_TRI_MODE_BUS_TRI    2
#define CAPTURE_CTRL_TRI_MODE_MASK_TRI   3

#define CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL   0
#define CAPTURE_CTRL_TRI_TYPE_RISE_EDGE   1
#define CAPTURE_CTRL_TRI_TYPE_FALL_EDGE   2
#define CAPTURE_CTRL_TRI_TYPE_HIGH_LEVEL  3
HI_U32 g_force_stop_dc = 0;
HI_U32 DC_Check_TrigerType(DC_TRIG_TYPE_ENUM enTrigerType)
{
        DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC]3 enTrigerType =%d \r\n! ",enTrigerType);
        if((enTrigerType >= EN_TRIG_TYPE_MAX) && (enTrigerType != EN_TRIG_TYPE_MASK_DATA))
            return HI_ERR_FAILURE;
        else
            return HI_ERR_SUCCESS;
}

HI_U32 DC_Check_DataSrc(DC_TRIG_DATA_ENUM  enDataSrc)
{
        if(enDataSrc >= EN_TRIG_DATA_MAX)
            return HI_ERR_FAILURE;
        else
            return HI_ERR_SUCCESS;
}

HI_U32 DC_Check_AgcGain(HI_S32 lAgcGain)
{
    if(lAgcGain==DRV_DC_UN_FIX_AGCGAIN)
    {
        return HI_ERR_SUCCESS;
    }
    if(lAgcGain < -12 || lAgcGain > 48 || (((HI_U32)lAgcGain&0x1) != 0x0))
        return HI_ERR_FAILURE;
    else
        return HI_ERR_SUCCESS;
}

HI_U32 DC_Check_FixAgcGain(HI_S32 lAgcGain)
{
    if(lAgcGain < -12 || lAgcGain > 48 || (((HI_U32)lAgcGain&0x1) != 0x0))
        return HI_ERR_FAILURE;
    else
        return HI_ERR_SUCCESS;
}


HI_U32 DC_Check_Addr_And_Len(HI_U32 ulStartAddr,HI_U32 ulEndAddr,HI_U32 ulAfterPointlen)
{
    if (ulStartAddr>= ulEndAddr)
        return HI_ERR_FAILURE;

    if (((ulStartAddr&0x3)!=0) 
        ||((ulEndAddr & 0x3)!=0) 
        ||((ulAfterPointlen & 0x3)!=0)
        ||(ulAfterPointlen == 0))
    {
        return HI_ERR_FAILURE;
    }

    // ��оƬ��Աȷ�ϴ������Ĳɼ����Ȳ����ܴ��ڲ������䣬���ʹ����Ա���ô����������
    if((ulEndAddr - ulStartAddr) < ulAfterPointlen)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;

}


HI_PRV HI_U32 dc_check_para(DC_INPUT_PARA_STRU *pDcInput)
{
    DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC]1 enTrigerType =%d \r\n! ",pDcInput->enTrigerType); 

    if(pDcInput==HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    if(HI_ERR_SUCCESS != DC_Check_Addr_And_Len(pDcInput->ulStartAddr,pDcInput->ulEndAddr,pDcInput->ulAfterPointlen))
    {
        DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC] STARTADDR || ENDADDR || ulAfterPointlen ERROR! ");
        return HI_ERR_FAILURE;
    }   
    DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC]2 enTrigerType =%d \r\n! ",pDcInput->enTrigerType);
    if (HI_ERR_SUCCESS != DC_Check_TrigerType(pDcInput->enTrigerType))
    {
        DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC] enTrigerType ERROR!\r\n ");
        return HI_ERR_FAILURE;
    }
    if (HI_ERR_SUCCESS != DC_Check_DataSrc(pDcInput->enDataSrc))
    {
        DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC] enDataSrc ERROR! \r\n");
        return HI_ERR_FAILURE;
    }
    
    if (HI_ERR_SUCCESS != DC_Check_AgcGain(pDcInput->lAgcGain))
    {
        DRV_NDM_DC_DRV_DEBUG_Print("[DRV DC] lAgcGain ERROR! \r\n");
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}
        

HI_VOID DC_Data_CollectPrepare(HI_IN DC_INPUT_PARA_STRU *pDcInput)
{
    DC_TRIG_DATA_ENUM  enDataSrcTemp = pDcInput->enDataSrc;
    HI_U32 ctrl_reg_val = 0;
    HI_U32 ctrl_tri_type = 0;
    HI_U32 ctrl_tri_mode = 0;
    HI_U32 ctrl_tri_sel = 0;
    HI_U32 ctrl_valid_ctrl = 0;

    /*��ղ����ռ�*/
    (hi_void)memset_s((HI_VOID *)pDcInput->ulStartAddr, (pDcInput->ulEndAddr - pDcInput->ulStartAddr)
        ,0xff, (pDcInput->ulEndAddr - pDcInput->ulStartAddr));
    HI_MDM_DataCacheFlush((HI_PVOID)pDcInput->ulStartAddr, (HI_S32)(pDcInput->ulEndAddr-pDcInput->ulStartAddr));

    //2.PHY����
    if(pDcInput->enDataSrc!=EN_TRIG_DA_12_BIT)
    {
        drv_set_collect_data_flag(COLLECT_DATA_ENABLE);
    }

    //3.�˲�������
    //HI_MAC_CloseAfeRxHpf();//�ر�AFE�Ľ��ո�ͨ�˲���
    drv_close_afe_rx_hpf();
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetOutPutVal(IO_EXP_NUM_BPF_SHORT, HI_IO_EXP_OUT_VAL_HIGHT);
#endif

    //�ȴ�(1s)��ȷ�����ͻ���ɵ�ǰ����;Ӳ���˲���by pass˲������������Ҫ�ȴ�һ��
    if(pDcInput->ulWaitEnvOkTime)
    {
        HI_MDM_Sleep(pDcInput->ulWaitEnvOkTime);
    }



    //4.����AGC����
    if ((pDcInput->lAgcGain) != DRV_DC_UN_FIX_AGCGAIN) //��ʾ�Ƿ��Զ�AGC
    {
        hi_u32 temp_value = 0;
        struct C_fix_aagc_gain *fix_aagc_gain = (struct C_fix_aagc_gain *)&temp_value;

        HI_REG_WRITE32(REG_DC_AGC_SET, HI_TRUE);
        HI_REG_READ32(REG_DC_AGC_VAL, temp_value);

        fix_aagc_gain->fix_aagc_gain = (HI_U32)pDcInput->lAgcGain;
        HI_REG_WRITE32(REG_DC_AGC_VAL, temp_value);
    }
    else
    {
        HI_REG_WRITE32(REG_DC_AGC_SET, HI_FALSE);
    }

    //5.���ò�����ʼ������ַ
    HI_REG_WRITE32(DC_RX_START_ADDR,pDcInput->ulStartAddr);//������ʼ��ַ
    HI_REG_WRITE32(DC_RX_END_ADDR,pDcInput->ulEndAddr); //����������ַ

    if ((EN_TRIG_INCREMENTAL_SEQUENCE == enDataSrcTemp) || (EN_TRIG_DESCENDING_SEQUENCE == enDataSrcTemp))
    {
        enDataSrcTemp = EN_TRIG_AD_12_BIT;
    }
    if ((EN_TRIG_LOGIC_32_BIT_LOGIC == enDataSrcTemp) || (EN_TRIG_LOGIC_16_HBIT_LOGIC == enDataSrcTemp))
    {
        enDataSrcTemp = EN_TRIG_LOGIC_16_LBIT_LOGIC;
    }

    //6.���ô������ͺ�����Դ
    if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
    {
        ctrl_tri_sel =0xff;//�ɼ�����ʱ����Դ��д0xff		
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL;//no care
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_NOT_TRI;//no care

    }
    else if(EN_TRIG_TYPE_MASK_DATA == pDcInput->enTrigerType)
    {

        ctrl_tri_sel = 0;//no care		  
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL;//no care
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_MASK_TRI;

    }
    else
    {
        ctrl_tri_sel = pDcInput->enTrigerType;//no care 	   
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_HIGH_LEVEL;
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_BUS_TRI;

    }
    ctrl_valid_ctrl  = 1;

    ctrl_reg_val|=(ctrl_tri_sel<<CAPTURE_CTRL_REG_TRI_SEL_BIT_ST);
    ctrl_reg_val|=(ctrl_valid_ctrl<<CAPTURE_CTRL_REG_VALID_CTRL_BIT_ST);
    ctrl_reg_val|=(ctrl_tri_type<<CAPTURE_CTRL_REG_TRI_TYPE_BIT_ST);
    ctrl_reg_val|=(ctrl_tri_mode<<CAPTURE_CTRL_REG_TRI_MODE_BIT_ST);
    ctrl_reg_val = ctrl_reg_val|((unsigned int)enDataSrcTemp<<CAPTURE_CTRL_REG_LADDR_BIT_ST);

    if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
    {
        HI_REG_WRITE32(DC_CAPTURE_CTRL,ctrl_reg_val);/*��������ԴΪAD; ä�ɣ�������*/
    }
    else
    {
        HI_REG_WRITE32(DC_DELAY_COUNT,pDcInput->ulAfterPointlen); //�������ٲ�pDcInput->ulAfterPointlen��С����
        HI_REG_WRITE32(DC_CAPTURE_CTRL,ctrl_reg_val);//����Ϊ�ߵ�ƽ����
    }  

}

HI_VOID DC_Data_CollectStart(HI_IN DC_INPUT_PARA_STRU *pDcInput)
{
    //7.��������
    if (pDcInput->enDataSrc == EN_TRIG_FFT_ORDER_32_BIT 
        || (pDcInput->enDataSrc == EN_TRIG_FFT_DISORDER_32_BIT)
        || (pDcInput->enDataSrc == EN_TRIG_IMP_18_BIT)
        || (pDcInput->enDataSrc == EN_TRIG_LOGIC_32_BIT_LOGIC))
    {
        if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
            HI_REG_WRITE32(DC_CONTROL,0x011); //��������,�ɼ��������Զ�ֹͣ,32bits����
        else
            HI_REG_WRITE32(DC_CONTROL,0x001); //��������,��������ڲ�ulAfterPointlenֹͣ��
    }
    else if(pDcInput->enDataSrc == EN_TRIG_LOGIC_16_HBIT_LOGIC)
    {
        if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
            HI_REG_WRITE32(DC_CONTROL,0x211); //��������,�ɼ��������Զ�ֹͣ,32bits����
        else
            HI_REG_WRITE32(DC_CONTROL,0x201); //��������,��������ڲ�ulAfterPointlenֹͣ��
    }
    else if (pDcInput->enDataSrc == EN_TRIG_INCREMENTAL_SEQUENCE 
        || (pDcInput->enDataSrc == EN_TRIG_DESCENDING_SEQUENCE))
    {
        if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
            HI_REG_WRITE32(DC_CONTROL, (0x111 | ((HI_U32)(pDcInput->enDataSrc - HI_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE) << HI_DRV_DATA_COLLECT_TEST_MODE_OFFSET)));
        else
            HI_REG_WRITE32(DC_CONTROL, (0x101 | ((HI_U32)(pDcInput->enDataSrc - HI_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE) << HI_DRV_DATA_COLLECT_TEST_MODE_OFFSET)));
    }
    else
    {
        if (EN_TRIG_TYPE_NOISE == pDcInput->enTrigerType)
            HI_REG_WRITE32(DC_CONTROL,0x111); //��������,�ɼ��������Զ�ֹͣ,��16bits��Ч���ݣ���16bitsΪ0
        else
            HI_REG_WRITE32(DC_CONTROL,0x101); //��������,��������ڲ�ulAfterPointlenֹͣ��
    }
}



HI_U32 DC_Data_CollectWaitResult(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_U32 *pWillWrieAddr,HI_BOOL *pOverTurn)
{

    HI_U32 ulTimeOutMs = pDcInput->ulWaitResultTimeOut;
    HI_U32 ulCurTimeMs=0;
    HI_U32 ulTempRegVal = 0;
    g_force_stop_dc=0;
    for(;;)
    {
        HI_REG_READ32(DC_RX_STATUS,ulTempRegVal);      
        if(ulTempRegVal&0x1)
        {
            break;
        }
        
        HI_MDM_Sleep(10);
        ulCurTimeMs+=10;
        if(ulCurTimeMs>=ulTimeOutMs||g_force_stop_dc)
        {
            //��ʱ
            HI_REG_WRITE32(DC_CONTROL,0x0);//ֹͣ����   
            g_force_stop_dc=0;
            return HI_ERR_TIMEOUT;
        }
    }
    g_force_stop_dc=0;
    
    //fifo���
    HI_REG_READ32(DC_RX_STATUS, ulTempRegVal);
    if (ulTempRegVal & HI_DRV_DATA_COLLECT_OVERFLOW)
    {
        return HI_ERR_COLLECT_DATA_OVERFLOW_ERR;
    }

    //�´�Ҫд���ڴ��ַ
    HI_REG_READ32(DC_RX_CURR_ADDR,*pWillWrieAddr);
    

    HI_REG_READ32(DC_RX_STATUS,ulTempRegVal);
    if((ulTempRegVal&HI_DRV_DATA_COLLECT_ADDR_CYCLE_IND)||(*pWillWrieAddr==pDcInput->ulStartAddr))
    {
        *pOverTurn = HI_TRUE;
    }
    else
    {
        *pOverTurn = HI_FALSE;
    }

    
    return HI_ERR_SUCCESS;
}

//���ز������
HI_U32 DC_Data_CollectGetResult(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf,HI_U32 ulWillWriteAddr,HI_BOOL bOverTurn)
{
    
    if(ulWillWriteAddr<pDcInput->ulStartAddr||ulWillWriteAddr>=pDcInput->ulEndAddr)
    {
        return HI_ERR_INVALID_RAM_ADDR;
    }

    //��ת�����������������ݾ���Ч
    if(bOverTurn)
    {
        //��һ��:ͨ������Ƿ���ַ�ж�ȷ����һ��������
        sBuf->bufAddr[0] = ulWillWriteAddr;
        sBuf->bufSize[0] = pDcInput->ulEndAddr-ulWillWriteAddr;


        //�ڶ���:������ʱ��0
        if(ulWillWriteAddr==pDcInput->ulStartAddr)
        {
            sBuf->bufAddr[1] = 0;
            sBuf->bufSize[1] = 0;
        }
        else
        {
            sBuf->bufAddr[1] = pDcInput->ulStartAddr;
            sBuf->bufSize[1] = ulWillWriteAddr-pDcInput->ulStartAddr;
        }


    }
    else
    {
        sBuf->bufAddr[0] = pDcInput->ulStartAddr;
        sBuf->bufSize[0] = ulWillWriteAddr - pDcInput->ulStartAddr;
        sBuf->bufAddr[1] = 0;
        sBuf->bufSize[1] = 0;
    }
    return HI_ERR_SUCCESS;
}

HI_VOID DC_Data_AagcRestore(HI_VOID)
{
    hi_u32 temp_value = 0;
	struct C_fix_aagc_gain *fix_aagc_gain = (struct C_fix_aagc_gain *)&temp_value;

	HI_REG_WRITE32(REG_DC_AGC_SET, (HI_U32)pnv_v200_is_fix_aagc_enable());//�ָ�����
	HI_REG_READ32(REG_DC_AGC_VAL, temp_value);

    fix_aagc_gain->fix_aagc_gain = (HI_U8)pnv_v200_get_fix_aagc_gain();
    HI_REG_WRITE32(REG_DC_AGC_VAL, temp_value);//�ָ�����
}

HI_VOID DC_Data_CollectRestore(HI_IN DC_INPUT_PARA_STRU *pDcInput)//�ָ�
{
    //9.�ָ��˲�
    //HI_MAC_OpenAfeRxHpf();//��AFE�Ľ��ո�ͨ�˲���
    drv_open_afe_rx_hpf();
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetOutPutVal(IO_EXP_NUM_BPF_SHORT, HI_IO_EXP_OUT_VAL_LOW);
#endif

	DC_Data_AagcRestore();
    if(pDcInput->enDataSrc!=EN_TRIG_DA_12_BIT)
    {
        drv_set_collect_data_flag(COLLECT_DATA_DISABLE);
    }

}


// TODO: �����ָ����̿����޸�Ϊ: ��һ��ȫ�ֱ�������bit��ʾ����Щ������Ҫ�ָ����ڻָ��ӿ��и��ݱ�־λ�ָ�
HI_VOID DC_Data_CollectForceRestore(HI_VOID)//�ָ�
{
    //HI_MAC_OpenAfeRxHpf();//��AFE�Ľ��ո�ͨ�˲���
    drv_open_afe_rx_hpf();
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetOutPutVal(IO_EXP_NUM_BPF_SHORT, HI_IO_EXP_OUT_VAL_LOW);
#endif
	DC_Data_AagcRestore();
    drv_set_collect_data_flag(COLLECT_DATA_DISABLE);
}
//*****************************************************************************
// ��������: DC_Data_Collect
// ��������: �����ɼ�
//
// ����˵��:
//           pDcInput�����ݲɼ��Ĳ���
//           sBuf�����ظ��û����ɼ�������ָ��ͳ��ȡ�
// �� �� ֵ:
//           �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE (HI_U32)(-1)��
//
//*****************************************************************************
HI_U32 DC_Data_Collect(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf,HI_DRV_DATA_COLLECT_STEP_E step)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulWillWriteAddr = 0;
    HI_BOOL bOverTurn = HI_FALSE;

    //1.������
    /*�ж��û���εĺϷ���*/
    if (HI_ERR_SUCCESS != dc_check_para(pDcInput))
    {
        return HI_ERR_FAILURE;
    }

    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_DC);
    
    //׼������
    if(HI_DRV_DATA_COLLECT_STEP_PREPARE&(HI_U32)step)
    {
        DC_Data_CollectPrepare(pDcInput);
    }

    
    //��ʼ����
    if(HI_DRV_DATA_COLLECT_STEP_START&(HI_U32)step)
    {
        (HI_VOID)DC_Data_CollectStart(pDcInput);
    }
    

    //�ȴ��������
    if(HI_DRV_DATA_COLLECT_STEP_GET_RESULT&(HI_U32)step)
    {
        ret = DC_Data_CollectWaitResult(pDcInput,&ulWillWriteAddr,&bOverTurn);
        if(ret!=HI_ERR_SUCCESS)
        {
            DC_Data_CollectRestore(pDcInput);
            HI_MDM_DataCacheInvalidate((HI_PVOID)pDcInput->ulStartAddr, (HI_S32)(pDcInput->ulEndAddr-pDcInput->ulStartAddr));
            HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_DC,0);
            return ret;
        }
        ret = DC_Data_CollectGetResult(pDcInput,sBuf,ulWillWriteAddr,bOverTurn);
        if(ret!=HI_ERR_SUCCESS)
        {
            DC_Data_CollectRestore(pDcInput);
            HI_MDM_DataCacheInvalidate((HI_PVOID)pDcInput->ulStartAddr, (HI_S32)(pDcInput->ulEndAddr-pDcInput->ulStartAddr));
            HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_DC,0);
            return ret;
        }
        
        DC_Data_CollectRestore(pDcInput);
        HI_MDM_DataCacheInvalidate((HI_PVOID)pDcInput->ulStartAddr, (HI_S32)(pDcInput->ulEndAddr-pDcInput->ulStartAddr));
    }

    
    //�ָ�Ĭ�ϼĴ�����MAC����
    if(HI_DRV_DATA_COLLECT_STEP_FORCE_RESTORE&(HI_U32)step)
    {
/*
                �������ˡ�z00193780
                ������ˡ�c00233580
                ��������������ο����ӿڣ��ͻ��п���ʹ�ø÷�֧���豣����
                ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/
        DC_Data_CollectForceRestore();
    }
    
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_DC,0);
    return ret;
}
#endif

