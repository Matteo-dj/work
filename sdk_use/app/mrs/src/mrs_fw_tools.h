//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_tools.h
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2011-12-08
//  ��������   : ����֡��ʽ��Ҫ��һЩ����С���� 
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-08
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#ifndef __MRS_FW_PROTOTOOLS_H__
#define __MRS_FW_PROTOTOOLS_H__
HI_START_HEADER

#include "hi_usr_nv.h"
#include "Hi_ft_nv.h"
#include "hi_stdlib.h"


#define MRS_TOOLS_ARRAY_COUNT(arr)  (HI_ARRAY_COUNT(arr))
#define MRS_TOOLS_ALEN(x)           (HI_ARRAY_COUNT(x))

#define MRS_NOT_USED(p)             ((p) = (p))

#define MRS_MAX(a,b)                HI_MAX((a), (b))
#define MRS_MIN(a,b)                HI_MIN((a), (b))

#define MRS_ABS_DIFF(a, b)          (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

#define MRS_COMBIN16(a,b)           HI_MAKEU16((a), (b))
#define MRS_COMBIN32(a,b,c,d)       HI_MAKEU32(HI_MAKEU16((a), (b)), HI_MAKEU16((c), (d)))

#define MRS_LOCK_INIT(x)     ((x) = 0)            //��ʼ����
#define MRS_LOCK(x)          ((x) = 1)            //����
#define MRS_UNLOCK(x)        ((x) = 0)            //ǿ�ƽ���
#define MRS_TRY_LOCK(x)      (((x) == 0) ? ((x) = 1) : 0)   //���Լ���
#define MRS_LOCK_STATUS(x)       (x)             //��״̬

#define MRS_CHAR_CAPITAL(c)    (((c) >= 'A') && ((c) <= 'Z'))  // �ж��ַ�Ϊ��д��ĸ
#define MRS_CHAR_DIGIT(c)      (((c) >= '0') && ((c) <= '9'))  // �ж��ַ�Ϊ����

#define MRS_ASCII_0                           ('0')   // ASCII�� '0'
#define MRS_ASCII_9                           ('9')   // ASCII�� '9'
#define MRS_ASCII_A                           ('A')   // ASCII�� 'A'
#define MRS_ASCII_Z                           ('Z')   // ASCII�� 'Z'

#define MRS_METER_ADDR_INDEX_0                 (0)     // ���ַ�±�0
#define MRS_METER_ADDR_INDEX_1                 (1)     // ���ַ�±�1
#define MRS_METER_ADDR_INDEX_2                 (2)     // ���ַ�±�2
#define MRS_METER_ADDR_INDEX_3                 (3)     // ���ַ�±�3
#define MRS_METER_ADDR_INDEX_4                 (4)     // ���ַ�±�4
#define MRS_METER_ADDR_INDEX_5                 (5)     // ���ַ�±�5

#define MRS_MAC_ADDR_INDEX_0                   (0)     // MAC��ַ�±�0
#define MRS_MAC_ADDR_INDEX_1                   (1)     // MAC��ַ�±�1
#define MRS_MAC_ADDR_INDEX_2                   (2)     // MAC��ַ�±�2
#define MRS_MAC_ADDR_INDEX_3                   (3)     // MAC��ַ�±�3
#define MRS_MAC_ADDR_INDEX_4                   (4)     // MAC��ַ�±�4
#define MRS_MAC_ADDR_INDEX_5                   (5)     // MAC��ַ�±�5

#define MRS_ADDR_CHECK_LEN_1                   (1)     // ��ַ����1
#define MRS_ADDR_CHECK_LEN_3                   (3)     // ��ַ����3

#define MRS_V_VERSION_STRING        ("V")
#define MRS_R_VERSION_STRING        ("R")
#define MRS_C_VERSION_STRING        ("C")
#define MRS_B_VERSION_STRING        ("B")
#define MRS_SPC_VERSION_STRING      ("SPC")

#define MRS_V_VERSION_WIDTH         (3)
#define MRS_R_VERSION_WIDTH         (3)
#define MRS_C_VERSION_WIDTH         (2)
#define MRS_B_VERSION_WIDTH         (3)
#define MRS_SPC_VERSION_WIDTH       (3)

