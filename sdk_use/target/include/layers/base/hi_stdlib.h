/**@defgroup hct_stdlib ��ȫC
 * @ingroup os
 * ��ȫC�ӿ���Ҫ�ǻ��ڱ�׼C�ӿ�������һ����������д������ַ�ռ����ޣ��Ӷ���ֹдԽ�硣
 */
//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : hi_stdlib.h
//  �� �� ��   : V1.0
//  ��������   : 2017��04��14��
//  ��������   : C��ӿ�
//  �����б�   :
//****************************************************************************
#ifndef __HI_STDLIB_H__
#define __HI_STDLIB_H__

//****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>
#if !defined(MAC_V200_LLT)
#include <los_typedef.h>
#endif
HI_START_HEADER

//****************************************************************************
//*****************************************************************************
// ��ע: ��ǰ�汾�������˰�ȫC��ӿڷ�֧���¸��汾������C�⺯����֧
#ifndef errno_t
typedef int errno_t;
#endif

/**
* @ingroup  hct_stdlib
* @brief  ����Դ�ַ�����Ŀ�Ļ�������
*
* @par ����:
* ����Դ�ַ�����Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [OUT] ����#char *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #char * ��Դ��������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t strcpy_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  hct_stdlib
* @brief  ����ָ������Դ�ַ�����Ŀ�Ļ�������
*
* @par ����:
* ����ָ������Դ�ַ�����Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [IN] ����#char *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #char * ��Դ��������
* @param count [IN] ����#size_t����Դ�������и��Ƶ��ַ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t strncpy_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  ��Դ�ַ������ӵ�Ŀ���ַ�������
*
* @par ����:
* ����ָ������Դ�ַ�����Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [IN] ����#char *�� Ŀ�Ļ�������
* @param dest [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #char * ��Դ��������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t strcat_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  hct_stdlib
* @brief  ��ָ�����ȵ�Դ�ַ������ӵ�Ŀ���ַ������档
*
* @par ����:
* ��ָ�����ȵ�Դ�ַ������ӵ�Ŀ���ַ������档
* @attention �ޡ�
*
* @param dest [IN] ����#char *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #char * ��Դ��������
* @param count [IN] ����#size_t��Դ��������Ҫ���ӵ��ַ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t strncat_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  ����Դ�����������ݵ�Ŀ�Ļ�������
*
* @par ����:
* ����Դ�����������ݵ�Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [IN] ����#void *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #void * ��Դ��������
* @param count [IN] ����#HI_U32����Դ�������и����ַ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t memcpy_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  ����Ŀ�Ļ�����Ϊ�ض�ֵ��
*
* @par ����:
* ����Ŀ�Ļ�����Ϊ�ض�ֵ��
* @attention �ޡ�
*
* @param dest [IN] ����#void *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param c [IN] ����#int ��Ҫ����Ŀ�Ļ������е��ַ���
* @param count [IN] ����#size_t��Ҫ����Ŀ�Ļ������ķ���������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t memset_s(void *dest, size_t dest_max, int c, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  �ƶ�Դ�����������ݵ�Ŀ�Ļ�������
*
* @par ����:
* �ƶ�Դ�����������ݵ�Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [IN] ����#void *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param src [IN] ����#const #void* ��Դ��������
* @param count [IN] ����#size_t����Դ���������ƶ����ַ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern errno_t memmove_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  ���ַ�������ָ���ķָ����ָ�����ַ�����
*
* @par ����:
* �ƶ�Դ���ַ�������ָ���ķָ����ָ�����ַ�����
* @attention �ޡ�
*
* @param token [IN] ����#char *�� Ҫ�ָ���ַ�����
* @param delimit [IN] ����#const char *�� �ַ����ָ�����
* @param context [IN] ����#char* ���������HI_strtok_s���λ����Ϣ��
*
* @retval #char*����ָ�� ָ����token�е���һ��token��
* @retval #HI_NULL û���ҵ�ָ�������ַ������߷�������
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern char *strtok_s(char *token, const char *delimit, char **context);

/**
* @ingroup  hct_stdlib
* @brief  �����ݸ�ʽ�������Ŀ�Ļ�������
*
* @par ����:
* �����ݸ�ʽ�������Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [IN] ����#char *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param format [IN] ����#const #char *�� ��ʽ�������ַ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ �����롣
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
extern int sprintf_s(char *dest, size_t dest_max, const char *format, ...);

/**
* @ingroup  hct_stdlib
* @brief �����ݰ���ָ�����ȸ�ʽ�������Ŀ�Ļ�������
*
* @par ����:
* �����ݰ���ָ�����ȸ�ʽ�������Ŀ�Ļ�������
* @attention �ޡ�
*
* @param dest [OUT] ����#char *�� Ŀ�Ļ�������
* @param dest_max [IN] ����#size_t�� Ŀ�Ļ�������С��
* @param size_t [IN] ����#size_t�� Ҫ�����Ŀ�Ļ������ĸ�ʽ���ַ�������
* @param format [IN] ����#const #char *�� ��ʽ�������ַ�����
* @param ... [IN] ��ѡ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ �����롣
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
extern int snprintf_s(char *dest, size_t dest_max, size_t count, const char *format, ...);

/*
�ǰ�ȫ�ӿ�
*/

