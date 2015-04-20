/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : AA_HiResPixels.c
Purpose     : Demonstrates high resolution pixels
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "WM.h"

/*******************************************************************
*
*       defines
*
********************************************************************
*/

#define countof(Array) (sizeof(Array) / sizeof(Array[0]))

#define AA_FACTOR    4
#define POLY_SIZE   19
#define POLY_POINTS  3

/*******************************************************************
*
*       static variables
*
********************************************************************
*/

static int _pos_x1   = 30;
static int _pos_y1   = 30;
static int _pos_x2   = 125;
static int _pos_y2   = 30;
static int _pos_x3   = 220 * AA_FACTOR;
static int _pos_y3   = 30  * AA_FACTOR;
static int _color_d  = -1;
static GUI_COLOR _color_a  = 0xFF00FE;
static GUI_COLOR _color_b  = 0x00FEFF;
static GUI_COLOR _color_c  = 0xFEFFFE;

static const GUI_POINT _aPolygon_src[] = {
  {          0         , -POLY_SIZE * 1.4142 },
  {  POLY_SIZE * 1.2247,  POLY_SIZE * 0.7071 },
  { -POLY_SIZE * 1.2247,  POLY_SIZE * 0.7071 },
};

static const GUI_POINT _aPolygonHiRes_src[] = {
  {          0 * AA_FACTOR,          -POLY_SIZE * AA_FACTOR * 1.4142 },
  {  POLY_SIZE * AA_FACTOR * 1.2247,  POLY_SIZE * AA_FACTOR * 0.7071 },
  { -POLY_SIZE * AA_FACTOR * 1.2247,  POLY_SIZE * AA_FACTOR * 0.7071 },
};

static GUI_POINT _aPolygon[POLY_POINTS];
static GUI_POINT _aPolygonHiRes[POLY_POINTS];

/*******************************************************************
*
*       static functions
*
********************************************************************
*/

/*******************************************************************
*
*       _cbWindow

  This is the callback for the window. A callback was used
  for memory devices.
*/
static void _cbWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(_color_a);
    GUI_ClearRect( 0, 0, 250, 14);
    GUI_SetBkColor(_color_b);
    GUI_ClearRect( 0, 15, 250, 29);
    GUI_SetBkColor(GUI_BLACK);
    GUI_ClearRect( 0, 30, 250, 60);
    GUI_SetColor(_color_c);
    GUI_FillPolygon(_aPolygon, POLY_POINTS, _pos_x1, _pos_y1);
    GUI_AA_FillPolygon(_aPolygon, POLY_POINTS, _pos_x2, _pos_y2);
    GUI_AA_EnableHiRes();
    GUI_AA_FillPolygon(_aPolygonHiRes, POLY_POINTS, _pos_x3, _pos_y3);
    GUI_AA_DisableHiRes();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _CalcColor

  Calculates the color-fading.
*/
static void _CalcColor(void) {
  _color_a += 0x000002 * _color_d;
  _color_b += 0x000200 * _color_d;
  _color_c += 0x020002 * _color_d;
  if (_color_c == 0xFEFFFE || _color_c == 0x00FF00) {
    _color_d = -_color_d;
  }
}

/*******************************************************************
*
*       _ShowHiResPixels

  This is frame-function for the callback. It creates the window
  and handles the rotation of polygons and colors.
*/
static void _ShowHiResPixels(void) {
  WM_HWIN hWindow;
  const GUI_FONT *font_old;
  float pi, step, angle;
  int i, tm;
  pi = 3.1415926f;
  step = pi / 180;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  font_old = GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringAt("AA_HiResPixels - Sample", 160, 5);
  GUI_SetFont(font_old);
  GUI_SetColor(GUI_RED);
  GUI_DispStringHCenterAt("not\nantialised", 65, 100);
  GUI_SetColor(GUI_GREEN);
  GUI_DispStringHCenterAt("antialised", 160, 100);
  GUI_SetColor(GUI_BLUE);
  GUI_DispStringHCenterAt("antialised\nwith high\nresolution", 255, 100);
  hWindow = WM_CreateWindow(35, 140, 250, 60, WM_CF_SHOW | WM_CF_MEMDEV, &_cbWindow, 0);
  WM_SelectWindow(hWindow);
  GUI_AA_SetFactor(AA_FACTOR);
  while (1) {
    for (i=0, angle=0; i<360; i++) {
      tm = GUI_GetTime();
      angle += step;
      GUI_RotatePolygon(_aPolygonHiRes, _aPolygonHiRes_src, POLY_POINTS, angle);
      GUI_RotatePolygon(_aPolygon, _aPolygon_src, POLY_POINTS, angle);
      _CalcColor();
      WM_InvalidateWindow(hWindow);
      while (((GUI_GetTime()-tm) < 50) || (WM_Exec1() != 0));
    }
  }
  WM_DeleteWindow(hWindow);
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates the effect of high resolution coordinates
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _ShowHiResPixels();
}