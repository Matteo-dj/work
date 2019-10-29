//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hi_errno.h
//  �� �� ��   : V1.0
//  ��������   : ���õĴ�����ֵ����
//  �����б�   : ��
//  �޸���ʷ   :
//
//*****************************************************************************

#ifndef __HI_ERRNO_H__
#define __HI_ERRNO_H__
//*****************************************************************************

#define  HI_ERR_FAILURE                                   (HI_U32)(-1)
#define  HI_ERR_SYSTEM                                    (HI_U32)(-1)


//*****************************************************************************
// ϵͳ�� [0-3000) , -1
//*****************************************************************************
#define  HI_ERR_SUCCESS                                   (0)
#define  HI_ERR_INVALID_PARAMETER                         (1)
#define  HI_ERR_INVALID_ID                                (2)
#define  HI_ERR_NO_INITILIZATION                          (3)
#define  HI_ERR_NOT_FOUND                                 (4)
#define  HI_ERR_FULL                                      (7)
#define  HI_ERR_INSUFFICIENT_BUFFER                       (8)
#define  HI_ERR_CONSUMED                                  (9)
#define  HI_ERR_CONTINUE                                  (10)
#define  HI_ERR_TOO_SMALL_BUFFER                          (11)
#define  HI_ERR_NOT_FREEE_SPACE                           (12)
#define  HI_ERR_NO_MORE_DATA                              (13)
#define  HI_ERR_MALLOC_FAILUE                             (15)
#define  HI_ERR_NOT_ENOUGH_MEMORY                         (16)
#define  HI_ERR_INVALID_HEAP_ADDR                         (17)
#define  HI_ERR_SHUTDOWN                                  (18) // δ�ܹػ�
#define  HI_ERR_NOT_CONNECT                               (19) // δ��ȷ����UE
#define  HI_ERR_BUSY                                      (20) // ϵͳæ����ʹ�ó�ͻ
#define  HI_ERR_INITILIZATION                             (21) // ��ʼ��ʧ��
#define  HI_ERR_POWER_ON_PROCESS                          (22) // ���ڴ�����������, ĳЩ��Դ������
#define  HI_ERR_SKIP                                      (23)
#define  HI_ERR_NOT_REGISTER                              (24)
#define  HI_ERR_TOO_LARGE_FRAME                           (25)
#define  HI_ERR_NO_OS_RES_INIT                            (26) // ���ڳ�ʼ��OS��Դ������, ĳЩ��Դ������
#define  HI_ERR_NO_INIT_APP_OS_RES                        (27)
#define  HI_ERR_INITILIZATION_ALREADY                     (28)
#define  HI_ERR_RAM_ALIGN                                 (30)
#define  HI_ERR_NOT_SUPPORT                               (40) //  ��֧��
#define  HI_ERR_NOT_EXIST                                 (41)
#define  HI_ERR_FEATURE_NOT_SUPPORT                       (42)
#define  HI_ERR_INVALID_CHIP_TYPE                         (43)
#define  HI_ERR_INVALID_PRODUCT_TYPE                      (44)
#define  HI_ERR_INVALID_PRODUCT_CONFIG                    (45)
#define  HI_ERR_WRITE_FLASH_FAILURE                       (46)
#define  HI_ERR_ACCESS_DENIED                             (47)
#define  HI_ERR_MEM_POOL_REQUEST_FAIL                     (48)
#define  HI_ERR_NOT_SUPPORT_ID                            (49)
#define  HI_ERR_UNAVAILABLE                               (50)
#define  HI_ERR_TIMEOUT                                   (51)
#define  HI_ERR_CFG_NOT_ALLOW                             (52)
#define  HI_ERR_BAD_DATA                                  (53)
#define  HI_ERR_HEAP_INIT_FAIL                            (54)
#define  HI_ERR_OSRES_INIT_FAIL                           (55)
#define  HI_ERR_TASK_INIT_FAIL                            (56)
#define  HI_ERR_ADDR_ACCESS_FAILURE                       (57) // invalid address to access.
#define  HI_ERR_INVALID_TYPE                              (58)
#define  HI_ERR_TOO_LARGE_DATA                            (59)
#define  HI_ERR_INVALID_CODE_ADDR                         (60)
#define  HI_ERR_INVALID_RAM_ADDR                          (61)
#define  HI_ERR_INVALID_PERIOD                            (62)
#define  HI_ERR_BAD_FRAME                                 (63)
#define  HI_ERR_BAD_FRAME_SIZE                            (64)
#define  HI_ERR_BAD_FRAME_PORT                            (65)
#define  HI_ERR_OBJ_NOT_FOUND                             (66)
#define  HI_ERR_NV_CFG_DISABLE                            (67)
#define  HI_ERR_EXIST                                     (68)
#define  HI_ERR_REGISTER_PARAM                            (69)
#define  HI_ERR_AUTH_INVALID                              (70)
#define  HI_ERR_CHECKSUM_INVALID                          (71)
#define  HI_ERR_PROTO_INVALID                             (72)
#define  HI_ERR_INTERRUPT                                 (73)
#define  HI_ERR_HW_PRODUCT_ID_NOT_EXIST                   (74)
#define  HI_ERR_NOT_ENOUGH_FD                             (75)
#define  HI_ERR_INVOKE_SEQUENCE                           (76)

