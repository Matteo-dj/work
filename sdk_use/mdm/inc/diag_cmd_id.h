//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : diag_cmd_id.h
//  �� �� ��   : V1.0 
//  ��    ��   : ���� 0013042 
//  ��������   : 2008��07��07��
//  ��������   : ���ļ�������DIAGϵͳ����ID���������������
//               CAUTION:
//               This file is generated by RDE(HiStudio Tool at PC side), 
//               PLEASE DON'T change this file by hand.
//                
//  �����б�   : ��
//  �޸���ʷ   : 
//  1.��    �� : 2008��07��07��
//    ��    �� : RDE 
//    �޸����� : �����ļ� 
//  2.��    �� : 2009��11��22��
//    ��    �� : RDE 
//    �޸����� : 
//               ɾ������DIAG_CMD_LTE_CSQ_IND�����޸�ͬ����������ID
//  3.��    �� : 2009��11��23��
//    ��    �� : RDE 
//    �޸����� : 
//               1������SIM����������
//               2��ɾ��DIAG_CMD_GET_USIMM_INFO����
//               3��ɾ��DIAG_CMD_LTE_SET_BEARER_TYPE
//    
//*****************************************************************************

#ifndef __DIAG_CMD_ID_H__
#define __DIAG_CMD_ID_H__

//*****************************************************************************
// DIAG ����ID����
// 
// DIAG CMD����
// 
// DSP ͸��Ӧ�� : [0x0000, 0x1000)
// PS  : [0x1000, 0x3000)
// BSP : [0x3000, 0x3500)
// ��չ: [0x3500, 0x5000)
// MSP : [0x5000, 0xFFFF)
// DSPҵ��Ӧ��: [0xFFFF, 0x00010F00)
// ��չ: [0x00010F00, 0xFFFFFFFF)
// 
//*****************************************************************************
 
#define DIAG_CMD_DSP_TRANS_BASE_ID      (0x00000000) // DSP 
#define DIAG_CMD_PS_BASE_ID             (0x00001000) // PS 
#define DIAG_CMD_BSP_BASE_ID            (0x00003000) // BSP 
#define DIAG_CMD_MSP_BASE_ID            (0x00005000) // MSP  
#define DIAG_CMD_DSP_CT_BASE_ID         (0x00010000)
#define DIAG_CMD_APP_BASE_ID            (0x00010F00) // DSP 
#define DIAG_CMD_END                    (0xFFFFFFFF)

#define DIAG_IS_DSP_CMD(x)    (x>=DIAG_CMD_DSP_TRANS_BASE_ID && x<DIAG_CMD_PS_BASE_ID) 
#define DIAG_IS_PS_CMD(x)     (x>=DIAG_CMD_PS_BASE_ID  && x<DIAG_CMD_BSP_BASE_ID) 
#define DIAG_IS_BSP_CMD(x)    (x>=DIAG_CMD_BSP_BASE_ID && x<DIAG_CMD_MSP_BASE_ID) 
#define DIAG_IS_MSP_CMD(x)    (x>=DIAG_CMD_MSP_BASE_ID && x<DIAG_CMD_DSP_CT_BASE_ID) 
#define DIAG_IS_CT_DSP_CMD(x) (x>=DIAG_CMD_DSP_CT_BASE_ID && x<DIAG_CMD_APP_BASE_ID) 
//*****************************************************************************

#define DIAG_L1_A_ID_MIN  0x0050  //[0x0050, 0x1000)
#define DIAG_L1_A_ID_MAX  0x1000  
#define DIAG_MAC_A_ID_MIN 0x1000 // [0x1000, 0x2000)
#define DIAG_MAC_A_ID_MAX 0x2000  

#define DIAG_L1_B_ID_MIN  0x2100  // [0x2100, 0x2900)
#define DIAG_L1_B_ID_MAX  0x2900  
#define DIAG_MAC_B_ID_MIN 0x2900 // [0x2900, 0x3000)
#define DIAG_MAC_B_ID_MAX 0x3000  

#define DIAG_APP_A_ID_MIN  0x5600  // [0x5600, 0x5E00)
#define DIAG_APP_A_ID_MAX  0x5E00  

#define DIAG_APP_B_ID_MIN  0x5E00  // [0x5E00, 0x6600)
#define DIAG_APP_B_ID_MAX  0x6600  

#define DAIG_SAL_ID_MIN    0x7000
#define DIAG_SAL_ID_MAX    0x8000


