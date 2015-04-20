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
#include "FRAMEWIN_Private.h"
#include "MESSAGEBOX.h"
#include "MENU.h"

#include "FiveChess.h"

#define NEW_GAME_ID		0x100
#define EXIT_GAME_ID	0x101
#define ABOUT_GAME_ID	0x102
#define ABOUT_LAN_ID	0x103
#define MAIN_GAME_ID	0x104
#define MAIN_HELP_ID	0x105


#define GAME_RESID			0
#define GAME_HELP_RESID		1
#define GAME_NEW_RESID		2
#define GAME_EXIT_RESID		3
#define GAME_ABOUT_RESID	4
#define GAME_LAN_RESID		5
#define GAME_MEWIN_RESID	3
#define GAME_CWIN_RESID		4

typedef struct{
	MENU_ITEM_DATA MenuData;
	int ResID;
} MENU_ITEM_DATA_EXT;


//所有用到的图片...
void* qipanBmp, *whiteBmp, *blackBmp;

extern const GUI_BITMAP bmwhite, bmqipan, bmblack;

static void WinDialog_cbCallback(WM_MESSAGE * pMsg);


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "FiveChess", 0,              10,  10, 400-20, 420-25, FRAMEWIN_SF_MOVEABLE, 0  },
};


//#ifdef LANGUAGE_EN
int CurrentLan;
static char* language[][2] = {
	{"Game", " 游戏"},
	{"Help", " 帮助"},
	{"New Game", " 新建游戏 "},
	{"Exit Game", " 退出游戏 "},
	{"About Game", " 关于 "},
	{"language", " 语言 "},
	{"You Win!", " 你胜了! "},
	{"Computer Win!", " 电脑胜了! "},
};


static  MENU_ITEM_DATA_EXT MainMenu[] = {
	{{"Game", MAIN_GAME_ID, 0, 0}, GAME_RESID},
	{{"Help", MAIN_HELP_ID, 0, 0}, GAME_HELP_RESID}
};

static  MENU_ITEM_DATA_EXT GameMenu[] = {
	{{"New Game", NEW_GAME_ID, 0, 0}, GAME_NEW_RESID},
	{{0, 0,  MENU_IF_SEPARATOR, 0}, -1},
	{{"Exit Game", EXIT_GAME_ID, 0, 0}, GAME_EXIT_RESID}
};

static  MENU_ITEM_DATA_EXT HelpMenu[] = {
	{{"About Game", ABOUT_GAME_ID, 0, 0}, GAME_ABOUT_RESID},
	{{"language", ABOUT_LAN_ID, 0, 0}, GAME_LAN_RESID}
};
/*
static const MENU_ITEM_DATA MainMenu[] = {
	{"Game", 0, 0, 0},
	{"Help", 0, 0, 0},
};

static const MENU_ITEM_DATA GameMenu[] = {
	{"New Game", NEW_GAME_ID, 0, 0},
	{0, 0,  MENU_IF_SEPARATOR, 0},
	{"Exit Game", EXIT_GAME_ID, 0, 0},
};

static const MENU_ITEM_DATA HelpMenu[] = {
	{"About Game", ABOUT_GAME_ID, 0, 0},
};
typedef struct {
  const char* pText;
  U16         Id;
  U16         Flags;
  MENU_Handle hSubmenu;
} MENU_ITEM_DATA;
*/


WM_CALLBACK* OldCallback;


extern GUI_FONT GUI_FontHZ_SimSun_13;

#include "string.h"
void ChangeLanguage(int index)
{
	int i = 0, j = 0;
	for(j = 0; j < GUI_COUNTOF(HelpMenu); j++){		
		for(i = 0; i < GUI_COUNTOF(language); i++){
			if(i == HelpMenu[j].ResID){
				HelpMenu[j].MenuData.pText = language[i][index];
				break;
			}
		}
	}
	for(j = 0; j < GUI_COUNTOF(GameMenu); j++){		
		for(i = 0; i < GUI_COUNTOF(language); i++){
			if(i == GameMenu[j].ResID){
				GameMenu[j].MenuData.pText = language[i][index];
				break;
			}
		}
	}
	for(j = 0; j < GUI_COUNTOF(MainMenu); j++){		
		for(i = 0; i < GUI_COUNTOF(language); i++){
			if(i == MainMenu[j].ResID){
				MainMenu[j].MenuData.pText = language[i][index];
				break;
			}
		}
	}
}

static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu,  char* pText, U16 Id, U16 Flags) 
{
  MENU_ITEM_DATA Item;
  Item.pText    = pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}

static MENU_Handle _CreateMenu(WM_HWIN hWin) 
{
	int i = 0;
	MENU_Handle hMenu, hMenuGame, hMenuHelp;
//	MENU_SetDefaultFont(&GUI_Font10_1);
	hMenuGame = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
	for(i = 0; i < GUI_COUNTOF(GameMenu); i++){
		MENU_AddItem(hMenuGame, &GameMenu[i].MenuData);
	}
	hMenuHelp = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
	for(i = 0; i < GUI_COUNTOF(HelpMenu); i++){
		MENU_AddItem(hMenuHelp, &HelpMenu[i].MenuData);
	}
	// Create main menu
	hMenu = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_HORIZONTAL, 0);
	MainMenu[0].MenuData.hSubmenu = hMenuGame;
	MainMenu[1].MenuData.hSubmenu = hMenuHelp;
	for(i = 0; i < GUI_COUNTOF(MainMenu); i++){
		MENU_AddItem(hMenu, &MainMenu[i].MenuData);
	}
	return hMenu;
}

