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
File        : LCDConf_1520_M1_M122x31.h
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

#define LCD_CONTROLLER     1520

#define LCD_XSIZE           122
#define LCD_YSIZE            32
#define LCD_BITSPERPIXEL      1

#define LCD_NUM_CONTROLLERS   2

#define LCD_FIRSTSEG0         0
#define LCD_LASTSEG0         60
#define LCD_FIRSTSEG1        61
#define LCD_LASTSEG1        121

#define LCD_FIRSTCOM1          LCD_FIRSTCOM0
#define LCD_LASTCOM1           LCD_LASTCOM0

/*********************************************************************
*
*                   Simple bus configuration
*
**********************************************************************
*/

char LCD_X_Read00(void);
char LCD_X_Read01(void);
char LCD_X_Read10(void);
char LCD_X_Read11(void);
void LCD_X_Write00(char c);
void LCD_X_Write01(char c);
void LCD_X_Write10(char c);
void LCD_X_Write11(char c);
#define LCD_READ_A1C1(Byte)  Byte = LCD_X_Read01()
#define LCD_READ_A0C1(Byte)  Byte = LCD_X_Read00()
#define LCD_READ_A1(Byte)    Byte = LCD_X_Read11()
#define LCD_READ_A0(Byte)    Byte = LCD_X_Read10()
#define LCD_WRITE_A1C1(Byte) LCD_X_Write01(Byte)
#define LCD_WRITE_A0C1(Byte) LCD_X_Write00(Byte)
#define LCD_WRITE_A1(Byte)   LCD_X_Write11(Byte)
#define LCD_WRITE_A0(Byte)   LCD_X_Write10(Byte)

/*********************************************************************
*
*                   Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() \
  LCD_X_Init(); \
  WRITE_CMD0(0xe2);  /* reset       */ \
  WRITE_CMD0(0xae);  /* display off */ \
  WRITE_CMD0(0xa9);  /* 1/64 duty   */ \
  WRITE_CMD0(0xaf);  /* display on  */ \
  WRITE_CMD1(0xe2);  /* reset       */ \
  WRITE_CMD1(0xae);  /* display off */ \
  WRITE_CMD1(0xa9);  /* 1/64 duty   */ \
  WRITE_CMD1(0xaf);  /* display on  */

#endif /* LCDCONF_H */

