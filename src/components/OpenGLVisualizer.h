#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
#include "../PluginProcessor.h"

namespace fleen
{

/**
 * @brief OpenGL-accelerated real-time visualizer
 * 
 * Hardware-accelerated waveform and spectrum visualization
 * inspired by Vital's spectral warping displays.
 */
class OpenGLVisualizer : public juce::Component,
                         private juce::Timer
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    OpenGLVisualizer();
    ~OpenGLVisualizer() override;

    // ========================================================================
    // Component Interface
    // ========================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // ========================================================================
    // Setup
    // ========================================================================
    
    /** @brief Set processor reference for audio data */
    void setProcessor (PluginProcessor* newProcessor);
    
    /** @brief Update display with latest audio data */
    void updateDisplay();
    
    /** @brief Set visualization mode */
    enum class Mode
    {
        waveform,
        spectrum,
        spectrogram
    };
    
    void setMode (Mode newMode);

private:
    // ========================================================================
    // Timer Interface
    // ========================================================================
    
    void timerCallback() override;

    // ========================================================================
    // Rendering
    // ========================================================================
    
    /** @brief Draw waveform visualization */
    void drawWaveform (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw spectrum visualization */
    void drawSpectrum (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw background grid */
    void drawGrid (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw label overlay */
    void drawLabel (juce::Graphics& g, const juce::Rectangle<float>& bounds);

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    PluginProcessor* processor { nullptr };
    
    // Visualization state
    Mode visualizerMode { Mode::waveform };
    std::array<float, 512> waveformBuffer {};
    std::array<float, 256> spectrumBuffer {};
    
    // Animation
    float phase { 0.0f };
    float glowIntensity { 0.0f };
    
    // Colours
    juce::Colour primaryColour { 0, 150, 200 };
    juce::Colour secondaryColour { 255, 100, 150 };
    juce::Colour gridColour { 60, 60, 70 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLVisualizer)
};

} // namespace fleen
