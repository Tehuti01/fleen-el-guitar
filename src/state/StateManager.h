#pragma once

#include <JuceHeader.h>

namespace fleen
{

/**
 * @brief Thread-safe state manager for plugin parameters
 * 
 * Provides atomic parameter access and state synchronization
 * between audio and GUI threads.
 */
class StateManager
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    StateManager() = default;
    ~StateManager() = default;

    // ========================================================================
    // State Management
    // ========================================================================
    
    /**
     * @brief Update parameter value (thread-safe)
     * @param parameterId Parameter identifier
     * @param newValue New parameter value
     */
    void setParameter (const juce::String& parameterId, float newValue);
    
    /**
     * @brief Get parameter value (thread-safe)
     * @param parameterId Parameter identifier
     * @return Current parameter value
     */
    float getParameter (const juce::String& parameterId) const;
    
    /**
     * @brief Get all parameter values
     * @return Map of parameter IDs to values
     */
    std::map<juce::String, float> getAllParameters() const;
    
    /**
     * @brief Reset all parameters to defaults
     */
    void resetToDefaults();

    // ========================================================================
    // State Serialization
    // ========================================================================
    
    /**
     * @brief Serialize state to ValueTree
     * @return ValueTree containing current state
     */
    juce::ValueTree saveState() const;
    
    /**
     * @brief Load state from ValueTree
     * @param state State to load
     */
    void loadState (const juce::ValueTree& state);
    
    /**
     * @brief Serialize state to JSON
     * @return JSON string
     */
    juce::String saveToJson() const;
    
    /**
     * @brief Load state from JSON
     * @param json JSON string
     */
    void loadFromJson (const juce::String& json);

    // ========================================================================
    // Undo/Redo
    // ========================================================================
    
    /** @brief Save current state for undo */
    void saveUndoState();
    
    /** @brief Undo last change */
    bool undo();
    
    /** @brief Redo last undone change */
    bool redo();
    
    /** @brief Clear undo/redo history */
    void clearHistory();

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    // Current parameter values
    std::map<juce::String, std::atomic<float>> parameters;
    
    // Default values
    std::map<juce::String, float> defaultValues;
    
    // Undo/redo stacks
    juce::Array<juce::ValueTree> undoStack;
    juce::Array<juce::ValueTree> redoStack;
    
    static constexpr int maxUndoStates = 32;
    
    // Critical section for thread safety
    mutable juce::SpinLock stateLock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateManager)
};

} // namespace fleen
