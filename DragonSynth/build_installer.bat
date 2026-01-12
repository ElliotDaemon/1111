@echo off
echo ========================================
echo    Dragon Synth - Installer Builder
echo ========================================
echo.

:: Check if build exists
if not exist "build\DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3" (
    echo ERROR: VST3 plugin not found!
    echo Please run build_windows.bat first.
    pause
    exit /b 1
)

:: Check for Inno Setup
set INNO_PATH=
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    set "INNO_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
) else if exist "C:\Program Files\Inno Setup 6\ISCC.exe" (
    set "INNO_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
) else (
    echo ERROR: Inno Setup 6 not found!
    echo.
    echo Please download and install Inno Setup from:
    echo https://jrsoftware.org/isdl.php
    echo.
    echo After installing, run this script again.
    pause
    exit /b 1
)

:: Create installer output directory
if not exist "build\Installer" mkdir "build\Installer"

:: Create a simple icon if not exists
if not exist "Resources\icon.ico" (
    echo Creating placeholder icon...
    mkdir Resources 2>nul
    :: Create a minimal .ico file (this creates a valid but simple icon)
    powershell -Command "$bytes = [byte[]]@(0,0,1,0,1,0,16,16,0,0,1,0,32,0,104,4,0,0,22,0,0,0); $bytes += [byte[]]::new(1128); [System.IO.File]::WriteAllBytes('Resources\icon.ico', $bytes)"
    if %ERRORLEVEL% neq 0 (
        echo Warning: Could not create icon, using default.
    )
)

echo.
echo Building installer with Inno Setup...
echo.

"%INNO_PATH%" "Installer\DragonSynth.iss"

if %ERRORLEVEL% neq 0 (
    echo ERROR: Installer build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo    INSTALLER CREATED SUCCESSFULLY!
echo ========================================
echo.
echo Installer location: build\Installer\DragonSynth_v1.0.0_Setup.exe
echo.
echo You can now distribute this installer!
echo.
pause
