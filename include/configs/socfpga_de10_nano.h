/*
 * Copyright (C) 2017, Intel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TERASIC_DE10_H__
#define __CONFIG_TERASIC_DE10_H__

#include <asm/arch/base_addr_ac5.h>

#define CONFIG_SYS_BOOTM_LEN	(64 << 20)

/* U-Boot Commands */
#define CONFIG_FAT_WRITE
#define CONFIG_HW_WATCHDOG

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE		0x40000000	/* 1GiB */

/* Booting Linux */
#define CONFIG_BOOTFILE		"fitImage"
#define CONFIG_BOOTARGS		"console=ttyS0," __stringify(CONFIG_BAUDRATE) " mem=512M memmap=512M$512M"
#define CONFIG_BOOTCOMMAND	"run mmcload; run mmcboot"
#define CONFIG_LOADADDR		0x01000000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

/* Ethernet on SoC (EMAC) */
#if defined(CONFIG_CMD_NET)
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031
#endif

#define CONFIG_ENV_IS_IN_MMC

#ifndef CONFIG_SYS_MMCSD_FS_BOOT_PARTITION
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION 1
#endif

#ifndef CONFIG_SYS_MMCSD_FS_OS_PARTITION
#define CONFIG_SYS_MMCSD_FS_OS_PARTITION 2
#endif

/* Extra Environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"preboot=usb start\0" \
	"loadaddr=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"bootimage=zImage\0" \
	"fdt_addr=100\0" \
	"fdtimage=socfpga.dtb\0" \
	"fpgadata=0x02000000\0" \
	"core=init.rbf\0" \
	"fpgaload=load mmc 0:$mmc_boot $fpgadata $core; fpga load 0 $fpgadata $filesize; bridge enable\0" \
	"scrload=load mmc 0:${mmc_boot} ${loadaddr} u-boot.scr;source ${loadaddr}\0" \
	"scrtest=if test -e mmc 0:$mmc_boot /u-boot.scr;then run scrload;fi\0" \
	"ethaddr=02:03:04:05:06:07\0" \
	"bootm ${loadaddr} - ${fdt_addr}\0" \
	"mmc_boot=" __stringify(CONFIG_SYS_MMCSD_FS_BOOT_PARTITION) "\0" \
	"mmc_os=" __stringify(CONFIG_SYS_MMCSD_FS_OS_PARTITION) "\0" \
	"mmcroot=/dev/mmcblk0p" __stringify(CONFIG_SYS_MMCSD_FS_OS_PARTITION) "\0" \
	"mmcboot=setenv bootargs " CONFIG_BOOTARGS " root=${mmcroot} rw rootwait;" "bootz ${loadaddr} - ${fdt_addr}\0" \
	"mmcload=mmc rescan;" \
		"run fpgaload;" \
		"run scrtest;" \
		"load mmc 0:${mmc_os} ${loadaddr} /boot/${bootimage};" \
		"load mmc 0:${mmc_os} ${fdt_addr} /boot/${fdtimage}\0" \

/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TERASIC_DE10_H__ */
