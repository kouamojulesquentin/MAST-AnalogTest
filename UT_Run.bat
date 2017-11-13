
@set Build=%1
@set Options=%2

@if "%Build%"=="" goto Syntax

@REM @set originalPath=%path%
@set DestFile=Mast_Core_UT\UT_Results\UT_Result.txt

@set Build_Dir=cmake_%Build%
@set targetDir=".\%Build_Dir%\Bin\"
@set DestFileFromExe=..\..\%DestFile%

@echo ===== Target dir: %targetDir%
@pushd %targetDir%

@if not exist Mast_Core_UT.exe goto No_exe
@REM @echo Dir:             %cd%
@REM @echo DestFileFromExe: %DestFileFromExe%


@if     "%Options%"=="--display_success"  Mast_Core_UT.exe %Options% >      %DestFileFromExe%
@if not "%Options%"=="--display_success"  Mast_Core_UT.exe %Options% | mtee %DestFileFromExe%


@set Build=
@set Options=
@REM @set Path=%originalPath%
@popd
@echo.
@echo Results have been saved into: "%DestFile%"

@goto :EOF
:No_exe
@echo.
@echo. ---- file %CD%\Mast_Core_UT.exe does not exist! ----
@echo.
@goto :EOF
:Syntax
@echo.
@echo. Syntax is: "UT_Run Debug|Release [Options]"
@echo.
@goto :EOF
