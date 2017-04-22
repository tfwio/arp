@echo off
echo Making DrumSynthPlug win distribution ...
echo ------------------------------------------------------------------
echo Updating version numbers ...
call python update_version.py
echo ------------------------------------------------------------------
echo Building ...
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
msbuild DrumSynthPlug.sln /t:DrumSynthPlug-app ^
/p:configuration=release;platform=x64 ^
/flp:logfile=build-win.log;errorsonly ^
/fileLogger ^
/nologo
pause