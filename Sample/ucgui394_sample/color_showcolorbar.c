/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : COLOR_ShowColorBar.c
Purpose     : Example draws a color bar
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "LCDConf.h"

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DemoShowColorBar
*/
static void _DemoShowColorBar(void) {
  int x0 = 60, y0 = 40, yStep = 15, i;
  int xsize = LCD_GetDevCap(LCD_DEVCAP_XSIZE) - x0;
	GUI_SetFont(&GUI_Font8x16);
  #if (LCD_FIXEDPALETTE) 
    y0 = 66;
    yStep = 13;
    GUI_DispStringAt("Fixed palette: ", 0, 40);
    GUI_DispDecMin(LCD_FIXEDPALETTE);
  #endif
  GUI_DispStringAt("Red",     0, y0 +      yStep);
  GUI_DispStringAt("Green",   0, y0 +  3 * yStep);
  GUI_DispStringAt("Blue",    0, y0 +  5 * yStep);
  GUI_DispStringAt("Grey",    0, y0 +  6 * yStep);
  GUI_DispStringAt("Yellow",  0, y0 +  8 * yStep);
  GUI_DispStringAt("Cyan",    0, y0 + 10 * yStep);
  GUI_DispStringAt("Magenta", 0, y0 + 12 * yStep);
  for (i = 0; i < xsize; i++) {
    U16 cs = (255 * (U32)i) / xsize;
    U16 x = x0 + i;;
    /* Red */
    GUI_SetColor(cs);
    GUI_DrawVLine(x, y0             , y0 +     yStep - 1);
    GUI_SetColor(0xff + (255 - cs) * 0x10100L);
    GUI_DrawVLine(x, y0 +      yStep, y0 + 2 * yStep - 1);
    /* Green */
    GUI_SetColor(cs<<8);
    GUI_DrawVLine(x, y0 +  2 * yStep, y0 + 3 * yStep - 1);
    GUI_SetColor(0xff00 + (255 - cs) * 0x10001L);
    GUI_DrawVLine(x, y0 +  3 * yStep, y0 + 4 * yStep - 1);
    /* Blue */
    GUI_SetColor(cs * 0x10000L);
    GUI_DrawVLine(x, y0 +  4 * yStep, y0 + 5 * yStep - 1);
    GUI_SetColor(0xff0000 + (255 - cs) * 0x101L);
    GUI_DrawVLine(x, y0 +  5 * yStep, y0 + 6 * yStep - 1);
    /* Gray */
    GUI_SetColor((U32)cs * 0x10101L);
    GUI_DrawVLine(x, y0 +  6 * yStep, y0 + 7 * yStep - 1);
    /* Yellow */
    GUI_SetColor(cs * 0x101);
    GUI_DrawVLine(x, y0 +  7 * yStep, y0 + 8 * yStep - 1);
    GUI_SetColor(0xffff + (255 - cs) * 0x10000L);
    GUI_DrawVLine(x, y0 +  8 * yStep, y0 + 9 * yStep - 1);
    /* Cyan */
    GUI_SetColor(cs * 0x10100L);
    GUI_DrawVLine(x, y0 +  9 * yStep, y0 + 10 * yStep - 1);
    GUI_SetColor(0xffff00 + (255 - cs) * 0x1L);
    GUI_DrawVLine(x, y0 + 10 * yStep, y0 + 11 * yStep - 1);
    /* Magenta */
    GUI_SetColor(cs * 0x10001);
    GUI_DrawVLine(x, y0 + 11 * yStep, y0 + 12 * yStep - 1);
    GUI_SetColor(0xff00ff + (255 - cs) * 0x100L);
    GUI_DrawVLine(x, y0 + 12 * yStep, y0 + 13 * yStep - 1);
  }
}

/*******************************************************************
*
*       MainTask
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("COLOR_ShowColorBar - Sample", 160, 5);
  _DemoShowColorBar();
  while(1)
    GUI_Delay(100);
}
