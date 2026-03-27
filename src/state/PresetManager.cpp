#include "PresetManager.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

PresetManager::PresetManager (PluginProcessor& proc)
    : processor (proc)
{
}

// ============================================================================
// Preset Management
// ============================================================================

void PresetManager::loadFactoryPresets()
{
    presets.clear();
    
    // Load factory presets from binary data
    // These would be embedded in the binary during build
    
    // Default preset
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.3f, nullptr);
        state.setProperty ("bass", 0.6f, nullptr);
        state.setProperty ("mid", 0.5f, nullptr);
        state.setProperty ("treble", 0.5f, nullptr);
        state.setProperty ("presence", 0.5f, nullptr);
        state.setProperty ("reverb", 0.3f, nullptr);
        state.setProperty ("compression", 0.4f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        
        presets.add (Preset ("Default", "Factory", state, true));
    }
    
    // Clean preset
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
        state.setProperty ("bypass", false, nullptr);
        
        presets.add (Preset ("Clean", "Factory", state, true));
    }
    
    // Crunch preset
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.5f, nullptr);
        state.setProperty ("bass", 0.6f, nullptr);
        state.setProperty ("mid", 0.7f, nullptr);
        state.setProperty ("treble", 0.5f, nullptr);
        state.setProperty ("presence", 0.6f, nullptr);
        state.setProperty ("reverb", 0.2f, nullptr);
        state.setProperty ("compression", 0.5f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        
        presets.add (Preset ("Crunch", "Factory", state, true));
    }
    
    // High Gain preset
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.4f, nullptr);
        state.setProperty ("drive", 0.8f, nullptr);
        state.setProperty ("bass", 0.7f, nullptr);
        state.setProperty ("mid", 0.6f, nullptr);
        state.setProperty ("treble", 0.4f, nullptr);
        state.setProperty ("presence", 0.5f, nullptr);
        state.setProperty ("reverb", 0.3f, nullptr);
        state.setProperty ("compression", 0.6f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        
        presets.add (Preset ("High Gain", "Factory", state, true));
    }
    
    // Lead preset
    {
        juce::ValueTree state ("Preset");
        state.setProperty ("gain", 0.5f, nullptr);
        state.setProperty ("drive", 0.7f, nullptr);
        state.setProperty ("bass", 0.5f, nullptr);
        state.setProperty ("mid", 0.8f, nullptr);
        state.setProperty ("treble", 0.6f, nullptr);
        state.setProperty ("presence", 0.7f, nullptr);
        state.setProperty ("reverb", 0.4f, nullptr);
        state.setProperty ("compression", 0.5f, nullptr);
        state.setProperty ("bypass", false, nullptr);
        
        presets.add (Preset ("Lead", "Factory", state, true));
    }
    
    currentPresetIndex = 0;
    isModified = false;
}

void PresetManager::loadUserPresets()
{
    const auto presetDir = getPresetDirectory();
    
    if (!presetDir.exists())
        presetDir.createDirectory();
    
    // Find all JSON preset files
    juce::Array<juce::File> presetFiles;
    presetDir.findChildFiles (presetFiles, juce::File::findFiles, false, "*.json");
    
    for (const auto& file : presetFiles)
    {
        if (auto preset = loadPresetFromJson (file))
        {
            preset->isFactoryPreset = false;
            presets.add (*preset);
        }
    }
    
    sortPresets();
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
    
    // Apply preset state to processor
    auto& parameters = processor.getParameters();
    
    for (auto paramName : preset.state.getPropertyNames())
    {
        if (auto* param = parameters.getParameter (paramName.toString()))
        {
            const float value = static_cast<float> (preset.state.getProperty (paramName, 0.0f));
            param->setValueNotifyingHost (value);
        }
    }
    
    currentPresetIndex = index;
    isModified = false;
    
    notifyPresetChanged();
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
    // Create preset from current state
    juce::ValueTree state ("Preset");
    
    auto& parameters = processor.getParameters();
    for (auto* param : parameters.getParameters())
    {
        state.setProperty (param->paramID, param->getValue(), nullptr);
    }
    
    Preset newPreset (name, category, state, false);
    
    // Save to file
    const auto presetDir = getPresetDirectory();
    const auto file = presetDir.getChildFile (name + ".json");
    
    if (!savePresetToJson (newPreset, file))
        return false;
    
    // Add to preset list
    presets.add (newPreset);
    sortPresets();
    
    // Find and load the new preset
    for (int i = 0; i < presets.size(); ++i)
    {
        if (presets[i].name == name)
        {
            currentPresetIndex = i;
            break;
        }
    }
    
    isModified = false;
    notifyPresetChanged();
    
    return true;
}

// ============================================================================
// Preset Operations
// ============================================================================

