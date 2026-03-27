#pragma once

#include <JuceHeader.h>
#include "utils/GoldenRatio.h"

namespace fleen
{

/**
 * @brief Custom LookAndFeel for premium skeuomorphic aesthetic
 * 
 * Implements hardware-inspired visual style with:
 * - Photorealistic knob rendering with filmstrip support
 * - Neumorphic shadows using Melatonin Blur
 * - Golden Ratio-based typography and spacing
 * - LED indicators with emissive glow
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    // ========================================================================
    // Component Rendering
    // ========================================================================
    
    /** @brief Draw rotary slider (knob) with skeuomorphic styling */
    void drawRotarySlider (juce::Graphics&,
                          int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;
    
    /** @brief Draw button with neumorphic styling */
    void drawButtonBackground (juce::Graphics&, juce::Button&,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    /** @brief Draw combo box with premium styling */
    void drawComboBox (juce::Graphics&, int width, int height,
                      bool isButtonDown, int buttonX, int buttonY,
                      int buttonW, int buttonH,
                      juce::ComboBox&) override;
    
    /** @brief Draw popup menu with dark theme */
    void drawPopupMenuBackground (juce::Graphics&, juce::PopupMenu&) override;

    // ========================================================================
    // Typography
    // ========================================================================
    
    /** @brief Get typeface based on Golden Ratio hierarchy */
    juce::Typeface::Ptr getTypefaceForFont (const juce::Font&) override;
    
    /** @brief Draw text with Golden Ratio sizing */
    void drawText (juce::Graphics&, juce::String text,
                  juce::Rectangle<int> area, float fontSize,
                  juce::Colour colour, int justification);

    // ========================================================================
    // Effects
    // ========================================================================
    
    /**
     * @brief Draw neumorphic shadow effect
     * @param g Graphics context
     * @param bounds Component bounds
     * @param isRecessed True for inset shadow (recessed controls)
     * @param blurRadius Blur radius (higher = softer shadow)
     */
    void drawNeumorphicShadow (juce::Graphics& g,
                              const juce::Rectangle<int>& bounds,
                              bool isRecessed,
                              int blurRadius);
    
    /**
     * @brief Draw metallic gradient for knobs
     * @param g Graphics context
     * @param bounds Knob bounds
     * @param rotation Rotation angle for highlight
     */
    void drawMetallicGradient (juce::Graphics& g,
                              const juce::Rectangle<int>& bounds,
                              float rotation);
    
    /**
     * @brief Draw LED glow effect
     * @param g Graphics context
     * @param centreX Centre X position
     * @param centreY Centre Y position
     * @param radius LED radius
     * @param colour LED colour
     * @param intensity Glow intensity (0.0 - 1.0)
     */
    void drawLedGlow (juce::Graphics& g,
                     float centreX, float centreY,
                     float radius,
                     const juce::Colour& colour,
                     float intensity);

    // ========================================================================
    // Assets
    // ========================================================================
    
    /** @brief Load knob filmstrip image */
    void loadKnobFilmstrip (const juce::String& assetPath);
    
    /** @brief Set knob filmstrip for rendering */
    void setKnobFilmstrip (int filmstripIndex);

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    // Filmstrip images for skeuomorphic knobs
    juce::Image knobFilmstrip;
    int filmstripFrames { 64 };
    
    // Cached gradients for performance
    juce::ColourGradient metallicGradient;
    juce::ColourGradient darkGradient;
    
    // Shadow colours for neumorphism
    juce::Colour shadowLight { 255, 255, 255, 40 };
    juce::Colour shadowDark { 0, 0, 0, 80 };
    
    // Typography
    juce::Font baseFont { "Inter", 14.0f, juce::Font::plain };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};

} // namespace fleen
