@ECHO OFF

ECHO CleanUp.BAT:         Deleting output

REM ****************************************
REM   Delete working folders
REM ****************************************

IF "%OS%" == "Windows_NT" GOTO WinNT
FOR %%i IN (Output, Source, Debug, Release) DO DELTREE %%i
GOTO CONT2
:WinNT
FOR %%i IN (Output, Source, Debug, Release) DO IF EXIST %%i RD %%i /S/Q
:CONT2

REM ****************************************
REM   Delete files
REM ****************************************

FOR %%i IN (OPT, PLG, APS, NCB) DO IF EXIST *.%%i DEL *.%%i
