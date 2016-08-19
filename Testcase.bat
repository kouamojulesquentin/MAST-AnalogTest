@echo ====================================== Testcase.bat ==============================================

@set targetDir=".\Build_Qt\bin\release"
@set AppFileName=TestCasesApp.exe

@echo ===== Target dir: %targetDir%

@pushd %targetDir%
@if not exist %AppFileName% goto No_exe
@REM @cls && %AppFileName% -sf "c:\models\1500_wrapper.sit"  --print_graph -p SVF_Simu -lc 10
@cls && %AppFileName% -tc 1500  --print_graph -p SVF_Simu -lc 7
@popd
@goto :EOF

:No_exe
@echo.
@echo. ---- file %CD%\%AppFileName% does not exist! ----
@echo.
@popd
@goto :EOF

@echo.
