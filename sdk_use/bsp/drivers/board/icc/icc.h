
#ifndef __ICC_H__
#define __ICC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_icc.h>
#include <dfx_icc.h>

#define HI_ICC_LISTEN_SOCKET_LIMIT_CONNECTION_COUNT 1//listen socket�������connect���Ӹ���
#define HI_ICC_SERVER_DEFAULT_SEND_OUT_TIME 10//���豸socketĬ�Ϸ��ͳ�ʱʱ��
#define HI_ICC_SERVER_DEFAULT_RCV_OUT_TIME 10//���豸socketĬ�Ͻ��ճ�ʱʱ��
#define ICC_MAX_TRY_CONNECT_FAIL_TIMES 3//������������ʧ��x�κ��ж������쳣���л�ͨ��״̬Ϊ�Ͽ�

#define HI_ICC_CMD_TBL_SIZE  (5)//ICCģ�������û�ע�������Ӧ����������
#define HI_ICC_TEMP_BUF_SIZE (512)//ͨ��Ԥ�������С:��;1���û��޷�������������ʱ��ͨ��ʹ�øû����ȡsocket������
#define HI_ICC_MAGIC_NUM 0x56127895//ͨ��ħ����:��δʹ��
#define HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN 16//IP��ַ�ַ�����ʽ����

#define HI_ICC_PORT_NUM_20000 (20000)//��ʹ�ö˿ں�(����)�������г���ʼ�����ò��������ط���Ӧ�õ���
#define HI_ICC_PORT_NUM_20001 (20001)//��ʹ�ö˿ں�(����)�������г���ʼ�����ò��������ط���Ӧ�õ���

//*****************************************************************************
// ��������: HI_ICC_CMD_F
// ��������: �û�ע�ᵽICC�����������ICC�յ��û��������øú���
//
// ����˵��:
//      socket[in] �յ�����ĵ�ͨ���˿ںţ����豸��Ҫ��������ͨ����ͨ��֪ͨ���豸
//      pIccCmdPkt[in] ICC�����ͷ
//
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      HI_ERR_ICC_CMD_NOT_SUPPORT:��֧�ָ���������������ϡ�
//
// ����Ҫ��:
//      �û����÷��ش�����HI_ERR_ICC_CMD_NOT_FOUND
//      �û������socket���в��������÷��ش�����HI_ERR_ICC_CMD_NOT_SUPPORT
//
// ���þ���:
// ��    ��: �ߺ���20150205
//*****************************************************************************

typedef HI_U32(*HI_ICC_CMD_F) (HI_IN HI_SOCKET socket,HI_IN HI_ICC_CMD_PKT_S *pIccCmdPkt);
//ICCӦ���������ݲ�֧��
typedef HI_U32(*HI_ICC_IND_F) (HI_OUT HI_SOCKET socket,HI_OUT HI_ICC_IND_PKT_S *pIccIndPkt);


typedef enum
{
    ICC_CHANGE_CONNECT_OPT_CONNECT,//�л�ͨ��״̬Ϊ����״̬
    ICC_CHANGE_CONNECT_OPT_CONNECT_FAIL,//ͨ����������ʧ�ܴ���+1,����ʧ�ܴ����ﵽICC_MAX_TRY_CONNECT_FAIL_TIMESͨ����Ϊ�Ͽ�״̬
    ICC_CHANGE_CONNECT_OPT_DISCONNECT,//�л�ͨ��״̬Ϊ�Ͽ�״̬
}ICC_CHANGE_CONNECT_OPT_E;//����ͨ������״̬����



typedef struct
{
    HI_U8 bConnect:1;
    HI_U8 bClient:1;
    HI_U8 bServer:1;
    HI_U8 reserver_1:5;
    HI_U8 tryConnectFailTimes;
    HI_U16 phyPortNum;
    HI_U32 ulSemId;
}ICC_CLIENT_CHANNEL_S;




typedef struct
{
    HI_U32 ulMinId;//�û�����Ӧ��ID����
    HI_U32 ulMaxId;//�û�����Ӧ��ID����
    HI_ICC_CMD_F pfnInputCmd;//�����û��������
    HI_ICC_IND_F pfnInputInd;//�����û�Ӧ�����
} HI_ICC_CMD_OBJ_S;//�û�����Ӧ��ṹ

typedef struct
{
    HI_ICC_CMD_OBJ_S cmdTbl[HI_ICC_CMD_TBL_SIZE];
    HI_U8 temp_buf[HI_ICC_TEMP_BUF_SIZE];
    HI_ICC_STAT_S errCtx;
    ICC_CLIENT_CHANNEL_S channel[ICC_CHANNEL_ID_MAX];
    HI_CHAR ucTargetIp[HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN];//Ŀ�� IP��ַ "255.255.255.255"
}HI_ICC_CTRL_S;







HI_EXTERN HI_BOOL g_icc_is_init;
HI_EXTERN HI_ICC_CTRL_S g_icc_ctrl;

#define ICC_GET_ERR_CTX()        ((HI_ICC_STAT_S *)(&(g_icc_ctrl.errCtx)))
#define ICC_GET_CTRL_CTX()       ((HI_ICC_CTRL_S *)(&(g_icc_ctrl)))

#define ICC_CHECK_NOT_INIT_RETURN(ret) do{if(!g_icc_is_init){return ret;}}while(0)
#define ICC_CHECK_NOT_INIT_RETURN_NOVAL()    do{if(!g_icc_is_init){return ;}}while(0)

HI_U32 icc_send(HI_S32 socket,HI_PBYTE buf,HI_S32 totalLen);
HI_U32 icc_receive(HI_S32 socket,HI_PBYTE buf,HI_S32 totalLen);
HI_U32 icc_close_socket(HI_SOCKET s);
HI_U32 icc_connect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum,HI_U32 ulTimeOutsec);
HI_VOID icc_check_connect_status(HI_VOID);


#endif
