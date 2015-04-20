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
File        : LCDConf_501_C8_C96x64.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*       General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE         96
#define LCD_YSIZE         64
#define LCD_CONTROLLER   501
#define LCD_BITSPERPIXEL   8
#define LCD_FIXEDPALETTE 233
#define LCD_SWAP_RB        1

/*********************************************************************
*
*       Simple bus configuration
*
**********************************************************************
*/

#ifndef WIN32
  void LCD_X_Write00(char c);
  void LCD_X_Write01(char c);
  #define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
  #define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
#endif

/*********************************************************************
*
*       Init sequence
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() /* tbd */

#endif /* LCDCONF_H */

