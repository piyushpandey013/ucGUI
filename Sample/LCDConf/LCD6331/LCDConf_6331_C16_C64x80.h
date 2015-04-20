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
File        : LCDConf_6331_C16_C64x80.h
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

#define LCD_XSIZE          64                /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE          80                /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL   16
#define LCD_CONTROLLER   6331
#define LCD_FIXEDPALETTE  565
#define LCD_SWAP_RB         1
#define LCD_FIRSTSEG0       2                 /* Do not use the first n segments */
#define LCD_FIRSTCOM0       2                 /* Do not use the first n com lines */

#define LCD_MIRROR_X       (1)


/*********************************************************************
*
*       Simple bus configuration
*
**********************************************************************
*/

#ifndef WIN32
  void LCD_X_Write00(char c);
  void LCD_X_Write01(char c);
  void LCD_X_WriteM01(unsigned char * pData, int NumBytes);
  #define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
  #define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
  #define LCD_WRITEM_A1(pData, NumBytes) LCD_X_WriteM01(pData, NumBytes)
#endif

/*********************************************************************
*
*       Init sequence
*
**********************************************************************
*/

/* 'Driver Output Mode' and 'Entry Mode' will be initialized by the driver */

#define LCD_INIT_CONTROLLER() \
  LCD_WRITE_A0(0x02); LCD_WRITE_A0(0x01);                     /* Oscillation Mode Set: OSC =1 */ \
  LCD_WRITE_A0(0x24); LCD_WRITE_A0(0x07);                     /* DCDC Clock Division Set: DIV=111 (divide by 16) */ \
  LCD_WRITE_A0(0x2C);                                         /* Standby Mode OFF */ \
  LCD_WRITE_A0(0x22); LCD_WRITE_A0(0x01);                     /* Bias Set: BIAS=1 (1/6) */ \
  LCD_WRITE_A0(0x20); LCD_WRITE_A0(0x01);                     /* DC-DC Select: DC=1 */ \
  LCD_WRITE_A0(0x26); LCD_WRITE_A0(0x0F);                     /* OP-AMP: on, all 3 boosters on */ \
  LCD_WRITE_A0(0x2A); LCD_WRITE_A0(0xB0);                     /* Contrast: 0xb0 */ \
  LCD_WRITE_A0(0x28); LCD_WRITE_A0(0x00);                     /* Temperature Compensation Set */ \
  LCD_WRITE_A0(0x2E);                                         /* DDRAM Burst Mode: off */ \
  LCD_WRITE_A0(0x30); LCD_WRITE_A0(0x19);                     /* Addressing Mode Set: GSM=00, DSG=1, SGF=0, SGM=0, SGP=1 */ \

#endif /* LCDCONF_H */
