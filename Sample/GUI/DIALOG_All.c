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
  "English", "Deutsch", "Fran鏰is", "Japanese", "Italiano", NULL
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

/*
//houhh 20061018...
void AutoEndDialog(GUI_TIMER_MESSAGE* TimeMsg)
{
	GUI_EndDialog(TimeMsg->Context, 1);
}

static void _cbCallback(WM_MESSAGE * pMsg) {
	WM_HWIN hWin = pMsg->hWin;
	static GUI_TIMER_HANDLE Timer1; //houhh 20061018...
	switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
		Timer1 = GUI_TIMER_Create((GUI_TIMER_CALLBACK*)AutoEndDialog, 1000*4, hWin, 0); //houhh 20061018...
		break;		
	case WM_DELETE: //houhh 20061018...
		GUI_TIMER_Delete(Timer1);
		break;	
    default:
		WM_DefaultProc(pMsg);
	}
}

*/


#include "windows.h"
//houhh 20061018...
void AutoEndDialog(GUI_TIMER_MESSAGE* TimeMsg)
{
//	GUI_EndDialog(TimeMsg->Context, 1);
//	MessageBox(NULL, "ttt", "ttttt", 0);
}

static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hEdit0, hEdit1, hEdit2, hEdit3, hListBox, hDropd0, hDropd1;
  WM_HWIN hWin = pMsg->hWin;
  static GUI_TIMER_HANDLE Timer1;	//houhh 20061018...

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
	  Timer1 = GUI_TIMER_Create((GUI_TIMER_CALLBACK*)AutoEndDialog, 1000*4, hWin, 0);	//houhh 20061018...

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
			      GUI_MessageBox("This text is shown\nin a message box",
                   "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);

          //  GUI_EndDialog(hWin, 0);
          }
          if (Id == GUI_ID_CANCEL) {      /* Cancel Button */
            GUI_EndDialog(hWin, 1);
          }
          break;
      }
      break;
	 case WM_DELETE:	//houhh 20061018...
	//	GUI_TIMER_Delete(Timer1);
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



void MainTask_Dialog(void) {

  GUI_Init();
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */


//  while (1) {
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
//    GUI_Delay(100);
//  }
}


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/


#include "BUTTON.h"
#include "BUTTON_Private.h"

#include "stdio.h"
#include "windows.h"


//extern GUI_CONST_STORAGE GUI_BITMAP bmgreenbutton;
//extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
//extern GUI_CONST_STORAGE GUI_BITMAP bmgreenbuttonlow;
const GUI_BITMAP * pBmp; //pointer to const bitmap

