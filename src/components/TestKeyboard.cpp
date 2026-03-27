#include "TestKeyboard.h"
#include "../PluginProcessor.h"

namespace fleen
{

TestKeyboard::TestKeyboard (PluginProcessor& proc)
    : processor (proc),
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
}

TestKeyboard::~TestKeyboard()
{
    keyboardState.removeListener (this);
}

void TestKeyboard::paint (juce::Graphics& g)
{
    g.setRenderQuality (juce::Graphics::highRenderingQuality);
    
    // Dark background
    g.setColour (juce::Colour::fromRGB (25, 25, 30));
    g.fillRect (getLocalBounds());
    
    // Label
    g.setColour (juce::Colours::white.withMultipliedAlpha (0.7f));
    g.setFont (12.0f);
    g.drawText ("Test Keyboard", getLocalBounds().removeFromTop (20), juce::Justification::centred);
}

void TestKeyboard::resized()
{
    auto bounds = getLocalBounds().reduced (8);
    bounds.removeFromTop (20); // Space for label
    keyboardComponent.setBounds (bounds);
}

void TestKeyboard::handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Send note to processor for testing
    // In a full implementation, this would trigger guitar samples
}

void TestKeyboard::handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    // Handle note off
}

} // namespace fleen
