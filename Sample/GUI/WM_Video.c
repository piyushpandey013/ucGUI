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
File        : WM_Video.c
Purpose     :
----------------------------------------------------------------------
*/

#include <stddef.h>
#include <stdio.h>

#include "WM.h"
#include "GUI.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"

static void _cbFrameWinTest(WM_MESSAGE* pMsg);

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define MAX_WINDOWS   5

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static WM_HWIN _ahWin[MAX_WINDOWS];
static char    _IsCompletelyVis;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _CreateWindow
*/
static void _CreateWindow(void) {
  unsigned i;
  for (i = 0; i < MAX_WINDOWS; i++) {
    if (_ahWin[i] == 0) {
      WM_HWIN hWin;
      char ac[32];
      sprintf(ac, "Test window %d", i + 1);
      hWin = FRAMEWIN_CreateEx(5 + 10 * i, 135 + 10 * i, 120, 60, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, ac, _cbFrameWinTest);
      FRAMEWIN_SetClientColor(hWin, GUI_INVALID_COLOR);
      _ahWin[i] = hWin;
      break;
    }
  }
}

/*********************************************************************
*
*       _DeleteWindow
*/
static void _DeleteWindow(void) {
  unsigned i;
  for (i = 0; i < MAX_WINDOWS; i++) {
    if (WM_IsVisible(_ahWin[i])) {
      WM_DeleteWindow(_ahWin[i]);
      _ahWin[i] = 0;
      break;
    }
  }
}

/*********************************************************************
*
*       _ShowWindow
*/
static void _ShowWindow(void) {
  unsigned i;
  for (i = 0; i < MAX_WINDOWS; i++) {
    if (_ahWin[i] != 0) {
      if (WM_IsVisible(_ahWin[i]) == 0) {
        WM_ShowWindow(_ahWin[i]);
        break;
      }
    }
  }
}

/*********************************************************************
*
*       _HideWindow
*/
static void _HideWindow(void) {
  unsigned i;
  for (i = 0; i < MAX_WINDOWS; i++) {
    if (WM_IsVisible(_ahWin[i])) {
      WM_HideWindow(_ahWin[i]);
      break;
    }
  }
}

/*********************************************************************
*
*       Static code, callbacks
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbBkWin
*/
static void _cbBkWin(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    break;
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      int Id = WM_GetId(pMsg->hWinSrc);
      switch (Id) {
      case GUI_ID_BUTTON0:  /* Create window */
        _CreateWindow();
        break;
      case GUI_ID_BUTTON1:  /* Delete window */
        _DeleteWindow();
        break;
      case GUI_ID_BUTTON2:  /* Show window   */
        _ShowWindow();
        break;
      case GUI_ID_BUTTON3:  /* Hide window   */
        _HideWindow();
        break;
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWinVideo
*/
static void _cbFrameWinVideo(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (_IsCompletelyVis) {
      GUI_SetBkColor(GUI_DARKGREEN);
      GUI_Clear();
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringAt("Completely visible", 5, 5);
    } else {
      GUI_SetBkColor(GUI_GRAY);
      GUI_Clear();
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringAt("Not completely visible", 5, 5);
    }
    break;
  case WM_NOTIFY_VIS_CHANGED:
    {
      WM_HWIN hWin;
      int IsCompletelyVis;
      hWin = WM_GetClientWindow(pMsg->hWin);
      IsCompletelyVis = WM_IsCompletelyVisible(hWin);
      if (_IsCompletelyVis != IsCompletelyVis) {
        _IsCompletelyVis = IsCompletelyVis;
        WM_InvalidateWindow(hWin);    /* Only required if content changes if partially hidden */
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWinTest
*/
static void _cbFrameWinTest(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_DARKRED);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  FRAMEWIN_Handle hWinVideo;
  BUTTON_Handle hBut;
  GUI_Init();
  WM_SetCallback(WM_HBKWIN, _cbBkWin);
  /* Create buttons */
  hBut = BUTTON_CreateEx(240,  5, 75, 18, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
  BUTTON_SetText(hBut, "Create win");
  hBut = BUTTON_CreateEx(240, 25, 75, 18, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
  BUTTON_SetText(hBut, "Delete win");
  hBut = BUTTON_CreateEx(240, 45, 75, 18, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
  BUTTON_SetText(hBut, "Show win");
  hBut = BUTTON_CreateEx(240, 65, 75, 18, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
  BUTTON_SetText(hBut, "Hide win");
  /* Create framewin video */
  hWinVideo = FRAMEWIN_CreateEx(5, 5, 170, 120, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Video window", _cbFrameWinVideo);
  FRAMEWIN_SetClientColor(hWinVideo, GUI_INVALID_COLOR);
  /* Create test windows */
  _CreateWindow();
  _CreateWindow();
  _CreateWindow();
  while (1) {
    GUI_Delay(1000);
  }
}

/**************************** end of file ***************************/
