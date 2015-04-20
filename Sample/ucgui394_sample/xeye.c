/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*           (C) 1996    SEGGER Microcontroller Systeme GmbH          *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

**** emWin/GSC Grafical user interface for embedded applications ****
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : XEye_Sample.c
Purpose     : Demonstrates the use of transparent windows
----------------------------------------------------------------------
*/

#include <stddef.h>
#include <math.h>
#include "GUI.h"
#include "FRAMEWIN.h"

/*********************************************************************
*
*       defines
*
**********************************************************************
*/

#define FACTOR   5
#define EYE_X1  18
#define EYE_X2  54
#define EYE_Y   26
#define EYE_RX  16
#define EYE_RY  24
#define MIN_VISIBILITY 50          /* Make sure at least this many pixels stay visible when moved */

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

/* None */

/*********************************************************************
*
*       static code, helper functions
*
**********************************************************************
*/

/*******************************************************************
*
*       _Min
*/
static int _Min(int a, int b) {
  return (a < b) ? a : b;
}

/*******************************************************************
*
*       _DrawEye
*/
static void _DrawEye(int x0, int y0, int rx, int ry, int x1, int y1) {
  int dx, dy, x, y;
  float Hyp;
  GUI_SetColor(GUI_WHITE);
  GUI_FillEllipse(x0, y0, rx, ry);
  GUI_SetColor(GUI_BLACK);
  GUI_DrawEllipse(x0, y0, rx, ry);
  /* Calculate the coordinates */
  dx  = x1 - x0 + 1;
  dy  = y1 - y0 + 1;
  Hyp = sqrt(dx * dx + dy * dy);
  x   = (dx * _Min(rx - 5, abs(dx)) * FACTOR / Hyp) + x0 * FACTOR;
  y   = (dy * _Min(ry - 5, abs(dy)) * FACTOR / Hyp) + y0 * FACTOR;
  /* Draw the pupil */
  GUI_AA_SetFactor(FACTOR);
  GUI_AA_EnableHiRes();
  GUI_SetColor(0xD00000);
  GUI_AA_FillCircle(x, y, 5.75 * FACTOR);
  GUI_SetColor(GUI_BLACK);
  GUI_AA_FillCircle(x, y, 3.75 * FACTOR);
  GUI_SetColor(GUI_GRAY);
  GUI_AA_FillCircle(x - 1.25 * FACTOR, y - 1.25 * FACTOR, 1.25 * FACTOR);
  GUI_AA_DisableHiRes();
}

/*********************************************************************
*
*       static code, callbacks
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbTransWindow
*/
static void _cbTransWindow(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  GUI_PID_STATE State;
  GUI_PID_GetState(&State);
  switch (pMsg->MsgId) {
  case WM_PAINT:
    State.x -= WM_GetWindowOrgX(hWin);
    State.y -= WM_GetWindowOrgY(hWin);
    _DrawEye(EYE_X1, EYE_Y, EYE_RX, EYE_RY, State.x, State.y);
    _DrawEye(EYE_X2, EYE_Y, EYE_RX, EYE_RY, State.x, State.y);
    break;
  case WM_TOUCH:
    if (pMsg->Data.p) {
      GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
      if (pState->Pressed) {
        int mx, my, x;
        I32 Sum, SumY;
        I32 OutConst = EYE_RX * EYE_RX * EYE_RY * EYE_RY + (EYE_RX * EYE_RX * EYE_RY >> 1);
        mx = pState->x;
        my = (pState->y < EYE_Y) ? (EYE_Y - pState->y) : (pState->y - EYE_Y);
        if (my >= 0 && my <= EYE_RY) {
          int y;
          x = EYE_RX;
          for (y = 0; y <= my; y++) {
            SumY =((I32)(EYE_RX * EYE_RX)) * ((I32)(y * y));
            while (Sum = SumY + ((I32)(EYE_RY * EYE_RY)) * ((I32)(x * x)), (x > 0) && (Sum > OutConst)) {
              x--;
            }
          }
        } else {
          x = -EYE_RX;
        }
        if (((mx >= EYE_X1 - x) && (mx <= EYE_X1 + x)) || 
            ((mx >= EYE_X2 - x) && (mx <= EYE_X2 + x)) || WM_HasCaptured(hWin))
        {
          WM_SetCaptureMove(hWin, pState, MIN_VISIBILITY);
        } else {
          /* Transfer the message to the underlying window, since the transparent area has been clicked. */
          WM_HWIN hBelow = WM_Screen2hWinEx(hWin, State.x, State.y);
          if (hBelow) {
            pState->x = State.x - WM_GetWindowOrgX(hBelow);
            pState->y = State.y - WM_GetWindowOrgY(hBelow);
            WM_SendMessage(hBelow, pMsg);
          }
        }
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}


/*******************************************************************
*
*       _DemoTransWindow
*/
static void _DemoTransWindow(void) {
  WM_HWIN hTransWin, hFrameWin;
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_EnableMemdev(WM_HBKWIN);
  WM_SetDesktopColor(GUI_RED);
  hTransWin = WM_CreateWindow(10,  10, 72, 52, WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP,
                              &_cbTransWindow, 0);
  hFrameWin = FRAMEWIN_Create("Framewin", 0, WM_CF_SHOW, 100, 70, 120, 100);
  FRAMEWIN_SetActive(hFrameWin, 1);
  FRAMEWIN_SetMoveable(hFrameWin, 1);
  GUI_CURSOR_Show();
  while (1) {
    WM_InvalidateWindow(hTransWin);
    GUI_Delay(20);
  }
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _DemoTransWindow();
}


