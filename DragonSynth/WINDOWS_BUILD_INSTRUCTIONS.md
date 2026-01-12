# 🐉 Dragon Synth - Windows Build & Installer Guide

## Quick Start

### Prerequisites

1. **Visual Studio 2019 or 2022** (free Community edition works)
   - Download: https://visualstudio.microsoft.com/downloads/
   - During install, select **"Desktop development with C++"** workload

2. **CMake** (3.22 or newer)
   - Download: https://cmake.org/download/
   - ✅ Check "Add CMake to PATH" during installation

3. **Inno Setup 6** (for creating the installer)
   - Download: https://jrsoftware.org/isdl.php

---

## 🔨 Building the Plugin

### Option A: Using the Build Script (Easiest)

1. **Open "Developer Command Prompt for VS 2022"**
   - Press Windows key, search for "Developer Command Prompt"
   - Or find it in Start Menu → Visual Studio 2022 → Developer Command Prompt

2. **Navigate to the DragonSynth folder:**
   ```cmd
   cd C:\path\to\DragonSynth
   ```

3. **Run the build script:**
   ```cmd
   build_windows.bat
   ```

4. **Wait for the build** (first build downloads JUCE, takes 5-10 minutes)

### Option B: Manual Build

```cmd
cd DragonSynth
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

---

## 📦 Creating the Installer

After building successfully:

1. **Run the installer builder:**
   ```cmd
   build_installer.bat
   ```

2. **Find your installer at:**
   ```
   build\Installer\DragonSynth_v1.0.0_Setup.exe
   ```

---

## 🎹 Manual Installation (Without Installer)

If you just want to use the plugin without creating an installer:

1. **Copy the VST3 folder:**
   ```
   From: build\DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3
   To:   C:\Program Files\Common Files\VST3\Dragon Synth.vst3
   ```

2. **Rescan plugins in your DAW**

---

## 📁 Build Output Locations

After a successful build:

| File | Location |
|------|----------|
| VST3 Plugin | `build\DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3\` |
| Standalone EXE | `build\DragonSynth_artefacts\Release\Standalone\Dragon Synth.exe` |
| Installer | `build\Installer\DragonSynth_v1.0.0_Setup.exe` |

---

## 🎵 Using in Your DAW

### FL Studio
1. Go to **Options → Manage Plugins**
2. Click **"Start scan"** or **"Find more plugins"**
3. Find **Dragon Synth** in the plugin list
4. Add to a channel

### Ableton Live
1. Go to **Preferences → Plug-ins**
2. Enable **"Use VST3 plug-in system folder"**
3. Click **"Rescan"**
4. Find Dragon Synth in your plugins

### Other DAWs
- Most DAWs auto-detect plugins in `C:\Program Files\Common Files\VST3\`
- Look for a "Rescan" or "Refresh" plugins option

---

## ⌨️ Keyboard Shortcuts (In Plugin)

| Keys | Notes |
|------|-------|
| Z, X, C, V, B, N, M | C, D, E, F, G, A, B (lower octave) |
| S, D, G, H, J | C#, D#, F#, G#, A# (lower octave) |
| Q, W, E, R, T, Y, U, I | C, D, E, F, G, A, B, C (upper octave) |
| 2, 3, 5, 6, 7 | C#, D#, F#, G#, A# (upper octave) |
| [ | Octave down |
| ] | Octave up |

---

## ❓ Troubleshooting

### "CMake not found"
- Reinstall CMake and check "Add to PATH"
- Restart your command prompt after installing

### "Visual Studio compiler not found"
- Use "Developer Command Prompt for VS" instead of regular CMD
- Or reinstall VS with C++ workload

### "Inno Setup not found"
- Download from https://jrsoftware.org/isdl.php
- Install to default location

### Build takes too long
- First build downloads JUCE (~100MB), subsequent builds are faster
- Use `-j` flag for parallel builds: `cmake --build . --config Release -j8`

---

## 📜 License

MIT License - Feel free to modify and distribute!

---

**Made with 🔥 and ❄️ by Dragon Audio**
