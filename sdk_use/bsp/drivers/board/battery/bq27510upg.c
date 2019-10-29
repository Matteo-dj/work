
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)
#include <hi_driver.h>
#include <hi_mdm_sem.h>
#include <hi_mdm_watchdog.h>
#include "bq27510.h"
#include "battery_debug.h"

/*
    升级每行文件数据个数:128足够，不出大变动文件格式不会发生变化，只会对文件中的ff进行修改
    文件大小100-300K足够，因为文件个数不会发生变动
    升级完以后，reset芯片后，做3s的延时在读取芯片
    文件每行已w: x: c: 开头
*/
HI_BOOL g_test_not_change_mod = HI_FALSE;
#define BSP_I2C_MAX_TRANSFER_LEN             128         //升级文件一条指令最大数据个数
#define BQ27510_UPGRADE_RETRY_TIMES          3          //升级尝试次数
#define BSP_MAX_ASC_PER_LINE                 400
#define BQ_UPG_SLEEP_AFTER_CHANGE_MOD        500
#define BQ_UPG_FILE_MAX_SIZE                 (300*1024) //300K
#define BQ_UPG_FILE_MIN_SIZE                 (100*1024)//100K
#define  g_debug_add_mdelay 1

/**************************************************************
函数名:bq27510_mdelay
功能:延时函数
参数:
    us:延时时长(ms)
返回值:
    非0:i2c操作错误或数据校验错误
***************************************************************/
HI_PRV HI_U32 bq27510_mdelay(HI_U32 ms)
{
    HI_U64 ullMs = (HI_U64)(((HI_U64)ms + (HI_U64)g_debug_add_mdelay) * (HI_U64)1000);

    if (ullMs > 0xffffffff)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    HI_MDM_Udelay((HI_U32)ullMs);
    return HI_ERR_SUCCESS;
}
/**************************************************************
函数名:bq27510_data_check
功能:从param[0]Reg连续读出n-1个数据，与pram[1]开始的n-1个数据进行比较
参数:clintAddress:设备地址左移1位
     param:
     len:
返回值:
    非0:i2c操作错误或数据校验错误
***************************************************************/
HI_PRV HI_U32 bq27510_data_check(HI_U8 clintAddress,HI_U8 *param,HI_U32 len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_I2C_DATA_S pDataStruct={0};
    unsigned char p_dst[BSP_I2C_MAX_TRANSFER_LEN] = { 0 };
    pDataStruct.pSendBuf = param;
    pDataStruct.ulSendLength = 1;
    pDataStruct.pReceiveBuf = p_dst;
    pDataStruct.ulReceiveLength = len-1;

    ret = HI_I2C_SendRecvData(HI_I2C_IDX_0, clintAddress, &pDataStruct);
    if ((len - 1) > sizeof(p_dst))
    {
        return HI_ERR_FAILURE;
    }
    
    if (((len-1) <= BSP_I2C_MAX_TRANSFER_LEN)
        &&(0 != memcmp((HI_CONST HI_VOID *)(param+1), (HI_CONST HI_VOID *)p_dst, (HI_S32)(len-1))))
    {
        //增加统计量
        return HI_ERR_FAILURE;
    }

    return ret;
}


/**************************************************************
函数名:bq27510_changeToRomMod
功能:切换bq芯片模式为升级模式
参数:
返回值:

***************************************************************/

HI_PRV HI_U32 bq27510_changeToRomMod(HI_VOID)
{
    //设备地址
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 cmd[]={0x00,0x00,0x0F};//将0x00->0x00，0x0F->0x01

    ret = bq27510_write_bytes(BQ27510_NORMAL_ADDR,cmd, sizeof(cmd));
    DRV_NDM_BQ_UPG_DEBUG_Print("bq27510_changeToRomMod %u\r\n",ret);
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    return ret;

}


/**************************************************************
函数名:bq27510_changeToNorMalMod
功能:切换模式为正常模式
参数:
返回值:

***************************************************************/
HI_U32 bq27510_changeToNorMalMod(HI_VOID)
{
    //设备地址
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 cmd0[]={0x00,0x0F};//将0x00->0x00，0x0F->0x01
    HI_U8 cmd1[]={0x64,0x0F,0x00};//将0x00->0x00，0x0F->0x01
    //HI_U8 cmd2[]={0x65,0x00};//将0x00->0x00，0x0F->0x01

    ret += bq27510_write_bytes(BQ27510_ROM_MOD_ADDR,cmd0, sizeof(cmd0));
    ret += bq27510_write_bytes(BQ27510_ROM_MOD_ADDR,cmd1, sizeof(cmd1));
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    DRV_NDM_BQ_UPG_DEBUG_Print("bq27510_changeToNorMalMod %u\r\n",ret);

    return ret;

}