#define  HI_ERR_LOADFILE_NEED_FIRST_BLOCK                 (104) // while bin file load,req need first block
#define  HI_ERR_DIAG_CMD_SIZE_INVALID                     (106)

#define  HI_ERR_QUEUE_FULL                                (300)
#define  HI_ERR_QUEUE_NOT_EXIST                           (301)
#define  HI_ERR_INVALID_TASK_ID                           (302)
#define  HI_ERR_INVALID_TASK_QUEUE                        (303)
#define  HI_ERR_QUEUE_EMPTY                               (304)
#define  HI_ERR_CREATE_SEM_FAIL                           (305)
#define  HI_ERR_EVT_EMPTY                                 (306)
#define  HI_ERR_WAIT_SEM_FAIL                             (307)
#define  HI_ERR_APP_CHL_UNKNOWN                           (410) // APPͨ���Ƿ�(�����Ҳ����Ϸ���оƬ���ͻ��ǲ�Ʒ��̬)
#define  HI_ERR_INVALID_TASK_PRIO                         (411)

#define  HI_ERR_NV_NOT_SUPPORT_ID                         (520) //  NV������""NVIM Not Exist"
#define  HI_ERR_NV_ERROR_READ                             (521) //  NV��ȡʧ��" "Read NVIM Failure"
#define  HI_ERR_NV_NOT_SUPPORT_LENTH                      (522) //  NVдʧ�ܣ����ȹ���""Write Error for Length Overflow"
#define  HI_ERR_NV_BAD_BLOCK                              (523) //  NVдʧ�ܣ�Flash����" "Write Error for Flash Bad Block"
#define  HI_ERR_NV_ERROR_WRITE                            (524) //  NVдʧ�ܣ���������" "Write Error for Unknown Reason"
#define  HI_ERR_NO_MORE_MEMORY	                          (558) // �ڴ治�� "No Memory"
#define  HI_ERR_OS_MSG_BODY_NULL                          (565) // ���յ�����Ϣ��ΪNULL(��Ӧ��ΪNULL)
#define  HI_ERR_NV_INVALID_INDEX                          (566)

#define  HI_ERR_FLASH_SYS_CALL                            (600)
#define  HI_ERR_UART_SYS_CALL                             (601)
#define  HI_ERR_FLASH_OPEN_FAIL                           (602)
#define  HI_ERR_FLASH_UP_TO_MAX_TIMES_WR_REG              (603)//�ﵽд״̬�Ĵ���������

#define  HI_ERR_QUEUE_NOT_OPEN                            (1000)
#define  HI_ERR_QUEUE_UNAVAILABLE                         (1001)
#define  HI_ERR_QUEUE_SHUTDOWN                            (1002)

#define  HI_ERR_NV_ITEM_IS_EMPTY                          (1100)
#define  HI_ERR_NV_FLASH_FULL                             (1103)
#define  HI_ERR_NV_NOT_SUPPORT_UPDATA                     (1104)
#define  HI_ERR_NV_NEED_BACK_FAIL                         (1105)
#define  HI_ERR_NV_INVALID_NVFILE                         (1106)
#define  HI_ERR_NV_INVALID_NVFILE_SECTOR_BAD              (1107)
#define  HI_ERR_NV_ITEM_TOO_LARGE                         (1108)

