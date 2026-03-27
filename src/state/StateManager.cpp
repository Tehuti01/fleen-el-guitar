#include "StateManager.h"

namespace fleen
{

// ============================================================================
// State Management
// ============================================================================

void StateManager::setParameter (const juce::String& parameterId, float newValue)
{
    const juce::ScopedLock lock (stateLock);
    
    if (parameters.find (parameterId) != parameters.end())
    {
        parameters[parameterId].store (newValue);
    }
    else
    {
        parameters[parameterId].store (newValue);
        defaultValues[parameterId] = newValue;
    }
}

float StateManager::getParameter (const juce::String& parameterId) const
{
    const juce::ScopedLock lock (stateLock);
    
    auto it = parameters.find (parameterId);
    if (it != parameters.end())
        return it->second.load();
    
    return 0.0f;
}

std::map<juce::String, float> StateManager::getAllParameters() const
{
    const juce::ScopedLock lock (stateLock);
    
    std::map<juce::String, float> result;
    for (const auto& [id, value] : parameters)
    {
        result[id] = value.load();
    }
    return result;
}

void StateManager::resetToDefaults()
{
    const juce::ScopedLock lock (stateLock);
    
    for (auto& [id, value] : parameters)
    {
        auto defaultIt = defaultValues.find (id);
        if (defaultIt != defaultValues.end())
        {
            value.store (defaultIt->second);
        }
    }
}

// ============================================================================
// State Serialization
// ============================================================================

juce::ValueTree StateManager::saveState() const
{
    const juce::ScopedLock lock (stateLock);
    
    juce::ValueTree state ("FleenElGuitarState");
    
    for (const auto& [id, value] : parameters)
    {
        state.setProperty (id, value.load(), nullptr);
    }
    
    return state;
}

void StateManager::loadState (const juce::ValueTree& state)
{
    if (state.getType().toString() != "FleenElGuitarState")
        return;
    
    const juce::ScopedLock lock (stateLock);
    
    for (auto propertyName : state.getPropertyNames())
    {
        const juce::String id = propertyName.toString();
        if (parameters.find (id) != parameters.end())
        {
            parameters[id].store (state.getProperty (id, 0.0f));
        }
    }
}

juce::String StateManager::saveToJson() const
{
    const juce::ScopedLock lock (stateLock);
    
    juce::DynamicObject::Ptr json = new juce::DynamicObject();
    
    for (const auto& [id, value] : parameters)
    {
        json->setProperty (id, static_cast<double> (value.load()));
    }
    
    return juce::JSON::toString (json.get());
}

void StateManager::loadFromJson (const juce::String& json)
{
    const juce::ScopedLock lock (stateLock);
    
    auto value = juce::JSON::parse (json);
    if (auto* object = value.getDynamicObject())
    {
        for (auto& prop : object->getProperties())
        {
            const juce::String id = prop.name.toString();
            if (parameters.find (id) != parameters.end())
            {
                parameters[id].store (static_cast<float> (prop.value.toDouble()));
            }
        }
    }
}

// ============================================================================
// Undo/Redo
// ============================================================================

void StateManager::saveUndoState()
{
    const juce::ScopedLock lock (stateLock);
    
    // Save current state to undo stack
    undoStack.add (saveState());
    
    // Limit stack size
    while (undoStack.size() > maxUndoStates)
        undoStack.remove (0);
    
    // Clear redo stack when new action is performed
    redoStack.clear();
}

bool StateManager::undo()
{
    const juce::ScopedLock lock (stateLock);
    
    if (undoStack.isEmpty())
        return false;
    
    // Save current state to redo stack
    redoStack.add (saveState());
    
    // Load previous state
    loadState (undoStack.removeAndReturn (undoStack.size() - 1));
    
    return true;
}

bool StateManager::redo()
{
    const juce::ScopedLock lock (stateLock);
    
    if (redoStack.isEmpty())
        return false;
    
    // Save current state to undo stack
    undoStack.add (saveState());
    
    // Load next state
    loadState (redoStack.removeAndReturn (redoStack.size() - 1));
    
    return true;
}

void StateManager::clearHistory()
{
    const juce::ScopedLock lock (stateLock);
    
    undoStack.clear();
    redoStack.clear();
}

} // namespace fleen
