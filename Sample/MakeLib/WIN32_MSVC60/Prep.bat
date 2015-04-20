@ECHO OFF
ECHO Prep.bat:            Preparing environment
goto Start

******************************************************************************
*
* File      : Prep.bat
* Parameters: None
* Purpose   : Sets path and other environment variables as required by tool chain
*
* This file as is uses the Microsoft Compiler MSVC 6.0 for x86 targets.
* It needs to be modified if a library for a target compiler is
* desired or if the compiler is installed in a different location.
*
******************************************************************************

:START

REM Next line avoids multiple setting of path
IF "%_PREP_VC60_%" == "_PREP_VC60_" GOTO CONT

SET _PREP_VC60_=_PREP_VC60_
SET VSCommonDir=C:\Program Files\Microsoft Visual Studio\Common
SET MSDevDir=C:\Program Files\Microsoft Visual Studio\Common\msdev98
SET MSVCDir=C:\Program Files\Microsoft Visual Studio\VC98
SET VcOsDir=WIN95

IF "%OS%" == "Windows_NT" SET VcOsDir=WINNT
IF "%OS%" == "Windows_NT" SET PATH=Temp;%MSDevDir%\BIN;%MSVCDir%\BIN;%VSCommonDir%\TOOLS\%VcOsDir%;%VSCommonDir%\TOOLS;%PATH%
IF "%OS%" == ""           SET PATH="%MSDevDir%\BIN";"%MSVCDir%\BIN";"%VSCommonDir%\TOOLS\%VcOsDir%";"%VSCommonDir%\TOOLS";"%windir%\SYSTEM";"%PATH%"

:CONT

SET INCLUDE=%MSVCDir%\INCLUDE;Temp\Source
SET LIB=%MSVCDir%\LIB;%MSVCDir%\MFC\LIB;%LIB%

