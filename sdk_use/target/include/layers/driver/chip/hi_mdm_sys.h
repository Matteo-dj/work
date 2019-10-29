/**@defgroup hct_sys ϵͳ
 * @ingroup drivers
 */

#ifndef __HI_MDM_SYS_H__
#define __HI_MDM_SYS_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX            1           /**< �û��Զ������ݴ�С         */

/**
* @ingroup hct_sys
 * ��ʾ�ڴ��С
 */
typedef enum
{
    HI_MEMORY_SIZE_UNKOWN = 0,
    HI_MEMORY_SIZE_2M = 0x200000,  //2M
    HI_MEMORY_SIZE_8M = 0x800000,  //8M
    HI_MEMORY_SIZE_16M= 0x1000000, //16M
    HI_MEMORY_SIZE_32M= 0x2000000, //32M
} HI_MEMORY_SIZE_E;

/**
* @ingroup hct_sys
 * ��ʾоƬ�汾��Ϣ
 */
typedef enum
{
    HI_CHIP_VERSION_UNKNOW  = 0,/**< δ֪оƬ�汾*/
    HI_CHIP_VERSION_V100,       /**< V100оƬ*/
    HI_CHIP_VERSION_V200,       /**< V200оƬ*/
}HI_HW_CHIP_VER_E;

/**
 *@ingroup hct_sys
 * PLCͨ��״̬�ṹ��
 */
typedef struct
{
    HI_U8 ucChlStatus;          /**<  ͨ��״̬��ȡֵ�ο�HI_MDM_CHL_STATUS_XX        */
    HI_U8 reserved[3];
}HI_MDM_CONNECT_STATUS_IND_S;

/**
 *@ingroup hct_sys
 * �������ݰ���ʽ
 */
typedef struct
{
    HI_U32 aulUser[HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX];       /**< �û��Զ�������         */
    HI_U32 ulDataSize;                      /**< ���δ����������            */
    HI_U8 aucData[0];                       /**< ���δ��������                 */
}HI_MDM_APPDATA_TRANSMIT_DL_S;

/**
 *@ingroup hct_sys
 * �������ݰ���ʽ
 */
typedef struct
{
    HI_U32 aulUser[HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX];   /**< �û��Զ�������         */
    HI_U32 ulDataSize;      /**< ���δ����������            */
    HI_U8 aucData[0];       /**< ���δ��������                 */
}HI_MDM_APPDATA_TRANSMIT_UL_S;

/**
 * @ingroup hct_sys
* @brief  MDM��Ϣ��ؽṹ��
*/
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];    /**< MAC��ַ      */
    HI_U8 aucNdmMacAddr[HI_PLC_MAC_ADDR_LEN];           /**< Ԥ����MDM�ڲ�ʹ���ֶΣ���ʼ��Ϊ0       */

    HI_U32 ulSn;                    /**<��Ϣ�������кţ��粻��Ҫ������0��ͨ��ģ��MDM�㲻�޸ĸ����ȡֵ��ֱ�ӷ��ظ�APP�㣬APP������ʶ���Ƿ�֡     */
    HI_U16 usTransId;           /**<����ID�ţ�ȡֵΪulObjId����Ч��Χ1~4����ǰ�̶�Ϊ1���ݲ�֧��2~4      */
    HI_U16 usControl  :15;      /**<Ԥ��  */
    HI_U16 usDULink   :1;       /**<��ʾ��Ϣ����HI_FALSE��ʾ���У�HI_TRUE��ʾ����  */
} HI_MDM_MSG_CTRL_S;

/**
 * @ingroup hct_sys
* @brief  �û���Ϣ����
*/
typedef struct
{
    HI_U16  usSwVer;        /**< ����汾�ţ�BCD���ʽ*/
    HI_U16  usYear  : 7;    /**< �汾����-�꣬BIN���ʽ*/
    HI_U16  usMonth : 4;    /**< �汾����-�£�BIN���ʽ*/
    HI_U16  usDay   : 5;    /**< �汾����-�գ�BIN���ʽ*/
    HI_CHAR szChipCode[2];  /**< оƬ���룬ASCII���ʽ*/
    HI_CHAR szManuCode[2];  /**< ���̴��룬ASCII���ʽ*/
    HI_U8   ucBootVer;      /**< Bootloader�汾��BIN���ʽ*/
    HI_U8   reserved[3];    /**< Ԥ���ֽ�*/
}HI_MDM_USER_INFO_S;

