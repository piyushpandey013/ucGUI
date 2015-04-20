/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : MT_MultiTasking.c
Purpose     : Example demonstrates MultiTasking capabilities of emWin
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"
#include "FRAMEWIN.h"
#include <stddef.h>

#if GUI_OS == 0
  #error Multitasking sample requires task awareness (#define GUI_OS 1)
#endif

/*******************************************************************
*
*       Define how to create a task and start multitasking
*
********************************************************************

  If not using embOS you have to change the hardware
  dependent macros to work with your OS

*/

#ifndef WIN32 
  #include "RTOS.h"    /* Definitions for embOS */
  #define CREATE_TASK(pTCB, pName, pFunc, Priority, pStack)  OS_CREATETASK(pTCB, pName, pFunc, Priority, pStack)
  #define START_MT()  OS_Terminate(0)
  #define Delay(t)    OS_Delay(t)
#else
  #include "SIM.h"     /* Definitions for the Win32 simulation */
  #define CREATE_TASK(pTCB, pName, pFunc, Priority, pStack)   SIM_CreateTask(pName, pFunc)
  #define START_MT()  SIM_Start()
  #define Delay(t)    SIM_Delay(t)
#endif

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _cbCallbackT0
*/
static int XPos;
static const char aText[] = "Moving text...";

static void _cbCallbackT0(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Handle the paint message */
    GUI_SetBkColor(GUI_RED);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_Clear();
    GUI_DispStringAt(aText, XPos, 0);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _cbCallbackT1
*/
static void _cbCallbackT1(WM_MESSAGE * pMsg) {
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Handle the paint message */
    GUI_SetBkColor(GUI_BLUE);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_Clear();
    GUI_DispStringHCenterAt("Moving window...", 
                            WM_GetWindowSizeX(hWin) / 2, 
                            WM_GetWindowSizeY(hWin) / 2);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _cbBackgroundWin
*/
static void _cbBackgroundWin(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Handle only the paint message */
    GUI_SetBkColor(0x00CC00);
    GUI_Clear();
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("emWinGSC - multitasking demo\n", 160, 5);
    GUI_SetFont(&GUI_Font13_1);
    GUI_DispStringAt("Scrolling text and moving windows without flickering", 5, 35);
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _Task_0
*/
static void _Task_0(void) {
  /* Create frame window */
  FRAMEWIN_Handle hFrameWin = FRAMEWIN_Create("Task 0",  NULL, WM_CF_SHOW | WM_CF_STAYONTOP,  0, 70, 200, 40);
  /* Create child window */
  WM_HWIN         hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), 
                                                     WM_CF_SHOW | WM_CF_MEMDEV,  _cbCallbackT0, 0);
  FRAMEWIN_SetActive(hFrameWin, 0);
  /* Make sure the right window is active... */
  WM_SelectWindow(hChildWin);
  /* ...and the right font is selected */
  GUI_SetFont(&GUI_FontComic24B_ASCII);
  while(1) {
    GUI_RECT Rect;
    int XLen = GUI_GetStringDistX(aText);   /* Get the length of the string */
    WM_GetClientRect(&Rect);                /* Get the size of the window */
    /* Show moving text */
    for (XPos = 0; XPos < (Rect.x1 - Rect.x0) - XLen; XPos++) {
      WM_InvalidateWindow(hChildWin);
      Delay(50);
    }
    for (; XPos >= 0; XPos--) {
      WM_InvalidateWindow(hChildWin);
      Delay(100);
    }
  }
}

/*******************************************************************
*
*       _Task_1
*/
static void _Task_1(void) {
  /* Create frame window */
  FRAMEWIN_Handle hFrameWin = FRAMEWIN_Create("Task 1", NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 
                                              20, 170, 200, 40);
  /* Create child window */
  WM_HWIN hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), WM_CF_SHOW | WM_CF_MEMDEV,
                                             _cbCallbackT1, 0);
  FRAMEWIN_SetActive(hFrameWin, 0);
  while(1) {
    int i;
    int nx = 80;
    int ny = 90;
    /* Move window continously */
    for (i = 0; i < ny; i++) {
      WM_MoveWindow(hFrameWin, 0, -2);
      Delay(50);
    }
    for (i = 0; i < nx; i++) {
      WM_MoveWindow(hFrameWin, 2, 0);
      Delay(50);
    }
    for (i = 0; i < ny; i++) {
      WM_MoveWindow(hFrameWin, 0, 2);
      Delay(50);
    }
    for (i = 0; i < nx; i++) {
      WM_MoveWindow(hFrameWin, -2, 0);
      Delay(50);
    }
  }
}

/*******************************************************************
*
*       _GUI_Task
*
  This task does the background processing.
  The MainTask job is to update invalid windows, but other things such as
  evaluating mouse or touch input may also be done.
*/
static void _GUI_Task(void) {
  while(1) {
    GUI_Exec();           /* Do the background work ... Update windows etc.) */
    GUI_X_ExecIdle();     /* Nothing left to do for the moment ... Idle processing */
  }
}


/*******************************************************************
*
*       MainTask
*
********************************************************************
*/

#ifndef WIN32
  /* Stacks */
  static OS_STACKPTR int Stack_0[600];
  static OS_STACKPTR int Stack_1[600];
  static OS_STACKPTR int Stack_2[600];
  static OS_TASK aTCB[3];               /* Task control blocks */
#endif

void MainTask(void) {
  /* Init GUI */
  GUI_Init();
  WM_SetCreateFlags(WM_CF_MEMDEV);              /* Use memory devices on all windows to avoid flicker */
  WM_SetCallback(WM_HBKWIN, _cbBackgroundWin);  /* Set callback for background window */
  /* Create tasks */
  CREATE_TASK(&aTCB[0], "Task_0",   _Task_0,   80, Stack_0);
  CREATE_TASK(&aTCB[1], "Task_1",   _Task_1,   60, Stack_1);
  CREATE_TASK(&aTCB[2], "GUI_TASK", _GUI_Task,  1, Stack_2);
  /* Start multitasking */
  START_MT();
}