#define MRS_SRV_VER_LEN    (6)
#define MRS_MANU_CODE_LEN  (2)
#define MRS_CHIP_CODE_LEN  (2)
#define MRS_SW_VER_LEN     (2)

//�����ڴ���������Ϣ
#define MRS_COLLECTOR_07_PROTOCOL_CONTROL         0x03   //�ض������֣�����645-2007��Լ
#define MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL   0x08   //�㲥Уʱ�����еĿ�����
#define MRS_MAXIMUM_DEMAND_CLEAR_CONTROL          0x10   //645-97��Լ������������������

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
#define MRS_BOARDCAST_FRAME_LEN                   18     //�㲥Уʱ���ĳ���
#define MRS_BOARDCAST_TIMEING_CONTROL             0x08   //�㲥Уʱ�����еĿ�����
#endif

//�汾�ж�
#define mrsToolsIsCCO()         HI_MDM_IsProductCon(HI_MDM_GetProductType())       
#define mrsToolsIsMeter()       HI_MDM_IsProductMeter(HI_MDM_GetProductType())
#define mrsToolsIsCLTI()        HI_MDM_IsProductMeterI(HI_MDM_GetProductType())
#define mrsToolsIsTSTA()        HI_MDM_IsProductTSta(HI_MDM_GetProductType())
#define mrsToolsIsIICollector() HI_MDM_IsProductMeterII(HI_MDM_GetProductType())
#define mrsToolsIsSTA()         (mrsToolsIsMeter() || mrsToolsIsCLTI())

//оƬ�����ж�
#define mrsToolsIsTChip()       (HI_FTM_CHIP_TYPE_3911T == HI_MDM_GetHwChipType())

#define DUT_NORMAL_MODE_ADDR    {0xCE,0xBB,0xDC,0xEB,0xEE,0xDC}
#define DUT_NORMAL_MODE_CTRL    0x1E

//����
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/14   ���ⵥ�ź�:DTS2014012607684 */
#define MRS_CCO_ALGORISM_TYPE   (10)
#define MRS_CCO_HEX_TYPE        (16)
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */

#define MRS_BIT_PER_BYTE        (8)
//HI�ӿ�
#define mrsToolsMalloc(sz)              HI_MDM_Malloc(HI_MOD_ID_APP_COMMON,sz)
#define mrsToolsFree(sz)                do{if(sz)HI_MDM_Free(HI_MOD_ID_APP_COMMON,(HI_PVOID)sz);sz=0;}while(0)
//weichao sdk-delete beijing-add
//#define mrsToolsMemmove_s(des,max,src,cnt)  HI_memmove_s(des,max,src,cnt)
#define mrsToolsMemset_s(des,max,src,cnt)   memset_s(des,max,src,cnt)
#define mrsToolsMemcpy_s(des,max,src,cnt)   memcpy_s(des,max,src,cnt)
#define mrsToolsMemZero_s(des,max,cnt)      memset_s(des,max,0,cnt)
//weichao 
#define mrsToolsMemcmp(b1,b2,cnt)       memcmp(b1,b2,cnt)

#define  MRS_INVALID_MAC                "\x00\x00\x00\x00\x00\x00"  // ��Ч��ַ
#define  MRS_BROADCAST_MAC              "\xff\xff\xff\xff\xff\xff"  // �㲥Mac��ַ
#define  MRS_LOCAL_BROADCAST_MAC        "\x00\xff\xff\xff\xff\xff"  // ���ع㲥Mac��ַ
#define  MRS_BROADCAST_ADDR             "\x99\x99\x99\x99\x99\x99"  // �㲥���ַ
#define  MRS_COLLECTOR_ADDR             "\xBB\xBB\xBB\xBB\xBB\xBB"  // �ɼ�����ַ
#define  MRS_LOCAL_BROADCAST_MAC              "\x00\xff\xff\xff\xff\xff"  // ���ع㲥Mac��ַ