/**
* @ingroup  hct_sys
* @brief  MDM��ӦAPP��Ϣ�Ľӿں���ָ��
*
* @par  ����:MDM��ӦAPP��Ϣ�Ľӿں���ָ��
* @attention
* <ul><li></li>ʹ��֮ǰҪ��ע��</ul>
* @param  ulMdmMsgId [IN] ���� #HI_U32��MDM��Ϣ��ӦID��
* @param  pMdmMsg [IN] ���� #HI_PVOID����ʾAPP���յ�MDM��Ϣ������μ�HI_MDM_MSG_APPRCV_S���ݽṹ��
* @param  pData [IN] ���� #HI_PVOID����ʾAPP���MDM�������Ľ������ݣ���ѡ��
* @param  ulOption[IN] ���� #HI_PVOID��Ԥ����
* @retval   HI_ERR_SUCCESS��ʾ���óɹ���HI_ERR_FAILURE��ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm.h: �ļ�����������·�����ӿ�.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_APPMSG_F)(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption);

/**
 * @ingroup hct_sys
* @brief  APP����
*/
typedef struct
{
    HI_U32 ulObjId;                 /**<����ID��        */
    HI_MDM_APPMSG_F pfnAppMsgRcv;   /**<MDM��ӦAPP��Ϣ�Ľӿں�������        */
} HI_MDM_APPOBJ_S;



