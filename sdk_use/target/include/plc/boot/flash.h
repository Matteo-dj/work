#ifndef _INCLUDE_BOOT_FLASH_H_
#define _INCLUDE_BOOT_FLASH_H_

#include <boot/flash_partition.h>

/* This constant declares the max. oobsize / page, which
 * is supported now. If you add a chip with bigger oobsize/page
 * adjust this accordingly.
 */
#define NAND_MAX_OOBSIZE	512 
#define NAND_MAX_PAGESIZE	8192

typedef struct ptentry ptentry;

/* flash partitions are defined in terms of blocks
** (flash erase units)
** @flags: to indentify this entry belongs to which flash
*/
struct flash_ops {

	int (*flash_init)(void);
	int (*flash_destory)(void);

	int (*flash_erase)(struct ptentry* ptn);
	int (*flash_read)(struct ptentry* ptn, int ext, unsigned long offset,
			void* data, unsigned long bytes);

	int (*flash_write)(struct ptentry* ptn, unsigned long *offset, int ext,
			const void* data, unsigned long bytes);

	int (*get_flash_total_size)(unsigned long *size);
	int (*get_flash_chip_size)(unsigned long *size);
	int (*get_flash_block_size)(unsigned int *size);
	int (*get_flash_page_size)(unsigned int *size);
	int (*get_flash_oob_size)(unsigned int *size);
	int (*get_flash_chip_nr)(unsigned int *nr);
};

struct flash_entry{
	char* name;
	int (*regist_flash)(struct flash_entry* flash_entry);
	struct flash_ops* ops;
	unsigned long size;
	unsigned flags;		//which type of flash we are
}; 

/* tools to populate and query the partition table */
void flash_init_ptn(void);
int flash_add_ptn(ptentry *ptn);
void flash_clear_ptn(void);
ptentry *flash_find_ptn(const char *name);
ptentry *flash_get_ptn(unsigned n);
unsigned flash_get_ptn_count(void);
void flash_dump_ptn(void);

int flash_init(void);
int flash_erase(ptentry *ptn);
int flash_read(ptentry *ptn, unsigned long offset, 
               void *data, unsigned long bytes);
int flash_write(ptentry* ptn, unsigned long *offset, int ext,
		const void* data, unsigned long bytes);

/* Get total size of special flash type */
int flash_total_size(unsigned int type, unsigned long *size);
/* Get one chip size of special type flash*/
int flash_chip_size(unsigned int type, unsigned long *size);
/* Get erase size of special flash */
int flash_block_size(unsigned int type, unsigned int *erasesize);
/* Get oob size of nand flash, 
 * any other flash will not support this interface
 */
int flash_oob_size(unsigned int type, unsigned int *ext_size);
/* Get write size of special flash */
int flash_page_size(unsigned int type, unsigned int *write_size);
/* Get how many chips of special flash on our board */
int flash_chip_nr(unsigned int type, unsigned int *nr);


struct flash_write_opt{
	struct ptentry *ptn;
	unsigned long rx_length;
	unsigned long offset;
	unsigned int extra;
	unsigned int is_end;
	unsigned int align_size;
};

int flash_update_open(struct flash_write_opt *fw_opt, 
		ptentry *ptn_write, unsigned long rx_length);
int flash_update(struct flash_write_opt *fw_opt,const void *data,
		unsigned length, int * actual);
int flash_update_close(struct flash_write_opt *fw_opt);

#endif