// ����������
#define  HI_ERR_SUM_ONLY_NVCOPY_OK                        (1200)
#define  HI_ERR_SUM_BACK_OK                               (1201) // ���˳ɹ�
#define  HI_ERR_SUM_BACK_FAIL                             (1202) // ����ʧ��
#define  HI_ERR_SUM_BACK_NO_VERSION                       (1203) // û�а汾�ɻ���
#define  HI_ERR_SUM_UPDATE_RESET_FAILUE                   (1204)
#define  HI_ERR_SUM_NOT_NETWORKED                         (1205) // δ�������
#define  HI_ERR_SUM_UPG_FILE_CRC_INCORRECT                (1206) //�����ļ�CRCУ��ʧ��
#define  HI_ERR_SUM_UPG_FILE_VRC_INCORRECT                (1207) //�����ļ�VRC�汾����
#define  HI_ERR_SUM_READ_BSN_NV_ERROR                     (1208) //��ȡBSN����NV��ʧ��
#define  HI_ERR_SUM_VER_CP_NEG_NOT_FORCE_UPG              (1209) //�汾���ͣ�����ǿ������
#define  HI_ERR_SUM_UPG_TIMEOUT                           (1210)
#define  HI_ERR_SUM_SOFT_RST_CAUSE_UPG_STOP               (1211)
#define  HI_ERR_SUM_INVALID_UPG_ID                        (1212) //ֹͣ����ID��ƥ��򲻴�����������
#define  HI_ERR_SUM_MEM_NOT_ENOUGH                        (1213) //�����ڴ治��
#define  HI_ERR_SUM_STA_LIST_NUM_ERROR                    (1214) //վ�����������Ԥ��(0,2047)
#define  HI_ERR_SUM_LIST_MEM_NOT_ENOUGH                   (1215) //�����ڴ治��(վ���б�)
#define  HI_ERR_SUM_LIST_CRC_ERROR                        (1216) //CRC��ʵ�ʼ��㲻һ��
#define  HI_ERR_SUM_LIST_NOT_MATCH_OPTION                 (1217) //�б�������ѡ�һ��
#define  HI_ERR_SUM_STA_AFFIRM_STATUS_NOT_MATCH           (1218) //ִ������ʱSTA״̬������Ԥ��
#define  HI_ERR_SUM_STA_START_STATUS_NOT_MATCH            (1219) //��������ʱSTA״̬������Ԥ��
#define  HI_ERR_SUM_NOT_SUPPORT_DEVICE_UPG                (1220) //����ģʽ��֧���豸����
#define  HI_ERR_SUM_GET_FILE_HEAD_INFO_FAIL               (1221) //��ȡ�����ļ�ͷ��Ϣʧ��
#define  HI_ERR_SUM_SAME_START_REQ                        (1222) //��ͬ��ʼ����֡
#define  HI_ERR_SUM_FAIL                                  (1225) //����
#define  HI_ERR_SUM_QRY_TIME_OUT_SAME_VRC                 (1230) //��ѯ��ʱ�汾����ͬ
#define  HI_ERR_SUM_QRY_TIME_OUT_NOT_SAME_VRC             (1231) //��ѯ��ʱ�汾�Ų�ͬ
#define  HI_ERR_SUM_QRY_TIME_OUT_NOT_HISI_FILE            (1232) //�Ǻ�˼�����ļ�

#define  HI_ERR_SUM_FILE_LEN_ERROR                        1233 /* �����ļ����ȳ������� */
#define  HI_ERR_SUM_OPTION_ERROR                          1234 /* ����ѡ�ƥ�� */

// DIAG
#define  HI_ERR_DIAG_PRINT_PARSER_FAIL                    (1300)
#define  HI_ERR_DIAG_PRINT_CAL_DATA_SIZE                  (1301)




#define HI_ERR_NV_BACKUP_FILE_FAIL                       (2001)
#define HI_ERR_NV_BACKUP_DATA_FAIL                       (2002)
#define HI_ERR_NV_WRITE_FILE_FAIL                        (2003)
#define HI_ERR_NV_UPDATA_DATA_FAIL                       (2004)
#define HI_ERR_NV_UPDATA_FILE_FAIL                       (2005)
#define HI_ERR_NV_NOT_SUPPORT_WRITE                      (2007)
#define HI_ERR_NV_FSEC_TOTAL_NUM_INVALID                 (2008)  // ����NV������Ƿ�
/**
 * @ingroup hct_errcode
 * NVģ��:У��NV�ļ�����,ulSectionDataSize��С����
 */
#define HI_ERR_NV_FILE_CHECK_SIZE_WRONG                  (2010)
/**
 * @ingroup hct_errcode
 * NVģ��:У��NV�ļ�����,ħ���ִ���
 */
#define HI_ERR_NV_FILE_CHECK_MAGIC_WRONG                 (2011)
/**
 * @ingroup hct_errcode
 * NVģ��:У��NV�ļ�����,CRC����
 */
#define HI_ERR_NV_FILE_CHECK_CRC_WRONG                   (2012)
/**
 * @ingroup hct_errcode
 * NVģ��:ˢ��FTM��NV�ļ�������ռ�ʧ��
 */
#define HI_ERR_NV_REFRESH_FTM_DATA_MALLOC_FAIL           (2013)





