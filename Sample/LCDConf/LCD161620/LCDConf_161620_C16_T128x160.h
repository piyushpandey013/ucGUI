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
File        : LCDConf_161620_C16_T128x160.h
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

#define LCD_XSIZE           (128)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE           (160)   /* Y-resolution of LCD, Logical coor. */
#define LCD_BITSPERPIXEL     (12)
#define LCD_CONTROLLER      (180)
#define LCD_FIXEDPALETTE (444121)

/*********************************************************************
*
*       Simple bus configuration
*
**********************************************************************
*/

#ifndef WIN32
  void LCD_X_Write00(char c);
  void LCD_X_Write01(char c);
  char LCD_X_Read00(void);
  char LCD_X_Read01(void);
  #define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
  #define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
  #define LCD_READ_A1(Byte)  Byte = LCD_X_Read01()
  #define LCD_READ_A0(Byte)  Byte = LCD_X_Read00()
#endif

/*********************************************************************
*
*       Init sequence
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER() \
  LCD_X_Init();  \
  LCD_WRITE_REG( 2, 0x01); /* Reset */ \
  LCD_WRITE_REG(32, 0x08); /* DC/DC and regulator all off */ \
  LCD_WRITE_REG(34, 0x08); /* Vdc x 6 */ \
  LCD_WRITE_REG(32, 0x0f); /* DC/DC and regulator all on */ \
  LCD_WRITE_REG( 0, 0x00); /* RMW off, display off, normal operation, ADX, ADC, ADR = 0 */ \
  LCD_WRITE_REG( 1, 0x20); /* 1 pixel/2 byte */ \
  LCD_WRITE_REG( 6, 0x00); /* Line inversion */ \
  LCD_WRITE_REG(38, 0x00); /* Calibration start */ \
  GUI_Delay(100);          /* Delay */ \
  LCD_WRITE_REG(38, 0x01); /* Calibration stop */ \
  LCD_WRITE_REG( 0, 0x40)  /* RMW off, display on, normal operation, ADX, ADC, ADR = 0 */
 
#endif /* LCDCONF_H */
