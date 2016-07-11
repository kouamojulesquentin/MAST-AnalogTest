@SET /A LOOP=0
@SET /A RST_LOOP=0

@set targetDir=".\Build_Qt\bin\Release"
@set LogFile=%targetDir%\Log.txt
@set CumulativeLogFile=%targetDir%\Log_Cumulative.txt

@if exist "%CumulativeLogFile%" del "%CumulativeLogFile%"

:Restart
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

@Call UT_Run Release
@type %LogFile%     >> %CumulativeLogFile%
@goto Restart
