@ECHO OFF
goto Start

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Mitsubishi NC30 Compiler for M16C targets.
*
******************************************************************************

-silent : Suppresses the copyright message display at startup
-c      : Creates a relocatable file (extension .r30) and ends processing
-I      : Specifies the directory containing the file(s) specified in #include
-dir    : Specifies the destination directory
-OS     : Maximum optimization of speed followed by ROM size
-fFRAM  : Changes the default attribute of RAM data to far
-fETI   : Performs operation after extending char-type data to the int type (Extended according to ANSI standards)

:START

NC30 -silent -c -IInc -dir Temp\Output -OS -fFRAM -fETI Temp\Source\%1.c
IF ERRORLEVEL 1 PAUSE
ECHO Temp\Output\%1.R30>>Temp\Output\Lib.dat