#define  HI_ERR_ACCOUNT_LOCKED                            (2100)//����У��ר�ã������ӿ�����
#define  HI_ERR_PASS_WORD_WRONG                           (2101)//����У��ר�ã������ӿ�����

#define  HI_ERR_SYSTEM_CALL_ERROR                         (2999)

//*****************************************************************************


//*****************************************************************************
// ������ [3000, 5000)
//*****************************************************************************

//CIPHER�����붨��
#define HI_ERR_CIPHER_DEV_BUSY		               (3000)
#define HI_ERR_CIPHER_DEV_IDLE                     (3001)
#define HI_ERR_CIPHER_DEV_CLOSE                    (3002)
#define HI_ERR_CIPHER_INVAL                        (3003)
#define HI_ERR_CIPHER_MEM_ALLOC                    (3004)//sys
#define HI_ERR_CIPHER_ADDRESS_INFORMAL             (3005)
#define HI_ERR_CIPHER_MEMLENGTH_NOT_MULTIPLE       (3006)
#define HI_ERR_CIPHER_INVALID_SUSPEND              (3007)//sys
#define HI_ERR_CIPHER_CACHE_ALIGN                  (3008)
#define HI_ERR_CIPHER_INVALID_HANDLE               (3009)
#define HI_ERR_CIPHER_INVALID_PARA                 (3010)
#define HI_ERR_CIPHER_FAILED_INIT                  (3011)
#define HI_ERR_CIPHER_FAILED_GETHANDLE             (3012)
#define HI_ERR_CIPHER_FAILED_RELEASEHANDLE         (3013)
#define HI_ERR_CIPHER_FAILED_CONFIGAES             (3014)
#define HI_ERR_CIPHER_FAILED_CONFIGDES             (3015)
#define HI_ERR_CIPHER_FAILED_ENCRYPT               (3016)
#define HI_ERR_CIPHER_FAILED_DECRYPT               (3017)
#define HI_ERR_CIPHER_BUSY                         (3018)
#define HI_ERR_CIPHER_EFUSE_WRT_BIT_FAIL           (3019)


//DMAC�����붨��
#define HI_ERR_DMAC_NOT_INIT                       (3100)
#define HI_ERR_DMAC_REPEAT_INIT                    (3101)
#define HI_ERR_DMAC_INVALID_POINT                  (3102)
#define HI_ERR_DMAC_INVALID_PARA                   (3103)
#define HI_ERR_DMAC_FAILED_INIT                    (3104)
#define HI_ERR_DMAC_FAILED_GETCHANNEL              (3105)
#define HI_ERR_DMAC_FAILED_MEMTRANS                (3106)
#define HI_ERR_DMAC_FAILED_PERTRANS                (3107)
#define HI_ERR_DMAC_FAILED_GETSTAT                 (3108)
#define HI_ERR_DMAC_FAILED_LISTRESET               (3109)
#define HI_ERR_DMAC_FAILED_ADDLIST                 (3110)
#define HI_ERR_DMAC_FAILED_LISTTRANS               (3111)

//GPIO�����붨��
#define HI_ERR_GPIO_GROUP_NUMBER_BEYOUND           (3201)
#define HI_ERR_GPIO_BIT_NUMBER_BEYOUND             (3202)
#define HI_ERR_GPIO_DIRECTION_NOT_MATCH            (3203)
#define HI_ERR_GPIO_DIRECTION_INVALID              (3204)
#define HI_ERR_GPIO_NOT_INIT                       (3205)
#define HI_ERR_GPIO_FAILED_INIT                    (3206)
#define HI_ERR_GPIO_REPEAT_INIT                    (3207)
#define HI_ERR_GPIO_POINTER_NULL                   (3208)
#define HI_ERR_GPIO_PARAMETER_ERROR                (3209)
#define HI_ERR_GPIO_PORT_OVERFLOW_FAILED           (3210)
#define HI_ERR_GPIO_INVALID_BUF                    (3211)
#define HI_ERR_GPIO_PIN_OVERFLOW_FAILED            (3212)
#define HI_ERR_GPIO_INTERRUPT_BIT_DISABLE_FAILED   (3213)
#define HI_ERR_GPIO_INTERRUPT_MODE_OVERFLOW_FAILED (3214)
#define HI_ERR_GPIO_INTERRUPT_MODE_FAILED          (3215)
#define HI_ERR_GPIO_CURR_PIN_BUSY                  (3216)
#define HI_ERR_GPIO_INVALID_SUSPEND                (3217)

