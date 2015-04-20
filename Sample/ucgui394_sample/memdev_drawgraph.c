/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : MEMDEV_DrawGraph.c
Purpose     : Example for drawing graphs
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "LCD_ConfDefaults.h"
#include <math.h>
#include <stdlib.h>

/*******************************************************************
*
*       defines
*
********************************************************************
*/

#define YSIZE   (LCD_YSIZE - 100)
#define DEG2RAD (3.1415926f / 180)

#if LCD_BITSPERPIXEL == 1
  #define COLOR_GRAPH0 GUI_WHITE
  #define COLOR_GRAPH1 GUI_WHITE
#else
  #define COLOR_GRAPH0 GUI_GREEN
  #define COLOR_GRAPH1 GUI_YELLOW
#endif

/*******************************************************************
*
*      structure containing information for drawing routine
*/
typedef struct {
  I16 *aY;
} PARAM;

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _Draw
  
  Draws the graph area
*/
static void _Draw(void * p) {
  int i;
  PARAM * pParam = (PARAM *)p;
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetColor(GUI_DARKGRAY);
  GUI_ClearRect(19, (LCD_YSIZE - 20) - YSIZE, (LCD_XSIZE - 2), (LCD_YSIZE - 21));
  for (i = 0; i < (YSIZE / 2); i += 20) {
    GUI_DrawHLine((LCD_YSIZE - 20) - (YSIZE / 2) + i, 19, (LCD_XSIZE - 2));
    if (i) {
      GUI_DrawHLine((LCD_YSIZE - 20) - (YSIZE / 2) - i, 19, (LCD_XSIZE - 2));
    }
  }
  for (i = 40; i < (LCD_XSIZE - 20); i += 40) {
    GUI_DrawVLine(18 + i, (LCD_YSIZE - 20) - YSIZE, (LCD_YSIZE - 21));
  }
  GUI_SetColor(COLOR_GRAPH0);
  GUI_DrawGraph(pParam->aY, (LCD_XSIZE - 20), 19, (LCD_YSIZE - 20) - YSIZE);
}

/*******************************************************************
*
*       _Draw2
*/
static void _Draw2(void * p) {
  PARAM * pParam = (PARAM *)p;
  _Draw(p);
  GUI_SetColor(COLOR_GRAPH1);
  GUI_DrawGraph(pParam->aY+15, (LCD_XSIZE - 20), 19, (LCD_YSIZE - 20) - YSIZE);
}

/*******************************************************************
*
*       _Label

  Labels the x & y-axis
*/
static void _Label(void) {
  int x, y;
  GUI_SetBkColor(GUI_RED);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("MEMDEV_DrawGraph - Sample", 160, 5);
  GUI_SetPenSize(1);
  GUI_ClearRect(0, (LCD_YSIZE - 21) - YSIZE, (LCD_XSIZE - 1), (LCD_YSIZE - 1));
  GUI_DrawRect(18, (LCD_YSIZE - 21) - YSIZE, (LCD_XSIZE - 1), (LCD_YSIZE - 20));
  GUI_SetFont(&GUI_Font6x8);
  for (x = 0; x < (LCD_XSIZE - 20); x += 40) {
    int xPos = x + 18;
    GUI_DrawVLine(xPos, (LCD_YSIZE - 20), (LCD_YSIZE - 14));
    GUI_DispDecAt(x / 40, xPos - 2, (LCD_YSIZE - 9), 1);
  }
  for (y = 0; y < YSIZE / 2; y += 20) {
    int yPos = (LCD_YSIZE - 20) - YSIZE / 2 + y;
    GUI_DrawHLine(yPos, 13, 18);
    if (y) {
      GUI_GotoXY(1, yPos - 4);
      GUI_DispSDec(-y / 20, 2);
      yPos = (LCD_YSIZE - 20) - YSIZE / 2 - y;
      GUI_DrawHLine(yPos, 13, 18);
      GUI_GotoXY(1, yPos - 4);
      GUI_DispSDec(y / 20, 2);
    } else {
      GUI_DispCharAt('0', 7, yPos - 4);
    }
  }
}

/*******************************************************************
*
*       _GetRandomData
*/
static void _GetRandomData(I16 * paY, int Time, int n) {
  int aDiff, i;
  if (Time > 5000)
    Time -= 5000;
  if (Time > 2500)
    Time = 5000 - Time;
  Time /= 200;
  aDiff = Time * Time + 1;
  for (i = 0; i < n; i++) {
    if (!i) {
      paY[i] = rand() % YSIZE;
    } else {
      I16 yNew;
      int yD = aDiff - (rand() % aDiff);
      if (rand() & 1) {
        yNew = paY[i-1] + yD;
      } else {
        yNew = paY[i-1] - yD;
      }
      if (yNew > YSIZE) {
        yNew -= yD;
      } else { if (yNew < 0)
        yNew += yD;
      }
      paY[i] = yNew;
    }
  }
}

