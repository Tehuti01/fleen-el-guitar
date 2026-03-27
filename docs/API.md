# API Reference - Fleen El-Guitar

This document provides API reference for developers extending or modifying Fleen El-Guitar.

## 📚 Table of Contents

- [Core Classes](#core-classes)
- [DSP Components](#dsp-components)
- [GUI Components](#gui-components)
- [State Management](#state-management)
- [Utilities](#utilities)

---

## Core Classes

### PluginProcessor

**File:** `src/PluginProcessor.h`

Main audio processor class inheriting from `juce::AudioProcessor`.

```cpp
namespace fleen {

class PluginProcessor : public juce::AudioProcessor,
                        private juce::AudioProcessorValueTreeState::Listener
{
public:
    PluginProcessor();
    ~PluginProcessor() override;
    
    // AudioProcessor interface
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    
    // State access
    juce::AudioProcessorValueTreeState& getParameters();
    StateManager& getStateManager();
    PresetManager& getPresetManager();
    
    // Real-time parameter access
    float getGain() const;
    float getDrive() const;
    
private:
    juce::AudioProcessorValueTreeState parameters;
    StateManager stateManager;
    PresetManager presetManager;
    
    // DSP Chain
    juce::dsp::ProcessorChain<
        juce::dsp::GainChain<juce::dsp::Gain<float>>,
        GainStage,
        ToneStack,
        Distortion,
        Compressor,
        Reverb,
        juce::dsp::DryWetMix<float>
    > dspChain;
    
    // Atomic parameters for real-time access
    std::atomic<float> gain { 0.5f };
    std::atomic<float> drive { 0.5f };
    // ... more parameters
};

} // namespace fleen
```

#### Key Methods

| Method | Description | Thread Safety |
|--------|-------------|---------------|
| `prepareToPlay()` | Initialize DSP chain | Audio thread |
| `processBlock()` | Process audio buffer | Audio thread (real-time) |
| `getParameters()` | Get APVTS instance | Thread-safe |
| `getStateManager()` | Get state manager | Thread-safe |

---

### PluginEditor

**File:** `src/PluginEditor.h`

Main editor class with skeuomorphic interface.

```cpp
namespace fleen {

class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;
    
private:
    void timerCallback() override;
    
    void setupComponents();
    void attachParameters();
    void setupOpenGL();
    
    PluginProcessor& processor;
    CustomLookAndFeel customLookAndFeel;
    
    // Components
    SkeuomorphicKnob gainKnob, driveKnob, /* ... */;
    LedIndicator powerLed, bypassLed;
    MeterDisplay inputMeter, outputMeter;
    OpenGLVisualizer visualizer;
    
    // Attachments
    std::unique_ptr<Attachment> gainAttachment;
    // ... more attachments
};

} // namespace fleen
```

---

## DSP Components

### GainStage

**File:** `src/dsp/GainStage.h`

Clean gain amplification with smooth parameter transitions.

```cpp
class GainStage
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    
    void setGain(float newGain);
    float getGain() const;
    
private:
    std::atomic<float> gainLinear { 0.5f };
};
```

### ToneStack

**File:** `src/dsp/ToneStack.h`

Baxandall-style tone control (Bass, Mid, Treble, Presence).

```cpp
class ToneStack
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    
    void setBass(float newBass);
    void setMid(float newMid);
    void setTreble(float newTreble);
    void setPresence(float newPresence);
    
private:
    juce::dsp::IIR::Filter<float> bassLeft, bassRight;
    juce::dsp::IIR::Filter<float> midLeft, midRight;
    juce::dsp::IIR::Filter<float> trebleLeft, trebleRight;
    juce::dsp::IIR::Filter<float> presenceLeft, presenceRight;
};
```

### Distortion

**File:** `src/dsp/ToneStack.h`

Soft-clipping distortion using tanh waveshaping.

```cpp
class Distortion
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    
    void setDrive(float newDrive);
    
private:
    std::atomic<float> driveLinear { 0.5f };
};
```

### Compressor

**File:** `src/dsp/ToneStack.h`

RMS-based compression with smooth attack/release.

```cpp
class Compressor
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    
    void setAmount(float amount);
    
private:
    float attackTime { 0.003f };
    float releaseTime { 0.1f };
    float ratio { 4.0f };
    float threshold { -20.0f };
};
```

### Reverb

**File:** `src/dsp/ToneStack.h`

High-quality reverb using JUCE's Reverb class.

```cpp
class Reverb
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    template<typename ProcessContext>
    void process(const ProcessContext& context) noexcept;
    
    void setMix(float mix);
    
private:
    juce::dsp::Reverb<float> reverb;
};
```

---

## GUI Components

### SkeuomorphicKnob

**File:** `src/components/SkeuomorphicKnob.h`

Hardware-inspired rotary knob with photorealistic rendering.

```cpp
class SkeuomorphicKnob : public juce::Component
{
public:
    explicit SkeuomorphicKnob(const juce::String& name);
    
    void setValue(float newValue, juce::NotificationType notification);
    float getValue() const { return value; }
    
    void setRange(float newMin, float newMax);
    
    juce::Slider& getSlider();
    void setLabelText(const juce::String& newText);
    
private:
    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    
    juce::Slider slider;
    float value { 0.5f };
    float minValue { 0.0f };
    float maxValue { 1.0f };
};
```

#### Usage Example

```cpp
// Create knob
auto* gainKnob = new SkeuomorphicKnob("GAIN");
gainKnob->setRange(0.0f, 1.0f);
gainKnob->setValue(0.5f, juce::dontSendNotification);
addAndMakeVisible(gainKnob);

// Attach to parameter
using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
gainAttachment = std::make_unique<Attachment>(
    parameters, "gain", gainKnob->getSlider());
```

### LedIndicator

**File:** `src/components/LedIndicator.h`

LED indicator with emissive glow effect.

```cpp
class LedIndicator : public juce::Component
{
public:
    enum class Color { red, blue, green, amber };
    
    explicit LedIndicator(Color ledColor = Color::red);
    
    void setState(bool isOn);
    bool getState() const { return isOn; }
    void toggle();
    void setBrightness(float newBrightness);
};
```

### MeterDisplay

**File:** `src/components/MeterDisplay.h`

VU-style level meter with ballistic response.

```cpp
class MeterDisplay : public juce::Component
{
public:
    MeterDisplay();
    
    void setLevel(float newLevel);
    float getLevel() const { return currentLevel; }
    void setSensitivity(float newSensitivity);
    
private:
    float currentLevel { 0.0f };
    float attackTime { 0.01f };
    float releaseTime { 0.3f };
};
```

### OpenGLVisualizer

**File:** `src/components/OpenGLVisualizer.h`

OpenGL-accelerated real-time visualization.

```cpp
class OpenGLVisualizer : public juce::Component,
                         private juce::Timer
{
public:
    enum class Mode { waveform, spectrum, spectrogram };
    
    OpenGLVisualizer();
    
    void setProcessor(PluginProcessor* newProcessor);
    void updateDisplay();
    void setMode(Mode newMode);
    
private:
    void timerCallback() override;
    void paint(juce::Graphics&) override;
    
    PluginProcessor* processor { nullptr };
    Mode visualizerMode { Mode::waveform };
};
```

---

## State Management

### StateManager

**File:** `src/state/StateManager.h`

Thread-safe state management with undo/redo support.

```cpp
class StateManager
{
public:
    void setParameter(const juce::String& parameterId, float newValue);
    float getParameter(const juce::String& parameterId) const;
    std::map<juce::String, float> getAllParameters() const;
    void resetToDefaults();
    
    juce::ValueTree saveState() const;
    void loadState(const juce::ValueTree& state);
    
    juce::String saveToJson() const;
    void loadFromJson(const juce::String& json);
    
    void saveUndoState();
    bool undo();
    bool redo();
    void clearHistory();
};
```

#### Usage Example

```cpp
// Set parameter
stateManager.setParameter("gain", 0.75f);

// Save state for undo
stateManager.saveUndoState();

// Undo last change
if (stateManager.undo())
{
    // Undo successful
}

// Serialize to JSON
juce::String json = stateManager.saveToJson();

// Load from JSON
stateManager.loadFromJson(json);
```

### PresetManager

**File:** `src/state/PresetManager.h`

Preset management with JSON serialization.

```cpp
class PresetManager
{
public:
    struct Preset
    {
        juce::String name;
        juce::String category;
        juce::ValueTree state;
        bool isFactoryPreset { true };
    };
    
    explicit PresetManager(PluginProcessor& processor);
    
    void loadFactoryPresets();
    void loadUserPresets();
    
    const juce::Array<Preset>& getPresetList() const;
    int getCurrentPresetIndex() const;
    juce::String getCurrentPresetName() const;
    
    void loadPreset(int index);
    void loadPreset(const juce::String& name);
    bool savePreset(const juce::String& name, const juce::String& category = "User");
    
    bool deletePreset(int index);
    bool isPresetModified() const;
    
    static juce::File getPresetDirectory();
};
```

---

## Utilities

### GoldenRatio

**File:** `src/utils/GoldenRatio.h`

Golden Ratio constants and helper functions.

```cpp
namespace GoldenRatio
{
    // Constants
    constexpr float phi = 1.61803398875f;
    constexpr float invPhi = 0.61803398875f;
    
    // Spacing scale
    constexpr int baseUnit = 10;
    constexpr int spacingScale[] = { 6, 10, 16, 26, 42, 68, 110, 178 };
    
    inline int getSpacing(int tier);
    inline float getFontSize(int steps);
    inline int getPrimaryDimension(int total);
    inline int getSecondaryDimension(int total);
    
    inline juce::Rectangle<int> centerInBounds(
        const juce::Rectangle<int>& bounds,
        int componentWidth,
        int componentHeight);
}
```

#### Usage Example

```cpp
// Get spacing for layout
const int spacing = GoldenRatio::getSpacing(2); // Returns 16

// Calculate font size
const float fontSize = GoldenRatio::getFontSize(1); // Returns ~22.7

// Golden Ratio layout split
const int mainWidth = GoldenRatio::getPrimaryDimension(getWidth());  // 61.8%
const int sideWidth = GoldenRatio::getSecondaryDimension(getWidth()); // 38.2%
```

### CustomLookAndFeel

**File:** `src/lookandfeel/CustomLookAndFeel.h`

Custom LookAndFeel with skeuomorphic styling.

```cpp
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                         float sliderPos, float rotaryStartAngle,
                         float rotaryEndAngle, juce::Slider&) override;
    
    void drawButtonBackground(juce::Graphics&, juce::Button&,
                             const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted,
                             bool shouldDrawButtonAsDown) override;
    
    void drawNeumorphicShadow(juce::Graphics& g,
                             const juce::Rectangle<int>& bounds,
                             bool isRecessed,
                             int blurRadius);
    
    void drawLedGlow(juce::Graphics& g,
                    float centreX, float centreY,
                    float radius,
                    const juce::Colour& colour,
                    float intensity);
};
```

---

## Parameter IDs

All parameters are identified by string IDs:

| ID | Name | Range | Default |
|----|------|-------|---------|
| `gain` | Gain | 0.0 - 1.0 | 0.5 |
| `drive` | Drive | 0.0 - 1.0 | 0.5 |
| `bass` | Bass | 0.0 - 1.0 | 0.5 |
| `mid` | Mid | 0.0 - 1.0 | 0.5 |
| `treble` | Treble | 0.0 - 1.0 | 0.5 |
| `presence` | Presence | 0.0 - 1.0 | 0.5 |
| `reverb` | Reverb | 0.0 - 1.0 | 0.3 |
| `compression` | Compression | 0.0 - 1.0 | 0.5 |
| `bypass` | Bypass | 0.0 - 1.0 | 0.0 |

---

## Thread Safety Guidelines

### Audio Thread (Real-Time)

```cpp
// ✅ Safe in audio thread
float gain = processor.getGain(); // Atomic access
dspChain.process(context);        // Lock-free processing

// ❌ NOT safe in audio thread
juce::String name = preset.getName(); // Allocation
myMutex.lock();                        // Locking
```

### GUI Thread

```cpp
// ✅ Safe in GUI thread
parameter->setValueNotifyingHost(0.5f);
repaint();
presetManager.savePreset("My Preset", "User");

// ⚠️ Use MessageManager for thread marshalling
juce::MessageManager::callAsync([]()
{
    // Runs on message thread
    myComponent->setVisible(true);
});
```

---

<div align="center">

**Fleen El-Guitar API Reference**

For more information, see [Developer Guide](DEVELOPER_GUIDE.md)

</div>
