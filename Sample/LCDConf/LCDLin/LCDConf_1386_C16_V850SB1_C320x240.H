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
File        : LCDConf_1386_C16_V850SB1_C320x240.h
Purpose     : Sample configuration file for Quarter VGA TFT
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

#define LCD_XSIZE      (320)      /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (240)      /* Y-resolution of LCD, Logical coor. */

#define LCD_CONTROLLER 1386

#define LCD_SWAP_BYTE_ORDER (0)
#define LCD_BITSPERPIXEL    (16)
#define LCD_SWAP_RB         (1)
#define LCD_USE_BITBLT      (1)

/*********************************************************************
*
*                   Full bus configuration
*
**********************************************************************
*/

#define LCD_READ_MEM(Off)            *((U16 *)        (0x600000+(((U32)(Off))<<1)))
#define LCD_WRITE_MEM(Off,Data)      *((U16 *)        (0x600000+(((U32)(Off))<<1)))=Data
#define LCD_READ_REG(Off)            *((volatile U16*)(0x400000+(((U32)(Off))<<1)))
#define LCD_WRITE_REG(Off,Data)      *((volatile U16*)(0x400000+(((U32)(Off))<<1)))=Data

/*********************************************************************
*
*                   Define contents of registers
*
**********************************************************************
*/

