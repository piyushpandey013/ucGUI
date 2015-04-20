@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the GHS Compiler for V850 targets.
*
* It needs to be modified if the compiler is installed in a different location.
*
******************************************************************************

:START

ECHO Prep.bat:            Preparing environment
del _MAKELIB.bat
if "%_PREP_%" == "_PREP_" goto end
set _PREP_=_PREP_
set PATH=C:\tool\c\ghs\v850_m2k;%PATH%;
:end
