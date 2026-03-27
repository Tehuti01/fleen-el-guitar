#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace fleen
{

/**
 * @brief LED indicator component with emissive glow
 * 
 * Hardware-style LED with realistic light falloff and color options.
 */
class LedIndicator : public juce::Component
{
public:
    // ========================================================================
    // Enums
    // ========================================================================
    
    enum class Color
    {
        red,
        blue,
        green,
        amber
    };

    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    explicit LedIndicator (Color ledColor = Color::red);
    ~LedIndicator() override;

    // ========================================================================
    // Component Interface
    // ========================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // ========================================================================
    // State
    // ========================================================================
    
    /** @brief Set LED on/off state */
    void setState (bool isOn);
    
    /** @brief Get current state */
    bool getState() const { return isOn; }
    
    /** @brief Toggle state */
    void toggle();
    
    /** @brief Set brightness (0.0 - 1.0) */
    void setBrightness (float newBrightness);

private:
    // ========================================================================
    // Rendering
    // ========================================================================
    
    /** @brief Draw LED body */
    void drawLedBody (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw glow effect */
    void drawGlow (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Get LED color */
    juce::Colour getLedColour() const;

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    Color ledColor;
    bool isOn { false };
    float brightness { 1.0f };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LedIndicator)
};

} // namespace fleen