static void _MessageBox(const char* pText, const char* pCaption) 
{
  WM_HWIN hWin;
  hWin = MESSAGEBOX_Create(pText, pCaption, GUI_MESSAGEBOX_CF_MOVEABLE);
  WM_MakeModal(hWin);
  GUI_ExecCreatedDialog(hWin);
}

MENU_Handle hMainMenu, hMainMenu2;

static void OnMenuDeal(WM_MESSAGE* pMsg, WM_HWIN hWin)
{
	MENU_MSG_DATA* pData = (MENU_MSG_DATA*)pMsg->Data.p;
	MENU_Handle    hMenu = pMsg->hWinSrc;
	switch (pData->MsgType)
	{
	case MENU_ON_ITEMSELECT:
		switch (pData->ItemId){
		case NEW_GAME_ID:
			//	NewGame();
			WM_SelectWindow(WM_GetFirstChild(hWin));
			Game_Init();
			break;
		case EXIT_GAME_ID: 
			GUI_EndDialog(hWin, 1);			
			//	ExitGame();
			break;
		case ABOUT_GAME_ID:
			_MessageBox("UCGUI FiveChess Game!\nEmail:ucgui@163.com\nhttp://www.ucgui.com", "About");
			break;
		case ABOUT_LAN_ID:
			if(!CurrentLan){
				FRAMEWIN_AddMenu(hWin, hMainMenu2);
				CurrentLan = 1;
			}
			else{
				CurrentLan = 0; 
				FRAMEWIN_AddMenu(hWin, hMainMenu);
			}

		//	_MessageBox("UCGUI FiveChess Game!\nEmail:ucgui@163.com\nhttp://www.ucgui.com", "About");
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void _cbCallback(WM_MESSAGE * pMsg) 
{
	GUI_PID_STATE* State;
	WM_HWIN hWinDlg;
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hDialog = 0;
	//  FRAMEWIN_Obj* pObj;	
	switch (pMsg->MsgId) {
		
	case WM_TOUCH:
		State = (GUI_PID_STATE*) pMsg->Data.p;
		if(State && !State->Pressed)
		{
			//The chess is paint to the dialog's client dlg, so select it, 
			//if select hWin will not be show chess...
			WM_SelectWindow(WM_GetFirstChild(hWin));
			
			if(My_work(State->x, State->y)==0)//如返回的是0，则说明玩家没有走，
				return;//则六即返回，让玩家重新输入		
			if(Check_win()==1)//玩家赢了
			{
				hWinDlg = MESSAGEBOX_Create("   You Win!!!   ", "Win", GUI_MESSAGEBOX_CF_MOVEABLE);
				WM_MakeModal(hWinDlg);
				OldCallback = WM_SetCallback(WM_GetFirstChild(hWinDlg), WinDialog_cbCallback);
				GUI_ExecCreatedDialog(hWinDlg);
			}
			Computer_think();
			Computer_work();
			if(Check_win()==2)// 电脑赢了
			{
				hWinDlg = MESSAGEBOX_Create("   Computer Win!!!   ", "Failed", GUI_MESSAGEBOX_CF_MOVEABLE);
				WM_MakeModal(hWinDlg);
				OldCallback = WM_SetCallback(WM_GetFirstChild(hWinDlg), WinDialog_cbCallback);
				GUI_ExecCreatedDialog(hWinDlg);
			}
		}
		break;
	case WM_MENU:
		OnMenuDeal(pMsg, hWin);
		break;
	case WM_PAINT:
		Update_all();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void WinDialog_cbCallback(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	//Add by ucgui 20051129. Because the parent hwnd should be close.
	WM_Obj *pWin;
	pWin = WM_H2P(hWin);  
	hWin = pWin->hParent;	
	
	switch (pMsg->MsgId) {
    case WM_KEY:
		{
			int Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			switch (Key) {
			case GUI_KEY_ESCAPE:
				Game_Init();
				GUI_EndDialog(hWin, 1);             /* End dialog with return value 1 if <ESC> is pressed */
				break;
			case GUI_KEY_ENTER:
				Game_Init();
				GUI_EndDialog(hWin, 0);             /* End dialog with return value 0 if <ENTER> is pressed */
				break;
			}
		}
		break;
    case WM_NOTIFY_PARENT:
		{
			int NCode = pMsg->Data.v;             /* Get notification code */
			int Id    = WM_GetId(pMsg->hWinSrc);  /* Get control ID */
			switch (NCode) {
			case WM_NOTIFICATION_RELEASED:      /* React only if released */
				if (Id == GUI_ID_OK) {
					Game_Init();
					GUI_EndDialog(hWin, 0);         /* End dialog with return value 0 if OK */
				}
				break;
			}
		}
		break;
    default:
		OldCallback(pMsg);	//call old callback proc...		
		// WM_DefaultProc(pMsg);
	}
}

void MainTask_Chess(void)
{
	int i = 0;
	WM_HWIN hWin;
//	MENU_Handle hMainMenu, hMainMenu2;
	GUI_Init();
	WM_SetDesktopColor(GUI_RED);      /* Automacally update desktop window */	
	WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
	GUI_CURSOR_Select(&GUI_CursorCrossLI);  
	GUI_CURSOR_Show(); 
	qipanBmp = (void*)&bmqipan;
	whiteBmp = (void*)&bmwhite;
	blackBmp = (void*)&bmblack;

	GUI_SetBkColor(GUI_RED);
	GUI_Clear();
	while(1);
	Game_Init();	
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, WM_HBKWIN, 0, 0); 
	MENU_SetDefaultFont(&GUI_FontHZ_SimSun_13);
	hMainMenu = _CreateMenu(hWin);
	ChangeLanguage(1);
	hMainMenu2 = _CreateMenu(hWin);
	FRAMEWIN_AddMenu(hWin, hMainMenu2);
    GUI_ExecCreatedDialog(hWin);
}

