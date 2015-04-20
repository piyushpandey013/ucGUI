@ECHO OFF
goto Start

******************************************************************************
*
* File      : MakeLib.bat
* Parameters: 2 (optional)
*             %1: Source path   Default: GUI
*             %2: Config path   Default: Config
* Purpose   : Generate a library of the GUI
*             It takes the GUI sources and configuration files and generates
*             a library
*
* Input:      Sources:             GUI\...\*.c
*             Configuration files: Config\*.h 
* Output:     Lib\GUI.lib   (extension depends on tool chain)
*
*
* This file does not need to be modified; it does not contain any target
* dependencies.
*
******************************************************************************

:START

ECHO MakeLib.bat
REM ****************************************
REM   Prepare environment
REM ****************************************

CALL PREP.BAT

REM ****************************************
REM   Prepare working folders
REM ****************************************

IF "%OS%" == "Windows_NT" GOTO WinNT_0
DELTREE /Y Temp
DELTREE /Y Lib
GOTO CONT_0
:WinNT_0
IF EXIST Temp RD Temp    /S/Q
IF EXIST Lib  RD Lib /S/Q
:CONT_0
MD Temp
MD Temp\Source
MD Temp\Output
MD Lib

REM ****************************************
REM   Copy source files
REM ****************************************

IF NOT "%1" == "" SET SOURCE_PATH=%1
IF     "%1" == "" SET SOURCE_PATH=GUI
IF EXIST %SOURCE_PATH%\Core\GUI.h GOTO COPY_SOURCE
ECHO No GUI files!
PAUSE
:COPY_SOURCE
ECHO MAKELIB.bat:         Copying source files (*.bat, *.c, *.h) from %SOURCE_PATH%
FOR %%i IN (C, BAT)    DO IF EXIST %SOURCE_PATH%\AntiAlias\*.%%i    XCOPY %SOURCE_PATH%\AntiAlias\*.%%i    Temp\Source /Q
FOR %%i IN (C, BAT)    DO IF EXIST %SOURCE_PATH%\ConvertColor\*.%%i XCOPY %SOURCE_PATH%\ConvertColor\*.%%i Temp\Source /Q
FOR %%i IN (C, BAT)    DO IF EXIST %SOURCE_PATH%\ConvertMono\*.%%i  XCOPY %SOURCE_PATH%\ConvertMono\*.%%i  Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\Core\*.%%i         XCOPY %SOURCE_PATH%\Core\*.%%i         Temp\Source /Q
FOR %%i IN (C, BAT)    DO IF EXIST %SOURCE_PATH%\Font\*.%%i         XCOPY %SOURCE_PATH%\Font\*.%%i         Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\JPEG\*.%%i         XCOPY %SOURCE_PATH%\JPEG\*.%%i         Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\LCDDriver\*.%%i    XCOPY %SOURCE_PATH%\LCDDriver\*.%%i    Temp\Source /Q
FOR %%i IN (C, BAT)    DO IF EXIST %SOURCE_PATH%\MemDev\*.%%i       XCOPY %SOURCE_PATH%\MemDev\*.%%i       Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\MultiLayer\*.%%i   XCOPY %SOURCE_PATH%\MultiLayer\*.%%i   Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\VNC\*.%%i          XCOPY %SOURCE_PATH%\VNC\*.%%i          Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\Widget\*.%%i       XCOPY %SOURCE_PATH%\Widget\*.%%i       Temp\Source /Q
FOR %%i IN (C, H, BAT) DO IF EXIST %SOURCE_PATH%\WM\*.%%i           XCOPY %SOURCE_PATH%\WM\*.%%i           Temp\Source /Q
SET SOURCE_PATH=

REM ****************************************
REM   Copy configuration files
REM ****************************************

IF NOT "%2" == "" SET CONFIG_PATH=%2
IF     "%2" == "" SET CONFIG_PATH=Config
IF EXIST %CONFIG_PATH%\LCDConf.h GOTO COPY_CONFIG
ECHO No Config files!
PAUSE
:COPY_CONFIG
FOR %%i IN (GUIConf, LCDConf, GUITouchConf) DO IF EXIST %CONFIG_PATH%\%%i.h XCOPY %CONFIG_PATH%\%%i.h Temp\Source /Q
SET CONFIG_PATH=

REM ****************************************
REM   Compile source files
REM ****************************************

FOR %%i IN (Temp\Source\CC*.bat) DO CALL %%i

REM ****************************************
REM   Link
REM ****************************************

CALL LIB.bat

REM ****************************************
REM   Delete working folders
REM ****************************************

IF "%OS%" == "Windows_NT" GOTO WinNT_1
DELTREE /Y Temp
GOTO READY
:WinNT_1
IF EXIST Temp RD Temp /S/Q
:READY

