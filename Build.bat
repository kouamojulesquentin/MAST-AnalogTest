@title Build.bat

@if not defined chain  (set chain=Build_Qt)
@REM @if not defined chain  (set chain=Build_CB)

@set target=%1
@if not defined target (set target=debug)

@set Build_Qt=Build_Qt
@set UT_targetFile=".\Build_Qt\bin\%target%\Lib_UT.exe"
@set TCA_targetFile=".\Build_Qt\bin\%target%\TestCasesApp.exe"

@echo.
@echo ======= Chain: %chain%, Target File: %UT_targetFile%
@echo.
@REM goto :EOF

@REM @if exist %UT_targetFile%  del %UT_targetFile%
@REM @if exist %TCA_targetFile% del %TCA_targetFile%

@if not exist Lib_UT\Generated mkdir Lib_UT\Generated

@set make_exe="c:\mingw-w64\Gcc-4.9.3\mingw32\bin\make.exe"

@echo.
@echo ===================== Building Logger =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Logger -f Makefile    %target%

@echo.
@echo ===================== Building Lib =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Lib -f Makefile    %target%

@if errorlevel 0  (
@echo.
@echo ===================== Building Lib_UT Runner.cpp =====================
@echo.
%make_exe% -C Lib_UT -f Runner.mak

@echo.
@echo ===================== Building Lib_UT =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Lib_UT -f Makefile %target%

@echo.
@echo ===================== Building SIT_Reader =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\SIT_reader -f Makefile    %target%

@echo.
@echo ===================== Building SIT_Reader_UT =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\SIT_reader_UT -f Makefile %target%

@echo.
@echo ===================== Building TestCasesApp =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\TestCasesApp -f Makefile  %target%
)

:CheckExe
@echo.
@echo.
@if not exist %UT_targetFile%  echo =============== Error: Failed to generate Mast Lib unit test executable =====================
@if not exist %TCA_targetFile% echo =============== Error: Failed to generate TestCasesApp executable =====================
@echo.
