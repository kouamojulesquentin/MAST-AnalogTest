@title Build.bat

@if not defined chain  (set chain=Build_Qt)
@REM @if not defined chain  (set chain=Build_CB)

@set target=%1
@if not defined target (set target=debug)

@set Build_Qt=Build_Qt
@if /i %chain% equ %Build_Qt% (set targetFile=".\Build_Qt\bin\%target%\Lib_UT.exe") else (set targetFile=".\bin\%target%\Lib_UT.exe")

@echo ======= Chain: %chain%, Target File: %targetFile%
@echo.
@REM goto :EOF

@if exist %targetFile% del %targetFile%
@if not exist Lib_UT\Generated mkdir Lib_UT\Generated

@set make_exe="c:\mingw-w64\Gcc-4.9.3\mingw32\bin\make.exe"

@if /i %chain% equ %Build_Qt% (

@echo ===================== Building Lib =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Lib -f Makefile    %target%

@echo.
@echo ===================== Building Lib_UT Runner.cpp =====================
@echo.
%make_exe% -C Lib_UT -f Runner.mak

@echo.
@echo ===================== Building Lib_UT =====================
@echo.
%make_exe% -j4 -C %Build_Qt%\Lib_UT -f Makefile %target%

@goto CheckExe
)
else (

@echo ===================== Building Lib =====================
%make_exe% -j4 -C Lib -f Lib_Windows.mak    %target%

@echo.
@echo ===================== Building Lib_UT Runner.cpp =====================
@echo.
%make_exe% -C Lib_UT -f Runner.mak

@echo.
@echo ===================== Building Lib_UT =====================
@echo.
%make_exe% -j4 -C Lib_UT -f Lib_UT_Windows.mak %target%
)


@REM pushd Lib
@REM g++.exe -shared -Wl,--output-def=..\bin\Debug\libLib.def -Wl,--out-implib=..\bin\Debug\libLib.a -Wl,--dll  ..\obj\Debug\src\AccessInterface.o ..\obj\Debug\src\BinaryVector.o ..\obj\Debug\src\Chain.o ..\obj\Debug\src\DefaultBinaryPathSelector.o ..\obj\Debug\src\GmlPrinterVisitor.o ..\obj\Debug\src\Linker.o ..\obj\Debug\src\ParentNode.o ..\obj\Debug\src\PrettyPrinterVisitor.o ..\obj\Debug\src\Register.o ..\obj\Debug\src\SVFVector.o ..\obj\Debug\src\SystemModel.o ..\obj\Debug\src\SystemModelNode.o ..\obj\Debug\src\Tap.o ..\obj\Debug\src\Utility.o  -o ..\bin\Debug\Lib.dll  -luser32
@REM g++.exe -shared -Wl, --output-def=..\bin\Debug\libLib.def -Wl, --out-implib=..\bin\Debug\libLib.a -Wl, --dll  ..\obj\Debug\src\AccessInterface.o ..\obj\Debug\src\BinaryVector.o ..\obj\Debug\src\Chain.o ..\obj\Debug\src\DefaultBinaryPathSelector.o ..\obj\Debug\src\GmlPrinterVisitor.o ..\obj\Debug\src\Linker.o ..\obj\Debug\src\ParentNode.o ..\obj\Debug\src\PrettyPrinterVisitor.o ..\obj\Debug\src\Register.o ..\obj\Debug\src\SVFVector.o ..\obj\Debug\src\SystemModel.o ..\obj\Debug\src\SystemModelNode.o ..\obj\Debug\src\Tap.o ..\obj\Debug\src\Utility.o  -o ..\bin\Debug\Lib.dll  -luser32
@REM popd

:CheckExe
@echo.
@echo.
@if not exist %targetFile% echo =============== Error: Failed to generate unit test executable =====================
@echo.
