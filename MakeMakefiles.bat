@echo ====================================== MakeMakefiles.bat ==============================================

@set tool_path="..\cbp2make\bin\Release\cbp2make.exe"

:Workspace
%tool_path% --verbose -in NewMast.workspace -out workspace.mak      -windows -unix
@REM %tool_path% --verbose -in NewMast.workspace -out ws_Windows.mak      -windows
@REM %tool_path% --verbose -in NewMast.workspace -out ws_Linux.mak        -unix

:Lib
@REM %tool_path% --verbose -in Lib/Lib.cbp       -out Lib/lib_Windows.mak -windows
@REM %tool_path% --verbose -in Lib/Lib.cbp       -out Lib/lib_Linux.mak   -unix

:Lib_UT
@REM %tool_path% --verbose -in Lib_UT/Lib_UT.cbp       -out Lib_UT/lib_UT_Windows.mak -windows
@REM %tool_path% --verbose -in Lib_UT/Lib_UT.cbp       -out Lib_UT/lib_UT_Linux.mak   -unix
