echo off
echo Making DrumSynthPlug win distribution ...
echo ------------------------------------------------------------------
echo Updating version numbers ...
echo.
call python update_version.py
echo.
echo ------------------------------------------------------------------
echo Building ...
echo.
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
msbuild Arpeggio.sln ^
  /t:arpeggio-vst2 ^
  /p:configuration=release;platform=win32 ^
  /flp:logfile=build-win.log;errorsonly ^
  /fileLogger ^
  /nologo
pause