/*******************************************************************
*
*       _DemoRandomGraph
*/
static void _DemoRandomGraph(void) {
  PARAM Param;
  int tDiff, t0;
  GUI_RECT Rect = {19, (LCD_YSIZE - 20) - YSIZE, (LCD_XSIZE - 2), (LCD_YSIZE - 21)};
  GUI_HMEM hMem = GUI_ALLOC_AllocZero((LCD_XSIZE - 20) * sizeof(I16));
  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_RED);
  GUI_ClearRect(0, 55, LCD_XSIZE, 75);
  GUI_SetFont(&GUI_FontComic18B_1);
  GUI_DispStringAt("Random graph", 20, 55);
  GUI_Lock();
  Param.aY = GUI_ALLOC_h2p(hMem);
  GUI_SetFont(&GUI_Font6x8);
  t0 = GUI_GetTime();
  while((tDiff = (GUI_GetTime() - t0)) < 10000) {
    int t1, tDiff2;
    _GetRandomData(Param.aY, tDiff, (LCD_XSIZE - 20));
    t1 = GUI_GetTime();
    GUI_MEMDEV_Draw(&Rect, _Draw, &Param, 0, 0);
    tDiff2 = GUI_GetTime() - t1;
    if (tDiff2 < 100) {
      GUI_Delay(100 - tDiff2);
    }
  }
  GUI_Unlock();
  GUI_ALLOC_Free(hMem);
}

/*******************************************************************
*
*       _GetSineData
*/
static void _GetSineData(I16 * paY, int n) {
  int i;
  for (i = 0; i < n; i++) {
    float s = sin(i * DEG2RAD * 4);
    paY[i] = s * YSIZE / 2 + YSIZE / 2;
  }
}

/*******************************************************************
*
*       _DemoSineWave
*/
static void _DemoSineWave(void) {
  PARAM Param;
  I16 * pStart;
  int t0, Cnt = 0;
  GUI_RECT Rect = {19, (LCD_YSIZE - 20) - YSIZE, (LCD_XSIZE - 2), (LCD_YSIZE - 21)};
  GUI_HMEM hMem = GUI_ALLOC_AllocZero(405 * sizeof(I16));
  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_RED);
  GUI_ClearRect(0, 55, LCD_XSIZE, 75);
  GUI_SetFont(&GUI_FontComic18B_1);
  GUI_DispStringAt("Sine wave", 20, 55);
  pStart = GUI_ALLOC_h2p(hMem);
  _GetSineData(pStart, 405);
  GUI_SetFont(&GUI_Font6x8);
  t0 = GUI_GetTime();
  while((GUI_GetTime() - t0) < 10000) {
    int t1, tDiff2;
    if (Cnt++ % 90) {
      Param.aY++;
    } else {
      Param.aY = pStart;
    }
    t1 = GUI_GetTime();
    GUI_MEMDEV_Draw(&Rect, _Draw2, &Param, 0, 0);
    tDiff2 = GUI_GetTime() - t1;
    if (tDiff2 < 100) {
      GUI_Delay(100 - tDiff2);
    }
  }
  GUI_ALLOC_Free(hMem);
}

/*******************************************************************
*
*       _DrawOrData
*/
static void _DrawOrData(GUI_COLOR Color, I16 * paY) {
  GUI_SetColor(Color);
  GUI_DrawGraph(paY, (LCD_XSIZE - 20), 19, (LCD_YSIZE - 20) - YSIZE);
}

/*******************************************************************
*
*       _DemoOrData
*/
static void _DemoOrData(void) {
  int i;
  PARAM Param;
  GUI_RECT Rect = {19, (LCD_YSIZE - 20) - YSIZE, (LCD_XSIZE - 2), (LCD_YSIZE - 21)};
  GUI_HMEM hMem = GUI_ALLOC_AllocZero(405 * sizeof(I16));
  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_RED);
  GUI_ClearRect(0, 55, LCD_XSIZE, 75);
  GUI_SetFont(&GUI_FontComic18B_1);
  GUI_DispStringAt("Several waves...", 20 ,55);
  Param.aY = GUI_ALLOC_h2p(hMem);
  _GetSineData(Param.aY, 405);
  GUI_MEMDEV_Draw(&Rect, _Draw, &Param, 0, 0);
  for (i = 0; (i < 90); i++) {
    _DrawOrData(GUI_GREEN, ++Param.aY);
    GUI_Delay(10);
  }
  GUI_ALLOC_Free(hMem);
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates the use of a memory device
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _Label();
  while(1) {
    _DemoRandomGraph();
    _DemoSineWave();
    _DemoOrData();
  }
}
