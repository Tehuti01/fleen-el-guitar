#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

namespace fleen
{

class PluginProcessor;

/**
 * @brief Mini keyboard for testing guitar sounds
 */
class TestKeyboard : public juce::Component,
                     public juce::MidiKeyboardStateListener
{
public:
    explicit TestKeyboard (PluginProcessor&);
    ~TestKeyboard() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
private:
    PluginProcessor& processor;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestKeyboard)
};

} // namespace fleen
