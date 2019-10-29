#include "app_common.h"
#include "cts_debug.h"
#include "dfx_app.h"
#include "drv_dfx.h"
#include "dfx_mac.h"
#include <hi-early-debug.h>
//*****************************************************************************
HI_U32 cts_HI_MDM_Crc(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId)
{
    CTS_PRINT0("enter cts_HI_MDM_Crc");

       HI_U8 buf[500] = {0};
	HI_U32 bufSize = sizeof(buf);
	HI_U32 crcValue32 = 0;
	HI_U16 crcValue16 = 0;

	HI_U32 i=0;
	for (i=0;i<bufSize;i++)
	{
		buf[i]=i;
	}
    switch (pucParam[0])
    	{
    	case 0:
		crcValue16 = HI_MDM_Crc16(crcValue16,buf,bufSize);
             crcValue32 = HI_MDM_Crc32(crcValue32,buf,bufSize);        
		break;
	case 1:
             crcValue16 = 0xffff;
             crcValue16 = HI_MDM_Crc16(crcValue16,buf,bufSize);
             crcValue32 = 0xffffffff;
		crcValue32 = HI_MDM_Crc32(crcValue32,buf,bufSize);
		break;
	case 2:
		buf[0]=16;
		crcValue16 = HI_MDM_Crc16(crcValue16,buf,bufSize);
		break;
	case 3:
		buf[0]=32;
		crcValue32 = HI_MDM_Crc32(crcValue32,buf,bufSize);
		break;
	case 4:
		bufSize = 0;
		crcValue16 = HI_MDM_Crc16(crcValue16,buf,bufSize);
		break;
	case 5:
		bufSize = 0;
		crcValue32 = HI_MDM_Crc32(crcValue32,buf,bufSize);
		break;
	case 6:
		bufSize= 1;
		crcValue16 = HI_MDM_Crc16(crcValue16,buf,bufSize);
		break;
	case 7:
		bufSize = 1;
		crcValue32 = HI_MDM_Crc32(crcValue32,buf,bufSize);
		break;
	default:
		CTS_PRINT0("cts_HI_MDM_Crc default");
		break;
    	}
 	CTS_PRINT2("test CRC, crcValue32=%x, crcValue16=%x",crcValue32,crcValue16);
	
    CTS_PRINT0("end cts_HI_MDM_Crc");
    return HI_CTS_CheckTest(uObjId, HI_TRUE); // HI_TRUE 表示用例执行成功 
}


