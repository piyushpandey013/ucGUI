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
File        : 2DGL_DrawJPG.c
Purpose     : Example for drawing JPG files
----------------------------------------------------------------------
*/

#include <windows.h>
#include <stdio.h>

#include "GUI.h"

/*******************************************************************
*
*       Static functions
*
********************************************************************
*/

/*******************************************************************
*
*       _ShowJPG
*
* Shows the contents of a JPEG file
*/
static void _ShowJPG(const char * sFilename) {
  int XSize, YSize, XPos, YPos;
  GUI_JPEG_INFO Info;
  DWORD NumBytesRead;
  HANDLE hFile = CreateFile(sFilename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  DWORD FileSize = GetFileSize(hFile, NULL);
  char * pFile = malloc(FileSize);
  ReadFile(hFile, pFile, FileSize, &NumBytesRead, NULL);
  CloseHandle(hFile);
  GUI_ClearRect(0, 60, 319, 239);
  GUI_JPEG_GetInfo(pFile, FileSize, &Info);
  XSize = Info.XSize;
  YSize = Info.YSize;
  XPos = (XSize > 320) ?  0 : 160 - (XSize / 2);
  YPos = (YSize > 180) ? 60 : 150 - (YSize / 2);
  if (!GUI_JPEG_Draw(pFile, FileSize, XPos, YPos)) {
    GUI_Delay(2000);
  }
  free(pFile);
}

/*******************************************************************
*
*       _DrawWindowsDirectoryBitmaps
*
* Iterates over all *.jpg-files of the windows directory
*/
static void _DrawWindowsDirectoryBitmaps(void) {
  char acPath[_MAX_PATH];
  char acMask[_MAX_PATH];
  char acFile[_MAX_PATH];
  WIN32_FIND_DATA Context;
  HANDLE hFind;

  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("DrawJPG - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x16);

  GetWindowsDirectory(acPath, sizeof(acPath));
  sprintf(acMask, "%s\\web\\wallpaper\\*.jpg", acPath);
  hFind = FindFirstFile(acMask, &Context);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      sprintf(acFile, "%s\\web\\wallpaper\\%s", acPath, Context.cFileName);
      GUI_DispStringAtCEOL(acFile, 5, 40);
      _ShowJPG(acFile);
    } while (FindNextFile(hFind, &Context));
  }
}

/*******************************************************************
*
*       MainTask
*
********************************************************************
*/

void MainTask_JPEG(void) {
  GUI_Init();
  while(1) {
    _DrawWindowsDirectoryBitmaps();
  }
}