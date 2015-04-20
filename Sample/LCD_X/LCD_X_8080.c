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
File        : LCD_X_8080.c
Purpose     : Port routines
----------------------------------------------------------------------
*/

/*********************************************************************
*
*           Hardware configuration
*
**********************************************************************
  Needs to be adopted to your target hardware.
*/

/* Configuration example:

#define Chip_30600
#include <IOM16C.H>

#define LCD_CLR_A0()      P8 &= ~(1<<0)
#define LCD_SET_A0()      P8 |=  (1<<0)
#define LCD_CLR_WR()      P8 &= ~(1<<1)
#define LCD_SET_WR()      P8 |=  (1<<1)
#define LCD_CLR_RD()      P8 &= ~(1<<2)
#define LCD_SET_RD()      P8 |=  (1<<2)
#define LCD_CLR_RESET()   P8 &= ~(1<<3)
#define LCD_SET_RESET()   P8 |=  (1<<3)
#define LCD_CLR_CS()      P8 &= ~(1<<4)
#define LCD_SET_CS()      P8 |=  (1<<4)
#define LCD_DATA_IN       P7
#define LCD_DATA_OUT      P7
#define LCD_SET_DIR_IN()  P7D = 0
#define LCD_SET_DIR_OUT() P7D = 0xff
#define LCD_DELAY(ms)     GUI_Delay(ms)
*/

/*********************************************************************
*
*           High level LCD access macros
*
**********************************************************************
  Usually, there is no need to modify these macros.
  It should be sufficient ot modify the low-level macros
  above.
*/

#define LCD_X_READ()      \
  LCD_SET_DIR_IN();       \
  LCD_CLR_CS();           \
  LCD_CLR_RD();           \
  c = LCD_DATA_IN;        \
  LCD_SET_CS();           \
  LCD_SET_RD();           \
  return c

#define LCD_X_WRITE(data) \
  LCD_SET_DIR_OUT();      \
  LCD_DATA_OUT = data;    \
  LCD_CLR_CS();           \
  LCD_CLR_WR();           \
  LCD_SET_WR();           \
  LCD_SET_CS()

/*********************************************************************
*
*           Initialisation
*
**********************************************************************
  This routine should be called from your application program
  to set port pins to their initial values
*/

void LCD_X_Init(void) {
  LCD_SET_CS();
  LCD_SET_RD();
  LCD_SET_WR();
  LCD_CLR_RESET();
  LCD_DELAY(2);
  LCD_SET_RESET();
}

/*********************************************************************
*
*           Access routines
*
**********************************************************************
  Usually, there is no need to modify these routines.
  It should be sufficient ot modify the low-level macros
  above.
*/

/* Write to controller, with A0 = 0 */
void LCD_X_Write00(char c) {
  LCD_CLR_A0();
  LCD_X_WRITE(c);
}

/* Write to controller, with A0 = 1 */
void LCD_X_Write01(char c) {
  LCD_SET_A0();
  LCD_X_WRITE(c);
}

/* Write multiple bytes to controller, with A0 = 1 */
void LCD_X_WriteM01(char * pData, int NumBytes) {
  LCD_SET_A0();
  for (; NumBytes; NumBytes--) {
    LCD_X_WRITE(*pData++);
  }
}

/* Read from controller, with A0 = 0 */
char LCD_X_Read00(void) {
  char c;
  LCD_CLR_A0();
  LCD_X_READ();
}

/* Read from controller, with A0 = 1 */
char LCD_X_Read01(void) {
  char c;
  LCD_SET_A0();
  LCD_X_READ();
}

