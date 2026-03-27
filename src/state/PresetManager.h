#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace fleen
{

// Forward declaration
class PluginProcessor;

/**
 * @brief Preset management system with JSON serialization
 */
class PresetManager
{
public:
    struct Preset
    {
        juce::String name;
        juce::String category;
        juce::ValueTree state;
        bool isFactoryPreset { true };
        
        Preset() = default;
        Preset (const juce::String& n, const juce::String& cat, const juce::ValueTree& s, bool factory = true)
            : name (n), category (cat), state (s), isFactoryPreset (factory) {}
    };

    explicit PresetManager (PluginProcessor& processor);
    ~PresetManager() = default;

    void loadFactoryPresets();
    void loadUserPresets();
    
    const juce::Array<Preset>& getPresetList() const { return presets; }
    int getCurrentPresetIndex() const { return currentPresetIndex; }
    juce::String getCurrentPresetName() const;
    
    void loadPreset (int index);
    void loadPreset (const juce::String& name);
    bool savePreset (const juce::String& name, const juce::String& category = "User");
    
    bool deletePreset (int index);
    bool isPresetModified() const { return isModified; }
    void resetModifiedFlag() { isModified = false; }
    
    static juce::File getPresetDirectory();

private:
    PluginProcessor& processor;
    juce::Array<Preset> presets;
    int currentPresetIndex { 0 };
    bool isModified { false };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManager)
};

} // namespace fleen
