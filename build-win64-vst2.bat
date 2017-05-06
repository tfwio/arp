@echo off
SET sln_name=arpeggio
SET vcp_name=arpeggio-vst2
SET tgt=x64
echo Making %sln_name% win distribution ...
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
:: /T=...-app

::build %sln_name%.sln /t:%vcp_name%-app /p:configuration=release;platform=win32 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly 
:: - removed `/v:quiet `
::/flp:logfile=build-win.log;errorsonly /noconsolelogger /fileLogger /nologo
msbuild %sln_name%.sln /t:%vcp_name% /p:configuration=release;platform=%tgt% ^
/flp:logfile=build-win.log;errorsonly ^
/fileLogger ^
/nologo
