/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1386_C16A_S1C33240_VGAMON.h
Purpose     : Sample configuration file for EPSON 13806 LCD controller
              connected to VGA monitor
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE      (640)      /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (480)      /* Y-resolution of LCD, Logical coor. */

#define LCD_CONTROLLER 1386

#define LCD_SWAP_BYTE_ORDER (1)
#define LCD_BITSPERPIXEL    (16)
#define LCD_SWAP_RB         (1)

/*********************************************************************
*
*                   Full bus configuration
*
**********************************************************************
*/

#define LCD_READ_MEM(Off)            *((U16 *)(0xa00000+(((U32)(Off))<<1)))
#define LCD_WRITE_MEM(Off,data)      *((U16 *)(0xa00000+(((U32)(Off))<<1)))=data
#define LCD_READ_REG(Off)            *((volatile U16*)(0x800000+(((U32)(Off))<<1)))
#define LCD_WRITE_REG(Off,data)      *((volatile U16*)(0x800000+(((U32)(Off))<<1)))=data

/*********************************************************************
*
*                   Init sequence for 16 bit access
*
**********************************************************************
*/

#define REG_CRTTV_HDP              (((LCD_XSIZE + 7) >> 3) - 1)
#define REG_CRTTV_MEM_ADDR_OFFSET0 ((((LCD_XSIZE + 15) >> 4) * LCD_BITSPERPIXEL) & 0xff)
#define REG_CRTTV_MEM_ADDR_OFFSET1 ((((LCD_XSIZE + 15) >> 4) * LCD_BITSPERPIXEL) >> 8)
#define REG_CRTTV_MEM_ADDR_OFFSET  ((REG_CRTTV_MEM_ADDR_OFFSET1 << 8) + REG_CRTTV_MEM_ADDR_OFFSET0)

#if   (LCD_BITSPERPIXEL == 4)
  #define REG_CRTTV_DISPLAY_MODE 0x02
#elif (LCD_BITSPERPIXEL == 8)
  #define REG_CRTTV_DISPLAY_MODE 0x03
#elif (LCD_BITSPERPIXEL == 16)
  #define REG_CRTTV_DISPLAY_MODE 0x05
#endif

#define  LCD_INIT_CONTROLLER() \
  LCD_WRITE_REG(0x0000/2, 0x0000);                        /* REG_MISC */ \
  LCD_WRITE_REG(0x001c/2, 0x0002);                        /* REG_MPCLK_CFG */ \
  LCD_WRITE_REG(0x0020/2, 0x0380);                        /* REG_MEM_CFG, REG_DRAM_REFRESH */ \
  LCD_WRITE_REG(0x002a/2, 0x0100);                        /* REG_DRAM_TIMINGS_CTRL0, REG_DRAM_TIMINGS_CTRL1 */ \
  LCD_WRITE_REG(0x01fc/2, 0x0000);                        /* REG_DISPLAY_MODE */ \
  LCD_WRITE_REG(0x0004/2, 0x0000);                        /* REG_GPIO_CONFIG0, REG_GPIO_CONFIG1 */ \
  LCD_WRITE_REG(0x0008/2, 0x0000);                        /* REG_GPIO_CTRL0, REG_GPIO_CTRL1 */ \
  LCD_WRITE_REG(0x0010/2, 0x0002);                        /* REG_MEM_CLOCK_CFG */ \
  LCD_WRITE_REG(0x0014/2, 0x0000);                        /* REG_LCD_PCLK_CFG */ \
  LCD_WRITE_REG(0x0018/2, 0x0000);                        /* REG_CRTTV_PCLK_CFG */ \
  LCD_WRITE_REG(0x001e/2, 0x0000);                        /* REG_CPU2MEM_WAIT_SEL */ \
  LCD_WRITE_REG(0x0020/2, 0x0380);                        /* REG_MEM_CFG, REG_DRAM_REFRESH */ \
  LCD_WRITE_REG(0x002a/2, 0x0100);                        /* REG_DRAM_TIMINGS_CTRL0, REG_DRAM_TIMINGS_CTRL1 */ \
  LCD_WRITE_REG(0x0050/2, REG_CRTTV_HDP);                 /* REG_CRTTV_HDP */ \
  LCD_WRITE_REG(0x0052/2, 0x0113);                        /* REG_CRTTV_HNDP, REG_CRTTV_HRTC_START */ \
  LCD_WRITE_REG(0x0054/2, 0x000b);                        /* REG_CRT_HRTC_PULSE */ \
  LCD_WRITE_REG(0x0056/2, 0x01df);                        /* REG_CRTTV_VDP0, REG_CRTTV_VDP1 */ \
  LCD_WRITE_REG(0x0058/2, 0x092b);                        /* REG_CRTTV_VNDP, REG_CRTTV_VRTC_START */ \
  LCD_WRITE_REG(0x005a/2, 0x1001);                        /* REG_CRT_VRTC_PULSE, REG_TV_OUTPUT_CTRL */ \
  LCD_WRITE_REG(0x0060/2, REG_CRTTV_DISPLAY_MODE);        /* REG_CRTTV_DISPLAY_MODE */ \
  LCD_WRITE_REG(0x0062/2, 0x0000);                        /* REG_CRTTV_START_ADDR0, REG_CRTTV_START_ADDR1 */ \
  LCD_WRITE_REG(0x0064/2, 0x0000);                        /* REG_CRTTV_START_ADDR2 */ \
  LCD_WRITE_REG(0x0066/2, REG_CRTTV_MEM_ADDR_OFFSET);     /* REG_CRTTV_MEM_ADDR_OFFSET0, REG_CRTTV_MEM_ADDR_OFFSET1 */ \
  LCD_WRITE_REG(0x0068/2, 0x0000);                        /* REG_CRTTV_PIXEL_PANNING */ \
  LCD_WRITE_REG(0x006a/2, 0x0000);                        /* REG_CRTTV_FIFO_HIGH_THRESHOLD, REG_CRTTV_FIFO_LOW_THRESHOLD */ \
  LCD_WRITE_REG(0x01f0/2, 0x0010);                        /* REG_PWR_SAVE_CFG, REG_PWR_SAVE_STATUS */ \
  LCD_WRITE_REG(0x01fc/2, 0x0002);                        /* REG_DISPLAY_MODE */

/*********************************************************************
*
*                   LCD_ON / LCD_OFF
*
**********************************************************************
*/

#define LCD_OFF() LCD_WRITE_REG(0x01fc, 0x0000)
#define LCD_ON()  LCD_WRITE_REG(0x01fc, 0x0002)

#endif /* LCDCONF_H */

