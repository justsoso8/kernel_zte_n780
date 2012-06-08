/*
 * drivers/media/video/msm/mt9t112_reg_qtech.c
 *
 * Refer to drivers/media/video/msm/mt9d112_reg.c
 * For MT9T112: 3.1Mp, 1/4-Inch System-On-A-Chip (SOC) CMOS Digital Image Sensor
 *
 * Copyright (C) 2009-2010 ZTE Corporation.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Created by jia.jia@zte.com.cn
 */
/*-----------------------------------------------------------------------------------------
  when         who          what, where, why                         comment tag
  --------     ----         -------------------------------------    ----------------------
  2011-03-17   lijing       adjust orientation of preview for P725A  ZTE_CAM_LJ_20110317
  2011-02-15   wt           change af model from change register     ZTE_CAM_WT_20110215
  2010-12-23   lijing       update prev_snap_tbl                     ZTE_CAM_LJ_20101223
  2010-12-16   zt           modify PLL setting                       ZTE_ZT_CAM_20101216
                            modify clk
                            modify init setting, i.e., prev_snap
                            modify LSC
                            modify setting of brightness
                            modify setting of saturation
                            modify setting of sharpness
                            setting of lens_for_outdoor and
                            lens_for_indoor are disused
  2010-12-15   jia.jia      add support for exposure compensation    ZTE_MSM_CAMERA_JIA_20101215
  2010-09-27   zt           eliminate the preview flash under the    ZTE_MSM_CAMERA_ZT_001
                            specific A light source
  2010-08-09   jia          modify setting of AWB and lensshading    ZTE_MSM_CAMERA_JIA_001
                            to improve effects of preview and
                            snapshot
  2010-07-28   li.jing      fix bug of white line display during     ZTE_MSM_CAMERA_LIJING_001
                            snapshot
  2010-07-21   jia          fix bug of wrong ptr of                  ZTE_MSM_CAMERA_JIA_001
                            "lens_for_indoor_tbl"
  2010-05-13   zh.shj       modify config of AWB & LSC settings      ZTE_MSM_CAMERA_ZHSHJ_001
                            for different projects
  2010-05-06   zh.shj       modify config of lens shading for Mcnex  ZTE_MSM_CAMERA_ZHSHJ_001
                            MT9T112 and add AF TWICE process to 
                            improve AF effect 
  2010-04-15   zh.shj       add process for different board types    ZTE_MSM_CAMERA_ZHSHJ_001
                            e.g., CONFIG_MACH_BLADE and CONFIG_
                            MACH_RAISE
  2010-03-13   zh.shj       improve performance of auto WB,          ZTE_MSM_CAMERA_ZHSHJ_001
                            contrast, and LS in indoor environment
  2010-03-03   zh.shj       Add process of lens shading before       ZTE_MSM_CAMERA_ZHSHJ_001
                            preview and snapshot
  2010-02-21   zh.shj       set sharpenss value according to         ZTE_MSM_CAMERA_ZHSHJ_001
                            different brightness level;
                            patched from Aptina, USA;
                            modify contrast value according to
                            patch provided by Aptina, USA;
                            enlarge AF windows;
                            improve AWB effect for new sensor;
  2010-02-04   zh.shj       improve effect of preview and snapshot   ZTE_MSM_CAMERA_ZHSHJ_001
                            after new module is available
  2010-01-28   zh.shj       modify setting of maximum brightness     ZTE_MSM_CAMERA_ZHSHJ_001
  2010-01-26   zh.shj       fix bug of inconsistence during setting  ZTE_MSM_CAMERA_ZHSHJ_001
                            minimum brightness
  2010-01-22   zh.shj       improve effect of preview and snapshot   ZTE_MSM_CAMERA_ZHSHJ_001
                            under poor light condition
  2010-01-14   zh.shj       modify config of sharpness               ZTE_MSM_CAMERA_ZHSHJ_001
  2010-01-06   zh.shj       modify config of PLL to make PCLK equal  ZTE_MSM_CAMERA_ZHSHJ_001
                            to MCLK
                            modify config of AWB and CCM
                            modify config of auto mode of white balance
                            modify config of brightness
  2010-01-05   zh.shj       modify config of contrast                ZTE_MSM_CAMERA_ZHSHJ_001
  2009-12-28   zh.shj       add config for MT9T112-3.1Mp-AF          ZTE_MSM_CAMERA_ZHSHJ_001
  2009-12-28   jia.jia      merged from mt9t111_reg_qtech.c          ZTE_MSM_CAMERA_JIA_001
  2009-12-21   zh.shj       improve effects of white balance         ZTE_MSM_CAMERA_ZHSHJ_001
  2009-12-21   chg          for vfe zoom                             CHG_CAM_20091217
  2009-12-19   zh.shj       add config for contrast                  ZTE_MSM_CAMERA_ZHSHJ_001
  2009-12-11   jia.jia      rename file with mt9t111_reg_qtech       ZTE_MSM_CAMERA_JIA_001
  2009-12-03   jia.jia      Improve efficiency of sensor init        ZTE_MSM_CAMERA_JIA_001
  2009-12-01   jia.jia      Refactor code for sensor init            ZTE_MSM_CAMERA_JIA_001
  2009-11-09   jia.jia      add wb and af settings                   ZTE_MSM_CAMERA_ZHSHJ_001
  2009-11-26   zh.shj       improve effects of AF, AWB and LSC;      ZTE_MSM_CAMERA_ZHSHJ_001
  2009-11-09   jia.jia      add orientation setting for preview      ZTE_MSM_CAMERA_ZHSHJ_001
                            and snapshot
  2009-11-05   zh.shj       mt9t111 preview function implemented     ZTE_MSM_CAMERA_ZHSHJ_001
  2009-10-24   jia.jia      Merged from kernel-v4515                 ZTE_MSM_CAMERA_JIA_001
------------------------------------------------------------------------------------------*/

#include "mt9t11x.h"

/* PLL Setup */
static struct mt9t11x_i2c_reg_conf const pll_tbl[] = {
    {0x001A, 0x0219 , WORD_LEN, 1},	// RESET_AND_MISC_CONTROL       
    {0x001A, 0x0018 , WORD_LEN, 10},// RESET_AND_MISC_CONTROL       

    {0x0014, 0x2425, WORD_LEN, 0}, 	// PLL_CONTROL
    {0x0014, 0x2145, WORD_LEN, 0},	//PLL control: BYPASS PLL = 8517
    {0x0010, 0x0820, WORD_LEN, 0},	//PLL Dividers = 2080
    {0x0012, 0x0070, WORD_LEN, 0},	//PLL P Dividers = 112
    {0x002A, 0x7788, WORD_LEN, 0},	//PLL P Dividers 4-5-6 = 30600
    {0x001A, 0x0218, WORD_LEN, 0},  //Reset Misc. Control = 536
    {0x0014, 0x2545, WORD_LEN, 0},	//PLL control: TEST_BYPASS on = 9541
    {0x0014, 0x2547, WORD_LEN, 0},	//PLL control: PLL_ENABLE on = 9543
    {0x0014, 0x2447, WORD_LEN, 0},	//PLL control: SEL_LOCK_DET on = 9287
    {0x0014, 0x2047, WORD_LEN, 10},	//PLL control: TEST_BYPASS off = 8263
    {0x0014, 0x2046, WORD_LEN, 0},	//PLL control: PLL_BYPASS off = 8262
    {0x0022, 0x0438, WORD_LEN, 0},	//Reference clock count for 20 us = 1080
    {0x001E, 0x0777, WORD_LEN, 0},	//Pad Slew Rate = 1911
    {0x0018, 0x402B, WORD_LEN, 0}, 	// STANDBY_CONTROL_AND_STATUS
    {0x3B82, 0x0004, WORD_LEN, 0}, 	// I2C_MASTER_CONTROL
    {0x0018, 0x402F, WORD_LEN, 0}, 	// STANDBY_CONTROL_AND_STATUS
    {0x0018, 0x402E, WORD_LEN, 50}, // STANDBY_CONTROL_AND_STATUS
};

/* Clock Setup */
static struct mt9t11x_i2c_reg_conf const clk_tbl[] = {
    {0x0614, 0x0001, WORD_LEN, 10},	// SECOND_SCL_SDA_PD
    {0x0614, 0x0001, WORD_LEN, 10},	// SECOND_SCL_SDA_PD
    {0x0614, 0x0001, WORD_LEN, 10}, // SECOND_SCL_SDA_PD
    {0x0614, 0x0001, WORD_LEN, 10},	// SECOND_SCL_SDA_PD
    {0x0614, 0x0001, WORD_LEN, 10},	// SECOND_SCL_SDA_PD
    {0x0614, 0x0001, WORD_LEN, 50},	// SECOND_SCL_SDA_PD    
};

/* Preview and Snapshot Setup */
static struct mt9t11x_i2c_reg_conf const prev_snap_tbl[] = {
    // timing setting
    {0x098E, 0x603C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0F00, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6800, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0400, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6802, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0300, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE88E, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x68A0, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x082E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4802, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4804, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4806, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x060D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4808, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x080D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x480A, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0111, WORD_LEN, 0}, 	// MCU_DATA_0
/*
 * ZTE_CAM_LJ_20110317
 * adjust orientation of preview for P725A
 */
#if defined(CONFIG_MACH_TURIES)    
    {0x098E, 0x480C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x046C, WORD_LEN, 0}, 	// MCU_DATA_0
#else
    {0x098E, 0x480C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x046F, WORD_LEN, 0}, 	// MCU_DATA_0
#endif
    
    {0x098E, 0x480F, WORD_LEN, 0}, 	// MCU_ADDRESS   
    {0x0990, 0x00CC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4811, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0381, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4813, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x024F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x481D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x03B9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x481F, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x05D0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4825, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x07F2, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x482B, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0408, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x482D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0308, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C00, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0800, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C02, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0600, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC8E, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6CA0, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x082D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x484A, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x484C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x484E, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x060B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4850, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x080B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4852, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0111, WORD_LEN, 0}, 	// MCU_DATA_0
/*
 * ZTE_CAM_LJ_20110317
 * adjust orientation of snapshot for P725A
 */
#if defined(CONFIG_MACH_TURIES)        
    //{0x098E, 0x4854, WORD_LEN, 0}, 	// MCU_ADDRESS
    //{0x0990, 0x0024, WORD_LEN, 0}, 	// MCU_DATA_0
#else
    {0x098E, 0x4854, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0027, WORD_LEN, 0}, 	// MCU_DATA_0
#endif

