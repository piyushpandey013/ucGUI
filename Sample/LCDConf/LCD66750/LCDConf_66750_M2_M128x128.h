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
File        : LCDConf_66750_M2_M128x128.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*       General configuration
*
**********************************************************************
*/

#define LCD_XSIZE 128
#define LCD_YSIZE 128
#define LCD_CONTROLLER 66750
#define LCD_BITSPERPIXEL 2
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 0
#define LCD_SWAP_XY  0

/*********************************************************************
*
*       Simple bus configuration
*
**********************************************************************
*/

void LCD_X_Write00(char c);
void LCD_X_Write01(char c);
char LCD_X_Read00(void);
char LCD_X_Read01(void);
#define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
#define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
#define LCD_READ_A1(Byte)  Byte = LCD_X_Read01()
#define LCD_READ_A0(Byte)  Byte = LCD_X_Read00()
void LCD_X_Init(void);
#define LCD_X_INIT() LCD_X_Init()

/*********************************************************************
*
*       Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() \
  LCD_X_INIT();       \
  LCD_WRITE_A0(0);    \
  LCD_WRITE_A0(0x03); \
  LCD_WRITE_A1(0x03); \
  LCD_WRITE_A1(0x0c); /* BS2-0:000, BT1-BT0:11, DC1-DC0:00, AP1-AP0:11, SLP:0, STB:0 (Power control) */ \
  LCD_WRITE_A0(0);    \
  LCD_WRITE_A0(0x04); \
  LCD_WRITE_A1(0x00); \
  LCD_WRITE_A1(0x10)  /* CT5-CT0:001000 (Contrast) */

/*********************************************************************
*
*       Definition of LCD_On and LCD_Off
*
**********************************************************************
*/

#define LCD_ON()      \
  LCD_WRITE_A0(0);    \
  LCD_WRITE_A0(0x07); \
  LCD_WRITE_A1(0x00); \
  LCD_WRITE_A1(0x01)  /* PS1-PS0:0, DHE:0, GS:0, REV:0, D:1 (Display control) */

#define LCD_OFF()     \
  LCD_WRITE_A0(0);    \
  LCD_WRITE_A0(0x07); \
  LCD_WRITE_A1(0x00); \
  LCD_WRITE_A1(0x00)  /* PS1-PS0:0, DHE:0, GS:0, REV:0, D:0 (Display control) */

#endif /* LCDCONF_H */

