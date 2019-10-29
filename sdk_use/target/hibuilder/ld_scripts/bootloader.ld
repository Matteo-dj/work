;/************************************************************************
;*
;* DESCRIPTION
;*
;*       Linker description (RAM) file for the following:
;*
;*       BOARD:      AT91SAM9261-EK_REV0
;*
;************************************************************************/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(boot_start)
SECTIONS
{
	. = 0x00000000;
	BOOTLOADER_START = .;

	. = ALIGN(8);
	.text :	{
		*(.text)
	}

	. = ALIGN(8);
	.data : {
		*(.data)
	}
	
	. = ALIGN(4);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	}

	. = ALIGN(8);
	__bss_start = .;
	.bss  : { 
		BOOTLOADER_BSS = . ;
		. = ALIGN(8);
		*(.bss)
		. = ALIGN(8);
		*(.bss*)
		. = ALIGN(8);
		*(COMMON) 
		. = ALIGN(8);
	}
	_end = .;

	BOOTLOADER_END = . ;

	. = ALIGN(8);
	BOOTLOADER_START_ADDR = (0x70155000);
}
