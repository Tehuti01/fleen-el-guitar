#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_opengl/juce_opengl.h>

namespace fleen
{

class PluginProcessor;
class SkeuomorphicKnob;
class LedIndicator;
class MeterDisplay;
class OpenGLVisualizer;
class PresetSelector;
class CustomLookAndFeel;

/**
 * @brief Fleen El-Guitar Plugin Editor
 * 
 * Premium skeuomorphic interface with:
 * - Golden Ratio layout
 * - 12 premium controls
 * - Real-time visualizations
 * - Test keyboard
 */
class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setupComponents();
    void attachParameters();
    void timerCallback() override;

    PluginProcessor& processor;
    CustomLookAndFeel* customLookAndFeel;
    
    // Main panels
    juce::Component mainPanel;
    juce::Component sidebar;
    juce::Component bottomPanel;
    
    // Knobs (12 premium controls)
    SkeuomorphicKnob* gainKnob;
    SkeuomorphicKnob* driveKnob;
    SkeuomorphicKnob* bassKnob;
    SkeuomorphicKnob* midKnob;
    SkeuomorphicKnob* trebleKnob;
    SkeuomorphicKnob* presenceKnob;
    SkeuomorphicKnob* reverbKnob;
    SkeuomorphicKnob* compressionKnob;
    SkeuomorphicKnob* delayKnob;
    SkeuomorphicKnob* chorusKnob;
    SkeuomorphicKnob* depthKnob;
    SkeuomorphicKnob* speedKnob;
    
    // Indicators
    LedIndicator* powerLed;
    LedIndicator* bypassLed;
    
    // Meters
    MeterDisplay* inputMeter;
    MeterDisplay* outputMeter;
    
    // Visualizer
    OpenGLVisualizer* visualizer;
    
    // Preset selector
    PresetSelector* presetSelector;
    
    // Bypass button
    juce::TextButton bypassButton;
    
    // Parameter attachments
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    std::unique_ptr<Attachment> gainAttachment;
    std::unique_ptr<Attachment> driveAttachment;
    std::unique_ptr<Attachment> bassAttachment;
    std::unique_ptr<Attachment> midAttachment;
    std::unique_ptr<Attachment> trebleAttachment;
    std::unique_ptr<Attachment> presenceAttachment;
    std::unique_ptr<Attachment> reverbAttachment;
    std::unique_ptr<Attachment> compressionAttachment;
    std::unique_ptr<Attachment> delayAttachment;
    std::unique_ptr<Attachment> chorusAttachment;
    std::unique_ptr<Attachment> depthAttachment;
    std::unique_ptr<Attachment> speedAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

} // namespace fleen
