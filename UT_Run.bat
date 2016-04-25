
@set Build=%1
@set Options=%2

@if "%Build%"=="" goto Syntax

@pushd bin\%Build%
@REM @echo Dir: %cd%
@REM @dir
@set Path=C:\MinGW\bin;%Path%

Lib_UT.exe %Options% | mtee ..\..\Lib_UT\UT_Results\UT_Result.txt

@set Build=
@set Options=
@popd

@goto :EOF
:Syntax
@echo Syntax is: Ut_Run Debug|Release
@goto :EOF
