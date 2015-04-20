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
File        : WinMain.c
Purpose     : Windows Simulator, main program
---------------------------END-OF-HEADER------------------------------
*/

#include <windows.h>
#include <stdio.h>
#include "ResourceSim.h"
#include "LCD.h"
#include "GUI.h"
#include "SIM.h"
#include "LCDSIM.h"
#include "Branding\Branding.h"
#include "LCDSIM_Private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#ifndef SIM_WINMAIN
  #define SIM_WINMAIN 1
#endif

#define countof(exp) (sizeof(exp)/sizeof(exp[0]))
#define HARDKEYS_MAX   128
#define LOG_COLOR_RED 1

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/

typedef void TaskMain(void);

/*********************************************************************
*
*       Static const data
*
**********************************************************************
*/
typedef struct {
  int VKey;
  int Key;
} VKEY2KEY;

const VKEY2KEY aVKey2Key[] = {
  {VK_UP,         GUI_KEY_UP},
  {VK_DOWN,       GUI_KEY_DOWN},
  {VK_RIGHT,      GUI_KEY_RIGHT},
  {VK_LEFT,       GUI_KEY_LEFT},
  {VK_HOME,       GUI_KEY_HOME},
  {VK_END,        GUI_KEY_END},
  {VK_SHIFT,      GUI_KEY_SHIFT},
  {VK_CONTROL,    GUI_KEY_CONTROL},
  {VK_BACK,       GUI_KEY_BACKSPACE},
  {VK_INSERT,     GUI_KEY_INSERT},
  {VK_DELETE,     GUI_KEY_DELETE},
};

/*********************************************************************
*
*       Static variables
*
**********************************************************************
*/

static int       _xPosLCD = -1;     // position of the LCD
static int       _yPosLCD = -1;     // position of the LCD
static int       _rgbTransparent;   // color for transparency
static int       _MagX = 1;          // Magnification X
static int       _MagY = 1;          // Magnification Y
static char      _MessageBoxOnError = 1;
static HANDLE    _hLogo;
static HBITMAP   _ahBmpDevice[2];
static int       _aLUTModifyCnt[LCDSIM_MAX_DISPLAYS];
static int       _aModifyCnt[LCDSIM_MAX_DISPLAYS];
static struct {
  int x0, y0, x1, y1;
  char IsPressed;
  SIM_HARDKEY_CB* pfCallback;
  int Mode;
} _aHardkey[HARDKEYS_MAX];


static int       _NumHardkeys;

static HANDLE    _hFileError;
static HINSTANCE _hInst;                                    // current instance
static HWND      _hWndMain;
//static HWND      _hParent;
static HWND      _hWndLCD1;
static char      _KeyBuffer;
static char      _CmdKill;
static char *    _pCmdLine;
static DWORD     _SuspendCount;
static int       _timeStartup;
static HMENU     _hMenuPopup;

/* class names */
static const char acClassNameMain[]      = "uC-GUI Simulation Mainframe";
static const char acClassNameLCDInfo[]   = "uC-GUI LCD Info";
static const char acClassNameLCD[]       = "uC-GUI LCD";
static const char acClassNameDevice[]    = "uC-GUIDevice";
static const char acClassNameHardkey[]   = "uC-GUIHardkey";
static const char acClassNameLog[]       = "Log";

/* LOG-window */
static char _LOG_acBuffer[5000];
static HWND _LOG_hWnd;
static int  _LOG_x0 = 0;
static int  _LOG_y0;
static int  _LOG_xsize = 500;
static int  _LOG_ysize = 60;

/* thread management */
static HANDLE _ghThread = NULL;

/* multitasking support */
static U8     _NumTask;
static U32    _aThreadID[100];
static HANDLE _ahThread[100];

/* LCD-window */
typedef struct {
  HWND hWnd;
  int  ModifyCnt;
} LCD_WINDATA;

/*********************************************************************
*
*       Forward declarations
*
**********************************************************************
*/

