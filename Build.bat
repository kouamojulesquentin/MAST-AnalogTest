@title Build.bat

@if not defined chain  (set chain=Build_Qt)
@REM @if not defined chain  (set chain=Build_CB)

@set target=%1
@if not defined target (set target=debug)

@set Build_Qt=Build_Qt
@set UT_targetFile=".\Build_Qt\bin\%target%\Mast_UT.exe"
@set TCA_targetFile=".\Build_Qt\bin\%target%\TestCasesApp.exe"

@echo.
@echo ======= Chain: %chain%, Target File: %UT_targetFile%
@echo.
@REM goto :EOF

@REM @if exist %UT_targetFile%  del %UT_targetFile%
@REM @if exist %TCA_targetFile% del %TCA_targetFile%

@if not exist Mast_UT\Generated mkdir Mast_UT\Generated

@set make_exe="c:\mingw-w64\Gcc-4.9.3\mingw32\bin\make.exe"

@echo.
@echo ===================== Building Logger =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Logger -f Makefile    %target%

@echo.
@echo ===================== Building Mast_Core =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Mast_Core -f Makefile    %target%

@echo.
@echo ===================== Building SIT_Reader =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\SIT_reader -f Makefile    %target%

@echo.
@echo ===================== Building Mast =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Mast -f Makefile    %target%

@if errorlevel 0  (
@echo.
@echo ===================== Building Mast_UT Runner.cpp =====================
@echo.
%make_exe% -C Mast_UT -f Runner.mak

@echo.
@echo ===================== Building Mast_UT =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Mast_UT -f Makefile %target%

@echo.
@echo ===================== Building SIT_Reader_UT Runner.cpp =====================
@echo.
%make_exe% -C SIT_reader_UT -f Runner.mak


@echo.
@echo ===================== Building SIT_Reader_UT =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\SIT_reader_UT -f Makefile %target%

@echo.
@echo ===================== Building External_Libs =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\External_Libs -f Makefile %target%

@echo.
@echo ===================== Building Optional_Libs =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Optional_Libs -f Makefile %target%

@echo.
@echo ===================== Building TestCasesApp =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\TestCasesApp -f Makefile  %target%

@echo.
@echo ===================== Building MastExample_CPP =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\MastExample_CPP -f Makefile  %target%

)

:CheckExe
@echo.
@echo.
@if not exist %UT_targetFile%  echo =============== Error: Failed to generate Mast_UT executable =====================
@if not exist %TCA_targetFile% echo =============== Error: Failed to generate TestCasesApp executable =====================
@echo.