#define mrsToolsInvalidMac(x)           (memcmp(x,MRS_INVALID_MAC,6)==0)
#define mrsToolsBroadCastMac(x)         ((memcmp(x,MRS_BROADCAST_MAC,6)==0)||(memcmp(x,MRS_BROADCAST_ADDR,6)==0)) //Matteo
#define mrsToolsLocalBroadCastMac(x)    (memcmp(x,MRS_LOCAL_BROADCAST_MAC,6)==0)
#define mrsToolsBroadCastAddr(x)        (memcmp(x,MRS_BROADCAST_ADDR,6)==0)
#define mrsToolsCollectorAddr(x)        (memcmp(x,MRS_COLLECTOR_ADDR,6)==0)
#define mrsToolsAbnormalMac(x)          (mrsToolsInvalidMac(x) || mrsToolsBroadCastMac(x))
#define mrsToolsNormalMac(x)            (!mrsToolsAbnormalMac(x))
#define mrsToolsMemEq(x,y,sz)           (memcmp(x,y,sz)==0)
#define mrsToolsNormalAddr(x)           (mrsToolsCheckBCD(x,HI_METER_ADDR_LEN) && !mrsToolsBroadCastAddr(x))
#define mrsToolsZeroAddr(x)             (mrsToolsInvalidMac(x))




//���ز���dС����С��a�ı���,a��2��x�η�
#define mrsToolsAlign(d,a)     (((d)+(a-1))&~(a-1))    

// �ж��ֽ�Ϊ0x00ת��Ϊ0xFE
#define MRS_TOOLS_00_TO_FE(a)           do{ if(a == 0x00) a = 0xFE; }while(0);
// �ж��ֽ�Ϊ0xFEת��Ϊ0x00
#define MRS_TOOLS_FE_TO_00(a)           do{ if(a == 0xFE) a = 0x00; }while(0);

HI_VOID mrsSetBitMap(HI_U8 *pBitmap, HI_U16 pos);
HI_BOOL mrsGetBitMap(HI_U8 *pBitmap, HI_U16 pos);


//��ȡ��ģ���MAC
HI_U8 * mrsToolsGetLocalMac(HI_U8 []);

//��ȡ�������е�ϵͳ��Ϣ
HI_U32 mrsToolsGetLocalSystemInfo(HI_SYS_STATUS_INFO_S * status_info);

//��ȡԶ�̰汾��Ϣ
HI_U32 mrsToolsGetRemoteSystemInfo(HI_U8 * mac, HI_SYS_STATUS_INFO_S * status_info);

//���ߺ���
HI_PUBLIC HI_U8    mrsToolsCalcCheckSum(HI_PBYTE,HI_U16);
HI_PUBLIC HI_U16   mrsToolsCalcCheckSum2(HI_PBYTE,HI_U16);
HI_PUBLIC HI_PBYTE mrsToolsStreamFindByte(HI_PBYTE,HI_U16,HI_U8);
HI_PUBLIC HI_U32   mrsToolsCheck645Frame(HI_PBYTE,HI_U16);
HI_PUBLIC HI_U32   mrsToolsCheck3762Frame(HI_PBYTE,HI_U16);
HI_PUBLIC HI_BOOL  mrsToolsCheckBCD(HI_PBYTE,HI_U16);
HI_PUBLIC HI_BOOL  mrsToolsCheckBCDEx(HI_PBYTE,HI_U16);
HI_PUBLIC HI_U8    mrsToolsGet645Ver(HI_U8);

HI_PUBLIC HI_BOOL  mrsToolsMemIsXX(HI_U8* pucBuf, HI_U32 ulBufLen, HI_U8 ucCh);
#define mrsToolsMemIsEq(pucBuf, ulBufLen, ucCh) mrsToolsMemIsXX((pucBuf), (ulBufLen), (ucCh))
#define mrsToolsMemIsZero(pucBuf, ulBufLen)     mrsToolsMemIsXX((pucBuf), (ulBufLen), 0x00)
#define mrsToolsMemIsFF(pucBuf, ulBufLen)       mrsToolsMemIsXX((pucBuf), (ulBufLen), 0xFF)

