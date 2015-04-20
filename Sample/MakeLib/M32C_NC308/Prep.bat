@ECHO OFF
goto Start

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Mitsubishi NC308 Compiler for M32C targets.
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START

@ECHO OFF

ECHO Prep.bat:            Preparing environment

if "%_PREP308_%" == "_PREP308_" goto cont
set _PREP308_=_PREP308_

SET TMPPATH=%PATH%
IF EXIST C:\MTOOL\BIN\NC308.EXE SET TOOLPATH=C:\MTOOL
IF EXIST C:\TOOL\C\MITSUBISHI\NC308WA310R2\BIN\NC308.EXE SET TOOLPATH=C:\TOOL\C\MITSUBISHI\NC308WA310R2
SET PATH=%TOOLPATH%\BIN;%TOOLPATH%\LIB30;%PATH%

:cont

SET BIN308=%TOOLPATH%\BIN
SET INC308=%TOOLPATH%\INC308
SET LIB308=%TOOLPATH%\LIB308
SET TMP308=%TOOLPATH%\TMP
