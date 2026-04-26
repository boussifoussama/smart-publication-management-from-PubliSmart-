@echo off
set QTDIR=C:\Qt\6.7.3\mingw_64
set PATH=%QTDIR%\bin;C:\Qt\Tools\mingw1120_64\bin;%PATH%
set MINGW_BINDIR=C:\Qt\Tools\mingw1120_64\bin

cd /d "%~dp0"

echo Nettoyage...
if exist build (
    cd build
    %MINGW_BINDIR%\mingw32-make.exe clean
    cd ..
) else (
    mkdir build
)

echo Génération du Makefile...
%QTDIR%\bin\qmake.exe SmartMarket.pro -o build\Makefile

echo Compilation...
cd build
%MINGW_BINDIR%\mingw32-make.exe

cd ..
echo Compilation terminée!
pause
