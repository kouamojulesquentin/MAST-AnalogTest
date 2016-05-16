@echo ====================================== MakeMakefiles.bat ==============================================

@set tool_path="..\cbp2make\bin\Release\cbp2make.exe"

@set makefile_lib_windows=Lib_Windows.mak
@set makefile_lib_linux=Lib_Linux.mak
@set makefile_lib_ut_windows=Lib_UT_Windows.mak
@set makefile_lib_ut_linux=Lib_UT_Linux.mak

@if exist .\Lib\%makefile_lib_windows%        del .\Lib\%makefile_lib_windows%
@if exist .\Lib_UT\%makefile_lib_ut_windows%  del .\Lib_UT\%makefile_lib_ut_windows%
@REM @if exist .\Lib\%makefile_lib_linux%          del .\Lib\%makefile_lib_linux%
@REM @if exist .\Lib_UT\%makefile_lib_ut_linux%    del .\Lib_UT\%makefile_lib_ut_linux%

:Workspace
@REM %tool_path% --verbose -in NewMast.workspace -makefile "debug,release,all" -out workspace.mak      -windows -unix
@REM %tool_path% --verbose -in NewMast.workspace -out ws_Windows.mak      -windows
@REM %tool_path% --verbose -in NewMast.workspace -out ws_Linux.mak        -unix

:Lib
:Additional options for linker: -Wl,--output-def=..\bin\Debug\libLib.def -Wl,--out-implib=..\bin\Debug\libLib.a -Wl,--dll
%tool_path% --verbose -in Lib/Lib.cbp       -makefile "debug,release,all" -out Lib/%makefile_lib_windows% -windows
@REM %tool_path% --verbose -in Lib/Lib.cbp       -makefile "debug,release,all" -out Lib/%makefile_lib_linux%   -unix
:Lib_UT
%tool_path% --verbose -in Lib_UT/Lib_UT.cbp -makefile "debug,release,all" -out Lib_UT/%makefile_lib_ut_windows% -windows
@REM %tool_path% --verbose -in Lib_UT/Lib_UT.cbp -makefile "debug,release,all" -out Lib_UT/%makefile_lib_ut_linux%   -unix


@REM @rename Lib\Lib.cbp.mak.windows       %makefile_lib_windows%
@REM @rename Lib\Lib.cbp.mak.unix          %makefile_lib_linux%
@REM @rename Lib_UT\Lib_UT.cbp.mak.windows %makefile_lib_ut_windows%
@REM @rename Lib_UT\Lib_UT.cbp.mak.unix    %makefile_lib_ut_linux%
