
#include "hi_low_power.h"
#include "hi_sal_cfg.h"
#include "app_mrs.h"
#include "mrs_prepare_test_mode.h"

//*****************************************************************************
// ��������: MRS_ChangeDiagUartConfig
// ��������: ˢ�µ��Դ�������
//
// ����˵��:
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ��޸Ĳ�����
//    ����������
//
// ����Ҫ��: TODO: ...�ڵ�����HI_MDM_UART_SetFreq�ӿں��ٵ��øýӿڣ�ʵ�ַ�Ƶֵ���������á�
// ���þ���: TODO: ...
//*****************************************************************************
HI_U32 MRS_RefreshDiagUartConfig(HI_VOID)
{
    HI_UART_INIT_TYPE_S initData;
    
    initData.baud_rate = 115200;
    initData.data_bits = 8;
    initData.stop_bits = 1;
    initData.parity = 0;
    
	return HI_MDM_UART_Ioctl(1, UART_CFG_SET_ATTR, (HI_U32)&initData);     
}

//*****************************************************************************
// ��������: MRS_PrepareTestMode
// ����˵��:
//          ulRate:�����õ�ҵ�񴮿ڲ�����
//          ucParity:�����õ�ҵ�񴮿���żУ��

// ��������: �������ģʽǰ׼�����ڵ����á�����uartʱ��Դ�����³�ʼ���������á�
//*****************************************************************************
HI_VOID MRS_PrepareUartConfig(HI_U32 ulRate, HI_U8 ucParity)
{
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_TEST_MODE);
    HI_MDM_UART_SetFreq(UART_CLK_APB);
    MRS_ChangeBaudRate(ulRate, ucParity);
    MRS_RefreshDiagUartConfig();
}

//*****************************************************************************
// ��������: MRS_RestoreFromTestMode
// ����˵��:
//          ulRate:ҵ�񴮿ڴ��ָ��Ĳ�����
//          ucParity:ҵ�񴮿ڴ��ָ�����żУ��

// ��������: �˳�����ģʽ�󣬻ָ����ڵ����á�
//*****************************************************************************
HI_VOID MRS_RestoreUartConfig(HI_U32 ulRate, HI_U8 ucParity)
{   
    HI_MDM_UART_SetFreq(UART_CLK_XTAL);
	MRS_ChangeBaudRate(ulRate, ucParity);
    MRS_RefreshDiagUartConfig();
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_TEST_MODE,0);
}

            