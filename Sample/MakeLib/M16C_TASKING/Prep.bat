@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Tasking Compiler for Mitsubishi M16C targets.
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START

@ECHO OFF

ECHO PREP.BAT:     Preparing environment

if "%_PREP_TASKING_%" == "_PREP_TASKING_" goto cont

SET _PREP_TASKING_=_PREP_TASKING_

SET TOOLPATH=C:\Tool\C\TASKING\cm16c_v22r1
SET PATH=%TOOLPATH%\bin;%PATH%

:cont