#define LCD_REG001 (0x00) // Miscellaneous Register
#define LCD_REGXXX (0x00) // Display Mode Register
#define LCD_REG004 (0x04) // General IO Pins Configuration Register 0
#define LCD_REG005 (0x00) // General IO Pins Configuration Register 1
#define LCD_REG008 (0x04) // General IO Pins Control Register 0
#define LCD_REG009 (0x00) // General IO Pins Control Register 1
#define LCD_REG010 (0x01) // Memory Clock Configuration Register
#define LCD_REG014 (0x11) // LCD Pixel Clock Configuration Register
#define LCD_REG018 (0x00) // CRT/TV Pixel Clock Configuration Register
#define LCD_REG01C (0x01) // MediaPlug Clock Configuration Register
#define LCD_REG01E (0x00) // CPU To Memory Wait State Select Register
#define LCD_REG020 (0x80) // Memory Configuration Register
#define LCD_REG021 (0x01) // DRAM Refresh Rate Register
#define LCD_REG02A (0x11) // DRAM Timings Control Register 0
#define LCD_REG02B (0x13) // DRAM Timings Control Register 1
#define LCD_REG030 (0x1c) // Panel Type Register
#define LCD_REG031 (0x00) // MOD Rate Register
#define LCD_REG032 (LCD_XSIZE / 8 - 1) // LCD Horizontal Display Width Register
#define LCD_REG034 (0x03) // LCD Horizontal Non-Display Period Register
#define LCD_REG035 (0x00) // TFT FPLINE Start Position Register
#define LCD_REG036 (0x0F) // TFT FPLINE Pulse Width Register
#define LCD_REG038 ((LCD_YSIZE - 1) % 256) // LCD Vertical Display Height Register 0
#define LCD_REG039 ((LCD_YSIZE - 1) / 256) // LCD Vertical Display Height Register 1
#define LCD_REG03A (0x00) // LCD Vertical Non-Display Period Register
#define LCD_REG03B (0x00) // TFT FPFRAME Start Position Register
#define LCD_REG03C (0x00) // TFT FPFRAME Pulse Width Register
#define LCD_REG040 (0x05) // LCD Display Mode Register
#define LCD_REG041 (0x00) // LCD Miscellaneous Register
#define LCD_REG042 (0x00) // LCD Display Start Address Register 0
#define LCD_REG043 (0x00) // LCD Display Start Address Register 1
#define LCD_REG044 (0x00) // LCD Display Start Address Register 2
#define LCD_REG046 ((LCD_XSIZE / (16 / LCD_BITSPERPIXEL)) % 256) // LCD Memory Address Offset Register 0
#define LCD_REG047 ((LCD_XSIZE / (16 / LCD_BITSPERPIXEL)) / 256) // LCD Memory Address Offset Register 1
#define LCD_REG048 (0x00) // LCD Pixel Panning Register
#define LCD_REG04A (0x00) // LCD Display FIFO High Threshold Control Register
#define LCD_REG04B (0x00) // LCD Display FIFO Low Threshold Control Register
#define LCD_REG050 (0x4F) // CRT/TV Horizontal Display Width Register
#define LCD_REG052 (0x13) // CRT/TV Horizontal Non-Display Period Register
#define LCD_REG053 (0x01) // CRT/TV HRTC Start Position Register
#define LCD_REG054 (0x0B) // CRT/TV HRTC Pulse Width Register
#define LCD_REG056 (0xDF) // CRT/TV Vertical Display Height Register 0
#define LCD_REG057 (0x01) // CRT/TV Vertical Display Height Register 1
#define LCD_REG058 (0x2B) // CRT/TV Vertical Non-Display Period Register
#define LCD_REG059 (0x09) // CRT/TV VRTC Start Position Register
#define LCD_REG05A (0x01) // CRT/TV VRTC Pulse Width Register
#define LCD_REG05B (0x10) // TV Output Control Register
#define LCD_REG060 (0x03) // CRT/TV Display Mode Register
#define LCD_REG062 (0x00) // CRT/TV Display Start Address Register 0
#define LCD_REG063 (0x00) // CRT/TV Display Start Address Register 1
#define LCD_REG064 (0x00) // CRT/TV Display Start Address Register 2
#define LCD_REG066 (0x40) // CRT/TV Memory Address Offset Register 0
#define LCD_REG067 (0x01) // CRT/TV Memory Address Offset Register 1
#define LCD_REG068 (0x00) // CRT/TV Pixel Panning Register
#define LCD_REG06A (0x00) // CRT/TV Display FIFO High Threshold Control Register
#define LCD_REG06B (0x00) // CRT/TV Display FIFO Low Threshold Control Register
#define LCD_REG070 (0x00) // LCD Ink/Cursor Control Register
#define LCD_REG071 (0x01) // LCD Ink/Cursor Start Address Register
#define LCD_REG072 (0x00) // LCD Cursor X Position Register 0
#define LCD_REG073 (0x00) // LCD Cursor X Position Register 1
#define LCD_REG074 (0x00) // LCD Cursor Y Position Register 0
#define LCD_REG075 (0x00) // LCD Cursor Y Position Register 1
#define LCD_REG076 (0x00) // LCD Ink/Cursor Blue Color 0 Register
#define LCD_REG077 (0x00) // LCD Ink/Cursor Green Color 0 Register
#define LCD_REG078 (0x00) // LCD Ink/Cursor Red Color 0 Register
#define LCD_REG07A (0x1F) // LCD Ink/Cursor Blue Color 1 Register
#define LCD_REG07B (0x3F) // LCD Ink/Cursor Green Color 1 Register
#define LCD_REG07C (0x1F) // LCD Ink/Cursor Red Color 1 Register
#define LCD_REG07E (0x00) // LCD Ink/Cursor FIFO Threshold Register
#define LCD_REG080 (0x00) // CRT/TV Ink/Cursor Control Register
#define LCD_REG081 (0x01) // CRT/TV Ink/Cursor Start Address Register
#define LCD_REG082 (0x00) // CRT/TV Cursor X Position Register 0
#define LCD_REG083 (0x00) // CRT/TV Cursor X Position Register 1
#define LCD_REG084 (0x00) // CRT/TV Cursor Y Position Register 0
#define LCD_REG085 (0x00) // CRT/TV Cursor Y Position Register 1
#define LCD_REG086 (0x00) // CRT/TV Ink/Cursor Blue Color 0 Register
#define LCD_REG087 (0x00) // CRT/TV Ink/Cursor Green Color 0 Register
#define LCD_REG088 (0x00) // CRT/TV Ink/Cursor Red Color 0 Register
#define LCD_REG08A (0x1F) // CRT/TV Ink/Cursor Blue Color 1 Register
#define LCD_REG08B (0x3F) // CRT/TV Ink/Cursor Green Color 1 Register
#define LCD_REG08C (0x1F) // CRT/TV Ink/Cursor Red Color 1 Register
#define LCD_REG08E (0x00) // CRT/TV Ink/Cursor FIFO Threshold Register
#define LCD_REG1F0 (0x10) // Power Save Configuration Register
#define LCD_REG1F1 (0x00) // Power Save Status Register
#define LCD_REG1F4 (0x00) // CPU-to-Memory Access Watchdog Timer Register
#define LCD_REG1FC (0x01) // Display Mode Register

