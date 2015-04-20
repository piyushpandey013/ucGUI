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
File        : LCDConf_0713_M1_M128x64.h
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

#define LCD_CONTROLLER (1501)

#define LCD_XSIZE      (128)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (64)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL (1)

/*********************************************************************
*
*                   Simple bus configuration
*
**********************************************************************
*/

char LCD_X_Read00(void);
char LCD_X_Read01(void);
void LCD_X_Write00(char c);
void LCD_X_Write01(char c);
void LCD_X_WriteM01(char * pData, int NumBytes);
#define LCD_READ_A1(Byte)  Byte = LCD_X_Read01()
#define LCD_READ_A0(Byte)  Byte = LCD_X_Read00()
#define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
#define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
#define LCD_WRITEM_A1(pData, NumBytes) LCD_X_WriteM01(pData, NumBytes)

/*********************************************************************
*
*                   Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() \
  LCD_WRITE_A0(0xE2);  /* Software Reset                                   */ \
  LCD_WRITE_A0(0xA1);  /* ADC -> 0xa0 = normal, 0xa1 = revers              */ \
  LCD_WRITE_A0(0xC0);  /* SHL -> 0xc0 = normal, 0xc8 = revers              */ \
  LCD_WRITE_A0(0xA3);  /* LCD bias select 1/9                              */ \
  LCD_WRITE_A0(0x2F);  /* Use of all built in units VC = 1, VR = 1, VF = 1 */ \
  LCD_WRITE_A0(0x26);  /* Regulator resistor select, R1 & R2               */ \
  LCD_WRITE_A0(0x81);  /* Enter electronic volume mode                     */ \
  LCD_WRITE_A0(0x40);  /* Electronic volume: medium                        */ \
  LCD_WRITE_A0(0xAF);  /* display on */

#endif /* LCDCONF_H */
 
