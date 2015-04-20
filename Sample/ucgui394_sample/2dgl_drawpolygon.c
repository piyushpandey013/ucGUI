/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : 2DGL_DrawPolygon.c
Purpose     : Example for drawing polygons
----------------------------------------------------------------------
*/

#include "GUI.h"

/*******************************************************************
*
*       static variables
*
********************************************************************
*/

static const GUI_POINT _aPointArrow[] = {
  {  0,   0},
  {-40, -30},
  {-10, -20},
  {-10, -70},
  { 10, -70},
  { 10, -20},
  { 40, -30},
};

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

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DrawPolygon

  Draws polygons of different shapes and colors
*/
static void _DrawPolygon(void) {
  int y = 90;
  /* clear display */
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  /* display text */
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_DispStringAt("DrawPolygon - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringAt("using", 5, 40);
  GUI_DispStringAt("GUI_FillPolygon", 5, 55);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_DispStringAt("Polygons of arbitrary shape\nin any color", 160, y + 90);
  GUI_Delay(500);
  /* draw filled polygons */
  while (1) {
    GUI_ClearRect(100, y, 220, y + 85);
    GUI_SetColor(GUI_BLUE);
    GUI_FillPolygon (&_aPointArrow[0], 7, 160, y + 80);
    GUI_Delay(1000);
    GUI_ClearRect(100, y, 220, y + 85);
    GUI_SetColor(GUI_RED);
    GUI_FillPolygon (&_aPointStar[0], 8, 160, y + 45);
    GUI_Delay(1000);
    GUI_ClearRect(100, y, 220, y + 85);
    GUI_SetColor(GUI_GREEN);
    GUI_FillPolygon(&_aPointHexagon[0], 6, 160, y + 45);
    GUI_Delay(1000);
  }
}

/*******************************************************************
*
*       MainTask
*
*       Example for drawing polygons
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _DrawPolygon();
  while (1);
}