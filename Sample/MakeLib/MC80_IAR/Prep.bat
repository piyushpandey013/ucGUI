@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the IAR Compiler for M16C80 targets.
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START

@ECHO OFF

ECHO Prep.bat:            Preparing environment

IF "%_PREP_M16C80_%" == "_PREP_M16C80_" GOTO CONT
SET _PREP_M16C80_=_PREP_M16C80_

SET TOOLPATH=C:\TOOL\C\IAR\EW32_M32C_V211a
SET PATH=%TOOLPATH%\M32C\BIN;%TOOLPATH%\COMMON\BIN;%PATH%

:CONT

SET C_INCLUDE=%TOOLPATH%\M32C\INC;%TOOLPATH%\M32C\INC\CLIB