// SPI �����붨��
#define HI_ERR_SPI_WRITE_TIMEOUT                   (3301)
#define HI_ERR_SPI_READ_TIMEOUT                    (3302)
#define HI_ERR_SPI_PARAMETER_WRONG                 (3303)

// CRC �����붨��
#define HI_ERR_CRC_BAD_PARA                        (3401)
#define HI_ERR_CRC_INVALID_SUSPEND                 (3402)

// LED ���������붨��
#define HI_ERR_LED_PARAMETER_ERROR                 (3501)


// WATCHDOG �����붨��
#define HI_ERR_WATCHDOG_PARA_ERROR                 (3601)

//FLASH ���������붨��
#define HI_ERR_FLASH_ADDR_BEYOND                   (3701)
#define HI_ERR_FLASH_WRITE_IN_BOOT                 (3702)
#define HI_ERR_FLASH_INVALID_SUSPEND               (3703)
#define HI_ERR_FLASH_WRITE_WRONG				   (3704)
#define HI_ERR_FLASH_READ_WRONG                    (3705)
#define HI_ERR_FLASH_DATA_CHECK_WRONG			   (3706)
#define HI_ERR_FLASH_MAX_SPI_OP                    (3707)
#define HI_ERR_FLASH_INVALID_SIZE                  (3708)

#define HI_ERR_ACCESS_PRIO_DENIED                  (3709)
#define HI_ERR_UFC_BLK_INVALID                     (3710)
#define HI_ERR_ACCESS_LIMIT                        (3711)



// UART ���������붨��
#define HI_ERR_UART_INVALID_SUSPEND                (3801)
#define HI_ERR_UART_INVALID_PARITY                      (3802)
#define HI_ERR_UART_INVALID_DATA_BITS               (3803)
#define HI_ERR_UART_INVALID_STOP_BITS               (3804)
#define HI_ERR_UART_INVALID_BAUD                        (3805)
#define HI_ERR_UART_INVALID_COM_PORT                (3806)

#define HI_ERR_DRV_UPG_NO_VERSION                  (3900)
#define HI_ERR_DRV_BLK_INVALID                     (3901)
#define HI_ERR_DRV_UPG_FILE_INVALID                (3902)
#define HI_ERR_DRV_UPG_CODE_FILE_INVALID           (3903)
#define HI_ERR_DRV_UPG_NV_FILE_INVALID             (3904)
#define HI_ERR_DRV_UPG_FLASH_BAD                   (3905)
#define HI_ERR_DRV_UPG_BUSY                        (3906)
#define HI_ERR_DRV_UPG_UPG_ID_INVALID              (3907)
#define HI_ERR_DRV_UPG_VERSION_OLDER               (3908)

//BQоƬ����������
#define HI_ERR_UPG_NOT_ALLOW                       (4000)

//˫��ͨ�Ŵ���
#define HI_ERR_ICC_SOCKET_ERR                      (4050)//ICC socket���� :���漰����ԭ��
#define HI_ERR_ICC_SOCKET_SEND                     (4051)//ICC socket����:���ʹ���
#define HI_ERR_ICC_SOCKET_RECV                     (4052)//ICC socke����:����
#define HI_ERR_ICC_SOCKET_CONNECT                  (4053)//ICC socket����:����
#define HI_ERR_ICC_CMD_NOT_FOUND                   (4055)//ICC ����:����δע�ᵽICCģ��
#define HI_ERR_ICC_CMD_NOT_SUPPORT                 (4056)//ICC ����:�����֧��
#define HI_ERR_ICC_TBL_FULL                        (4057)//ICC����:����ע��ʧ��
#define HI_ERR_ICC_BAD_DATA                        (4058)//ICC����:Ӧ�ò��жϿ���Ϊ�������
#define HI_ERR_FMGR_INITING                        (4059)
#define HI_ERR_ICC_CALL_API_FAIL                   (4060)//ICC����:���ýӿڴ���
#define HI_ERR_ICC_CLOSE_FAIL                      (4061)
#define HI_ERR_ICC_CLISE_VALID_SOCKET              (4062)
#define HI_ERR_ICC_NOT_INIT                        (4063)

#define HI_ERR_LOW_POWER_INVALID_PARAM          (4080)



// SD�� �����붨�� ��Χ(4100 -- 4200)
#define HI_ERR_FILE_MGR_OPEN_FAIL               (4100)
#define HI_ERR_FILE_MGR_READ_FAIL               (4101)
#define HI_ERR_FILE_MGR_WRITE_FAIL              (4102)
#define HI_ERR_FILE_MGR_CLOSE_FAIL              (4103)
#define HI_ERR_FILE_MGR_ACCESS_FAIL             (4104)
#define HI_ERR_FILE_MGR_LSEEK_FAIL              (4105)
#define HI_ERR_FILE_MGR_STAT_FAIL               (4106)
#define HI_ERR_FILE_MGT_STATFS_FAIL             (4107)

