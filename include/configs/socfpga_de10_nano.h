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
#define CONFIG_FS_FAT
#define CONFIG_HW_WATCHDOG

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE		0x40000000	/* 1GiB */

/* Booting Linux */
#define CONFIG_BOOTFILE		"fitImage"
#define CONFIG_BOOTARGS		"console=ttyS0," __stringify(CONFIG_BAUDRATE) " $v loop.max_part=8 isolcpus=1 mem=511M memmap=513M$511M"
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

/* Extra Environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"preboot=usb start\0" \
	"loadaddr=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"bootimage=zImage_dtb\0" \
	"fdt_addr=100\0" \
	"fdtimage=socfpga.dtb\0" \
	"fpgadata=0x02000000\0" \
	"core=menu.rbf\0" \
	"fpgacheck=if mt 0xFFD05054 0;then run fpgaload;else if mt 0x1FFFF000 0x87654321;then env import 0x1FFFF004;run fpgaload;fi;fi\0" \
	"fpgaload=load mmc 0:$mmc_boot $fpgadata $core;fpga load 0 $fpgadata $filesize;bridge enable;mw 0x1FFFF000 0;mw 0xFFD05054 0x12345678\0" \
	"scrload1=load mmc 0:$mmc_boot $loadaddr /linux/u-boot.scr;source $loadaddr\0" \
	"scrtest=if test -e mmc 0:$mmc_boot /linux/u-boot.scr;then run scrload1;fi\0" \
	"ethaddr=02:03:04:05:06:07\0" \
	"bootm $loadaddr - $fdt_addr\0" \
	"mmc_boot=" __stringify(CONFIG_SYS_MMCSD_FS_BOOT_PARTITION) "\0" \
	"mmcroot=/dev/mmcblk0p" __stringify(CONFIG_SYS_MMCSD_FS_BOOT_PARTITION) "\0" \
	"v=loglevel=4\0" \
	"mmcboot=setenv bootargs " CONFIG_BOOTARGS " root=$mmcroot loop=linux/linux.img ro rootwait;" "bootz $loadaddr - $fdt_addr\0" \
	"mmcload=mmc rescan;" \
		"run fpgacheck;" \
		"run scrtest;" \
		"load mmc 0:$mmc_boot $loadaddr /linux/$bootimage;" \
		"setexpr.l fdt_addr $loadaddr + 0x2C;" \
		"setexpr.l fdt_addr *$fdt_addr + $loadaddr\0"

/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TERASIC_DE10_H__ */
