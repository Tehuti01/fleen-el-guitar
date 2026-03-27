#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

namespace fleen
{

/**
 * @brief Simple state manager using APVTS
 */
class StateManager
{
public:
    StateManager() = default;
    ~StateManager() = default;

    void resetToDefaults() {}

    juce::ValueTree saveState() const
    {
        return juce::ValueTree ("FleenElGuitarState");
    }

    void loadState (const juce::ValueTree&) {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateManager)
};

} // namespace fleen
