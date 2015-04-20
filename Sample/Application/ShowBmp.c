
#include <windows.h>

#include "gui.h"

#include "ShowBmp.h"

typedef struct tagGUIBMP{
	char* bmpfile;
	int XPos;
	int YPos;
	DWORD FileSize;
	char* pFile;
}BMP;



void ShowBMP(void* bmpcontent, int XPos, int YPos, int fromfile)
//void ShowBMP(const char* sFilename)
{
/* 2005-11-20 21:47:32
	  int XSize, YSize, XPos, YPos;
	  DWORD NumBytesRead;
	  static HANDLE hFile = NULL;
	  static DWORD FileSize = 0;
	  static char* pFile = NULL;
	  if(hFile == NULL){
		  hFile = CreateFile(sFilename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		  FileSize = GetFileSize(hFile, NULL);
		  pFile = malloc(FileSize);
		  ReadFile(hFile, pFile, FileSize, &NumBytesRead, NULL);
	  }
	  //CloseHandle(hFile);
	  GUI_ClearRect(0, 0, 319, 239);
	  XSize = GUI_BMP_GetXSize(pFile);
	  YSize = GUI_BMP_GetYSize(pFile);
	  XPos = (XSize > 320) ? 0 : 160 - (XSize / 2);
	  YPos = (YSize > 180) ? 0 : 150 - (YSize / 2);
	  XPos = 0;
	  YPos = 0;*/
	  if(bmpcontent == NULL) return;
	  if(fromfile)
	  {
		GUI_BMP_Draw((char*)bmpcontent, XPos, YPos);

	  }else{
		GUI_DrawBitmap((GUI_BITMAP*)bmpcontent, XPos, YPos);
	  }
//	  free(pFile);
}

char* ReadBmpFile(char* filename)
{
//	char* qipanBmp = NULL, whiteBmp = NULL, blackBmp = NULL;
	char currentPath[200];
	char* lpFile = 0;
	HANDLE hFile = 0;
	unsigned int FileSize = 0, NumBytesRead = 0;
	
	GetCurrentDirectory(255, currentPath);
	if(currentPath[strlen(currentPath)-1] == '\\'){
		strcat(currentPath, filename);
	}
	else{
		strcat(currentPath, "\\");
		strcat(currentPath, filename);
	}

	hFile = CreateFile(currentPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile != INVALID_HANDLE_VALUE){
		FileSize = GetFileSize(hFile, NULL);
		lpFile = malloc(FileSize);
		ReadFile(hFile, lpFile, FileSize, &NumBytesRead, NULL);
	}

	return lpFile;
}