@echo ====================================== MakeMakefiles.bat ==============================================

@set tool_path="..\cbp2make\bin\Release\cbp2make.exe"

@REM %tool_path% -in NewMast.workspace -out ws_makefile.mak      -unix -windows
@REM %tool_path% -in Lib/Lib.cbp       -out Lib/lib.mak -unix -windows
%tool_path% -in Lib/Lib.cbp       -out Lib/lib_Windows.mak -windows
%tool_path% -in Lib/Lib.cbp       -out Lib/lib_Linux.mak -unix

