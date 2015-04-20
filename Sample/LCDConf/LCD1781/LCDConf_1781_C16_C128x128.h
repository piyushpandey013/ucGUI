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
File        : LCDConf_1781_C16_C128x128.h
Purpose     : Internal sample configuration file
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

#define LCD_XSIZE         128
#define LCD_YSIZE         128

#define LCD_CONTROLLER   1781

#define LCD_BITSPERPIXEL   16
#define LCD_SWAP_RB         1

#define LCD_FIRSTCOM0       2

/*********************************************************************
*
*       Simple bus configuration
*
**********************************************************************
*/

void LCD_X_Write00(char c);
void LCD_X_Write01(char c);
void LCD_X_WriteM01(char * pData, int NumBytes);
char LCD_X_Read00(void);
char LCD_X_Read01(void);
#define LCD_WRITE_A1(Byte) LCD_X_Write01(Byte)
#define LCD_WRITE_A0(Byte) LCD_X_Write00(Byte)
#define LCD_WRITEM_A1(pData, NumBytes) LCD_X_WriteM01(pData, NumBytes)
#define LCD_READ_A1(Byte)  Byte = LCD_X_Read01()
#define LCD_READ_A0(Byte)  Byte = LCD_X_Read00()

/*********************************************************************
*
*       Initialisation macro
*
**********************************************************************
*/

#define LCD_INIT_CONTROLLER()                                                                   \
  LCD_X_Init();                                                                                 \
  LCD_WRITE_A0(0x94); /* Exit sleep mode */                                                     \
  LCD_WRITE_A0(0xd1); /* Oscillation mode set: Use internal clock and internal oscillator on */ \
  LCD_WRITE_A0(0xbb); /* COM output scan direction */                                           \
  LCD_WRITE_A1(0x01); /*   COM2->COM65, COM131->COM68 */                                        \
  LCD_WRITE_A0(0xbc); /* Data output scan direction set */                                      \
  LCD_WRITE_A1(0x00); /*   Normal page/column */                                                \
  LCD_WRITE_A1(0x00); /*   RGB color arrangement */                                             \
  LCD_WRITE_A1(0x00); /*   16bpp mode */                                                        \
  LCD_WRITE_A0(0xca); /* Display control set */                                                 \
  LCD_WRITE_A1(0x00); /*   Dummy */                                                             \
  LCD_WRITE_A1(0x20); /*   Duty = 1/132 */                                                      \
  LCD_WRITE_A1(0x00); /*   Dummy */                                                             \
  LCD_WRITE_A0(0x20); /* Power control register set */                                          \
  LCD_WRITE_A1(0x0b); /*   Booster x6 */                                                        \
  LCD_WRITE_A0(0x81); /* Contrast level & internal regulator resistor set */                    \
  LCD_WRITE_A1(0x1b); /*   EVR = 0x1b */                                                        \
  LCD_WRITE_A1(0x05); /*   1 + R2 / R1 = 11.37 */                                               \
  LCD_WRITE_A0(0xa7); /* Invers display */                                                      \
  LCD_WRITE_A0(0xfb); /* Set biasing ratio */                                                   \
  LCD_WRITE_A1(0x05); /*   1/12 bias */                                                         \
  LCD_WRITE_A0(0x92); /* Fill Enable/Disable */                                                 \
  LCD_WRITE_A1(0x01); /*   Enable filled color option */                                        \
  LCD_WRITE_A0(0xaf)  /* Display on */

/*********************************************************************
*
*       LCD_GET_BUSY macro
*
**********************************************************************
  Purpose:
    This macro should return 1 if the busy line of the SSD1781 is high
    and 0 if the busy line of the SSD1781 is low.
    The LCD_GET_BUSY busy macro is queried at the begin of each low level
    function of the LCD driver.
*/
/*
#define LCD_GET_BUSY() ...
*/

/*********************************************************************
*
*       LCD_WAIT macro
*
**********************************************************************
  Purpose:
    If the busy line is not available on the display module this macro
    can be used to call a function which waits a while in dependence
    of the number of pixels drawn by the DrawRect function of the SSD1781.
    The LCD_WAIT macro is called after each DrawRect function call of the SSD1781.
*/
#define LCD_WAIT(NumPixels) GUI_X_Delay((NumPixels + 900) / 1000)

#endif /* LCDCONF_H */

