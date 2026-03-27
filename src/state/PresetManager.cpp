#include "PresetManager.h"
#include "../PluginProcessor.h"

namespace fleen
{

PresetManager::PresetManager (PluginProcessor& proc)
    : processor (proc)
{
}

void PresetManager::loadFactoryPresets()
{
    presets.clear();
    
    // 1. Default - Clean starting point
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.3f, nullptr);
        state.setProperty ("bass", 0.5f, nullptr);
        state.setProperty ("mid", 0.5f, nullptr);
        state.setProperty ("treble", 0.5f, nullptr);
        state.setProperty ("presence", 0.5f, nullptr);
        state.setProperty ("reverb", 0.3f, nullptr);
        state.setProperty ("compression", 0.4f, nullptr);
        state.setProperty ("delay", 0.2f, nullptr);
        state.setProperty ("chorus", 0.2f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("1. Default", "Factory", state, true));
    }
    
    // 2. Crystal Clean
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.6f, nullptr);
        state.setProperty ("drive", 0.1f, nullptr);
        state.setProperty ("bass", 0.5f, nullptr);
        state.setProperty ("mid", 0.6f, nullptr);
        state.setProperty ("treble", 0.6f, nullptr);
        state.setProperty ("presence", 0.5f, nullptr);
        state.setProperty ("reverb", 0.2f, nullptr);
        state.setProperty ("compression", 0.3f, nullptr);
        state.setProperty ("delay", 0.1f, nullptr);
        state.setProperty ("chorus", 0.3f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("2. Crystal Clean", "Factory", state, true));
    }
    
    // 3. Warm Jazz
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.55f, nullptr);
        state.setProperty ("drive", 0.15f, nullptr);
        state.setProperty ("bass", 0.65f, nullptr);
        state.setProperty ("mid", 0.45f, nullptr);
        state.setProperty ("treble", 0.4f, nullptr);
        state.setProperty ("presence", 0.4f, nullptr);
        state.setProperty ("reverb", 0.35f, nullptr);
        state.setProperty ("compression", 0.4f, nullptr);
        state.setProperty ("delay", 0.15f, nullptr);
        state.setProperty ("chorus", 0.2f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("3. Warm Jazz", "Factory", state, true));
    }
    
    // 4. Classic Crunch
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.55f, nullptr);
        state.setProperty ("bass", 0.6f, nullptr);
        state.setProperty ("mid", 0.7f, nullptr);
        state.setProperty ("treble", 0.5f, nullptr);
        state.setProperty ("presence", 0.6f, nullptr);
        state.setProperty ("reverb", 0.2f, nullptr);
        state.setProperty ("compression", 0.5f, nullptr);
        state.setProperty ("delay", 0.1f, nullptr);
        state.setProperty ("chorus", 0.1f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("4. Classic Crunch", "Factory", state, true));
    }
    
    // 5. Blues Breaker
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.55f, nullptr);
        state.setProperty ("drive", 0.45f, nullptr);
        state.setProperty ("bass", 0.55f, nullptr);
        state.setProperty ("mid", 0.65f, nullptr);
        state.setProperty ("treble", 0.55f, nullptr);
        state.setProperty ("presence", 0.55f, nullptr);
        state.setProperty ("reverb", 0.25f, nullptr);
        state.setProperty ("compression", 0.45f, nullptr);
        state.setProperty ("delay", 0.2f, nullptr);
        state.setProperty ("chorus", 0.15f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("5. Blues Breaker", "Factory", state, true));
    }
    
    // 6. High Gain Metal
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.4f, nullptr);
        state.setProperty ("drive", 0.85f, nullptr);
        state.setProperty ("bass", 0.75f, nullptr);
        state.setProperty ("mid", 0.5f, nullptr);
        state.setProperty ("treble", 0.45f, nullptr);
        state.setProperty ("presence", 0.55f, nullptr);
        state.setProperty ("reverb", 0.3f, nullptr);
        state.setProperty ("compression", 0.65f, nullptr);
        state.setProperty ("delay", 0.15f, nullptr);
        state.setProperty ("chorus", 0.1f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("6. High Gain Metal", "Factory", state, true));
    }
    
    // 7. Singing Lead
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.7f, nullptr);
        state.setProperty ("bass", 0.5f, nullptr);
        state.setProperty ("mid", 0.8f, nullptr);
        state.setProperty ("treble", 0.65f, nullptr);
        state.setProperty ("presence", 0.75f, nullptr);
        state.setProperty ("reverb", 0.45f, nullptr);
        state.setProperty ("compression", 0.55f, nullptr);
        state.setProperty ("delay", 0.35f, nullptr);
        state.setProperty ("chorus", 0.25f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("7. Singing Lead", "Factory", state, true));
    }
    
    // 8. Ambient Shimmer
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.25f, nullptr);
        state.setProperty ("bass", 0.45f, nullptr);
        state.setProperty ("mid", 0.4f, nullptr);
        state.setProperty ("treble", 0.6f, nullptr);
        state.setProperty ("presence", 0.65f, nullptr);
        state.setProperty ("reverb", 0.7f, nullptr);
        state.setProperty ("compression", 0.5f, nullptr);
        state.setProperty ("delay", 0.55f, nullptr);
        state.setProperty ("chorus", 0.6f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("8. Ambient Shimmer", "Factory", state, true));
    }
    
    // 9. Funk Rhythm
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.55f, nullptr);
        state.setProperty ("drive", 0.2f, nullptr);
        state.setProperty ("bass", 0.45f, nullptr);
        state.setProperty ("mid", 0.7f, nullptr);
        state.setProperty ("treble", 0.7f, nullptr);
        state.setProperty ("presence", 0.6f, nullptr);
        state.setProperty ("reverb", 0.15f, nullptr);
        state.setProperty ("compression", 0.6f, nullptr);
        state.setProperty ("delay", 0.1f, nullptr);
        state.setProperty ("chorus", 0.4f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("9. Funk Rhythm", "Factory", state, true));
    }
    
    // 10. Modern Prog
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.45f, nullptr);
        state.setProperty ("drive", 0.65f, nullptr);
        state.setProperty ("bass", 0.6f, nullptr);
        state.setProperty ("mid", 0.55f, nullptr);
        state.setProperty ("treble", 0.6f, nullptr);
        state.setProperty ("presence", 0.65f, nullptr);
        state.setProperty ("reverb", 0.35f, nullptr);
        state.setProperty ("compression", 0.55f, nullptr);
        state.setProperty ("delay", 0.25f, nullptr);
        state.setProperty ("chorus", 0.3f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        presets.add (Preset ("10. Modern Prog", "Factory", state, true));
    }
    
    currentPresetIndex = 0;
    isModified = false;
}