//*****************************************************************************
// һ����ʼ����ϵͳ��ص�DIAG���� [0x5000, 0x5200)
//*****************************************************************************
#define DIAG_CMD_HOST_CONNECT                             (DIAG_CMD_HOST_CONNECT_FOR_LOG)
#define DIAG_CMD_HOST_DISCONNECT                          (DIAG_CMD_HOST_DISCONNECT_FOR_LOG)
#define DIAG_CMD_UE_ALIVE_SPY                             (0x5002)
#define DIAG_CMD_HOST_TEST                                (0x5003)
#define DIAG_CMD_HOST_DEVICE_INFO                         (0x5004)
#define DIAG_CMD_SET_AT_PHY_CHANNEL                       (0x5005)  // ����AT����ͨ��,ͨ��DIAG��������DIAGͨ��ģʽ
#define DIAG_CMD_MODE_RANK                                (0x5006)
#define DIAG_CMD_STOP                                     (0x5007)  // DIAG�����ֹͣ��ȡ�� 
#define DIAG_CMD_PORT_TEST                                (0x5008)
#define DIAG_CMD_LOG_INIT                                 (0x5009)
#define DIAG_CMD_SET_SDM_INFO                             (0x500A)
#define DIAG_CMD_CHANNEL_CONFIG                           (0x500B)
#define DIAG_CMD_CHANNEL_USR1                             (0x500C) // CTS
#define DIAG_CMD_CHANNEL_USR2                             (0x500D)

#define DIAG_CMD_SIM_INIT_STATUS_IND                      (0x5050)
#define DIAG_CMD_UE_POWERON_IND                           (0x5051)
#define DIAG_CMD_UE_POWEROFF_IND                          (0x5052)
#define DIAG_CMD_TIMER_OUT_IND                            (0x5053)
#define DIAG_CMD_UE_SYS_STATUS_IND                        (0x5054)
#define DIAG_CMD_PORT_TEST_IND                            (0x5055)
#define DIAG_CMD_VCOM_SPY                                 (0x5056)
#define DIAG_CMD_LOG_IN_PWD_CHECK_RESULT                  (DIAG_CMD_LOG_IN_PWD_CHECK_RESULT_FOR_LOG)//��������У����                 
//*****************************************************************************


//*****************************************************************************
// �����豸������ָ�� [0x5200, 0x5300)
//*****************************************************************************
#define DIAG_CMD_POWER_OFF                                (0x5200) // �ػ� 
#define DIAG_CMD_UE_RST                                   (0x5201) // �����豸
#define DIAG_CMD_POWER_INFO_IND                           (0x5202)
#define DIAG_CMD_SCPU_START                               (0x5203) // ������CPU(����DSP) 

// UE�Ķ�д��������ڴ桢�Ĵ�����Flash�����衣
#define DIAG_CMD_MEM_RD                                   (0x5203)
#define DIAG_CMD_MEM_WR                                   (0x5204)
#define DIAG_CMD_REG_RD								      (0x5205)
#define DIAG_CMD_REG_WR                                   (0x5206)
#define DIAG_CMD_GPIO_RD                                  (0x5207)
#define DIAG_CMD_GPIO_WR                                  (0x5208)
#define DIAG_CMD_MEM_DUMP                                 (0x5209) // ����DUMP
#define DIAG_CMD_MEM_DUMP_IND                             (0x520A)
#define DIAG_CMD_M                                        (0x5220) // M
#define DIAG_CMD_D                                        (0x5221) // D

// DIAG_CMD_SW_VER_QRY:   �����汾��Ϣ�ɹ۲��DIAG����
// DIAG_CMD_HW_VER_QRY:   Ӳ���汾��Ϣ�ɹ۲��DIAG����
// DIAG_CMD_IMEI_QRY:     ��ѯIMEI��
// DIAG_CMD_HW_INFO_QRY:  �豸�����Բ�Ʒ�汾��Ϣ
// DIAG_CMD_SOC_INFO_QRY: ��ѯоƬ�İ汾
#define DIAG_CMD_SW_VER_QRY                               (0x520B)
#define DIAG_CMD_HW_VER_QRY                               (0x520C)
#define DIAG_CMD_IMEI_QRY                                 (0x520D)
#define DIAG_CMD_HW_INFO_QRY                              (0x520E)
#define DIAG_CMD_SOC_INFO_QRY                             (0x520F)
#define DIAG_CMD_BUF_CFG                                  (0x5210) // DIAG_CMD_BUF_CFG    : LOG��������ͨ����������, Ŀ����MUX�洢��������
#define DIAG_CMD_GETBUF_CFG                               (0x5211) // DIAG_CMD_GETBUF_CFG :  ��ȡ��������ͨ���������� 
#define DIAG_CMD_GET_UNLOCK_CODE                          (0x5212)
//*****************************************************************************


