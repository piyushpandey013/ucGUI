@ECHO OFF
goto Start

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the Microsoft Compiler MSVC 6.0 for x86 targets.
* This file needs to be modified if a library for a target compiler is
* desired.
*
* If you would like to do this, please get in touch with us;
* we may have a version for your target compiler.
*
******************************************************************************

:START

ECHO /nologo /out:Lib\GUI.lib>>Temp\Output\Lib.dat
LINK -lib @Temp\Output\Lib.dat
IF ERRORLEVEL 1 PAUSE

