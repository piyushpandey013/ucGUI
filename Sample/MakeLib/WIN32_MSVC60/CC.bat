@ECHO OFF
goto Start

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Microsoft Compiler MSVC 6.0 for x86 targets.
* This file needs to be modified if a library for a target compiler is
* desired.
*
* If you would like to do this, please get in touch with us; we may have a
* version for your target compiler.
*
******************************************************************************

:START
CL /nologo /G5 /W3 /Ox /Oa /Ow /Og /Oi /Os /Op /Ob2 /Gy /c /FoTemp\Output\ /D WIN32 /D _WINDOWS Temp\Source\%1.c
IF ERRORLEVEL 1 PAUSE
ECHO Temp\Output\%1.OBJ>>Temp\Output\Lib.dat

