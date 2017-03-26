/*
 * Copyright (C) 2017, Intel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TERASIC_DE10_H__
#define __CONFIG_TERASIC_DE10_H__

#include <asm/arch/base_addr_ac5.h>

/* U-Boot Commands */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_DOS_PARTITION
#define CONFIG_FAT_WRITE
#define CONFIG_HW_WATCHDOG

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE		0x40000000	/* 1GiB */

/* Booting Linux */
#define CONFIG_BOOTFILE		"fitImage"
#define CONFIG_BOOTARGS		"console=ttyS0," __stringify(CONFIG_BAUDRATE)
#define CONFIG_BOOTCOMMAND	"run mmcload; run mmcboot"
#define CONFIG_LOADADDR		0x01000000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

/* Ethernet on SoC (EMAC) */
#if defined(CONFIG_CMD_NET)
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031
#endif

#define CONFIG_ENV_IS_IN_MMC

/* Extra Environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"ramboot=setenv bootargs " CONFIG_BOOTARGS ";" \
		"bootm ${loadaddr} - ${fdt_addr}\0" \
	"bootimage=zImage\0" \
	"fdt_addr=100\0" \
	"fdtimage=socfpga_cyclone5_de10_sockit.dtb\0" \
	"bootm ${loadaddr} - ${fdt_addr}\0" \
	"mmc_boot=" __stringify(CONFIG_SYS_MMCSD_FS_BOOT_PARTITION) "\0" \
	"mmc_os=" __stringify(CONFIG_SYS_MMCSD_FS_OS_PARTITION) "\0" \
	"mmcroot=/dev/mmcblk0p${mmc_os}\0" \
	"mmcboot=setenv bootargs " CONFIG_BOOTARGS \
		" root=${mmcroot} rw rootwait;" \
		"bootz ${loadaddr} - ${fdt_addr}\0" \
	"mmcload=mmc rescan;" \
		"load mmc 0:${mmc_boot} ${loadaddr} ${bootimage};" \
		"load mmc 0:${mmc_boot} ${fdt_addr} ${fdtimage}\0" \

/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TERASIC_DE10_H__ */
