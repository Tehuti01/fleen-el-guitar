# Fleen El-Guitar - Project Context

Professional-grade guitar processing VST3 plugin built with JUCE 8 and modern C++20.

## 🎸 Project Overview

**Fleen El-Guitar** combines cutting-edge DSP algorithms with a premium skeuomorphic interface. It is designed for high-performance, real-time audio processing with a focus on both sound quality and visual aesthetics (utilizing the Golden Ratio $\Phi = 1.618$ for UI layout).

### Core Technologies
- **Framework:** [JUCE 8](https://juce.com/)
- **Language:** C++20 (requires Clang 14+, GCC 11+, or MSVC 2022+)
- **Build System:** CMake 3.24+ with [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)
- **Graphics:** OpenGL-accelerated visualizers, [Melatonin Blur](https://github.com/sudara/melatonin_blur) for neumorphic shadows
- **UI:** [JIVE](https://github.com/ImJimmi/JIVE) for declarative layouts, [Foleys GUI Magic](https://github.com/ffAudio/foleys_gui_magic)

### Architecture
- **DSP Chain:** Gain → Tone Stack (Bass/Mid/Treble/Presence) → Distortion → Compressor → Reverb.
- **State Management:** JSON-based preset system with `AudioProcessorValueTreeState` for parameter persistence.
- **Real-Time Safety:** Lock-free and allocation-free audio thread processing using `std::atomic` for parameter syncing.

---

## 🛠️ Building and Running

### Prerequisites
- CMake 3.24+
- C++20 compatible compiler
- Ninja (optional, recommended for faster builds)

### Standard Build Workflow
```bash
# Configure and Build
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Output Locations
# VST3: build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3
# Standalone: build/FleenElGuitar_artefacts/Release/Standalone/
```

### Testing and Validation
- **Unit Tests:** Uses [Catch2](https://github.com/catchorg/Catch2) (located in `tests/`).
- **Plugin Validation:** Use [pluginval](https://pluginval.com/) to ensure VST3 stability.
  ```bash
  pluginval --validate path/to/FleenElGuitar.vst3 --level 2
  ```

---

## 📏 Development Conventions

### Coding Standards
- **Naming:** 
  - `PascalCase` for Classes/Structs.
  - `camelCase` for functions and variables.
  - `PascalCase` or `UPPER_SNAKE_CASE` for constants.
- **Formatting:** 4-space indentation, ~100 character line limit.
- **Audio Thread:** **CRITICAL:** Never use `new`, `malloc`, `free`, `printf`, `std::mutex`, or file I/O inside `processBlock`. Use `std::atomic` or lock-free queues for communication.

### Project Structure
- `src/dsp/`: Core audio algorithms.
- `src/components/`: Custom JUCE UI components (Knobs, Meters, Visualizers).
- `src/state/`: Preset and parameter state logic.
- `src/lookandfeel/`: Custom `juce::LookAndFeel` implementations.
- `assets/`: Binary resources including presets (JSON), fonts, and images.

---

## 📦 Key Commands & Scripts

- **Install (macOS):** `brew install fleen-el-guitar` (using `homebrew/fleen-el-guitar.rb`).
- **Quick Install:** `curl -fsSL ... | bash` (using `scripts/install.sh`).
- **Cleanup:** `rm -rf build/` to reset the build environment.

---

## 📖 Documentation Reference
- [Developer Guide](docs/DEVELOPER_GUIDE.md) - Detailed architecture and extension guide.
- [API Reference](docs/API.md) - Internal code documentation.
- [User Manual](docs/USER_MANUAL.md) - Plugin control reference.
