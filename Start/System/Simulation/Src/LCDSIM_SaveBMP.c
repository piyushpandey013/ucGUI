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
File        : LCDSIM_SaveBMP.c
Purpose     : BMP export for Simulator under Windows
---------------------------END-OF-HEADER------------------------------
*/

#include <windows.h>

#include "GUI.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _WriteByte2File
*
*  This function will be called by GUI_BMP_Serialize to write the
*  bytes to the file
*/
static void _WriteByte2File(U8 Data, void * p) {
  U32 nWritten;
  WriteFile(*(HANDLE *)p, &Data, 1, &nWritten, NULL);
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/

/*********************************************************************
*
*       LCDSIM_SaveBMPEx
*/
int LCDSIM_SaveBMPEx(const char * sFileName, int x0, int y0, int xSize, int ySize) {
  HANDLE hFile = CreateFile(sFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (hFile == INVALID_HANDLE_VALUE) {
    return 1;
  }
  GUI_BMP_SerializeEx(_WriteByte2File, x0, y0, xSize, ySize, &hFile);
  if (CloseHandle(hFile) == 0) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       LCDSIM_SaveBMP
*/
int LCDSIM_SaveBMP(const char * sFileName) {
  return LCDSIM_SaveBMPEx(sFileName, 0, 0, LCD_GetXSize(), LCD_GetYSize());
}

