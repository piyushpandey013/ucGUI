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
File        : WIDGET_Multipage.c
Purpose     : Demonstrates the use of a MULTIPAGE widget
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include "GUI.h"
#include "MULTIPAGE.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialogs.
* It has been created manually, but could also be created by a GUI-builder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = {
  { WINDOW_CreateIndirect,    "Dialog 1", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { BUTTON_CreateIndirect,    "Button",   GUI_ID_BUTTON0,      5,  30,  80,  20, 0},
  { TEXT_CreateIndirect,      "Dialog 1", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
  { WINDOW_CreateIndirect,    "Dialog 2", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { CHECKBOX_CreateIndirect,  "",         GUI_ID_CHECK0,       5,  30,   0,   0, 0},
  { CHECKBOX_CreateIndirect,  "",         GUI_ID_CHECK1,       5,  50,   0,   0, 0},
  { TEXT_CreateIndirect,      "Select 0", GUI_ID_TEXT0,       25,  30,  50,  15, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,      "Select 1", GUI_ID_TEXT1,       25,  50,  50,  15, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,      "Dialog 2", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate3[] = {
  { WINDOW_CreateIndirect,    "Dialog 3", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { RADIO_CreateIndirect,     "",         GUI_ID_RADIO0,       5,  30,   0,   0, 0, 3},
  { TEXT_CreateIndirect,      "Option 1", GUI_ID_TEXT0,       25,  30,  50,  15, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,      "Option 2", GUI_ID_TEXT1,       25,  50,  50,  15, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,      "Option 3", GUI_ID_TEXT2,       25,  70,  50,  15, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,      "Dialog 3", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate4[] = {
  { WINDOW_CreateIndirect,    "Dialog 4", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { MULTIEDIT_CreateIndirect, "Text",     GUI_ID_MULTIEDIT0,   5,  30, 200,  40  },
  { TEXT_CreateIndirect,      "Dialog 4", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate5[] = {
  { WINDOW_CreateIndirect,    "Dialog 5", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { SLIDER_CreateIndirect,    "",         GUI_ID_SLIDER0,      5,  30, 200,  40  },
  { TEXT_CreateIndirect,      "Dialog 5", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate6[] = {
  { WINDOW_CreateIndirect,    "Dialog 6", 0,                   0,   0, 260, 100, FRAMEWIN_CF_MOVEABLE },
  { SCROLLBAR_CreateIndirect, "",         GUI_ID_SCROLLBAR0,   5,  30, 200,  40  },
  { TEXT_CreateIndirect,      "Dialog 6", 0,                   5,  10,  50,  20, TEXT_CF_LEFT }
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbDialog1
*/
static void _cbDialog1(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialog2
*/
static void _cbDialog2(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialog3
*/
static void _cbDialog3(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialog4
*/
static void _cbDialog4(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg, hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hItem = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);
    MULTIEDIT_SetText(hItem, "MULTIEDIT widget");
    MULTIEDIT_SetInsertMode(hItem, 1);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialog5
*/
static void _cbDialog5(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialog6
*/
static void _cbDialog6(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbBkWindow
*
* Purpose:
*   Callback routine of the background window, shows the sample title and draws the background
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("WIDGET_Multipage - Sample", 160, 5);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWin
*
* Purpose:
*   Callback routine of the frame window behind the MULTIPAGE widget
*/
static void _cbFrameWin(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_GREEN);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _DemoMultiPage
*
* Purpose:
*   Demonstrates the use of a multipage widget
*/
static void _DemoMultiPage(void) {
  WM_HWIN hMultiPage, hFrameWin, hDialog;
  /* Enable use of memory devices */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_EnableMemdev(WM_HBKWIN);
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow); /* Use own callback routine for background window */
  /* Create the frame window */
  hFrameWin = FRAMEWIN_Create("FrameWindow", &_cbFrameWin, WM_CF_SHOW, 40, 44, 240, 152);
  FRAMEWIN_SetActive(hFrameWin, 1);
  FRAMEWIN_SetMoveable(hFrameWin, 1);
  /* Create the MULTIPAGE widget */
  hMultiPage = MULTIPAGE_CreateEx(7, 6, 220, 120, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, 0);
  GUI_Delay(500);
  /* Create and attache the MULTIPAGE dialog windows */
  hDialog = GUI_CreateDialogBox(_aDialogCreate1, 
                                GUI_COUNTOF(_aDialogCreate1), 
                                &_cbDialog1, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 1");
  GUI_Delay(500);
  hDialog = GUI_CreateDialogBox(_aDialogCreate2, 
                                GUI_COUNTOF(_aDialogCreate2), 
                                &_cbDialog2, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 2");
  GUI_Delay(500);
  hDialog = GUI_CreateDialogBox(_aDialogCreate3, 
                                GUI_COUNTOF(_aDialogCreate3), 
                                &_cbDialog3, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 3");
  GUI_Delay(500);
  hDialog = GUI_CreateDialogBox(_aDialogCreate4, 
                                GUI_COUNTOF(_aDialogCreate4), 
                                &_cbDialog4, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 4");
  GUI_Delay(500);
  hDialog = GUI_CreateDialogBox(_aDialogCreate5, 
                                GUI_COUNTOF(_aDialogCreate5), 
                                &_cbDialog5, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 5");
  GUI_Delay(500);
  hDialog = GUI_CreateDialogBox(_aDialogCreate6, 
                                GUI_COUNTOF(_aDialogCreate6), 
                                &_cbDialog6, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "Page 6");
  GUI_Delay(500);
  /* Demonstrate the use of MULTIPAGE_SetAlign */
  MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_RIGHT);
  GUI_Delay(500);
  MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_RIGHT | MULTIPAGE_ALIGN_BOTTOM);
  GUI_Delay(500);
  MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_LEFT | MULTIPAGE_ALIGN_BOTTOM);
  while (1) {
    GUI_Exec();
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
  _DemoMultiPage();
}
