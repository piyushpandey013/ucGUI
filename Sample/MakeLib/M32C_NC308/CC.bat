@ECHO OFF
goto Start

******************************************************************************
*
* File      : CC.bat
* Parameters: %1 Name of file to compile (without extension; .c is added)
* Purpose   : Compile one file and add it to the list of files to put in
*             Library
*
* This file as is uses the Mitsubishi NC308 Compiler for M32C targets.
*
******************************************************************************

-silent : Suppresses the copyright message display at startup
-M82    : Generates object code for M32C/80 Series (Remove this switch for M16C80 targets)
-c      : Creates a relocatable file (extension .r30) and ends processing
-I      : Specifies the directory containing the file(s) specified in #include
-dir    : Specifies the destination directory
-OS     : Maximum optimization of speed followed by ROM size
-fFRAM  : Changes the default attribute of RAM data to far
-fETI   : Performs operation after extending char-type data to the int type (Extended according to ANSI standards)

:START

NC308 -silent -M82 -c -IInc -dir Temp\Output -OS -fFRAM -fETI Temp\Source\%1.c
IF ERRORLEVEL 1 PAUSE
ECHO Temp\Output\%1.R30>>Temp\Output\Lib.dat

