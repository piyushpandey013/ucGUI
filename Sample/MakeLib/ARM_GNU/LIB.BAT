@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file is written for IAR ARM tool chain
*
******************************************************************************
:START

ECHO MAKELIB.BAT:         Creating GUI target library using IAR ARM tool-chain

REM ****************************************
REM   Create library
REM ****************************************

IF EXIST Lib\GUI_GNU.LIB DEL Lib\GUI_GNU.LIB
@ECHO save>>temp\lib.dat
@ECHO end>>temp\lib.dat
arm-uclibc-ar.exe -M <temp/LIB.DAT
IF ERRORLEVEL 1 PAUSE
