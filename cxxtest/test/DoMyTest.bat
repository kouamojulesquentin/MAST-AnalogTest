@echo ====================================== DoMyTest.bat ==============================================
@echo off

cls

@set testId=%1
@set compiler=%2

@if /i "%1%" equ "" goto Syntax_Usage

@if /i "%compiler%" equ "clang" set compiler=Clang
@if /i "%compiler%" equ "gcc"   set compiler=Gpp
@if /i "%compiler%" equ "gpp"   set compiler=Gpp
@if /i "%compiler%" equ "cpp"   set compiler=Cpp

@if not defined compiler set compiler=Clang

@if /i "%testId%" equ "1"     goto test_1
@if /i "%testId%" equ "2"     goto test_2
@if /i "%testId%" equ "3"     goto test_3
@if /i "%testId%" equ "4"     goto test_4
@if /i "%testId%" equ "5"     goto test_5
@if /i "%testId%" equ "6"     goto test_6
@if /i "%testId%" equ "7"     goto test_7
@if /i "%testId%" equ "8"     goto test_8
@if /i "%testId%" equ "clang" goto test_all_clang
@if /i "%testId%" equ "gcc"   goto test_all_gcc
@if /i "%testId%" equ "gpp"   goto test_all_gcc
@if /i "%testId%" equ "cpp"   goto test_all_cpp

:test_specified
@call :Do_test "%testId%"
goto :EOF

:test_1
@call :Do_test test_asserts_failures
@if /i "%testId%"  neq "all" goto :EOF

:test_2
@call :Do_test test_data_driven_failures
@if /i "%testId%"  neq "all" goto :EOF

:test_3
@call :Do_test test_data_driven_successes_details
@if /i "%testId%"  neq "all" goto :EOF

:test_4
@call :Do_test test_asserts_successes_details
@if /i "%testId%"  neq "all" goto :EOF

:test_5
@call :Do_test test_pointer_asserts_failures
@if /i "%testId%"  neq "all" goto :EOF

:test_6
@call :Do_test test_pointer_asserts_successes
@if /i "%testId%"  neq "all" goto :EOF

:test_7
@call :Do_test test_pointer_asserts_successes_details
@if /i "%testId%"  neq "all" goto :EOF

:test_8
@call :Do_test test_asserts_successes
@if /i "%testId%"  neq "all" goto :EOF

goto :EOF
:test_all_clang
python test_cxxtest.py TestClangFOG
goto :EOF
:test_all_gcc
python test_cxxtest.py TestGppFOG
goto :EOF
:test_all_cpp
python test_cxxtest.py TestCppFOG
goto :EOF

goto :EOF
:Do_test
@echo ====================================================================================================================================================================
@echo   Running test: "%~1"
@echo ====================================================================================================================================================================
@echo.
python test_cxxtest.py Test%compiler%FOG.%~1
@echo.
@echo.
goto :EOF

:Syntax_Usage
@echo.
@echo Syntax usage:
@echo ============
@echo.
@echo "DoMyTest Clang|Gcc|Cpp"
@echo Or
@echo "DoMyTest test_id Clang|Gcc|Cpp"
 goto :EOF
