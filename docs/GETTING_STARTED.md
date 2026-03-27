# Getting Started with Fleen El-Guitar

This guide will help you install and start using Fleen El-Guitar VST3.

## 📋 System Requirements

### Minimum Requirements

| Component | Requirement |
|-----------|-------------|
| **Operating System** | macOS 10.15+, Windows 10+, Linux (Ubuntu 20.04+) |
| **CPU** | Intel Core i5 or equivalent (Apple Silicon supported) |
| **RAM** | 4 GB |
| **Disk Space** | 500 MB |
| **Plugin Format** | VST3 host required |

### Recommended Requirements

| Component | Recommendation |
|-----------|----------------|
| **Operating System** | macOS 12+, Windows 11+, Linux (Ubuntu 22.04+) |
| **CPU** | Intel Core i7 / Apple M1 or better |
| **RAM** | 8 GB |
| **Display** | 1920x1080 or higher |

---

## 🚀 Installation

### Homebrew Installation (macOS)

The easiest way to install on macOS is via Homebrew:

```bash
# Step 1: Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Step 2: Add the Fleen Homebrew tap
brew tap tehuti01/audio

# Step 3: Install Fleen El-Guitar
brew install fleen-el-guitar
```

**Verify Installation:**

```bash
ls ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
```

### Manual Installation

#### macOS

1. Download the latest release from [GitHub Releases](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest)

2. Extract the downloaded archive:
   ```bash
   unzip FleenElGuitar-v1.0.0-macOS.zip
   ```

3. Copy the VST3 bundle to your plugins directory:
   ```bash
   cp -R FleenElGuitar.vst3 ~/Library/Audio/Plug-Ins/VST3/
   ```

4. (Optional) Install the standalone application:
   ```bash
   cp -R "Fleen El-Guitar.app" ~/Applications/
   ```

#### Windows

1. Download the Windows installer from [GitHub Releases](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest)

2. Run the installer and follow the prompts

3. The installer will automatically place the VST3 in:
   ```
   C:\Program Files\Common Files\VST3\
   ```

#### Linux

1. Download the Linux build from [GitHub Releases](https://github.com/tehuti01/Fleen-el-guitar-vst/releases/latest)

2. Extract and install:
   ```bash
   mkdir -p ~/.vst3
   cp -R FleenElGuitar.vst3 ~/.vst3/
   ```

### Install Script (macOS)

For automated installation on macOS:

```bash
curl -fsSL https://raw.githubusercontent.com/tehuti01/Fleen-el-guitar-vst/main/scripts/install.sh | bash
```

---

## 🎸 Quick Start

### Using in Your DAW

1. **Open your DAW** (Logic Pro, Ableton Live, Cubase, etc.)

2. **Scan for new plugins** (if necessary):
   - **Logic Pro**: Plugins are scanned automatically
   - **Ableton Live**: Options → Preferences → Plug-Ins → Browse
   - **Cubase**: Devices → Plug-in Information → Refresh

3. **Insert Fleen El-Guitar** on a guitar track:
   - Look for "Fleen El-Guitar" or "FleenElGuitar" in your VST3 plugins list
   - Insert as an insert effect

4. **Select a preset** from the dropdown menu:
   - Start with "Default" for a balanced tone
   - Try "Clean" for pristine clean tones
   - Use "Crunch" or "High Gain" for distorted sounds

5. **Adjust controls** to taste:
   - Start with Gain and Drive for basic tone shaping
   - Use the EQ section (Bass, Mid, Treble, Presence) for frequency balancing
   - Add Reverb and Compression for depth and sustain

### Standalone Mode

1. Launch the standalone application:
   - **macOS**: `/Applications/Fleen El-Guitar.app`
   - **Windows**: `C:\Program Files\Fleen El-Guitar\Fleen El-Guitar.exe`

2. Connect your audio interface

3. Adjust buffer size in settings for optimal latency

---

## 🔧 Building from Source

### Prerequisites

Install the required build tools:

#### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake ninja
```

#### Windows

1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with C++ workload
2. Install [CMake](https://cmake.org/download/)
3. Install [Ninja](https://ninja-build.org/)

#### Linux

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y cmake ninja-build g++ libasound2-dev libcurl4-openssl-dev libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev libglu1-mesa-dev

# Fedora
sudo dnf install -y cmake ninja-build gcc-c++ alsa-lib-devel curl-devel libX11-devel libXcomposite-devel libXcursor-devel libXinerama-devel mesa-libGL-devel
```

### Build Steps

```bash
# Clone the repository
git clone https://github.com/tehuti01/Fleen-el-guitar-vst.git
cd Fleen-el-guitar-vst

# Create build directory
mkdir build && cd build

# Configure (Release build)
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# The VST3 will be in:
# build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `CMAKE_BUILD_TYPE` | Build configuration (Debug/Release) | Release |
| `CMAKE_INSTALL_PREFIX` | Installation directory | System default |
| `JUCE_ENABLE_LTO` | Link-time optimization | ON |

### Testing

```bash
# Run unit tests
ctest --test-dir build

# Validate plugin with pluginval
# (Download pluginval from https://github.com/Tracktion/pluginval)
pluginval /path/to/FleenElGuitar.vst3
```

---

## ❓ Troubleshooting

### Plugin Not Showing in DAW

1. **Verify installation location**:
   ```bash
   # macOS
   ls ~/Library/Audio/Plug-Ins/VST3/FleenElGuitar.vst3
   
   # Windows
   dir "C:\Program Files\Common Files\VST3\FleenElGuitar.vst3"
   ```

2. **Rescan plugins** in your DAW

3. **Check DAW compatibility** (must support VST3)

### Audio Issues

1. **Increase buffer size** in your DAW or audio interface settings

2. **Check sample rate** compatibility (44.1kHz, 48kHz, 88.2kHz, 96kHz supported)

3. **Disable other plugins** to isolate the issue

### Build Errors

1. **Update CMake** to version 3.24 or later:
   ```bash
   cmake --version
   ```

2. **Clean build directory**:
   ```bash
   rm -rf build && mkdir build
   ```

3. **Update submodules**:
   ```bash
   git submodule update --init --recursive
   ```

---

## 📞 Support

If you encounter issues not covered here:

1. Check the [FAQ](FAQ.md)
2. Search [existing issues](https://github.com/tehuti01/Fleen-el-guitar-vst/issues)
3. [Create a new issue](https://github.com/tehuti01/Fleen-el-guitar-vst/issues/new) with:
   - Your operating system
   - DAW name and version
   - Steps to reproduce the issue
   - Any error messages

---

<div align="center">

**Next Steps:** [User Manual](USER_MANUAL.md) → 

</div>
