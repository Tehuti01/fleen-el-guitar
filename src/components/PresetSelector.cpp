#include "PresetSelector.h"
#include "../PluginProcessor.h"

namespace fleen
{

PresetSelector::PresetSelector (PluginProcessor& proc)
    : processor (proc),
      saveButton ("Save"),
      deleteButton ("Delete"),
      prevButton ("<"),
      nextButton (">")
{
    addAndMakeVisible (presetBox);
    addAndMakeVisible (saveButton);
    addAndMakeVisible (deleteButton);
    addAndMakeVisible (prevButton);
    addAndMakeVisible (nextButton);
    
    presetBox.setColour (juce::ComboBox::backgroundColourId, juce::Colour::fromRGB (40, 40, 45));
    presetBox.setColour (juce::ComboBox::textColourId, juce::Colours::white);
    
    saveButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    deleteButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    prevButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    nextButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    
    presetBox.onChange = [this]
    {
        const int index = presetBox.getSelectedId() - 1;
        if (index >= 0 && onPresetChanged)
            onPresetChanged (index);
    };
    
    saveButton.onClick = [this]
    {
        juce::AlertWindow::showMessageBoxAsync (
            juce::AlertWindow::NoIcon,
            "Save Preset",
            "Enter preset name:",
            juce::String(),
            nullptr,
            [this] (int buttonId, const juce::String& text)
            {
                if (buttonId == 1 && text.isNotEmpty() && onPresetSaved)
                    onPresetSaved (text);
            }
        );
    };
}

PresetSelector::~PresetSelector()
{
}

void PresetSelector::paint (juce::Graphics& g)
{
    g.setRenderQuality (juce::Graphics::highRenderingQuality);
    
    // Dark background
    g.setColour (juce::Colour::fromRGB (25, 25, 30));
    g.fillRect (getLocalBounds());
    
    // Subtle border
    g.setColour (juce::Colour::fromRGB (60, 60, 70));
    g.drawRect (getLocalBounds(), 1);
}

void PresetSelector::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);
    const int buttonWidth = 50;
    const int height = 28;
    
    prevButton.setBounds (bounds.removeFromLeft (buttonWidth).withHeight (height));
    bounds.removeFromLeft (4);
    
    presetBox.setBounds (bounds.removeFromLeft (bounds.getWidth() - buttonWidth * 3 - 12).withHeight (height));
    bounds.removeFromLeft (4);
    
    nextButton.setBounds (bounds.removeFromLeft (buttonWidth).withHeight (height));
    bounds.removeFromLeft (4);
    
    saveButton.setBounds (bounds.removeFromLeft (buttonWidth).withHeight (height));
    bounds.removeFromLeft (4);
    
    deleteButton.setBounds (bounds.removeFromLeft (buttonWidth).withHeight (height));
}

void PresetSelector::loadPresets()
{
    presetBox.clear();
    const auto& presets = processor.getPresetManager().getPresetList();
    
    for (int i = 0; i < presets.size(); ++i)
    {
        presetBox.addItem (presets[i].name, i + 1);
    }
    
    presetBox.setSelectedId (processor.getPresetManager().getCurrentPresetIndex() + 1);
}

void PresetSelector::selectPreset (int index)
{
    presetBox.setSelectedId (index + 1);
}

void PresetSelector::saveCurrentPreset (const juce::String& name)
{
    processor.getPresetManager().savePreset (name, "User");
    loadPresets();
}

void PresetSelector::deleteCurrentPreset()
{
    const int index = presetBox.getSelectedId() - 1;
    if (processor.getPresetManager().deletePreset (index))
    {
        loadPresets();
        if (onPresetDeleted)
            onPresetDeleted();
    }
}

} // namespace fleen
