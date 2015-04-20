@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the GNU ARM Compiler
*
******************************************************************************

:START

ECHO CC.BAT:              Compiling %1.c with GNU ARM compiler
arm-uclibc-gcc -D TARGET_AT91 -D OS_LIBMODE_SP -c -mcpu=arm7tdmi -MD -Wall -O3 -mapcs-frame -mthumb-interwork -o temp/Output/%1.o temp/Source/%1.c
IF ERRORLEVEL 1 PAUSE
IF NOT EXIST temp\Lib.dat @ECHO create lib/GUI_GNU.lib>temp\Lib.dat
@ECHO addmod temp/output/%1.o>>temp/lib.dat

