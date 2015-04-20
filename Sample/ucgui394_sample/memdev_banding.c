/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : MEMDEV_Banding.c
Purpose     : Example demonstrating the use of banding memory devices
----------------------------------------------------------------------
*/

#include "GUI.h"

/*******************************************************************
*
*       defines
*
********************************************************************
*/

#define USE_BANDING_MEMDEV (1) /* Set to 0 for drawing without banding memory device */

#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))

/*******************************************************************
*
*       static variables
*
********************************************************************
*/

static const GUI_POINT aPoints[] = {
  {-50,  0},
  {-10, 10},
  {  0, 50},
  { 10, 10},
  { 50,  0},
  { 10,-10},
  {  0,-50},
  {-10,-10}
};

typedef struct {
  int XPos_Poly, YPos_Poly;
  int XPos_Text, YPos_Text;
  GUI_POINT aPointsDest[8];
} tDrawItContext;

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DrawIt
*/
static void _DrawIt(void * pData) {
  tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
  GUI_Clear();
  GUI_SetFont(&GUI_Font8x8);
  GUI_SetTextMode(GUI_TM_TRANS);
  /* draw background */
  GUI_SetColor(GUI_GREEN);
  GUI_FillRect(pDrawItContext->XPos_Text, 
               pDrawItContext->YPos_Text - 25,
               pDrawItContext->XPos_Text + 100,
               pDrawItContext->YPos_Text - 5);
  /* draw polygon */
  GUI_SetColor(GUI_BLUE);
  GUI_FillPolygon(pDrawItContext->aPointsDest, SIZE_OF_ARRAY(aPoints), 160, 120);
  /* draw foreground */
  GUI_SetColor(GUI_RED);
  GUI_FillRect(220 - pDrawItContext->XPos_Text, 
               pDrawItContext->YPos_Text + 5,
               220 - pDrawItContext->XPos_Text + 100,
               pDrawItContext->YPos_Text + 25);
}

/*******************************************************************
*
*       _DemoBandingMemdev
*/
static void _DemoBandingMemdev(void) {
  tDrawItContext DrawItContext;
  int i, swap=0;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("MEMDEV_Banding - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x9);
  GUI_DispStringHCenterAt("Banding memory device\nwithout flickering", 160, 40);
  DrawItContext.XPos_Poly = 160;
  DrawItContext.YPos_Poly = 120;
  DrawItContext.YPos_Text = 116;
  while (1) {
    swap = ~swap;
    for (i = 0; i < 220; i++) {
      float angle = i * 3.1415926 / 55;
      DrawItContext.XPos_Text = (swap) ? i : 220 - i;
      /* Rotate the polygon */
      GUI_RotatePolygon(DrawItContext.aPointsDest, aPoints, 
                        SIZE_OF_ARRAY(aPoints), angle);
      #if USE_BANDING_MEMDEV
      {
        GUI_RECT Rect = {0, 70, 320,170};
        /* Use banding memory device for drawing */
        GUI_MEMDEV_Draw(&Rect, &_DrawIt, &DrawItContext, 0, 0);
      }
      #else
        /* Simple drawing without using memory devices */
        _DrawIt((void *)&DrawItContext);
      #endif
      #ifdef WIN32
        GUI_Delay(20); /* Use a short delay only in the simulation */
      #endif
    }
  }
}

/*******************************************************************
*
*       MainTask
*
*       Demonstrates banding memory devices
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  _DemoBandingMemdev();
}

