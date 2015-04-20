@ECHO OFF
ECHO Lib.bat: Creating library
goto Start

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the Watcom Compile for x86 targets.
*
******************************************************************************

:START
wlib -q -b Lib\GUI.lib @temp\output\lib.dat

IF ERRORLEVEL 1 PAUSE

