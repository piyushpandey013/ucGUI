/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : 2DGL_DrawScale.c
Purpose     : Drawing a scale using GUI-functions
----------------------------------------------------------------------
*/

#include "GUI.h"
#include <math.h>
#include <stddef.h>

#define SPEED 1100

/*******************************************************************
*
*       static code
*
********************************************************************
*/
/*******************************************************************
*
*       _DrawScale

  Drawing a scale using GUI-functions
*/
static void _DrawScale(void) {
  int XSize = LCD_GetXSize();
  int YSize = LCD_GetYSize();
  int XMid  = XSize / 2;
  int i;
  int r1   = 110;
  int r2   = 140;
  int rt   = 100;
  int y    = 240;
  int step =  15;
  int r = (r1 + r2) / 2;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_DispStringAt("DrawScale - Sample", 160, 5);
  while (1) {
    int c = 0;
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font8x16);
    GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_Delay(SPEED / 2);
    GUI_DispStringAtCEOL("using", 5, 40);
    GUI_DispStringAtCEOL("GUI_AA_DrawArc", 5, 55);
    GUI_Delay(SPEED * 2);
    GUI_SetPenSize(r2 - r1);
    GUI_SetColor(0x0000AA);
    GUI_AA_DrawArc(XMid, y, r, r, 45, 60);
    GUI_Delay(SPEED);
    GUI_SetColor(0x00AA00);
    GUI_AA_DrawArc(XMid, y, r, r, 60, 90);
    GUI_Delay(SPEED);
    GUI_SetPenSize(2);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawArc(XMid, y, r1, r1, 45, 135);
    GUI_Delay(SPEED);
    GUI_AA_DrawArc(XMid, y, r2, r2, 45, 135);
    GUI_Delay(SPEED);
    GUI_DispStringAtCEOL("", 5, 55);
    GUI_Delay(200);
    GUI_DispStringAtCEOL("using", 5, 40);
    GUI_DispStringAtCEOL("GUI_AA_DrawLine & GUI_DispCharAt", 5, 55);
    GUI_Delay(SPEED * 3);
    for (i = 45; i <= 135; i += step) {
      float co = cos(i * 3.1415926 / 180);
      float si = sin(i * 3.1415926 / 180);
      int   x1 = XMid - r1 * co;
      int   y1 = y    - r1 * si;
      int   x2 = XMid - (r2 - 1) * co;
      int   y2 = y    - (r2 - 1) * si;
      int   xt = XMid - rt * co;
      int   yt = y    - rt * si;
      GUI_SetColor(GUI_WHITE);
      GUI_SetPenSize(2);
      GUI_AA_DrawLine(x1, y1, x2, y2);
      GUI_SetColor(GUI_GREEN);
      GUI_SetFont(&GUI_Font8x8);
      GUI_DispCharAt('0' + c++, xt - 4, yt - 4);
      GUI_Delay(SPEED / 2);
    }
    GUI_Delay(SPEED * 3);
    GUI_ClearRect(0, 30, 320, 240);
  }
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates GUI-functions
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _DrawScale();
  while(1); 
}