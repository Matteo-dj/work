#include <types.h>
#include <ctype.h>
#include <command.h>
#ifdef BSP_CODEX_CHECK_CLOSE
volatile u32    g_manage_authority = FALSE;
#endif
volatile u32    Unused_Parameter;
#define UNUSED_PARAMETER(x)     (Unused_Parameter = ((u32)(x)))
extern cmd_tbl_t _u_boot_list_2_cmd_2_md;
extern cmd_tbl_t _u_boot_list_2_cmd_2_mw;
extern cmd_tbl_t _u_boot_list_2_cmd_2_sf;
extern cmd_tbl_t _u_boot_list_2_cmd_2_go;
extern cmd_tbl_t _u_boot_list_2_cmd_2_reset;
extern cmd_tbl_t _u_boot_list_2_cmd_2_loady;
extern cmd_tbl_t _u_boot_list_2_cmd_2_burnupg;
extern cmd_tbl_t _u_boot_list_2_cmd_2_burnbin;
#ifdef BSP_CODEX_CHECK_CLOSE
extern cmd_tbl_t _u_boot_list_2_cmd_2_password;
#endif
void boot_regist_cmd(void)
{
    //新增命令需要加在后面
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_md);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_mw);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_sf);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_go);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_reset);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_loady);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_burnupg);
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_burnbin);
#ifdef BSP_CODEX_CHECK_CLOSE    
    UNUSED_PARAMETER(&_u_boot_list_2_cmd_2_password);
#endif
}

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
					? toupper((unsigned char)(*cp)) : (unsigned char)*cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

