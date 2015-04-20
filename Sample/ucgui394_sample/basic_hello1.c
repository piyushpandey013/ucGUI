/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : BASIC_Hello1.c
Purpose     : Simple demo drawing "Hello world"
----------------------------------------------------------------------
*/

#include "GUI.h"

/*******************************************************************
*
*       MainTask
*
********************************************************************
*/

void MainTask(void) {
  int i=0;
/*
 ToDo:  Make sure hardware is initilized first!!
*/
  GUI_Init();
  GUI_DispString("Hello world!");
  while(1) {
    GUI_DispDecAt( i++, 20,20,4);
    if (i>9999) i=0;
  }
}
