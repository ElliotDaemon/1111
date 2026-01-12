@echo off
setlocal enabledelayedexpansion
title Dragon Synth - Build and Install
color 0B

echo.
echo   ===============================================
echo   =                                             =
echo   =        DRAGON SYNTH - ONE-CLICK SETUP      =
echo   =                                             =
echo   ===============================================
echo.

:: Check for admin rights
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [!] Requesting administrator privileges...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

echo [*] Running as Administrator
echo.

:: Find Visual Studio
set "VS_PATH="
for %%v in (2022 2019) do (
    for %%e in (Community Professional Enterprise) do (
        if exist "C:\Program Files\Microsoft Visual Studio\%%v\%%e\VC\Auxiliary\Build\vcvars64.bat" (
            set "VS_PATH=C:\Program Files\Microsoft Visual Studio\%%v\%%e\VC\Auxiliary\Build\vcvars64.bat"
            echo [+] Found Visual Studio %%v %%e
            goto :found_vs
        )
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\%%v\%%e\VC\Auxiliary\Build\vcvars64.bat" (
            set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\%%v\%%e\VC\Auxiliary\Build\vcvars64.bat"
            echo [+] Found Visual Studio %%v %%e
            goto :found_vs
        )
    )
)

:not_found_vs
echo.
echo [X] ERROR: Visual Studio not found!
echo.
echo     Please install Visual Studio (free Community edition):
echo     https://visualstudio.microsoft.com/downloads/
echo.
echo     During installation, select:
echo     "Desktop development with C++"
echo.
pause
exit /b 1

:found_vs
echo [*] Setting up build environment...
call "%VS_PATH%" >nul 2>&1

:: Check CMake
where cmake >nul 2>&1
if %errorLevel% neq 0 (
    echo.
    echo [X] ERROR: CMake not found!
    echo.
    echo     Please install CMake:
    echo     https://cmake.org/download/
    echo.
    echo     CHECK: "Add CMake to PATH" during install!
    echo.
    pause
    exit /b 1
)
echo [+] CMake found

:: Get script directory
cd /d "%~dp0"
echo [+] Working directory: %CD%

:: Clean and create build folder
echo.
echo [*] Preparing build folder...
if exist build rmdir /s /q build 2>nul
mkdir build
cd build

:: Configure
echo.
echo [*] Configuring project (downloading JUCE - please wait)...
echo     This may take 2-5 minutes on first run...
echo.
cmake -G "Visual Studio 17 2022" -A x64 .. 2>nul
if %errorLevel% neq 0 (
    cmake -G "Visual Studio 16 2019" -A x64 ..
)
if %errorLevel% neq 0 (
    echo [X] CMake configuration failed!
    pause
    exit /b 1
)
echo [+] Configuration complete

:: Build
echo.
echo [*] Building Dragon Synth (this takes 2-5 minutes)...
echo.
cmake --build . --config Release -j %NUMBER_OF_PROCESSORS%
if %errorLevel% neq 0 (
    echo.
    echo [X] Build failed!
    pause
    exit /b 1
)

echo.
echo [+] Build successful!

:: Install VST3
echo.
echo [*] Installing VST3 plugin...
set "VST3_DEST=C:\Program Files\Common Files\VST3\Dragon Synth.vst3"

if exist "%VST3_DEST%" (
    echo [*] Removing old version...
    rmdir /s /q "%VST3_DEST%" 2>nul
)

xcopy /E /I /Y "DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3" "%VST3_DEST%" >nul
if %errorLevel% neq 0 (
    echo [X] Failed to install VST3!
    pause
    exit /b 1
)
echo [+] VST3 installed to: %VST3_DEST%

:: Copy standalone to user folder
echo.
echo [*] Installing Standalone app...
set "APP_DEST=%USERPROFILE%\Dragon Synth"
if not exist "%APP_DEST%" mkdir "%APP_DEST%"
copy /Y "DragonSynth_artefacts\Release\Standalone\Dragon Synth.exe" "%APP_DEST%\" >nul
echo [+] Standalone installed to: %APP_DEST%\Dragon Synth.exe

:: Done!
echo.
echo   ===============================================
echo   =                                             =
echo   =          INSTALLATION COMPLETE!            =
echo   =                                             =
echo   ===============================================
echo.
echo   VST3 Plugin Location:
echo   C:\Program Files\Common Files\VST3\Dragon Synth.vst3
echo.
echo   Standalone App Location:
echo   %APP_DEST%\Dragon Synth.exe
echo.
echo   -----------------------------------------------
echo   NEXT STEPS:
echo   -----------------------------------------------
echo   1. Open your DAW (FL Studio, Ableton, etc.)
echo   2. Rescan/Refresh VST plugins
echo   3. Find "Dragon Synth" in your plugin list
echo   4. Add it to a track and enjoy!
echo.
echo   KEYBOARD CONTROLS:
echo   Z-M keys = Play notes (lower octave)
echo   Q-U keys = Play notes (upper octave)
echo   [ and ] = Change octave
echo   -----------------------------------------------
echo.

:: Ask to launch standalone
set /p "LAUNCH=Launch Dragon Synth now? (Y/N): "
if /i "%LAUNCH%"=="Y" (
    start "" "%APP_DEST%\Dragon Synth.exe"
)

echo.
echo Done! Press any key to exit...
pause >nul