/*************************************************************
*
*       _cbBkWin - Callback for WM_HBKWIN (entire LCD)
*/
static void _cbBkWin(WM_MESSAGE* pMsg)
{
	int Id;
	WM_HWIN hWin_1;
	//BUTTON_Handle hBut;
	//   hWin_1=pMsg->MsgId;
	hWin_1=pMsg->hWin;	//houhh 20061019
	switch (pMsg->MsgId) {
	case WM_PAINT:
		GUI_SetColor(GUI_WHITE);
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_DispStringAt("Hello World!", 20, 30);

		//  GUI_DrawBitmap(&bmlogo, 25,41);
		break;
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
			Id = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			if (Id == GUI_ID_BUTTON0){
				//	GUI_EndDialog(hWin_1, 0);
				char buf[255];
				static int i = 0;
				sprintf(buf, "%d:Hello World!", i++);
				GUI_SetColor(GUI_RED);
			//	GUI_SetBkColor(GUI_WHITE);
				WM_SelectWindow(hWin_1);
				GUI_DispStringAt(buf, 20, 30);
				//	GUI_DispString("Hello World!");
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}
/*********************************************************************
*
*       _cbButton
*
* Purpose: 
*  1. Calls the owner draw function if the WM_PAINT message has been send
*  2. Calls the original callback for further messages
*  3. After processing the messages the function evaluates the pressed-state
*     if the WM_TOUCH message has been send
*/
//static void _cbButton(WM_MESSAGE *pMsg) {
// WM_HWIN hDlg;
// hDlg=pMsg->MsgId;
// if (pMsg->MsgId == WM_TOUCH) { 
//  
//  GUI_EndDialog(hDlg, 1);}
      
//}
/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/

void MainTask_FreeError(void)
{
	WM_HWIN  hWin_1/*,hWin_2*/;
	BUTTON_Handle hBut;
	GUI_Init();
	/* Use memory devices for all windows */
	WM_SetCreateFlags(WM_CF_MEMDEV);
	WM_EnableMemdev(WM_HBKWIN);
	WM_SetDesktopColor(GUI_GREEN);
	{
		hWin_1=WM_CreateWindow(0,0,320,240,WM_CF_SHOW, _cbBkWin, 0 );
		//GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);	
		/************************Create Button*************************/
		//   pBmp = &bmgreenbutton;
		//   hBut = BUTTON_CreateEx(120, 170, ((U16P) pBmp->XSize), ((U16P) pBmp->YSize),
		//							hWin_1, WM_CF_HIDE, 0, GUI_ID_BUTTON0);
		//houhh 20061019
		hBut = BUTTON_CreateEx(120, 170, 80, 20,
			hWin_1, WM_CF_HIDE, 0, GUI_ID_BUTTON0);	
		//   BUTTON_SetBitmap(hBut,BUTTON_BI_UNPRESSED,&bmgreenbutton);
		//   BUTTON_SetBitmap(hBut,BUTTON_BI_PRESSED,&bmgreenbuttonlow);
				
		WIDGET_SetEffect(hBut,&WIDGET_Effect_None);
		BUTTON_SetFocussable(hBut,0);
		WM_ShowWindow(hBut);
		//	WM_SetCallback(hBut, _cbButton);
		/************************Create Button*************************/
		//	hWin_2=WM_CreateWindow(0,0,320,240,WM_CF_SHOW, 0, 0 );
		//	GUI_Delay(100);	//houhh 20061019
	}

//houhh 20061019	
	while (1) {
		if (!GUI_Exec()) {
			GUI_X_WAIT_EVENT();      /* Wait for event (keyboard, mouse or whatever) */
		}
	}
}


/*******************************************************************
*
*       _cbWindow1
*/
static void _cbWindow1(WM_MESSAGE* pMsg) {
  int x, y;
  switch (pMsg->MsgId) {
  case WM_PAINT:
//    GUI_SetBkColor(_WindowColor1);
//    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    x = WM_GetWindowSizeX(pMsg->hWin);
    y = WM_GetWindowSizeY(pMsg->hWin);
    GUI_DispStringHCenterAt("Window 1", x / 2, (y / 2) - 12);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

void MainTask_button(void)
{
	int i;
	WM_HWIN Win1, Button1, Edit1;
	GUI_Init();
	Win1 = WM_CreateWindow(0,0,320,240,WM_CF_SHOW,NULL,0);
//	Win1 = WM_CreateWindow(0,0,320,240,WM_CF_SHOW,_cbWindow1,0);	//houhh 20061024...
	
	WM_SelectWindow(Win1);
	
    GUI_SetColor(GUI_WHITE); 
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font32_ASCII); 
	GUI_Clear(); 
//  GUI_Delay(200);
//	while(1);
	
	Button1 = BUTTON_CreateAsChild(110,20,240,40,Win1,GUI_ID_OK,WM_CF_SHOW); 
	
	
	BUTTON_SetText(Button1,"ADS"); 
	BUTTON_SetBkColor(Button1,0,GUI_YELLOW);
	WM_Paint(Button1); 
//	while(1);


	BUTTON_SetState(Button1,BUTTON_STATE_PRESSED);
	
	for (i=0;i<3000000;i++); 
	
	BUTTON_SetBkColor(Button1,1,GUI_LIGHTRED);  
	WM_Paint(Button1); 
	
	// Edit1 = EDIT_Create(200,200,300,40,GUI_ID_OK,12,WM_CF_SHOW);  //位置
	Edit1 = EDIT_CreateAsChild(200,200,100,40,Win1,GUI_ID_OK,WM_CF_SHOW,12);
	EDIT_SetText(Edit1,"1234");          //内容
	EDIT_SetBkColor(Edit1,1,GUI_GREEN);        //按下背色
	EDIT_SetTextColor(Edit1,0,GUI_RED);        //字体颜色
	EDIT_SetFont(Edit1,&GUI_Font24_ASCII);        //字体大小
	EDIT_SetTextAlign(Edit1,GUI_TA_HCENTER|GUI_TA_VCENTER);   //对齐方式
	WM_Paint(Edit1); 
//	while(1);
	
	while (1) {
		if (!GUI_Exec())
			GUI_X_ExecIdle();
	}
}


