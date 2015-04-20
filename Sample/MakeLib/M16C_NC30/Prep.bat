@ECHO OFF
goto Start

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Mitsubishi NC30 Compiler for M16C targets.
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START

@ECHO OFF

ECHO Prep.bat:            Preparing environment

if "%_PREP30_%" == "_PREP30_" goto cont
set _PREP30_=_PREP30_

SET TMPPATH=%PATH%
IF EXIST C:\MTOOL\BIN\NC30.EXE SET TOOLPATH=C:\MTOOL
IF EXIST C:\TOOL\C\MITSUBISHI\NC30WA400\BIN\NC30.EXE SET TOOLPATH=C:\TOOL\C\MITSUBISHI\NC30WA400
SET PATH=%TOOLPATH%\BIN;%TOOLPATH%\LIB30;%PATH%

:cont

SET BIN30=%TOOLPATH%\BIN
SET INC30=%TOOLPATH%\INC30
SET LIB30=%TOOLPATH%\LIB30
SET TMP30=%TOOLPATH%\TMP
