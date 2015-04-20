@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the IAR ARM Compiler
*
******************************************************************************

:START

ECHO CC.BAT:              Compiling %1.c with IAR ARM compiler
ICCARM -D OS_LIBMODE_DP -o temp\Output\ -s9 --library_module  --cpu_mode arm --code_model small --endian little --cpu ARM7TDMI --stack_align 8 --interwork -e --require_prototypes --silent temp\Source\%1.c
IF ERRORLEVEL 1 PAUSE
ECHO temp\Output\%1.r79>>temp\Lib.DAT

