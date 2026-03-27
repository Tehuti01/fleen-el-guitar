#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace fleen
{

/**
 * @brief VU meter display component
 * 
 * Real-time level meter with ballistic response and gradient coloring.
 */
class MeterDisplay : public juce::Component,
                     public juce::Timer
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    MeterDisplay();
    ~MeterDisplay() override;

    // ========================================================================
    // Timer Interface
    // ========================================================================
    
    void timerCallback() override;

    // ========================================================================
    // Component Interface
    // ========================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // ========================================================================
    // Level Control
    // ========================================================================
    
    /** @brief Set current level (0.0 - 1.0) */
    void setLevel (float newLevel);
    
    /** @brief Get current level */
    float getLevel() const { return currentLevel; }
    
    /** @brief Set meter sensitivity */
    void setSensitivity (float newSensitivity);

private:
    // ========================================================================
    // Rendering
    // ========================================================================
    
    /** @brief Draw meter background */
    void drawBackground (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw meter level bar */
    void drawLevelBar (juce::Graphics& g, const juce::Rectangle<float>& bounds, float level);
    
    /** @brief Draw scale markings */
    void drawScale (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Get color for level (green -> yellow -> red) */
    juce::Colour getLevelColour (float level) const;

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    float currentLevel { 0.0f };
    float targetLevel { 0.0f };
    float sensitivity { 1.0f };
    
    // Ballistic response
    float attackTime { 0.01f };
    float releaseTime { 0.3f };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterDisplay)
};

} // namespace fleen
