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
File        : LCDSIM_SaveSBMP.c
Purpose     : SBMP export for Simulator under Windows
---------------------------END-OF-HEADER------------------------------
*/

#include <windows.h>

#include "GUI_Protected.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _WriteByteToFile
*/
static void _WriteByteToFile(U8 Data, HANDLE hFile) {
  U32 nWritten;
  WriteFile(hFile, &Data, 1, &nWritten, NULL);
}

/*********************************************************************
*
*       _WriteU16ToFile
*/
static void _WriteU16ToFile(U16 Data, HANDLE hFile) {
  _WriteByteToFile((U8)Data, hFile);
  _WriteByteToFile((U8)(Data >> 8), hFile);
}

/*********************************************************************
*
*       _WriteRGBToFile
*/
static void _WriteRGBToFile(U32 Data, HANDLE hFile) {
  _WriteByteToFile((U8)Data, hFile);             /* Write R */
  _WriteByteToFile((U8)(Data >> 8), hFile);      /* Write G */
  _WriteByteToFile((U8)(Data >> 16), hFile);     /* Write B */
}

/*********************************************************************
*
*       _WritePixel
*/
static void _WritePixel(U32 Data, int NumColors, HANDLE hFile) {
  if (NumColors) {
    _WriteByteToFile(Data, hFile);    /* write pixel index */
  } else {
    _WriteRGBToFile(Data, hFile);     /* write pixel color */
  }
}

/*********************************************************************
*
*       _Serialize
*/
static void _Serialize(int x0, int y0, int xSize, int ySize, HANDLE hFile) {
  int i;
  int x, y;
  int BPP            = LCD_GetBitsPerPixelEx(GUI_Context.SelLayer);/**/
  int NumColors      = (BPP > 8) ? 0 : (1 << BPP);
  int Cnt            = 0;
  int DataNew;
  int Data = -1;
  char aIsUsed[256] = {0};
  char aIndex2Index[256];
  GUI_LOCK();
  /* Analyze which indices are really used */
  if (NumColors) {
    NumColors = 0;
    for (y = 0; y < ySize; y++) {
      for (x = 0; x < xSize; x++) {
        aIsUsed[LCD_GetPixelIndex(x0 + x, y0 + y)] = 1;
      }
    }
    for (i = 0; i < 256; i++) {
      if (aIsUsed[i]) {
        aIndex2Index[i] = NumColors++;
      }
    }
  }
  /* Write header */
  _WriteByteToFile('S', hFile);
  _WriteByteToFile('B', hFile);
  _WriteU16ToFile((U16)xSize, hFile);
  _WriteU16ToFile((U16)ySize, hFile);
  _WriteU16ToFile((U16)NumColors, hFile);
  /* Write index colors */
  for (i = 0; i < 256; i++) {
    if (aIsUsed[i]) {
      GUI_COLOR Color = GUI_Index2Color(i);
      _WriteRGBToFile(Color, hFile);
    }
  }
  /* Write pixels */
  for (y = 0; y < ySize; y++) {
    for (x = 0; x < xSize; x++) {
      if (NumColors) {
        DataNew = aIndex2Index[LCD_GetPixelIndex(x0 + x, y0 + y)];
      } else {
        DataNew = LCD_GetPixelColor(x0 + x, y0 + y);
      }
      /* Write data record if necessary */
      if ((Cnt == 0xFFFF) | (Data != DataNew)) {
        if (Cnt) {
          _WriteU16ToFile(Cnt, hFile);
          _WritePixel(Data, NumColors, hFile);
        }
        Data = DataNew;
        Cnt = 0;
      }
      Cnt++;
    }
  }
  /* Write last record */
  _WriteU16ToFile(Cnt, hFile);
  _WritePixel(DataNew, NumColors, hFile);
  GUI_UNLOCK();
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/

/*********************************************************************
*
*       LCDSIM_SaveSBMPEx
*/
int LCDSIM_SaveSBMPEx(const char * sFileName, int x0, int y0, int xSize, int ySize) {
  HANDLE hFile = CreateFile(sFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (hFile == INVALID_HANDLE_VALUE) {
    return 1;
  }
  _Serialize(x0, y0, xSize, ySize, hFile);
  if (CloseHandle(hFile) == 0) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       LCDSIM_SaveSBMP
*/
int LCDSIM_SaveSBMP(const char * sFileName) {
  return LCDSIM_SaveSBMPEx(sFileName, 0, 0, LCD_GetXSizeEx(GUI_Context.SelLayer), LCD_GetYSizeEx(GUI_Context.SelLayer));
}

