# Fleen El-Guitar VST3

<div align="center">

![Fleen El-Guitar Banner](docs/images/banner.png)

**Professional Guitar Processing VST3 Plugin with Premium Skeuomorphic Interface**

[![Release](https://img.shields.io/github/v/release/tehuti01/Fleen-el-guitar-vst?style=for-the-badge)](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest)
[![License](https://img.shields.io/github/license/tehuti01/Fleen-el-guitar-vst?style=for-the-badge)](LICENSE)
[![Build](https://img.shields.io/github/actions/workflow/status/tehuti01/Fleen-el-guitar-vst/build.yml?style=for-the-badge)](https://github.com/tehuti01/Fleen-el-guitar-vst/actions)
[![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Windows%20%7C%20Linux-lightgrey?style=for-the-badge)](https://github.com/tehuti01/Fleen-el-guitar-vst)

[Download](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest) • 
[Documentation](docs/README.md) • 
[Discord](https://discord.gg/example) • 
[Issues](https://github.com/tehuti01/Fleen-el-guitar-vst/issues)

</div>

---

## 🎸 Overview

**Fleen El-Guitar** is a professional-grade guitar processing VST3 plugin that combines cutting-edge DSP algorithms with a premium skeuomorphic interface. Built with JUCE 8 and modern C++20, it delivers studio-quality tone shaping with an intuitive, hardware-inspired design.

### Key Features

- 🎛️ **Premium Skeuomorphic Interface** - Hardware-inspired controls with photorealistic rendering
- 📐 **Golden Ratio Layout** - Mathematically balanced UI design using Φ = 1.618
- 🔥 **Professional DSP Chain** - Gain, Drive, 3-Band EQ, Presence, Reverb, and Compression
- ✨ **OpenGL Visualizations** - Real-time waveform and spectrum displays at 60 FPS
- 🎨 **Custom LookAndFeel** - Neumorphic shadows with Melatonin Blur integration
- 📦 **Preset Management** - JSON-based preset system with factory and user presets
- ⚡ **Real-Time Safe** - Lock-free, allocation-free audio processing
- 🌐 **Cross-Platform** - macOS, Windows, and Linux support

---

## 🚀 Quick Start

### Installation

#### Option 1: Homebrew (macOS)

```bash
# Add the Homebrew tap
brew tap tehuti01/audio

# Install the plugin
brew install fleen-el-guitar
```

#### Option 2: Install Script (macOS)

```bash
curl -fsSL https://raw.githubusercontent.com/tehuti01/Fleen-el-guitar-vst/main/scripts/install.sh | bash
```

#### Option 3: Manual Installation

1. Download the latest release from [Releases](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest)
2. Extract the archive
3. Copy `FleenElGuitar.vst3` to your VST3 plugins directory:
   - **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
   - **Windows**: `C:\Program Files\Common Files\VST3\`
   - **Linux**: `~/.vst3/` or `/usr/lib/vst3/`

### Building from Source

#### Prerequisites

- **CMake** 3.24 or later
- **C++20** compatible compiler (Clang 14+, GCC 11+, MSVC 2022+)
- **Git** for version control

#### Build Steps

```bash
# Clone the repository
git clone https://github.com/tehuti01/Fleen-el-guitar-vst.git
cd Fleen-el-guitar-vst

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Install (optional)
cmake --install . --prefix ~/.local
```

---

## 🎛️ Interface

<div align="center">
  <img src="docs/images/interface.png" alt="Fleen El-Guitar Interface" width="800"/>
</div>

### Controls

| Control | Description | Range |
|---------|-------------|-------|
| **GAIN** | Input gain stage | 0-100% |
| **DRIVE** | Distortion amount | 0-100% |
| **BASS** | Low frequency control | 0-100% |
| **MID** | Mid frequency control | 0-100% |
| **TREBLE** | High frequency control | 0-100% |
| **PRESENCE** | Ultra-high frequency boost | 0-100% |
| **REVERB** | Reverb mix level | 0-100% |
| **COMP** | Compression amount | 0-100% |
| **BYPASS** | Effect bypass toggle | On/Off |

### Visualizers

- **Waveform Display** - Real-time input/output waveform visualization
- **Spectrum Analyzer** - Frequency spectrum display
- **Input/Output Meters** - VU-style level meters with ballistic response
- **LED Indicators** - Power and bypass status indicators

---

## 📦 Presets

Fleen El-Guitar includes 5 factory presets:

| Preset | Category | Description |
|--------|----------|-------------|
| **Default** | Factory | Balanced starting point |
| **Clean** | Factory | Crystal clean tone |
| **Crunch** | Factory | Light overdrive |
| **High Gain** | Factory | Heavy distortion |
| **Lead** | Factory | Singing lead tone |

### Managing Presets

Presets are stored in JSON format at:
- **macOS**: `~/Library/Application Support/Fleen/El-Guitar/Presets/`
- **Windows**: `%APPDATA%\Fleen\El-Guitar\Presets\`
- **Linux**: `~/.config/Fleen/El-Guitar/Presets/`

---

## 🏗️ Architecture

### Technical Stack

| Component | Technology |
|-----------|------------|
| **Framework** | JUCE 8 |
| **Language** | C++20 |
| **Build System** | CMake 3.24+ |
| **GUI Extension** | JIVE, Foleys GUI Magic |
| **Effects** | Melatonin Blur, Melatonin Inspector |
| **Graphics** | OpenGL-accelerated |

### DSP Chain

```
Input → Gain → Tone Stack (Bass/Mid/Treble/Presence) 
       → Distortion → Compressor → Reverb → Output
```

All processing is:
- ✅ Lock-free
- ✅ Allocation-free
- ✅ Real-time safe
- ✅ Sample-accurate

---

## 📖 Documentation

- [**Getting Started**](docs/GETTING_STARTED.md) - Installation and setup guide
- [**User Manual**](docs/USER_MANUAL.md) - Complete control reference
- [**Developer Guide**](docs/DEVELOPER_GUIDE.md) - Building and extending
- [**API Reference**](docs/API.md) - Code documentation
- [**FAQ**](docs/FAQ.md) - Frequently asked questions

---

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

### Development Workflow

```bash
# Fork the repository
git fork https://github.com/tehuti01/Fleen-el-guitar-vst

# Clone your fork
git clone https://github.com/YOUR_USERNAME/Fleen-el-guitar-vst

# Create a feature branch
git checkout -b feature/your-feature

# Make changes and commit
git add .
git commit -m "feat: add your feature"

# Push and create PR
git push origin feature/your-feature
```

---

## 📄 License

This project is licensed under the **GPL-3.0-or-later** License. See the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

Built with amazing open-source libraries:

- [**JUCE**](https://juce.com/) - Cross-platform audio framework
- [**Melatonin Blur**](https://github.com/sudara/melatonin_blur) - High-performance shadows
- [**JIVE**](https://github.com/ImJimmi/JIVE) - Declarative UI layouts
- [**Foleys GUI Magic**](https://github.com/ffAudio/foleys_gui_magic) - Runtime GUI designer

---

## 📬 Contact

- **Website**: [Fleen Audio](https://fleenaudio.example.com)
- **Twitter**: [@FleenAudio](https://twitter.com/FleenAudio)
- **Email**: support@fleenaudio.example.com
- **Discord**: [Join our community](https://discord.gg/example)

---

<div align="center">

**Made with ❤️ for musicians and audio engineers**

⭐ Star this repo if you find it useful!

</div>
