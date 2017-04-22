@echo off
echo Making DrumSynthPlug win distribution ...
echo ------------------------------------------------------------------
echo Updating version numbers ...
call python update_version.py
echo ------------------------------------------------------------------
echo Building ...
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
REM - set preprocessor macros like this, for instance to enable demo build:
REM - SET CMDLINE_DEFINES="DEMO_VERSION"

REM - Could build individual targets like this:
REM - msbuild DrumSynthPlug-app.vcxproj /p:configuration=release /p:platform=win32

:: I just want to target...
:: /T=DrumSynthPlug-app

::build DrumSynthPlug.sln /t:DrumSynthPlug-app /p:configuration=release;platform=win32 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly 
:: - removed `/v:quiet `
::/flp:logfile=build-win.log;errorsonly /noconsolelogger /fileLogger /nologo
msbuild DrumSynthPlug.sln /t:DrumSynthPlug-vst2 /p:configuration=release;platform=x64 ^
/flp:logfile=build-win.log;errorsonly ^
/fileLogger ^
/nologo