#ifdef BQ_UPG_DEBUG
HI_PRV HI_U32 bq27510_changeToRomMod_debug()
{
    DRV_NDM_BQ_UPG_DEBUG_Print("change RomModSucess\r\n");
    return HI_ERR_SUCCESS;
}
HI_U32 bq27510_write_bytes_debug(HI_U8 clintAddress,HI_U8 *param,HI_U32 len)
{

    /*与输入一致*/
    HI_U32 i=0;
    DRV_NDM_BQ_UPG_DEBUG_Print("w: 16 ");
    for(i=0;i<len-1;i++)
    {
        DRV_NDM_BQ_UPG_DEBUG_Print("%02x ",*(param+i));
    }
    DRV_NDM_BQ_UPG_DEBUG_Print("%02x",*(param+i));

    DRV_NDM_BQ_UPG_DEBUG_Print("\r\n");
    /*与输入一致*/
    sw_mdelay(5);
    return HI_ERR_SUCCESS;

}
HI_PRV HI_U32 bq27510_mdelay_debug(HI_U32 ms)
{
    /*与输入一致*/
    DRV_NDM_BQ_UPG_DEBUG_Print("x: %u\r\n",ms);
    /*与输入一致*/
    sw_mdelay(5);
    return HI_ERR_SUCCESS;
}
HI_PRV HI_U32 bq27510_data_check_debug(HI_U8 clintAddress,HI_U8 *param,HI_U32 len)
{
    HI_U32 i = 0;
    /*与输入一致*/
    DRV_NDM_BQ_UPG_DEBUG_Print("c: 16 ");
    for(i=0;i<len-1;i++)
    {
        DRV_NDM_BQ_UPG_DEBUG_Print("%02x ",*(param+i));
    }
    DRV_NDM_BQ_UPG_DEBUG_Print("%02x",*(param+i));

    DRV_NDM_BQ_UPG_DEBUG_Print("\r\n");
    /*与输入一致*/
    sw_mdelay(5);
    return HI_ERR_SUCCESS;
}
#endif

/**************************************************************
函数名:bq27510_exe_online
功能:解析一行升级文件，并执行
参数:
返回值:

***************************************************************/
HI_PRV HI_U32 bq27510_exe_online(HI_U8 clintAddress,HI_UCHAR *oneline,HI_U32 lineLength)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_UCHAR p_src[BSP_I2C_MAX_TRANSFER_LEN] = { 0 };
    unsigned char ucTmpBuf[4] = {0};
    unsigned cmdParamCount = 0;
    HI_U32 delay = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    if(oneline[1]!= ':')
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    switch(oneline[0])
    {
        case 'W':
        case 'w':
        {

            for(i=2,j=0; i<lineLength; i+=2,j++)
            {
                (hi_void)memset_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), 0x0, sizeof(ucTmpBuf));
                (hi_void)memcpy_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), (HI_CONST HI_VOID *)(oneline+i), 2);
                p_src[j] = (HI_UCHAR)strtoul((char *)ucTmpBuf, (char **)HI_NULL, 16);
            }
            cmdParamCount = (lineLength -2)/2;
#ifdef BQ_UPG_DEBUG
            ret = bq27510_write_bytes_debug(clintAddress, p_src+1, cmdParamCount-1);
#else
            ret = bq27510_write_bytes(clintAddress, p_src+1, cmdParamCount-1);
#endif
            break;
        }
        case 'X':
        case 'x':
        {
            (hi_void)memset_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), 0x00, sizeof(ucTmpBuf));
			if(sizeof(ucTmpBuf) >= lineLength-2){
				(hi_void)memcpy_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), (HI_CONST HI_VOID *)(oneline+2), lineLength-2);	
			}   
            delay = (HI_U32)atoi((char *)ucTmpBuf);

			/* BEGIN: Added by b00208046, 2014/5/14   问题单号:DTS2014051406125*/
			if(delay==200)//解决新旧升级文件兼容性问题
			{
				delay=400;
			}
			/* END:   Added by b00208046, 2014/5/14 */

