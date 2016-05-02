
@set Build=%1
@set Options=%2

@if "%Build%"=="" goto Syntax

@pushd bin\%Build%
@REM @echo Dir: %cd%
@REM @dir
@set Path=C:\MinGW\bin;%Path%
@set DestFile=Lib_UT\UT_Results\UT_Result.txt
@set DestFileFromExe=..\..\%DestFile%

@if     "%Options%"=="--display_success"  Lib_UT.exe %Options% >      %DestFileFromExe%
@if not "%Options%"=="--display_success"  Lib_UT.exe %Options% | mtee %DestFileFromExe%


@set Build=
@set Options=
@popd
@echo.
@echo Results have been saved into: "%DestFile%"

@goto :EOF
:Syntax
@echo Syntax is: Ut_Run Debug|Release
@goto :EOF
