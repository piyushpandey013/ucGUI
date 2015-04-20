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
File        : SIM_X.c
Purpose     : Windows Simulator externals
              The init routine in this file can be used to set up the
              simulator
---------------------------END-OF-HEADER------------------------------
*/

#include "SIM.h"
#include "GUI.h"

void SIM_X_Init() {
  if (((LCD_GetDevCap(LCD_DEVCAP_XSIZE) * LCD_GetXMag()) == 320) && 
      ((LCD_GetDevCap(LCD_DEVCAP_YSIZE) * LCD_GetYMag()) == 240) && 
      (LCD_GetNumLayers() == 1)) {
    SIM_SetLCDPos(71,38);                   // Define the position of the LCD in the bitmap
    SIM_SetTransColor  (0xff0000);          // Define the transparent color
/*
    SIM_SetLCDColorBlack(0, 0x808080);      // Define black color
    SIM_SetLCDColorWhite(0, 0xc0c0c0);      // Define white color
*/
  }
}
