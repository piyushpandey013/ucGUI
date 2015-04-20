@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file is written for the IAR ARM toolchain
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START
ECHO PREP.BAT:            Preparing environment for V850 IAR

if "%_PREP_ARM_IAR_%" == "_PREP_ARM_IAR_" goto cont
set _PREP_ARM_IAR_=_PREP_ARM_IAR_

SET TOOLPATH=C:\tool\C\IAR\Ew32_ARM_V3.30A\
set PATH=%TOOLPATH%\ARM\bin;%TOOLPATH%\common\bin;%PATH%
SET C_INCLUDE=%TOOLPATH%\ARM\INC;

:cont

