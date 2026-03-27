#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

namespace fleen
{

// Forward declaration to avoid circular dependency
class PluginProcessor;

/**
 * @brief Preset management system with JSON serialization
 *
 * Handles loading, saving, and browsing of plugin presets.
 * Supports factory presets and user presets.
 */
class PresetManager
{
public:
    // ========================================================================
    // Structures
    // ========================================================================

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

    // ========================================================================
    // Construction / Destruction
    // ========================================================================

    explicit PresetManager (PluginProcessor& processor);
    ~PresetManager() = default;

    // ========================================================================
    // Preset Management
    // ========================================================================
    
    /** @brief Load factory presets from binary data */
    void loadFactoryPresets();
    
    /** @brief Load user presets from disk */
    void loadUserPresets();
    
    /** @brief Get list of all presets */
    const juce::Array<Preset>& getPresetList() const { return presets; }
    
    /** @brief Get current preset index */
    int getCurrentPresetIndex() const { return currentPresetIndex; }
    
    /** @brief Get current preset name */
    juce::String getCurrentPresetName() const;
    
    /** @brief Load preset by index */
    void loadPreset (int index);
    
    /** @brief Load preset by name */
    void loadPreset (const juce::String& name);
    
    /** @brief Save current state as new preset */
    bool savePreset (const juce::String& name, const juce::String& category = "User");

    // ========================================================================
    // Preset Operations
    // ========================================================================
    
    /** @brief Delete user preset */
    bool deletePreset (int index);
    
    /** @brief Check if current preset is modified */
    bool isPresetModified() const { return isModified; }
    
    /** @brief Reset modified flag */
    void resetModifiedFlag() { isModified = false; }
    
    /** @brief Get preset directory path */
    static juce::File getPresetDirectory();

    // ========================================================================
    // Serialization
    // ========================================================================
    
    /** @brief Export preset to file */
    bool exportPresetToFile (int index, const juce::File& file);
    
    /** @brief Import preset from file */
    bool importPresetFromFile (const juce::File& file);
    
    /** @brief Export all presets to JSON */
    bool exportAllPresets (const juce::File& file);
    
    /** @brief Import presets from JSON */
    bool importPresets (const juce::File& file);

private:
    // ========================================================================
    // Internal Methods
    // ========================================================================
    
    /** @brief Load preset from JSON file */
    std::optional<Preset> loadPresetFromJson (const juce::File& file);
    
    /** @brief Save preset to JSON file */
    bool savePresetToJson (const Preset& preset, const juce::File& file);
    
    /** @brief Sort presets by category and name */
    void sortPresets();
    
    /** @brief Notify listeners of preset change */
    void notifyPresetChanged();

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    PluginProcessor& processor;
    
    juce::Array<Preset> presets;
    int currentPresetIndex { 0 };
    bool isModified { false };
    
    // Preset categories
    static constexpr const char* factoryCategory = "Factory";
    static constexpr const char* userCategory = "User";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManager)
};

} // namespace fleen
