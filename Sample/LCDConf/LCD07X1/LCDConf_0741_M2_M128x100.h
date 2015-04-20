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
File        : LCDConf_0741_M2_M128x100.h
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

#define LCD_CONTROLLER (741)

#define LCD_XSIZE      (128)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (100)   /* Y-resolution of LCD, Logical coor. */
#define LCD_REVERSE     1

#define LCD_FIRSTSEG0   0
#define LCD_LASTSEG0    (LCD_XSIZE + LCD_FIRSTSEG0 - 1)

#define LCD_BITSPERPIXEL (2)

/*********************************************************************
*
*                   Simple bus configuration
*
**********************************************************************
*/

void LCD_X_Init(void);
void LCD_X_Write01(char Data);
void LCD_X_WriteM01(char * pData, int NumBytes);
void LCD_X_Write00(char Cmd);
#define LCD_WRITE_A1(data)            LCD_X_Write01(data)
#define LCD_WRITEM_A1(data, NumBytes) LCD_X_WriteM01(data, NumBytes)
#define LCD_WRITE_A0(cmd)             LCD_X_Write00(cmd)

/*********************************************************************
*
*                   Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER()   \
  LCD_X_Init();  \
  LCD_WRITE_A0(0xe2);                     /* Reset */  \
  LCD_WRITE_A0(0x2f);                     /* Power control on  xxx Change needed !!!*/  \
  LCD_WRITE_A0(0x67);                     /* Set DC DC to 6 times */  \
  LCD_WRITE_A0(0xc0);                     /* COM direction */  \
  LCD_WRITE_A0(0xa1);                     /* SEG direction */  \
  LCD_WRITE_A0(0x44); LCD_WRITE_A0(0x0);  /* Set first COM to COM0 */  \
  LCD_WRITE_A0(0xab);                     /* Turn oscillator on */  \
  LCD_WRITE_A0(0x88); LCD_WRITE_A0(0x0);  /* Set gray level 0 */  \
  LCD_WRITE_A0(0x8a); LCD_WRITE_A0(0x44); /* Set gray level 1 */  \
  LCD_WRITE_A0(0x8c); LCD_WRITE_A0(0x66); /* Set gray level 2 */  \
  LCD_WRITE_A0(0x8e); LCD_WRITE_A0(0x99); /* Set gray level 3 */  \
  LCD_WRITE_A0(0x90); \
  LCD_WRITE_A0(0x48); LCD_WRITE_A0(0x81); /* Set Duty cycle */    \
  LCD_WRITE_A0(0x81); LCD_WRITE_A0(0x23); /* Set el. volume register */ \
  LCD_WRITE_A0(0x25); \
  LCD_WRITE_A0(0x55); \
  LCD_WRITE_A0(0x40); LCD_WRITE_A0(0x72); /* Start line */ \
  LCD_WRITE_A0(0xaf);

#endif /* LCDCONF_H */

