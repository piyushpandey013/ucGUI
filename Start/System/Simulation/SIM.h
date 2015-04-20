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
File        : SIM.h
Purpose     : Declares public functions of Simulation
----------------------------------------------------------------------
*/

#ifndef SIM_H
#define SIM_H

/********************************************************************
*
*              Publics for Hardkey simulation
*
*********************************************************************
*/

typedef void SIM_HARDKEY_CB(int KeyIndex, int State);

int              SIM_HARDKEY_GetNum(void);
int              SIM_HARDKEY_GetState(unsigned int i);
SIM_HARDKEY_CB*  SIM_HARDKEY_SetCallback(unsigned int KeyIndex, SIM_HARDKEY_CB* pfCallback);
int              SIM_HARDKEY_SetMode (unsigned int KeyIndex, int Mode);
int              SIM_HARDKEY_SetState(unsigned int KeyIndex, int State);

/********************************************************************
*
*              Publics for LCD
*
*********************************************************************
*/
void     SIM_SetLCDPos(int x, int y);
int      SIM_SetTransColor(int Color);
int      SIM_SetLCDColorBlack (unsigned int Index, int Color);
int      SIM_SetLCDColorWhite (unsigned int Index, int Color);
void     SIM_SetMag(int MagX, int MagY);
int      SIM_GetMagX(void);
int      SIM_GetMagY(void);

/********************************************************************
*
*              Routine(s) in user application
*
*********************************************************************
*/
void  SIM_X_Init(void);   /* Allow init before application starts ... Use it to set LCD offset etc */

/********************************************************************
*
*              Publics used by GUI_X module
*
*********************************************************************
*/

void SIM_Delay (int ms);
void SIM_ExecIdle(void);
int  SIM_GetTime(void);
int  SIM_GetKey(void);
int  SIM_WaitKey(void);
void SIM_StoreKey(int);


/********************************************************************
*
*              Publics for logging, warning, errorout
*
*********************************************************************
*/

void SIM_Log(const char *s);
void SIM_Log1(const char *s, int p0);
void SIM_Log2(const char *s, int p0, int p1);
void SIM_Log3(const char *s, int p0, int p1, int p2);
void SIM_Log4(const char *s, int p0, int p1, int p2,int p3);
void SIM_Warn(const char *s);
void SIM_Warn1(const char *s, int p0);
void SIM_Warn2(const char *s, int p0, int p1);
void SIM_Warn3(const char *s, int p0, int p1, int p2);
void SIM_Warn4(const char *s, int p0, int p1, int p2, int p3);
void SIM_ErrorOut(const char *s);
void SIM_ErrorOut1(const char *s, int p0);
void SIM_ErrorOut2(const char *s, int p0, int p1);
void SIM_ErrorOut3(const char *s, int p0, int p1, int p2);
void SIM_ErrorOut4(const char *s, int p0, int p1, int p2, int p3);
void SIM_EnableMessageBoxOnError(int Status);

/********************************************************************
*
*              Commandline support
*
*********************************************************************
*/

const char *SIM_GetCmdLine(void);

/********************************************************************
*
*              Multitasking support
*
*********************************************************************
*/

void SIM_CreateTask(char * pName, void * pFunc);
void SIM_Start(void);
unsigned long SIM_GetTaskID(void);
void SIM_Lock(void);
void SIM_Unlock(void);
void SIM_InitOS(void);

#endif /* LCD_H */




