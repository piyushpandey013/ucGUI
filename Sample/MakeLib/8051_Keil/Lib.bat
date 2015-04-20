@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the Keil C51 Compiler for 8051 targets.
*
******************************************************************************

:START

CALL Temp\Lib1.bat
IF ERRORLEVEL 1 PAUSE

