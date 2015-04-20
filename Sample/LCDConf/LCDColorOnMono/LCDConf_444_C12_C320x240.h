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
File        : LCDConf_444_C12_C320x240.h
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

#define LCD_XSIZE      (320)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (240)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL (12)

#define LCD_CONTROLLER 444

#define LCD_SWAP_BYTE_ORDER (1)

/*********************************************************************
*
*                   Full bus configuration
*
**********************************************************************
*/

#define LCD_READ_MEM(Off)            *((U16*)         (0xc0000000+(((U32)(Off))<<1)))
#define LCD_WRITE_MEM(Off,data)      *((U16*)         (0xc0000000+(((U32)(Off))<<1)))=data
 
#define  LCD_INIT_CONTROLLER()

#endif /* LCDCONF_H */

 
