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
File        : LCDConf_8811_M1_M128x80.h
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

#define LCD_CONTROLLER 1504

#define LCD_XSIZE      (128)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE       (80)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL (1)

/*********************************************************************
*
*                   Simple bus configuration
*
**********************************************************************
*/

void LCD_X_Write00 (char c);
void LCD_X_Write01 (char c);
void LCD_X_WriteM01(char * pData, int NumBytes);
char LCD_X_Read00  (void);
char LCD_X_Read01  (void);
#define LCD_WRITE_A0(Byte)             LCD_X_Write00(Byte)
#define LCD_WRITE_A1(Byte)             LCD_X_Write01(Byte)
#define LCD_WRITEM_A1(pData, NumBytes) LCD_X_WriteM01(pData, NumBytes)
#define LCD_READ_A1(Byte)              Byte = LCD_X_Read01()
#define LCD_READ_A0(Byte)              Byte = LCD_X_Read00()

/*********************************************************************
*
*                   Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() \
  LCD_X_Init(); \
  LCD_WRITE_A0(0xE2); /* Software Reset */ \
  LCD_WRITE_A0(0xAB); /* Internal Oscillator ON */ \
  LCD_WRITE_A0(0xE1); /* power save mode off */ \
  LCD_WRITE_A0(0x62); /* 4x multiplier */ \
  LCD_WRITE_A0(0x81); /* VOP first byte */ \
  LCD_WRITE_A0(0xD3); /* VOP 2nd byte */ \
  LCD_WRITE_A0(0x2B); /* Internal Charge Pump ON */ \
  LCD_WRITE_A0(0xAF); /* Display ON */ \
  LCD_WRITE_A0(0x1E); /* Frame Rate Frequency 50Hz */ \
  LCD_WRITE_A0(0xA1); /* X mirroring ON */ \
  LCD_WRITE_A0(0x40); /* set first diplay line */ \
  LCD_WRITE_A0(0x00); /* set initial display line */ \
  LCD_WRITE_A0(0x1A); /* p=automatically set */ \
  LCD_WRITE_A0(0xAF); /* Display ON */ \

#endif /* LCDCONF_H */