#ifdef BQ_UPG_DEBUG
            ret = bq27510_mdelay_debug(delay);
#else
            ret = bq27510_mdelay(delay);
#endif
            break;
        }
        case 'C':
        case 'c':
        {
            for(i=2,j=0; i<lineLength; i+=2,j++)
            {
                (hi_void)memset_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), 0x0, sizeof(ucTmpBuf));
                (hi_void)memcpy_s((HI_VOID *)ucTmpBuf, sizeof(ucTmpBuf), (HI_CONST HI_VOID *)(oneline+i), 2);
                p_src[j] = (HI_UCHAR)strtoul((char *)ucTmpBuf, (char **)HI_NULL, 16);
            }
            cmdParamCount = (lineLength -2)/2;


            if (cmdParamCount > sizeof(p_src))
            {
                return HI_ERR_INVALID_PARAMETER;   
            }
#ifdef BQ_UPG_DEBUG
            ret = bq27510_data_check_debug(clintAddress, p_src+1, cmdParamCount-1);
#else
            ret = bq27510_data_check(clintAddress, p_src+1, cmdParamCount-1);
#endif
            break;
        }
        case 'R':
        case 'r':
        {
            // TODO: 增加R处理
            break;
        }
        default:
            return HI_ERR_INVALID_PARAMETER;
    }
    return ret;
}
/**************************************************************
函数名:bq27510_firmware_program
功能: 解析升文件
参数:
返回值:

***************************************************************/
HI_PRV HI_U32 bq27510_firmware_program(HI_U8 clintAddress, const unsigned char *pgm_data, unsigned int filelen)
{
    HI_UCHAR oneLine[BSP_MAX_ASC_PER_LINE] = {0};
    HI_UCHAR temp = 0;
    HI_UCHAR * p_cur = HI_NULL;
    HI_U32   cmdLen = 0;//当前命令有效长度
    HI_U32 ulCounter = 0;//代表升级失败次数

    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 i = 0;

bq27510_firmware_program_begin:
    g_bq27510_status.currentExeLine = 0;
    if(ulCounter > BQ27510_UPGRADE_RETRY_TIMES) {
        return HI_ERR_FAILURE;
    }
    ulCounter++;


    p_cur = (HI_UCHAR *)pgm_data;


    while(filelen!=0)
    {
        if((HI_U32)p_cur - (HI_U32)pgm_data >= filelen)
        {
            return HI_ERR_SUCCESS;
        }

        i=0;
        cmdLen = 0;
        (hi_void)memset_s((HI_VOID *)oneLine, sizeof(oneLine), 0, BSP_MAX_ASC_PER_LINE);
        while((i < BSP_MAX_ASC_PER_LINE) && ((HI_U32)p_cur - (HI_U32)pgm_data <= filelen))
        {
            temp = *(p_cur++);
            i++;
            if(('\r' == temp) || ('\n' == temp))
            {
                break;
            }
            if(' ' != temp)
            {
                oneLine[cmdLen++] = temp;
            }
        }
/* BEGIN: Modified by b00208046, 2014/3/29   问题单号:DTS2014022202882 */
        HI_MDM_WDG_Feed();

        if(cmdLen)
        {
            ret = bq27510_exe_online(clintAddress, oneLine, cmdLen);
            g_bq27510_status.currentExeLine++;
            if(ret!=0)
            {
               goto bq27510_firmware_program_begin;
            }
        }

    }
/* END:   Modified by b00208046, 2014/3/29 */
    return HI_ERR_SUCCESS;
}

