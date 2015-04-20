@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the GHS Compiler for V850 targets.
*
******************************************************************************

:START

CALL _Makelib.bat
IF ERRORLEVEL 1 PAUSE

