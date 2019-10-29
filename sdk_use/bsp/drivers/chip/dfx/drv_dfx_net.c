#include "drv_dfx_cmn_inc.h"
#include <dfx_sal.h>//for DIAG_CMD_ETH_CONFIG_STRU  HI_ARP_INFO_S
#include "drv_dfx_net.h"
#include <sal_os_net_api.h>
#include "lwip/static_route.h"
#include <api_shell.h>
#include <sal_common.h>
#include <os_vx_sal_api.h>
#include <sal_os_net.h>


#define IP_ADDR_NONE 0xffffffff

typedef struct
{
    HI_U32 ulBufStAddr;
    HI_U32 ulBufSize;
    HI_U32 ulUsedSize;
}DRX_PRINT_BUF_S;


HI_U32 arp_cmd(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_U32 argc = 0;
    HI_CHAR **argv = HI_NULL;
    HI_CHAR * pReportBuf = HI_NULL;
    HI_U32 ulReoprtBufLen = 1024; 
    HI_U32 ulArpTblSize =  0;
    HI_U32 ulArpCnt = 0;
    HI_ARP_ITEM_S *pArpArray = HI_NULL;
    HI_ARP_ITEM_S *pArpInfo = HI_NULL;
    HI_U32 tempLen = 0;
    HI_U32 i= 0;
    HI_U32 ipAddr[4]={0};

    pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_SUCCESS;
    }
    
    argc=pHandle->argc;
    argv=pHandle->argv;
    pReportBuf = pHandle->pOutLineBuf;
    
    if(argc==1 && (strcmp(argv[0],"-a")==0))
    {
        ulArpTblSize = SAL_ArpGetTblSize();
        pArpArray = (HI_ARP_ITEM_S *)HI_MDM_Malloc(HI_MOD_ID_DFX,ulArpTblSize*sizeof(HI_ARP_ITEM_S));
        if(pArpArray==HI_NULL)
        {
            HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
            diag_shell_cmd_restore(pHandle);
            return HI_ERR_SUCCESS;
        }

        HI_MDM_GetArpInfo(pArpArray,&ulArpCnt);
        (hi_void)snprintf_s(pReportBuf, ulReoprtBufLen, ulReoprtBufLen - 1,"\r\nip_address      mac_address      eth_name\n");
        
        for(i=0;i<ulArpCnt;i++)
        {
            
            pArpInfo=&pArpArray[i];
                tempLen = strlen(pReportBuf);
                (hi_void)snprintf_s(pReportBuf+tempLen, ulReoprtBufLen-tempLen, ulReoprtBufLen-tempLen - 1,"%u.%u.%u.%u",
                           pArpInfo->ipAddr[0],pArpInfo->ipAddr[1],pArpInfo->ipAddr[2],pArpInfo->ipAddr[3]);
                tempLen = strlen(pReportBuf);
                (hi_void)snprintf_s(pReportBuf+tempLen, ulReoprtBufLen-tempLen, ulReoprtBufLen-tempLen - 1,"%x:%x:%x:%x:%x:%x",
                           pArpInfo->macAddr[0],pArpInfo->macAddr[1],pArpInfo->macAddr[2],pArpInfo->macAddr[3],pArpInfo->macAddr[4],pArpInfo->macAddr[5]);
                tempLen = strlen(pReportBuf);
                int ret1 = 0;
                ret1 = snprintf_s(pReportBuf+tempLen, ulReoprtBufLen-tempLen, ulReoprtBufLen-tempLen - 1,"%s\n",pArpInfo->name);
                if(ret1 < 0){
                    continue;
                }
        }
        
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pReportBuf,(HI_U16)strlen(pReportBuf),HI_TRUE);
        HI_MDM_Free(HI_MOD_ID_DFX,pArpArray);
    }
    else if(argc==2
        &&(strcmp(argv[0],"-d")==0))
    {
        *(HI_U32 *)ipAddr = ipaddr_addr(argv[1]);
        if(SAL_DelArp((HI_U8*)ipAddr)==HI_ERR_SUCCESS)
        {
            HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_SUCCESS,(HI_U16)strlen(STR_IND_SUCCESS)+1,HI_TRUE);
        }
        else
        {
            HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_FAIL,(HI_U16)strlen(STR_IND_FAIL)+1,HI_TRUE);
        }
        
    }
    else
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1,HI_TRUE);
    }
    
    diag_shell_cmd_restore(pHandle);
    return HI_ERR_SUCCESS;
}

