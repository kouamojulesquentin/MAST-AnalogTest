@echo ====================================== Testcase.bat ==============================================

@set targetDir=".\Build_Qt\bin\release"
@set AppFileName=TestCasesApp.exe

@echo ===== Target dir: %targetDir%

@pushd %targetDir%
@if not exist %AppFileName% goto No_exe
@REM @cls && %AppFileName% -sf "c:\models\1500_wrapper.sit"  --print_graph -p SVF_Simu -lc 10
@REM @cls && %AppFileName% -tc 1500  --print_graph -p loopback -lc 7
@REM @cls && %AppFileName% -tc 1500  --print_graph -lc 7 -p loopback
@REM @cls && %AppFileName% -tc SIT_File -tco "..\..\..\TestCasesApp\SIT_Testcases\1500_Wrapper_4xRegs_12xBits.sit" --print_graph_file Parsed.gml -lc 7
@REM @cls && %AppFileName% -tc SIT_File -tco "-lc 7 -f ..\..\..\TestCasesApp\SIT_Testcases\1500_Wrapper_4xRegs_12xBits.sit" --print_graph_file Parsed.gml -p OpenOCD -po "ocd.conf"
@REM @cls && %AppFileName% -tc SIT_File -tco "-lc 7 -f ..\..\..\TestCasesApp\SIT_Testcases\1500_Wrapper_4xRegs_12xBits.sit" -p SVF_Emu
@REM @cls && %AppFileName% -tc SIT_File -tco "-lc 7 -f ..\..\..\TestCasesApp\SIT_Testcases\1500_Wrapper_4xRegs_12xBits.sit" -p I2C_Emu
@cls && %AppFileName% -pg -tc SIT_File -tco "--name zybo -f ..\..\..\TestCasesApp\SIT_Testcases\Zybo-1xReg_11bits.sit" -p I2C_Emu
@REM @cls && %AppFileName% -tc SIT_File -tco "-lc 7" --print_graph_file Parsed.gml -p loopback

@REM @cls && %AppFileName% -tc 1500  --print_graph -p SVF_Simu --protocol_options "to_sut.svf, from_sut.dat" -lc 7
@popd
@goto :EOF

:No_exe
@echo.
@echo. ---- file %CD%\%AppFileName% does not exist! ----
@echo.
@popd
@goto :EOF

@echo.
