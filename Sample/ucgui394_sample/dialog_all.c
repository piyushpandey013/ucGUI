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
File        : Dialog_All.c
Purpose     : Example demonstrating DIALOG and widgets
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"

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
  { FRAMEWIN_CreateIndirect, "Dialog", 0,                 30,   5, 260, 230, FRAMEWIN_CF_MOVEABLE, 0  },
  { BUTTON_CreateIndirect,   "OK",     GUI_ID_OK,        100,   5,  60,  20 },
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,    100,  30,  60,  20 },
  { TEXT_CreateIndirect,     "LText",  0,                 10,  55,  48,  15, TEXT_CF_LEFT  },
  { TEXT_CreateIndirect,     "RText",  0,                 10,  80,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT0,      60,  55, 100,  15, 0, 50 },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT1,      60,  80, 100,  15, 0, 50 },
  { TEXT_CreateIndirect,     "Hex",    0,                 10, 100,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT2,      60, 100, 100,  15, 0, 6 },
  { TEXT_CreateIndirect,     "Bin",    0,                 10, 120,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT3,      60, 120, 100,  15 },
  { LISTBOX_CreateIndirect,  NULL,     GUI_ID_LISTBOX0,   10,  10,  60,  40 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK0,     10, 140,   0,   0 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK1,     30, 140,   0,   0 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER0,    60, 140, 180,  20 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER1,    10, 170, 230,  30 },
  { DROPDOWN_CreateIndirect,  NULL,    GUI_ID_DROPDOWN0, 170,  10,  80,  60, 0, 3  },
  { DROPDOWN_CreateIndirect,  NULL,    GUI_ID_DROPDOWN1, 170,  60,  80,  60, 0, 3  }
};


/*********************************************************************
*
*              Initializers for listbox
*/
static const GUI_ConstString _apListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", NULL
};

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_RED);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("DIALOG_All - Sample", 160, 5);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbCallback
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hEdit0, hEdit1, hEdit2, hEdit3, hListBox, hDropd0, hDropd1;
  WM_HWIN hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
      /* Get window handles for all widgets */
      hEdit0   = WM_GetDialogItem(hWin, GUI_ID_EDIT0);
      hEdit1   = WM_GetDialogItem(hWin, GUI_ID_EDIT1);
      hEdit2   = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
      hEdit3   = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
      hListBox = WM_GetDialogItem(hWin, GUI_ID_LISTBOX0);
      hDropd0  = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
      hDropd1  = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1);
      /* Initialize all widgets */
      EDIT_SetText(hEdit0, "EDIT widget 0");
      EDIT_SetText(hEdit1, "EDIT widget 1");
      EDIT_SetTextAlign(hEdit1, GUI_TA_LEFT);
      EDIT_SetHexMode(hEdit2, 0x1234, 0, 0xffffff);
      EDIT_SetBinMode(hEdit3, 0x1234, 0, 0xffff);
      LISTBOX_SetText(hListBox, _apListBox);
      WM_DisableWindow (WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK0));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      SLIDER_SetWidth( WM_GetDialogItem(hWin, GUI_ID_SLIDER0), 5);
      SLIDER_SetValue( WM_GetDialogItem(hWin, GUI_ID_SLIDER1), 50);
      SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
      DROPDOWN_AddString(hDropd0, "Item 0");
      DROPDOWN_AddString(hDropd0, "Item 1");
      DROPDOWN_AddString(hDropd0, "Item 2");
      DROPDOWN_AddString(hDropd1, "Item 0");
      DROPDOWN_AddString(hDropd1, "Item 1");
      DROPDOWN_AddString(hDropd1, "Item 2");
      DROPDOWN_AddString(hDropd1, "Item 3");
      DROPDOWN_AddString(hDropd1, "Item 4");
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
*       Demonstrates a dialog box
*
**********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
  while (1) {
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
    GUI_Delay(1000);
  }
}

