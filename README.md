# Fleen El-Guitar VST3

<div align="center">

**Professional Guitar Processing VST3 Plugin**

[![Release](https://img.shields.io/github/v/release/Tehuti01/fleen-el-guitar?style=for-the-badge)](https://github.com/Tehuti01/fleen-el-guitar/releases/latest)
[![License](https://img.shields.io/github/license/Tehuti01/fleen-el-guitar?style=for-the-badge)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-macOS-lightgrey?style=for-the-badge)](https://github.com/Tehuti01/fleen-el-guitar)

[Download](#-download) • [Documentation](docs/) • [Issues](https://github.com/Tehuti01/fleen-el-guitar/issues)

</div>

---

## 🎸 Overview

**Fleen El-Guitar** is a professional guitar processing VST3 plugin featuring:

- 🎛️ **10 Premium Factory Presets** - From clean to high gain
- 💎 **Skeuomorphic Interface** - Hardware-inspired premium design  
- 📐 **Golden Ratio Layout** - Mathematically balanced UI
- 🔥 **Professional DSP** - Gain, Drive, EQ, Reverb, Compression
- ✨ **Real-time Visualizers** - Waveform and spectrum displays
- 📦 **Preset Manager** - Save, load, and organize your sounds

---

## 📦 Download

### Quick Download

**Download the plugin zip file:**

[**📥 Download FleenElGuitar-macOS.zip**](https://github.com/Tehuti01/fleen-el-guitar/releases/latest)

---

## 📦 Installation

### Step 1: Download

Download the latest release from:
```
https://github.com/Tehuti01/fleen-el-guitar/releases/latest
```

You'll get a file called `FleenElGuitar-macOS.zip`

### Step 2: Extract

Double-click the `.zip` file to extract it. You'll see a folder called `FleenElGuitar.vst3`

### Step 3: Install

**Copy the `FleenElGuitar.vst3` folder to your VST3 plugins folder:**

#### macOS (Recommended Location)
```bash
cp -R ~/Downloads/FleenElGuitar.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

#### macOS (System-wide)
```bash
sudo cp -R ~/Downloads/FleenElGuitar.vst3 /Library/Audio/Plug-Ins/VST3/
```

#### Windows
```
C:\Program Files\Common Files\VST3\
```

#### Linux
```
~/.vst3/
```

### Step 4: Use in Your DAW

1. **Restart your DAW** (Logic Pro, Ableton Live, GarageBand, etc.)

2. **Rescan plugins** if your DAW requires it:
   - **Logic Pro**: Automatic on launch
   - **Ableton Live**: Preferences → Plug-Ins → Browse
   - **GarageBand**: Automatic on launch
   - **Cubase**: Devices → Plug-in Information → Refresh

3. **Find the plugin** in your plugin browser:
   - Search for "Fleen" or "Fleen El-Guitar"

4. **Load a preset** and start playing!

---

## ✅ Verify Installation

Open Terminal and run:

```bash
ls -la ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

You should see the plugin folder listed.

---

## 🎛️ Using the Plugin

### Factory Presets

| # | Preset Name | Best For |
|---|-------------|----------|
| 1 | Default | Starting point |
| 2 | Crystal Clean | Clean tones |
| 3 | Warm Jazz | Jazz guitar |
| 4 | Classic Crunch | Rock rhythm |
| 5 | Blues Breaker | Blues lead |
| 6 | High Gain Metal | Metal rhythm |
| 7 | Singing Lead | Lead guitar |
| 8 | Ambient Shimmer | Ambient textures |
| 9 | Funk Rhythm | Funk/Pop |
| 10 | Modern Prog | Progressive |

---

## 🎚️ Controls

| Control | Function |
|---------|----------|
| **GAIN** | Input level |
| **DRIVE** | Distortion amount |
| **BASS** | Low frequencies (200Hz) |
| **MID** | Mid frequencies (1kHz) |
| **TREBLE** | High frequencies (3kHz) |
| **PRESENCE** | Ultra-high frequencies (5kHz) |
| **REVERB** | Reverb mix |
| **COMPRESSION** | Dynamic compression |
| **BYPASS** | Enable/disable effect |

---

## 📁 File Locations

| Type | Location |
|------|----------|
| **VST3 Plugin** | `~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3` |
| **User Presets** | `~/Library/Application Support/Fleen/El-Guitar/Presets/` |
| **Logs** | `~/Library/Logs/Fleen/` |

---

## 🐛 Troubleshooting

### Plugin Not Showing in DAW

**Step 1:** Verify the file is in the correct location
```bash
ls -la ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

**Step 2:** Check file permissions
```bash
chmod -R 755 ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

**Step 3:** Restart your DAW completely

**Step 4:** Rescan plugins in your DAW settings

### Get Help

- **Documentation:** [docs/](docs/)
- **Issues:** [GitHub Issues](https://github.com/Tehuti01/fleen-el-guitar/issues)
- **Discussions:** [GitHub Discussions](https://github.com/Tehuti01/fleen-el-guitar/discussions)

---

## 🏗️ Building from Source

```bash
# Clone the repository
git clone https://github.com/Tehuti01/fleen-el-guitar.git
cd fleen-el-guitar

# Create build directory
mkdir build && cd build

# Configure
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# The VST3 will be in:
# build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3
```

### Requirements

- macOS 10.15 or later
- CMake 3.24+
- Ninja build system
- Xcode Command Line Tools

---

## 📄 License

GPL-3.0-or-later - See [LICENSE](LICENSE) for details.

---

## 🙏 Credits

Built with:
- [JUCE Framework](https://juce.com/)
- [CMake](https://cmake.org/)

---

<div align="center">

**Made with ❤️ for musicians**

[Download Latest](https://github.com/Tehuti01/fleen-el-guitar/releases/latest) • [Report Issue](https://github.com/Tehuti01/fleen-el-guitar/issues)

</div>
