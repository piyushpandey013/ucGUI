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
File        : LCDSIM.C
Purpose     : Driver for Simulator under Windows
---------------------------END-OF-HEADER------------------------------
*/

#include <windows.h>
#include <memory.h>
#include <math.h>

#include "LCD.h"
#include "GUI.h"
#include "LCDSIM.h"
#include "LCDSIM_Private.h"
#include "SIM.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MARK_MODIFIED(LayerIndex)         { if (_apFix[LayerIndex]) (*(int*)(_apFix[LayerIndex] + LCDSIM_OFF_MODIFY_CNT))++; }
#define MARK_LUT_MODIFIED(LayerIndex)     { ++_LUT_ModifyCnt; if (_apFix[LayerIndex]) *(int*)(_apFix[LayerIndex] + LCDSIM_OFF_LUT_MODIFY_CNT) = _LUT_ModifyCnt; }
#define XY2PTR(x,y, LayerIndex)           ((U8*) (_apaaPixel[LayerIndex] + (    x + _aBytesPerLine[LayerIndex] * y)))
#define XY2PTR_DWORD(x,y, LayerIndex)     ((U32*)(_apaaPixel[LayerIndex] + (4 * x + _aBytesPerLine[LayerIndex] * y)))
#define RETURN_IF_NOT_INITIALIZED(rvalue) if (!_aBPP[0]) return rvalue;
#define RECTWIDTH(lpRect)                 ((lpRect)->right - (lpRect)->left +1)
#define RECTHEIGHT(lpRect)                ((lpRect)->bottom - (lpRect)->top +1)

#ifdef WIN32
  #ifndef ASSERT
    #ifdef _DEBUG
      #define ASSERT(Exp) if (!(Exp)) SIM_ErrorOut("LCDSim.c - Assertion failed: " #Exp);
    #else
      #define ASSERT(Exp)
    #endif
  #endif
#endif

#ifdef LCD_ASSERT
  #undef LCD_ASSERT
#endif
#define LCD_ASSERT(v) ASSERT(v)

#ifdef _MSC_VER
  #define INLINE __forceinline
#else
  #define INLINE
#endif

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
int LCDSIM_aLCDColorWhite[LCDSIM_MAX_DISPLAYS];
int LCDSIM_aLCDColorBlack[LCDSIM_MAX_DISPLAYS];

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GUI_PID_STATE _State;
static U8* _apaaPixel[LCDSIM_MAX_DISPLAYS];      // pointer to pixel data (video memory)
static U8* _apFix[LCDSIM_MAX_DISPLAYS];          // pointer to data area which contains display info

static int         _NumDisplays;
static int         _LUT_ModifyCnt;
static int         _aBytesPerLine[LCDSIM_MAX_DISPLAYS];
static int         _aFixedPalette[LCDSIM_MAX_DISPLAYS];
static int         _aXSize[LCDSIM_MAX_DISPLAYS];
static int         _aYSize[LCDSIM_MAX_DISPLAYS];
static int         _aVXSize[LCDSIM_MAX_DISPLAYS];
static int         _aVYSize[LCDSIM_MAX_DISPLAYS];
static int         _aBPP[LCDSIM_MAX_DISPLAYS];
static int         _aNumColors[LCDSIM_MAX_DISPLAYS];
static BITMAPINFO* _apBitmapInfo[LCDSIM_MAX_DISPLAYS];

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _CheckBreak: Allow break in debug build
*/
#ifdef _DEBUG
static void _CheckBreak(int x, int y, int LayerIndex) {
  static int LayerIndex_Break = -1;
  static int X_Break = -1;
  static int Y_Break = -1;
  static int BreakCnt;
  if (LayerIndex_Break != -1) {
    if (   ((x == X_Break) | (X_Break == -1))
        && ((y == Y_Break) | (Y_Break == -1))
        && (LayerIndex == LayerIndex_Break))
    {
      BreakCnt++;     // Set breakpoint here !!!
    }
  }
}
#endif

/*********************************************************************
*
*         _NotifyMouseState
*
* Informs the application that the mouse state has changed
*/
static void _NotifyMouseState(void) {
  if (_State.Pressed) {
    GUI_TOUCH_StoreState(_State.x, _State.y);
  } else {
    GUI_TOUCH_StoreState(-1, -1);
  }
  GUI_MOUSE_StoreState(&_State);
}

/*********************************************************************
*
*       _ColorRef2Color
*/
INLINE static U32 _ColorRef2Color(COLORREF ColorRef) {
  return (ColorRef & (255 << 8)) | ((ColorRef & 255) << 16) | ((ColorRef & (255 << 16)) >> 16);
}

