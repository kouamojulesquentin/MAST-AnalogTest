@echo ====================================== MakeQtMakefiles.bat ==============================================


@echo Making Makefiles for Debug

@REM "C:\Qt\5.6\mingw49_32\bin\qmake.exe" -help
@REM goto :EOF
@echo off

:Make sure destination directory exist before call QMake !
if not exist Build_Qt mkdir Build_Qt

:QMake environment variable should point to something like: "C:\Qt\5.6\mingw49_32\bin\qmake.exe"
%Qmake% -Wall  NewMast.pro -r -o Build_Qt -spec win32-g++



