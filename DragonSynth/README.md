# 🐉 Dragon Synth VST Plugin

A powerful synthesizer VST plugin with a stunning **dark dragon glass metallic** UI theme and **marble synthesizer keyboard**. Create rich, evolving sounds using dual oscillators, filters, and envelopes.

![Dragon Synth](https://img.shields.io/badge/VST3-Plugin-blue) ![JUCE](https://img.shields.io/badge/JUCE-7.0.9-orange) ![License](https://img.shields.io/badge/License-MIT-green)

## ✨ Features

### 🎹 Sound Engine
- **Dual Oscillators** with 5 waveform types each:
  - Sine (pure, clean tones)
  - Saw (rich harmonics, great for leads)
  - Square (hollow, punchy character)
  - Triangle (soft, mellow sound)
  - Noise (for texture and effects)
- **OSC2 Detune & Octave** controls for thick, layered sounds
- **16-voice Polyphony** for complex chord progressions
- **Anti-aliased oscillators** using PolyBLEP for pristine sound quality

### 🎚️ Filter Section
- **State Variable Filter** with 3 modes:
  - Low Pass (smooth, warm character)
  - High Pass (bright, airy sounds)
  - Band Pass (focused, resonant tones)
- **Cutoff & Resonance** controls
- **Filter Envelope Amount** for dynamic timbral movement

### 📈 Dual ADSR Envelopes
- **Amplitude Envelope** (Attack, Decay, Sustain, Release)
- **Filter Envelope** (Attack, Decay, Sustain, Release)
- Sculpt your sound from plucky stabs to lush pads

### 🎨 Stunning UI Design
- **Dark Dragon Glass** theme with metallic accents
- **Glowing ice-blue** indicators and waveform display
- **Real-time waveform visualization**
- Dragon scale pattern background

### 🎹 Marble Synthesizer Keyboard
- Beautiful **white and black marble** textured keys
- **Realistic 3D appearance** with veining patterns
- **Glowing feedback** when keys are pressed
- **Mouse interaction** - click and drag to play

### ⌨️ PC Keyboard Support
Play the synth with your computer keyboard:

**Lower Octave (Z row):**
| Key | Note |
|-----|------|
| Z | C |
| S | C# |
| X | D |
| D | D# |
| C | E |
| V | F |
| G | F# |
| B | G |
| H | G# |
| N | A |
| J | A# |
| M | B |

**Upper Octave (Q row):**
| Key | Note |
|-----|------|
| Q | C |
| 2 | C# |
| W | D |
| 3 | D# |
| E | E |
| R | F |
| 5 | F# |
| T | G |
| 6 | G# |
| Y | A |
| 7 | A# |
| U | B |
| I | C (next octave) |

**Octave Control:**
- `[` - Shift down one octave
- `]` - Shift up one octave

### 🎛️ MIDI Support
- Full MIDI input support for external keyboards
- Pitch bend wheel support
- Velocity sensitive

## 🔧 Building the Plugin

### Prerequisites

- **CMake** 3.22 or higher
- **C++17** compatible compiler:
  - Windows: Visual Studio 2019/2022, or MinGW
  - macOS: Xcode 12+ or Clang
  - Linux: GCC 9+ or Clang 10+
- **Git** (for fetching JUCE)

### Linux Dependencies

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libasound2-dev \
    libjack-jackd2-dev \
    libcurl4-openssl-dev \
    libfreetype6-dev \
    libx11-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxext-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev \
    mesa-common-dev
```

### macOS Dependencies

```bash
# Install Xcode command line tools
xcode-select --install

# Install CMake via Homebrew (optional)
brew install cmake
```

### Windows Dependencies

- Install [Visual Studio](https://visualstudio.microsoft.com/) with C++ workload
- Install [CMake](https://cmake.org/download/)

### Build Instructions

```bash
# Clone or navigate to the project directory
cd DragonSynth

# Create build directory
mkdir build
cd build

# Configure (this will download JUCE automatically)
cmake ..

# Build
cmake --build . --config Release

# The plugin will be in:
# - VST3: build/DragonSynth_artefacts/Release/VST3/
# - AU (macOS only): build/DragonSynth_artefacts/Release/AU/
# - Standalone: build/DragonSynth_artefacts/Release/Standalone/
```

### Build Options

```bash
# Debug build
cmake --build . --config Debug

# Parallel build (faster)
cmake --build . --config Release -j8

# Specific format only
cmake --build . --target DragonSynth_VST3
```

## 📦 Installation

### VST3 Plugin

Copy the VST3 folder to your system's plugin directory:

**Windows:**
```
C:\Program Files\Common Files\VST3\
```

**macOS:**
```
/Library/Audio/Plug-Ins/VST3/
~/Library/Audio/Plug-Ins/VST3/
```

**Linux:**
```
~/.vst3/
/usr/lib/vst3/
```

### Audio Unit (macOS only)

Copy the AU component to:
```
/Library/Audio/Plug-Ins/Components/
~/Library/Audio/Plug-Ins/Components/
```

### Standalone Application

The standalone version can be run directly without a DAW.

## 🎵 Usage Tips

### Creating a Fat Lead Sound
1. Set OSC1 to **Saw**
2. Set OSC2 to **Saw** with +7 cents detune
3. Filter: Low Pass, Cutoff ~4000Hz, Resonance ~2
4. Amp Env: Attack 5ms, Decay 200ms, Sustain 0.7, Release 300ms

### Plucky Bass
1. Set OSC1 to **Square**
2. Set OSC2 to **Square**, Octave -1
3. Filter: Low Pass, Cutoff ~800Hz, Resonance ~3
4. Filter Env: Attack 0ms, Decay 150ms, Sustain 0, Release 100ms
5. Filter Env Amount: 0.8

### Ethereal Pad
1. Set OSC1 to **Sine**
2. Set OSC2 to **Triangle**, Octave +1, Detune +12
3. Filter: Low Pass, Cutoff ~2000Hz, Resonance ~1
4. Amp Env: Attack 500ms, Decay 1000ms, Sustain 0.8, Release 2000ms

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 🙏 Acknowledgments

- Built with [JUCE](https://juce.com/) Framework
- Inspired by classic analog synthesizers
- Dragon glass aesthetic inspired by fantasy themes

---

**Made with 🔥 and ❄️ by Dragon Audio**
