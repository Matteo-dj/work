
/**@defgroup hct_print ��ӡ  
 * @ingroup dfx
 */

/** 
* @file hi-early-debug.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
* 
* ���������ļ��������ڵ��Դ�ӡ�ӿڡ�\n
* @li ���ڵ��Դ�ӡ�ӿ�Ĭ��ͨ��UART1��ӡ��ͨ�Ų���:������115200������λ8λ����У�飬1λֹͣλ��
* @li �û�����ͨ���޸�BOOTUP_UART_BASEѡ�����ĸ����Դ��ڽ��д�ӡ��
* @li ʹ�ô��ڵ������ּ��ɽ��յ���ӡ��Ϣ��HSO��console�޷���ʾ��ӡ����Ϣ��
* @li �û�����ͨ��edb_set_print_level�޸Ĵ�ӡ�ȼ��������Ƿ���д�ӡ��ϵͳĬ���ڳ�ʼ�����֮�󽫴�ӡ���ȼ��޸�Ϊ0���ڷ�������ʱ����ӡ���ȼ��޸�Ϊ1��
* @li ���ڴ�ӡΪ��ʱ����������벻Ҫ������ҵ�����(�ر���Ƶ�����õ�����)�е��ô��ڴ�ӡ�ӿڡ�
* @li printf��dprintf�ӿڣ��������������ź�����������˽�ֹ���жϺ͹��ж�ʱ����printf��dprintf�ӿڡ�
*
*/

#ifndef __HI_EARLY_DEBUG_H__
#define __HI_EARLY_DEBUG_H__

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ���
*
* @par ����:
* ��ӡ�ַ������������з��š�
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see edb_put_str_p0
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_only(const char *str);

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ�����
*
* @par ����:
* ��ӡ�ַ����������з���
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see edb_put_str_only
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p0(const char *str);

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ�����һ��������
*
* @par ����:
* ��ӡ�ַ�����һ�������������С�
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
* @param p1 [IN] ��һ��������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p1(const char *str, const unsigned int p1);

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ���������������
*
* @par ����:
* ��ӡ�ַ��������������������С�
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
* @param p1 [IN] ��һ��������
* @param p2 [IN] �ڶ���������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p2(const char *str, const unsigned int p1, const unsigned int p2);

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ���������������
*
* @par ����:
* ��ӡ�ַ��������������������С�
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
* @param p1 [IN] ��һ��������
* @param p2 [IN] �ڶ���������
* @param p3 [IN] ������������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p3(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3);

/**
* @ingroup  hct_print
* @brief  ��ӡ�ַ������ĸ�������
*
* @par ����:
* ��ӡ�ַ������ĸ������������С�
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param str [IN] �ַ�����
* @param p1 [IN] ��һ��������
* @param p2 [IN] �ڶ���������
* @param p3 [IN] ������������
* @param p4 [IN] ���ĸ�������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p4(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3, const unsigned int p4);

/**
* @ingroup  hct_print
* @brief ��ӡBUF���ݡ�
*
* @par ����:
* �����ض���ʽ��ӡBUF��
* @attention 
* @li �ýӿ�֧������������������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή���������ʵʱ�ԡ�
* @li �ýӿ�֧����LISR��������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã������ڴ�ӡ�Ǻ�ʱ�������ή��������жϵ���ʵʱ�ԡ�
*
* @param buf [IN] bufָ�롣
* @param len [IN] buf��С��
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_put_buf(const char *buf, const unsigned int len);

/**
* @ingroup  hct_print
* @brief ��ʽ����ӡ��
*
* @par ����:
* ��ʽ����ӡ��
* @attention �����л��ж���380Byteջ�ռ�ʹ�á�
*
* @param fmt [IN] ��ʽ���ַ�����
* @param ... [IN] ��ʽ��������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern int  printf(const char *fmt, ...);

/**
* @ingroup  hct_print
* @brief ��ʽ����ӡ��
*
* @par ����:
* ��ʽ����ӡ��
* @attention 1.�����л��ж���380Byteջ�ռ�ʹ�á�\n
*            2.��������������ͻ�ʹ��printf��
* @param fmt [IN] ��ʽ���ַ�����
* @param ... [IN] ��ʽ��������
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/

extern void dprintf(const char *fmt, ...);

/**
* @ingroup  hct_print
* @brief ���ô�ӡ���ȼ���
*
* @par ����:
* ���ô�ӡ���ȼ���
* @attention �ޡ�
*
* @param level [IN] ��ӡ���ȼ�������ֵΪ0:�رմ�ӡ���ܣ�����ֵΪ1:�򿪴�ӡ���ܡ�
*
* @par Dependency:
* @li hi-early-debug.h: �ýӿ��������ڵ�ͷ�ļ���
* @see ��
* @since HiMDL_V200R001C00
*/
extern void edb_set_print_level(unsigned int level);

#endif

