@echo ====================================== UT_Run_InLoop.bat ==============================================

@set Loop_BuiltType=%1
@set Loop_Options=%2
@echo Loop_BuiltType: %Loop_BuiltType%, Loop_Options: %Loop_Options%

@SET /A LOOP=0
@SET /A RST_LOOP=0


@set targetDir=".\Build_Qt\bin\%Loop_BuiltType%"
@set LogFile=%targetDir%\Log.txt
@set CumulativeLogFile=%targetDir%\Log_Cumulative.txt

@if exist "%CumulativeLogFile%" del "%CumulativeLogFile%"
@echo "This is a marker to know when user want to stop loop (by deleting the file)" > CanLoop.txt

:Restart
@REM @echo Loop_BuiltType: %Loop_BuiltType%, Loop_Options: %Loop_Options%
@SET /A LOOP+=1
@SET /A RST_LOOP+=1

@if "%RST_LOOP%" == "50" (
  del "%CumulativeLogFile%"
  Set /A RST_LOOP=0
)

@echo.
@echo ====================================================
@echo Loop %LOOP%:
@echo ==================================================== >> %CumulativeLogFile%
@echo Loop %LOOP%:  >> %CumulativeLogFile%
@echo.              >> %CumulativeLogFile%

@Call UT_Run %Loop_BuiltType% %Loop_Options%
@type %LogFile%     >> %CumulativeLogFile%
@popd

@if not exist CanLoop.txt goto :EOF
@goto Restart
