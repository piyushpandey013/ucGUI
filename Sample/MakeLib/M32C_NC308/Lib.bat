@ECHO OFF
goto Start

******************************************************************************
*
* File      : Lib.bat
* Parameters: None
* Purpose   : Put all (object) files in linklist into the library
*
* This file as is uses the Mitsubishi NC308 Compiler for M32C targets.
*
******************************************************************************

-C : Creates new library file
@  : Specifies command file

:START

ECHO -C Lib\GUI>Temp\Output\PARA.DAT
COPY Temp\Output\PARA.DAT+Temp\Output\Lib.dat Temp\Output\LINK.DAT
LB308 @Temp\Output\LINK.DAT

IF ERRORLEVEL 1 PAUSE