//*****************************************************************************
// RTC �����붨�� ��Χ(4300)
#define HI_ERR_RTC_ERR_READ                         (4300)
#define HI_ERR_RTC_ERR_WRITE                        (4301)
#define HI_ERR_RTC_ERR_SET                          (4302)
#define HI_ERR_RTC_ERR_INIT                         (4303)

//*****************************************************************************
// KEY �����붨�� ��Χ(4400)
#define HI_ERR_KEY_ERR_VALUE                         (4400)
#define HI_ERR_KEY_INVALID_CB                        (4401)
#define HI_ERR_KEY_ENABLE_INT                        (4402)

//*****************************************************************************
// RF �����붨�� ��Χ(4500)
#define HI_ERR_RF_CMD_ERR                            (4500)

//*****************************************************************************
// ���� �����붨�� ��Χ(4600)
#define HI_ERR_COLLECT_DATA_OVERFLOW_ERR             (4600)//оƬ��ʾ�������
#define HI_ERR_COLLECT_DATA_OVERFLOW_SW_CHECK        (4601)//����жϴ�����󳤶Ȳ������ó�������������󳤶�

//*****************************************************************************
// EFUSE �����붨�� ��Χ(4700)
#define HI_ERR_EFUSE_ALREADY_BURN                    (4700)
#define HI_ERR_EFUSE_READBACK                        (4701)

//*****************************************************************************
// RNG �����붨�� ��Χ(4800)
#define HI_ERR_RNG_NO_AVAILABLE_RNG                  (4800)
#define HI_ERR_RNG_LOW_OSA                           (4801)

//*****************************************************************************
// MAC�� [5000, 8000)
//*****************************************************************************
#define HI_ERR_NOT_FOUND_NETWORK                  	(5000) //NDC����δ�ҵ�����
#define HI_ERR_NDM_VER                            	(5001) //��ʾNDM�İ汾���ܺ�CCO/STA����
#define HI_ERR_MULTI_APP                          	(5002) //�û�����
#define HI_ERR_STOP_SEARCH_NETWORK  			    (5004) //����ֹͣ
#define HI_ERR_MAC_SNID_CHANGING                    (5005)
#define HI_ERR_TF_IDENTIFY_DOING                 	(5006) //��̨��ʶ����
#define HI_ERR_SLOT_TIME_NOT_ARRIVAL                (5007)


/**
 * @ingroup hct_errcode
 * OSA����:�����Ƿ�
 */
#define HI_ERR_TASK_INVALID_PARAM   (6000)
/**
 * @ingroup hct_errcode
 * OSA����:��������ʧ��(OS)
 */
#define HI_ERR_TASK_CREATE_FAIL     (6001)
/**
 * @ingroup hct_errcode
 * OSA����:ɾ������ʧ��(OS)
 */
#define HI_ERR_TASK_DELETE_FAIL     (6002)
/**
 * @ingroup hct_errcode
 * OSA����:��������ʧ��(OS)
 */
#define HI_ERR_TASK_SUPPEND_FAIL    (6003)
/**
 * @ingroup hct_errcode
 * OSA����:�ָ�����ʧ��(OS)
 */
#define HI_ERR_TASK_RESUME_FAIL     (6004)
/**
 * @ingroup hct_errcode
 * OSA����:��ȡ�������ȼ�ʧ��(OS)
 */
#define HI_ERR_TASK_GET_PRI_FAIL    (6005)
/**
 * @ingroup hct_errcode
 * OSA����:�����������ȼ�ʧ��(OS)
 */
#define HI_ERR_TASK_SET_PRI_FAIL    (6006)
/**
 * @ingroup hct_errcode
 * OSA����:������ʧ��(δʹ��)
 */
#define HI_ERR_TASK_LOCK_FAIL       (6007)
/**
 * @ingroup hct_errcode
 * OSA����:��������ʧ��(δʹ��)
 */
#define HI_ERR_TASK_UNLOCK_FAIL     (6008)
/**
 * @ingroup hct_errcode
 * OSA����:sleepʧ��(os)
 */
#define HI_ERR_TASK_DELAY_FAIL      (6009)
/**
 * @ingroup hct_errcode
 * OSA����:��ȡ������Ϣʧ��(OS)
 */
#define HI_ERR_TASK_GET_INFO_FAIL   (6010)
/**
 * @ingroup hct_errcode
 * OSA����:ע��������Ȼص�����ʧ��(OS)
 */
