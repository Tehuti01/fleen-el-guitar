# Fleen El-Guitar VST3

<div align="center">

**Professional Guitar Processing VST3 Plugin**

[![Release](https://img.shields.io/github/v/release/Tehuti01/fleen-el-guitar?style=for-the-badge)](https://github.com/Tehuti01/fleen-el-guitar/releases/latest)
[![License](https://img.shields.io/github/license/Tehuti01/fleen-el-guitar?style=for-the-badge)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-macOS-lightgrey?style=for-the-badge)](https://github.com/Tehuti01/fleen-el-guitar)

[Download](#-installation) • [Documentation](docs/) • [Issues](https://github.com/Tehuti01/fleen-el-guitar/issues)

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

## 📦 Installation

### Method 1: One-Click Install (Recommended) ⭐

```bash
# Download and run the installer
curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh -o install.sh
chmod +x install.sh
./install.sh
```

**Or use the one-liner:**

```bash
curl -fsSL https://raw.githubusercontent.com/Tehuti01/fleen-el-guitar/main/install.sh | bash
```

The installer will:
- ✅ Find or create your VST3 folder
- ✅ Download the latest release
- ✅ Install to the correct location
- ✅ Verify the installation
- ✅ Show manual instructions if needed

### Method 2: Manual Download

1. **Download** the latest release:
   ```
   https://github.com/Tehuti01/fleen-el-guitar/releases/latest
   ```

2. **Extract** the downloaded `.zip` file

3. **Copy** the `.vst3` file to your VST3 folder:
   ```bash
   cp -R FleenElGuitar.vst3 ~/Library/Audio/Plug-Ins/VST3/
   ```

### Method 3: Homebrew (macOS)

```bash
# Install Homebrew if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install the plugin
brew tap Tehuti01/audio
brew install fleen-el-guitar
```

---

## ✅ Verify Installation

After installation, verify the plugin is installed:

```bash
# Check if plugin exists
ls -la ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3

# You should see the plugin folder listed
```

---

## 🎛️ Using the Plugin

### In Your DAW

1. **Restart your DAW** (Logic Pro, Ableton Live, GarageBand, etc.)

2. **Rescan plugins** if your DAW requires it:
   - **Logic Pro**: Automatic on launch
   - **Ableton Live**: Preferences → Plug-Ins → Browse
   - **GarageBand**: Automatic on launch
   - **Cubase**: Devices → Plug-in Information → Refresh

3. **Find the plugin** in your plugin browser:
   - Search for "Fleen" or "Fleen El-Guitar"
   - It's in the "Instrument" or "Effect" category

4. **Load a preset** and start playing!

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

**Step 1:** Verify installation
```bash
ls -la ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

**Step 2:** Check file permissions
```bash
chmod -R 755 ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

**Step 3:** Rescan plugins in your DAW

**Step 4:** Restart your DAW completely

### Installation Failed

If the automatic installer fails:

1. **Check the error message** - it will tell you what went wrong

2. **Try manual installation:**
   ```bash
   # Download from releases
   curl -L https://github.com/Tehuti01/fleen-el-guitar/releases/latest/download/FleenElGuitar-macOS.zip -o fleen.zip
   
   # Extract
   unzip fleen.zip
   
   # Install manually
   cp -R FleenElGuitar.vst3 ~/Library/Audio/Plug-Ins/VST3/
   ```

3. **Check internet connection:**
   ```bash
   ping github.com
   ```

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
