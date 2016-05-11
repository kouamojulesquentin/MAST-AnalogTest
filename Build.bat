@title Build.bat

@set target=%1
@if "%target%"=="" set target=debug
@set make_exe="c:\mingw-w64\Gcc-4.9.3\mingw32\bin\make.exe"
@set makefile_lib_windows=Lib_Windows.mak
@set makefile_lib_ut_windows=Lib_UT_Windows.mak

%make_exe% -j4 -C Lib    -f %makefile_lib_windows%     %target%
%make_exe% -j4 -C Lib_UT -f %makefile_lib_ut_windows%  %target%

@REM pushd Lib
@REM g++.exe -shared -Wl,--output-def=..\bin\Debug\libLib.def -Wl,--out-implib=..\bin\Debug\libLib.a -Wl,--dll  ..\obj\Debug\src\AccessInterface.o ..\obj\Debug\src\BinaryVector.o ..\obj\Debug\src\Chain.o ..\obj\Debug\src\DefaultBinaryPathSelector.o ..\obj\Debug\src\GmlPrinterVisitor.o ..\obj\Debug\src\Linker.o ..\obj\Debug\src\ParentNode.o ..\obj\Debug\src\PrettyPrinterVisitor.o ..\obj\Debug\src\Register.o ..\obj\Debug\src\SVFVector.o ..\obj\Debug\src\SystemModel.o ..\obj\Debug\src\SystemModelNode.o ..\obj\Debug\src\Tap.o ..\obj\Debug\src\Utility.o  -o ..\bin\Debug\Lib.dll  -luser32
@REM g++.exe -shared -Wl, --output-def=..\bin\Debug\libLib.def -Wl, --out-implib=..\bin\Debug\libLib.a -Wl, --dll  ..\obj\Debug\src\AccessInterface.o ..\obj\Debug\src\BinaryVector.o ..\obj\Debug\src\Chain.o ..\obj\Debug\src\DefaultBinaryPathSelector.o ..\obj\Debug\src\GmlPrinterVisitor.o ..\obj\Debug\src\Linker.o ..\obj\Debug\src\ParentNode.o ..\obj\Debug\src\PrettyPrinterVisitor.o ..\obj\Debug\src\Register.o ..\obj\Debug\src\SVFVector.o ..\obj\Debug\src\SystemModel.o ..\obj\Debug\src\SystemModelNode.o ..\obj\Debug\src\Tap.o ..\obj\Debug\src\Utility.o  -o ..\bin\Debug\Lib.dll  -luser32
@REM popd
