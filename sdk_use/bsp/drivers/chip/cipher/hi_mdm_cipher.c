#include <hi_types.h>
#include <hi_mpi_cipher.h>

HI_U32 HI_MDM_HASH_CalcInit(HI_HANDLE pHandle, HI_MDM_HASH_ATTR_S *pstHashAttr)
{
    return HI_MPI_HASH_CalcInit(pstHashAttr, pHandle);
}


HI_U32 HI_MDM_HASH_CalcUpdate(HI_HANDLE pHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    return HI_MPI_HASH_CalcUpdate(pHandle, pu8InputData, u32InputDataLen);
}


HI_U32 HI_MDM_HASH_CalcFinal(HI_HANDLE pHandle, HI_U8 *pu8OutputHash)
{
    return HI_MPI_HASH_CalcFinal(pHandle, pu8OutputHash);
}


HI_U32 HI_MDM_HASH_Sha256(HI_U8 *pu8InputData, HI_U32 u32InputDataLen, HI_U8 *pu8OutputHash)
{
    HI_U32 ulRet = 0;
    HI_U32 ulHandle = 0;
    HI_MDM_HASH_ATTR_S stHashAttr = {0};

    if ((HI_NULL == pu8InputData) || (u32InputDataLen == 0) || (HI_NULL == pu8OutputHash))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    stHashAttr.eShaType = HI_MDM_HASH_TYPE_SHA256;
    stHashAttr.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
    ulRet = HI_MPI_HASH_CalcInit(&stHashAttr, (HI_HANDLE)&ulHandle);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = HI_MPI_HASH_CalcUpdate((HI_HANDLE)&ulHandle, pu8InputData, u32InputDataLen);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = HI_MPI_HASH_CalcFinal((HI_HANDLE)&ulHandle, pu8OutputHash);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

