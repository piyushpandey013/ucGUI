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
File        : LCDConf_8535_M1_M93x65.h
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

#define LCD_CONTROLLER 1505

#define LCD_XSIZE       (93)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE       (65)   /* Y-resolution of LCD, Logical coor. */
#define LCD_XOFF        (133 - LCD_XSIZE)

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

#define LCD_INIT_CONTROLLER()                                          \
  LCD_X_Init();                                                        \
  LCD_WRITE_A0(0x01); /* jump to command page 111 */                   \
  LCD_WRITE_A0(0x10); /* power-down control, data entry mode */        \
  LCD_WRITE_A0(0x0E); /* select command page 110 */                    \
  LCD_WRITE_A0(0x06); /* sets display mode normal */                   \
  LCD_WRITE_A0(0x0C); /* mirror X = 1, mirror Y = 0 */                 \
  LCD_WRITE_A0(0x12); /* set bias system 010 */                        \
  LCD_WRITE_A0(0x20); /* set current for bias system IB = 0 */         \
  LCD_WRITE_A0(0x84); /* set multiplex rate 100 */                     \
  LCD_WRITE_A0(0x01); /* jump to command page 111 */                   \
  LCD_WRITE_A0(0x0D); /* select command page 101 */                    \
  LCD_WRITE_A0(0x08); /* Number of HV-gen voltage multiplication 00 */ \
  LCD_WRITE_A0(0x12); /* set temperature coefficient 010 */            \
  LCD_WRITE_A0(0xA8); /* set VLCD register */                          \
  LCD_WRITE_A0(0x07); /* VLCD range, enable/disable HV-gen */          \
  LCD_WRITE_A0(0x01)  /* jump to command page 111 */

#endif /* LCDCONF_H */