/*ossroute eth_name [-p(ip)][-n(netmask)] [-g(gateway)]*/
HI_U32 dfx_add_route(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	HI_U32 ret = HI_ERR_FAILURE;
	HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
	HI_U32 argc = 0;
	HI_CHAR **argv = HI_NULL;
	HI_U32 ulReoprtBufLen = 512;
	HI_U32 i = 0;
	HI_CHAR *pStrIp = HI_NULL;
	HI_CHAR *pStrMask = HI_NULL;
	HI_CHAR *pStrGw = HI_NULL;
	HI_U8 arrayIp[4] = {0};
	HI_U8 arrayMask[4] = {0};
	HI_U8 arrayGw[4] = {0};

	pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);	  
	if(pHandle==HI_NULL)
	{
		HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
		return HI_ERR_SUCCESS;
	}
	argc=pHandle->argc;
	argv=pHandle->argv;

	for(i=1;i<argc;i=i+2)
	{
		if(strcmp(argv[i],"-p")==0)
		{
			pStrIp = argv[i+1];
			
		}
		else if(strcmp(argv[i],"-n")==0)
		{
			pStrMask = argv[i+1];
		}
		else if(strcmp(argv[i],"-g")==0)
		{
			pStrGw = argv[i+1];
		}
		else
		{
			HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
			diag_shell_cmd_restore(pHandle);
			return HI_ERR_FAILURE;
		}
		
	}


	HI_IP4_ADDR_STR_TO_ARRAY(pStrIp,arrayIp,&ret);
	if(ret!=HI_ERR_SUCCESS)
	{
		HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
		diag_shell_cmd_restore(pHandle);
		return HI_ERR_FAILURE;
	}

	HI_IP4_ADDR_STR_TO_ARRAY(pStrMask,arrayMask,&ret);
	if(ret!=HI_ERR_SUCCESS)
	{
		HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG, (HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
		diag_shell_cmd_restore(pHandle);
		return HI_ERR_FAILURE;
	}
	
	HI_IP4_ADDR_STR_TO_ARRAY(pStrGw,arrayGw,&ret);
	if(ret!=HI_ERR_SUCCESS)
	{
		HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG, (HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
		diag_shell_cmd_restore(pHandle);
		return HI_ERR_FAILURE;
	}



	ret = SAL_Add_Route(argv[0],arrayIp,arrayMask,arrayGw);
	
	if(ret == HI_ERR_SUCCESS)
	{
		HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)STR_IND_SUCCESS,(HI_U16)strlen(STR_IND_SUCCESS)+1,HI_TRUE);

	}
	else
	{
		HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)STR_IND_FAIL,(HI_U16)strlen(STR_IND_FAIL)+1,HI_TRUE);

	}
	diag_shell_cmd_restore(pHandle);
	return ret;

}
/*osdroute eth_name [-p(ip)]*/
HI_U32 dfx_del_route(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_U32 argc = 0;
    HI_CHAR **argv = HI_NULL;
    HI_U32 ulReoprtBufLen = 512;
    HI_CHAR *pStrIp = HI_NULL;
    HI_U8 arrayIp[4] = {0};

    pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_FAILURE;
    }
    
    argc=pHandle->argc;
    argv=pHandle->argv;

    if(argc==1)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1,HI_TRUE);

    }

    pStrIp = argv[0];
    HI_IP4_ADDR_STR_TO_ARRAY(pStrIp,arrayIp,&ret);
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG, (HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
        diag_shell_cmd_restore(pHandle);
        return HI_ERR_FAILURE;
    }

    ret = SAL_routeDelete(arrayIp);
    if(ret == HI_ERR_SUCCESS)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)STR_IND_SUCCESS,(HI_U16)strlen(STR_IND_SUCCESS)+1,HI_TRUE);

    }
    else
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)STR_IND_FAIL,(HI_U16)strlen(STR_IND_FAIL)+1,HI_TRUE);
    }
    
    diag_shell_cmd_restore(pHandle);
    return ret;
}

HI_U32 dfx_route(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_CHAR * pReportBuf = HI_NULL;
    HI_U32 ulReoprtBufLen=512;

    pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    pReportBuf = pHandle->pOutLineBuf;
    lwip_route_table_show(pReportBuf, ulReoprtBufLen);
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)pReportBuf,(HI_U16)strlen(pReportBuf)+1,HI_TRUE);
    diag_shell_cmd_restore(pHandle);
    return HI_ERR_SUCCESS;

}

