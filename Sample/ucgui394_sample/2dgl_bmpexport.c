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
File        : 2DGL_BMPExport.c
Purpose     : Demonstrates the use of GUI_BMP_Serialize
---------------------------END-OF-HEADER------------------------------
*/

#include <windows.h>
#include "GUI.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

//static HANDLE _hFile;

static const GUI_POINT _aPointStar[] = {
  {  0, -36},
  {  8,  -8},
  { 36,   0},
  {  8,   8},
  {  0,  36},
  { -8,   8},
  {-36,   0},
  { -8,  -8}
};

static const GUI_POINT _aPointHexagon[] = {
  {  0, -30},
  { 26, -15},
  { 26,  15},
  {  0,  30},
  {-26,  15},
  {-26, -15},
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _DrawSomething
*
*  Draws something to LCD
*/
static void _DrawSomething(void) {
  int xSize = LCD_GetXSize();
  int ySize = LCD_GetYSize();
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringHCenterAt("Demo of GUI_BMP_Serialize", xSize / 2, (ySize - GUI_GetFontSizeY()) / 2);
  GUI_SetColor(GUI_GREEN);
  GUI_FillPolygon (&_aPointHexagon[0], GUI_COUNTOF(_aPointHexagon), xSize / 2, ySize / 5);
  GUI_SetColor(GUI_RED);
  GUI_FillPolygon (&_aPointStar[0],    GUI_COUNTOF(_aPointStar),    xSize / 2, ySize / 5 * 4);
}

/*********************************************************************
*
*       _WriteByte2File
*
*  This function will be called by GUI_BMP_Serialize to write the
*  bytes to the file
*/
static void _WriteByte2File(U8 Data, void * p) {
  U32 nWritten;
  WriteFile(*((HANDLE *)p), &Data, 1, &nWritten, NULL);
}

/*********************************************************************
*
*       _ExportToFile
*
*  Demonstrates the use of GUI_BMP_Serialize
*/
static void _ExportToFile(void) {
  HANDLE hFile = CreateFile("C:\\GUI_BMP_Serialize.bmp", 
                            GENERIC_WRITE, 0, 0, 
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  GUI_BMP_Serialize(_WriteByte2File, &hFile);
  CloseHandle(hFile);
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _DrawSomething();
  _ExportToFile();
}

