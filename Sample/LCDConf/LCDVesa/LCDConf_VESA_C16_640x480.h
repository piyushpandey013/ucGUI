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
File        : LCDConf_VESA_C16_640x480.h
Purpose     : Sample configuration file
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

#define LCD_XSIZE            640   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE            480   /* Y-resolution of LCD, Logical coor. */
#define LCD_BITSPERPIXEL      16
#define LCD_CONTROLLER      8600
#define LCD_SWAP_RB            1
#define LCD_INIT_CONTROLLER()

#endif