//*****************************************************************************
// ������־���� [0x5300, 0x5400)
//*****************************************************************************

// ��־���IND
#define DIAG_CMD_LOG_IND                                  (0x5300)
#define DIAG_CMD_LOG_PRINT_TXT_IND                        (0x5301)
#define DIAG_CMD_LOG_PRINT_BIN_IND                        (0x5302)
#define DIAG_CMD_LOG_PRINT_RAW_TXT_IND                    (0x5303)
#define DIAG_CMD_LOG_AIR_IND                              (0x5304)
#define DIAG_CMD_LOG_LAYER_IND                            (0x5305)
#define DIAG_CMD_LOG_USERPLANE_IND                        (0x5306)
#define DIAG_CMD_LOG_EVENT_IND                            (0x5307)
#define DIAG_CMD_LOG_EVENT_EX_IND                         (0x5308)
#define DIAG_CMD_LOG_DSP_IND                              (0x5309)
#define DIAG_CMD_LOG_MSG_IND                              (0x530A) 
#define DIAG_CMD_LOG_LTE_L1_IND                           (0x530B) 

// CAT����������(0x2100+...)
#define DIAG_CMD_LOG_CAT_PRINT                            (0x5310)
#define DIAG_CMD_LOG_CAT_LAYER                            (0x5311)
#define DIAG_CMD_LOG_CAT_AIR                              (0x5312)
#define DIAG_CMD_LOG_CAT_USERPLANE                        (0x5313)
#define DIAG_CMD_LOG_CAT_L1                               (0x5314)
#define DIAG_CMD_LOG_CAT_EVENT                            (0x5315)
#define DIAG_CMD_LOG_CAT_CMD                              (0x5316)
#define DIAG_CMD_LOG_CAT_CFG                              (0x5317)
#define DIAG_CMD_VER_QRY                                  (0x5318)
#define DIAG_CMD_SYS_QRY                                  (0x5319)
#define DIAG_CMD_OS_T_QRY                                 (0x5321)
#define DIAG_CMD_SYSE_QRY_R                               (0x5323)
#define DIAG_CMD_DCHL_CTX_QRY                             (0x5324)
#define DIAG_CMD_SYS_END                                  (0x5325)
#define DIAG_CMD_VER_S_QRY                                (0x5330)
#define DIAG_CMD_CHL_ADBGM                                (0x5331)
#define DIAG_CMD_CHL_DDBGM                                (0x5332)
#define DIAG_CMD_CHL_SET                                  (0x5333)
#define DIAG_CMD_CHL_SET_END                              (0x5338)

//*****************************************************************************


//*****************************************************************************
// �ġ���ά�ɲ����� [0x5400, 0x5500)
//*****************************************************************************
#define DIAG_CMD_GET_CPU_INFO                             (0x5400)

#define DIAG_CMD_HSO_AT_SWT                               (0x5450)
#define DIAG_CMD_HSO_AT                                   (0x5451)
#define DIAG_CMD_MSP_SDM_QURY                             (0x5452)
#define DIAG_CMD_MSP_SDM_QURY_IND                         (0x5453)
#define DIAG_CMD_GTR_SET                                  (0x5454)
//*****************************************************************************


//*****************************************************************************
// �塢NV��д���� [0x5500, 0x5600)
//*****************************************************************************
#define DIAG_CMD_NV_RD                                    (0x5500)
#define DIAG_CMD_NV_WR                                    (0x5501)
#define DIAG_CMD_NV_RD_IND                                (0x5502)
#define DIAG_CMD_NV_QRY                                   (0x5503)
#define DIAG_CMD_NV_IMPORT                                (0x5504)
#define DIAG_CMD_NV_EXPORT                                (0x5505)
#define DIAG_CMD_NV_EXPORT_IND                            (0x5506)
#define DIAG_CMD_NV_DEL                                   (0x5507)
#define DIAG_CMD_NV_FILE_LOAD                             (0x5508)
#define DIAG_CMD_NV_FILE_UPDATE                           (0x5509)
#define DIAG_CMD_NV_FILE_EXPORT                           (0x550A)
#define DIAG_CMD_NV_UPDATE                                (0x550B)
#define DIAG_CMD_NV_BACK                                  (0x550C)
#define DIAG_CMD_NV_REFRESH                               (0x550D)
#define DIAG_CMD_NV_CHANGESYSMODE                         (0x550E)
//*****************************************************************************


#endif // __DIAG_CMD_ID_H__