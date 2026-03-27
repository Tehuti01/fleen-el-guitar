#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "lookandfeel/CustomLookAndFeel.h"
#include "components/SkeuomorphicKnob.h"
#include "components/LedIndicator.h"
#include "components/MeterDisplay.h"
#include "components/OpenGLVisualizer.h"

namespace fleen
{

/**
 * @brief Fleen El-Guitar Plugin Editor
 * 
 * Premium skeuomorphic interface with Golden Ratio-based layout.
 * Features hardware-inspired controls with photorealistic rendering.
 */
class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    // ========================================================================
    // Component Interface
    // ========================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;

private:
    // ========================================================================
    // Timer Interface
    // ========================================================================
    
    void timerCallback() override;

    // ========================================================================
    // Layout Helpers
    // ========================================================================
    
    /** @brief Calculate spacing based on Golden Ratio */
    static constexpr float phi = 1.61803398875f;
    static constexpr int baseUnit = 10;
    
    int getSpacing (int tier) const
    {
        // Fibonacci-based spacing scale
        constexpr int spacing[] = { 6, 10, 16, 26, 42, 68 };
        return spacing[juce::jlimit (0, 5, tier)];
    }
    
    /** @brief Setup the component hierarchy */
    void setupComponents();
    
    /** @Attach parameters to UI controls */
    void attachParameters();
    
    /** @brief Setup the OpenGL context for visualizations */
    void setupOpenGL();

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    PluginProcessor& processor;
    
    // Custom LookAndFeel
    CustomLookAndFeel customLookAndFeel;
    
    // Main Components
    juce::Component mainPanel;
    juce::Component sidebar;
    
    // Skeuomorphic Knobs
    SkeuomorphicKnob gainKnob;
    SkeuomorphicKnob driveKnob;
    SkeuomorphicKnob bassKnob;
    SkeuomorphicKnob midKnob;
    SkeuomorphicKnob trebleKnob;
    SkeuomorphicKnob presenceKnob;
    SkeuomorphicKnob reverbKnob;
    SkeuomorphicKnob compressionKnob;
    
    // LED Indicators
    LedIndicator powerLed;
    LedIndicator bypassLed;
    
    // Meter Displays
    MeterDisplay inputMeter;
    MeterDisplay outputMeter;
    
    // OpenGL Visualizer
    OpenGLVisualizer visualizer;
    
    // Bypass Button
    juce::TextButton bypassButton;
    
    // Preset ComboBox
    juce::ComboBox presetBox;
    
    // Parameter Attachments
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> gainAttachment;
    std::unique_ptr<Attachment> driveAttachment;
    std::unique_ptr<Attachment> bassAttachment;
    std::unique_ptr<Attachment> midAttachment;
    std::unique_ptr<Attachment> trebleAttachment;
    std::unique_ptr<Attachment> presenceAttachment;
    std::unique_ptr<Attachment> reverbAttachment;
    std::unique_ptr<Attachment> compressionAttachment;
    std::unique_ptr<Attachment> bypassAttachment;
    
    // Timer for meter updates (60 FPS)
    static constexpr int meterUpdateInterval = 16; // ~60 Hz
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

} // namespace fleen