/*osipconfig  eth_name*/
HI_U32 ip_read(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_ETH_CONFIG_STRU stEthCfg = {{0}, {0}, {0}};
    HI_U32 pTempLen = 0;
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_U32 argc =0;
    HI_CHAR **argv = HI_NULL;
    HI_CHAR * pReportBuf = HI_NULL;
    HI_U32 ulReoprtBufLen=512;

    pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_SUCCESS;
    }
    
    argc=pHandle->argc;
    argv=pHandle->argv;
    pReportBuf = pHandle->pOutLineBuf;

    if(argc!=1)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1,HI_TRUE);
        return HI_ERR_SUCCESS;

    }

    ret = SAL_GetEthCfg(argv[0], &stEthCfg ,(HI_U16)sizeof(DIAG_CMD_ETH_CONFIG_STRU));
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)"\r\ndev name Fail!\n",(HI_U16)strlen("\r\ndev name Fail!\n")+1,HI_TRUE);
        diag_shell_cmd_restore(pHandle);
        return HI_ERR_FAILURE;
    }

    (hi_void)snprintf_s(pReportBuf, ulReoprtBufLen, ulReoprtBufLen - 1,"\r\nIP ADDR:\t%u.%u.%u.%u"
        ,stEthCfg.ip_addr[0],stEthCfg.ip_addr[1],stEthCfg.ip_addr[2],stEthCfg.ip_addr[3]);
    pTempLen = strlen(pReportBuf);
    (hi_void)snprintf_s(pReportBuf+pTempLen, ulReoprtBufLen-pTempLen, ulReoprtBufLen-pTempLen - 1,"\r\nNET MASK:\t%u.%u.%u.%u"
        ,stEthCfg.net_mask[0],stEthCfg.net_mask[1],stEthCfg.net_mask[2],stEthCfg.net_mask[3]);
    pTempLen = strlen(pReportBuf);

    (hi_void)snprintf_s(pReportBuf+pTempLen, ulReoprtBufLen-pTempLen, ulReoprtBufLen-pTempLen - 1,"\r\nMAC ADDR:\t%x:%x:%x:%x:%x:%x"
        ,stEthCfg.mac_addr[0],stEthCfg.mac_addr[1],stEthCfg.mac_addr[2],stEthCfg.mac_addr[3],stEthCfg.mac_addr[4],stEthCfg.mac_addr[5]);
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)pReportBuf,(HI_U16)strlen(pReportBuf)+1,HI_TRUE);
    diag_shell_cmd_restore(pHandle);
    return HI_ERR_SUCCESS;
}
/*ossipconfig eth_name [-p(ip)] [-n(netmask)] [-m(mac)] */
HI_U32 ip_write(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{    
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_ETH_CONFIG_STRU stEthCfg = {{0}, {0}, {0}};
    HI_CHAR *pStrMac = HI_NULL;
    HI_CHAR *pStrIp = HI_NULL;
    HI_CHAR *pStrMask = HI_NULL;
    HI_U32 i = 0;
    HI_CHAR *nptr = HI_NULL;
    HI_CHAR *endptr = HI_NULL;
    HI_U32 tempIpAddr = 0;

    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_U32 argc;
    HI_CHAR **argv;
    HI_U32 ulReoprtBufLen=512;


    pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_SUCCESS;
    }
    argc=pHandle->argc;
    argv=pHandle->argv;


    //a.不考虑"-p xxx -p xxx" 场景
    for(i=1;i<argc;i=i+2)
    {
        if(strcmp(argv[i],"-p")==0)
        {
            pStrIp = argv[i+1];
        }
        else if(strcmp(argv[i],"-n")==0)
        {
            pStrMask = argv[i+1];
        }
        else if(strcmp(argv[i],"-m")==0)
        {
            pStrMac = argv[i+1];
        }
        else
        {
            HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
            diag_shell_cmd_restore(pHandle);
            return HI_ERR_FAILURE;
        }
        
    }

    ret = SAL_GetEthCfg(argv[0], &stEthCfg ,(HI_U16)sizeof(DIAG_CMD_ETH_CONFIG_STRU));
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)"\r\ndev name Fail!\n",(HI_U16)strlen("\r\ndev name Fail!\n")+1 ,HI_TRUE);
        diag_shell_cmd_restore(pHandle);
        return HI_ERR_FAILURE;
    }


    
    if(pStrIp!=HI_NULL) 
    {
        tempIpAddr= ipaddr_addr(pStrIp);
        if(tempIpAddr==IP_ADDR_NONE)
        {
            HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,  (HI_PBYTE)"invalid ip.\n",(HI_U16)strlen("invalid ip.\n")+1, HI_TRUE);
            diag_shell_cmd_restore(pHandle);
            return HI_ERR_FAILURE;
        }

        (hi_void)memcpy_s(stEthCfg.ip_addr, sizeof(stEthCfg.ip_addr), &tempIpAddr, sizeof(tempIpAddr));
    } 

    if(pStrMask!=HI_NULL)
    {
        tempIpAddr = ipaddr_addr(pStrMask);
        if(tempIpAddr==0xffffffff)
        {
            HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)"invalid netmask.\n",(HI_U16)strlen("invalid netmask.\n")+1, HI_TRUE);
            diag_shell_cmd_restore(pHandle);
            return HI_ERR_FAILURE;
        }

        (hi_void)memcpy_s(stEthCfg.net_mask, sizeof(stEthCfg.net_mask), &tempIpAddr, sizeof(tempIpAddr));
    }


    if(pStrMac!=HI_NULL)
    {
        nptr = pStrMac;
        endptr = HI_NULL;
        for(i=0;i<6;i++)
        {
            stEthCfg.mac_addr[i]=(HI_U8)strtoul(nptr,&endptr,16);
            
            if(*endptr=='\0'&&i!=5)//mac地址不完整
            {
                HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,  (HI_PBYTE)"invalid mac.\n",(HI_U16)strlen("invalid mac.\n")+1, HI_TRUE);
                
                    
                diag_shell_cmd_restore(pHandle);
                return HI_ERR_FAILURE;

            }
            nptr=endptr+1;
        }       
    }


    ret = SAL_SetEthCfg(argv[0], &stEthCfg ,(HI_U16)sizeof(DIAG_CMD_ETH_CONFIG_STRU));
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)"set cfg fail.\n",(HI_U16)strlen("set cfg fail.\n")+1, HI_TRUE);
    }
    

    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)"set cfg success.\n",(HI_U16)strlen("set cfg success.\n")+1, HI_TRUE);
    diag_shell_cmd_restore(pHandle);
    return HI_ERR_SUCCESS;
}