bool PresetManager::deletePreset (int index)
{
    if (index < 0 || index >= presets.size())
        return false;
    
    if (presets[index].isFactoryPreset)
        return false; // Can't delete factory presets
    
    // Delete file
    const auto presetDir = getPresetDirectory();
    const auto file = presetDir.getChildFile (presets[index].name + ".json");
    if (file.exists())
        file.deleteFile();
    
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

// ============================================================================
// Serialization
// ============================================================================

bool PresetManager::exportPresetToFile (int index, const juce::File& file)
{
    if (index < 0 || index >= presets.size())
        return false;
    
    return savePresetToJson (presets[index], file);
}

bool PresetManager::importPresetFromFile (const juce::File& file)
{
    if (auto preset = loadPresetFromJson (file))
    {
        preset->isFactoryPreset = false;
        presets.add (*preset);
        sortPresets();
        return true;
    }
    
    return false;
}

bool PresetManager::exportAllPresets (const juce::File& file)
{
    juce::DynamicObject::Ptr json = new juce::DynamicObject();
    juce::var presetArray;
    
    for (const auto& preset : presets)
    {
        juce::DynamicObject::Ptr presetObj = new juce::DynamicObject();
        presetObj->setProperty ("name", preset.name);
        presetObj->setProperty ("category", preset.category);
        presetObj->setProperty ("isFactory", preset.isFactoryPreset);
        
        // Convert ValueTree to JSON
        juce::DynamicObject::Ptr stateObj = new juce::DynamicObject();
        for (auto propName : preset.state.getPropertyNames())
        {
            stateObj->setProperty (propName.toString(),
                                   preset.state.getProperty (propName));
        }
        
        presetObj->setProperty ("state", stateObj);
        presetArray.getArray()->add (presetObj);
    }
    
    json->setProperty ("presets", presetArray);
    
    auto outputStream = file.createOutputStream();
    if (outputStream == nullptr)
        return false;
    
    *outputStream << juce::JSON::toString (json.get());
    return true;
}

bool PresetManager::importPresets (const juce::File& file)
{
    auto jsonString = file.loadFileAsString();
    auto value = juce::JSON::parse (jsonString);
    
    if (auto* object = value.getDynamicObject())
    {
        if (auto* presetArray = object->getProperty ("presets").getArray())
        {
            for (auto& presetValue : *presetArray)
            {
                if (auto* presetObj = presetValue.getDynamicObject())
                {
                    juce::String name = presetObj->getProperty ("name").toString();
                    juce::String category = presetObj->getProperty ("category").toString();
                    bool isFactory = presetObj->getProperty ("isFactory", false);
                    
                    juce::ValueTree state ("Preset");
                    if (auto* stateObj = presetObj->getProperty ("state").getDynamicObject())
                    {
                        for (auto& prop : stateObj->getProperties())
                        {
                            state.setProperty (prop.name.toString(),
                                              prop.value.toDouble(), nullptr);
                        }
                    }
                    
                    presets.add (Preset (name, category, state, isFactory));
                }
            }
            
            sortPresets();
            return true;
        }
    }
    
    return false;
}

// ============================================================================
// Internal Methods
// ============================================================================

std::optional<PresetManager::Preset> PresetManager::loadPresetFromJson (const juce::File& file)
{
    auto jsonString = file.loadFileAsString();
    auto value = juce::JSON::parse (jsonString);
    
    if (auto* object = value.getDynamicObject())
    {
        Preset preset;
        preset.name = object->getProperty ("name").toString();
        preset.category = object->getProperty ("category", "User").toString();
        preset.isFactoryPreset = object->getProperty ("isFactory", false);
        
        juce::ValueTree state ("Preset");
        if (auto* stateObj = object->getProperty ("state").getDynamicObject())
        {
            for (auto& prop : stateObj->getProperties())
            {
                state.setProperty (prop.name.toString(),
                                  prop.value.toDouble(), nullptr);
            }
        }
        
        preset.state = state;
        return preset;
    }
    
    return std::nullopt;
}

bool PresetManager::savePresetToJson (const Preset& preset, const juce::File& file)
{
    juce::DynamicObject::Ptr json = new juce::DynamicObject();
    json->setProperty ("name", preset.name);
    json->setProperty ("category", preset.category);
    json->setProperty ("isFactory", preset.isFactoryPreset);
    
    juce::DynamicObject::Ptr stateObj = new juce::DynamicObject();
    for (auto propName : preset.state.getPropertyNames())
    {
        stateObj->setProperty (propName.toString(),
                               preset.state.getProperty (propName));
    }
    
    json->setProperty ("state", stateObj);
    
    auto outputStream = file.createOutputStream();
    if (outputStream == nullptr)
        return false;
    
    *outputStream << juce::JSON::toString (json.get());
    return true;
}

void PresetManager::sortPresets()
{
    // Sort: Factory presets first, then user presets, alphabetically within each category
    std::sort (presets.begin(), presets.end(),
        [] (const Preset& a, const Preset& b)
        {
            if (a.isFactoryPreset != b.isFactoryPreset)
                return a.isFactoryPreset;
            
            if (a.category != b.category)
                return a.category < b.category;
            
            return a.name < b.name;
        });
}

void PresetManager::notifyPresetChanged()
{
    // Could add listener pattern here for UI updates
    // For now, the UI polls the preset manager
}

} // namespace fleen
