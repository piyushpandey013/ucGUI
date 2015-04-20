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
File        : LCD_X_I2CBUS.c
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

#define LCD_SLAVE_ADR      0x3c
#define LCD_CLR_SDA_IN()   P8 &= ~(1<<0)
#define LCD_CLR_SCL()      P8 &= ~(1<<2)
#define LCD_CLR_RESET()    P8 &= ~(1<<3)
#define LCD_CLR_SA0()      P8 &= ~(1<<4)
#define LCD_SET_SDA_IN()   P8 |=  (1<<0)
#define LCD_SET_SCL()      P8 |=  (1<<2)
#define LCD_SET_RESET()    P8 |=  (1<<3)
#define LCD_SET_SA0()      P8 |=  (1<<4)
#define LCD_READ_BIT()    (P8 &   (1<<1))
#define LCD_INIT_PORTS() \
  P8D &= ~(1<<1);        \
  PUR2 = 0xff;           \

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

#ifndef NOP
  #define NOP()
#endif

#define LCD_START() \
  LCD_CLR_SDA_IN(); \
  NOP();            \
  LCD_CLR_SCL();    \
  NOP()

#define LCD_STOP()  \
  LCD_SET_SCL();    \
  NOP();            \
  LCD_SET_SDA_IN(); \
  NOP()

#define LCD_SLAVE_WRITE ((LCD_SLAVE_ADR << 1) + 0)
#define LCD_SLAVE_READ  ((LCD_SLAVE_ADR << 1) + 1)
#define LCD_CNTRLBT_WRITE_CMD  0x00
#define LCD_CNTRLBT_WRITE_DATA 0x40

/*********************************************************************
*
*           Initialisation
*
**********************************************************************
  This routine should be called from your application program
  to set port pins to their initial values
*/

void LCD_X_Init(void) {
  LCD_INIT_PORTS();
  if (LCD_SLAVE_ADR & 1)
    LCD_SET_SA0();
  else
    LCD_CLR_SA0();
  LCD_CLR_RESET();
  LCD_SET_SDA_IN();
  LCD_SET_SCL();
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

/* Read 1 byte, MSB first */
static U8 _Read1(void) {
  U8 c = 0;
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<7); else c &= ~(1<<7); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<6); else c &= ~(1<<6); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<5); else c &= ~(1<<5); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<4); else c &= ~(1<<4); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<3); else c &= ~(1<<3); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<2); else c &= ~(1<<2); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<1); else c &= ~(1<<1); LCD_CLR_SCL();
  LCD_SET_SCL(); if (LCD_READ_BIT()) c |= (1<<0); else c &= ~(1<<0); LCD_CLR_SCL();
  LCD_SET_SDA_IN(); LCD_SET_SCL(); LCD_CLR_SCL();
  return c;
}

/* Write 1 byte, MSB first */
static void _Send1(U8 Data) {
  if (Data&(1<<7)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<6)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<5)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<4)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<3)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<2)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<1)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  if (Data&(1<<0)) { LCD_SET_SDA_IN(); } else { LCD_CLR_SDA_IN(); } NOP(); LCD_SET_SCL(); NOP(); LCD_CLR_SCL(); NOP(); 
  LCD_SET_SCL(); NOP(); LCD_CLR_SCL();
}

/* Read status */
char LCD_X_Read00(void) {
  U8 c;
  LCD_STOP();
  LCD_START();
  _Send1(LCD_SLAVE_READ);
  c = _Read1();
  LCD_STOP();
  LCD_START();
  _Send1(LCD_SLAVE_WRITE);
  _Send1(LCD_CNTRLBT_WRITE_DATA);
  return c;
}

/* Write data */
void LCD_X_Write01(char c) {
  _Send1(c);
}

/* Write multiple data bytes*/
void LCD_X_WriteM01(char * pData, int NumBytes) {
  for (; NumBytes; NumBytes--) {
    _Send1(*pData++);
  }
}

/* Write command */
void LCD_X_Write00(char c) {
  LCD_STOP();
  LCD_START();
  _Send1(LCD_SLAVE_WRITE);
  _Send1(LCD_CNTRLBT_WRITE_CMD);
  _Send1(c);
  LCD_STOP();
  LCD_START();
  _Send1(LCD_SLAVE_WRITE);
  _Send1(LCD_CNTRLBT_WRITE_DATA);
}