// ���ַת��ΪMAC��ַ
// ��α��ַ˳�����Ϊ645����˳��
// ��α��������Լ����
// ʹ����ͨ������ֵ�ж��Ƿ�ת���ɹ�
HI_PUBLIC HI_U32 mrsToolsConvertMeter2Mac(HI_IN HI_U8 aucMeter[HI_METER_ADDR_LEN],  
                                                    HI_IN HI_U8 ucProto, 
                                                    HI_OUT HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);
// �жϸ�MAC��ַ�Ƿ��ɵ����ת������(��Ч:HI_TRUE; ��Ч:HI_FALSE)
HI_PUBLIC HI_BOOL mrsToolsIsMacAddrFromMeter(HI_U8 *ucMacAddr);


/*������ת���ɶ���BCD��,������*/
HI_PUBLIC HI_U32 mrsIntToBcd(HI_U32 data,HI_U8 bcd[],HI_U8 len);
// HEX��������
HI_PUBLIC HI_VOID mrsHexInvert(HI_U8 * buf, HI_U16 len);
// ������Ч�Լ��(a:��ǰֵ b:���ֵ c:Ĭ��ֵ d:��Сֵ)
#define MRS_DATA_VALID_CHECK(a,b,c,d) ((((a) <= (b)) && ((a) >= (d)))? (a):((a) = (c)))

// ���ݱ߽�ֵ���(a:��ǰֵ b: ���ֵ c: ��Сֵ)
#define MRS_DATA_BOUNDARY_CHECK(a,b,c) (((a) > (b))? ((a) = (b)) : (((a) < (c))? ((a) = (c)) : (a)))
/* >> Bit31..Bit30..Bit23..Bit22..Bit15..Bit14..Bit7..Bit6..Bit1..Bit0 << */
#define BIT32_SET(x,i)    ((x) = (HI_U32)((((HI_U32)1) << (i)) | (x)))
#define BIT32_CLR(x,i)    ((x) = (HI_U32)((~(HI_U32)(((HI_U32)1) << (i))) & (x)))
#define BIT32_GET(x,i)    (((((HI_U32)1) << (i)) & (x))? 1 : 0)

#define BIT16_SET(x,i)    ((x) = (HI_U16)((((HI_U16)1) << (i)) | (x)))
#define BIT16_CLR(x,i)    ((x) = (HI_U16)((~(HI_U16)(((HI_U16)1) << (i))) & (x)))
#define BIT16_GET(x,i)    (((((HI_U16)1) << (i)) & (x))? 1 : 0)
 
#define BIT8_SET(x,i)    ((x) = ((((HI_U8)1) << (i)) | (x)))
#define BIT8_CLR(x,i)    ((x) = ((~(HI_U8)(((HI_U8)1) << (i))) & (x)))
#define BIT8_GET(x,i)    (((((HI_U8)1) << (i)) & (x))? 1 : 0)

HI_CHAR* mrsFindStrEnd(HI_CHAR* pStr1, HI_CHAR* pStr2);
HI_U32 mrsStrToInt(HI_CHAR* pStr, HI_U8 ucType);
HI_BOOL mrsIsLetterValid(HI_CHAR *pStr);
HI_BOOL mrsIsLetterValidHex(HI_CHAR *pStr);
HI_BOOL mrsIsMeterMatch(HI_U8* pucMeter, HI_U8* pucCmpMeter);        

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_BOOL mrsToolIsBbu(HI_VOID);
HI_BOOL mrsToolCheckBbu(HI_U8 ucFuncCfg);
HI_BOOL mrsIsSimuChlOpened(HI_VOID);
// ��ȡ14F1��06F2��06F4��06F5��06F10����֡��ʱʱ�䣨����ģ��ͨ������״̬��ȡ��
HI_U32 mrsToolsGetTimeOut(HI_U32 ulSimuChlClosedTimeOut);
HI_BOOL mrsToolsCheckBroadCast(HI_PBYTE ptr,HI_U16 len);
#endif

HI_BOOL mrsSrvVerifyMac(HI_U8 *pucMac);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_BOOL mrsSrvVerifyCcoMac(HI_U8 *pucMac);
#endif
HI_END_HEADER
#endif

