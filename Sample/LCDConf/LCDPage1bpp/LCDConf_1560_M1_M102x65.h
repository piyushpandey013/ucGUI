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
File        : LCDConf_1560_M1_LDB_OG10651.h
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

#define LCD_CONTROLLER     1560

#define LCD_XSIZE          102
#define LCD_YSIZE          65
#define LCD_BITSPERPIXEL   1

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
  LCD_WRITE_A0(0xc5);  /* output status register (102 segments, 64 commons) */ \
  LCD_WRITE_A0(0x25);  /* turn on internal power supply                     */ \
  LCD_WRITE_A0(0xed);  /* power on completion command                       */ \
  LCD_WRITE_A0(0xae);  /* display off                                       */ \
  LCD_WRITE_A0(0xa9);  /* 1/64 duty                                         */ \
  LCD_WRITE_A0(0xa0);  /* adc normal                                        */ \
  LCD_WRITE_A0(0xab);  /* duty + 1                                          */ \
  LCD_WRITE_A0(0xa6);  /* normal display (dark pixel, light background)     */ \
  LCD_WRITE_A0(0x8b);  /* contrast mid level                                */ \
  LCD_WRITE_A0(0x40);  /* initial display line 0                            */ \
  LCD_WRITE_A0(0xaf);  /* display on                                        */

#endif /* LCDCONF_H */

