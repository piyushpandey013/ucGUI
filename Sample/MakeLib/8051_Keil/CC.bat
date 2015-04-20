@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Keil C51 Compiler for 8051 targets.
*
******************************************************************************

LARGE             : Defines large memory model
OPTIMIZE (4,SIZE) : Optimization level
INCDIR            : Include path
DEBUG             : Include debug information
OBJECTEXTEND      : Include extended debug information
PRINT             : Generates list file

:START

ECHO CC.bat:     Compiling %1.c
C51 .\Temp\Source\%1.c LARGE OPTIMIZE (4,SIZE) INCDIR (Config\;GUI\Core\;GUI\WM\) DEBUG OBJECTEXTEND PRINT(.\Temp\Output\%1.lst) OBJECT(.\Temp\Output\%1.obj)
IF ERRORLEVEL 1 PAUSE
IF NOT EXIST Temp\Lib1.bat ECHO LIB51 CREATE Lib\GUI.LIB>>Temp\Lib1.bat
ECHO LIB51 ADD Temp\Output\%1.OBJ TO Lib\GUI.LIB>>Temp\Lib1.bat
