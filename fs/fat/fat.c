/*
 * fat.c
 *
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to uboot
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <blk.h>
#include <config.h>
#include <exports.h>
#include <fat.h>
#include <asm/byteorder.h>
#include <part.h>
#include <malloc.h>
#include <memalign.h>
#include <linux/compiler.h>
#include <linux/ctype.h>
#include "ff.h"

static struct blk_desc *cur_dev;
static disk_partition_t cur_part_info;

#define DOS_BOOT_MAGIC_OFFSET	0x1fe
#define DOS_FS_TYPE_OFFSET	0x36
#define DOS_FS32_TYPE_OFFSET	0x52
#define DOS_FS64_TYPE_OFFSET	0x03

int fat_blk_disk_read(__u32 block, __u32 nr_blocks, void *buf)
{
	ulong ret;

	if (!cur_dev)
		return -1;

	ret = blk_dread(cur_dev, block, nr_blocks, buf);

	if (nr_blocks && ret == 0)
		return -1;

	return ret;
}

static int disk_read(__u32 block, __u32 nr_blocks, void *buf)
{
	ulong ret;

	if (!cur_dev)
		return -1;

	ret = blk_dread(cur_dev, cur_part_info.start + block, nr_blocks, buf);

	if (nr_blocks && ret == 0)
		return -1;

	return ret;
}

FATFS FatFs;

int fat_set_blk_dev(struct blk_desc *dev_desc, disk_partition_t *info)
{
	ALLOC_CACHE_ALIGN_BUFFER(unsigned char, buffer, dev_desc->blksz);

	cur_dev = dev_desc;
	cur_part_info = *info;

	/* Make sure it has a valid FAT header */
	if (disk_read(0, 1, buffer) != 1) {
		cur_dev = NULL;
		return -1;
	}

	/* Check if it's actually a DOS volume */
	if (memcmp(buffer + DOS_BOOT_MAGIC_OFFSET, "\x55\xAA", 2)) {
		cur_dev = NULL;
		return -1;
	}

	/* Check for FAT12/FAT16/FAT32 filesystem */
	if(!memcmp(buffer + DOS_FS_TYPE_OFFSET, "FAT", 3) || !memcmp(buffer + DOS_FS32_TYPE_OFFSET, "FAT32", 5) || !memcmp(buffer + DOS_FS64_TYPE_OFFSET, "EXFAT", 5))
	{
		if(FR_OK == f_mount(&FatFs, "", 1)) return 0;
	}

	cur_dev = NULL;
	return -1;
}

int fat_register_device(struct blk_desc *dev_desc, int part_no)
{
	disk_partition_t info;

	/* First close any currently found FAT filesystem */
	cur_dev = NULL;

	/* Read the partition table, if present */
	if (part_get_info(dev_desc, part_no, &info)) {
		if (part_no != 0) {
			printf("** Partition %d not valid on device %d **\n",
					part_no, dev_desc->devnum);
			return -1;
		}

		info.start = 0;
		info.size = dev_desc->lba;
		info.blksz = dev_desc->blksz;
		info.name[0] = 0;
		info.type[0] = 0;
		info.bootable = 0;
#if CONFIG_IS_ENABLED(PARTITION_UUIDS)
		info.uuid[0] = 0;
#endif
	}

	return fat_set_blk_dev(dev_desc, &info);
}

int file_fat_detectfs(void)
{
	printf("\nnot implemented yet.\n");
	return 0;
}

int file_fat_ls(const char *dir)
{
	printf("\nContent of %s\n\n", dir);

    	FRESULT res;
	DIR d;
    	FILINFO fno;

    	res = f_opendir(&d, dir);
    	if(res == FR_OK)
	{
		for (;;)
		{
            		res = f_readdir(&d, &fno);
            		if (res != FR_OK || fno.fname[0] == 0) break;
			if (fno.fattrib & AM_DIR)
 			{
                		printf("     <DIR> %s\n", fno.fname);
            		}
			else
			{
                		printf("%10llu %s\n", fno.fsize, fno.fname);
	            	}
        	}
        	f_closedir(&d);
    	}

	return 0;
}

int fat_exists(const char *filename)
{
	FILINFO fno;
	return FR_OK == f_stat(filename, &fno);
}

int fat_size(const char *filename, loff_t *size)
{
	FILINFO fno;
	*size = 0;
	if(FR_OK == f_stat(filename, &fno))
	{
		*size = fno.fsize;
		return 0;
	}

	return -1;
}

int file_fat_read_at(const char *filename, loff_t pos, void *buffer,
		     loff_t maxsize, loff_t *actread)
{
	FIL fil;
	FRESULT fr;
	*actread = 0;

	printf("reading %s\n", filename);

	fr = f_open(&fil, filename, FA_READ);
	if(fr != FR_OK) return -1;

	fr = f_lseek(&fil, pos);
	if(fr != FR_OK) 
	{
		f_close(&fil);
		return -1;
	}

	UINT rd;
	fr = f_read(&fil, buffer, maxsize ? maxsize : f_size(&fil), &rd);
	*actread = rd;
	f_close(&fil);
	return (fr == FR_OK) ? 0 : -1;
}

int file_fat_read(const char *filename, void *buffer, int maxsize)
{
	loff_t actread;
	int ret;

	ret =  file_fat_read_at(filename, 0, buffer, maxsize, &actread);
	if (ret)
		return ret;
	else
		return actread;
}

int fat_read_file(const char *filename, void *buf, loff_t offset, loff_t len,
		  loff_t *actread)
{
	int ret;

	ret = file_fat_read_at(filename, offset, buf, len, actread);
	if (ret)
		printf("** Unable to read file %s **\n", filename);

	return ret;
}

void fat_close(void)
{
}
