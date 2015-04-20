@ECHO OFF
goto Start

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the Tasking Compiler for Mitsubishi M16C targets.
*
******************************************************************************

:START

ECHO -C Lib\GUI>Temp\Output\PARA.DAT
REM COPY Temp\Output\PARA.DAT+Temp\Output\Lib.dat Temp\Output\LINK.DAT
REM LB308 @Temp\Output\LINK.DAT
ARM16 -r Lib\GUI.A -f Temp\Output\Lib.dat

IF ERRORLEVEL 1 PAUSE

