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

if "%_PREP_ARM_GNU_%" == "_PREP_ARM_GNU_" goto cont
set _PREP_ARM_GNU_=_PREP_ARM_GNU_

SET TOOLPATH=C:\Cygnus\arm-tools\
set PATH=%TOOLPATH%\bin;%PATH%
SET C_INCLUDE=%TOOLPATH%\include;
:cont