#define HI_ERR_TASK_REGISTER_SCHEDULE_FAIL  (6011)


/**
 * @ingroup hct_errcode
 * OSA��Ϣ:�����Ƿ�
 */
#define HI_ERR_MSG_INVALID_PARAM    (6020)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:������Ϣ����ʧ��(OS)
 */
#define HI_ERR_MSG_CREATE_Q_FAIL    (6021)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:δʹ��
 */
#define HI_ERR_MSG_DELETE_Q_FAIL    (6022)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:�ȴ���Ϣʧ��(OS)
 */
#define HI_ERR_MSG_WAIT_FAIL        (6023)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:������Ϣʧ��(OS)
 */
#define HI_ERR_MSG_SEND_FAIL        (6024)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:δʹ��
 */
#define HI_ERR_MSG_GET_Q_INFO_FAIL  (6025)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:ɾ����Ϣ����ʧ��(OS)
 */
#define HI_ERR_MSG_Q_DELETE_FAIL    (6026)
/**
 * @ingroup hct_errcode
 * OSA��Ϣ:�ȴ���Ϣ��ʱ(OS)
 */
#define HI_ERR_MSG_WAIT_TIME_OUT    (HI_ERR_TIMEOUT)


/**
 * @ingroup hct_errcode
 * OSA�ź���:�����Ƿ�
 */
#define HI_ERR_SEM_INVALID_PARAM    (6040)
/**
 * @ingroup hct_errcode
 * OSA�ź���:����ʧ��
 */
#define HI_ERR_SEM_CREATE_FAIL      (6041)
/**
 * @ingroup hct_errcode
 * OSA�ź���:ɾ��ʧ��
 */
#define HI_ERR_SEM_DELETE_FAIL      (6042)
/**
 * @ingroup hct_errcode
 * OSA�ź���:�ȴ�ʧ��
 */
#define HI_ERR_SEM_WAIT_FAIL        (6043)
/**
 * @ingroup hct_errcode
 * OSA�ź���:�ͷ�ʧ��
 */
#define HI_ERR_SEM_SIG_FAIL         (6044)
/**
 * @ingroup hct_errcode
 * OSA�ź���:�ȴ���ʱ
 */
#define HI_ERR_SEM_WAIT_TIME_OUT    (HI_ERR_TIMEOUT)



/**
 * @ingroup hct_errcode
 * OSA�жϹ���:�����Ƿ�
 */
#define HI_ERR_ISR_INVALID_PARAM    (6060)
/**
 * @ingroup hct_errcode
 * OSA�жϹ���:ע���ж�ʧ��(OS)
 */
#define HI_ERR_ISR_REQ_IRQ_FAIL     (6061)
/**
 * @ingroup hct_errcode
 * OSA�жϹ���:δʹ��(OS)
 */
#define HI_ERR_ISR_ADD_JOB_MALLOC_FAIL (6062)
/**
 * @ingroup hct_errcode
 * OSA�жϹ���:����жϺ���ʧ��(OS)
 */
#define HI_ERR_ISR_ADD_JOB_SYS_FAIL (6063)



/**
 * @ingroup hct_errcode
 * OSA�¼�����:�����Ƿ�
 */
#define HI_ERR_EVENT_INVALID_PARAM      (6080)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:����EVENTʧ��(���㹻handle)
 */
#define HI_ERR_EVENT_CREATE_NO_HADNLE   (6081)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:����EVENTʧ��(OS����)
 */
#define HI_ERR_EVENT_CREATE_SYS_FAIL    (6082)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:дEVENTʧ��(OS����)
 */
#define HI_ERR_EVENT_SEND_FAIL          (6083)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:��EVENTʧ��(OS����)
 */
#define HI_ERR_EVENT_WAIT_FAIL          (6084)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:���EVENTʧ��(OS����)
 */
#define HI_ERR_EVENT_CLEAR_FAIL         (6086)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:EVENTģ���ظ���ʼ��
 */
#define HI_ERR_EVENT_RE_INIT            (6087)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:EVENT��ʼ��ʧ�ܣ����㹻�ڴ�ռ䴴��EVENT��Դ
 */
#define HI_ERR_EVENT_NOT_ENOUGH_MEMORY  (6088)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:EVENTģ��δ��ʼ��
 */
#define HI_ERR_EVENT_NOT_INIT           (6089)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:ɾ��EVENTʧ��(OS)
 */
#define HI_ERR_EVENT_DELETE_FAIL        (6090)
/**
 * @ingroup hct_errcode
 * OSA�¼�����:��EVENTʧ��(��ʱ)
 */
