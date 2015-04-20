/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : Dialog_All.c
Purpose     : Example demonstrating DIALOG and widgets
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "GUI.H"
#include "DIALOG.h"

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
* This table conatins the info required to create the dialog.
* It has been created manually, but could also be created by a GUI-builder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Dialog", 0,              10,  10, 180, 230, FRAMEWIN_CF_MOVEABLE, 0  },
  { BUTTON_CreateIndirect,   "OK",     GUI_ID_OK,     100,  5,   60,  20 },
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL, 100,  30,  60,  20 },
  { TEXT_CreateIndirect,     "LText",  0,              10,  55,  48,  15, TEXT_CF_LEFT  },
  { TEXT_CreateIndirect,     "RText",  0,              10,  80,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT0,   60,  55, 100,  15, 0, 50 },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT1,   60,  80, 100,  15, 0, 50 },
  { TEXT_CreateIndirect,     "Hex",    0,              10, 100,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT2,   60, 100, 100,  15, 0, 6 },
  { TEXT_CreateIndirect,     "Bin",    0,              10, 120,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT3,   60, 120, 100,  15 },
  { LISTBOX_CreateIndirect,  NULL,     GUI_ID_LISTBOX0,10,  10,  60,  40 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK0,  10, 140,   0,   0 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK1,  30, 140,   0,   0 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER0, 60, 140, 100,  20 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER1, 10, 170, 150,  30 }
};


/*********************************************************************
*
*              Initializers for listbox
*/
static const GUI_ConstString _apListBox[] = {
  "English", "Deutsch", "Fran鏰is", "Japanese", "Italiano", NULL
};

/*********************************************************************
*
*       static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hEdit0, hEdit1, hEdit2, hEdit3, hListBox;
  WM_HWIN hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
      /* Get window handles for all widgets */
      hEdit0   = WM_GetDialogItem(hWin, GUI_ID_EDIT0);
      hEdit1   = WM_GetDialogItem(hWin, GUI_ID_EDIT1);
      hEdit2   = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
      hEdit3   = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
      hListBox = WM_GetDialogItem(hWin, GUI_ID_LISTBOX0);
      /* Initialize all widgets */
      EDIT_SetText(hEdit0, "EDIT widget 0");
      EDIT_SetText(hEdit1, "EDIT widget 1");
      EDIT_SetTextAlign(hEdit1, GUI_TA_LEFT);
      EDIT_SetHexMode(hEdit2, 0x1234, 0, 0xffff);
      EDIT_SetBinMode(hEdit3, 0x1234, 0, 0xffff);
      LISTBOX_SetText(hListBox, _apListBox);
      WM_DisableWindow (WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK0));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      SLIDER_SetWidth( WM_GetDialogItem(hWin, GUI_ID_SLIDER0), 5);
      SLIDER_SetValue( WM_GetDialogItem(hWin, GUI_ID_SLIDER1), 50);
      SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
      break;
    case WM_KEY:
      switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
      case GUI_KEY_ESCAPE:
        GUI_EndDialog(hWin, 1);
        break;
      case GUI_KEY_ENTER:
        GUI_EndDialog(hWin, 0);
        break;
      }
      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:    /* React only if released */
          if (Id == GUI_ID_OK) {          /* OK Button */
            GUI_EndDialog(hWin, 0);
          }
          if (Id == GUI_ID_CANCEL) {      /* Cancel Button */
            GUI_EndDialog(hWin, 1);
          }
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
extern GUI_FONT GUI_Font8_2;
extern GUI_FONT GUI_Font4x6_2;

void MainTask_dialog324(void) {
  GUI_Init();
  WM_SetDesktopColor(GUI_RED);      /* Automacally update desktop window */
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */

// 2006-11-19 16:32:46
//  GUI_SetFont(&GUI_Font4x6_2);
/* 2006-11-20 21:25:14
  GUI_SetFont(&GUI_Font8x16x2x2);
  GUI_DispString("Hello\n");
  GUI_DispString("8x16x2x2\n");//回车不对多空了一行，使下面的显示"2x2"隔了一
  GUI_DispString("2x2");
  while(1);
*/
  GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
}