/*********************************************************************
*
*       _ColorIndex2COLORREF
*/
INLINE static U32 _ColorIndex2COLORREF(int Index16, int LayerIndex) {
  static U32 _Color;
  static int _Index16_Cache = -1;
  if (_Index16_Cache != Index16) {
    _Index16_Cache = Index16;
    _Color = _ColorRef2Color(LCD_Index2ColorEx(Index16, LayerIndex));
  }
  return _Color;
}

/*********************************************************************
*
*         _COLORREF2Index
*/
static U16 _COLORREF2Index(COLORREF ColorRef) {
  U32 Color;
  Color = _ColorRef2Color(ColorRef);
  return LCD_Color2Index(Color);
}

/*********************************************************************
*
*         _Filter1
*/
static int _Filter1(int Color, int Black, int White, int Shift) {
  Color = (Color >> Shift) & 255;
  White = (White >> Shift) & 255;
  Black = (Black >> Shift) & 255;
  return Black + Color * (White - Black) / 255;
}

/*********************************************************************
*
*         _FilterColor
*/
static int _FilterColor(int Color, int Black, int White) {
  int r, g, b;
  r  = _Filter1(Color, Black, White,  0);
  g  = _Filter1(Color, Black, White,  8);
  b  = _Filter1(Color, Black, White, 16);
  return r | (g <<8) | (b << 16);
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
/*********************************************************************
*
*         LCDSIM_Init()
*
* Purpose:
* Allocate & Initialize the shared memory area and create
* the bitmap used for display and as simulated video memory.
* The bitmap will be 8bpp for index color modes (LUT) with no more than
* 8 bpp, 24 bpp for all other modes.
* 
*/
char* LCDSIM_Init(void) {
  int i, j;
  int NumColors;
  //
  // Get LCD info from driver
  //
  _NumDisplays = LCD_GetNumLayers();
  for (i = 0; i < LCDSIM_MAX_DISPLAYS; i++) {
    if (!LCDSIM_aLCDColorWhite[i]) {
      LCDSIM_aLCDColorWhite[i] = 0xffffff;
    }
  }
  for (i = 0; i < _NumDisplays ; i++) {
    int XSize             = LCD_GetXSizeEx(i) * LCD_GetXMagEx(i);
    int YSize             = LCD_GetYSizeEx(i) * LCD_GetXMagEx(i);
    int BPP               = LCD_GetBitsPerPixel_L0Ex(i);
    int BytesPerLine      = (BPP <= 8) ? (XSize + 3) & ~3 : (4 * XSize + 3) & ~3;
    int VXSize            = LCD_GetVXSizeEx(i);
    int VYSize            = LCD_GetVYSizeEx(i);
    int DeltaMode         = LCD_GetDeltaModeEx(i);
    if (DeltaMode) {
      NumColors = 1 << BPP;
    } else {
      NumColors = LCD_GetNumColorsEx(i);
    }
    //
    // Alloc shared memory
    //
    {
      char ac[80] = "emWinLCDMap";
      int sizeofSMem = 0x400000+4096;  // Use 4MB to be on the safe side ...
            HANDLE hMap;
            U8*    pSMem;
      if (i) {
        int len = strlen(ac);
        ac[len]   = '0' + i;
        ac[len+1] = 0;
      }
            hMap    = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, sizeofSMem, ac);
            pSMem   = (U8*)MapViewOfFile(hMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeofSMem);
            if (!pSMem) {
        return "Could not alloc Server data ...";
            }
      _apFix[i]      = pSMem;
      _apaaPixel[i]  = pSMem+4096;
    }
    _aXSize[i]        = XSize;
    _aYSize[i]        = YSize;
    _aVXSize[i]       = VXSize;
    _aVYSize[i]       = VYSize;
    _aBPP[i]          = LCD_GetBitsPerPixel_L0Ex(i);
    _aFixedPalette[i] = LCD_GetFixedPaletteEx(i);
    _aNumColors[i]    = NumColors;
    _aBytesPerLine[i] = BytesPerLine;
    //
    // Init shared memory and static parameter data
    //
    memset(_apFix[i],0, 4096);
    memset(_apaaPixel[i],0, BytesPerLine * YSize);
    strcpy((char*)_apFix[i], "emWin GSC Simulation");
          *(int*)(_apFix[i] + LCDSIM_OFF_XSIZE) = XSize;
          *(int*)(_apFix[i] + LCDSIM_OFF_YSIZE) = YSize;
          *(int*)(_apFix[i] + LCDSIM_OFF_VXSIZE) = VXSize;
          *(int*)(_apFix[i] + LCDSIM_OFF_VYSIZE) = VYSize;
  //    *(int*)(_apFix[i] + LCDSIM_OFF_CONTROLLER) = LCD_CONTROLLER;
          *(int*)(_apFix[i] + LCDSIM_OFF_BPP) = BPP;
          *(int*)(_apFix[i] + LCDSIM_OFF_NUMCOLORS) = NumColors;
          *(int*)(_apFix[i] + LCDSIM_OFF_DELTA_MODE) = DeltaMode;
          _apBitmapInfo[i] = (BITMAPINFO*) (_apFix[i] + 256);
  // Init simulation colors
    if (BPP == LCD_GetBitsPerPixelEx(i)) {
      if (BPP <= 8) {
        for (j = 0; j < NumColors; j++) {
          LCD_COLOR color = LCD_Index2ColorEx(j, i);
          LCDSIM_SetLUTEntry((U8)j, color, i);
        }
      }
    } else {
      for (j = 0; j < NumColors; j++) {
        LCD_COLOR Color = j * 0x111111;
        LCDSIM_SetLUTEntry((U8)j, Color, i);
      }
    }
	  {
		  //
		  // Init BITMAPINFO
		  //
		  BITMAPINFO* pBitmapInfo;
		  pBitmapInfo = _apBitmapInfo[i];
		  memset(&pBitmapInfo->bmiHeader, 0, sizeof(pBitmapInfo->bmiHeader));
		  pBitmapInfo->bmiHeader.biSize  = sizeof (BITMAPINFOHEADER);
		  pBitmapInfo->bmiHeader.biWidth =   _aXSize[i];
		  pBitmapInfo->bmiHeader.biHeight=  -_aYSize[i];    // Neg. for top up bitmap
		  pBitmapInfo->bmiHeader.biPlanes = 1;  // must be set to 1
		  pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		  if (_aBPP[i] <= 8) {
  		  pBitmapInfo->bmiHeader.biBitCount = 8;
		  } else {
	  	  pBitmapInfo->bmiHeader.biBitCount = 32;
		  }
    }
  }
  return NULL;
}

