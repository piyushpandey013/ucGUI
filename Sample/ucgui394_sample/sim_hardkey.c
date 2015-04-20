/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : SIM_Hardkey.c
Purpose     : Example demonstrating the HARDKEY simulation
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "SIM.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include <stddef.h>

/*******************************************************************
*
*       static variables
*
********************************************************************
*/

static const GUI_ConstString _aListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", "Español", NULL
};

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DemoHardkey

  Demonstrates HARDKEY simulation with a listbox
*/
static void _DemoHardkey(void) {
  LISTBOX_Handle hListBox;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringAt("Hardkey - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x8);
  GUI_DispStringAt("Up",   5, 54);
  GUI_DispStringAt("Down", 5, 78);
  /* Create the listbox */
  hListBox = LISTBOX_Create(_aListBox, 100, 100, 120, 75, WM_CF_SHOW);
  WM_SetFocus(hListBox);
  LISTBOX_SetFont(hListBox, &GUI_Font13B_1);
  SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
  /* Handle the listbox */
  while (1) {
    WM_Exec();
  }
  /* Delete listbox widget */
  LISTBOX_Delete(hListBox);
  GUI_Clear();
}

/*******************************************************************
*
*       _cbKeyUp

  Callback for the up-key
*/
static void _cbKeyUp(int key, int state) {
  GUI_StoreKeyMsg(GUI_KEY_UP, state);
};

/*******************************************************************
*
*       _cbKeyDown

  Callback for the down-key
*/
static void _cbKeyDown(int key, int state) {
  GUI_StoreKeyMsg(GUI_KEY_DOWN, state);
};

/*******************************************************************
*
*       MainTask
*
*       Demonstrates HARDKEY simulation
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  /* set callback routine for hardkey */
  SIM_HARDKEY_SetCallback(1, &_cbKeyUp);
  SIM_HARDKEY_SetCallback(2, &_cbKeyDown);
  /* start demo */
  _DemoHardkey();
}
