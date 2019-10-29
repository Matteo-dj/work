#ifndef __FLASH_PARTITION_H__
#define __FLASH_PARTITION_H__
/* Partition flags defination */
#define FLASH_TYPE_MASK         0x0F
#define FLASH_TYPE_UNKNOW       0x00
#define FLASH_TYPE_NAND         0x01
#define FLASH_TYPE_SPI          0x02
#define FLASH_TYPE_EMMC         0x03
#define FLASH_TYPE_NOR          0x04
#define FLASH_TYPE_MAX          (FLASH_TYPE_NOR)
	
#define FS_TYPE_MASK            0xF0
#define FS_TYPE_NONE            0x00
#define FS_TYPE_YAFFS           0x10
#define FS_TYPE_CRAMFS          0x20
#define FS_TYPE_JAFFS           0x30
#define FS_TYPE_MAX             (FS_TYPE_JAFFS)

/* Max partitions defination */
#define MAX_FLASH_PTN_ITEM	32
/* flash partitions are defined in terms of blocks
** (flash erase units)
** @flags: to indentify this entry belongs to which flash
*/
struct ptentry
{
	char name[16];
	unsigned long  start;
	unsigned long length;
	unsigned flags;
};

#endif