/*********************************************************************
*
*         LCDSIM_Paint()
*         LCDSIM_PaintAt()
*         LCDSIM_PaintEx()
*
**********************************************************************
*/
void LCDSIM_PaintEx(HWND hWnd, LPRECT lpDCRect, LPRECT lpDIBRect) {
  int LayerIndex;
  HDC hDC;
  PAINTSTRUCT ps;
  RETURN_IF_NOT_INITIALIZED(;);
  LayerIndex = GetWindowLong(hWnd, GWL_USERDATA);
  hDC = BeginPaint(hWnd, &ps);
  //
  // Draw background if necessary
  //
  if (ps.fErase) {
    RECT r = {0, 0, 4095, 4095 };
    FillRect(hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
    ps.fErase = 0;
  }
  // Make sure to use the stretching mode best for color pictures
  SetStretchBltMode(hDC, COLORONCOLOR);
// Determine whether to call StretchDIBits() or SetDIBitsToDevice()
  if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
      (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect))) {
    SetDIBitsToDevice(hDC,        // hDC
    lpDCRect->left,             // DestX
                lpDCRect->top,              // DestY
                RECTWIDTH(lpDCRect),        // nDestWidth
                RECTHEIGHT(lpDCRect),       // nDestHeight
                lpDIBRect->left,            // SrcX
                _aYSize[LayerIndex] -  lpDIBRect->top - RECTHEIGHT(lpDIBRect),     // SrcY
                0,                          // nStartScan
                _aYSize[LayerIndex],                 // nNumScans
                XY2PTR(0,0, LayerIndex),              // lpBits
                _apBitmapInfo[LayerIndex],  // lpBitsInfo
                DIB_RGB_COLORS);            // wUsage
  } else {
    StretchDIBits(hDC,            // hDC
                   lpDCRect->left,               // DestX
                   lpDCRect->top,                // DestY
                   RECTWIDTH(lpDCRect),          // nDestWidth
                   RECTHEIGHT(lpDCRect),         // nDestHeight
                   lpDIBRect->left,              // SrcX
                   lpDIBRect->top,               // SrcY
                   RECTWIDTH(lpDIBRect),         // wSrcWidth
                   RECTHEIGHT(lpDIBRect),        // wSrcHeight
       XY2PTR(0,0, LayerIndex),              // lpBits
                   _apBitmapInfo[LayerIndex],    // lpBitsInfo
                   DIB_RGB_COLORS,               // wUsage
                   SRCCOPY);                     // dwROP
  }
  EndPaint(hWnd, &ps);
}

