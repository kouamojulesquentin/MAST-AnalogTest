@echo ====================================== Sit_Demo.bat ==============================================

@set targetDir=".\cmake_debug\Bin"
@set AppFileName=SIT_reader_demo.exe
@REM @set SitFile="..\..\MastExample_CPP\Example_1.sit"
@set SitFile="..\..\TestCasesApp\SIT_Testcases\1500_Wrapper_SPI.sit"

@echo ===== Target dir: %targetDir%

@pushd %targetDir%
@if not exist %AppFileName% goto No_exe

@cls && %AppFileName% %SitFile%

@popd
@goto :EOF

:No_exe
@echo.
@echo. ---- file %CD%\%AppFileName% does not exist! ----
@echo.
@popd
@goto :EOF

@echo.