void main(void);
void MainTask(void);
static LRESULT CALLBACK _WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK _WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK _WndProcDevice(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK _WndProcLCDInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK _WndProcHardkey(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _VirtKey2Key
*/
static int _VirtKey2Key(int VirtKey) {  
  int i;
  for (i=0; aVKey2Key[i].VKey; i++) {
    if (aVKey2Key[i].VKey == VirtKey) {
      return aVKey2Key[i].Key;
    }
  }
  return 0;
}

/*********************************************************************
*
*       _Keydown2ASCII
*/
static U16 _Keydown2ASCII(WPARAM wParam) {
  int Ret;
  U8 aState[256];
  U16 Key;
  GetKeyboardState(aState);
  Ret = ToAscii(wParam, MapVirtualKey(wParam, 0), aState, &Key, 0);
  if (Ret == 1) {
    return Key & 255;
  }
  return (U16)wParam;
}

/*********************************************************************
*
*       _MessageBox1
*/
static void _MessageBox1(LPCTSTR lpText) {
  MessageBox(_hWndMain, lpText, BRANDING_GetAppNameShort(), MB_OK | MB_ICONEXCLAMATION);
}

/*********************************************************************
*
*       _LinesIntersect
*/
static int _LinesIntersect(int xLeft0 ,int xRight0, int xLeft1 ,int xRight1) {
  int x0 = max(xLeft0, xLeft1);
  int x1 = min(xRight0, xRight1);
  return (x1 - x0 >= 0);
}

/*********************************************************************
*
*       _GetWidthOfBitmap
*/
static int _GetWidthOfBitmap(HBITMAP hBmp) {
  BITMAP bmp;
  GetObject(hBmp, sizeof(bmp), &bmp);
  return bmp.bmWidth;
}

/*********************************************************************
*
*       _GetYSizeOfBitmap
*/
static int _GetYSizeOfBitmap(HBITMAP hBmp) {
  BITMAP bmp;
  GetObject(hBmp, sizeof(bmp), &bmp);
  return bmp.bmHeight;
}

/*********************************************************************
*
*       _GetScanLine
*/
static void _GetScanLine(HBITMAP hBmp, int y, DWORD* pRGB) {
  if (hBmp) {
    HDC hdc;
    BITMAPV4HEADER BmpInfo = {0};
    BmpInfo.bV4Size = sizeof(BmpInfo);
    hdc = GetWindowDC(NULL);
    /* Fill in the Bitmap info structure */
    GetDIBits(hdc, hBmp, 0, 0, NULL, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS);
    BmpInfo.bV4V4Compression = BI_RGB;
    BmpInfo.bV4BitCount = 32;
    if (BmpInfo.bV4Height > 0)
      y = BmpInfo.bV4Height - y - 1;
    GetDIBits(hdc, hBmp, y, 1, pRGB, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS);
  }
}

/*********************************************************************
*
*       _CompareBits
*/
static int _CompareBits(U32 Color0, U32 Color1) {
  if ((Color0 & 0xffffff) == (Color1 & 0xffffff)) {
    return 0;
  }
  return 1;
}


/*********************************************************************
*
*       _SetBitmapRegion

This is a key function for the display of the bitmap. It assigns
a clipping region to the window identical to the outline of the
bitmap.

*/
static void _SetBitmapRegion(HWND hWnd, HBITMAP hBmp, DWORD rgbTrans, char IsHardkey) {
  if (hBmp) {
    HRGN hRgn = 0;
    HRGN hRgn0;
    POINT Point = {0};
    DWORD acBits[4096];
    int y;
    int yOff = 0;
    int xOff = 0;
    int XSize = _GetWidthOfBitmap(hBmp);
    int YSize = _GetYSizeOfBitmap(hBmp);
    RECT WinRect;
    if (IsHardkey) {
      HWND hWndParent = GetParent(hWnd);
      ClientToScreen(hWndParent, &Point);
      GetWindowRect(hWnd, &WinRect);
      YSize = WinRect.bottom - WinRect.top;
      yOff = WinRect.top  - Point.y;
      xOff = WinRect.left - Point.x;
    }
    for (y = 0; y < YSize; y++) {
      int i, i0, i1;
      _GetScanLine(hBmp, y + yOff, &acBits[0]);
      for (i = 0; i < XSize;) {
        while (_CompareBits(acBits[i], rgbTrans) == 0 && i < XSize) {
          i++;
        }
        i0 = i;
        while (_CompareBits(acBits[i], rgbTrans) != 0 && i < XSize) {
          i++;
        }
        i1 = i;
        if (i0 <XSize) {
          hRgn0 = CreateRectRgn(i0 - xOff, y, i1 - xOff, y + 1);
          if (hRgn0) {
            if (hRgn) {
              CombineRgn(hRgn, hRgn, hRgn0, RGN_OR);
              DeleteObject(hRgn0);
            } else {
              hRgn = hRgn0;
            }
          } else {
            _MessageBox1("Could not create region");
          }
        }
      }
    }
    SetWindowRgn(hWnd, hRgn,0);  // Note: Region is now owned by the system, do not delete
  }
}

/*********************************************************************
*
*       Find hardkeys
*/
static void _AddHardkeyRegion(int y, int x0, int x1) {
  //
  // Find a hardkey that we can attach this segment to
  //
  int i;
  for (i = 0; i < countof(_aHardkey); i++) {
    if (x1 > x0) {
      if (_aHardkey[i].y1 + 1 == y) {
        if (_LinesIntersect(x0, x1, _aHardkey[i].x0, _aHardkey[i].x1)) {
          _aHardkey[i].y1++;
          _aHardkey[i].x0 = min(_aHardkey[i].x0, x0);
          _aHardkey[i].x1 = max(_aHardkey[i].x1, x1);
          return; // We are done since there can be only one Rect
        }
      }
    }
  }
  //
  // Find a free hardkey
  //
  for (i = 0; i < countof(_aHardkey); i++) {
    if ((_aHardkey[i].x1 == 0) &&  (_aHardkey[i].x0 == 0)) {
      _aHardkey[i].x0 = x0;
      _aHardkey[i].x1 = x1;
      _aHardkey[i].y0 = y;
      _aHardkey[i].y1 = y;
      _NumHardkeys++;
      return;
    }
  }
  SIM_ErrorOut("_AddHardkeyRegion: - Too many hardkeys defined in bitmap");
}
          
/*********************************************************************
*
*       _FindHardkeys
*/
static void _FindHardkeys(HBITMAP hBmp, DWORD rgbTrans) {
  DWORD aColor[4096];
  if (hBmp) {
    int x, y;
    int XSize = _GetWidthOfBitmap(hBmp);
    int yMax = _GetYSizeOfBitmap(hBmp);
    for (y = 0; y < yMax; y++) {
      int x0, x1;
      _GetScanLine(hBmp, y, &aColor[0]);
      for (x = 0; x < XSize;) {
        while ((_CompareBits(aColor[x], rgbTrans) == 0) && (x < XSize)) {
          x++;
        }
        x0 = x;
        while ((_CompareBits(aColor[x], rgbTrans) != 0) && (x < XSize)) {
          x++;
        }
        x1 = x;
        if (x1 > x0) {
          _AddHardkeyRegion(y, x0, x1);
        }
      }
    }
  }
}

/*********************************************************************
*
*       _GetYSizePhysEx
*/
static int _GetYSizePhysEx(int LayerIndex) {
  return LCD_GetYSizeEx(LayerIndex) * LCD_GetYMagEx(LayerIndex);
}

/*********************************************************************
*
*       _GetXSizePhysEx
*/
static int _GetXSizePhysEx(int LayerIndex) {
  return LCD_GetXSizeEx(LayerIndex) * LCD_GetXMagEx(LayerIndex);
}

/*********************************************************************
*
*       _GetXSizePhys, _GetYSizePhys
*/
static int _GetYSizePhys(void) { return _GetYSizePhysEx(0);}
static int _GetXSizePhys(void) { return _GetXSizePhysEx(0);}

/*********************************************************************
*
*       Static functions: Log window
*
**********************************************************************
*/
/*********************************************************************
*
*       _LOG_Paint
*/
static void _LOG_Paint(HWND hWnd, HDC hdc) {
  RECT rt;
  int i, NumLines, WinSizeY;
  HANDLE hFont;
  SIZE size; 
  char *s = _LOG_acBuffer;
  GetClientRect(hWnd, &rt);
  WinSizeY = rt.bottom-rt.top;
  if (*s == '\n')
    s++;
  hFont = GetStockObject(DEFAULT_GUI_FONT);
  SelectObject(hdc, hFont);
  GetTextExtentPoint32(hdc, "@", 1, &size);
  //
  // Count Lines
  NumLines=1;
  for (i = 0; *(s + i); i++)
    if (*(s + i) == '\n')
      NumLines++;
  //
  // Recalc layout
  rt.top += WinSizeY-NumLines * size.cy;
  while (*s) {
    SIZE SegSize;
    int Len;
    switch (*s) {
    case 0:
      return;
    case LOG_COLOR_RED:
      SetTextColor(hdc, RGB(0, 0, 0xff));
      break;
    case '\n':
      rt.top += size.cy;
      rt.left = 0;
      s++;
    }
    if (*s == LOG_COLOR_RED) {
      SetTextColor(hdc, RGB(255, 0, 0));
      s++;
    }
    Len = 0;
    while (*(s + Len) && (*(s + Len) != '\n') && (*(s + Len) != LOG_COLOR_RED))
      Len++;
    DrawText(hdc, s, Len, &rt, DT_LEFT);
    GetTextExtentPoint32(hdc, s, Len, &SegSize);
    rt.left += SegSize.cx;
    SetTextColor(hdc, RGB(0, 0, 0));
    s += Len;
  }
}

/*********************************************************************
*
*       _LOG_Clear
*/
static void _LOG_Clear(void) {
  _LOG_acBuffer[0] = 0;
}

/*********************************************************************
*
*       _LOG_DelFirstLine
*/
static void _LOG_DelFirstLine(void) {
  int i;
  for (i = 0; _LOG_acBuffer[i]; i++) {
    if (_LOG_acBuffer[i]=='\n') {
      strcpy(&_LOG_acBuffer[0], &_LOG_acBuffer[i + 1]);
      return;
    }
  }
}

/*********************************************************************
*
*       _LOG_AddPlain
*/
static void _LOG_AddPlain(const char*s) {
  while (strlen(_LOG_acBuffer) + strlen(s) > sizeof(_LOG_acBuffer)) {
    _LOG_DelFirstLine();
  }
  strcpy(&_LOG_acBuffer[strlen(_LOG_acBuffer)], s);
  if (_LOG_hWnd) {
    InvalidateRect(_LOG_hWnd, NULL, TRUE);
  }
}

/*********************************************************************
*
*       _LOG_Add
*/
static void __cdecl _LOG_Add(const char *format ,... ) {
  char ac[200];
  va_list arglist;
  va_start(arglist, format);
  sprintf (ac, format
           , va_arg(arglist,int)
           , va_arg(arglist,int)
           , va_arg(arglist,int)
           , va_arg(arglist,int)
           , va_arg(arglist,int));
  _LOG_AddPlain(ac);
};

/*********************************************************************
*
*       _LOG_AddRed
*/
static void _LOG_AddRed(void) {
  char ac[200];
  ac[0] = LOG_COLOR_RED;
  ac[1] = 0;
  _LOG_Add(&ac[0]);
}

/*********************************************************************
*
*       _WndProcLog
*/
static LRESULT CALLBACK _WndProcLog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  switch (message) {
    case WM_PAINT:
      {
        HDC hdc = BeginPaint(hWnd, &ps);
        _LOG_Paint(hWnd, hdc);
      }
      EndPaint(hWnd, &ps);
      break;
    case WM_DESTROY: _LOG_hWnd = 0; break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

/*********************************************************************
*
*       _LOG_Init
*/
static void _LOG_Init(HINSTANCE hInst, HICON hIcon) {
  static char IsInitialized=0;
  if (!IsInitialized) {
    WNDCLASSEX wcex;
    IsInitialized =1;
    memset (&wcex, 0, sizeof(wcex));
    wcex.cbSize       = sizeof(WNDCLASSEX); 
    wcex.hInstance    = hInst;
    wcex.style        = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc  = (WNDPROC)_WndProcLog;
    wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName= acClassNameLog;
    wcex.hIcon        = hIcon;
    wcex.hCursor      = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wcex);
  }
}

/*********************************************************************
*
*       _LOG_Create
*/
static void _LOG_Create(HINSTANCE hInst, HWND hWndParent) {
  if (_LOG_hWnd !=0)  // Create only one instance of this window
    return;
  _LOG_Init(hInst,0);
  _LOG_hWnd = CreateWindow(acClassNameLog, "Log",
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME 
                           | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CHILD | WS_CLIPSIBLINGS
                           | WS_VISIBLE | WS_VSCROLL,
                           _LOG_x0, _LOG_y0, _LOG_xsize, _LOG_ysize,
                           hWndParent, NULL, hInst, NULL);
}

/*********************************************************************
*
*       Clipboard support
*/

static void _OnCopy(int LayerIndex) {
  HGLOBAL hMemClipboard;
  BITMAPINFOHEADER bmiHeader = {0};
  int XSize = _GetXSizePhysEx(LayerIndex);
  int YSize = _GetYSizePhysEx(LayerIndex);
  int BPP   = LCD_GetBitsPerPixel_L0Ex(LayerIndex);
  int x,y, i,Size;
  int BytesPerLine = ((BPP > 8) ? (2 * XSize + 2) : (XSize + 3)) & ~3;
  int NumColors = (BPP > 8) ? 0 : (1 << BPP);
  union {
    U8*               u8;
    U16*              u16;
    COLORREF*         ColorRef;
    BITMAPINFOHEADER* BitmapInfoHeader;
  } p;
  Size = sizeof(BITMAPINFOHEADER) + NumColors * 4 + YSize * BytesPerLine;
  hMemClipboard = GlobalAlloc(GMEM_MOVEABLE |GMEM_DDESHARE , Size);  // Note that GlobalFree is called automatically by windows
  p.u8      = GlobalLock(hMemClipboard);
  bmiHeader.biClrUsed = NumColors;
  bmiHeader.biBitCount = (BPP  <= 8) ? 8 : 16;
  bmiHeader.biHeight = YSize;
  bmiHeader.biPlanes = 1;
  bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmiHeader.biWidth = XSize;
  /* Fill in bitmap info header */
  *p.BitmapInfoHeader++ = bmiHeader;
  /* Fill in color table */
  for (i = 0; i < NumColors; i++) {
    COLORREF Color;
    Color = LCDSIM_Index2Color(i, LayerIndex);
    Color = ((Color>>16)&255) | (Color & 0xff00) | ((Color & 0xff) <<16);
    *p.ColorRef++ = Color;
  }
  /* Fill in the pixels */
  for (y = YSize-1; y >= 0; y--) {
    for (x = 0; x < XSize; x++) {
      if (BPP <= 8) {
        *p.u8++ = LCDSIM_GetPixelIndex(x, y, LayerIndex);
      } else {
        int Color = LCDSIM_GetPixelColor(x, y, LayerIndex);
        int r = ((Color >> 16) * 31 + 127) / 255;
        int g = (((Color >> 8) & 255) * 31 + 127) / 255;
        int b = ((Color & 255) * 31 + 127) / 255;
        *p.u16++ = (r << 10) | (g << 5) | b;   // 16 bpp Bitmaps in windows are 555: rrrrrgggggbbbbb
      }
    }
    /* Align pointer to next U32 */
    if (BytesPerLine &3)
      p.u8 += 3- (BytesPerLine &3);
  }
  /* Copy to clipboard */
  OpenClipboard(NULL);
  EmptyClipboard();
  SetClipboardData(CF_DIB, hMemClipboard);
  CloseClipboard();
  GlobalUnlock(hMemClipboard);
}
  
/*********************************************************************
*
*       Static functions: Thread management
*
**********************************************************************
*/
/*********************************************************************
*
*       _Thread
*/
static DWORD __stdcall _Thread(void* Parameter) {
#ifdef WATCOM
  main();
#else
  MainTask();
#endif
  _ghThread =0;
  SIM_Log("\nApplication terminated.");
  return 0;
}

/*********************************************************************
*
*       _THREAD_IsRunning
*/
static char _THREAD_IsRunning(void) {
  return _ghThread ? 1 : 0;
}

/*********************************************************************
*
*       _THREAD_IsSuspended
*/
static char _THREAD_IsSuspended(void) {
  return _SuspendCount ? 1 : 0;
}

/*********************************************************************
*
*       _THREAD_KillAll
*/
static void _THREAD_KillAll(void) {
// Try to get the thread to terminate by itself
  int i = 0;
  _CmdKill = 1;
  while (_ghThread && i++ < 20)
    Sleep(10);
  if (_ghThread)
    TerminateThread(_ghThread, 0);
  _ghThread = 0;
  _CmdKill = 0;
}

/*********************************************************************
*
*       _THREAD_Sleep
*/
static void _THREAD_Sleep(int ms) {
  while (ms > 0) {
    if (_CmdKill) {
      // Temporarily increase threads priority in order to make
      // sure it is not interrupted before exit
      SetThreadPriority(_ghThread, THREAD_PRIORITY_HIGHEST);
      _ghThread = 0;
      ExitThread(0);
    }
    Sleep(10); ms -= 10;
  }
}

/*********************************************************************
*
*       _THREAD_StartApplication
*/
static void _THREAD_StartApplication(void) {
  DWORD ThreadId;
  _LOG_Clear();
  SIM_Log("Application started");
  if (_ghThread)
    _THREAD_KillAll();
  _ghThread = CreateThread(NULL, 0, _Thread, NULL, 0, &ThreadId);
  _SuspendCount=0;
}

/*********************************************************************
*
*       _THREAD_StopApplication
*/
static void _THREAD_StopApplication(void) {
  int i;
  SIM_Log("\nApplication suspended");
  if (_SuspendCount) {
    _MessageBox1("Application paused already ...");
    return;
  }
  if (!_ghThread) {
    _MessageBox1("Application is not running...");
    return;
  }
  _SuspendCount = SuspendThread(_ghThread) + 1;
  for (i = 0; i < _NumTask; i++) {
    SuspendThread(_ahThread[i]);
  }
}

/*********************************************************************
*
*       _THREAD_ContinueApplication
*/
static void _THREAD_ContinueApplication(void) {
  int i;
  SIM_Log("\nApplication continued");
  if (!_SuspendCount) {
    _MessageBox1("Application is not stopped...");
    return;
  }
  if (!_ghThread) {
    _MessageBox1("Application is not running...");
    return;
  }
  _SuspendCount = ResumeThread(_ghThread) - 1;
  for (i = 0; i < _NumTask; i++) {
    ResumeThread(_ahThread[i]);
  }
}
 
/*********************************************************************
*
*       _CreateTask
*/
static unsigned long __stdcall _CreateTask(LPVOID lpParameter) {
  ((TaskMain*)lpParameter)();
  return 0;
}

/*********************************************************************
*
*       Static functions: Logging
*
**********************************************************************
*/
/*********************************************************************
*
*       _LogTime
*/
static void _LogTime(void) {
  char ac[80];
  sprintf(ac,"\n%d:   ",SIM_GetTime());
  _LOG_Add(ac);
}

/*********************************************************************
*
*       _SendToErrorFile
*/
static void _SendToErrorFile(const char* s) {
  DWORD NumBytesWritten;
  if (_hFileError == 0) {
    _hFileError = CreateFile("SimError.log", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,  FILE_ATTRIBUTE_NORMAL, NULL);
  }
  WriteFile(_hFileError, "\r\n", 2, &NumBytesWritten, NULL);
  WriteFile(_hFileError, s, strlen(s), &NumBytesWritten, NULL);
}

/*********************************************************************
*
*       _RegisterClasses
*/
static void _RegisterClasses(void){
  WNDCLASSEX wcex;
  HICON hIcon = LoadIcon(_hInst, (LPCTSTR)IDR_MAINFRAME);
  memset (&wcex, 0, sizeof(wcex));
  wcex.cbSize       = sizeof(WNDCLASSEX); 
  wcex.hInstance    = _hInst;
  //
  // Register main window
  //
  wcex.style        = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc  = (WNDPROC)_WndProcMain;
  wcex.hIcon        = LoadIcon(_hInst, (LPCTSTR)IDR_MAINFRAME);
  wcex.hCursor      = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground= (HBRUSH)(COLOR_APPWORKSPACE+1);
  wcex.lpszMenuName = (LPCSTR)IDC_SIMULATION;
  wcex.lpszClassName= acClassNameMain;
  RegisterClassEx(&wcex);
  //
  // Register Device window
  //
  wcex.lpfnWndProc  = (WNDPROC)_WndProcDevice;
  wcex.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName= acClassNameDevice;
  RegisterClassEx(&wcex);
  //
  // Register LCD Info window
  //
  wcex.style        = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc  = (WNDPROC)_WndProcLCDInfo;
  wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszClassName= acClassNameLCDInfo;
  RegisterClassEx(&wcex);//
  //
  // Register Hardkey window
  //
  wcex.style        = 0;
  wcex.lpfnWndProc  = (WNDPROC)_WndProcHardkey;
  wcex.lpszClassName= acClassNameHardkey;
  RegisterClassEx(&wcex);
  //
  // Register LCD window
  //
  wcex.lpfnWndProc  = (WNDPROC)_WndProcLCD;
  wcex.lpszClassName= acClassNameLCD;
  RegisterClassEx(&wcex);//
  //
  // Register LOG window
  //
  _LOG_Init(_hInst, hIcon);
}
 
/*********************************************************************
*
*       _WndProcLCD
*/
static LRESULT CALLBACK _WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int xPos = (signed short)LOWORD(lParam) / _MagX;  // horizontal position of cursor 
  int yPos = (signed short)HIWORD(lParam) / _MagY;  // vertical position of cursor 
  unsigned int LayerIndex = GetWindowLong(hWnd, GWL_USERDATA);
  switch (message) {
  case WM_CREATE:
    SetTimer(hWnd, 0, 20, NULL);
    break;
  case WM_TIMER:
    if (LayerIndex == 0) {
      LCDSIM_CheckMouseState();
    }
    if (LayerIndex < countof(_aModifyCnt)) {
      int NewCnt = LCDSIM_GetModifyCnt(LayerIndex);
      if (_aModifyCnt[LayerIndex] != NewCnt) {
        if (InvalidateRect(hWnd, NULL, FALSE)) {
          _aModifyCnt[LayerIndex] = NewCnt;            // invalidation successfull
        }
      }
    }
    if (_ghThread == NULL)
      PostQuitMessage(0);
    break;
  case WM_PAINT:
    LCDSIM_Paint(hWnd);
    break;
  // Handle mouse events
  case WM_RBUTTONDOWN:
    { POINT Point;
      Point.x = (signed short)LOWORD(lParam);
      Point.y = (signed short)HIWORD(lParam);
      ClientToScreen(hWnd, &Point);
      TrackPopupMenu(_hMenuPopup, TPM_RIGHTBUTTON, Point.x, Point.y, 0, GetParent(hWnd), NULL);
    }
    break;
  case WM_LBUTTONUP:
  case WM_LBUTTONDOWN:
  case WM_MOUSEMOVE:
    {
      int fwKeys = wParam;        // key flags 
      if ((xPos < 0) | (yPos < 0)
        | (xPos > LCD_GetDevCap(LCD_DEVCAP_XSIZE))
        | (yPos > LCD_GetDevCap(LCD_DEVCAP_YSIZE)))
      {
        xPos = -1;
        yPos = -1;
        fwKeys = 0;
      }
      LCDSIM_SetMouseState(xPos, yPos, fwKeys);
    }
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _CreateWndLCDInfo
*/
static void _CreateWndLCDInfo(int LayerIndex) {
  int NumColors, DeltaMode;
  int x, y;
  char ac[80];
  HWND hWnd;
  int xSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
  int ySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);
  int ySizeCaption = GetSystemMetrics(SM_CYCAPTION);
  DeltaMode = LCD_GetDeltaModeEx(LayerIndex);
  x = _GetXSizePhys() + xSizeFrame * 2;
  y = LayerIndex * 100;
  wsprintf(ac, "Colors #%d", LayerIndex);
  if (DeltaMode) {
    NumColors = 1 << LCD_GetBitsPerPixel_L0Ex(LayerIndex);
  } else {
    NumColors = LCD_GetNumColorsEx(LayerIndex);
  }
  if (NumColors > 256) {
    hWnd = CreateWindow(acClassNameLCDInfo, ac,
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME 
                             | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CHILD | WS_CLIPSIBLINGS,
                             x, y, 
                             128 + xSizeFrame * 2, 
                             128 + ySizeFrame * 2 + ySizeCaption,
                             _hWndMain, NULL, _hInst, NULL);
  } else {
    int ysize;
    ysize = (NumColors + 15) / 16 *10 + 30;       
    hWnd = CreateWindow(acClassNameLCDInfo, ac,
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME 
                        | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CHILD | WS_CLIPSIBLINGS,
                        x, y, 
                        160 + 10,
                        ysize,
                        _hWndMain, NULL, _hInst, NULL);
    SetTimer(hWnd, 0, 20, NULL);
  }
  SetWindowLong(hWnd, GWL_USERDATA, LayerIndex);
  ShowWindow(hWnd, SW_SHOW);
} 

/*********************************************************************
*
*       _CreateWndLCD
*/
static void _CreateWndLCD(void) {
  int  i;
  int  NumDisplays = LCD_GetNumLayers();
  int  xSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
  int  ySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);
  int  ySizeCaption = GetSystemMetrics(SM_CYCAPTION);
  char acTitle[200];
  for (i = 0; i < NumDisplays; i++) {
    int  XSize = _GetXSizePhysEx(i);
    int  YSize = _GetYSizePhysEx(i);
    int  BPP   = LCD_GetBitsPerPixel_L0Ex(i);
    int  FixedPalette = LCD_GetFixedPaletteEx(i);
    HWND hWnd;
    wsprintf(acTitle, "LCD #%d %d*%d %dbpp, FixedPalette %d", i, XSize, YSize, BPP, FixedPalette);
    hWnd = CreateWindow(acClassNameLCD, acTitle,
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
                        | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CHILD | WS_CLIPSIBLINGS,
                        20 * i, 20 * i, XSize * _MagX + 2 * xSizeFrame, YSize * _MagY + ySizeCaption + 2 * ySizeFrame,
                        _hWndMain, NULL, _hInst, NULL);
    SetWindowLong(hWnd, GWL_USERDATA, i);
    ShowWindow(hWnd, SW_SHOW);
    _CreateWndLCDInfo(i);
  }
}

/*********************************************************************
*
*       About box
*/
static LRESULT CALLBACK _About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_INITDIALOG:
    {
      char acBuffer[80];
      sprintf(acBuffer, "About %s", BRANDING_GetAppNameLong());
      SetWindowText(hDlg, acBuffer);
      SetDlgItemText(hDlg, IDC_APPNAME, BRANDING_GetAppNameShort());
      sprintf(acBuffer, "Version: %s", GUI_GetVersionString());
      SetDlgItemText(hDlg, IDC_VERSION, acBuffer);
      SetDlgItemText(hDlg, IDC_COPYRIGHT, BRANDING_GetCopyright());
    }
    return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return TRUE;
    }
    break;
  }
  return FALSE;
}

