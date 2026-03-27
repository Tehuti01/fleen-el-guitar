#pragma once

#include <JuceHeader.h>
#include "utils/GoldenRatio.h"

namespace fleen
{

/**
 * @brief Skeuomorphic rotary knob component
 * 
 * Hardware-inspired knob with photorealistic rendering,
 * smooth animations, and precise control.
 */
class SkeuomorphicKnob : public juce::Component
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    explicit SkeuomorphicKnob (const juce::String& name);
    ~SkeuomorphicKnob() override;

    // ========================================================================
    // Component Interface
    // ========================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;
    void mouseEnter (const juce::MouseEvent&) override;
    void mouseExit (const juce::MouseEvent&) override;

    // ========================================================================
    // Value Interface
    // ========================================================================
    
    /** @brief Set current value (0.0 - 1.0) */
    void setValue (float newValue, juce::NotificationType notification);
    
    /** @brief Get current value */
    float getValue() const { return value; }
    
    /** @brief Set value range */
    void setRange (float newMin, float newMax);
    
    /** @brief Get minimum value */
    float getMinValue() const { return minValue; }
    
    /** @brief Get maximum value */
    float getMaxValue() const { return maxValue; }

    // ========================================================================
    // Slider Access
    // ========================================================================
    
    /** @brief Get internal slider for APVTS attachment */
    juce::Slider& getSlider() { return slider; }
    const juce::Slider& getSlider() const { return slider; }
    
    /** @brief Set label text */
    void setLabelText (const juce::String& newText);

private:
    // ========================================================================
    // Rendering
    // ========================================================================
    
    /** @brief Draw metallic knob body */
    void drawKnobBody (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw indicator line */
    void drawIndicator (juce::Graphics& g, const juce::Rectangle<float>& bounds, float angle);
    
    /** @brief Draw value label */
    void drawLabel (juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    /** @brief Draw highlight ring */
    void drawHighlightRing (juce::Graphics& g, const juce::Rectangle<float>& bounds);

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    juce::Slider slider;
    juce::String labelText;
    
    float value { 0.5f };
    float minValue { 0.0f };
    float maxValue { 1.0f };
    
    // Interaction state
    bool isDragging { false };
    bool isHovered { false };
    float lastMouseY { 0.0f };
    
    // Animation
    float highlightAlpha { 0.0f };
    
    // Colours
    juce::Colour knobBase { 60, 60, 70 };
    juce::Colour knobHighlight { 100, 100, 110 };
    juce::Colour indicatorColour { 255, 200, 50 };
    juce::Colour ringColour { 0, 150, 200 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SkeuomorphicKnob)
};

} // namespace fleen
