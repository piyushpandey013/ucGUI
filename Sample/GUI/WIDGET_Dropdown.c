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
File        : WIDGET_Dropdown.c
Purpose     : Example demonstrating the DROPDOWN widget
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "FRAMEWIN.h"
#include "DROPDOWN.h"
#include "LISTBOX.h"
#include <stddef.h>

#define SPEED 1000

/*******************************************************************
*
*       static code
*
********************************************************************
*/
/*******************************************************************
*
*       _ShowSeveralFunctions
*/
static void _ShowSeveralFunctions(DROPDOWN_Handle hDropDown) {
  int NumEntries, i, Key = 0, Cnt = 15;
  char ac[] = "-- sec to play with dropdown control";
  /* Set focus */
  GUI_DispStringAtCEOL("WM_SetFocus", 5, 55);
  GUI_Delay(SPEED * 0.9);
  WM_SetFocus(hDropDown);
  GUI_Delay(SPEED * 0.7);
  /* Add strings */
  GUI_DispStringAtCEOL("DROPDOWN_AddString", 5, 55);
  GUI_Delay(SPEED * 0.8);
  DROPDOWN_AddString(hDropDown, "English");
  DROPDOWN_AddString(hDropDown, "Deutsch");
  DROPDOWN_AddString(hDropDown, "Français");
  DROPDOWN_AddString(hDropDown, "Japanese");
  DROPDOWN_AddString(hDropDown, "Italiano");
  DROPDOWN_AddString(hDropDown, "Español");
  DROPDOWN_AddString(hDropDown, "Other language ...");
  GUI_Delay(SPEED * 0.6);
  /* Increment selection */
  GUI_DispStringAtCEOL("DROPDOWN_IncSel", 5, 55);
  GUI_Delay(SPEED);
  NumEntries = DROPDOWN_GetNumItems(hDropDown);
  for (i = 0; i < (NumEntries - 2); i++) {
    DROPDOWN_IncSel(hDropDown);
    GUI_Delay(SPEED / 6);
	}
  GUI_Delay(SPEED / 4);
  /* Expand dropdown */
  GUI_DispStringAtCEOL("DROPDOWN_Expand", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_Expand(hDropDown);
  GUI_Delay(SPEED * 0.75);
  /* Add scrollbar */
  GUI_DispStringAtCEOL("DROPDOWN_SetAutoScroll", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_SetAutoScroll(hDropDown, 1);
  GUI_Delay(SPEED * 0.75);
  /* Set font */
  GUI_DispStringAtCEOL("DROPDOWN_SetFont", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_SetFont(hDropDown, &GUI_Font16B_1);
  GUI_Delay(SPEED * 0.75);
  /* Set text color */
  GUI_DispStringAtCEOL("DROPDOWN_SetTextColor", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_SetTextColor(hDropDown, 0, 0x00BB00);
  DROPDOWN_SetTextColor(hDropDown, 2, GUI_BLACK);
  GUI_Delay(SPEED * 0.75);
  /* Set background color */
  GUI_DispStringAtCEOL("DROPDOWN_SetBkColor", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_SetBkColor(hDropDown, 0, GUI_YELLOW);
  DROPDOWN_SetBkColor(hDropDown, 2, GUI_RED);
  GUI_Delay(SPEED * 0.75);
  /* Delete item */
  GUI_DispStringAtCEOL("DROPDOWN_DeleteItem", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_DeleteItem(hDropDown, 5);
  GUI_Delay(SPEED * 0.75);
  /* Collapse dropdown */
  GUI_DispStringAtCEOL("DROPDOWN_Collapse", 5, 55);
  GUI_Delay(SPEED);
  DROPDOWN_Collapse(hDropDown);
  GUI_Delay(SPEED * 0.75);
  /* Decrement selection */
  GUI_DispStringAtCEOL("DROPDOWN_DecSel", 5, 55);
  GUI_Delay(SPEED);
  NumEntries = DROPDOWN_GetNumItems(hDropDown);
  for (i = 0; i < (NumEntries - 2); i++) {
    DROPDOWN_DecSel(hDropDown);
    GUI_Delay(SPEED / 6);
	}
  GUI_Delay(SPEED / 4);
  /* Let user play with dropdown control */
  GUI_DispStringAtCEOL("", 5, 55);
  while (!Key && (Cnt > 0)) {
    ac[0] = '0' + (Cnt / 10);
    ac[1] = '0' + (Cnt-- % 10);
    GUI_DispStringAtCEOL(ac, 5, 40);
    GUI_Delay(1000);
    Key = GUI_GetKey();
  }
  /* Delete dropdown widget */
  GUI_DispStringAtCEOL("DROPDOWN_Delete", 5, 55);
  GUI_Delay(SPEED * 1.1);
  DROPDOWN_Delete(hDropDown);
  GUI_Delay(SPEED * 0.75);
}

/*******************************************************************
*
*       _DemoDropDown
*/
static void _DemoDropDown(void) {
  LISTBOX_Handle hDropDown;
  /* Display titel */
  GUI_SetBkColor(0xB00000);
  GUI_SetColor(0xFFFFFF);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("Dropdown - Sample", 160, 5);
  GUI_Delay(SPEED / 2);
  /* Create listbox */
  GUI_SetFont(&GUI_Font8x16);
  GUI_SetTextAlign(GUI_TA_LEFT);
  GUI_DispStringAtCEOL("using", 5, 40);
  GUI_DispStringAtCEOL("DROPDOWN_CreateEx", 5, 55);
  GUI_Delay(SPEED * 0.9);
  hDropDown = DROPDOWN_CreateEx(100, 80, 120, 115-50, WM_HBKWIN, WM_CF_SHOW, 0, 0);
  GUI_Delay(SPEED * 0.75);
  /* Show serveral functions of listbox */
  _ShowSeveralFunctions(hDropDown);
  /* Clear display */
  GUI_Clear();
  GUI_Delay(SPEED * 1.5);
}

/*******************************************************************
*
*       _DemoDropDownAsChild
*/
static void _DemoDropDownAsChild(void) {
  FRAMEWIN_Handle hFrame;
  LISTBOX_Handle hDropDown;
  /* Display titel */
  GUI_SetBkColor(0xB00000);
  GUI_SetColor(0xFFFFFF);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("Dropdown as child - Sample", 160, 5);
  GUI_Delay(SPEED / 2);
  /* Create framewin */
  GUI_SetFont(&GUI_Font8x16);
  GUI_SetTextAlign(GUI_TA_LEFT);
  GUI_DispStringAtCEOL("using", 5, 40);
  GUI_DispStringAtCEOL("FRAMEWIN_Create", 5, 55);
  GUI_Delay(SPEED);
  hFrame = FRAMEWIN_Create("DropDown", NULL, WM_CF_SHOW, 80, 80, 160, 140);
  FRAMEWIN_SetFont(hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetActive(hFrame, 1);
  GUI_Delay(SPEED * 0.75);
  /* Create listbox */
  GUI_DispStringAtCEOL("DROPDOWN_CreateEx", 5, 55);
  GUI_Delay(SPEED);
  hDropDown = DROPDOWN_CreateEx(20, 20, 120, 65, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, 0);
  GUI_Delay(SPEED * 0.75);
  /* Show serveral functions of listbox */
  _ShowSeveralFunctions(hDropDown);
  /* Delete framewin widget */
  GUI_DispStringAtCEOL("FRAMEWIN_Delete", 5, 55);
  GUI_Delay(SPEED);
  FRAMEWIN_Delete(hFrame);
  GUI_Delay(SPEED * 0.75);
  /* Clear display */
  GUI_Clear();
  GUI_Delay(SPEED * 1.5);
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates DROPDOWN widget
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  WM_SetDesktopColor(0xB00000);
  GUI_Exec();
  while(1) {
    _DemoDropDown();
    _DemoDropDownAsChild();
  }
}
