@echo off
echo ========================================
echo    Dragon Synth - Windows Build Script
echo ========================================
echo.

:: Check for CMake
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake not found! Please install CMake from https://cmake.org/download/
    echo Add CMake to your PATH during installation.
    pause
    exit /b 1
)

:: Check for Visual Studio
where cl >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo WARNING: Visual Studio compiler not found in PATH.
    echo Please run this script from "Developer Command Prompt for VS 2022"
    echo Or install Visual Studio with C++ workload.
    echo.
    echo Attempting to find Visual Studio...
    
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo ERROR: Could not find Visual Studio installation.
        echo Please install Visual Studio 2019 or 2022 with C++ workload.
        pause
        exit /b 1
    )
)

echo.
echo Step 1: Creating build directory...
if exist build rmdir /s /q build
mkdir build
cd build

echo.
echo Step 2: Configuring CMake (this will download JUCE - may take a few minutes)...
cmake -G "Visual Studio 17 2022" -A x64 ..
if %ERRORLEVEL% neq 0 (
    echo Trying Visual Studio 2019...
    cmake -G "Visual Studio 16 2019" -A x64 ..
)
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Step 3: Building Release version...
cmake --build . --config Release -j%NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo    BUILD SUCCESSFUL!
echo ========================================
echo.
echo VST3 Plugin: build\DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3
echo Standalone:  build\DragonSynth_artefacts\Release\Standalone\Dragon Synth.exe
echo.
echo To create installer, run: build_installer.bat
echo.
pause
