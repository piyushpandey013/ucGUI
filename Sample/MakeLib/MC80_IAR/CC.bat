@ECHO OFF
GOTO START

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the IAR Compiler for M16C80 targets.
*
******************************************************************************

-mf							: Memory model: far
-v1							: Processor variant: M16C/80
-s9							: Optimizes for speed: Full optimization
-o							: Sets output path
--no_inline					: Disables function inlining
--library_module 			: Makes module a library module
--warnings_affect_exit_code : Makes warnings affect the exit code
--silent 					: Specifies silent operation
-e							: Enables language extensions

:START

ECHO                      Compiling %1.c
ICCM32C -mf -v1 -s9 -o Temp\Output\ --no_inline --library_module --warnings_affect_exit_code --silent -e Temp\Source\%1.c
IF ERRORLEVEL 1 PAUSE
ECHO f-m Temp\Output\%1,Lib\GUI,,>>Temp\Output\OBJ.DAT