#define HI_ERR_EVENT_WAIT_TIME_OUT      HI_ERR_TIMEOUT



/**
 * @ingroup hct_errcode
 * OSA�ڴ����:�����Ƿ�
 */
#define HI_ERR_MEM_INVALID_PARAM                    (6100)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:�����ڴ�ʧ��(OS����)
 */
#define HI_ERR_MEM_CREAT_POOL_FAIL                  (6101)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:�����ڴ�ʧ��(����֧���ڴ�ظ���)
 */
#define HI_ERR_MEM_CREATE_POOL_NOT_ENOUGH_HANDLE    (6102)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:�ͷ��ڴ�ʧ��
 */
#define HI_ERR_MEM_FREE_FAIL                        (6103)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:�ظ���ʼ��
 */
#define HI_ERR_MEM_RE_INIT                          (6104)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:δ��ʼ��
 */
#define HI_ERR_MEM_NOT_INIT                         (6105)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:δʹ��
 */
#define HI_ERR_MEM_CREAT_POOL_MALLOC_FAIL           (6106)
/**
 * @ingroup hct_errcode
 * OSA�ڴ����:��ȡ�ڴ����Ϣ����
 */
#define HI_ERR_MEM_GET_INFO_FAIL                    (6107)


/**
 * @ingroup hct_errcode
 * OSAϵͳ��ʱ��:�����Ƿ�
 */
#define HI_ERR_TIMER_INVALID_PARAM      (6120)
/**
 * @ingroup hct_errcode
 * OSAϵͳ��ʱ��:������ʱ������
 */
#define HI_ERR_TIMER_CREATE_TIMER_FAIL  (6121)




/**
 * @ingroup hct_errcode
 * OSACPUռ����&�켣:�����Ƿ�
 */
#define HI_ERR_CPU_STAT_INVALID_PARAM               (6140)
/**
 * @ingroup hct_errcode
 * OSACPUռ����&�켣:û���㹻�ڴ�ռ�
 */
#define HI_ERR_CPU_STAT_NOT_ENOUGH_MEMORY           (6141)
/**
 * @ingroup hct_errcode
 * OSACPU�켣:�ظ���ʼ
 */
#define HI_ERR_CPU_STAT_TRACK_RESTART               (6142)
/**
 * @ingroup hct_errcode
 * OSACPU�켣:�ظ�ֹͣ
 */
#define HI_ERR_CPU_STAT_TRACK_RESTOP                (6143)
/**
 * @ingroup hct_errcode
 * OSACPU�켣:δ����
 */
#define HI_ERR_CPU_STAT_TRACK_NO_INIT               (6144)
/**
 * @ingroup hct_errcode
 * OSACPU�켣:Ҫ��¼��Ŀ���������¼��Ŀ����
 */
#define HI_ERR_CPU_STAT_TRACK_TOO_LARGE_COUNT       (6145)
/**
 * @ingroup hct_errcode
 * OSACPUռ����:CPUռ���ʹ����ظ���ʼ
 */
#define HI_ERR_CPU_STAT_PERCENT_RESTART             (6150)
/**
 * @ingroup hct_errcode
 * OSACPUռ����:CPUռ���ʹ����ظ�ֹͣ
 */
#define HI_ERR_CPU_STAT_PERCENT_RESTOP              (6151)
/**
 * @ingroup hct_errcode
 * OSACPUռ����:CPUռ���ʹ���δ����
 */
#define HI_ERR_CPU_STAT_PERCENT_NO_INIT             (6152)



#define HI_ERR_SHARE_TASK_INVALID_PARAM             (6200)
#define HI_ERR_SHARE_TASK_NOT_ENOUGH_MODULE_ID      (6201)
#define HI_ERR_SHARE_TASK_INVALID_MSG_ID            (6202)

#define HI_ERR_BATTERY_NOT_SUPPORT                  (6250)
#define HI_ERR_BATTERY_INVALID_PARAM                (6251)
#define HI_ERR_BATTERY_NOT_INIT                     (6252)
//*****************************************************************************


//*****************************************************************************
// Ӧ�ò� [8000, 10000)
//*****************************************************************************
// TODO:
//*****************************************************************************


//*****************************************************************************
// ����Ϊ��չ
//*****************************************************************************
#define HI_ERR_CCO_DATA  (10000) 

// TODO:
//*****************************************************************************


//*****************************************************************************
// ������Ŀǰ���Ϊ 0xFFFF
#define HI_ERR_MAX  0xFFFF
//*****************************************************************************



//*****************************************************************************
#endif // __HI_ERRNO_H__

