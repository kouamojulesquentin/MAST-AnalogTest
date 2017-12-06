
@set Project=%1
@set Build=%2
@set Options=%3

@if "%Project%"=="" goto Syntax
@if "%Build%"=="" goto Syntax

@set DestFile=%Project%\UT_Results\UT_Result.txt

@set Build_Dir=cmake_%Build%
@set targetDir=".\%Build_Dir%\Bin\"
@set DestFileFromExe=..\..\%DestFile%

@echo ===== Target dir: %targetDir%
@pushd %targetDir%

@if not exist %Project%.exe goto No_exe
@REM @echo Dir:             %cd%
@REM @echo DestFileFromExe: %DestFileFromExe%


@if     "%Options%"=="--display_success"  %Project%.exe %Options% >      %DestFileFromExe%
@if not "%Options%"=="--display_success"  %Project%.exe %Options% | mtee %DestFileFromExe%


@set Build=
@set Options=
@popd
@echo.
@echo Results have been saved into: "%DestFile%"

@goto :EOF
:No_exe
@echo.
@echo. ---- file %CD%\%Project%.exe does not exist! ----
@echo.
@goto :EOF
:Syntax
@echo.
@echo. Syntax is: "UT_Run ut_project_name Debug|Release [Options]"
@echo.
@goto :EOF
