/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : WIDGET_FrameWin.c
Purpose     : Example demonstrating the use of a FRAMEWIN widget
----------------------------------------------------------------------
*/

#include <stddef.h>
#include <string.h>
#include "GUI.h"
#include "FRAMEWIN.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define SPEED   1200

#define MSG_CHANGE_MAIN_TEXT (WM_USER + 0)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static FRAMEWIN_Handle  _hFrame;
static WM_CALLBACK*     _pcbOldFrame;
static char             _acMainText[100];
static int              _LockClose = 1;

/*******************************************************************
*
*       Static code
*
********************************************************************
*/

/*******************************************************************
*
*       _ChangeMainText
*
*  Sends a message to the background window and invalidate it, so
*  the callback of the background window display the new text.
*/
static void _ChangeMainText(char* pStr, int Delay) {
  WM_MESSAGE Message;
  Message.MsgId  = MSG_CHANGE_MAIN_TEXT;
  Message.Data.p = pStr;
  GUI_Delay(Delay);
  WM_SendMessage(WM_HBKWIN, &Message);
  WM_InvalidateWindow(WM_HBKWIN);
  GUI_Delay(Delay/3);
}

/*******************************************************************
*
*       _cbChild
*/
static void _cbChild(WM_MESSAGE * pMsg) {
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Handle the paint message */
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_Clear();
    GUI_DispStringHCenterAt("Client window", 
                            WM_GetWindowSizeX(hWin) / 2, 
                            WM_GetWindowSizeY(hWin) / 2);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _cbFrame
*/
static void _cbFrame(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      int Id = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      if (Id == GUI_ID_CLOSE) {
        if (_LockClose) {
          return;
        }
        _hFrame = 0;
      }
    }
    break;
  }
  if (_pcbOldFrame) {
    (*_pcbOldFrame)(pMsg);
  }
}

/*******************************************************************
*
*       _cbBkWindow

  This callback is necessary to redraw the background when
  frame window is moved
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case MSG_CHANGE_MAIN_TEXT:
      strcpy(_acMainText, pMsg->Data.p);
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case WM_PAINT:
      GUI_SetBkColor(GUI_BLACK);
      GUI_Clear();
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_Font24_ASCII);
      GUI_DispStringHCenterAt("WIDGET_FrameWin - Sample", 160, 5);
      GUI_SetFont(&GUI_Font8x16);
      GUI_DispStringHCenterAt(_acMainText, 160, 40);
      GUI_SetFont(&GUI_Font6x8);
      GUI_DispStringHCenterAt("The function FRAMEWIN_Create creates both the\n"
                              "frame window and the client window.", 160, 190);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _DemoFramewin

  Creates the frame window and sets the callback for frame, child
  and background window
*/
static void _DemoFramewin(void) {
  int i;
  char acInfoText[] = "-- sec to play with window";
  WM_HWIN hChild;
  /* Set callback for background window */
  WM_SetCallback(WM_HBKWIN, _cbBkWindow);
  /* Create frame window */
  _ChangeMainText("FRAMEWIN_Create", SPEED);
  _hFrame = FRAMEWIN_Create("Frame window", 0, WM_CF_SHOW, 50, 75, 220, 100);
  /* Set callback for frame window */
  _pcbOldFrame = WM_SetCallback(_hFrame, _cbFrame);
  /* Get the handle of the child window */
  hChild = WM_GetClientWindow(_hFrame);
  /* Set the callback of the child window */
  WM_SetCallback(hChild, _cbChild);
  /* Set moveable */
  FRAMEWIN_SetMoveable(_hFrame, 1);
  /* Create buttons */
  FRAMEWIN_AddCloseButton(_hFrame, 0, 0);
  FRAMEWIN_AddMaxButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0);
  FRAMEWIN_AddMinButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);
  /* Modify frame window attributes */
  _ChangeMainText("FRAMEWIN_SetActive", SPEED);
  FRAMEWIN_SetActive(_hFrame, 1);
  _ChangeMainText("FRAMEWIN_SetFont", SPEED);
  FRAMEWIN_SetFont(_hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTitleHeight(_hFrame, 20);
  _ChangeMainText("FRAMEWIN_SetTextColor", SPEED);
  FRAMEWIN_SetTextColor(_hFrame, GUI_YELLOW);
  _ChangeMainText("FRAMEWIN_SetTextAlign", SPEED);
  FRAMEWIN_SetTextAlign(_hFrame, GUI_TA_HCENTER);
  _ChangeMainText("FRAMEWIN_Minimize", SPEED);
  FRAMEWIN_Minimize(_hFrame);
  _ChangeMainText("FRAMEWIN_Maximize", SPEED);
  FRAMEWIN_Maximize(_hFrame);
  _ChangeMainText("FRAMEWIN_Restore", SPEED);
  FRAMEWIN_Restore(_hFrame);
  _ChangeMainText("FRAMEWIN_SetTitleVis", SPEED);
  for (i = 0; i < 5; i++) {
    FRAMEWIN_SetTitleVis(_hFrame, 0);
    GUI_Delay(200);
    FRAMEWIN_SetTitleVis(_hFrame, 1);
    GUI_Delay(200);
  }
  /* Time to play with frame window */
  _LockClose = 0;
  for (i = 250; (i > 0) && _hFrame; i--) {
    acInfoText[0] = '0' + ((i + 9) / 100);
    acInfoText[1] = '0' + (((i + 9) / 10) % 10);
    _ChangeMainText(acInfoText, 0);
    GUI_Delay(100);
  }
  if (_hFrame) {
    _ChangeMainText("FRAMEWIN_Delete", SPEED);
    FRAMEWIN_Delete(_hFrame);
  } else {
    _ChangeMainText("", 50);
  }
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates the use of a FRAMEWIN widget
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  WM_EnableMemdev(WM_HBKWIN);
  while(1) {
  	_DemoFramewin();
  }
}

