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
File        : LCDConf_1376_C8_C320x240.h
Purpose     : Internal sample configuration file
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

#define LCD_XSIZE      320      /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      240      /* Y-resolution of LCD, Logical coor. */

#define LCD_CONTROLLER 1376

#define LCD_BITSPERPIXEL    8
#define LCD_SWAP_BYTE_ORDER 1

/*********************************************************************
*
*       Full bus configuration
*
**********************************************************************
*/

#define LCD_READ_MEM(Off)       *((U16*)         (0x420000+(((U32)(Off))<<1)))
#define LCD_WRITE_MEM(Off,data) *((U16*)         (0x420000+(((U32)(Off))<<1)))=data
#define LCD_READ_REG(Off)       *((volatile U16*)(0x620000+(((U16)(Off))<<1)))
#define LCD_WRITE_REG(Off,data) *((volatile U16*)(0x620000+(((U16)(Off))<<1)))=data

/*********************************************************************
*
*       Define register contents
*
**********************************************************************
*/

#define LCD_REG04 0x00 // BUSCLK MEMCLK Config Register
#define LCD_REG05 0x43 // PCLK Config  Register
#define LCD_REG10 0xD0 // PANEL Type Register
#define LCD_REG11 0x00 // MOD Rate Register
#define LCD_REG12 0x2B // Horizontal Total Register
#define LCD_REG14 ((LCD_XSIZE / 8) - 1) // Horizontal Display Period Register
#define LCD_REG16 0x00 // Horizontal Display Period Start Pos Register 0
#define LCD_REG17 0x00 // Horizontal Display Period Start Pos Register 1
#define LCD_REG18 0xFA // Vertical Total Register 0
#define LCD_REG19 0x00 // Vertical Total Register 1
#define LCD_REG1C (LCD_YSIZE - 1) // Vertical Display Period Register 0
#define LCD_REG1D 0x00 // Vertical Display Period Register 1
#define LCD_REG1E 0x00 // Vertical Display Period Start Pos Register 0
#define LCD_REG1F 0x00 // Vertical Display Period Start Pos Register 1
#define LCD_REG20 0x87 // Horizontal Sync Pulse Width Register
#define LCD_REG22 0x00 // Horizontal Sync Pulse Start Pos Register 0
#define LCD_REG23 0x00 // Horizontal Sync Pulse Start Pos Register 1
#define LCD_REG24 0x80 // Vertical Sync Pulse Width Register
#define LCD_REG26 0x01 // Vertical Sync Pulse Start Pos Register 0
#define LCD_REG27 0x00 // Vertical Sync Pulse Start Pos Register 1
#define LCD_REG70 0x03 // Display Mode Register
#define LCD_REG71 0x00 // Special Effects Register

/*********************************************************************
*
*       Init sequence for 16 bit access (LCD)
*
**********************************************************************
*/

#if !LCD_SWAP_BYTE_ORDER
  #define LCD_WRITE_REGLH(Adr, d0, d1) LCD_WRITE_REG(Adr, ((d0)<<8) | (d1))
#else
  #define LCD_WRITE_REGLH(Adr, d0, d1) LCD_WRITE_REG(Adr, ((d1)<<8) | (d0))
#endif  

#define LCD_INIT_CONTROLLER() \
  LCD_WRITE_REGLH(0x04 >> 1, LCD_REG04, LCD_REG05); \
  LCD_WRITE_REGLH(0x10 >> 1, LCD_REG10, LCD_REG11); \
  LCD_WRITE_REGLH(0x12 >> 1, LCD_REG12, 0); \
  LCD_WRITE_REGLH(0x14 >> 1, LCD_REG14, 0); \
  LCD_WRITE_REGLH(0x16 >> 1, LCD_REG16, LCD_REG17); \
  LCD_WRITE_REGLH(0x18 >> 1, LCD_REG18, LCD_REG19); \
  LCD_WRITE_REGLH(0x1C >> 1, LCD_REG1C, LCD_REG1D); \
  LCD_WRITE_REGLH(0x1E >> 1, LCD_REG1E, LCD_REG1F); \
  LCD_WRITE_REGLH(0x20 >> 1, LCD_REG20, 0); \
  LCD_WRITE_REGLH(0x22 >> 1, LCD_REG22, LCD_REG23); \
  LCD_WRITE_REGLH(0x24 >> 1, LCD_REG24, 0); \
  LCD_WRITE_REGLH(0x26 >> 1, LCD_REG26, LCD_REG27); \
  LCD_WRITE_REGLH(0x70 >> 1, LCD_REG70, LCD_REG71); \

#endif /* LCDCONF_H */