    {0x098E, 0x4857, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x008C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4859, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x01F1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x485B, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4865, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x068C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4867, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0378, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x486D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0F69, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4873, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0808, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4875, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0608, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8A5, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0022, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8A6, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0024, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8A7, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0029, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8A8, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x002B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC844, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x00D2, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC92F, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC845, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x00AF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC92D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC88C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x006C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC930, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC88D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x005A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC92E, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xB825, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xA009, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0002, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xA00A, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xA00C, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4846, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x68AA, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0218, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6815, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6817, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x682D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x488E, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6CAA, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x008A, WORD_LEN, 0}, 	// MCU_DATA_0
    
    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 200}, 	// MCU_DATA_0
    
    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 200}, 	// MCU_DATA_0

    // Patch
    {0x0982, 0x0000, WORD_LEN, 0}, 	// ACCESS_CTL_STAT
    {0x098A, 0x0D61, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0FF6, WORD_LEN, 0},
    {0x0992, 0x01C5, WORD_LEN, 0},
    {0x0994, 0xC101, WORD_LEN, 0},
    {0x0996, 0x261F, WORD_LEN, 0},
    {0x0998, 0xF601, WORD_LEN, 0},
    {0x099A, 0xC6C1, WORD_LEN, 0},
    {0x099C, 0x0A26, WORD_LEN, 0},
    {0x099E, 0x18F6, WORD_LEN, 0},
    {0x098A, 0x0D71, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x304D, WORD_LEN, 0},
    {0x0992, 0xC4F0, WORD_LEN, 0},
    {0x0994, 0xCA01, WORD_LEN, 0},
    {0x0996, 0xF730, WORD_LEN, 0},
    {0x0998, 0x4DBD, WORD_LEN, 0},
    {0x099A, 0x0D8A, WORD_LEN, 0},
    {0x099C, 0xCC0D, WORD_LEN, 0},
    {0x099E, 0x8ABD, WORD_LEN, 0},
    {0x098A, 0x0D81, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x4224, WORD_LEN, 0},
    {0x0992, 0xC601, WORD_LEN, 0},
    {0x0994, 0xF713, WORD_LEN, 0},
    {0x0996, 0xC20E, WORD_LEN, 0},
    {0x0998, 0x39F6, WORD_LEN, 0},
    {0x099A, 0x304D, WORD_LEN, 0},
    {0x099C, 0xC4F0, WORD_LEN, 0},
    {0x099E, 0xCA02, WORD_LEN, 0},
    {0x098A, 0x0D91, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xF730, WORD_LEN, 0},
    {0x0992, 0x4DDE, WORD_LEN, 0},
    {0x0994, 0x4718, WORD_LEN, 0},
    {0x0996, 0xCE00, WORD_LEN, 0},
    {0x0998, 0x5FCC, WORD_LEN, 0},
    {0x099A, 0x0015, WORD_LEN, 0},
    {0x099C, 0xBDC8, WORD_LEN, 0},
    {0x099E, 0x37CC, WORD_LEN, 0},
    {0x098A, 0x0DA1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x1032, WORD_LEN, 0},
    {0x0992, 0xDD69, WORD_LEN, 0},
    {0x0994, 0xCC0F, WORD_LEN, 0},
    {0x0996, 0xA4DD, WORD_LEN, 0},
    {0x0998, 0x71CC, WORD_LEN, 0},
    {0x099A, 0x0EF4, WORD_LEN, 0},
    {0x099C, 0xDD6B, WORD_LEN, 0},
    {0x099E, 0xCC0E, WORD_LEN, 0},
    {0x098A, 0x0DB1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xDFDD, WORD_LEN, 0},
    {0x0992, 0x73CC, WORD_LEN, 0},
    {0x0994, 0x0FED, WORD_LEN, 0},
    {0x0996, 0xDD67, WORD_LEN, 0},
    {0x0998, 0xCC00, WORD_LEN, 0},
    {0x099A, 0x5FDD, WORD_LEN, 0},
    {0x099C, 0x47DE, WORD_LEN, 0},
    {0x099E, 0x3D18, WORD_LEN, 0},
    {0x098A, 0x0DC1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCE00, WORD_LEN, 0},
    {0x0992, 0x75CC, WORD_LEN, 0},
    {0x0994, 0x0033, WORD_LEN, 0},
    {0x0996, 0xBDC8, WORD_LEN, 0},
    {0x0998, 0x37CC, WORD_LEN, 0},
    {0x099A, 0x11CA, WORD_LEN, 0},
    {0x099C, 0xDDA7, WORD_LEN, 0},
    {0x099E, 0xCC11, WORD_LEN, 0},
    {0x098A, 0x0DD1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xE9DD, WORD_LEN, 0},
    {0x0992, 0x7FCC, WORD_LEN, 0},
    {0x0994, 0x0075, WORD_LEN, 0},
    {0x0996, 0xDD3D, WORD_LEN, 0},
    {0x0998, 0xDE51, WORD_LEN, 0},
    {0x099A, 0x18CE, WORD_LEN, 0},
    {0x099C, 0x0CE7, WORD_LEN, 0},
    {0x099E, 0xCC00, WORD_LEN, 0},
    {0x098A, 0x0DE1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x13BD, WORD_LEN, 0},
    {0x0992, 0xC837, WORD_LEN, 0},
    {0x0994, 0xCC0E, WORD_LEN, 0},
    {0x0996, 0xD5FD, WORD_LEN, 0},
    {0x0998, 0x0CED, WORD_LEN, 0},
    {0x099A, 0xCC0C, WORD_LEN, 0},
    {0x099C, 0xE7DD, WORD_LEN, 0},
    {0x099E, 0x51DE, WORD_LEN, 0},
    {0x098A, 0x0DF1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x5718, WORD_LEN, 0},
    {0x0992, 0xCE00, WORD_LEN, 0},
    {0x0994, 0xA9CC, WORD_LEN, 0},
    {0x0996, 0x0015, WORD_LEN, 0},
    {0x0998, 0xBDC8, WORD_LEN, 0},
    {0x099A, 0x37CC, WORD_LEN, 0},
    {0x099C, 0x133D, WORD_LEN, 0},
    {0x099E, 0xDDAB, WORD_LEN, 0},
    {0x098A, 0x0E01, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC0E, WORD_LEN, 0},
    {0x0992, 0x8ADD, WORD_LEN, 0},
    {0x0994, 0xB7CC, WORD_LEN, 0},
    {0x0996, 0x00A9, WORD_LEN, 0},
    {0x0998, 0xDD57, WORD_LEN, 0},
    {0x099A, 0xDE59, WORD_LEN, 0},
    {0x099C, 0x18CE, WORD_LEN, 0},
    {0x099E, 0x0CFB, WORD_LEN, 0},
    {0x098A, 0x0E11, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC00, WORD_LEN, 0},
    {0x0992, 0x19BD, WORD_LEN, 0},
    {0x0994, 0xC837, WORD_LEN, 0},
    {0x0996, 0xCC10, WORD_LEN, 0},
    {0x0998, 0xDCFD, WORD_LEN, 0},
    {0x099A, 0x0D0F, WORD_LEN, 0},
    {0x099C, 0xCC11, WORD_LEN, 0},
    {0x099E, 0x04FD, WORD_LEN, 0},
    {0x098A, 0x0E21, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0D11, WORD_LEN, 0},
    {0x0992, 0xCC0C, WORD_LEN, 0},
    {0x0994, 0xFBDD, WORD_LEN, 0},
    {0x0996, 0x59DE, WORD_LEN, 0},
    {0x0998, 0x4F18, WORD_LEN, 0},
    {0x099A, 0xCE00, WORD_LEN, 0},
    {0x099C, 0xBFCC, WORD_LEN, 0},
    {0x099E, 0x000D, WORD_LEN, 0},
    {0x098A, 0x0E31, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xBDC8, WORD_LEN, 0},
    {0x0992, 0x37CC, WORD_LEN, 0},
    {0x0994, 0x10F0, WORD_LEN, 0},
    {0x0996, 0xDDC9, WORD_LEN, 0},
    {0x0998, 0xCC00, WORD_LEN, 0},
    {0x099A, 0xBFDD, WORD_LEN, 0},
    {0x099C, 0x4FDE, WORD_LEN, 0},
    {0x099E, 0x4118, WORD_LEN, 0},
    {0x098A, 0x0E41, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCE0D, WORD_LEN, 0},
    {0x0992, 0x15CC, WORD_LEN, 0},
    {0x0994, 0x0025, WORD_LEN, 0},
    {0x0996, 0xBDC8, WORD_LEN, 0},
    {0x0998, 0x37CC, WORD_LEN, 0},
    {0x099A, 0x12DE, WORD_LEN, 0},
    {0x099C, 0xFD0D, WORD_LEN, 0},
    {0x099E, 0x37CC, WORD_LEN, 0},
    {0x098A, 0x0E51, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0D15, WORD_LEN, 0},
    {0x0992, 0xDD41, WORD_LEN, 0},
    {0x0994, 0xDE45, WORD_LEN, 0},
    {0x0996, 0x18CE, WORD_LEN, 0},
    {0x0998, 0x0D3B, WORD_LEN, 0},
    {0x099A, 0xCC00, WORD_LEN, 0},
    {0x099C, 0x25BD, WORD_LEN, 0},
    {0x099E, 0xC837, WORD_LEN, 0},
    {0x098A, 0x0E61, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC12, WORD_LEN, 0},
    {0x0992, 0x74FD, WORD_LEN, 0},
    {0x0994, 0x0D49, WORD_LEN, 0},
    {0x0996, 0xCC0D, WORD_LEN, 0},
    {0x0998, 0x3BDD, WORD_LEN, 0},
    {0x099A, 0x45FC, WORD_LEN, 0},
    {0x099C, 0x135C, WORD_LEN, 0},
    {0x099E, 0xDD5B, WORD_LEN, 0},
    {0x098A, 0x0E71, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFC13, WORD_LEN, 0},
    {0x0992, 0x5EDD, WORD_LEN, 0},
    {0x0994, 0x5DCC, WORD_LEN, 0},
    {0x0996, 0x005B, WORD_LEN, 0},
    {0x0998, 0xFD01, WORD_LEN, 0},
    {0x099A, 0xBEC6, WORD_LEN, 0},
    {0x099C, 0x01F7, WORD_LEN, 0},
    {0x099E, 0x01CA, WORD_LEN, 0},
    {0x098A, 0x0E81, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xC60C, WORD_LEN, 0},
    {0x0992, 0xF701, WORD_LEN, 0},
    {0x0994, 0xCB7F, WORD_LEN, 0},
    {0x0996, 0x01CC, WORD_LEN, 0},
    {0x0998, 0x393C, WORD_LEN, 0},
    {0x099A, 0x34BD, WORD_LEN, 0},
    {0x099C, 0xCF2B, WORD_LEN, 0},
    {0x099E, 0x30E7, WORD_LEN, 0},
    {0x098A, 0x0E91, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x02F6, WORD_LEN, 0},
    {0x0992, 0x08DF, WORD_LEN, 0},
    {0x0994, 0xC104, WORD_LEN, 0},
    {0x0996, 0x2636, WORD_LEN, 0},
    {0x0998, 0xDE23, WORD_LEN, 0},
    {0x099A, 0x1E91, WORD_LEN, 0},
    {0x099C, 0x2402, WORD_LEN, 0},
    {0x099E, 0x202E, WORD_LEN, 0},
    {0x098A, 0x0EA1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x18CE, WORD_LEN, 0},
    {0x0992, 0x08D8, WORD_LEN, 0},
    {0x0994, 0x181F, WORD_LEN, 0},
    {0x0996, 0x0608, WORD_LEN, 0},
    {0x0998, 0x251E, WORD_LEN, 0},
    {0x099A, 0x9140, WORD_LEN, 0},
    {0x099C, 0x111C, WORD_LEN, 0},
    {0x099E, 0x9140, WORD_LEN, 0},
    {0x098A, 0x0EB1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC3C, WORD_LEN, 0},
    {0x0992, 0x0430, WORD_LEN, 0},
    {0x0994, 0xED00, WORD_LEN, 0},
    {0x0996, 0xCC00, WORD_LEN, 0},
    {0x0998, 0x01BD, WORD_LEN, 0},
    {0x099A, 0x70A8, WORD_LEN, 0},
    {0x099C, 0x2010, WORD_LEN, 0},
    {0x099E, 0xDE23, WORD_LEN, 0},
    {0x098A, 0x0EC1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x1D91, WORD_LEN, 0},
    {0x0992, 0x40CC, WORD_LEN, 0},
    {0x0994, 0x3C04, WORD_LEN, 0},
    {0x0996, 0x30ED, WORD_LEN, 0},
    {0x0998, 0x005F, WORD_LEN, 0},
    {0x099A, 0x4FBD, WORD_LEN, 0},
    {0x099C, 0x70A8, WORD_LEN, 0},
    {0x099E, 0x30E6, WORD_LEN, 0},
    {0x098A, 0x0ED1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0238, WORD_LEN, 0},
    {0x0992, 0x3139, WORD_LEN, 0},
    {0x0994, 0xDE21, WORD_LEN, 0},
    {0x0996, 0xEC15, WORD_LEN, 0},
    {0x0998, 0xFD08, WORD_LEN, 0},
    {0x099A, 0x317E, WORD_LEN, 0},
    {0x099C, 0xA620, WORD_LEN, 0},
    {0x099E, 0xBD97, WORD_LEN, 0},
    {0x098A, 0x0EE1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xB07D, WORD_LEN, 0},
    {0x0992, 0x06DB, WORD_LEN, 0},
    {0x0994, 0x270C, WORD_LEN, 0},
    {0x0996, 0xDE21, WORD_LEN, 0},
    {0x0998, 0x3C18, WORD_LEN, 0},
    {0x099A, 0x38EC, WORD_LEN, 0},
    {0x099C, 0x25A3, WORD_LEN, 0},
    {0x099E, 0x13FD, WORD_LEN, 0},
    {0x098A, 0x0EF1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06E4, WORD_LEN, 0},
    {0x0992, 0x393C, WORD_LEN, 0},
    {0x0994, 0x3C3C, WORD_LEN, 0},
    {0x0996, 0x306F, WORD_LEN, 0},
    {0x0998, 0x057D, WORD_LEN, 0},
    {0x099A, 0x06DB, WORD_LEN, 0},
    {0x099C, 0x2659, WORD_LEN, 0},
    {0x099E, 0xDE21, WORD_LEN, 0},
    {0x098A, 0x0F01, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x1F0C, WORD_LEN, 0},
    {0x0992, 0x0203, WORD_LEN, 0},
    {0x0994, 0x306A, WORD_LEN, 0},
    {0x0996, 0x057D, WORD_LEN, 0},
    {0x0998, 0x06EC, WORD_LEN, 0},
    {0x099A, 0x2703, WORD_LEN, 0},
    {0x099C, 0x306C, WORD_LEN, 0},
    {0x099E, 0x05DE, WORD_LEN, 0},
    {0x098A, 0x0F11, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x21EC, WORD_LEN, 0},
    {0x0992, 0x0AC4, WORD_LEN, 0},
    {0x0994, 0x074F, WORD_LEN, 0},
    {0x0996, 0x0430, WORD_LEN, 0},
    {0x0998, 0xE704, WORD_LEN, 0},
    {0x099A, 0xA605, WORD_LEN, 0},
    {0x099C, 0xA004, WORD_LEN, 0},
    {0x099E, 0xA705, WORD_LEN, 0},
    {0x098A, 0x0F21, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xF606, WORD_LEN, 0},
    {0x0992, 0xED4F, WORD_LEN, 0},
    {0x0994, 0x04EB, WORD_LEN, 0},
    {0x0996, 0x05E7, WORD_LEN, 0},
    {0x0998, 0x052C, WORD_LEN, 0},
    {0x099A, 0x12FC, WORD_LEN, 0},
    {0x099C, 0x06E2, WORD_LEN, 0},
    {0x099E, 0xED02, WORD_LEN, 0},
    {0x098A, 0x0F31, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFC06, WORD_LEN, 0},
    {0x0992, 0xE0ED, WORD_LEN, 0},
    {0x0994, 0x00E6, WORD_LEN, 0},
    {0x0996, 0x0550, WORD_LEN, 0},
    {0x0998, 0xBDC8, WORD_LEN, 0},
    {0x099A, 0x1720, WORD_LEN, 0},
    {0x099C, 0x0FFC, WORD_LEN, 0},
    {0x099E, 0x06E2, WORD_LEN, 0},
    {0x098A, 0x0F41, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED02, WORD_LEN, 0},
    {0x0992, 0xFC06, WORD_LEN, 0},
    {0x0994, 0xE0ED, WORD_LEN, 0},
    {0x0996, 0x00E6, WORD_LEN, 0},
    {0x0998, 0x05BD, WORD_LEN, 0},
    {0x099A, 0xC7F7, WORD_LEN, 0},
    {0x099C, 0x30EC, WORD_LEN, 0},
    {0x099E, 0x00FD, WORD_LEN, 0},
    {0x098A, 0x0F51, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06E0, WORD_LEN, 0},
    {0x0992, 0xEC02, WORD_LEN, 0},
    {0x0994, 0xFD06, WORD_LEN, 0},
    {0x0996, 0xE2DE, WORD_LEN, 0},
    {0x0998, 0x21EC, WORD_LEN, 0},
    {0x099A, 0x25FD, WORD_LEN, 0},
    {0x099C, 0x06D3, WORD_LEN, 0},
    {0x099E, 0xDE47, WORD_LEN, 0},
    {0x098A, 0x0F61, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xEE14, WORD_LEN, 0},
    {0x0992, 0xAD00, WORD_LEN, 0},
    {0x0994, 0xF606, WORD_LEN, 0},
    {0x0996, 0xDBC1, WORD_LEN, 0},
    {0x0998, 0x0227, WORD_LEN, 0},
    {0x099A, 0x09DE, WORD_LEN, 0},
    {0x099C, 0x21EC, WORD_LEN, 0},
    {0x099E, 0x34FD, WORD_LEN, 0},
    {0x098A, 0x0F71, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06DE, WORD_LEN, 0},
    {0x0992, 0x2007, WORD_LEN, 0},
    {0x0994, 0xDE21, WORD_LEN, 0},
    {0x0996, 0xFC06, WORD_LEN, 0},
    {0x0998, 0xDEED, WORD_LEN, 0},
    {0x099A, 0x34DE, WORD_LEN, 0},
    {0x099C, 0x21FC, WORD_LEN, 0},
    {0x099E, 0x06E4, WORD_LEN, 0},
    {0x098A, 0x0F81, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED29, WORD_LEN, 0},
    {0x0992, 0xDE21, WORD_LEN, 0},
    {0x0994, 0xFC06, WORD_LEN, 0},
    {0x0996, 0xE6ED, WORD_LEN, 0},
    {0x0998, 0x27DE, WORD_LEN, 0},
    {0x099A, 0x21FC, WORD_LEN, 0},
    {0x099C, 0x06DC, WORD_LEN, 0},
    {0x099E, 0xED3E, WORD_LEN, 0},
    {0x098A, 0x0F91, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFD13, WORD_LEN, 0},
    {0x0992, 0x98CC, WORD_LEN, 0},
    {0x0994, 0x32DC, WORD_LEN, 0},
    {0x0996, 0x30ED, WORD_LEN, 0},
    {0x0998, 0x00FC, WORD_LEN, 0},
    {0x099A, 0x1398, WORD_LEN, 0},
    {0x099C, 0xBD70, WORD_LEN, 0},
    {0x099E, 0xA838, WORD_LEN, 0},
    {0x098A, 0x0FA1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3838, WORD_LEN, 0},
    {0x0992, 0x393C, WORD_LEN, 0},
    {0x0994, 0x3C7D, WORD_LEN, 0},
    {0x0996, 0x06DB, WORD_LEN, 0},
    {0x0998, 0x260C, WORD_LEN, 0},
    {0x099A, 0xFC08, WORD_LEN, 0},
    {0x099C, 0x1F1A, WORD_LEN, 0},
    {0x099E, 0xB306, WORD_LEN, 0},
    {0x098A, 0x0FB1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xD124, WORD_LEN, 0},
    {0x0992, 0x03FD, WORD_LEN, 0},
    {0x0994, 0x06E6, WORD_LEN, 0},
    {0x0996, 0xFC06, WORD_LEN, 0},
    {0x0998, 0xE61A, WORD_LEN, 0},
    {0x099A, 0xB306, WORD_LEN, 0},
    {0x099C, 0xD125, WORD_LEN, 0},
    {0x099E, 0x1B30, WORD_LEN, 0},
    {0x098A, 0x0FC1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED02, WORD_LEN, 0},
    {0x0992, 0x5F4F, WORD_LEN, 0},
    {0x0994, 0xED00, WORD_LEN, 0},
    {0x0996, 0xFC06, WORD_LEN, 0},
    {0x0998, 0xD1BD, WORD_LEN, 0},
    {0x099A, 0xE3AA, WORD_LEN, 0},
    {0x099C, 0xFD06, WORD_LEN, 0},
    {0x099E, 0xD7FE, WORD_LEN, 0},
    {0x098A, 0x0FD1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06D1, WORD_LEN, 0},
    {0x0992, 0xBDC5, WORD_LEN, 0},
    {0x0994, 0xA6FD, WORD_LEN, 0},
    {0x0996, 0x06E6, WORD_LEN, 0},
    {0x0998, 0x2008, WORD_LEN, 0},
    {0x099A, 0x5F4F, WORD_LEN, 0},
    {0x099C, 0xFD06, WORD_LEN, 0},
    {0x099E, 0xD7B7, WORD_LEN, 0},
    {0x098A, 0x0FE1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06DB, WORD_LEN, 0},
    {0x0992, 0xDE21, WORD_LEN, 0},
    {0x0994, 0xFC06, WORD_LEN, 0},
    {0x0996, 0xE6ED, WORD_LEN, 0},
    {0x0998, 0x2738, WORD_LEN, 0},
    {0x099A, 0x3839, WORD_LEN, 0},
    {0x099C, 0x3C3C, WORD_LEN, 0},
    {0x099E, 0x3C3C, WORD_LEN, 0},
    {0x098A, 0x0FF1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3C18, WORD_LEN, 0},
    {0x0992, 0xDE21, WORD_LEN, 0},
    {0x0994, 0x18EC, WORD_LEN, 0},
    {0x0996, 0x2730, WORD_LEN, 0},
    {0x0998, 0xED00, WORD_LEN, 0},
    {0x099A, 0x8F30, WORD_LEN, 0},
    {0x099C, 0xED08, WORD_LEN, 0},
    {0x099E, 0x18EC, WORD_LEN, 0},
    {0x098A, 0x1001, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x25EE, WORD_LEN, 0},
    {0x0992, 0x08BD, WORD_LEN, 0},
    {0x0994, 0xE326, WORD_LEN, 0},
    {0x0996, 0x30EC, WORD_LEN, 0},
    {0x0998, 0x02ED, WORD_LEN, 0},
    {0x099A, 0x06EC, WORD_LEN, 0},
    {0x099C, 0x00ED, WORD_LEN, 0},
    {0x099E, 0x04DE, WORD_LEN, 0},
    {0x098A, 0x1011, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x21EC, WORD_LEN, 0},
    {0x0992, 0x2930, WORD_LEN, 0},
    {0x0994, 0xED02, WORD_LEN, 0},
    {0x0996, 0x5F4F, WORD_LEN, 0},
    {0x0998, 0xED00, WORD_LEN, 0},
    {0x099A, 0xBDC5, WORD_LEN, 0},
    {0x099C, 0xCB30, WORD_LEN, 0},
    {0x099E, 0xEC04, WORD_LEN, 0},
    {0x098A, 0x1021, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFD06, WORD_LEN, 0},
    {0x0992, 0xE0EC, WORD_LEN, 0},
    {0x0994, 0x06FD, WORD_LEN, 0},
    {0x0996, 0x06E2, WORD_LEN, 0},
    {0x0998, 0xBD91, WORD_LEN, 0},
    {0x099A, 0x2630, WORD_LEN, 0},
    {0x099C, 0xC60A, WORD_LEN, 0},
    {0x099E, 0x3A35, WORD_LEN, 0},
    {0x098A, 0x1031, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x393C, WORD_LEN, 0},
    {0x0992, 0x3CF6, WORD_LEN, 0},
    {0x0994, 0x06D9, WORD_LEN, 0},
    {0x0996, 0xC103, WORD_LEN, 0},
    {0x0998, 0x2611, WORD_LEN, 0},
    {0x099A, 0xF606, WORD_LEN, 0},
    {0x099C, 0xCCF1, WORD_LEN, 0},
    {0x099E, 0x06CA, WORD_LEN, 0},
    {0x098A, 0x1041, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x2204, WORD_LEN, 0},
    {0x0992, 0xC601, WORD_LEN, 0},
    {0x0994, 0x2002, WORD_LEN, 0},
    {0x0996, 0xC602, WORD_LEN, 0},
    {0x0998, 0xF706, WORD_LEN, 0},
    {0x099A, 0xD9DE, WORD_LEN, 0},
    {0x099C, 0x21EC, WORD_LEN, 0},
    {0x099E, 0x34FD, WORD_LEN, 0},
    {0x098A, 0x1051, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x06DE, WORD_LEN, 0},
    {0x0992, 0xBD93, WORD_LEN, 0},
    {0x0994, 0xCBF6, WORD_LEN, 0},
    {0x0996, 0x06DB, WORD_LEN, 0},
    {0x0998, 0xC101, WORD_LEN, 0},
    {0x099A, 0x262E, WORD_LEN, 0},
    {0x099C, 0xDE23, WORD_LEN, 0},
    {0x099E, 0xEC37, WORD_LEN, 0},
    {0x098A, 0x1061, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xB306, WORD_LEN, 0},
    {0x0992, 0xDE23, WORD_LEN, 0},
    {0x0994, 0x25EC, WORD_LEN, 0},
    {0x0996, 0x3BB3, WORD_LEN, 0},
    {0x0998, 0x06D7, WORD_LEN, 0},
    {0x099A, 0x241E, WORD_LEN, 0},
    {0x099C, 0xFE06, WORD_LEN, 0},
    {0x099E, 0xD709, WORD_LEN, 0},
    {0x098A, 0x1071, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFF06, WORD_LEN, 0},
    {0x0992, 0xD7FC, WORD_LEN, 0},
    {0x0994, 0x06DE, WORD_LEN, 0},
    {0x0996, 0x30ED, WORD_LEN, 0},
    {0x0998, 0x02FC, WORD_LEN, 0},
    {0x099A, 0x06D7, WORD_LEN, 0},
    {0x099C, 0xC300, WORD_LEN, 0},
    {0x099E, 0x01ED, WORD_LEN, 0},
    {0x098A, 0x1081, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x00FC, WORD_LEN, 0},
    {0x0992, 0x06D7, WORD_LEN, 0},
    {0x0994, 0xBDE2, WORD_LEN, 0},
    {0x0996, 0x84FD, WORD_LEN, 0},
    {0x0998, 0x06DE, WORD_LEN, 0},
    {0x099A, 0x3838, WORD_LEN, 0},
    {0x099C, 0x393C, WORD_LEN, 0},
    {0x099E, 0x3C3C, WORD_LEN, 0},
    {0x098A, 0x1091, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3418, WORD_LEN, 0},
    {0x0992, 0xDE21, WORD_LEN, 0},
    {0x0994, 0x18E6, WORD_LEN, 0},
    {0x0996, 0x0DC4, WORD_LEN, 0},
    {0x0998, 0x0330, WORD_LEN, 0},
    {0x099A, 0xE706, WORD_LEN, 0},
    {0x099C, 0x18EC, WORD_LEN, 0},
    {0x099E, 0x0CC4, WORD_LEN, 0},
    {0x098A, 0x10A1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xE04F, WORD_LEN, 0},
    {0x0992, 0x0404, WORD_LEN, 0},
    {0x0994, 0x0404, WORD_LEN, 0},
    {0x0996, 0x04E7, WORD_LEN, 0},
    {0x0998, 0x0518, WORD_LEN, 0},
    {0x099A, 0xEC0C, WORD_LEN, 0},
    {0x099C, 0xC41C, WORD_LEN, 0},
    {0x099E, 0x4F04, WORD_LEN, 0},
    {0x098A, 0x10B1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x04E7, WORD_LEN, 0},
    {0x0992, 0x04E6, WORD_LEN, 0},
    {0x0994, 0x064F, WORD_LEN, 0},
    {0x0996, 0x05ED, WORD_LEN, 0},
    {0x0998, 0x02E6, WORD_LEN, 0},
    {0x099A, 0x054F, WORD_LEN, 0},
    {0x099C, 0x0505, WORD_LEN, 0},
    {0x099E, 0x05ED, WORD_LEN, 0},
    {0x098A, 0x10C1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x00EA, WORD_LEN, 0},
    {0x0992, 0x03AA, WORD_LEN, 0},
    {0x0994, 0x02ED, WORD_LEN, 0},
    {0x0996, 0x02E6, WORD_LEN, 0},
    {0x0998, 0x044F, WORD_LEN, 0},
    {0x099A, 0x0505, WORD_LEN, 0},
    {0x099C, 0x0505, WORD_LEN, 0},
    {0x099E, 0x0505, WORD_LEN, 0},
    {0x098A, 0x10D1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED00, WORD_LEN, 0},
    {0x0992, 0xEA03, WORD_LEN, 0},
    {0x0994, 0xAA02, WORD_LEN, 0},
    {0x0996, 0x3838, WORD_LEN, 0},
    {0x0998, 0x3831, WORD_LEN, 0},
    {0x099A, 0x393C, WORD_LEN, 0},
    {0x099C, 0xBDEB, WORD_LEN, 0},
    {0x099E, 0xB4CC, WORD_LEN, 0},
    {0x098A, 0x10E1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x33C4, WORD_LEN, 0},
    {0x0992, 0x30ED, WORD_LEN, 0},
    {0x0994, 0x0034, WORD_LEN, 0},
    {0x0996, 0xBD10, WORD_LEN, 0},
    {0x0998, 0x8E31, WORD_LEN, 0},
    {0x099A, 0xBD70, WORD_LEN, 0},
    {0x099C, 0xA838, WORD_LEN, 0},
    {0x099E, 0x393C, WORD_LEN, 0},
    {0x098A, 0x10F1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xBDA5, WORD_LEN, 0},
    {0x0992, 0x05CC, WORD_LEN, 0},
    {0x0994, 0x3780, WORD_LEN, 0},
    {0x0996, 0x30ED, WORD_LEN, 0},
    {0x0998, 0x0034, WORD_LEN, 0},
    {0x099A, 0xBD10, WORD_LEN, 0},
    {0x099C, 0x8E31, WORD_LEN, 0},
    {0x099E, 0xBD70, WORD_LEN, 0},
    {0x098A, 0x1101, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xA838, WORD_LEN, 0},
    {0x0992, 0x393C, WORD_LEN, 0},
    {0x0994, 0x3C3C, WORD_LEN, 0},
    {0x0996, 0xBDEC, WORD_LEN, 0},
    {0x0998, 0xC3DE, WORD_LEN, 0},
    {0x099A, 0x1D8F, WORD_LEN, 0},
    {0x099C, 0xC301, WORD_LEN, 0},
    {0x099E, 0x4B8F, WORD_LEN, 0},
    {0x098A, 0x1111, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xEC00, WORD_LEN, 0},
    {0x0992, 0xFD05, WORD_LEN, 0},
    {0x0994, 0x1BEC, WORD_LEN, 0},
    {0x0996, 0x02FD, WORD_LEN, 0},
    {0x0998, 0x051D, WORD_LEN, 0},
    {0x099A, 0xFC05, WORD_LEN, 0},
    {0x099C, 0x68B3, WORD_LEN, 0},
    {0x099E, 0x0565, WORD_LEN, 0},
    {0x098A, 0x1121, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x2205, WORD_LEN, 0},
    {0x0992, 0xFC05, WORD_LEN, 0},
    {0x0994, 0x6520, WORD_LEN, 0},
    {0x0996, 0x03FC, WORD_LEN, 0},
    {0x0998, 0x0568, WORD_LEN, 0},
    {0x099A, 0xB305, WORD_LEN, 0},
    {0x099C, 0x6A22, WORD_LEN, 0},
    {0x099E, 0x05FC, WORD_LEN, 0},
    {0x098A, 0x1131, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x056A, WORD_LEN, 0},
    {0x0992, 0x2010, WORD_LEN, 0},
    {0x0994, 0xFC05, WORD_LEN, 0},
    {0x0996, 0x68B3, WORD_LEN, 0},
    {0x0998, 0x0565, WORD_LEN, 0},
    {0x099A, 0x2205, WORD_LEN, 0},
    {0x099C, 0xFC05, WORD_LEN, 0},
    {0x099E, 0x6520, WORD_LEN, 0},
    {0x098A, 0x1141, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x03FC, WORD_LEN, 0},
    {0x0992, 0x0568, WORD_LEN, 0},
    {0x0994, 0xFD05, WORD_LEN, 0},
    {0x0996, 0x1FDE, WORD_LEN, 0},
    {0x0998, 0x1D8F, WORD_LEN, 0},
    {0x099A, 0xC301, WORD_LEN, 0},
    {0x099C, 0x728F, WORD_LEN, 0},
    {0x099E, 0xEC00, WORD_LEN, 0},
    {0x098A, 0x1151, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xFD05, WORD_LEN, 0},
    {0x0992, 0x21EC, WORD_LEN, 0},
    {0x0994, 0x02FD, WORD_LEN, 0},
    {0x0996, 0x0523, WORD_LEN, 0},
    {0x0998, 0xDE3F, WORD_LEN, 0},
    {0x099A, 0xEE00, WORD_LEN, 0},
    {0x099C, 0xAD00, WORD_LEN, 0},
    {0x099E, 0x30ED, WORD_LEN, 0},
    {0x098A, 0x1161, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x04CC, WORD_LEN, 0},
    {0x0992, 0x33D2, WORD_LEN, 0},
    {0x0994, 0xED00, WORD_LEN, 0},
    {0x0996, 0xEC04, WORD_LEN, 0},
    {0x0998, 0xBD70, WORD_LEN, 0},
    {0x099A, 0xA8DE, WORD_LEN, 0},
    {0x099C, 0x1D8F, WORD_LEN, 0},
    {0x099E, 0xC301, WORD_LEN, 0},
    {0x098A, 0x1171, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x768F, WORD_LEN, 0},
    {0x0992, 0xEC00, WORD_LEN, 0},
    {0x0994, 0xFD05, WORD_LEN, 0},
    {0x0996, 0x21EC, WORD_LEN, 0},
    {0x0998, 0x02FD, WORD_LEN, 0},
    {0x099A, 0x0523, WORD_LEN, 0},
    {0x099C, 0xDE3F, WORD_LEN, 0},
    {0x099E, 0xEE00, WORD_LEN, 0},
    {0x098A, 0x1181, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xAD00, WORD_LEN, 0},
    {0x0992, 0x30ED, WORD_LEN, 0},
    {0x0994, 0x04CC, WORD_LEN, 0},
    {0x0996, 0x33D4, WORD_LEN, 0},
    {0x0998, 0xED00, WORD_LEN, 0},
    {0x099A, 0xEC04, WORD_LEN, 0},
    {0x099C, 0xBD70, WORD_LEN, 0},
    {0x099E, 0xA85F, WORD_LEN, 0},
    {0x098A, 0x1191, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x4F30, WORD_LEN, 0},
    {0x0992, 0xED04, WORD_LEN, 0},
    {0x0994, 0xDE1D, WORD_LEN, 0},
    {0x0996, 0x8FC3, WORD_LEN, 0},
    {0x0998, 0x0160, WORD_LEN, 0},
    {0x099A, 0x8F1F, WORD_LEN, 0},
    {0x099C, 0x0008, WORD_LEN, 0},
    {0x099E, 0x0430, WORD_LEN, 0},
    {0x098A, 0x11A1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x1C04, WORD_LEN, 0},
    {0x0992, 0x40DE, WORD_LEN, 0},
    {0x0994, 0x1D8F, WORD_LEN, 0},
    {0x0996, 0xC301, WORD_LEN, 0},
    {0x0998, 0x608F, WORD_LEN, 0},
    {0x099A, 0x1F00, WORD_LEN, 0},
    {0x099C, 0x1004, WORD_LEN, 0},
    {0x099E, 0x301C, WORD_LEN, 0},
    {0x098A, 0x11B1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0420, WORD_LEN, 0},
    {0x0992, 0xCC33, WORD_LEN, 0},
    {0x0994, 0xF430, WORD_LEN, 0},
    {0x0996, 0xED02, WORD_LEN, 0},
    {0x0998, 0xCC9F, WORD_LEN, 0},
    {0x099A, 0xFFED, WORD_LEN, 0},
    {0x099C, 0x00EC, WORD_LEN, 0},
    {0x099E, 0x045F, WORD_LEN, 0},
    {0x098A, 0x11C1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x8460, WORD_LEN, 0},
    {0x0992, 0xBD70, WORD_LEN, 0},
    {0x0994, 0xF938, WORD_LEN, 0},
    {0x0996, 0x3838, WORD_LEN, 0},
    {0x0998, 0x3937, WORD_LEN, 0},
    {0x099A, 0x3C3C, WORD_LEN, 0},
    {0x099C, 0xCE04, WORD_LEN, 0},
    {0x099E, 0xDD1F, WORD_LEN, 0},
    {0x098A, 0x11D1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3720, WORD_LEN, 0},
    {0x0992, 0x1130, WORD_LEN, 0},
    {0x0994, 0xEC09, WORD_LEN, 0},
    {0x0996, 0xED02, WORD_LEN, 0},
    {0x0998, 0xEC07, WORD_LEN, 0},
    {0x099A, 0xED00, WORD_LEN, 0},
    {0x099C, 0xE604, WORD_LEN, 0},
    {0x099E, 0x54CB, WORD_LEN, 0},
    {0x098A, 0x11E1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x01BD, WORD_LEN, 0},
    {0x0992, 0x6F6C, WORD_LEN, 0},
    {0x0994, 0x3838, WORD_LEN, 0},
    {0x0996, 0x3139, WORD_LEN, 0},
    {0x0998, 0x373C, WORD_LEN, 0},
    {0x099A, 0x3C3C, WORD_LEN, 0},
    {0x099C, 0x7F04, WORD_LEN, 0},
    {0x099E, 0xEECC, WORD_LEN, 0},
    {0x098A, 0x11F1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x0016, WORD_LEN, 0},
    {0x0992, 0x30ED, WORD_LEN, 0},
    {0x0994, 0x02CC, WORD_LEN, 0},
    {0x0996, 0xFFEF, WORD_LEN, 0},
    {0x0998, 0xED00, WORD_LEN, 0},
    {0x099A, 0xCC00, WORD_LEN, 0},
    {0x099C, 0x10BD, WORD_LEN, 0},
    {0x099E, 0x70F9, WORD_LEN, 0},
    {0x098A, 0x1201, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC06, WORD_LEN, 0},
    {0x0992, 0x1430, WORD_LEN, 0},
    {0x0994, 0xED00, WORD_LEN, 0},
    {0x0996, 0x5F4F, WORD_LEN, 0},
    {0x0998, 0xBD70, WORD_LEN, 0},
    {0x099A, 0xA8CC, WORD_LEN, 0},
    {0x099C, 0x3B82, WORD_LEN, 0},
    {0x099E, 0x30ED, WORD_LEN, 0},
    {0x098A, 0x1211, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x02CC, WORD_LEN, 0},
    {0x0992, 0xFFFC, WORD_LEN, 0},
    {0x0994, 0xED00, WORD_LEN, 0},
    {0x0996, 0x6D06, WORD_LEN, 0},
    {0x0998, 0x2604, WORD_LEN, 0},
    {0x099A, 0xC601, WORD_LEN, 0},
    {0x099C, 0x2002, WORD_LEN, 0},
    {0x099E, 0xC603, WORD_LEN, 0},
    {0x098A, 0x1221, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x4FBD, WORD_LEN, 0},
    {0x0992, 0x70F9, WORD_LEN, 0},
    {0x0994, 0xCC3B, WORD_LEN, 0},
    {0x0996, 0x80BD, WORD_LEN, 0},
    {0x0998, 0x7052, WORD_LEN, 0},
    {0x099A, 0x30ED, WORD_LEN, 0},
    {0x099C, 0x041F, WORD_LEN, 0},
    {0x099E, 0x0501, WORD_LEN, 0},
    {0x098A, 0x1231, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xF31E, WORD_LEN, 0},
    {0x0992, 0x0510, WORD_LEN, 0},
    {0x0994, 0xD6CC, WORD_LEN, 0},
    {0x0996, 0x0614, WORD_LEN, 0},
    {0x0998, 0xED00, WORD_LEN, 0},
    {0x099A, 0xCC00, WORD_LEN, 0},
    {0x099C, 0x01BD, WORD_LEN, 0},
    {0x099E, 0x70A8, WORD_LEN, 0},
    {0x098A, 0x1241, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xCC00, WORD_LEN, 0},
    {0x0992, 0x1630, WORD_LEN, 0},
    {0x0994, 0xED02, WORD_LEN, 0},
    {0x0996, 0xCCFF, WORD_LEN, 0},
    {0x0998, 0xEFED, WORD_LEN, 0},
    {0x099A, 0x005F, WORD_LEN, 0},
    {0x099C, 0x4FBD, WORD_LEN, 0},
    {0x099E, 0x70F9, WORD_LEN, 0},
    {0x098A, 0x1251, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x306D, WORD_LEN, 0},
    {0x0992, 0x0626, WORD_LEN, 0},
    {0x0994, 0x0A1F, WORD_LEN, 0},
    {0x0996, 0x0504, WORD_LEN, 0},
    {0x0998, 0x06CE, WORD_LEN, 0},
    {0x099A, 0x04DD, WORD_LEN, 0},
    {0x099C, 0x1C11, WORD_LEN, 0},
    {0x099E, 0x0430, WORD_LEN, 0},
    {0x098A, 0x1261, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x6D09, WORD_LEN, 0},
    {0x0992, 0x270A, WORD_LEN, 0},
    {0x0994, 0x1F05, WORD_LEN, 0},
    {0x0996, 0x0806, WORD_LEN, 0},
    {0x0998, 0xCE04, WORD_LEN, 0},
    {0x099A, 0xDD1C, WORD_LEN, 0},
    {0x099C, 0x1108, WORD_LEN, 0},
    {0x099E, 0x3838, WORD_LEN, 0},
    {0x098A, 0x1271, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3831, WORD_LEN, 0},
    {0x0992, 0x393C, WORD_LEN, 0},
    {0x0994, 0x3C3C, WORD_LEN, 0},
    {0x0996, 0xBD87, WORD_LEN, 0},
    {0x0998, 0xEADE, WORD_LEN, 0},
    {0x099A, 0x1D8F, WORD_LEN, 0},
    {0x099C, 0xC301, WORD_LEN, 0},
    {0x099E, 0x4B8F, WORD_LEN, 0},
    {0x098A, 0x1281, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xEC00, WORD_LEN, 0},
    {0x0992, 0xFD05, WORD_LEN, 0},
    {0x0994, 0x1BEC, WORD_LEN, 0},
    {0x0996, 0x02FD, WORD_LEN, 0},
    {0x0998, 0x051D, WORD_LEN, 0},
    {0x099A, 0xFC05, WORD_LEN, 0},
    {0x099C, 0x65FD, WORD_LEN, 0},
    {0x099E, 0x051F, WORD_LEN, 0},
    {0x098A, 0x1291, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xD65B, WORD_LEN, 0},
    {0x0992, 0x4FFD, WORD_LEN, 0},
    {0x0994, 0x0521, WORD_LEN, 0},
    {0x0996, 0xD65C, WORD_LEN, 0},
    {0x0998, 0xFD05, WORD_LEN, 0},
    {0x099A, 0x23DE, WORD_LEN, 0},
    {0x099C, 0x3FEE, WORD_LEN, 0},
    {0x099E, 0x00AD, WORD_LEN, 0},
    {0x098A, 0x12A1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x00F7, WORD_LEN, 0},
    {0x0992, 0x13A0, WORD_LEN, 0},
    {0x0994, 0xD65D, WORD_LEN, 0},
    {0x0996, 0x4FFD, WORD_LEN, 0},
    {0x0998, 0x0521, WORD_LEN, 0},
    {0x099A, 0xD65E, WORD_LEN, 0},
    {0x099C, 0xFD05, WORD_LEN, 0},
    {0x099E, 0x23DE, WORD_LEN, 0},
    {0x098A, 0x12B1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3FEE, WORD_LEN, 0},
    {0x0992, 0x00AD, WORD_LEN, 0},
    {0x0994, 0x00F7, WORD_LEN, 0},
    {0x0996, 0x13A1, WORD_LEN, 0},
    {0x0998, 0xCC32, WORD_LEN, 0},
    {0x099A, 0xB230, WORD_LEN, 0},
    {0x099C, 0xED00, WORD_LEN, 0},
    {0x099E, 0xF613, WORD_LEN, 0},
    {0x098A, 0x12C1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xA04F, WORD_LEN, 0},
    {0x0992, 0x175F, WORD_LEN, 0},
    {0x0994, 0xFA13, WORD_LEN, 0},
    {0x0996, 0x9FBA, WORD_LEN, 0},
    {0x0998, 0x139E, WORD_LEN, 0},
    {0x099A, 0xED04, WORD_LEN, 0},
    {0x099C, 0xF613, WORD_LEN, 0},
    {0x099E, 0xA14F, WORD_LEN, 0},
    {0x098A, 0x12D1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED02, WORD_LEN, 0},
    {0x0992, 0xEA05, WORD_LEN, 0},
    {0x0994, 0xAA04, WORD_LEN, 0},
    {0x0996, 0xBD70, WORD_LEN, 0},
    {0x0998, 0xA838, WORD_LEN, 0},
    {0x099A, 0x3838, WORD_LEN, 0},
    {0x099C, 0x393C, WORD_LEN, 0},
    {0x099E, 0x3C3C, WORD_LEN, 0},
    {0x098A, 0x12E1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x3C3C, WORD_LEN, 0},
    {0x0992, 0x3CBD, WORD_LEN, 0},
    {0x0994, 0x7EF1, WORD_LEN, 0},
    {0x0996, 0xDE21, WORD_LEN, 0},
    {0x0998, 0xE644, WORD_LEN, 0},
    {0x099A, 0x4F30, WORD_LEN, 0},
    {0x099C, 0xED0A, WORD_LEN, 0},
    {0x099E, 0xDE1D, WORD_LEN, 0},
    {0x098A, 0x12F1, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x6D93, WORD_LEN, 0},
    {0x0992, 0x2607, WORD_LEN, 0},
    {0x0994, 0x8FC3, WORD_LEN, 0},
    {0x0996, 0x012F, WORD_LEN, 0},
    {0x0998, 0x8F20, WORD_LEN, 0},
    {0x099A, 0x07DE, WORD_LEN, 0},
    {0x099C, 0x1D8F, WORD_LEN, 0},
    {0x099E, 0xC301, WORD_LEN, 0},
    {0x098A, 0x1301, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x308F, WORD_LEN, 0},
    {0x0992, 0xE600, WORD_LEN, 0},
    {0x0994, 0x4F17, WORD_LEN, 0},
    {0x0996, 0x5F30, WORD_LEN, 0},
    {0x0998, 0xE30A, WORD_LEN, 0},
    {0x099A, 0xED0A, WORD_LEN, 0},
    {0x099C, 0xFC05, WORD_LEN, 0},
    {0x099E, 0x63ED, WORD_LEN, 0},
    {0x098A, 0x1311, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x02DE, WORD_LEN, 0},
    {0x0992, 0x21E6, WORD_LEN, 0},
    {0x0994, 0x444F, WORD_LEN, 0},
    {0x0996, 0x30ED, WORD_LEN, 0},
    {0x0998, 0x008F, WORD_LEN, 0},
    {0x099A, 0xC300, WORD_LEN, 0},
    {0x099C, 0x0630, WORD_LEN, 0},
    {0x099E, 0xED04, WORD_LEN, 0},
    {0x098A, 0x1321, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xEC0A, WORD_LEN, 0},
    {0x0992, 0xEE04, WORD_LEN, 0},
    {0x0994, 0xBDE2, WORD_LEN, 0},
    {0x0996, 0xCA30, WORD_LEN, 0},
    {0x0998, 0xEC06, WORD_LEN, 0},
    {0x099A, 0x2705, WORD_LEN, 0},
    {0x099C, 0xCCFF, WORD_LEN, 0},
    {0x099E, 0xFF20, WORD_LEN, 0},
    {0x098A, 0x1331, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x02EC, WORD_LEN, 0},
    {0x0992, 0x08FD, WORD_LEN, 0},
    {0x0994, 0x0563, WORD_LEN, 0},
    {0x0996, 0x30C6, WORD_LEN, 0},
    {0x0998, 0x0C3A, WORD_LEN, 0},
    {0x099A, 0x3539, WORD_LEN, 0},
    {0x099C, 0x3C3C, WORD_LEN, 0},
    {0x099E, 0xBDCD, WORD_LEN, 0},
    {0x098A, 0x1341, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xE4DE, WORD_LEN, 0},
    {0x0992, 0x231E, WORD_LEN, 0},
    {0x0994, 0x8E01, WORD_LEN, 0},
    {0x0996, 0x11CC, WORD_LEN, 0},
    {0x0998, 0x3C02, WORD_LEN, 0},
    {0x099A, 0x30ED, WORD_LEN, 0},
    {0x099C, 0x02CC, WORD_LEN, 0},
    {0x099E, 0xFFF3, WORD_LEN, 0},
    {0x098A, 0x1351, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0xED00, WORD_LEN, 0},
    {0x0992, 0xCC00, WORD_LEN, 0},
    {0x0994, 0x0CBD, WORD_LEN, 0},
    {0x0996, 0x70F9, WORD_LEN, 0},
    {0x0998, 0x3838, WORD_LEN, 0},
    {0x099A, 0x390B, WORD_LEN, 0},
    {0x099C, 0x0517, WORD_LEN, 0},
    {0x098A, 0x935F, WORD_LEN, 0}, 	// PHYSICAL_ADDR_ACCESS
    {0x0990, 0x00A9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x0010, WORD_LEN, 0}, 	// MCU_ADDRESS [MON_ADDR]
    {0x0990, 0x0D61, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x0003, WORD_LEN, 0}, 	// MCU_ADDRESS [MON_ALGO]
    {0x0990, 0x0004, WORD_LEN, 200}, 	// MCU_DATA_0
    
    // Color shift correction
    {0x316C, 0x84F0, WORD_LEN, 0}, 	// DAC_TXLO
    {0x098E, 0x4842, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x488A, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4815, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x485D, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0

    //lens shading
    /*
     * For narrow visual angle of lens only
     */
    //[85% with white paper in sunlight]
    {0x3640, 0x0230, WORD_LEN, 0}, 	// P_G1_P0Q0
    {0x3642, 0xCB8C, WORD_LEN, 0}, 	// P_G1_P0Q1
    {0x3644, 0x50F1, WORD_LEN, 0}, 	// P_G1_P0Q2
    {0x3646, 0x968E, WORD_LEN, 0}, 	// P_G1_P0Q3
    {0x3648, 0xA792, WORD_LEN, 0}, 	// P_G1_P0Q4
    {0x364A, 0x0030, WORD_LEN, 0}, 	// P_R_P0Q0
    {0x364C, 0xF36B, WORD_LEN, 0}, 	// P_R_P0Q1
    {0x364E, 0x4DF1, WORD_LEN, 0}, 	// P_R_P0Q2
    {0x3650, 0x8C2E, WORD_LEN, 0}, 	// P_R_P0Q3
    {0x3652, 0xA632, WORD_LEN, 0}, 	// P_R_P0Q4
    {0x3654, 0x0090, WORD_LEN, 0}, 	// P_B_P0Q0
    {0x3656, 0xF36B, WORD_LEN, 0}, 	// P_B_P0Q1
    {0x3658, 0x3771, WORD_LEN, 0}, 	// P_B_P0Q2
    {0x365A, 0xD2CE, WORD_LEN, 0}, 	// P_B_P0Q3
    {0x365C, 0xB452, WORD_LEN, 0}, 	// P_B_P0Q4
    {0x365E, 0x7F6F, WORD_LEN, 0}, 	// P_G2_P0Q0
    {0x3660, 0xD90C, WORD_LEN, 0}, 	// P_G2_P0Q1
    {0x3662, 0x4F11, WORD_LEN, 0}, 	// P_G2_P0Q2
    {0x3664, 0x90CE, WORD_LEN, 0}, 	// P_G2_P0Q3
    {0x3666, 0xB092, WORD_LEN, 0}, 	// P_G2_P0Q4
    {0x3680, 0x592D, WORD_LEN, 0}, 	// P_G1_P1Q0
    {0x3682, 0x93CD, WORD_LEN, 0}, 	// P_G1_P1Q1
    {0x3684, 0x490F, WORD_LEN, 0}, 	// P_G1_P1Q2
    {0x3686, 0x2A6F, WORD_LEN, 0}, 	// P_G1_P1Q3
    {0x3688, 0x9691, WORD_LEN, 0}, 	// P_G1_P1Q4
    {0x368A, 0x16CE, WORD_LEN, 0}, 	// P_R_P1Q0
    {0x368C, 0x0DEC, WORD_LEN, 0}, 	// P_R_P1Q1
    {0x368E, 0x04B0, WORD_LEN, 0}, 	// P_R_P1Q2
    {0x3690, 0x93AC, WORD_LEN, 0}, 	// P_R_P1Q3
    {0x3692, 0xF8F1, WORD_LEN, 0}, 	// P_R_P1Q4
    {0x3694, 0xCC2D, WORD_LEN, 0}, 	// P_B_P1Q0
    {0x3696, 0x28ED, WORD_LEN, 0}, 	// P_B_P1Q1
    {0x3698, 0x0E0C, WORD_LEN, 0}, 	// P_B_P1Q2
    {0x369A, 0xA7ED, WORD_LEN, 0}, 	// P_B_P1Q3
    {0x369C, 0x570D, WORD_LEN, 0}, 	// P_B_P1Q4
    {0x369E, 0xBCCD, WORD_LEN, 0}, 	// P_G2_P1Q0
    {0x36A0, 0x3E0D, WORD_LEN, 0}, 	// P_G2_P1Q1
    {0x36A2, 0xB6EE, WORD_LEN, 0}, 	// P_G2_P1Q2
    {0x36A4, 0x3AAE, WORD_LEN, 0}, 	// P_G2_P1Q3
    {0x36A6, 0x1C11, WORD_LEN, 0}, 	// P_G2_P1Q4
    {0x36C0, 0x65B1, WORD_LEN, 0}, 	// P_G1_P2Q0
    {0x36C2, 0xC88F, WORD_LEN, 0}, 	// P_G1_P2Q1
    {0x36C4, 0xC871, WORD_LEN, 0}, 	// P_G1_P2Q2
    {0x36C6, 0xFED0, WORD_LEN, 0}, 	// P_G1_P2Q3
    {0x36C8, 0xC154, WORD_LEN, 0}, 	// P_G1_P2Q4
    {0x36CA, 0x49D1, WORD_LEN, 0}, 	// P_R_P2Q0
    {0x36CC, 0xA54F, WORD_LEN, 0}, 	// P_R_P2Q1
    {0x36CE, 0x93D2, WORD_LEN, 0}, 	// P_R_P2Q2
    {0x36D0, 0x8AD0, WORD_LEN, 0}, 	// P_R_P2Q3
    {0x36D2, 0xC113, WORD_LEN, 0}, 	// P_R_P2Q4
    {0x36D4, 0x5D71, WORD_LEN, 0}, 	// P_B_P2Q0
    {0x36D6, 0x8890, WORD_LEN, 0}, 	// P_B_P2Q1
    {0x36D8, 0xD853, WORD_LEN, 0}, 	// P_B_P2Q2
    {0x36DA, 0x2711, WORD_LEN, 0}, 	// P_B_P2Q3
    {0x36DC, 0x1D34, WORD_LEN, 0}, 	// P_B_P2Q4
    {0x36DE, 0x6371, WORD_LEN, 0}, 	// P_G2_P2Q0
    {0x36E0, 0xDCEF, WORD_LEN, 0}, 	// P_G2_P2Q1
    {0x36E2, 0xE891, WORD_LEN, 0}, 	// P_G2_P2Q2
    {0x36E4, 0xE1B0, WORD_LEN, 0}, 	// P_G2_P2Q3
    {0x36E6, 0xA474, WORD_LEN, 0}, 	// P_G2_P2Q4
    {0x3700, 0x47CF, WORD_LEN, 0}, 	// P_G1_P3Q0
    {0x3702, 0x06AF, WORD_LEN, 0}, 	// P_G1_P3Q1
    {0x3704, 0xFC70, WORD_LEN, 0}, 	// P_G1_P3Q2
    {0x3706, 0x9372, WORD_LEN, 0}, 	// P_G1_P3Q3
    {0x3708, 0x6D72, WORD_LEN, 0}, 	// P_G1_P3Q4
    {0x370A, 0x5F2D, WORD_LEN, 0}, 	// P_R_P3Q0
    {0x370C, 0x18AC, WORD_LEN, 0}, 	// P_R_P3Q1
    {0x370E, 0xEF31, WORD_LEN, 0}, 	// P_R_P3Q2
    {0x3710, 0x4BED, WORD_LEN, 0}, 	// P_R_P3Q3
    {0x3712, 0x1B14, WORD_LEN, 0}, 	// P_R_P3Q4
    {0x3714, 0x2550, WORD_LEN, 0}, 	// P_B_P3Q0
    {0x3716, 0x8990, WORD_LEN, 0}, 	// P_B_P3Q1
    {0x3718, 0x9B4F, WORD_LEN, 0}, 	// P_B_P3Q2
    {0x371A, 0x1CD2, WORD_LEN, 0}, 	// P_B_P3Q3
    {0x371C, 0xAD4F, WORD_LEN, 0}, 	// P_B_P3Q4
    {0x371E, 0x582F, WORD_LEN, 0}, 	// P_G2_P3Q0
    {0x3720, 0x6ED0, WORD_LEN, 0}, 	// P_G2_P3Q1
    {0x3722, 0x0E33, WORD_LEN, 0}, 	// P_G2_P3Q2
    {0x3724, 0xD513, WORD_LEN, 0}, 	// P_G2_P3Q3
    {0x3726, 0x98B5, WORD_LEN, 0}, 	// P_G2_P3Q4
    {0x3740, 0xBA72, WORD_LEN, 0}, 	// P_G1_P4Q0
    {0x3742, 0x9990, WORD_LEN, 0}, 	// P_G1_P4Q1
    {0x3744, 0xDE76, WORD_LEN, 0}, 	// P_G1_P4Q2
    {0x3746, 0x4C75, WORD_LEN, 0}, 	// P_G1_P4Q3
    {0x3748, 0x1839, WORD_LEN, 0}, 	// P_G1_P4Q4
    {0x374A, 0xD3F2, WORD_LEN, 0}, 	// P_R_P4Q0
    {0x374C, 0x0230, WORD_LEN, 0}, 	// P_R_P4Q1
    {0x374E, 0x8516, WORD_LEN, 0}, 	// P_R_P4Q2
    {0x3750, 0x4974, WORD_LEN, 0}, 	// P_R_P4Q3
    {0x3752, 0x53F8, WORD_LEN, 0}, 	// P_R_P4Q4
    {0x3754, 0xA473, WORD_LEN, 0}, 	// P_B_P4Q0
    {0x3756, 0x1F72, WORD_LEN, 0}, 	// P_B_P4Q1
    {0x3758, 0xD453, WORD_LEN, 0}, 	// P_B_P4Q2
    {0x375A, 0x7A8F, WORD_LEN, 0}, 	// P_B_P4Q3
    {0x375C, 0x07B8, WORD_LEN, 0}, 	// P_B_P4Q4
    {0x375E, 0xBC72, WORD_LEN, 0}, 	// P_G2_P4Q0
    {0x3760, 0xA6D0, WORD_LEN, 0}, 	// P_G2_P4Q1
    {0x3762, 0xDDB6, WORD_LEN, 0}, 	// P_G2_P4Q2
    {0x3764, 0x5DF5, WORD_LEN, 0}, 	// P_G2_P4Q3
    {0x3766, 0x1579, WORD_LEN, 0}, 	// P_G2_P4Q4
    {0x3782, 0x02E0, WORD_LEN, 0}, 	// CENTER_ROW
    {0x3784, 0x0450, WORD_LEN, 0}, 	// CENTER_COLUMN
    {0x3210, 0x01B8, WORD_LEN, 0}, 	// COLOR_PIPELINE_CONTROL
    
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x0242, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFE23, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0x00B7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFC6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x0176, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFFDE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0xFF9E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFE81, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x02FC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_9]
    {0x0990, 0x001E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_10]
    {0x0990, 0x003A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0034, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0041, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0xFF89, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x001C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0xFFF9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0xFFE9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0062, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x008A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0xFF12, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_9]
    {0x0990, 0x0019, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48DA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_10]
    {0x0990, 0xFFE6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4906, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_XSHIFT_PRE_ADJ]
    {0x0990, 0x002F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4908, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_YSHIFT_PRE_ADJ]
    {0x0990, 0x0024, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8F4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_XSCALE]
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC8F5, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_YSCALE]
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48F6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_0]
    {0x0990, 0xCBEF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48F8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_1]
    {0x0990, 0xEA40, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48FA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_2]
    {0x0990, 0x95A0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48FC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_3]
    {0x0990, 0x509D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48FE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_4]
    {0x0990, 0x1680, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4900, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_5]
    {0x0990, 0xAD6D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4902, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_6]
    {0x0990, 0xFF4D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4904, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_AWB_WEIGHTS_7]
    {0x0990, 0xC757, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_9]
    {0x0990, 0x001E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_10]
    {0x0990, 0x003E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_9]
    {0x0990, 0x0015, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48DA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_10]
    {0x0990, 0xFFE9, WORD_LEN, 0}, 	// MCU_DATA_0

    // Pa calib
    {0x098E, 0x6837, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AE_TRACK_AE_MIN_VIRT_AGAIN]
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C37, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AE_TRACK_AE_MIN_VIRT_AGAIN]
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x682F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AE_TRACK_TARGET_AGAIN]
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C2F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AE_TRACK_TARGET_AGAIN]
    {0x0990, 0x0046, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x0011, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x003F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x0067, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x0085, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x009B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x00AD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x00BB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00C7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00D1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00DA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00E1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00E8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00EE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00F3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00FB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_1]
    {0x0990, 0x0011, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC20, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_2]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC21, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_3]
    {0x0990, 0x003F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC22, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_4]
    {0x0990, 0x0067, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC23, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_5]
    {0x0990, 0x0085, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC24, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_6]
    {0x0990, 0x009B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC25, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_7]
    {0x0990, 0x00AD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC26, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_8]
    {0x0990, 0x00BB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC27, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_9]
    {0x0990, 0x00C7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC28, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_10]
    {0x0990, 0x00D1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC29, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_11]
    {0x0990, 0x00DA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_12]
    {0x0990, 0x00E1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_13]
    {0x0990, 0x00E8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_14]
    {0x0990, 0x00EE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_15]
    {0x0990, 0x00F3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_16]
    {0x0990, 0x00F7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC2F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_17]
    {0x0990, 0x00FB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC30, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_NEUTRAL_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4926, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_START_GAMMA_BM]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4928, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_MID_GAMMA_BM]
    {0x0990, 0x0064, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x492A, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_STOP_GAMMA_BM]
    {0x0990, 0x0082, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x3C4D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_START_GAMMA_FTB]
    {0x0990, 0x1B58, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x3C4F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_STOP_GAMMA_FTB]
    {0x0990, 0x2AF8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x494B, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_START_GAIN_METRIC]
    {0x0990, 0x00C0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x494D, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_STOP_GAIN_METRIC]
    {0x0990, 0x0258, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x686B, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_BRIGHTNESS]
    {0x0990, 0x0002, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x686D, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_STOP_BRIGHTNESS]
    {0x0990, 0x0032, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C6B, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_BRIGHTNESS]
    {0x0990, 0x0002, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C6D, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_STOP_BRIGHTNESS]
    {0x0990, 0x0032, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4960, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_CONFIG]
    {0x0990, 0x7800, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4960, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_CONFIG]
    {0x0990, 0x6800, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4960, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_CONFIG]
    {0x0990, 0x4800, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4960, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_CONFIG]
    {0x0990, 0x4800, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6867, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_ALGO_RUN]
    {0x0990, 0x0174, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6867, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_ALGO_RUN]
    {0x0990, 0x01F4, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4972, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SOBEL_FLAT_START]
    {0x0990, 0x000F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4974, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SOBEL_FLAT_STOP]
    {0x0990, 0x00FE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4976, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SOBEL_SHARP_START]
    {0x0990, 0x001F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x4978, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SOBEL_SHARP_STOP]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC966, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_RAMP_START]
    {0x0990, 0x001F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC968, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SLOPE_START]
    {0x0990, 0x003C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC96A, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_THSTART]
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC967, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_RAMP_STOP]
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC969, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_SLOPE_STOP]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC96B, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_EXT_SFFB_THSTOP]
    {0x0990, 0x001F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC948, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_SYS_UV_COLOR_BOOST]
    {0x0990, 0x0004, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x687F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_SYSCTRL_ALGO_RUN]
    {0x0990, 0x002E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x6C7F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_SYSCTRL_ALGO_RUN]
    {0x0990, 0x0020, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC917, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_LL_START_1]
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC918, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_LL_START_2]
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC91A, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_LL_STOP_1]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC91B, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_LL_STOP_2]
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x326C, 0x0C00, WORD_LEN, 0}, 	// APERTURE_PARAMETERS_2D
    {0x098E, 0xC922, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_NR_STOP_0]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC923, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_NR_STOP_1]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC924, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_NR_STOP_2]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC925, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_NR_STOP_3]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC919, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_LL_STOP_0]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x3C51, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_CLUSTER_THRESHOLD]
    {0x0990, 0x0100, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE885, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_SYSCTRL_SEPIA_CR]
    {0x0990, 0x001E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE886, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_SYSCTRL_SEPIA_CB]
    {0x0990, 0x00D8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC85, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_SYSCTRL_SEPIA_CR]
    {0x0990, 0x001E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC86, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_SYSCTRL_SEPIA_CB]
    {0x0990, 0x00D8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE884, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_SYSCTRL_SOLARIZATION_TH]
    {0x0990, 0x0041, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC84, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_SYSCTRL_SOLARIZATION_TH]
    {0x0990, 0x0041, WORD_LEN, 0}, 	// MCU_DATA_0

    // for Day light
    {0x098E, 0xE84F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_K_B_R]
    {0x0990, 0x0083, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE84D, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_K_R_R]
    {0x0990, 0x0084, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC4F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_B_R]
    {0x0990, 0x0083, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC4D, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_R_R]
    {0x0990, 0x0084, WORD_LEN, 0}, 	// MCU_DATA_0
    
    
    {0x098E, 0xEC4E, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_G_R]
    {0x0990, 0x007E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC4E, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_G_R]
    {0x0990, 0x007E, WORD_LEN, 0}, 	// MCU_DATA_0

    // for A light
    {0x098E, 0xE84A, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_K_R_L]
    {0x0990, 0x0073, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE84C, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_K_B_L]
    {0x0990, 0x008B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC4A, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_R_L]
    {0x0990, 0x0073, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC4C, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_AWB_K_B_L]
    {0x0990, 0x008B, WORD_LEN, 0}, 	// MCU_DATA_0

    // Saturation / DDCCM
    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0006, WORD_LEN, 200}, 	// MCU_DATA_0
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0098, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0038, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0098, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0038, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC914, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_START_DESATURATION]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xC915, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_LL_END_DESATURATION]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    
    // AE Target
    {0x098E, 0xE81F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AE_RULE_BASE_TARGET]
    {0x0990, 0x0032, WORD_LEN, 0}, 	// MCU_DATA_0
    
    // Section 8
    {0x0018, 0x002A, WORD_LEN, 0}, 	// STANDBY_CONTROL_AND_STATUS
    {0x3084, 0x2409, WORD_LEN, 0}, 	// DAC_LD_4_5
    {0x3092, 0x0A49, WORD_LEN, 0}, 	// DAC_LD_18_19
    {0x3094, 0x4949, WORD_LEN, 0}, 	// DAC_LD_20_21
    {0x3096, 0x4950, WORD_LEN, 5}, 	// DAC_LD_22_23
    /*
     * ZTE_CAM_LJ_20101223
     * remove the following reg,else it will cause loss of preview data for v9 
     */
    //{0x3C20, 0x0000, WORD_LEN, 0}, 	// TX_SS_CONTROL
    //{0x001E, 0x0777, WORD_LEN, 5}, 	// PAD_SLEW_PAD_CONFIG
    
    {0x0018, 0x002A, WORD_LEN, 20}, 	// STANDBY_CONTROL_AND_STATUS=
};

