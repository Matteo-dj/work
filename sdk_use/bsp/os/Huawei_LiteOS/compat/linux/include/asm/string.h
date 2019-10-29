#ifndef __ASM_STRING_H__
#define __ASM_STRING_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "los_typedef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/


extern void bzero(void *s, size_t n);

extern void *memcpy(void *dst, const void *src, size_t n);

extern void *memmove(void *dst, const void *src, size_t n);

extern void *memset(void *dst, int c, size_t n);

extern char *strcpy(char *dst, const char *src);

extern size_t strlcpy(char *dst, const char *src, size_t n);

extern char *strncpy(char *dst, const char *src, size_t n);

extern char *strcat(char *dst, const char *src);

extern char *strncat(char *dst, const char *src, size_t n);

extern int memcmp(const void *str1, const void *str2, size_t n);

extern int strcasecmp(const char *s1, const char *s2);

extern int strncasecmp(const char *s1, const char *s2, size_t n);

extern int strcmp(const char *str1, const char *str2);

extern int strcoll(const char *str1, const char *str2);

extern int strncmp(const char *str1, const char *str2, size_t n);

extern size_t strxfrm(char *dst, const char *src, size_t n);

extern void *memchr(const void *src, int c, size_t n);

extern char *strchr(const char *src, int c);

extern size_t strcspn(const char *str1, const char *str2);

extern char *strpbrk(const char *str1, const char *str2);

extern char *strrchr(const char *src, int c);

extern size_t strspn(const char *str1, const char *str2);

extern char *strstr(const char *str1, const char *str2);

extern char *strtok_r( char *str,  const char *delim,  char **saveptr);

extern  char *strtok(char *src, const char *delim);

extern size_t strlen(const char *src);

extern char *strdup(const char *src);

extern char* strerror(int error_number);

extern char *strcasestr(const char * s, const char * find);

extern char *strsep(char **stringp, const char *delim);

extern unsigned long strtoul(const char *nptr, char **endptr, int base);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ASM_STRING_H__ */