void PresetManager::loadUserPresets()
{
    const auto presetDir = getPresetDirectory();
    
    if (!presetDir.exists())
        presetDir.createDirectory();
}

juce::String PresetManager::getCurrentPresetName() const
{
    if (currentPresetIndex >= 0 && currentPresetIndex < presets.size())
        return presets[currentPresetIndex].name;
    
    return "Untitled";
}

void PresetManager::loadPreset (int index)
{
    if (index < 0 || index >= presets.size())
        return;
    
    const auto& preset = presets[index];
    auto& parameters = processor.getAPVTS();
    
    for (int i = 0; i < preset.state.getNumProperties(); ++i)
    {
        auto propName = preset.state.getPropertyName (i).toString();
        if (auto* param = parameters.getParameter (propName))
        {
            const float value = static_cast<float> (preset.state.getProperty (propName, 0.0f));
            param->setValueNotifyingHost (value);
        }
    }
    
    currentPresetIndex = index;
    isModified = false;
}

void PresetManager::loadPreset (const juce::String& name)
{
    for (int i = 0; i < presets.size(); ++i)
    {
        if (presets[i].name == name)
        {
            loadPreset (i);
            return;
        }
    }
}

bool PresetManager::savePreset (const juce::String& name, const juce::String& category)
{
    juce::ValueTree state ("Preset");
    auto& apvts = processor.getAPVTS();
    
    for (int i = 0; i < apvts.state.getNumProperties(); ++i)
    {
        auto propName = apvts.state.getPropertyName (i).toString();
        state.setProperty (propName, apvts.state.getProperty (propName), nullptr);
    }
    
    Preset newPreset (name, category, state, false);
    presets.add (newPreset);
    
    const auto presetDir = getPresetDirectory();
    currentPresetIndex = presets.size() - 1;
    isModified = false;
    
    return true;
}

bool PresetManager::deletePreset (int index)
{
    if (index < 0 || index >= presets.size())
        return false;
    
    if (presets[index].isFactoryPreset)
        return false;
    
    presets.remove (index);
    
    if (currentPresetIndex >= presets.size())
        currentPresetIndex = juce::jmax (0, presets.size() - 1);
    
    return true;
}

juce::File PresetManager::getPresetDirectory()
{
    const auto appDataDir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory);
    return appDataDir.getChildFile ("Fleen")
                     .getChildFile ("El-Guitar")
                     .getChildFile ("Presets");
}

} // namespace fleen
