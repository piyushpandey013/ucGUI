@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Tasking Compiler for Mitsubishi M16C targets.
*
******************************************************************************

:START
CM16 Temp\Source\%1.c -Ml -T -s -w196 -O1 -A1 -gn -Cm16c62
IF ERRORLEVEL 1 PAUSE
MOVE %1.src Temp\Output\

ASM16 Temp\Output\%1.src -gAHLs -Cm16c62
IF ERRORLEVEL 1 PAUSE
MOVE %1.obj Temp\Output\

ECHO Temp\Output\%1.obj>>Temp\Output\Lib.dat

