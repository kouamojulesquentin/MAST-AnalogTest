
@set Build=%1
@set Options=%2

@if "%Build%"=="" goto Syntax

@set originalPath=%path%
@REM @set Path=C:\MinGW\bin;%Path%
@set DestFile=Mast_UT\UT_Results\UT_Result.txt

@if not defined chain  (set chain=Build_Qt)
@REM @if not defined chain  (set chain=Build_CB)

@set Build_Qt=Build_Qt
@if /i %chain% equ %Build_Qt% (
set targetDir=".\Build_Qt\bin\%Build%"
@set DestFileFromExe=..\..\..\%DestFile%
) else (
set  targetDir=".\bin\%Build%"
@set DestFileFromExe=..\..\%DestFile%
)

@echo ===== Target dir: %targetDir%
@pushd %targetDir%

@if not exist Mast_UT.exe goto No_exe
@REM @echo Dir: %cd%
@REM @dir


@if     "%Options%"=="--display_success"  Mast_UT.exe %Options% >      %DestFileFromExe%
@if not "%Options%"=="--display_success"  Mast_UT.exe %Options% | mtee %DestFileFromExe%


@set Build=
@set Options=
@REM @set Path=%originalPath%
@popd
@echo.
@echo Results have been saved into: "%DestFile%"

@goto :EOF
:No_exe
@echo.
@echo. ---- file %CD%\Mast_UT.exe does not exist! ----
@echo.
@goto :EOF
:Syntax
@echo.
@echo. Syntax is: "UT_Run Debug|Release"
@echo.
@goto :EOF