/*********************************************************************
*
*                   Init sequence for 16 bit access (LCD)
*
**********************************************************************
*/

#if !LCD_SWAP_BYTE_ORDER
  #define LCD_WRITE_REGLH(Adr, d0, d1) LCD_WRITE_REG(Adr, ((d0) << 8) | (d1))
#else
  #define LCD_WRITE_REGLH(Adr, d0, d1) LCD_WRITE_REG(Adr, ((d1) << 8) | (d0))
#endif  


#define LCD_INIT_CONTROLLER()                          \
  LCD_WRITE_REGLH(0x000 >> 1,          0, LCD_REG001); \
  LCD_WRITE_REGLH(0x1FC >> 1,          0,          0); \
  LCD_WRITE_REGLH(0x004 >> 1, LCD_REG004, LCD_REG005); \
  LCD_WRITE_REGLH(0x008 >> 1, LCD_REG008, LCD_REG009); \
  LCD_WRITE_REGLH(0x010 >> 1, LCD_REG010,          0); \
  LCD_WRITE_REGLH(0x014 >> 1, LCD_REG014,          0); \
  LCD_WRITE_REGLH(0x01E >> 1, LCD_REG01E,          0); \
  LCD_WRITE_REGLH(0x020 >> 1, LCD_REG020, LCD_REG021); \
  LCD_WRITE_REGLH(0x02A >> 1, LCD_REG02A, LCD_REG02B); \
  LCD_WRITE_REGLH(0x030 >> 1, LCD_REG030, LCD_REG031); \
  LCD_WRITE_REGLH(0x032 >> 1, LCD_REG032,          0); \
  LCD_WRITE_REGLH(0x034 >> 1, LCD_REG034, LCD_REG035); \
  LCD_WRITE_REGLH(0x036 >> 1, LCD_REG036,          0); \
  LCD_WRITE_REGLH(0x038 >> 1, LCD_REG038, LCD_REG039); \
  LCD_WRITE_REGLH(0x03A >> 1, LCD_REG03A, LCD_REG03B); \
  LCD_WRITE_REGLH(0x03C >> 1, LCD_REG03C,          0); \
  LCD_WRITE_REGLH(0x040 >> 1, LCD_REG040, LCD_REG041); \
  LCD_WRITE_REGLH(0x042 >> 1, LCD_REG042, LCD_REG043); \
  LCD_WRITE_REGLH(0x044 >> 1, LCD_REG044,          0); \
  LCD_WRITE_REGLH(0x046 >> 1, LCD_REG046, LCD_REG047); \
  LCD_WRITE_REGLH(0x048 >> 1, LCD_REG048,          0); \
  LCD_WRITE_REGLH(0x04A >> 1, LCD_REG04A, LCD_REG04B); \
  LCD_WRITE_REGLH(0x1F0 >> 1, LCD_REG1F0, LCD_REG1F1); \
  LCD_WRITE_REGLH(0x1FC >> 1, LCD_REG1FC,          0)

/*********************************************************************
*
*                   LCD_ON / LCD_OFF
*
**********************************************************************
*/

//#define LCD_OFF() LCD_WRITE_REGLH(0x1FC >> 1,          0,          0)
//#define LCD_ON()  LCD_WRITE_REGLH(0x1FC >> 1, LCD_REG1FC,          0)

#endif /* LCDCONF_H */

