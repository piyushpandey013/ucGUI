/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2002         SEGGER Microcontroller Systeme GmbH        *
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
File        : SCROLLBAR_Move.c
Purpose     : Example demonstrating scrollbar as child of a window
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "SCROLLBAR.h"
#include "EDIT.h"
#include <stddef.h>

/*******************************************************************
*
*       defines
*
********************************************************************
*/

#define EDIT_MAX_X 10
#define EDIT_MAX_Y 3

/*******************************************************************
*
*       static variables
*
********************************************************************
*/

static EDIT_Handle _aahEdit[EDIT_MAX_Y][EDIT_MAX_X];
static int _x,_y;

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _cbWindow

  The callback moves the edit-fiels when a notification message
  was send.
*/
static void _cbWindow(WM_MESSAGE *pMsg) {
  WM_SCROLL_STATE ScrollState;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED) {
      if (WM_GetId(pMsg->hWinSrc) == GUI_ID_HSCROLL) {
        WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
        if (_x != ScrollState.v) {
          int x, y;
          for (y = 0; y < EDIT_MAX_Y; y++) {
            for (x = 0; x < EDIT_MAX_X; x++) {
              WM_MoveWindow(_aahEdit[y][x], _x - ScrollState.v, 0);
            }
          }
          _x = ScrollState.v;
        }
      }
    }
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _DemoScrollbarMove

  This function creates the window and his child objects. Then it
  runs into a idle-loop, so that the window manager can handle the
  objects.
*/
static void _DemoScrollbarMove(void) {
  int x, y;
  WM_HWIN hWindow;
  SCROLLBAR_Handle hScroll;
  /* Clear display and display headline */
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_DispStringAt("SCROLLBAR_Move - Sample", 160, 5);
  GUI_SetTextAlign(GUI_TA_LEFT);
  GUI_SetColor(0xFFFFFF);
  GUI_SetFont(&GUI_Font8x16);
  /* Create the window */
  hWindow = WM_CreateWindow(50, 90, 220, 79, WM_CF_SHOW, &_cbWindow, 0);
  /* Create the scrollbar */
  hScroll = SCROLLBAR_CreateAttached(hWindow, 0);
  SCROLLBAR_SetNumItems(hScroll, 48 * EDIT_MAX_X);
  SCROLLBAR_SetPageSize(hScroll, 220);
  /* Create the edit-fields */
  for (y = 0; y < EDIT_MAX_Y; y++) {
    for (x = 0; x < EDIT_MAX_X; x++) {
      _aahEdit[y][x] = EDIT_CreateAsChild(x * 48, y * 22, 48, 22, 
                                        hWindow, 13, WM_CF_SHOW, 5);
      EDIT_SetTextAlign(_aahEdit[y][x], GUI_TA_RIGHT | GUI_TA_VCENTER);
      EDIT_SetFont(_aahEdit[y][x], &GUI_Font8x16);
      EDIT_SetDecMode(_aahEdit[y][x], ((y * EDIT_MAX_X) + x) * 100, -99999, +99999, 2, 0);
	  }
  }
  /* idle-loop */
  while (1) {
    GUI_Exec();
    GUI_Delay(20);
  }
  /* delete window and clear display */
  WM_DeleteWindow(hWindow);
  GUI_Clear();
}

/*******************************************************************
*
*       MainTask
*
*       Demonstates the use of a scrollbar as child of a window
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  while(1) {
    _DemoScrollbarMove();
  }
}
