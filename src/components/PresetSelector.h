#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace fleen
{

class PluginProcessor;

/**
 * @brief Premium preset selector with A/B/C/D switching
 */
class PresetSelector : public juce::Component
{
public:
    explicit PresetSelector (PluginProcessor&);
    ~PresetSelector() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadPresets();
    void selectPreset (int index);
    void saveCurrentPreset (const juce::String& name);
    void deleteCurrentPreset();
    
    std::function<void(int)> onPresetChanged;
    std::function<void(const juce::String&)> onPresetSaved;
    std::function<void()> onPresetDeleted;
    
private:
    PluginProcessor& processor;
    juce::ComboBox presetBox;
    juce::TextButton saveButton;
    juce::TextButton deleteButton;
    juce::TextButton prevButton;
    juce::TextButton nextButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetSelector)
};

} // namespace fleen
