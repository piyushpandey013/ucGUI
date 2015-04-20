/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                                                                    *
*                    emWin GSC sample code                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : FONT_ShiftJIS.c
Purpose     : Example demonstrating ShiftJIS capabilities of emWin
----------------------------------------------------------------------
*/

#include "GUI.h"

/*******************************************************************
*
*              Definition of ShiftJIS font
*
********************************************************************
*/

/* LATIN CAPITAL LETTER A */
static const unsigned char acFontSJIS13_0041[ 13] = { /* code 0041 */
  ________,
  ________,
  ___X____,
  ___X____,
  __X_X___,
  __X_X___,
  __X_X___,
  _XXXXX__,
  _X___X__,
  _X___X__,
  XXX_XXX_,
  ________,
  ________};

/* LATIN CAPITAL LETTER B */
static const unsigned char acFontSJIS13_0042[ 13] = { /* code 0042 */
  ________,
  ________,
  XXXXX___,
  _X___X__,
  _X___X__,
  _X___X__,
  _XXXX___,
  _X___X__,
  _X___X__,
  _X___X__,
  XXXXX___,
  ________,
  ________};

/* LATIN CAPITAL LETTER C */
static const unsigned char acFontSJIS13_0043[ 13] = { /* code 0043 */
  ________,
  ________,
  __XX_X__,
  _X__XX__,
  X____X__,
  X_______,
  X_______,
  X_______,
  X_______,
  _X___X__,
  __XXX___,
  ________,
  ________};

/* KATAKANA LETTER KE */
static const unsigned char acFontSJIS13_8350[ 26] = { /* code 8350 */
  __XX____,________,
  ___X____,________,
  ___X____,________,
  ___XXXXX,XXXX____,
  __X____X,________,
  _X_____X,________,
  X______X,________,
  ______X_,________,
  ______X_,________,
  _____X__,________,
  ____X___,________,
  __XX____,________,
  ________,________};

/* KATAKANA LETTER GE */
static const unsigned char acFontSJIS13_8351[ 26] = { /* code 8351 */
  __XX____,X_X_____,
  ___X____,_X_X____,
  ___X____,________,
  __XXXXXX,XXX_____,
  __X____X,________,
  _X_____X,________,
  X______X,________,
  ______X_,________,
  ______X_,________,
  _____X__,________,
  ____X___,________,
  __XX____,________,
  ________,________};

/* KATAKANA LETTER KO */
static const unsigned char acFontSJIS13_8352[ 26] = { /* code 8352 */
  ________,________,
  ________,________,
  __XXXXXX,XX______,
  ________,_X______,
  ________,_X______,
  ________,_X______,
  ________,_X______,
  ________,_X______,
  ________,_X______,
  _XXXXXXX,XXXX____,
  ________,________,
  ________,________,
  ________,________};

static const GUI_CHARINFO GUI_FontSJIS13_CharInfo[6] = {
   {   7,   7,  1, (void *)&acFontSJIS13_0041 } /* code 0041 */
  ,{   7,   7,  1, (void *)&acFontSJIS13_0042 } /* code 0042 */
  ,{   7,   7,  1, (void *)&acFontSJIS13_0043 } /* code 0043 */
  ,{  14,  14,  2, (void *)&acFontSJIS13_8350 } /* code 8350 */
  ,{  14,  14,  2, (void *)&acFontSJIS13_8351 } /* code 8351 */
  ,{  14,  14,  2, (void *)&acFontSJIS13_8352 } /* code 8352 */
};

static const GUI_FONT_PROP GUI_FontSJIS13_Prop2 = {
   0x8350 /* first character */
  ,0x8352 /* last character  */
  ,&GUI_FontSJIS13_CharInfo[  3] /* address of first character */
  ,(void*)0 /* pointer to next GUI_FONT_PROP */
};

static const GUI_FONT_PROP GUI_FontSJIS13_Prop1 = {
   0x0041 /* first character */
  ,0x0043 /* last character  */
  ,&GUI_FontSJIS13_CharInfo[  0] /* address of first character */
  ,(void *)&GUI_FontSJIS13_Prop2 /* pointer to next GUI_FONT_PROP */
};

static const GUI_FONT GUI_FontSJIS13 = {
   GUI_FONTTYPE_PROP_SJIS /* type of font    */
  ,13 /* height of font  */
  ,13 /* space of font y */
  ,1 /* magnification x */
  ,1 /* magnification y */
  ,(void *)&GUI_FontSJIS13_Prop1
};

/*******************************************************************
*
*  Definition of string containing ASCII and ShiftJIS characters
*
********************************************************************
*/

static const char aSJIS[] = {
  "ABC\x83\x50\x83\x51\x83\x52\x0"
};

/*******************************************************************
*
*        Demonstrates output of ShiftJIS characters
*
********************************************************************
*/

void DemoShiftJIS(void) {
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("FONT_ShiftJIS - Sample", 160, 5);
  /* Set ShiftJIS font */
  GUI_SetFont(&GUI_FontSJIS13);
  /* Display string */
  GUI_DispStringHCenterAt(aSJIS, 160, 80);
}

/*******************************************************************
*
*                 MainTask
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
  DemoShiftJIS();
  while(1)
    GUI_Delay(100);
}