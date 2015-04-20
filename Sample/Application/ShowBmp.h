#ifndef __SHOWBMP_H__
#define __SHOWBMP_H__


void ShowBMP(void* bmpcontent, int XPos, int YPos, int fromfile);
//void ShowBMP(char* bmpcontent, int XPos, int YPos);
//void ShowBMP(const char* sFilename);
char* ReadBmpFile(char* filename);

#endif //__SHOWBMP_H__