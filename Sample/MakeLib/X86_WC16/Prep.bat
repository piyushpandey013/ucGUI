@ECHO OFF
ECHO Prep.bat:            Preparing environment
goto Start

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Watcom Compile for x86 targets.
* It needs to be modified if a library for a target compiler is
* desired or if the compiler is installed in a different location.
*
******************************************************************************

:START

IF "%_PREP_WATCOM_%" == "_PREP_WATCOM_" GOTO CONT
call C:\Tool\C\watcom\SetVars.bat
Set path=C:\Tool\C\watcom\binnt;C:\Tool\C\watcom\binw;%path%

SET _PREP_WATCOM_=_PREP_WATCOM_


:CONT