/**
* @ingroup  hct_sys
* @brief ��ȡӲ���ڴ��С��
*
* @par ����:
* ��ȡӲ���ڴ��С��
* @attention ��ǰ������Hi3911 V200���ͣ�V100��FPGA��Ҫ����ʵ�������ʵ�֡�

* @param �ޡ�

* @retval #HI_MEMORY_SIZE_E  Ӳ���ڴ��Сö�١�

* @par Dependency:
* @li hi_mdm_sys.h:�ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_MEMORY_SIZE_E  HI_MDM_GetHwMemSize(HI_VOID);

/**
* @ingroup  hct_sys
* @brief ��ȡӲ��оƬ�汾�š�
*
* @par ����:
* ��ȡӲ��оƬ�汾�š�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_HW_CHIP_VER_E������ֵ Ӳ��оƬ�汾��ö�١�
*
* @par Dependency:
* @li hi_mdm_sys.h:�ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_HW_CHIP_VER_E HI_MDM_GetHwChipVer(HI_VOID);

/**
* @ingroup  hct_sys
* @brief ��ȡӲ��оƬ���͡�
*
* @par ����:
* ��ȡӲ��оƬ���͡�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_FTM_CHIP_TYPE_E  Ӳ��оƬ����ö�١�

* @par Dependency:
* @li hi_mdm_sys.h:�ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_FTM_CHIP_TYPE_E HI_MDM_GetHwChipType(HI_VOID);

/**
* @ingroup  hct_sys
* @brief  ����ARM����ʱ��Ƶ�ʺ�APB����ʱ��Ƶ�ʶ�Ӧȫ�ֱ���(��ǰ��֧��)��
*
* @par ����:
* ����ARM����ʱ��Ƶ�ʺ�APB����ʱ��Ƶ�ʶ�Ӧȫ�ֱ���(�����üĴ���)��
* @attention
* @li �ڳ�ʼ���׶ε��ã�����HI_MDM_UART_SetFreq����������ʱ��֮ǰ��
* @li �ڿͻ������޸�оƬʱ�ӼĴ�����ʹ�ã��û��뱣֤����ȷ�ԡ�
*
* @param  ulAhb [IN] ���� #HI_U32  ARM����ʱ��Ƶ�ʡ�
* @param  ulApb [IN] ���� #HI_U32  APB����ʱ��Ƶ�ʡ�

* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
*
* @par Dependency:
* @li hi_mdm_sys.h:�ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetArmAndApbClock(HI_U32 ulAhb,HI_U32 ulApb);

/**
* @ingroup  hct_sys
* @brief ������ģʽд��������NV�С�
*
* @par ����:
* ������ģʽд��������NV�С�
* @attention 
* @li ��֧���ж��е��á�
* @li �ýӿ��ڹ�װ���׶�ʹ�ã�����ʱ��ģʽ����Ϊ����ģʽ�����Խ��������ó�����ģʽ��
* @ll ģ�������󣬻��NV�ж�ȡ����ģʽ��������ǰģʽ��Ϊ��ģʽ��
*
* @param  ulStatus [IN] Ҫд��Ĺ���ģʽ��ȡֵ:
* @li #HI_ND_SYS_STATUS_NORMAL:����ģʽ
* @li #HI_ND_SYS_STATUS_TEST:����ģʽ
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetWorkStatus(HI_U32 ulStatus);

/**
* @ingroup  hct_sys
* @brief  �������塣
*
* @par ����:
* �������塣
* @attention �ýӿ�����ʱ��������Ӱ�졣
*
* @param  enCause [IN] ����Id���ͻ�Ԥ��ID��Χ[0x9000,0xa000)������ID���ڲ鿴ϵͳ����ԭ��ʱʹ�á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_E enCause);

/**
* @ingroup  hct_sys
* @brief  ���������ӿڵ��ú󣬵���������ʱ�����ã���ʱ����ڱ���ϵͳ��Ϣ��
*
* @par ����:
* ���������ӿڵ��ú󣬵���������ʱ�����á���ʱ����ڱ���ϵͳ��Ϣ��
* @attention �ޡ�
*
* @param  ulWdgTimeCfg [IN] ����ʱ�䣬��λ:ms���������÷�Χ:2000-30000��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetRebootDelay(HI_U32 ulWdgTimeCfg);

/**
* @ingroup  hct_sys
* @brief  ��ʱ��������ʹ�ܡ�
*
* @par ����:
* ��ʱ��������ʹ�ܡ�
* @attention �ޡ�
*
* @param  enDelayCon [IN] ��ʱ�����¼�ID��ȡֵ��Χ[0,8)��
* @param  ms [IN] ��ʱ����ʱ��(��λ:ms)�����֧��5���ӡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstDelayEnable(HI_SYS_RST_DELAY_CON_E enDelayCon,HI_U32 ms);

/**
* @ingroup  hct_sys
* @brief  ȥ��ʱ������
*
* @par ����:
* ȥ��ʱ������
* @attention �ޡ�
*
* @param  enDelayCon [IN] ��ʱ�����¼�ID,ȡֵ��Χ[0,8)��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstDelayDisable (HI_SYS_RST_DELAY_CON_E enDelayCon);

/**
* @ingroup  hct_sys
* @brief  ������λע�ắ����
*
* @par ����:
* ������λע�ắ����
* @attention �ޡ�
*
* @param  pfnRstNotify [IN] ͳ������ѯ������ϵͳ�����Ե��øú�����
*��ѯͳ��������⵽ͳ��������Ԥ��ʱ��û�б仯����������λ��
* @param  enTrigerCon [IN] ������λ�¼�ID��

* @param  reserved [IN] ����ֵ���ݲ�ʹ�ã����øýӿ�ʱ��ֵΪ0���ɡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstRegisterNotify (HI_CONST HI_SYS_RST_NOTIFY_F pfnRstNotify, HI_SYS_RST_TRIGGER_CON_E enTrigerCon,HI_U32 reserved);

/**
* @ingroup  hct_sys
* @brief  ��ȡͨ��ģ��Ĳ�ƷID, ����MAC��ַ��оƬ���͡���Ʒ��̬����Ϣ��
*
* @par ����:
* оƬ���͡���Ʒ��̬�ȴ�NV���ж�ȡ�������ȡʧ�����оƬ�ļĴ����ж�ȡ������
* @attention �ޡ�
*
* @param  aucPlcMac[HI_PLC_MAC_ADDR_LEN] [OUT] ͨ��ģ���MAC��ַ  ��
* @param  penChipType [OUT] ͨ��ģ������оƬ����, ����Ҫ�ò�������HI_NULL��
* @param  penProductType [OUT] ͨ��ģ��Ķ��ƵĲ�Ʒ��̬, ����Ҫ�ò�������HI_NULL��
* @param  penHwPlatType [OUT] �������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_GetProductId(HI_U8 aucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_FTM_CHIP_TYPE_E* penChipType,HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType,HI_OUT HI_FTM_HW_PLATFORM_TYPE_E* penHwPlatType);

/**
* @ingroup  hct_sys
* @brief  ��ȡģ���оƬ���ͣ�����ͷ��оƬ���͡��ն�оƬ���͡�
*
* @par ����:
* оƬ�����ȴ�NV���ж�ȡ�������ȡʧ�����оƬ�ļĴ����ж�ȡ������
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_FTM_CHIP_TYPE_3911C ͷ��оƬ��
* @retval #HI_FTM_CHIP_TYPE_3911T �ն�оƬ��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_FTM_CHIP_TYPE_E HI_MDM_GetChipType(HI_VOID);

/**
* @ingroup  hct_sys
* @brief ����APP�汾��Ϣ��
*
* @par ����:
* �ú�������������ģ��汾��Ϣ���û�ʹ��HSO��ver������Բ鿴APP��ģ��汾��Ϣ��
* @attention �ýӿ�����APP��ʼ��ǰ���á�
*
* @param x [IN] ���� #HI_U8 ��Ӧ��x�汾������ռ��3byte��
* @param y [IN] ���� #HI_U8 ��Ӧ��y�汾������ռ��3byte��
* @param z [IN] ���� #HI_U8 ��Ӧ��z�汾������ռ��3byte��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_CONFIG_APP_VER(x,y,z)  HI_MDM_DfxSetVerInfo(HI_DFX_APP_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())

/**
* @ingroup  hct_sys
* @brief  ��ȡ��Ʒ�汾��Ϣ���� оƬ����, �汾�ŵ�. �ַ�����ʽ��
*
* @par ����:
* ��ȡ��Ʒ�汾��Ϣ���� оƬ����, �汾�ŵ�. �ַ�����ʽ��
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_PRODUCT_INFO_S*  ���͵�ַ �汾��Ϣ�ĵ�ַ ��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_CONST HI_PRODUCT_INFO_S* HI_MDM_GetProductVerInfo(HI_VOID);

/**
* @ingroup  hct_sys
* @brief  ��ȡboot�汾��
*
* @par ����: ��ȡboot�İ汾����Ϣ boot�汾��Ϊxx.yy.zz �ֱ��ӦacBootVer�ĳ�Ա0��1��2(��16����).
* @attention �ޡ�
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: �ļ���������ϵͳ��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_GetBootVer(HI_OUT HI_U8 acBootVer[3]);

/**
* @ingroup  hct_sys
* @brief  �����û���Ϣ��
*
* @par ����: �����û���Ϣ����������汾�ŵȡ�
* @attention �ޡ�
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: �ļ���������ϵͳ��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_SetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);

/**
* @ingroup  hct_sys
* @brief  ��ȡ�û���Ϣ��
*
* @par ����: ��ȡ�û���Ϣ����������汾�ŵȡ�
* @attention �ޡ�
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: �ļ���������ϵͳ��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_GetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);

/**
* @ingroup  hct_sys
* @brief  ���û�ȡϵͳ��Ϣ, ���� ϵͳ״̬���汾�ŵȡ�
*
* @par ����:
* ϵͳ״̬��Ϣ�в������ݲ��Ǿ�̬�ģ���Ҫʵʱ���øýӿڻ�ȡϵͳ״̬��Ϣ�Ա�ͬ����
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_MDM_SYS_INFO_S* ���͵�ַ ϵͳ��Ϣ�ĵ�ַ��
*
* @par Dependency:
* @li hi_mdm_sys.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_CONST HI_MDM_SYS_INFO_S* HI_MDM_GetSysInfo(HI_VOID);


#ifdef __cplusplus
}
#endif
#endif
