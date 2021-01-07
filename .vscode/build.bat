@echo on
title qmake and nmake build prompt

set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
set QTDIR=C:\Qt\5.15.0\msvc2019_64
set WINKITS=C:\Program Files (x86)\Windows Kits\8.1\bin\x86
set PATH=%VCINSTALLDIR%\bin;%QTDIR%\bin;%WINKITS%;%PATH%
call "%VCINSTALLDIR%\vcvarsall.bat" amd64
mkdir build
cd build
qmake ../QRCodeGenerater.pro -spec win32-msvc "CONFIG+=debug"
nmake