/*********************************************************************
*
*       LCDSIM_PaintAt
*/
void LCDSIM_PaintAt(HWND hWnd, int x, int y) {
  RECT rDest, rSrc;
  int MagX, MagY;
  int LayerIndex = GetWindowLong(hWnd, GWL_USERDATA);
  MagX = SIM_GetMagX();
  MagY = SIM_GetMagY();
  rDest.top =0;
  rDest.bottom= _aYSize[LayerIndex] * MagY - 1;
  rDest.left  = 0;
  rDest.right = _aXSize[LayerIndex] * MagX - 1;
  rSrc.top =0;
  rSrc.bottom= _aYSize[LayerIndex] - 1;
  rSrc.left  = 0;
  rSrc.right = _aXSize[LayerIndex] - 1;
  rDest.left   += x;
  rDest.right  += x;
  rDest.top    += y;
  rDest.bottom += y;
  LCDSIM_PaintEx(hWnd, &rDest, &rSrc);
}

/*********************************************************************
*
*       LCDSIM_Paint
*/
void LCDSIM_Paint(HWND hWnd) { LCDSIM_PaintAt(hWnd, 0,0); }

/*********************************************************************
*
*       LCDSIM_GetModifyCnt
*/
int  LCDSIM_GetModifyCnt(int LayerIndex)     { 
  if (_apFix[LayerIndex]) {
    return *(int*)(_apFix[LayerIndex] + LCDSIM_OFF_MODIFY_CNT);
  } 
  return 0;
};

/*********************************************************************
*
*       LCDSIM_GetModifyCntInfo
*/
int  LCDSIM_GetModifyCntInfo(int LayerIndex) { return _LUT_ModifyCnt; };

/*********************************************************************
*
*       LCDSIM_Index2Color
*/
int LCDSIM_Index2Color(int Index, int LayerIndex) {
  U32 r,g,b;
  RETURN_IF_NOT_INITIALIZED(0);
  if (_aBPP[LayerIndex] <= 8) {
    r = _apBitmapInfo[LayerIndex]->bmiColors[Index].rgbRed;
    g = _apBitmapInfo[LayerIndex]->bmiColors[Index].rgbGreen;
    b = _apBitmapInfo[LayerIndex]->bmiColors[Index].rgbBlue;
  } 
  return (r | (g<<8) | (b<<16));
}

/*********************************************************************
*
*       LCDSIM_SetPixelIndex
*/
void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)  {
  RETURN_IF_NOT_INITIALIZED(;);
  #ifdef _DEBUG
    _CheckBreak(x, y, LayerIndex);
  #endif
  ASSERT(x >= 0);
  ASSERT(x < _aVXSize[LayerIndex]);
  ASSERT(y >= 0);
  ASSERT(y < _aVYSize[LayerIndex]);
  if (_aBPP[LayerIndex] <= 8) {
    *XY2PTR(x,y, LayerIndex) = Index;
  } else {
    U32 Index32 = _ColorIndex2COLORREF(Index, LayerIndex);
    *XY2PTR_DWORD(x, y, LayerIndex) = Index32;
  }
  MARK_MODIFIED(LayerIndex);
}

/*********************************************************************
*
*       LCDSIM_SetPixelColor
*/
void LCDSIM_SetPixelColor(int x, int y, LCD_COLOR PixelColor, int LayerIndex)  {
  RETURN_IF_NOT_INITIALIZED(;);
  #ifdef _DEBUG
    _CheckBreak(x, y, LayerIndex);
  #endif
  ASSERT(x >= 0);
  ASSERT(x < _aVXSize[LayerIndex]);
  ASSERT(y >= 0);
  ASSERT(y < _aVYSize[LayerIndex]);
  {
    COLORREF Index32 = PixelColor;
    *XY2PTR_DWORD(x, y, LayerIndex) = Index32;
  }
  MARK_MODIFIED(LayerIndex);
}