HI_U32  dfx_ping(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_U32 argc = 0;
    HI_CHAR **argv = HI_NULL;
    HI_CHAR * pReportBuf = HI_NULL;
    HI_U32 ulReoprtBufLen=512;
    struct ping_ctx ping_info ;
    HI_CHAR *pStrSize=HI_NULL;
    HI_U32 ulSize=32;
    HI_U32 i=0;
    HI_U32 ret=HI_ERR_SUCCESS;

	pHandle = diag_shell_cmd_prepare(pCmdParam,usCmdParamSize,ulReoprtBufLen);    
    if(pHandle==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_SUCCESS;
    }
    argc=pHandle->argc;
    argv=pHandle->argv;
    pReportBuf = pHandle->pOutLineBuf;

    if(argc<1)
    {
        diag_shell_cmd_restore(pHandle);
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
        return HI_ERR_FAILURE;
    }

    for(i=1;i<argc;i=i+2)
    {
        if(strcmp(argv[i],"-l")==0)
        {
            pStrSize = argv[i+1];
        }
        else
        {
            HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)STR_IND_PARAM_WRONG,(HI_U16)strlen(STR_IND_PARAM_WRONG)+1, HI_TRUE);
            diag_shell_cmd_restore(pHandle);
            return HI_ERR_FAILURE;
        }
        
    }
    
    if(pStrSize)
    {
        ulSize = strtoul(pStrSize, (char **)HI_NULL, 16);
    }

    (hi_void)memset_s((HI_VOID *)&ping_info, sizeof(ping_info), 0, sizeof(ping_info));
    ping_info.dst_ip = argv[0];
    ping_info.count = 1;
    ping_info.len = (HI_S32)ulSize;
    ret = osShellPing(&ping_info);
    int ret1 = 0;
    if(ret==HI_ERR_SUCCESS)
    {
        
        ret1 = snprintf_s(pReportBuf, ulReoprtBufLen, ulReoprtBufLen - 1,"success ip=%s len=%u count=%u ttl=%u rtt=%u type=%u code=%u\r\n"
            ,ping_info.dst_ip,ping_info.len,ping_info.count,ping_info.ttl,ping_info.rtt,ping_info.icmp_type,ping_info.icmp_code);
    }
    else
    {
        ret1 = snprintf_s(pReportBuf, ulReoprtBufLen, ulReoprtBufLen - 1,"fail ip=%s len=%u count=%u ttl=%u rtt=%u type=%u code=%u\r\n"
            ,ping_info.dst_ip,ping_info.len,ping_info.count,ping_info.ttl,ping_info.rtt,ping_info.icmp_type,ping_info.icmp_code);
    }

    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)pReportBuf,(HI_U16)strlen(pReportBuf)+1,HI_TRUE);
    diag_shell_cmd_restore(pHandle);
    if(ret1 < 0){
        return HI_ERR_SUCCESS;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 diag_cmd_id_ue_alive_spy(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    return HI_ERR_SUCCESS;
}