HI_PRV HI_BOOL bq27510_checkPerByte(HI_UCHAR c,HI_U32 i)
{
    HI_BOOL isCorrectByte = HI_FALSE;
    if((i==0)&&((c=='c')||(c=='C')||(c=='r')||(c=='R')||(c=='w')||(c=='W')||(c=='x')||(c=='X')))//待与FAE确认位置
    {
        isCorrectByte = HI_TRUE;
    }

    if((i==1)&&(c==':'))//待与FAE确认位置
    {
        isCorrectByte = HI_TRUE;
    }

    if((c<='9'&&c>='0')||(c<='F'&&c>='A')||(c<='f'&&c>='a'))
    {
        isCorrectByte = HI_TRUE;
    }

    return isCorrectByte;
}
/*****************************************************
文件校验原则:
文件大小最大:300k
文件大小最小:100K
文件内容校验:c/C/w/W/x/X/r/R:16进制格式
******************************************************/
HI_U32 bq27510_CheckUpgFile(HI_U32 ulOption, HI_U32 ulId, HI_UCHAR *pFile, HI_U32 ulFileSize)
{
    //FIILE SIZE 错误
    //FILE 内容错误
    HI_UCHAR *p_cur = pFile;
    HI_U32 i = 0;
    HI_UCHAR temp=0;
    HI_BOOL isContentExist = HI_FALSE;
    HI_U32 whileMaxCount = BQ_UPG_FILE_MAX_SIZE;

    if(pFile==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(ulFileSize > BQ_UPG_FILE_MAX_SIZE ||ulFileSize < BQ_UPG_FILE_MIN_SIZE)
    {
        g_bq27510_status.bqUpgFailReason = BQ_UPG_FAIL_REASON_ID_FILE_SIZE;
        return HI_ERR_FAILURE;

    }
    while(whileMaxCount--)
    {

        if((HI_U32)p_cur - (HI_U32)pFile >= ulFileSize)
        {
            if(isContentExist)
            {
                DRV_NDM_BQ_UPG_DEBUG_Print("[BAT_UPG_CHECK] check file sucess \r\n");
                return HI_ERR_SUCCESS;
            }
            else
            {
                 return HI_ERR_FAILURE;
            }
        }

        //验证每一行数据正确性
        i=0;
        while((i < BSP_MAX_ASC_PER_LINE)&&((HI_U32)p_cur-(HI_U32)pFile <= ulFileSize))
        {
            temp = *(p_cur++);
            if(('\r' == temp) || ('\n' == temp))
            {
                break;
            }

            //已下需要对i++
            if(' ' != temp)
            {

                if(HI_TRUE != bq27510_checkPerByte(temp,i))
                {
                    DRV_NDM_BQ_UPG_DEBUG_Print("[BAT_UPG_CHECK][err char=%c][i=%u] \r\n",temp,i);
                    g_bq27510_status.bqUpgFailReason = BQ_UPG_FAIL_REASON_ID_FILE_CONTENT;
                    return HI_ERR_FAILURE;
                }
                else
                {
                    isContentExist = HI_TRUE;
                    //nothing
                }
            }
            else
            {
                //nothing
            }
            i++;
        }

        if(i >= BSP_MAX_ASC_PER_LINE)//一行内容过长
        {
            DRV_NDM_BQ_UPG_DEBUG_Print("[BAT_UPG_CHECK] one line is too long\r\n");
            g_bq27510_status.bqUpgFailReason = BQ_UPG_FAIL_REASON_ID_FILE_CONTENT;
            return HI_ERR_FAILURE;
        }

    }
    return HI_ERR_FAILURE;
}

HI_PRV HI_U32 bq27510_DownLoadFirmWare(HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
#ifdef BQ_UPG_DEBUG
    if(g_test_not_change_mod != HI_TRUE)
    {
        ret = bq27510_changeToRomMod_debug();
    }
#else
    if(g_test_not_change_mod != HI_TRUE)
    {
        ret = bq27510_changeToRomMod();
    }
#endif
    if(ret != HI_ERR_SUCCESS)
    {
        //增加统计量
        DRV_NDM_BQ_UPG_DEBUG_Print("[%s,%d]change rom mode fail ERR CODE=%u\r\n",__func__,__LINE__,ret);
        return ret;
    }

    HI_MDM_Sleep(BQ_UPG_SLEEP_AFTER_CHANGE_MOD);//切换模式之后停留一段时间


    ret = bq27510_firmware_program(BQ27510_ROM_MOD_ADDR, pFile, ulFileSize);

    if(ret != HI_ERR_SUCCESS)
    {
        DRV_NDM_BQ_UPG_DEBUG_Print("[%s,%d]upgrate fail ERR_CODE = %u\r\n",__func__,__LINE__,ret);
        return ret;
    }
    return ret;
}

HI_U32 bq27510_Upgrade(HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_DownLoadFirmWare(ulOption, ulId, pFile, ulFileSize);
    BQ_I2C_MUTEX_UNLOCK();
    return ret;
}

#endif

