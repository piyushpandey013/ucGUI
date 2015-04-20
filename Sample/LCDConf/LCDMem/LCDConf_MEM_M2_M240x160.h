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
File        : LCDConf_MEM_M2_M240x160.h
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

#define LCD_CONTROLLER    0
#define LCD_XSIZE         240            /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE         160            /* Y-resolution of LCD, Logical coor. */
#define LCD_INTERFACEBITS 4              /* select 4 or 8 bit interface */
#define LCD_BITSPERPIXEL  2              /* Permitted values here: 1 or 2 */

#define LCD_TIMERINIT0  1200
#define LCD_TIMERINIT1  1500

#endif /* LCDCONF_H */

