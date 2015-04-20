@ECHO OFF
GOTO START

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Keil C51 Compiler for 8051 targets.
*
******************************************************************************

:START

ECHO Prep.bat:            Preparing environment
REM Next line avoids setting of path

IF "%_PREP_KEIL_%" == "_PREP_KEIL_" GOTO END

SET _PREP_KEIL_=_PREP_KEIL_
SET PATH=%PATH%;C:\PROGRAM FILES\KEIL\C51\BIN;

:END