/*********************************************************************
*
*       _MainWnd_Command
*/
static int _MainWnd_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int wmId    = LOWORD(wParam); 
//  int wmEvent = HIWORD(wParam);
// Parse the menu selections:
  switch (wmId) {
    case ID_EDIT_COPY:                _OnCopy(0);                                                          break;  // TBD: Use LayerIndex
    case ID_VIEW_LCD:                 _CreateWndLCD();                                                     break;
    case ID_VIEW_LCDINFO:             _CreateWndLCDInfo(0);                                                break;
    case IDM_ABOUT:                   DialogBox(_hInst, (LPCTSTR)IDD_ABOUTBOX, _hWndMain, (DLGPROC)_About); break;
    case IDM_EXIT:                    DestroyWindow(hWnd);                                                 break;
    case ID_FILE_STARTAPPLICATION:    _THREAD_StartApplication();                                          break;
    case ID_FILE_STOPAPPLICATION:     _THREAD_StopApplication();                                           break;
    case ID_FILE_CONTINUEAPPLICATION: _THREAD_ContinueApplication();                                       break;
    case ID_VIEW_LOG:                 _LOG_Create(_hInst, hWnd);                                           break;
    default: return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _HandleKeyEvents
*/
void _HandleKeyEvents(UINT Msg, WPARAM wParam) {
  int Key;
  switch (Msg) {
    case WM_KEYUP:
      Key = _VirtKey2Key(wParam);
      if (Key) {
        GUI_StoreKeyMsg(Key, 0);
      } else {
        if ((Key = _Keydown2ASCII(wParam)) != 0) {
          GUI_StoreKeyMsg(Key, 0);
        }
      }
      break;
    case WM_KEYDOWN:
      Key = _VirtKey2Key(wParam);
      if (Key) {
        GUI_StoreKeyMsg(Key, 1);
      } else {
        if ((Key = _Keydown2ASCII(wParam)) != 0) {
          GUI_StoreKeyMsg(Key, 1);
        }
      }
      break;
  }
}

/*********************************************************************
*
*       _WndProcDevice
*/
static LRESULT CALLBACK _WndProcDevice(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;
  RECT r;
  static int CaptureOn = 0;
  static int xPosOld, yPosOld;
  int xPos = (signed short)LOWORD(lParam);  // horizontal position of cursor
  int yPos = (signed short)HIWORD(lParam);  // vertical position of cursor
  _HandleKeyEvents(message, wParam);
  switch (message) {
  case WM_CREATE:
    CreateWindow(acClassNameLCD, "LCD window", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
                           _xPosLCD, _yPosLCD, _GetXSizePhys() * _MagX, _GetYSizePhys() * _MagY,
                           hWnd, NULL, _hInst, NULL);
    _SetBitmapRegion(hWnd, _ahBmpDevice[0], _rgbTransparent, 0);
    _FindHardkeys(_ahBmpDevice[1], _rgbTransparent);
    { int i;
      for (i = 0; i < countof(_aHardkey); i++) {
        if ((_aHardkey[i].x1 > _aHardkey[i].x0) && (_aHardkey[i].y1 > _aHardkey[i].y0)) {
          HWND hWndHardkey = CreateWindow(acClassNameHardkey, "Hardkey", WS_CHILD | WS_CLIPSIBLINGS
                                          | WS_VISIBLE,
                                          _aHardkey[i].x0,  _aHardkey[i].y0,
                                          _aHardkey[i].x1 - _aHardkey[i].x0 + 1,
                                          _aHardkey[i].y1 - _aHardkey[i].y0 + 1,
                                          hWnd, NULL, _hInst, NULL);
          SetWindowLong(hWndHardkey, GWL_USERDATA, i);
        }
      }
    }
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps); {
      HDC hdcImage = CreateCompatibleDC(hdc); {
        SelectObject(hdcImage, _ahBmpDevice[0]);
        BitBlt(hdc, 0, 0, 1000, 1000, hdcImage, 0, 0, SRCCOPY);
        } DeleteDC(hdcImage);
    } EndPaint(hWnd, &ps);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
// Handle mouse events
  case WM_RBUTTONDOWN:
    { POINT Point;
      Point.x = xPos;
      Point.y = yPos;
      ClientToScreen(hWnd, &Point);
      TrackPopupMenu(_hMenuPopup, TPM_RIGHTBUTTON, Point.x, Point.y, 0, hWnd, NULL);
    }
    break;
// Handle mouse events
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    CaptureOn = 1;
    xPosOld = xPos;
    yPosOld = yPos;
    break;
  case WM_LBUTTONUP:
    ReleaseCapture();
    CaptureOn =0;
    break;
  case WM_COMMAND:
    return _MainWnd_Command(hWnd, message, wParam, lParam);
  case WM_MOUSEMOVE:
    GetWindowRect(hWnd, &r);
    if (CaptureOn) {
      int xDiff, yDiff;
      xDiff = xPos - xPosOld;
      yDiff = yPos - yPosOld;
      if (xDiff | yDiff) {
        MoveWindow(hWnd, r.left + xDiff, r.top + yDiff, r.right - r.left, r.bottom - r.top, 1);
      }
    }
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _WndProcHardkey
*/
static LRESULT CALLBACK _WndProcHardkey(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;
  int xPos = (signed short)LOWORD(lParam);  // horizontal position of cursor
  int yPos = (signed short)HIWORD(lParam);  // vertical position of cursor
  unsigned int Index    = GetWindowLong(hWnd, GWL_USERDATA);
  switch (message) {
  case WM_CREATE:
    _SetBitmapRegion(hWnd, _ahBmpDevice[1], _rgbTransparent, 1);
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps); {
      HDC hdcImage = CreateCompatibleDC(hdc); {
        if (Index < countof(_aHardkey)) {
          SelectObject(hdcImage, _ahBmpDevice[_aHardkey[Index].IsPressed]);
          BitBlt(hdc, 0, 0, 1000, 1000, hdcImage, _aHardkey[Index].x0, _aHardkey[Index].y0, SRCCOPY);
        }
      } DeleteDC(hdcImage);
    } EndPaint(hWnd, &ps);
    break;
// Handle mouse events
  case WM_RBUTTONDOWN:
    { POINT Point;
      Point.x = xPos;
      Point.y = yPos;
      ClientToScreen(hWnd, &Point);
      TrackPopupMenu(_hMenuPopup, TPM_RIGHTBUTTON, Point.x, Point.y, 0, _hWndMain, NULL);
    }
    break;
// Handle mouse events
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    if (Index < countof(_aHardkey)) {
      SIM_HARDKEY_CB* cb = _aHardkey[Index].pfCallback;
      if (_aHardkey[Index].Mode == 0) {
        _aHardkey[Index].IsPressed = 1;
      } else {
        _aHardkey[Index].IsPressed ^= 1;
      }
      if (cb) {
        (*cb)(Index, 1);
      }
      InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
  case WM_LBUTTONUP:
    ReleaseCapture();
    if (Index < countof(_aHardkey)) {
      SIM_HARDKEY_CB* cb = _aHardkey[Index].pfCallback;
      if (_aHardkey[Index].Mode == 0) {
        _aHardkey[Index].IsPressed = 0;
      }
      if (cb) {
        (*cb)(Index, 0);
      }
      InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _WndProcLCDInfo
*/
static LRESULT CALLBACK _WndProcLCDInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hDC;
  PAINTSTRUCT ps;
  int NumColors, DeltaMode;
  int xSize = 32;
  int ySize = 16;
  static U16 aBits[32*16];
  static BITMAPINFO BitmapInfo;
  static int BitmapCreated;
  unsigned int LayerIndex = GetWindowLong(hWnd, GWL_USERDATA);
  DeltaMode = LCD_GetDeltaModeEx(LayerIndex);
  if (DeltaMode) {
    NumColors = 1 << LCD_GetBitsPerPixel_L0Ex(LayerIndex);
  } else {
    NumColors = LCD_GetNumColorsEx(LayerIndex);
  }
  switch (message) {
  case WM_PAINT:
    {
      hDC = BeginPaint(hWnd, &ps); {
        if (NumColors <= 256) {
          int i;
          for (i = 0; i < NumColors; i++) {
            int x0 =     10 * (i % 16);
            int y0 = 2 + 10 * (i / 16);
            int x1 = x0 + 8;
            int y1 = y0 + 8;
            HGDIOBJ hbPrev;
            U32 Color = LCDSIM_Index2Color(i, LayerIndex);
            HBRUSH hb = CreateSolidBrush(RGB(Color&255,      // Red component
                                             255&(Color>>8), // Green component
                                             Color>>16));    // Blue component
            hbPrev = SelectObject(hDC, hb);
            Rectangle (hDC, x0, y0, x1, y1);
            SelectObject(hDC, hbPrev);
            DeleteObject(hb);
          }
        } else {
          RECT Rect;
          if (!BitmapCreated) {
            memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
            memset(aBits, 0, sizeof(aBits));
            BitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
            BitmapInfo.bmiHeader.biWidth       = 32;
            BitmapInfo.bmiHeader.biHeight      = 16;
            BitmapInfo.bmiHeader.biPlanes      = 1;
            BitmapInfo.bmiHeader.biBitCount    = 16;
            BitmapInfo.bmiHeader.biCompression = BI_RGB;
            {
              int Color;
              for (Color = 0; Color < 8; Color++) {
                U8 r = (Color & 0x01) >> 0;
                U8 g = (Color & 0x02) >> 1;
                U8 b = (Color & 0x04) >> 2;
                int x;
                for (x = 0; x < xSize; x++) {
                  int xx = 31 - x;
                  int rr = r * xx;
                  int gg = g * xx;
                  int bb = b * xx;
                  U16 BlackValue = (rr << 10) + (gg << 5) +  bb;
                  U16 WhiteValue = ((r ? 0x1f : x) << 10) + ((g ? 0x1f : x) <<  5) +  (b ? 0x1f : x);
                  int BlackIndex = Color * 2 * xSize + x;
                  int WhiteIndex = BlackIndex + xSize;
                  aBits[BlackIndex] = BlackValue;
                  aBits[WhiteIndex] = WhiteValue;
                }
              }
            }
            BitmapCreated = 1;
          }
          GetClientRect(hWnd, &Rect);
          SetStretchBltMode(hDC, COLORONCOLOR);
          StretchDIBits(hDC,            // hDC
                        0,              // DestX
                        0,              // DestY
                        Rect.right,     // nDestWidth
                        Rect.bottom,    // nDestHeight
                        0,              // SrcX
                        0,              // SrcY
                        xSize,          // wSrcWidth
                        ySize,          // wSrcHeight
                        aBits,          // lpBits
                        &BitmapInfo,    // lpBitsInfo
                        DIB_RGB_COLORS, // wUsage
                        SRCCOPY);
        }
      } EndPaint(hWnd, &ps);
    }
    break;
  case WM_TIMER:
    if (_aLUTModifyCnt[LayerIndex] != LCDSIM_GetModifyCntInfo(LayerIndex)) {
      _aLUTModifyCnt[LayerIndex] = LCDSIM_GetModifyCntInfo(LayerIndex);
      InvalidateRect(hWnd, NULL, FALSE);
    }
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _InitMenu
*/
static void _InitMenu(HMENU hMenu) {
  int EnableStop      = (_THREAD_IsRunning() & !_THREAD_IsSuspended()) ? 0 : MF_GRAYED;
  int EnableContinue  = (_THREAD_IsRunning() &  _THREAD_IsSuspended()) ? 0 : MF_GRAYED;
  EnableMenuItem(hMenu, ID_FILE_STOPAPPLICATION,     MF_BYCOMMAND|EnableStop);
  EnableMenuItem(hMenu, ID_FILE_CONTINUEAPPLICATION, MF_BYCOMMAND|EnableContinue);
}

/*********************************************************************
*
*       _MainWnd_OnTimer
*/
static void _MainWnd_OnTimer(HWND hWnd) {
  static char acTitle[200];
  static char acTitleNew[200];
  strcpy(acTitleNew, BRANDING_GetAppNameLong());
  if (_THREAD_IsRunning()) {
    if (_THREAD_IsSuspended()) {
      strcat(acTitleNew, "(Suspended)");
    } else {
      strcat(acTitleNew, "(Executing)");
    }
  } else {
    strcat(acTitleNew, "(Terminated)");
  }
  if (strcmp(acTitle, acTitleNew)) {
    strcpy(acTitle, acTitleNew);
    SetWindowText(hWnd, acTitle);
  }
}

/*********************************************************************
*
*       _MainWnd_Paint
*/
static void _MainWnd_Paint(HWND hWnd) {
  PAINTSTRUCT ps;
  HDC hdc, hdcImage;
  hdc = BeginPaint(hWnd, &ps); {
    RECT r;
    GetClientRect(hWnd, &r);
    hdcImage = CreateCompatibleDC(hdc); {
      int x0, y0;
      HBITMAP hBitmap = LoadBitmap(_hInst, (LPCTSTR) IDB_LOGO);
      BITMAP Bitmap;
      GetObject(hBitmap, sizeof(Bitmap), &Bitmap);
      DeleteObject(hBitmap);
      x0 = (r.right  - r.left-Bitmap.bmWidth) / 2;
      y0 = (r.bottom - r.top-Bitmap.bmHeight) / 2;
      SelectObject(hdcImage, _hLogo);
      BitBlt(hdc, x0, y0, Bitmap.bmWidth, Bitmap.bmHeight, hdcImage, 0, 0, SRCCOPY);
    } DeleteDC(hdcImage);
  } EndPaint(hWnd, &ps);
}

/*********************************************************************
*
*       _WndProcMain
*/
static LRESULT CALLBACK _WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int r =0;
  _HandleKeyEvents(message, wParam);
  switch (message) {
  case WM_COMMAND:
    r = _MainWnd_Command(hWnd, message, wParam, lParam);
    break;
  case WM_PAINT: _MainWnd_Paint(hWnd); break;
  case WM_TIMER: _MainWnd_OnTimer(hWnd); break;
  case WM_CREATE:
    SetTimer  (hWnd, 0, 20, NULL);
    break;
  case WM_DESTROY:
    KillTimer  (hWnd, 0);
    PostQuitMessage(0);
    break;
  case WM_INITMENU: _InitMenu((HMENU)wParam); break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return r;
}

/*********************************************************************
*
*       _TranslateAccelerator

This function does basically the same thing as the WIN32 function
of the same name (without underscore)

*/
static int _TranslateAccelerator(HWND hWnd, HACCEL hAcc, MSG* pMsg) {
  if (pMsg->message == WM_KEYDOWN) {
    int i, NumAccels;
    ACCEL aAccel[20];
    NumAccels = CopyAcceleratorTable(hAcc, aAccel, 20);
    for (i = 0; i < NumAccels; i++) {
      int falt0 = (aAccel[i].fVirt & FALT) ? 1 : 0;
      int falt1 = pMsg->lParam & (1<<29) ? 1 : 0;
      if ((falt0 == falt1) && (pMsg->wParam == (WPARAM)aAccel[i].key)) {
        SendMessage(hWnd, WM_COMMAND, aAccel[i].cmd, 0);
        return 1;   // Message handled
      }
    }
  }
  return 0;   // Message no handled
};
 
/*********************************************************************
*
*       _WinMain_NoClean
*/
static int _WinMain_NoClean(HINSTANCE hInstance, 
                           HINSTANCE hPrevInstance, 
                           LPSTR lpCmdLine, 
                           int nCmdShow) {
  char *sErr;
  MSG msg;
  HACCEL hAccelTable;
  _pCmdLine = lpCmdLine;
  if (strlen(lpCmdLine)) {
    _MessageBoxOnError = 0;
  }
  _timeStartup = timeGetTime();
  _hInst = hInstance;
  _RegisterClasses();
  //
  // Load Resources
  //
  _hLogo = LoadImage(_hInst, (LPCTSTR) IDB_LOGO, IMAGE_BITMAP,  0, 0, 0);
  _ahBmpDevice[0] = (HBITMAP)LoadImage(_hInst, "Device.bmp",  IMAGE_BITMAP,  0, 0, LR_LOADFROMFILE);
  _ahBmpDevice[1] = (HBITMAP)LoadImage(_hInst, "Device1.bmp", IMAGE_BITMAP,  0, 0, LR_LOADFROMFILE);
  if (_ahBmpDevice[0] ==0)
    _ahBmpDevice[0] = (HBITMAP)LoadImage(_hInst, (LPCTSTR) IDB_DEVICE, IMAGE_BITMAP,  0, 0, 0);
  if (_ahBmpDevice[1] ==0)
    _ahBmpDevice[1] = (HBITMAP)LoadImage(_hInst, (LPCTSTR) IDB_DEVICE+1, IMAGE_BITMAP,  0, 0, 0);
  _hMenuPopup = LoadMenu(_hInst, (LPCSTR)IDC_SIMULATION_POPUP);
  _hMenuPopup = GetSubMenu(_hMenuPopup,0);
  //
  // Init LCD simulation
  //
  sErr = LCDSIM_Init();
  if (sErr) {
    _MessageBox1(sErr);
    return 1;
  }
  _LOG_y0 = _GetYSizePhys()+30;
  /* Use device simulation or standard window */
/*  if (_ahBmpDevice[0] && (_xPosLCD >= 0)) {
    BITMAP bmpDevice;
    GetObject(_ahBmpDevice[0], sizeof(bmpDevice), &bmpDevice);
    _hWndMain = CreateWindowEx(0, // Extended style -> Use WS_EX_TOPMOST if you want window to stay on top
                              acClassNameDevice, "Target device",
                              WS_CLIPCHILDREN | WS_POPUP| WS_VISIBLE,
                              10, 20, bmpDevice.bmWidth, bmpDevice.bmHeight, 0, NULL, _hInst, NULL);
  } else {
    _hWndMain = CreateWindow(acClassNameMain, BRANDING_GetAppNameLong(),
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT, 0, 
                             _GetXSizePhys() + 250, 
                             _GetYSizePhys() + 150,
                             NULL, NULL, hInstance, NULL);
    if (!_hWndMain)
      return FALSE;
    _CreateWndLCD();
    _LOG_Create(_hInst, _hWndMain);
  }*/
//////houhh 20061023...
    _hWndMain = CreateWindow(acClassNameMain, BRANDING_GetAppNameLong(),
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT, 0, 
                             _GetXSizePhys() + 250, 
                             _GetYSizePhys() + 150,
                             NULL, NULL, hInstance, NULL);
    if (!_hWndMain)
      return FALSE;
    _CreateWndLCD();
    _LOG_Create(_hInst, _hWndMain);
//////
  ShowWindow(_hWndMain, 1);
  _THREAD_StartApplication();
  hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SIMULATION);
  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0)) {
    if (!_TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  _THREAD_KillAll();      // Kill background thread
  return msg.wParam;
}

/*********************************************************************
*
*       SIM_HARDKEY_ ... functions
*
**********************************************************************

The following routines are available for the simulated application.
The routines are "C" linked.

*/
/*********************************************************************
*
*       SIM_HARDKEY_GetState
*/
int  SIM_HARDKEY_GetState(unsigned int i) {
  if (i > countof (_aHardkey))
    return 0;
  return _aHardkey[i].IsPressed;
}

/*********************************************************************
*
*       SIM_HARDKEY_SetState
*/
int  SIM_HARDKEY_SetState(unsigned int i, int State) {
  int r;
  if (i > countof (_aHardkey)) {
    return 0;
  }
  r = _aHardkey[i].IsPressed;
  _aHardkey[i].IsPressed = State;
  return r;
}

/*********************************************************************
*
*       SIM_HARDKEY_GetNum
*/
int  SIM_HARDKEY_GetNum(void) {
  return _NumHardkeys;
}

/*********************************************************************
*
*       SIM_HARDKEY_SetCallback
*/
SIM_HARDKEY_CB*  SIM_HARDKEY_SetCallback(unsigned int KeyIndex, SIM_HARDKEY_CB* pfCallback) {
  SIM_HARDKEY_CB* r;
  if (KeyIndex > countof (_aHardkey)) {
    return 0;
  }
  r = _aHardkey[KeyIndex].pfCallback;
  _aHardkey[KeyIndex].pfCallback = pfCallback;
  return r;
}

/*********************************************************************
*
*       SIM_HARDKEY_SetMode
*/
int  SIM_HARDKEY_SetMode (unsigned int KeyIndex, int Mode) {
  int r;
  if (KeyIndex > countof (_aHardkey)) {
    return 0;
  }
  r = _aHardkey[KeyIndex].Mode;
  _aHardkey[KeyIndex].Mode = Mode;
  return r;
}

/*********************************************************************
*
*       SIM_ ... functions
*
**********************************************************************

The following routines are available for the simulated application.
The routines are "C" linked.

*/
/*********************************************************************
*
*       SIM_SetTransColor
*/
int SIM_SetTransColor(int Color) {
  int r = _rgbTransparent;
  _rgbTransparent = Color;
  return r;
}

/*********************************************************************
*
*       SIM_SetLCDColorWhite
*/
int SIM_SetLCDColorWhite(unsigned int Index, int Color) {
  int r =0;
  if (Index < countof(LCDSIM_aLCDColorWhite)) {
    r = LCDSIM_aLCDColorWhite[Index];
    LCDSIM_aLCDColorWhite[Index] = Color;
  }
  return r;
}

/*********************************************************************
*
*       SIM_SetLCDColorBlack
*/
int SIM_SetLCDColorBlack(unsigned int Index, int Color) {
  int r =0;
  if (Index < countof(LCDSIM_aLCDColorBlack)) {
    r = LCDSIM_aLCDColorBlack[Index];
    LCDSIM_aLCDColorBlack[Index] = Color;
  }
  return r;
}

/*********************************************************************
*
*       SIM_SetMag
*/
void SIM_SetMag(int MagX, int MagY) {
  if (_MagX != 0) {
    _MagX = MagX;
  }
  if (_MagY != 0) {
    _MagY = MagY;
  }
}

/*********************************************************************
*
*       SIM_GetMagX, SIM_GetMagY
*/
int      SIM_GetMagX(void) { return _MagX; }
int      SIM_GetMagY(void) { return _MagY; }

/*********************************************************************
*
*       SIM_SetLCDPos
*/
void  SIM_SetLCDPos(int x, int y) {
  _xPosLCD = x;
  _yPosLCD = y;
}

/*********************************************************************
*
*       SIM_GetTime
*/
int SIM_GetTime(void) { return timeGetTime() - _timeStartup; }

/*********************************************************************
*
*       SIM_Delay
*/
void SIM_Delay(int ms) {
  _THREAD_Sleep(ms);
  SIM_GetTime();
}

/*********************************************************************
*
*       SIM_ExecIdle
*/
void SIM_ExecIdle(void) {
  _THREAD_Sleep(1);
}

/*********************************************************************
*
*       SIM_WaitKey
*/
int SIM_WaitKey(void) {
  int r;
  SIM_Log("\nSIM_WaitKey()");
  while (_KeyBuffer == 0) {
    GUI_Delay(10);
  }
  r = _KeyBuffer;
  _KeyBuffer =0;
  SIM_Log(" Done.");
  return r;
}

/*********************************************************************
*
*       SIM_GetKey
*/
int SIM_GetKey(void) {
  int r;
  r = _KeyBuffer;
  if (r)
    _KeyBuffer=0;
  return r;
}

/*********************************************************************
*
*       SIM_StoreKey
*/
void SIM_StoreKey(int Key) {
  if (!_KeyBuffer)
    _KeyBuffer = Key;
}

/*********************************************************************
*
*       SIM_Log
*/
void SIM_Log(const char *s) {
  OutputDebugString(s);
  _LogTime();
  _LOG_Add(s);
}

/*********************************************************************
*
*       SIM_Warn
*/
void SIM_Warn(const char *s) {
  _LogTime();
  _LOG_AddRed();
  _LOG_Add(s);
}

/*********************************************************************
*
*       SIM_ErrorOut
*/
void SIM_ErrorOut(const char *s) {
  static int Cnt;
  _LogTime();
  _LOG_AddRed();
  _LOG_Add(s);
  if (_MessageBoxOnError) {
    Cnt++;
    _MessageBox1(s);
  }
  _SendToErrorFile(s);
}

/*********************************************************************
*
*       SIM_EnableMessageBoxOnError
*/
void SIM_EnableMessageBoxOnError(int Status) {
  _MessageBoxOnError = Status;
}

/*********************************************************************
*
*       SIM_GetCmdLine
*/
const char * SIM_GetCmdLine(void) {
  return _pCmdLine;
}

/*********************************************************************
*
*       SIM_CreateTask
*/
void SIM_CreateTask(char * pName, void * pFunc) {
  if (_NumTask < countof(_ahThread)) {
    _ahThread[_NumTask] = CreateThread(NULL, 0, _CreateTask, pFunc, CREATE_SUSPENDED, &_aThreadID[_NumTask]);
    SetThreadPriority(_ahThread[_NumTask], THREAD_PRIORITY_LOWEST);
    ResumeThread(_ahThread[_NumTask]);
    _NumTask++;
  }
}

/*********************************************************************
*
*       SIM_Start
*/
void SIM_Start(void) {
  while(1)
    Sleep(10);
}

/*********************************************************************
*
*       WinMain
*
**********************************************************************
*/
#if (SIM_WINMAIN)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow) {
  int r;
  SIM_X_Init();
  r = _WinMain_NoClean(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
  if (_hWndLCD1)
    DestroyWindow(_hWndLCD1);
  if (_ahBmpDevice[0])
    DeleteObject(_ahBmpDevice[0]);  
  if (_ahBmpDevice[1])
    DeleteObject(_ahBmpDevice[1]);
  if (_hLogo) 
    DeleteObject(_hLogo);
  if (_hFileError)
    CloseHandle(_hFileError);
  return r;
}
#endif

/*************************** End of file ****************************/