static struct mt9t11x_i2c_reg_conf const wb_cloudy_tbl[] = {
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x01F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFF27, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0xFFFA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFFD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x01A6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFF76, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFF6A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x018D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC02, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_MODE]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x2C03, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_ALGO]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x683F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_ALGO_RUN]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3C, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_R_RATIO_PRE_AWB]
    {0x0990, 0x0035, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3D, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_B_RATIO_PRE_AWB]
    {0x0990, 0x006E, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const wb_daylight_tbl[] = {
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x01F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFF27, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0xFFFA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFFD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x01A6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFF76, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFF6A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x018D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC02, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_MODE]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x2C03, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_ALGO]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x683F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_ALGO_RUN]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3C, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_R_RATIO_PRE_AWB]
    {0x0990, 0x0037, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3D, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_B_RATIO_PRE_AWB]
    {0x0990, 0x0065, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const wb_flourescant_tbl[] = {
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x01F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFF27, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0xFFFA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFFD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x01A6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFF76, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFF6A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x018D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC02, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_MODE]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x2C03, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_ALGO]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x683F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_ALGO_RUN]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3C, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_R_RATIO_PRE_AWB]
    {0x0990, 0x0048, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3D, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_B_RATIO_PRE_AWB]
    {0x0990, 0x004A, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const wb_incandescent_tbl[] = {
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x0180, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFF7A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0x0018, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFCA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x017C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFFCC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0x000C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFF1F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x01E8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC02, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_MODE]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x2C03, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_ALGO]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x683F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_AWB_ALGO_RUN]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3C, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_R_RATIO_PRE_AWB]
    {0x0990, 0x0066, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC3D, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_B_RATIO_PRE_AWB]
    {0x0990, 0x0034, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const wb_auto_tbl[] = {
    {0x098E, 0x48B0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_0]
    {0x0990, 0x0242, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_1]
    {0x0990, 0xFE23, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_2]
    {0x0990, 0x00B7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_3]
    {0x0990, 0xFFC6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48B8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_4]
    {0x0990, 0x0176, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_5]
    {0x0990, 0xFFDE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_6]
    {0x0990, 0xFF9E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48BE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_7]
    {0x0990, 0xFE81, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_L_8]
    {0x0990, 0x02FC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_0]
    {0x0990, 0x0034, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48C8, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_1]
    {0x0990, 0x0041, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CA, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_2]
    {0x0990, 0xFF89, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CC, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_3]
    {0x0990, 0x001C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48CE, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_4]
    {0x0990, 0xFFF9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D0, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_5]
    {0x0990, 0xFFE9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D2, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_6]
    {0x0990, 0x0062, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D4, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_7]
    {0x0990, 0x008A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x48D6, WORD_LEN, 0}, 	// MCU_ADDRESS [CAM1_AWB_CCM_RL_8]
    {0x0990, 0xFF12, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xAC02, WORD_LEN, 0},  // MCU_ADDRESS [AWB_MODE]
    {0x0990, 0x000A, WORD_LEN, 0},  // MCU_DATA_0
    {0x098E, 0x2C03, WORD_LEN, 0},  // MCU_ADDRESS [AWB_ALGO]
    {0x0990, 0x01ff, WORD_LEN, 0},  // MCU_DATA_0
    {0x098E, 0x683F, WORD_LEN, 0},  // MCU_ADDRESS [PRI_A_CONFIG_AWB_ALGO_RUN]
    {0x0990, 0x01ff, WORD_LEN, 0},  // MCU_DATA_0
};

/* Auto focus Setup */
static struct mt9t11x_i2c_reg_conf const af_tbl[] = {
    {0x0604, 0x0101, WORD_LEN, 0},    // GPIO_DATA
    {0x0606, 0x0001, WORD_LEN, 0},    // GPIO_DIR
    {0x098E, 0x4403, WORD_LEN, 0},    // MCU_ADDRESS [AFM_ALGO]
    {0x0990, 0x8001, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x440B, WORD_LEN, 0},    // MCU_ADDRESS [AFM_POS_MIN]
    {0x0990, 0x0032, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x440D, WORD_LEN, 0},    // MCU_ADDRESS [AFM_POS_MAX]
    {0x0990, 0x03B6, WORD_LEN, 0},    // MCU_DATA_0

    //AF TWICE
    {0x098E, 0xB002, WORD_LEN, 0},    // MCU_ADDRESS [AFM_POS_MAX]
    {0x0990, 0x0002, WORD_LEN, 0},    // MCU_DATA_0
    
    {0x098E, 0x8400, WORD_LEN, 0},    // MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0006, WORD_LEN, 50},// MCU_DATA_0
    {0x098E, 0x3020, WORD_LEN, 0},    // MCU_ADDRESS [AF_ZONE_WEIGHTS_HI]
    {0x0990, 0x557D, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x3022, WORD_LEN, 0},    // MCU_ADDRESS [AF_ZONE_WEIGHTS_LO]
    {0x0990, 0x7D55, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0xB024, WORD_LEN, 0},    // MCU_ADDRESS [AF_BEST_POSITION]
    {0x0990, 0x0000, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x3003, WORD_LEN, 0},    // MCU_ADDRESS [AF_ALGO]


    {0x0990, 0x0008, WORD_LEN, 0},    // MCU_DATA_0  ZTE_CAM_WT_20110215 0x0002 -> 0x0008
    //AF window enlarged    
    {0x098E, 0x3005, WORD_LEN, 0},    // MCU_ADDRESS [AF_BEST_POSITION]
    {0x0990, 0x0059, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x3007, WORD_LEN, 0},    // MCU_ADDRESS [AF_BEST_POSITION]
    {0x0990, 0x0050, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x3009, WORD_LEN, 0},    // MCU_ADDRESS [AF_BEST_POSITION]
    {0x0990, 0x00D8, WORD_LEN, 0},    // MCU_DATA_0
    {0x098E, 0x300B, WORD_LEN, 0},    // MCU_ADDRESS [AF_BEST_POSITION]
    {0x0990, 0x009E, WORD_LEN, 0},    // MCU_DATA_0

    {0x098E, 0xB019, WORD_LEN, 0},    // MCU_ADDRESS [AF_ALGO]
    {0x0990, 0x0001, WORD_LEN, 50},// MCU_DATA_0
};

/* Contrast Setup */
static struct mt9t11x_i2c_reg_conf const contrast_tbl_0[] = {
    {0x098E, 0xBC08, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_SELECT]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x0030, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0040, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x0054, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x006F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x0083, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x0092, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x00A0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x00AC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00B7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00C1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00CB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00D3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00DC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00E3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00EB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F2, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const contrast_tbl_1[] = {
    {0x098E, 0xBC08, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_SELECT]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x000D, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0024, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x0042, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x0064, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x007E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x0092, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x00A3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x00B1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00BC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00C7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00D0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00D9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00E0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00E7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00EE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F4, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00FA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const contrast_tbl_2[] = {
    //[Contrast]
    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x0011, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0023, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x003F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x0067, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x0085, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x009B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x00AD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x00BB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00C7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00D1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00DA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00E1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00E8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00EE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00F3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00FB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const contrast_tbl_3[] = {
    {0x098E, 0xBC08, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_SELECT]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0

    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x0008, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0017, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x002E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x004F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x006C, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x0088, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x009F, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x00B0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00BE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00CA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00D4, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00DC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00E4, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00EA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00F0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F5, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00FA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0

    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const contrast_tbl_4[] = {
    {0x098E, 0xBC08, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_SELECT]
    {0x0990, 0x0001, WORD_LEN, 0}, 	// MCU_DATA_0

    {0x098E, 0xBC0B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_0]
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_1]
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_2]
    {0x0990, 0x0009, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0E, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_3]
    {0x0990, 0x0015, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC0F, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_4]
    {0x0990, 0x002B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC10, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_5]
    {0x0990, 0x0042, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC11, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_6]
    {0x0990, 0x005B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC12, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_7]
    {0x0990, 0x0078, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC13, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_8]
    {0x0990, 0x0095, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC14, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_9]
    {0x0990, 0x00AB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC15, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_10]
    {0x0990, 0x00BD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC16, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_11]
    {0x0990, 0x00CB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC17, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_12]
    {0x0990, 0x00D7, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC18, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_13]
    {0x0990, 0x00E0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC19, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_14]
    {0x0990, 0x00E8, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1A, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_15]
    {0x0990, 0x00EF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1B, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_16]
    {0x0990, 0x00F5, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1C, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_17]
    {0x0990, 0x00FA, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xBC1D, WORD_LEN, 0}, 	// MCU_ADDRESS [LL_GAMMA_CONTRAST_CURVE_18]
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0


    {0x098E, 0x8400, WORD_LEN, 0}, 	// MCU_ADDRESS [SEQ_CMD]
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const *contrast_tbl[] = {
    contrast_tbl_0,
    contrast_tbl_1,
    contrast_tbl_2,
    contrast_tbl_3,
    contrast_tbl_4,
};

static uint16_t const contrast_tbl_sz[] = {
    ARRAY_SIZE(contrast_tbl_0),
    ARRAY_SIZE(contrast_tbl_1),
    ARRAY_SIZE(contrast_tbl_2),
    ARRAY_SIZE(contrast_tbl_3),
    ARRAY_SIZE(contrast_tbl_4),
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_0[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0010, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_1[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x001E, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_2[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0028, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_3[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0032, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_4[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0042, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_5[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0058, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const brightness_tbl_6[] = {
    {0x098E, 0xE81F, WORD_LEN, 0},
    {0x0990, 0x0072, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const *brightness_tbl[] = {
    brightness_tbl_0,
    brightness_tbl_1,
    brightness_tbl_2,
    brightness_tbl_3,
    brightness_tbl_4,
    brightness_tbl_5,
    brightness_tbl_6,
};

static uint16_t const brightness_tbl_sz[] = {
    ARRAY_SIZE(brightness_tbl_0),
    ARRAY_SIZE(brightness_tbl_1),
    ARRAY_SIZE(brightness_tbl_2),
    ARRAY_SIZE(brightness_tbl_3),
    ARRAY_SIZE(brightness_tbl_4),
    ARRAY_SIZE(brightness_tbl_5),
    ARRAY_SIZE(brightness_tbl_6),
};

static struct mt9t11x_i2c_reg_conf const saturation_tbl_0[] = {
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0060, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0010, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0060, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0010, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const saturation_tbl_1[] = {
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0028, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0028, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const saturation_tbl_2[] = {
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0098, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0038, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x0098, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0038, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const saturation_tbl_3[] = {
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x00B0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0048, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x00B0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0048, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const saturation_tbl_4[] = {
    {0x098E, 0xE86F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_START_SATURATION]
    {0x0990, 0x00D0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xE870, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_A_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0060, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC6F, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_START_SATURATION]
    {0x0990, 0x00D0, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098E, 0xEC70, WORD_LEN, 0}, 	// MCU_ADDRESS [PRI_B_CONFIG_LL_END_SATURATION]
    {0x0990, 0x0060, WORD_LEN, 0}, 	// MCU_DATA_0
};

static struct mt9t11x_i2c_reg_conf const *saturation_tbl[] = {
    saturation_tbl_0,
    saturation_tbl_1,
    saturation_tbl_2,
    saturation_tbl_3,
    saturation_tbl_4,
};

static uint16_t const saturation_tbl_sz[] = {
    ARRAY_SIZE(saturation_tbl_0),
    ARRAY_SIZE(saturation_tbl_1),
    ARRAY_SIZE(saturation_tbl_2),
    ARRAY_SIZE(saturation_tbl_3),
    ARRAY_SIZE(saturation_tbl_4),
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_0[] = {
    {0x326C, 0x080A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},    
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_1[] = {
    {0x326C, 0x0A0A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0002, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_2[] = {
    {0x326C, 0x0B0A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0005, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0}, 
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_3[] = {
    {0x326C, 0x160A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0006, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},    
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_4[] = {
    {0x326C, 0x170A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0007, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},    
};

/*
 * sharpness settings for different brightness level
 */
static struct mt9t11x_i2c_reg_conf const sharpness_tbl_5[] = {
    //{0x326C, 0x110A, WORD_LEN, 0},
    {0x326C, 0x080A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},    
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_6[] = {
    //{0x326C, 0x120A, WORD_LEN, 0},
    {0x326C, 0x0A0A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0002, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},
    {0x0990, 0x000A, WORD_LEN, 0},    
};


static struct mt9t11x_i2c_reg_conf const sharpness_tbl_7[] = {
    {0x098E, 0xC917, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_1]
    {0x0990, 0x0002, WORD_LEN, 0},     // MCU_DATA_0
    {0x098E, 0xC918, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_2]
    {0x0990, 0x0000, WORD_LEN, 0},     // MCU_DATA_0                
    {0x326C, 0x1C02, WORD_LEN, 0},     // APERTURE_PARAMETERS_2D
};

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_8[] = {
    {0x098E, 0xC917, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_1]
    {0x0990, 0x0004, WORD_LEN, 0},     // MCU_DATA_0
    {0x098E, 0xC918, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_2]
    {0x0990, 0x0002, WORD_LEN, 0},     // MCU_DATA_0                   
    {0x326C, 0x1C02, WORD_LEN, 0},     // APERTURE_PARAMETERS_2D
};    

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_9[] = {
    {0x098E, 0xC917, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_1]
    {0x0990, 0x0006, WORD_LEN, 0},     // MCU_DATA_0
    {0x098E, 0xC918, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_2]
    {0x0990, 0x0002, WORD_LEN, 0},     // MCU_DATA_0             
    {0x326C, 0x1C02, WORD_LEN, 0},     // APERTURE_PARAMETERS_2D
};    

static struct mt9t11x_i2c_reg_conf const sharpness_tbl_10[] = {
    //{0x326C, 0x110A, WORD_LEN, 0},
    {0x326C, 0x080A, WORD_LEN, 0},
    {0x098E, 0xC917, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098E, 0xC918, WORD_LEN, 0},     // MCU_ADDRESS [CAM1_LL_LL_START_2]
    {0x0990, 0x000A, WORD_LEN, 0},     // MCU_DATA_0   
};

static struct mt9t11x_i2c_reg_conf const *sharpness_tbl[] = {
    sharpness_tbl_0,
    sharpness_tbl_1,
    sharpness_tbl_2,
    sharpness_tbl_3,
    sharpness_tbl_4,
    sharpness_tbl_5,
    sharpness_tbl_6,
    sharpness_tbl_7,
    sharpness_tbl_8,
    sharpness_tbl_9,    
    sharpness_tbl_10,
};

static uint16_t const sharpness_tbl_sz[] = {
    ARRAY_SIZE(sharpness_tbl_0),
    ARRAY_SIZE(sharpness_tbl_1),
    ARRAY_SIZE(sharpness_tbl_2),
    ARRAY_SIZE(sharpness_tbl_3),
    ARRAY_SIZE(sharpness_tbl_4),
    ARRAY_SIZE(sharpness_tbl_5),
    ARRAY_SIZE(sharpness_tbl_6),
    ARRAY_SIZE(sharpness_tbl_7),
    ARRAY_SIZE(sharpness_tbl_8),
    ARRAY_SIZE(sharpness_tbl_9),
    ARRAY_SIZE(sharpness_tbl_10),    
};

static struct mt9t11x_i2c_reg_conf const exposure_tbl_0[] = {
    {0x337E, 0xC000, WORD_LEN, 0},  // Y_RGB_OFFSET
};

static struct mt9t11x_i2c_reg_conf const exposure_tbl_1[] = {
    {0x337E, 0xE000, WORD_LEN, 0},  // Y_RGB_OFFSET
};

static struct mt9t11x_i2c_reg_conf const exposure_tbl_2[] = {
    {0x337E, 0x0000, WORD_LEN, 0},  // Y_RGB_OFFSET
};

static struct mt9t11x_i2c_reg_conf const exposure_tbl_3[] = {
    {0x337E, 0x1800, WORD_LEN, 0},  // Y_RGB_OFFSET
};

static struct mt9t11x_i2c_reg_conf const exposure_tbl_4[] = {
    {0x337E, 0x3000, WORD_LEN, 0},  // Y_RGB_OFFSET
};

static struct mt9t11x_i2c_reg_conf const *exposure_tbl[] = {
    exposure_tbl_0,
    exposure_tbl_1,
    exposure_tbl_2,
    exposure_tbl_3,
    exposure_tbl_4,
};

static uint16_t const exposure_tbl_sz[] = {
    ARRAY_SIZE(exposure_tbl_0),
    ARRAY_SIZE(exposure_tbl_1),
    ARRAY_SIZE(exposure_tbl_2),
    ARRAY_SIZE(exposure_tbl_3),
    ARRAY_SIZE(exposure_tbl_4),
};

/*
 * For wide visual angle of lens only
 */
static struct mt9t11x_i2c_reg_conf const lens_for_outdoor_tbl[] ={
#if 0 // ZTE_ZT_CAM_20101216 disused
    //[85% with white paper in sunlight]
    {0x3640, 0x08B0, WORD_LEN, 0}, 	// P_G1_P0Q0
    {0x3642, 0x9F8E, WORD_LEN, 0}, 	// P_G1_P0Q1
    {0x3644, 0x6A71, WORD_LEN, 0}, 	// P_G1_P0Q2
    {0x3646, 0x288D, WORD_LEN, 0}, 	// P_G1_P0Q3
    {0x3648, 0x9212, WORD_LEN, 0}, 	// P_G1_P0Q4
    {0x364A, 0x02B0, WORD_LEN, 0}, 	// P_R_P0Q0
    {0x364C, 0xD24E, WORD_LEN, 0}, 	// P_R_P0Q1
    {0x364E, 0x1432, WORD_LEN, 0}, 	// P_R_P0Q2
    {0x3650, 0x3D8E, WORD_LEN, 0}, 	// P_R_P0Q3
    {0x3652, 0xA372, WORD_LEN, 0}, 	// P_R_P0Q4
    {0x3654, 0x0690, WORD_LEN, 0}, 	// P_B_P0Q0
    {0x3656, 0xD66D, WORD_LEN, 0}, 	// P_B_P0Q1
    {0x3658, 0x3C51, WORD_LEN, 0}, 	// P_B_P0Q2
    {0x365A, 0x7FAD, WORD_LEN, 0}, 	// P_B_P0Q3
    {0x365C, 0xDE51, WORD_LEN, 0}, 	// P_B_P0Q4
    {0x365E, 0x05D0, WORD_LEN, 0}, 	// P_G2_P0Q0
    {0x3660, 0xA5AE, WORD_LEN, 0}, 	// P_G2_P0Q1
    {0x3662, 0x6B91, WORD_LEN, 0}, 	// P_G2_P0Q2
    {0x3664, 0x664D, WORD_LEN, 0}, 	// P_G2_P0Q3
    {0x3666, 0x9BB2, WORD_LEN, 0}, 	// P_G2_P0Q4
    {0x3680, 0x382E, WORD_LEN, 0}, 	// P_G1_P1Q0
    {0x3682, 0xAB8F, WORD_LEN, 0}, 	// P_G1_P1Q1
    {0x3684, 0x6690, WORD_LEN, 0}, 	// P_G1_P1Q2
    {0x3686, 0x1472, WORD_LEN, 0}, 	// P_G1_P1Q3
    {0x3688, 0xD712, WORD_LEN, 0}, 	// P_G1_P1Q4
    {0x368A, 0x5AEE, WORD_LEN, 0}, 	// P_R_P1Q0
    {0x368C, 0xA0CF, WORD_LEN, 0}, 	// P_R_P1Q1
    {0x368E, 0x18B1, WORD_LEN, 0}, 	// P_R_P1Q2
    {0x3690, 0x0C72, WORD_LEN, 0}, 	// P_R_P1Q3
    {0x3692, 0xF1B2, WORD_LEN, 0}, 	// P_R_P1Q4
    {0x3694, 0xFBC8, WORD_LEN, 0}, 	// P_B_P1Q0
    {0x3696, 0xA36E, WORD_LEN, 0}, 	// P_B_P1Q1
    {0x3698, 0x082F, WORD_LEN, 0}, 	// P_B_P1Q2
    {0x369A, 0x2D31, WORD_LEN, 0}, 	// P_B_P1Q3
    {0x369C, 0xDBAF, WORD_LEN, 0}, 	// P_B_P1Q4
    {0x369E, 0xC069, WORD_LEN, 0}, 	// P_G2_P1Q0
    {0x36A0, 0xB20E, WORD_LEN, 0}, 	// P_G2_P1Q1
    {0x36A2, 0x6A2F, WORD_LEN, 0}, 	// P_G2_P1Q2
    {0x36A4, 0x5C31, WORD_LEN, 0}, 	// P_G2_P1Q3
    {0x36A6, 0xB791, WORD_LEN, 0}, 	// P_G2_P1Q4
    {0x36C0, 0x5532, WORD_LEN, 0}, 	// P_G1_P2Q0
    {0x36C2, 0xEC0E, WORD_LEN, 0}, 	// P_G1_P2Q1
    {0x36C4, 0xE9F4, WORD_LEN, 0}, 	// P_G1_P2Q2
    {0x36C6, 0xD472, WORD_LEN, 0}, 	// P_G1_P2Q3
    {0x36C8, 0x1955, WORD_LEN, 0}, 	// P_G1_P2Q4
    {0x36CA, 0x5E52, WORD_LEN, 0}, 	// P_R_P2Q0
    {0x36CC, 0x8B70, WORD_LEN, 0}, 	// P_R_P2Q1
    {0x36CE, 0xA1B4, WORD_LEN, 0}, 	// P_R_P2Q2
    {0x36D0, 0x298E, WORD_LEN, 0}, 	// P_R_P2Q3
    {0x36D2, 0xEB32, WORD_LEN, 0}, 	// P_R_P2Q4
    {0x36D4, 0x33F2, WORD_LEN, 0}, 	// P_B_P2Q0
    {0x36D6, 0x460D, WORD_LEN, 0}, 	// P_B_P2Q1
    {0x36D8, 0xDB14, WORD_LEN, 0}, 	// P_B_P2Q2
    {0x36DA, 0xBA73, WORD_LEN, 0}, 	// P_B_P2Q3
    {0x36DC, 0x3635, WORD_LEN, 0}, 	// P_B_P2Q4
    {0x36DE, 0x5412, WORD_LEN, 0}, 	// P_G2_P2Q0
    {0x36E0, 0x83AE, WORD_LEN, 0}, 	// P_G2_P2Q1
    {0x36E2, 0xEF34, WORD_LEN, 0}, 	// P_G2_P2Q2
    {0x36E4, 0x85B3, WORD_LEN, 0}, 	// P_G2_P2Q3
    {0x36E6, 0x2C15, WORD_LEN, 0}, 	// P_G2_P2Q4
    {0x3700, 0xB36F, WORD_LEN, 0}, 	// P_G1_P3Q0
    {0x3702, 0x3893, WORD_LEN, 0}, 	// P_G1_P3Q1
    {0x3704, 0xA6F5, WORD_LEN, 0}, 	// P_G1_P3Q2
    {0x3706, 0xF3D5, WORD_LEN, 0}, 	// P_G1_P3Q3
    {0x3708, 0x3177, WORD_LEN, 0}, 	// P_G1_P3Q4
    {0x370A, 0x0248, WORD_LEN, 0}, 	// P_R_P3Q0
    {0x370C, 0x5D33, WORD_LEN, 0}, 	// P_R_P3Q1
    {0x370E, 0xC215, WORD_LEN, 0}, 	// P_R_P3Q2
    {0x3710, 0x86D6, WORD_LEN, 0}, 	// P_R_P3Q3
    {0x3712, 0x3F57, WORD_LEN, 0}, 	// P_R_P3Q4
    {0x3714, 0x88EF, WORD_LEN, 0}, 	// P_B_P3Q0
    {0x3716, 0x14F3, WORD_LEN, 0}, 	// P_B_P3Q1
    {0x3718, 0xD8F4, WORD_LEN, 0}, 	// P_B_P3Q2
    {0x371A, 0xBAB5, WORD_LEN, 0}, 	// P_B_P3Q3
    {0x371C, 0x7216, WORD_LEN, 0}, 	// P_B_P3Q4
    {0x371E, 0xFEEF, WORD_LEN, 0}, 	// P_G2_P3Q0
    {0x3720, 0x3093, WORD_LEN, 0}, 	// P_G2_P3Q1
    {0x3722, 0xF094, WORD_LEN, 0}, 	// P_G2_P3Q2
    {0x3724, 0xE8D5, WORD_LEN, 0}, 	// P_G2_P3Q3
    {0x3726, 0x0ED7, WORD_LEN, 0}, 	// P_G2_P3Q4
    {0x3740, 0xA3F4, WORD_LEN, 0}, 	// P_G1_P4Q0
    {0x3742, 0x4F94, WORD_LEN, 0}, 	// P_G1_P4Q1
    {0x3744, 0x7D54, WORD_LEN, 0}, 	// P_G1_P4Q2
    {0x3746, 0xB8D5, WORD_LEN, 0}, 	// P_G1_P4Q3
    {0x3748, 0x3717, WORD_LEN, 0}, 	// P_G1_P4Q4
    {0x374A, 0xF5D3, WORD_LEN, 0}, 	// P_R_P4Q0
    {0x374C, 0x0DF5, WORD_LEN, 0}, 	// P_R_P4Q1
    {0x374E, 0xB555, WORD_LEN, 0}, 	// P_R_P4Q2
    {0x3750, 0xC656, WORD_LEN, 0}, 	// P_R_P4Q3
    {0x3752, 0x5EB8, WORD_LEN, 0}, 	// P_R_P4Q4
    {0x3754, 0x8394, WORD_LEN, 0}, 	// P_B_P4Q0
    {0x3756, 0x2534, WORD_LEN, 0}, 	// P_B_P4Q1
    {0x3758, 0x5BB5, WORD_LEN, 0}, 	// P_B_P4Q2
    {0x375A, 0x9D34, WORD_LEN, 0}, 	// P_B_P4Q3
    {0x375C, 0x61F5, WORD_LEN, 0}, 	// P_B_P4Q4
    {0x375E, 0xA014, WORD_LEN, 0}, 	// P_G2_P4Q0
    {0x3760, 0x3094, WORD_LEN, 0}, 	// P_G2_P4Q1
    {0x3762, 0x2195, WORD_LEN, 0}, 	// P_G2_P4Q2
    {0x3764, 0xC7D4, WORD_LEN, 0}, 	// P_G2_P4Q3
    {0x3766, 0x0437, WORD_LEN, 0}, 	// P_G2_P4Q4
    {0x3782, 0x0320, WORD_LEN, 0}, 	// CENTER_ROW
    {0x3784, 0x0380, WORD_LEN, 0}, 	// CENTER_COLUMN
#endif    
};

static struct mt9t11x_i2c_reg_conf const lens_for_indoor_tbl[] ={
#if 0 //ZTE_ZT_CAM_20101216 disused
#if defined(CONFIG_MACH_BLADE) || defined (CONFIG_MACH_SMOOTH)    
    //[Lens Correction 10/08/00]
    {0x3640, 0x0130, WORD_LEN, 0},
    {0x3642, 0xB2E8, WORD_LEN, 0},
    {0x3644, 0x2F30, WORD_LEN, 0},
    {0x3646, 0x810E, WORD_LEN, 0},
    {0x3648, 0xA0D1, WORD_LEN, 0},
    {0x364A, 0x00F0, WORD_LEN, 0},
    {0x364C, 0x4AA8, WORD_LEN, 0},
    {0x364E, 0x2C90, WORD_LEN, 0},
    {0x3650, 0x870D, WORD_LEN, 0},
    {0x3652, 0xA991, WORD_LEN, 0},
    {0x3654, 0x01F0, WORD_LEN, 0},
    {0x3656, 0x544B, WORD_LEN, 0},
    {0x3658, 0x5CAF, WORD_LEN, 0},
    {0x365A, 0xD7ED, WORD_LEN, 0},
    {0x365C, 0x8B11, WORD_LEN, 0},
    {0x365E, 0x0170, WORD_LEN, 0},
    {0x3660, 0x844B, WORD_LEN, 0},
    {0x3662, 0x4150, WORD_LEN, 0},
    {0x3664, 0xEB0C, WORD_LEN, 0},
    {0x3666, 0xCE71, WORD_LEN, 0},
    {0x3680, 0x8D89, WORD_LEN, 0},
    {0x3682, 0x390D, WORD_LEN, 0},
    {0x3684, 0xF1AD, WORD_LEN, 0},
    {0x3686, 0x8DCF, WORD_LEN, 0},
    {0x3688, 0x2610, WORD_LEN, 0},
    {0x368A, 0x038C, WORD_LEN, 0},
    {0x368C, 0x452E, WORD_LEN, 0},
    {0x368E, 0x82AD, WORD_LEN, 0},
    {0x3690, 0xA610, WORD_LEN, 0},
    {0x3692, 0x306F, WORD_LEN, 0},
    {0x3694, 0x866E, WORD_LEN, 0},
    {0x3696, 0x6A6E, WORD_LEN, 0},
    {0x3698, 0xCE8E, WORD_LEN, 0},
    {0x369A, 0xCF30, WORD_LEN, 0},
    {0x369C, 0x1391, WORD_LEN, 0},
    {0x369E, 0xAC2E, WORD_LEN, 0},
    {0x36A0, 0x56CE, WORD_LEN, 0},
    {0x36A2, 0xEC4E, WORD_LEN, 0},
    {0x36A4, 0xAB90, WORD_LEN, 0},
    {0x36A6, 0x5E91, WORD_LEN, 0},
    {0x36C0, 0x0C92, WORD_LEN, 0},
    {0x36C2, 0xA3ED, WORD_LEN, 0},
    {0x36C4, 0xCDD2, WORD_LEN, 0},
    {0x36C6, 0x3A70, WORD_LEN, 0},
    {0x36C8, 0xC771, WORD_LEN, 0},
    {0x36CA, 0x7E11, WORD_LEN, 0},
    {0x36CC, 0x8ACD, WORD_LEN, 0},
    {0x36CE, 0xEEB1, WORD_LEN, 0},
    {0x36D0, 0x7C50, WORD_LEN, 0},
    {0x36D2, 0xA472, WORD_LEN, 0},
    {0x36D4, 0x7A51, WORD_LEN, 0},
    {0x36D6, 0xCDCA, WORD_LEN, 0},
    {0x36D8, 0x8433, WORD_LEN, 0},
    {0x36DA, 0xD44E, WORD_LEN, 0},
    {0x36DC, 0x5993, WORD_LEN, 0},
    {0x36DE, 0x14B2, WORD_LEN, 0},
    {0x36E0, 0xA7EE, WORD_LEN, 0},
    {0x36E2, 0x9093, WORD_LEN, 0},
    {0x36E4, 0x1D90, WORD_LEN, 0},
    {0x36E6, 0x0912, WORD_LEN, 0},
    {0x3700, 0x280D, WORD_LEN, 0},
    {0x3702, 0xDC10, WORD_LEN, 0},
    {0x3704, 0x59D2, WORD_LEN, 0},
    {0x3706, 0x1073, WORD_LEN, 0},
    {0x3708, 0xEDD4, WORD_LEN, 0},
    {0x370A, 0xAEED, WORD_LEN, 0},
    {0x370C, 0x8351, WORD_LEN, 0},
    {0x370E, 0x4C52, WORD_LEN, 0},
    {0x3710, 0x2DF3, WORD_LEN, 0},
    {0x3712, 0xBD34, WORD_LEN, 0},
    {0x3714, 0x902D, WORD_LEN, 0},
    {0x3716, 0xA9D1, WORD_LEN, 0},
    {0x3718, 0x7432, WORD_LEN, 0},
    {0x371A, 0x5533, WORD_LEN, 0},
    {0x371C, 0xE514, WORD_LEN, 0},
    {0x371E, 0xF129, WORD_LEN, 0},
    {0x3720, 0x8530, WORD_LEN, 0},
    {0x3722, 0x5533, WORD_LEN, 0},
    {0x3724, 0x6D72, WORD_LEN, 0},
    {0x3726, 0xD275, WORD_LEN, 0},
    {0x3740, 0xD572, WORD_LEN, 0},
    {0x3742, 0x5C91, WORD_LEN, 0},
    {0x3744, 0xD015, WORD_LEN, 0},
    {0x3746, 0xA675, WORD_LEN, 0},
    {0x3748, 0x3578, WORD_LEN, 0},
    {0x374A, 0x9A52, WORD_LEN, 0},
    {0x374C, 0x1171, WORD_LEN, 0},
    {0x374E, 0xBFB5, WORD_LEN, 0},
    {0x3750, 0x8BD5, WORD_LEN, 0},
    {0x3752, 0x0F38, WORD_LEN, 0},
    {0x3754, 0xCD92, WORD_LEN, 0},
    {0x3756, 0x5A10, WORD_LEN, 0},
    {0x3758, 0x90F4, WORD_LEN, 0},
    {0x375A, 0xE5B4, WORD_LEN, 0},
    {0x375C, 0x4C97, WORD_LEN, 0},
    {0x375E, 0xE852, WORD_LEN, 0},
    {0x3760, 0x5771, WORD_LEN, 0},
    {0x3762, 0xBA95, WORD_LEN, 0},
    {0x3764, 0x9855, WORD_LEN, 0},
    {0x3766, 0x2E58, WORD_LEN, 0},
    {0x3782, 0x02E0, WORD_LEN, 0},
    {0x3784, 0x03C0, WORD_LEN, 0},

    /*
      * The following setting will lead to
      * white line display during snapshot
      */
    //{0x3210, 0x01B8, WORD_LEN, 0}, 	// COLOR_PIPELINE_CONTROL
#elif defined(CONFIG_MACH_RAISE)
    //[Lens Correction 95% 05/12/10 16:06:47]
    {0x3640, 0x0230, WORD_LEN, 0}, 	// P_G1_P0Q0
    {0x3642, 0xADEB, WORD_LEN, 0}, 	// P_G1_P0Q1
    {0x3644, 0x4391, WORD_LEN, 0}, 	// P_G1_P0Q2
    {0x3646, 0xF28E, WORD_LEN, 0}, 	// P_G1_P0Q3
    {0x3648, 0xC391, WORD_LEN, 0}, 	// P_G1_P0Q4
    {0x364A, 0x0170, WORD_LEN, 0}, 	// P_R_P0Q0
    {0x364C, 0xA3AA, WORD_LEN, 0}, 	// P_R_P0Q1
    {0x364E, 0x3031, WORD_LEN, 0}, 	// P_R_P0Q2
    {0x3650, 0xF7EE, WORD_LEN, 0}, 	// P_R_P0Q3
    {0x3652, 0xEBF0, WORD_LEN, 0}, 	// P_R_P0Q4
    {0x3654, 0x02D0, WORD_LEN, 0}, 	// P_B_P0Q0
    {0x3656, 0x9B4B, WORD_LEN, 0}, 	// P_B_P0Q1
    {0x3658, 0x1291, WORD_LEN, 0}, 	// P_B_P0Q2
    {0x365A, 0xA2EE, WORD_LEN, 0}, 	// P_B_P0Q3
    {0x365C, 0x8291, WORD_LEN, 0}, 	// P_B_P0Q4
    {0x365E, 0x0130, WORD_LEN, 0}, 	// P_G2_P0Q0
    {0x3660, 0xF06A, WORD_LEN, 0}, 	// P_G2_P0Q1
    {0x3662, 0x4231, WORD_LEN, 0}, 	// P_G2_P0Q2
    {0x3664, 0x98AF, WORD_LEN, 0}, 	// P_G2_P0Q3
    {0x3666, 0xB7D1, WORD_LEN, 0}, 	// P_G2_P0Q4
    {0x3680, 0x3B8B, WORD_LEN, 0}, 	// P_G1_P1Q0
    {0x3682, 0xEECE, WORD_LEN, 0}, 	// P_G1_P1Q1
    {0x3684, 0xDE0F, WORD_LEN, 0}, 	// P_G1_P1Q2
    {0x3686, 0x25B0, WORD_LEN, 0}, 	// P_G1_P1Q3
    {0x3688, 0x34F0, WORD_LEN, 0}, 	// P_G1_P1Q4
    {0x368A, 0x132B, WORD_LEN, 0}, 	// P_R_P1Q0
    {0x368C, 0x860E, WORD_LEN, 0}, 	// P_R_P1Q1
    {0x368E, 0x154C, WORD_LEN, 0}, 	// P_R_P1Q2
    {0x3690, 0x248F, WORD_LEN, 0}, 	// P_R_P1Q3
    {0x3692, 0xE3F0, WORD_LEN, 0}, 	// P_R_P1Q4
    {0x3694, 0xAB4E, WORD_LEN, 0}, 	// P_B_P1Q0
    {0x3696, 0xBCED, WORD_LEN, 0}, 	// P_B_P1Q1
    {0x3698, 0x8570, WORD_LEN, 0}, 	// P_B_P1Q2
    {0x369A, 0x4C6E, WORD_LEN, 0}, 	// P_B_P1Q3
    {0x369C, 0x4B71, WORD_LEN, 0}, 	// P_B_P1Q4
    {0x369E, 0x9E0E, WORD_LEN, 0}, 	// P_G2_P1Q0
    {0x36A0, 0xF46C, WORD_LEN, 0}, 	// P_G2_P1Q1
    {0x36A2, 0xD950, WORD_LEN, 0}, 	// P_G2_P1Q2
    {0x36A4, 0x08EF, WORD_LEN, 0}, 	// P_G2_P1Q3
    {0x36A6, 0x0912, WORD_LEN, 0}, 	// P_G2_P1Q4
    {0x36C0, 0x3672, WORD_LEN, 0}, 	// P_G1_P2Q0
    {0x36C2, 0xD3EF, WORD_LEN, 0}, 	// P_G1_P2Q1
    {0x36C4, 0xD832, WORD_LEN, 0}, 	// P_G1_P2Q2
    {0x36C6, 0x27B1, WORD_LEN, 0}, 	// P_G1_P2Q3
    {0x36C8, 0x6A90, WORD_LEN, 0}, 	// P_G1_P2Q4
    {0x36CA, 0x16F2, WORD_LEN, 0}, 	// P_R_P2Q0
    {0x36CC, 0x3BAD, WORD_LEN, 0}, 	// P_R_P2Q1
    {0x36CE, 0x9690, WORD_LEN, 0}, 	// P_R_P2Q2
    {0x36D0, 0xCE30, WORD_LEN, 0}, 	// P_R_P2Q3
    {0x36D2, 0x8F13, WORD_LEN, 0}, 	// P_R_P2Q4
    {0x36D4, 0x1CB2, WORD_LEN, 0}, 	// P_B_P2Q0
    {0x36D6, 0x0050, WORD_LEN, 0}, 	// P_B_P2Q1
    {0x36D8, 0x8413, WORD_LEN, 0}, 	// P_B_P2Q2
    {0x36DA, 0xF692, WORD_LEN, 0}, 	// P_B_P2Q3
    {0x36DC, 0x1D74, WORD_LEN, 0}, 	// P_B_P2Q4
    {0x36DE, 0x4012, WORD_LEN, 0}, 	// P_G2_P2Q0
    {0x36E0, 0xFE2F, WORD_LEN, 0}, 	// P_G2_P2Q1
    {0x36E2, 0xE532, WORD_LEN, 0}, 	// P_G2_P2Q2
    {0x36E4, 0x4BF1, WORD_LEN, 0}, 	// P_G2_P2Q3
    {0x36E6, 0x9BF0, WORD_LEN, 0}, 	// P_G2_P2Q4
    {0x3700, 0xADAF, WORD_LEN, 0}, 	// P_G1_P3Q0
    {0x3702, 0x48AD, WORD_LEN, 0}, 	// P_G1_P3Q1
    {0x3704, 0x526F, WORD_LEN, 0}, 	// P_G1_P3Q2
    {0x3706, 0xC1F0, WORD_LEN, 0}, 	// P_G1_P3Q3
    {0x3708, 0xE0B0, WORD_LEN, 0}, 	// P_G1_P3Q4
    {0x370A, 0xE36D, WORD_LEN, 0}, 	// P_R_P3Q0
    {0x370C, 0x0950, WORD_LEN, 0}, 	// P_R_P3Q1
    {0x370E, 0x86D3, WORD_LEN, 0}, 	// P_R_P3Q2
    {0x3710, 0xB412, WORD_LEN, 0}, 	// P_R_P3Q3
    {0x3712, 0x0775, WORD_LEN, 0}, 	// P_R_P3Q4
    {0x3714, 0xA0AD, WORD_LEN, 0}, 	// P_B_P3Q0
    {0x3716, 0x720C, WORD_LEN, 0}, 	// P_B_P3Q1
    {0x3718, 0x91B0, WORD_LEN, 0}, 	// P_B_P3Q2
    {0x371A, 0x90F0, WORD_LEN, 0}, 	// P_B_P3Q3
    {0x371C, 0x9491, WORD_LEN, 0}, 	// P_B_P3Q4
    {0x371E, 0x8430, WORD_LEN, 0}, 	// P_G2_P3Q0
    {0x3720, 0x3190, WORD_LEN, 0}, 	// P_G2_P3Q1
    {0x3722, 0x5F52, WORD_LEN, 0}, 	// P_G2_P3Q2
    {0x3724, 0xCB72, WORD_LEN, 0}, 	// P_G2_P3Q3
    {0x3726, 0xB094, WORD_LEN, 0}, 	// P_G2_P3Q4
    {0x3740, 0x8C33, WORD_LEN, 0}, 	// P_G1_P4Q0
    {0x3742, 0x1C52, WORD_LEN, 0}, 	// P_G1_P4Q1
    {0x3744, 0xBA95, WORD_LEN, 0}, 	// P_G1_P4Q2
    {0x3746, 0xAF54, WORD_LEN, 0}, 	// P_G1_P4Q3
    {0x3748, 0x3FF8, WORD_LEN, 0}, 	// P_G1_P4Q4
    {0x374A, 0x8A92, WORD_LEN, 0}, 	// P_R_P4Q0
    {0x374C, 0x9FED, WORD_LEN, 0}, 	// P_R_P4Q1
    {0x374E, 0xE2B5, WORD_LEN, 0}, 	// P_R_P4Q2
    {0x3750, 0x3052, WORD_LEN, 0}, 	// P_R_P4Q3
    {0x3752, 0x1D78, WORD_LEN, 0}, 	// P_R_P4Q4
    {0x3754, 0xF5F2, WORD_LEN, 0}, 	// P_B_P4Q0
    {0x3756, 0xA672, WORD_LEN, 0}, 	// P_B_P4Q1
    {0x3758, 0x98B3, WORD_LEN, 0}, 	// P_B_P4Q2
    {0x375A, 0x0F35, WORD_LEN, 0}, 	// P_B_P4Q3
    {0x375C, 0x5077, WORD_LEN, 0}, 	// P_B_P4Q4
    {0x375E, 0x9333, WORD_LEN, 0}, 	// P_G2_P4Q0
    {0x3760, 0x3012, WORD_LEN, 0}, 	// P_G2_P4Q1
    {0x3762, 0xBC55, WORD_LEN, 0}, 	// P_G2_P4Q2
    {0x3764, 0xD174, WORD_LEN, 0}, 	// P_G2_P4Q3
    {0x3766, 0x5238, WORD_LEN, 0}, 	// P_G2_P4Q4
    {0x3782, 0x0324, WORD_LEN, 0}, 	// CENTER_ROW
    {0x3784, 0x03E0, WORD_LEN, 0}, 	// CENTER_COLUMN
#else
    //[Lens Correction 10/08/00]
    {0x3640, 0x0130, WORD_LEN, 0},
    {0x3642, 0xB2E8, WORD_LEN, 0},
    {0x3644, 0x2F30, WORD_LEN, 0},
    {0x3646, 0x810E, WORD_LEN, 0},
    {0x3648, 0xA0D1, WORD_LEN, 0},
    {0x364A, 0x00F0, WORD_LEN, 0},
    {0x364C, 0x4AA8, WORD_LEN, 0},
    {0x364E, 0x2C90, WORD_LEN, 0},
    {0x3650, 0x870D, WORD_LEN, 0},
    {0x3652, 0xA991, WORD_LEN, 0},
    {0x3654, 0x01F0, WORD_LEN, 0},
    {0x3656, 0x544B, WORD_LEN, 0},
    {0x3658, 0x5CAF, WORD_LEN, 0},
    {0x365A, 0xD7ED, WORD_LEN, 0},
    {0x365C, 0x8B11, WORD_LEN, 0},
    {0x365E, 0x0170, WORD_LEN, 0},
    {0x3660, 0x844B, WORD_LEN, 0},
    {0x3662, 0x4150, WORD_LEN, 0},
    {0x3664, 0xEB0C, WORD_LEN, 0},
    {0x3666, 0xCE71, WORD_LEN, 0},
    {0x3680, 0x8D89, WORD_LEN, 0},
    {0x3682, 0x390D, WORD_LEN, 0},
    {0x3684, 0xF1AD, WORD_LEN, 0},
    {0x3686, 0x8DCF, WORD_LEN, 0},
    {0x3688, 0x2610, WORD_LEN, 0},
    {0x368A, 0x038C, WORD_LEN, 0},
    {0x368C, 0x452E, WORD_LEN, 0},
    {0x368E, 0x82AD, WORD_LEN, 0},
    {0x3690, 0xA610, WORD_LEN, 0},
    {0x3692, 0x306F, WORD_LEN, 0},
    {0x3694, 0x866E, WORD_LEN, 0},
    {0x3696, 0x6A6E, WORD_LEN, 0},
    {0x3698, 0xCE8E, WORD_LEN, 0},
    {0x369A, 0xCF30, WORD_LEN, 0},
    {0x369C, 0x1391, WORD_LEN, 0},
    {0x369E, 0xAC2E, WORD_LEN, 0},
    {0x36A0, 0x56CE, WORD_LEN, 0},
    {0x36A2, 0xEC4E, WORD_LEN, 0},
    {0x36A4, 0xAB90, WORD_LEN, 0},
    {0x36A6, 0x5E91, WORD_LEN, 0},
    {0x36C0, 0x0C92, WORD_LEN, 0},
    {0x36C2, 0xA3ED, WORD_LEN, 0},
    {0x36C4, 0xCDD2, WORD_LEN, 0},
    {0x36C6, 0x3A70, WORD_LEN, 0},
    {0x36C8, 0xC771, WORD_LEN, 0},
    {0x36CA, 0x7E11, WORD_LEN, 0},
    {0x36CC, 0x8ACD, WORD_LEN, 0},
    {0x36CE, 0xEEB1, WORD_LEN, 0},
    {0x36D0, 0x7C50, WORD_LEN, 0},
    {0x36D2, 0xA472, WORD_LEN, 0},
    {0x36D4, 0x7A51, WORD_LEN, 0},
    {0x36D6, 0xCDCA, WORD_LEN, 0},
    {0x36D8, 0x8433, WORD_LEN, 0},
    {0x36DA, 0xD44E, WORD_LEN, 0},
    {0x36DC, 0x5993, WORD_LEN, 0},
    {0x36DE, 0x14B2, WORD_LEN, 0},
    {0x36E0, 0xA7EE, WORD_LEN, 0},
    {0x36E2, 0x9093, WORD_LEN, 0},
    {0x36E4, 0x1D90, WORD_LEN, 0},
    {0x36E6, 0x0912, WORD_LEN, 0},
    {0x3700, 0x280D, WORD_LEN, 0},
    {0x3702, 0xDC10, WORD_LEN, 0},
    {0x3704, 0x59D2, WORD_LEN, 0},
    {0x3706, 0x1073, WORD_LEN, 0},
    {0x3708, 0xEDD4, WORD_LEN, 0},
    {0x370A, 0xAEED, WORD_LEN, 0},
    {0x370C, 0x8351, WORD_LEN, 0},
    {0x370E, 0x4C52, WORD_LEN, 0},
    {0x3710, 0x2DF3, WORD_LEN, 0},
    {0x3712, 0xBD34, WORD_LEN, 0},
    {0x3714, 0x902D, WORD_LEN, 0},
    {0x3716, 0xA9D1, WORD_LEN, 0},
    {0x3718, 0x7432, WORD_LEN, 0},
    {0x371A, 0x5533, WORD_LEN, 0},
    {0x371C, 0xE514, WORD_LEN, 0},
    {0x371E, 0xF129, WORD_LEN, 0},
    {0x3720, 0x8530, WORD_LEN, 0},
    {0x3722, 0x5533, WORD_LEN, 0},
    {0x3724, 0x6D72, WORD_LEN, 0},
    {0x3726, 0xD275, WORD_LEN, 0},
    {0x3740, 0xD572, WORD_LEN, 0},
    {0x3742, 0x5C91, WORD_LEN, 0},
    {0x3744, 0xD015, WORD_LEN, 0},
    {0x3746, 0xA675, WORD_LEN, 0},
    {0x3748, 0x3578, WORD_LEN, 0},
    {0x374A, 0x9A52, WORD_LEN, 0},
    {0x374C, 0x1171, WORD_LEN, 0},
    {0x374E, 0xBFB5, WORD_LEN, 0},
    {0x3750, 0x8BD5, WORD_LEN, 0},
    {0x3752, 0x0F38, WORD_LEN, 0},
    {0x3754, 0xCD92, WORD_LEN, 0},
    {0x3756, 0x5A10, WORD_LEN, 0},
    {0x3758, 0x90F4, WORD_LEN, 0},
    {0x375A, 0xE5B4, WORD_LEN, 0},
    {0x375C, 0x4C97, WORD_LEN, 0},
    {0x375E, 0xE852, WORD_LEN, 0},
    {0x3760, 0x5771, WORD_LEN, 0},
    {0x3762, 0xBA95, WORD_LEN, 0},
    {0x3764, 0x9855, WORD_LEN, 0},
    {0x3766, 0x2E58, WORD_LEN, 0},
    {0x3782, 0x02E0, WORD_LEN, 0},
    {0x3784, 0x03C0, WORD_LEN, 0},

    /*
      * The following setting will lead to
      * white line display during snapshot
      */
    //{0x3210, 0x01B8, WORD_LEN, 0}, 	// COLOR_PIPELINE_CONTROL
#endif
#endif
};

struct mt9t11x_reg_t mt9t112_regs = {
    .pll_tbl                = pll_tbl,
    .pll_tbl_sz             = ARRAY_SIZE(pll_tbl),

    .clk_tbl                = clk_tbl,
    .clk_tbl_sz             = ARRAY_SIZE(clk_tbl),

    .prevsnap_tbl           = prev_snap_tbl,
    .prevsnap_tbl_sz        = ARRAY_SIZE(prev_snap_tbl),

    .wb_cloudy_tbl          = wb_cloudy_tbl,
    .wb_cloudy_tbl_sz       = ARRAY_SIZE(wb_cloudy_tbl),

    .wb_daylight_tbl        = wb_daylight_tbl,
    .wb_daylight_tbl_sz     = ARRAY_SIZE(wb_daylight_tbl),

    .wb_flourescant_tbl     = wb_flourescant_tbl,
    .wb_flourescant_tbl_sz  = ARRAY_SIZE(wb_flourescant_tbl),

    .wb_incandescent_tbl    = wb_incandescent_tbl,
    .wb_incandescent_tbl_sz = ARRAY_SIZE(wb_incandescent_tbl),

    .wb_auto_tbl            = wb_auto_tbl,
    .wb_auto_tbl_sz         = ARRAY_SIZE(wb_auto_tbl),

    .af_tbl                 = af_tbl,
    .af_tbl_sz              = ARRAY_SIZE(af_tbl),

    .contrast_tbl           = contrast_tbl,
    .contrast_tbl_sz        = contrast_tbl_sz,

    .brightness_tbl         = brightness_tbl,
    .brightness_tbl_sz      = brightness_tbl_sz,

    .saturation_tbl         = saturation_tbl,
    .saturation_tbl_sz      = saturation_tbl_sz,

    .sharpness_tbl          = sharpness_tbl,
    .sharpness_tbl_sz       = sharpness_tbl_sz,

    .exposure_tbl           = exposure_tbl,
    .exposure_tbl_sz        = exposure_tbl_sz,

    .lens_for_outdoor_tbl   = lens_for_outdoor_tbl,
    .lens_for_outdoor_tbl_sz= 0,    //ARRAY_SIZE(lens_for_outdoor_tbl),

    .lens_for_indoor_tbl    = lens_for_indoor_tbl,
    .lens_for_indoor_tbl_sz = 0,    //ARRAY_SIZE(lens_for_indoor_tbl),
};

