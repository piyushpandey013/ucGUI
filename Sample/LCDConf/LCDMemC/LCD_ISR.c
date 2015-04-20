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
File        : LCD_ISR.c
Purpose     : Interrupt service routine for display refresh written in 'C'.
              This routine should be used only as a sample. It shows how to
              write the video data to the display. It normally needs
              to be written in assembler to be fast enough.
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "LCD_Private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       Config defaults
*
* These configuration macros normally should be defined in LCDConf.h.
* The following configuration works on a M16C hardware
*/
#ifndef   LCD_PORT_DATA
  #define LCD_PORT_DATA P7  /* Port used to transfer the data */
#endif

#ifndef   LCD_PORT_CTRL
  #define LCD_PORT_CTRL P8  /* Port used for the control signals */
#endif

#ifndef   MASK_M
  #define MASK_M   (1 << 0) /* Mask to access the M signal of the control port */
#endif

#ifndef   MASK_FLM
  #define MASK_FLM (1 << 1) /* Mask to access the FLM signal (first line) of the control port */
#endif

#ifndef   MASK_CL1
  #define MASK_CL1 (1 << 2) /* Mask to access the line clock signal of the control port */
#endif

#ifndef   MASK_DON
  #define MASK_DON (1 << 3) /* Mask to access the pixel display on signal of the control port */
#endif

#ifndef   MASK_CL2
  #define MASK_CL2 (1 << 4) /* Mask to access the pixel clock signal of the control port */
#endif

/*********************************************************************
*
*       START_TIMER
*
* Purpose:
*   The timer macro is used to start a timer to generate timer interrupts.
*   The function LCD_ISR() should be called in reacton of the timer interrupts.
*   The frequency is set by the function _SetFreq() which is called before
*   this macro is executed.
*/
#ifndef   START_TIMER
  #define START_TIMER()                                              \
    TA1MR = 0;           /* Timer mode register */                   \
                         /* bit 01 : 00 Timer mode */                \
                         /* bit 2  :  0 No puls output */            \
                         /* bit 34 : 0X No gate function */          \
                         /* bit 5  : 0 */                            \
                         /* bit 67 : 00 clock source : fx/2 */       \
    TA1IC.0 = 1;         /* Timer interrupt register, priority 1 */  \
    TABSR  =  (1 << 1)   /* Timer count start flag */
#endif

/*********************************************************************
*
*       STOP_TIMER
*
* Purpose:
*   Stops the timer started with the START_TIMER macro.
*/
#ifndef   STOP_TIMER
  #define STOP_TIMER()                                               \
    TABSR &= ~(1 << 1);  /* Timer count start flag */                \
    P7D    = 0xff;                                                   \
    P8D    = 0xff
#endif

/*********************************************************************
*
*       Internal macros
*/
#define LCD_BYTESPERLINE (((LCD_VXSIZE_PHYS) + 7) / 8)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static U8 LCD_VRAMTYPE * _pData; /* Pointer to the video RAM of the display driver */
static char _Line;               /* Current display line to be accessed */
static char _GrayCnt;            /* Zero based index to currently accessed data pane of the driver */
static char _MCnt;               /* Counter used to decide when the M signal of the control port should be inverted */

/*********************************************************************
*
*       External data
*
**********************************************************************
*/
extern LCD_VRAMTYPE U8  LCD_VRAMTYPE * LCD__apVRam[LCD_BITSPERPIXEL]; /* External VRam panes of the display driver */
extern LCD_VRAMTYPE U16 LCD__aTimerReload[LCD_BITSPERPIXEL];          /* External timer values used to show each pane with a different time */

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetFreq
*
* Purpose:
*   Inits the timer used to refresh one display line
*/
void _SetFreq(int f) {
  TA1 = f;
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_ISR
*
* Purpose:
*   Sends the video data to the display.
*   This routine should be called from an external interrupt service routine.
*/
void LCD_ISR(void) {
  int i = LCD_BYTESPERLINE;
  /* Send data out */
  do {
    U8 Data = *(_pData++);
    #if (LCD_INTERFACEBITS == 4)
      LCD_PORT_CTRL |=  MASK_CL2;  /* Pixel clock signal high */
      LCD_PORT_DATA =   Data >> 4; /* Write the first pixel stored in the upper nibble */
      LCD_PORT_CTRL &= ~MASK_CL2;  /* Pixel clock signal low */
      LCD_PORT_CTRL |=  MASK_CL2;  /* Pixel clock signal high */
      LCD_PORT_DATA =   Data;      /* Write the second pixel stored in the lower nibble */
      LCD_PORT_CTRL &= ~MASK_CL2;  /* Pixel clock signal low */
    #elif (LCD_INTERFACEBITS == 8)
      LCD_PORT_CTRL |=  MASK_CL2;  /* Pixel clock signal high */
      LCD_PORT_DATA =   Data;      /* Write 8 bit pixel data */
      LCD_PORT_CTRL &= ~MASK_CL2;  /* Pixel clock signal low */
    #else
      #error This sample routine does not support this interface yet. Please add the code here.
    #endif
  } while (--i);
  /* Give line signals */
  if (_Line == 0) {
    LCD_PORT_CTRL |=  MASK_FLM; /* First line signal high */
    LCD_PORT_CTRL |=  MASK_CL1; /* Line clock signal high */
    LCD_PORT_CTRL &= ~MASK_CL1; /* Line clock signal low */
    LCD_PORT_CTRL &= ~MASK_FLM; /* First line signal low */
  } else {
    LCD_PORT_CTRL |=  MASK_CL1; /* Line clock signal high */
    LCD_PORT_CTRL &= ~MASK_CL1; /* Line clock signal low */
  }
  /* Invert the M signal regular */
  if (!--_MCnt) {
    _MCnt = 7;
    LCD_PORT_CTRL ^= MASK_M;
  }
  /* If the last line of a pane is written set the data pointer to the first byte of the next pane. */
  if (++_Line == LCD_YSIZE) {
    _Line = 0;
    _GrayCnt ^= 1;
    _SetFreq(LCD__aTimerReload[_GrayCnt]);
    _pData = (U8 LCD_VRAMTYPE *)LCD__apVRam[_GrayCnt]; /* Set the data pointer to the first byte of the right pane */
  }
}

/*********************************************************************
*
*       LCD_X_On
*/
void LCD_X_On(void) {
  _pData = (U8 LCD_VRAMTYPE *)LCD__apVRam[0];          /* Set the data pointer to the first byte of the first pane */
  _SetFreq(LCD__aTimerReload[0]);                      /* Init the timer with the first value */
  START_TIMER();
  LCD_PORT_CTRL |= MASK_DON;
}

/*********************************************************************
*
*       LCD_X_Off
*/
void LCD_X_Off(void) {
  STOP_TIMER();
  LCD_PORT_CTRL &= ~MASK_DON;
}

/*************************** End of file ****************************/
