# Developer Guide - Fleen El-Guitar

This guide is for developers who want to build, extend, or contribute to Fleen El-Guitar.

## 📋 Table of Contents

- [Architecture Overview](#architecture-overview)
- [Building from Source](#building-from-source)
- [Project Structure](#project-structure)
- [DSP Implementation](#dsp-implementation)
- [GUI System](#gui-system)
- [Testing](#testing)
- [Extending the Plugin](#extending-the-plugin)

---

## Architecture Overview

### System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      Fleen El-Guitar                         │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────┐  ┌─────────────────────────────────┐   │
│  │  PluginEditor   │  │         PluginProcessor          │   │
│  │                 │  │                                  │   │
│  │  ┌───────────┐  │  │  ┌──────────────────────────┐   │   │
│  │  │   Knobs   │  │  │  │   AudioProcessorValueTree │   │   │
│  │  └───────────┘  │  │  └──────────────────────────┘   │   │
│  │  ┌───────────┐  │  │  ┌──────────────────────────┐   │   │
│  │  │  Meters   │  │  │  │      StateManager        │   │   │
│  │  └───────────┘  │  │  └──────────────────────────┘   │   │
│  │  ┌───────────┐  │  │  ┌──────────────────────────┐   │   │
│  │  │Visualizer │  │  │  │     PresetManager        │   │   │
│  │  └───────────┘  │  │  └──────────────────────────┘   │   │
│  │                 │  │                                  │   │
│  │  CustomLookAndFeel  │  ┌──────────────────────────┐   │   │
│  │                 │  │  │    DSP Processor Chain   │   │   │
│  └─────────────────┘  │  │  Gain → Tone → Distortion│   │   │
│                       │  │  → Comp → Reverb → Out   │   │   │
│                       │  └──────────────────────────┘   │   │
└─────────────────────────────────────────────────────────────┘
```

### Data Flow

```
Audio Input
    ↓
[Gain Stage] ──────────────────── (Atomic: gain)
    ↓
[Tone Stack] ──────────────────── (Atomic: bass, mid, treble, presence)
    ↓
[Distortion] ──────────────────── (Atomic: drive)
    ↓
[Compressor] ──────────────────── (Atomic: compression)
    ↓
[Reverb] ──────────────────────── (Atomic: reverb)
    ↓
[Dry/Wet Mix]
    ↓
Audio Output
    ↓
Level Meters (Atomic: inputLevel, outputLevel)
```

---

## Building from Source

### Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| CMake | 3.24+ | Build system |
| C++ Compiler | C++20 | Language standard |
| Git | Latest | Version control |
| Ninja | Latest (optional) | Faster builds |

#### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake ninja git
```

#### Windows

1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with C++ workload
2. Install [CMake](https://cmake.org/download/)
3. Install [Ninja](https://ninja-build.org/)

#### Linux

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y cmake ninja-build g++ git \
    libasound2-dev libcurl4-openssl-dev \
    libx11-dev libxcomposite-dev libxcursor-dev \
    libxinerama-dev libglu1-mesa-dev libfreetype6-dev
```

### Build Commands

```bash
# Clone repository
git clone https://github.com/tehuti01/Fleen-el-guitar-vst.git
cd Fleen-el-guitar-vst

# Create build directory
mkdir build && cd build

# Configure (Release)
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release --parallel $(nproc)

# Output locations
# VST3: build/FleenElGuitar_artefacts/Release/VST3/FleenElGuitar.vst3
# Standalone: build/FleenElGuitar_artefacts/Release/Standalone/
```

### Build Configuration Options

```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Custom install prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/custom/path

# Disable LTO (faster builds, larger binary)
cmake .. -DJUCE_ENABLE_LTO=OFF

# Enable sanitizers (Debug only)
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
```

---

## Project Structure

```
Fleen-el-guitar-vst/
├── CMakeLists.txt              # Main build configuration
├── README.md                   # Project overview
├── LICENSE                     # GPL-3.0 license
├── CONTRIBUTING.md             # Contribution guidelines
├── .gitignore                  # Git ignore rules
├── .github/
│   └── workflows/
│       └── build.yml           # CI/CD pipeline
├── scripts/
│   └── install.sh              # Installation script
├── homebrew/
│   └── fleen-el-guitar.rb      # Homebrew formula
├── docs/
│   ├── README.md               # Documentation index
│   ├── GETTING_STARTED.md      # Installation guide
│   ├── USER_MANUAL.md          # User reference
│   ├── DEVELOPER_GUIDE.md      # This file
│   ├── API.md                  # API reference
│   └── FAQ.md                  # FAQ
├── assets/
│   ├── fonts/                  # Font files
│   ├── images/                 # Image assets
│   └── presets/                # Factory presets (JSON)
└── src/
    ├── PluginProcessor.h/cpp   # Main audio processor
    ├── PluginEditor.h/cpp      # Main GUI editor
    ├── lookandfeel/
    │   └── CustomLookAndFeel.h/cpp
    ├── dsp/
    │   ├── GainStage.h
    │   ├── ToneStack.h
    │   └── (Distortion, Compressor, Reverb)
    ├── components/
    │   ├── SkeuomorphicKnob.h/cpp
    │   ├── LedIndicator.h/cpp
    │   ├── MeterDisplay.h/cpp
    │   └── OpenGLVisualizer.h/cpp
    ├── state/
    │   ├── StateManager.h/cpp
    │   └── PresetManager.h/cpp
    └── utils/
        └── GoldenRatio.h
```

---

## DSP Implementation

### Adding a New DSP Effect

1. **Create header file** in `src/dsp/`:

```cpp
// src/dsp/MyEffect.h
#pragma once
#include <JuceHeader.h>

namespace fleen {

class MyEffect
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        // Initialize effect
    }
    
    void reset()
    {
        // Reset state
    }
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        const auto inputBlock = context.getInputBlock();
        const auto outputBlock = context.getOutputBlock();
        
        // Process audio
        for (size_t channel = 0; channel < inputBlock.getNumChannels(); ++channel)
        {
            auto* in = inputBlock.getChannelPointer(channel);
            auto* out = outputBlock.getChannelPointer(channel);
            
            for (size_t i = 0; i < inputBlock.getNumSamples(); ++i)
            {
                out[i] = processSample(in[i]);
            }
        }
    }
    
    void setParameter(float newValue)
    {
        parameter.store(newValue);
    }
    
private:
    float processSample(float sample)
    {
        // DSP algorithm here
        return sample;
    }
    
    double sampleRate { 44100.0 };
    std::atomic<float> parameter { 0.5f };
};

} // namespace fleen
```

2. **Add to processor chain** in `PluginProcessor.h`:

```cpp
#include "dsp/MyEffect.h"

juce::dsp::ProcessorChain<
    juce::dsp::GainChain<juce::dsp::Gain<float>>,
    GainStage,
    ToneStack,
    Distortion,
    Compressor,
    Reverb,
    MyEffect,  // Add your effect here
    juce::dsp::DryWetMix<float>
> dspChain;
```

3. **Update processBlock** in `PluginProcessor.cpp`:

```cpp
auto& myEffect = dspChain.get<6>(); // Index of your effect
myEffect.setParameter(myParameter.load());
```

---

## GUI System

### Adding a New Control

1. **Add parameter** to `PluginProcessor::createParameterLayout()`:

```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    "myparam", "My Parameter",
    juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
    0.5f
));
```

2. **Add atomic variable** for real-time access:

```cpp
std::atomic<float> myParameter { 0.5f };
```

3. **Update parameterChanged** listener:

```cpp
void PluginProcessor::parameterChanged(const juce::String& id, float value)
{
    if (id == "myparam")
        myParameter = value;
    // ... other parameters
}
```

4. **Add control to editor** in `PluginEditor`:

```cpp
// In header
SkeuomorphicKnob myKnob;
std::unique_ptr<Attachment> myAttachment;

// In setupComponents()
myKnob.setLabelText("MY PARAM");
mainPanel.addAndMakeVisible(myKnob);

// In attachParameters()
myAttachment = std::make_unique<Attachment>(
    processor.getParameters(), "myparam", myKnob.getSlider());
```

5. **Position in resized()**:

```cpp
void PluginEditor::resized()
{
    // ... existing layout
    
    myKnob.setBounds(/* your bounds */);
}
```

---

## Testing

### Unit Tests

Create tests in `tests/`:

```cpp
// tests/DSPTests.cpp
#include <catch2/catch.hpp>
#include "../src/dsp/GainStage.h"

TEST_CASE("GainStage processes audio correctly", "[dsp]")
{
    fleen::GainStage gainStage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    gainStage.prepare(spec);
    gainStage.setGain(0.5f);
    
    juce::AudioBuffer<float> buffer(2, 512);
    buffer.clear();
    
    // Add test signal
    for (int i = 0; i < 512; ++i)
    {
        buffer.setSample(0, i, 0.5f);
        buffer.setSample(1, i, 0.5f);
    }
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    gainStage.process(context);
    
    // Verify output
    for (int i = 0; i < 512; ++i)
    {
        REQUIRE(buffer.getSample(0, i) == Approx(0.25f).margin(0.001f));
        REQUIRE(buffer.getSample(1, i) == Approx(0.25f).margin(0.001f));
    }
}
```

### Plugin Validation

Use [pluginval](https://pluginval.com/) for validation:

```bash
# Download pluginval
curl -L https://github.com/Tracktion/pluginval/releases/download/v1.0.3/pluginval_macOS.zip -o pluginval.zip
unzip pluginval.zip

# Validate plugin
./pluginval.app/Contents/MacOS/pluginval \
    --validate /path/to/FleenElGuitar.vst3 \
    --level 2
```

---

## Extending the Plugin

### Adding a New Preset Category

1. **Update PresetManager**:

```cpp
void PresetManager::loadFactoryPresets()
{
    // ... existing presets
    
    // New category
    {
        juce::ValueTree state("Preset");
        // Set parameters
        presets.add(Preset("New Preset", "New Category", state, true));
    }
}
```

### Custom Visualizations

1. **Extend OpenGLVisualizer**:

```cpp
void OpenGLVisualizer::drawCustomVisualization(
    juce::Graphics& g,
    const juce::Rectangle<float>& bounds)
{
    // Your custom OpenGL rendering here
}
```

### Third-Party Integration

Fleen El-Guitar uses CPM.cmake for dependency management:

```cmake
# In CMakeLists.txt
CPMAddPackage(
    NAME some_library
    GITHUB_REPOSITORY user/some_library
    VERSION 1.0.0
)
```

---

## Performance Optimization

### Audio Thread Best Practices

```cpp
// ✅ DO: Use atomics for parameter access
float gain = gainParameter.load();

// ✅ DO: Pre-allocate buffers
std::array<float, 1024> buffer;

// ✅ DO: Use SIMD where possible
#include <juce_dsp/juce_dsp.h>

// ❌ DON'T: Allocate memory
auto* buffer = new float[size]; // NO!

// ❌ DON'T: Use locks
std::mutex mtx;
mtx.lock(); // NO!

// ❌ DON'T: File I/O
std::ifstream file("data.txt"); // NO!
```

### GUI Optimization

```cpp
// Set opaque for better performance
setOpaque(true);

// Minimize repaint area
repaint(smallBounds); // Not repaint()

// Use timer for animations, not continuous repaint
startTimerHz(60);

void timerCallback() override
{
    repaint();
}
```

---

## Debugging

### Enable Debug Output

```cpp
// In PluginProcessor.cpp
DBG("Plugin initialized");
DBG("Gain value: " << gain.load());

// Conditional debug
#if JUCE_DEBUG
    DBG("Debug info: " << someValue);
#endif
```

### Audio Graph Visualization

Use JUCE's audio graph debugger:

```cpp
// In CMakeLists.txt
target_compile_definitions(FleenElGuitar
    PRIVATE
        JUCE_ENABLE_AUDIO_GRAPH_DEBUGGER=1
)
```

---

## Code Style

See [CONTRIBUTING.md](../CONTRIBUTING.md#code-style) for detailed style guidelines.

Quick summary:
- **Classes**: PascalCase
- **Functions**: camelCase
- **Variables**: camelCase
- **Constants**: PascalCase or UPPER_SNAKE_CASE
- **Indentation**: 4 spaces
- **Line length**: ~100 characters

---

## Resources

- [JUCE Documentation](https://juce.com/learn/tutorials)
- [CMake Documentation](https://cmake.org/documentation/)
- [DSP Guide](https://www.dspguide.com/)
- [VST3 SDK](https://www.steinberg.net/developers/)

---

<div align="center">

**Fleen El-Guitar Developer Guide**

Previous: [API Reference](API.md) | Next: [Contributing](../CONTRIBUTING.md)

</div>
