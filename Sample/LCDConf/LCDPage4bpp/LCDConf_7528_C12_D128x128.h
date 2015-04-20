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
File        : LCDConf_7528_C12_D128x128.h
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

#define LCD_XSIZE            128
#define LCD_YSIZE            128

#define LCD_CONTROLLER      7528

#define LCD_BITSPERPIXEL    12
#define LCD_BITSPERPIXEL_L0  4
#define LCD_DELTA_MODE       1

/*********************************************************************
*
*                   Simple bus configuration
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

#define LCD_INIT_CONTROLLER()                                                         \
  LCD_X_Init();                                                                       \
  LCD_WRITE_A0(0xa2); /* ICON control register OFF */                                 \
  LCD_WRITE_A0(0xae); /* Display off */                                               \
  LCD_WRITE_A0(0x40); /* Set Initial Display Line Register */                         \
  LCD_WRITE_A0(0x00); /*   segment 0 */                                               \
  LCD_WRITE_A0(0x48); /* Partial Duty SET */                                          \
  LCD_WRITE_A0(0x00); /*   no operation max */                                        \
  LCD_WRITE_A0(0xa0); /* ADC normal */                                                \
  LCD_WRITE_A0(0xc8); /* SHL revers */                                                \
  LCD_WRITE_A0(0x44); /* Set initial COM0 register */                                 \
  LCD_WRITE_A0(0x00); /*   COM0 */                                                    \
  LCD_WRITE_A0(0xab); /* Oscillator on */                                             \
  LCD_WRITE_A0(0x67); /* Select DC-DC step-up */                                      \
  LCD_WRITE_A0(0x27); /* Select regulator register */                                 \
  LCD_WRITE_A0(0x81); /* Select electronic volum register */                          \
  LCD_WRITE_A0(0x38); /*   0x38 */                                                    \
  LCD_WRITE_A0(0x55); /* Select LCD bias */                                           \
  LCD_WRITE_A0(0x92); /* Select FRC and PWM mode */                                   \
  LCD_WRITE_A0(0x2c); /* Power control set: VC on */                                  \
  GUI_Delay(100);     /* Delay 100ms */                                               \
  LCD_WRITE_A0(0x2e); /* Power control set: VR on */                                  \
  GUI_Delay(100);     /* Delay 100ms */                                               \
  LCD_WRITE_A0(0x2f); /* Power control set: VF on */                                  \
  LCD_WRITE_A0(0x4c); /* Set N-line inversion */                                      \
  LCD_WRITE_A0(0x00); /*   0x00 */                                                    \
  LCD_WRITE_A0(0x38); /* Mode set */                                                  \
  LCD_WRITE_A0(0x05); /*   BE=1, EXT=1 */                                             \
  {                                                                                   \
    int i, j;                                                                         \
    U8 aRegInit[] = {00, 06, 11, 16, 21, 26, 31, 35, 39, 43, 47, 50, 53, 56, 58, 60}; \
    U8 Adr = 0x80;                                                                    \
    for (i = 0; i < 16; i++) {                                                        \
      for (j = 0; j < 4; j++) {                                                       \
        LCD_WRITE_A0(Adr++);                                                          \
        LCD_WRITE_A0(aRegInit[i]);                                                    \
      }                                                                               \
    }                                                                                 \
  }                                                                                   \
  LCD_WRITE_A0(0x38); /* Mode set */                                                  \
  LCD_WRITE_A0(0x04); /*   BE=1, EXT=0 */                                             \
  LCD_WRITE_A0(0xaf)  /* Display on */

#endif /* LCDCONF_H */

