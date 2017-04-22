echo off

:: where is python.exe?

REM - batch file to build VS2010 project and zip the resulting binaries (or make installer)
REM - updating version numbers requires python and python path added to %PATH% env variable 
REM - zipping requires 7zip in %ProgramFiles%\7-Zip\7z.exe
REM - building installer requires innotsetup in "%ProgramFiles(x86)%\Inno Setup 5\iscc"
REM - AAX codesigning requires ashelper tool added to %PATH% env variable and aax.key/.crt in .\..\..\..\Certificates\

echo Making DrumSynthPlug win distribution ...

echo ------------------------------------------------------------------
echo Updating version numbers ...

call python update_version.py

echo ------------------------------------------------------------------
echo Building ...

:: if exist "%ProgramFiles(x86)%" (goto 64-Bit) else (goto 32-Bit)
:: we want 32bit
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
::ll "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
::call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"

REM - set preprocessor macros like this, for instance to enable demo build:
REM - SET CMDLINE_DEFINES="DEMO_VERSION"

REM - Could build individual targets like this:
REM - msbuild DrumSynthPlug-app.vcxproj /p:configuration=release /p:platform=win32

:: I just want to target...
:: /T=DrumSynthPlug-app

::build DrumSynthPlug.sln /t:DrumSynthPlug-app /p:configuration=release;platform=win32 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly 
:: - removed `/v:quiet `
::/flp:logfile=build-win.log;errorsonly /noconsolelogger /fileLogger /nologo
msbuild DrumSynthPlug.sln /t:DrumSynthPlug-app:clean /p:configuration=release;platform=win32 /flp:logfile=build-win.log;errorsonly
::msbuild DrumSynthPlug.sln /p:configuration=release /p:platform=x64 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly;append

::       #echo ------------------------------------------------------------------
::       #echo Code sign aax binary...
::       #REM - x86
::       #REM - x64
::       
::       REM - Make Installer (InnoSetup)
::       
::       echo ------------------------------------------------------------------
::       echo Making Installer ...
::       
::       if exist "%ProgramFiles(x86)%" (goto 64-Bit-is) else (goto 32-Bit-is)
::       
::       :32-Bit-is
::       "%ProgramFiles%\Inno Setup 5\iscc" /cc ".\installer\DrumSynthPlug.iss"
::       goto END-is
::       
::       :64-Bit-is
::       "%ProgramFiles(x86)%\Inno Setup 5\iscc" /cc ".\installer\DrumSynthPlug.iss"
::       goto END-is
::       
::       :END-is
::       
::       REM - ZIP
::       REM - "%ProgramFiles%\7-Zip\7z.exe" a .\installer\DrumSynthPlug-win-32bit.zip .\build-win\app\win32\bin\DrumSynthPlug.exe .\build-win\vst3\win32\bin\DrumSynthPlug.vst3 .\build-win\vst2\win32\bin\DrumSynthPlug.dll .\build-win\rtas\bin\DrumSynthPlug.dpm .\build-win\rtas\bin\DrumSynthPlug.dpm.rsr .\build-win\aax\bin\DrumSynthPlug.aaxplugin* .\installer\license.rtf .\installer\readmewin.rtf
::       REM - "%ProgramFiles%\7-Zip\7z.exe" a .\installer\DrumSynthPlug-win-64bit.zip .\build-win\app\x64\bin\DrumSynthPlug.exe .\build-win\vst3\x64\bin\DrumSynthPlug.vst3 .\build-win\vst2\x64\bin\DrumSynthPlug.dll .\installer\license.rtf .\installer\readmewin.rtf
::       
::       echo ------------------------------------------------------------------
::       echo Printing log file to console...
::       
::       type build-win.log

pause