/*********************************************************************
*
*       LCDSIM_FillRect()
*/
void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)  {
  int x, y;
  RETURN_IF_NOT_INITIALIZED(;);
  if (x1 >= _aXSize[LayerIndex])
    x1 = _aXSize[LayerIndex] - 1;
  if (y1 >= _aYSize[LayerIndex])
    y1 = _aYSize[LayerIndex] - 1;
  if (_aBPP[LayerIndex] <= 8) {
    U8 * pOffset = XY2PTR(x0, y0, LayerIndex);
    for (y = y1 - y0; y >= 0; y--) {
      for (x = x1 - x0; x >= 0; x--) {
        *(pOffset + x) = Index;
      }
      pOffset += _aBytesPerLine[LayerIndex];
    }
  } else {
    U32 Index32 = _ColorIndex2COLORREF(Index, LayerIndex);
    U32 *pOffset = XY2PTR_DWORD(x0, y0, LayerIndex);
    for (y = y1 - y0; y >= 0; y--) {
      for (x = x1 - x0; x >= 0; x--) {
        *(pOffset + x) = Index32;
      }
      pOffset += _aBytesPerLine[LayerIndex] >> 2;
    }
  }
  MARK_MODIFIED(LayerIndex);
}

/*********************************************************************
*
*       LCDSIM_GetPixelIndex
*/
int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)  {
  RETURN_IF_NOT_INITIALIZED(0);
  ASSERT(x >= 0);
  ASSERT(x < _aVXSize[LayerIndex]);
  ASSERT(y >= 0);
  ASSERT(y < _aVYSize[LayerIndex]);
  if (_aBPP[LayerIndex] <= 8)
    return(*XY2PTR(x,y, LayerIndex));
  return _COLORREF2Index(*XY2PTR_DWORD(x,y, LayerIndex));
}

/*********************************************************************
*
*       LCDSIM_GetPixelColor
*/
int LCDSIM_GetPixelColor(int x, int y, int LayerIndex)  {
  if (_aBPP[LayerIndex] <= 8) {
    int Index = LCDSIM_GetPixelIndex(x, y, LayerIndex);
    return LCDSIM_Index2Color(Index, LayerIndex);
  }
  return *XY2PTR_DWORD(x,y, LayerIndex);

}

/*********************************************************************
*
*       LCDSIM_SetLUTEntry
*/
void LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color, int LayerIndex) {
  RETURN_IF_NOT_INITIALIZED(;);
  color = _FilterColor(color, LCDSIM_aLCDColorBlack[0], LCDSIM_aLCDColorWhite[0]);
        _apBitmapInfo[LayerIndex]->bmiColors[Pos].rgbRed   = (BYTE)color;
        _apBitmapInfo[LayerIndex]->bmiColors[Pos].rgbGreen = (BYTE)(color>>8);
        _apBitmapInfo[LayerIndex]->bmiColors[Pos].rgbBlue  = (BYTE)(color>>16);
        MARK_LUT_MODIFIED(LayerIndex);
        MARK_MODIFIED(LayerIndex);
}

/*********************************************************************
*
*       LCDSIM_GetMouseState
*/
int LCDSIM_GetMouseState(LCD_tMouseState *pState) {
  int x = *(int*)(_apFix[0]+LCDSIM_OFF_XPOS);
  int y = *(int*)(_apFix[0]+LCDSIM_OFF_YPOS);
  int KeyStat = *(int*)(_apFix[0]+LCDSIM_OFF_KEYSTAT);
  pState->KeyStat = KeyStat;
  pState->x =   x;
  pState->y =   y;
  return 0;
}

/*********************************************************************
*
*       LCDSIM_CheckMouseState
*/
void LCDSIM_CheckMouseState(void) {
  int x = *(int*)(_apFix[0]+LCDSIM_OFF_XPOS);
  int y = *(int*)(_apFix[0]+LCDSIM_OFF_YPOS);
  int KeyStat = *(int*)(_apFix[0]+LCDSIM_OFF_KEYSTAT);
  if ((_State.x != x) | (_State.y != y) | (_State.Pressed != KeyStat)) {
    _State.x = x;
    _State.y = y;
    _State.Pressed = KeyStat;
    _NotifyMouseState();
  }
}

/*********************************************************************
*
*       LCDSIM_SetMouseState
*/
void LCDSIM_SetMouseState(int x, int y, int KeyStat) {
  *(int*)(_apFix[0]+LCDSIM_OFF_XPOS) = x;
  *(int*)(_apFix[0]+LCDSIM_OFF_YPOS) = y;
  *(int*)(_apFix[0]+LCDSIM_OFF_KEYSTAT) = KeyStat;
  LCDSIM_CheckMouseState();
}

/*************************** End of file ****************************/