/**
* @ingroup  hct_stdlib
* @brief  �����ַ������ȡ�
*
* @par ����:
* �����ַ������ȡ�
* @attention �ޡ�
*
* @param str [IN] ����#const #char *�� �ַ�����
*
* @retval #HI_U32������ֵ �ַ������ȡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern size_t strlen(const char *str);

/**
* @ingroup  hct_stdlib
* @brief �Ƚ� �ַ����Ƿ���ͬ��
*
* @par ����:
* �Ƚ� �ַ����Ƿ���ͬ��
* @attention �ޡ�
*
* @param buf1 [IN] ����#const #void *�� �ַ���1��
* @param buf2 [IN] ����#const #void *�� �ַ���2��
* @param count [IN] ����#size_t�� ���Ƚϵĳ��ȡ�
*
* @retval 0 ���ַ�����ȡ�
* @retval ��0 ���ַ�������ȡ�
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern int memcmp(const void *buf1, const void *buf2, size_t count);

/**
* @ingroup  hct_stdlib
* @brief ������nptr�ַ������ݲ���base��ת�����޷��ŵĳ���������
*
* @par ����:
* ������nptr�ַ������ݲ���ibase��ת�����޷��ŵĳ���������
* @attention �ޡ�
*
* @param nptr [IN] ����#const #char *��Ҫת�����ַ�����
* @param endptr [IN] ����#char **�� ��һ������ת�����ַ���ָ�롣
* @param base [IN] ����#int���ַ��� str �����õĽ��ơ�
*
* @retval unsigned long����ֵ ת������޷��ų���������
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern unsigned long strtoul(const char *nptr, char **endptr, int base);

/**
* @ingroup  hct_stdlib
* @brief �ж��ַ���s2�Ƿ���s1���Ӵ���
*
* @par ����:
* �ж��ַ���s2�Ƿ���s1���Ӵ���
* @attention �ޡ�
*
* @param s1 [IN] ����#const #char *��Դ�ַ�����
* @param s2 [IN] ����#const #char *�����ַ�����
*
* @retval #char *ָ�� s2��s1���״γ��ֵĵ�ַ��
* @retval #HI_NULL �����Ӵ���
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#ifndef MAC_V200_LLT
extern char *strstr(const char *s1, const char *s2);
#endif

/**
* @ingroup  hct_stdlib
* @brief ��s1��ǰlen���ַ��в���s2��
*
* @par ����:
* ��s1��ǰlen���ַ��в���s2��
* @attention �ޡ�
*
* @param s1 [IN] ����#const #char *��Դ�ַ�����
* @param s2 [IN] ����#const #char *�����ַ�����
* @param len [IN] ����#size_t����ǰlen���ַ��в��ҡ�
*
* @retval #char *ָ�� s2��s1���״γ��ֵĵ�ַ��
* @retval #HI_NULL û���ҵ��������Ӵ���
*
* @par Dependency:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
extern char *strnstr(const char *s1, const char *s2, size_t len);

/**
* @ingroup  hct_stdlib
* @brief ��Դ�ַ����в����ַ���
*
* @par ����:
* ��Դ�ַ����в����ַ���
* @attention
* @li �����ҵ��ַ�Ϊ#int�ͣ��ӿڽ���ת��Ϊ�ַ����ٲ��ҡ�
* @li Դ�ַ����еĽ�����'\0'ҲΪ�������ַ���
*
* @param s1 [IN] ����#const #char *��Դ�ַ�����
* @param s2 [IN] ����#int���������ַ���
*
* @retval #HI_CHAR �ҵ����ַ��������ַ���Դ�ַ����еĵ�ַ��
* @retval #HI_NULL û���ҵ���
*
* @par ����:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#ifndef MAC_V200_LLT
extern char *strchr(const char *s1, int s2);
#endif

extern int atoi(const char *str);

HI_END_HEADER

#endif //__HI_STDLIB_H__

