@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the GHS Compiler for V850 targets.
*
******************************************************************************

:START
REM -Ospeed

echo CC.BAT:       Compiling %1 %2 %3 %4 %5 %6 %7 %8
@ccv850 -zda=all -list=temp\%1.lst -c -I.\start\inc\ -I.\cpu\ -noobj -noasmwarn  -nofarcalls -tmp=%temp% -Xc -Xneedprototype Temp\Source\%1.c %3 %4 %5 %6 %7 %8 %9
if errorlevel 1 goto ERR
@ECHO ax cr output\GUI.a %1.o>>_Makelib.bat
goto Done

:ERR
ECHO CC.BAT:       Error building library, build stopped
Pause
:Done

