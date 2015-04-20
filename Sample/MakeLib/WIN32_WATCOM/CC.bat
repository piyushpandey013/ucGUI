@ECHO OFF
goto Start

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Watcom Compile for x86 targets.
*
******************************************************************************

:START
wcc386 Temp\Source\%1.c -os -fo=temp\output\%1.obj -i=c:\tool\c\watcom\h; -i=c:\tool\c\watcom\h\nt; -w4 -e25  -zq -od -5r -bt=nt-mf -dWIN32 -d_WINDOWS 
IF ERRORLEVEL 1 PAUSE
ECHO +Temp\Output\%1.OBJ>>Temp\Output\Lib.dat

