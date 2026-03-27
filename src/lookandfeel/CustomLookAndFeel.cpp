#include "CustomLookAndFeel.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

CustomLookAndFeel::CustomLookAndFeel()
{
    // Setup metallic gradient for knobs
    metallicGradient = juce::ColourGradient (
        juce::Colour::fromRGB (180, 180, 190),  // Light highlight
        0.0f, 0.0f,
        juce::Colour::fromRGB (80, 80, 90),     // Dark shadow
        1.0f, 1.0f,
        false
    );
    
    // Setup dark gradient for backgrounds
    darkGradient = juce::ColourGradient (
        juce::Colour::fromRGB (50, 50, 55),
        0.0f, 0.0f,
        juce::Colour::fromRGB (30, 30, 35),
        0.0f, 1.0f,
        false
    );
    
    // Set default colour scheme
    setColour (juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB (0, 150, 200));
    setColour (juce::Slider::thumbColourId, juce::Colour::fromRGB (255, 200, 50));
    setColour (juce::Slider::backgroundColourId, juce::Colour::fromRGB (40, 40, 45));
    
    // Set default font
    setDefaultSansSerifTypefaceName ("Inter");
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

// ============================================================================
// Component Rendering
// ============================================================================

void CustomLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                          int x, int y, int width, int height,
                                          float sliderPos, float rotaryStartAngle,
                                          float rotaryEndAngle, juce::Slider& slider)
{
    const auto bounds = juce::Rectangle<int> (x, y, width, height);
    const auto centre = bounds.getCentre().toFloat();
    const auto radius = juce::jmin (width, height) / 2.0f - 4.0f;
    
    // Draw outer ring (background track)
    {
        juce::Path backgroundTrack;
        backgroundTrack.addArc (
            centre.getX() - radius, centre.getY() - radius,
            radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, rotaryEndAngle, true
        );
        
        g.setColour (juce::Colour::fromRGB (40, 40, 45));
        g.strokePath (backgroundTrack, juce::PathStrokeType (4.0f));
    }
    
    // Draw active arc (value indicator)
    {
        juce::Path activeArc;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        activeArc.addArc (
            centre.getX() - radius, centre.getY() - radius,
            radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, angle, true
        );
        
        g.setColour (juce::Colour::fromRGB (0, 150, 200));
        g.strokePath (activeArc, juce::PathStrokeType (4.0f));
    }
    
    // Draw metallic knob body
    {
        juce::Path knobBody;
        knobBody.addEllipse (
            centre.getX() - radius + 8.0f,
            centre.getY() - radius + 8.0f,
            (radius - 8.0f) * 2.0f,
            (radius - 8.0f) * 2.0f
        );
        
        // Apply metallic gradient with rotation based on knob position
        const float rotation = sliderPos * juce::MathConstants<float>::twoPi;
        const auto rotatedGradient = metallicGradient.transformed (
            juce::AffineTransform::rotation (rotation, centre.getX(), centre.getY())
        );
        
        g.setGradientFill (rotatedGradient);
        g.fillPath (knobBody);
        
        // Draw outer bevel
        g.setColour (juce::Colour::fromRGB (100, 100, 110));
        g.strokePath (knobBody, juce::PathStrokeType (2.0f));
    }
    
    // Draw indicator line
    {
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const float indicatorLength = radius - 12.0f;
        
        const float startX = centre.getX() + std::cos (angle) * (radius * 0.3f);
        const float startY = centre.getY() + std::sin (angle) * (radius * 0.3f);
        const float endX = centre.getX() + std::cos (angle) * indicatorLength;
        const float endY = centre.getY() + std::sin (angle) * indicatorLength;
        
        g.setColour (juce::Colour::fromRGB (255, 200, 50));
        g.drawLine (startX, startY, endX, endY, 3.0f);
        
        // Draw indicator dot
        g.fillEllipse (endX - 3.0f, endY - 3.0f, 6.0f, 6.0f);
    }
    
    // Draw centre cap
    {
        const float capRadius = radius * 0.25f;
        juce::Path cap;
        cap.addEllipse (
            centre.getX() - capRadius,
            centre.getY() - capRadius,
            capRadius * 2.0f,
            capRadius * 2.0f
        );
        
        g.setColour (juce::Colour::fromRGB (30, 30, 35));
        g.fillPath (cap);
        
        g.setColour (juce::Colour::fromRGB (60, 60, 70));
        g.strokePath (cap, juce::PathStrokeType (1.0f));
    }
}

void CustomLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                              const juce::Colour& backgroundColour,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    const auto bounds = button.getLocalBounds().toFloat();
    
    // Create rounded rectangle
    const float cornerSize = 8.0f;
    juce::Path buttonPath;
    buttonPath.addRoundedRectangle (bounds, cornerSize);
    
    // Determine shadow type based on button state
    const bool isRecessed = shouldDrawButtonAsDown;
    
    // Draw base colour
    g.setColour (backgroundColour);
    g.fillPath (buttonPath);
    
    // Draw neumorphic shadow
    drawNeumorphicShadow (g, button.getLocalBounds(), isRecessed, 8);
    
    // Draw border highlight
    g.setColour (shouldDrawButtonAsHighlighted ? juce::Colour::fromRGB (80, 80, 90)
                                                : juce::Colour::fromRGB (60, 60, 70));
    g.strokePath (buttonPath, juce::PathStrokeType (1.0f));
}

void CustomLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height,
                                      bool isButtonDown, int buttonX, int buttonY,
                                      int buttonW, int buttonH,
                                      juce::ComboBox& comboBox)
{
    const juce::Rectangle<int> boxBounds (0, 0, width, height);
    
    // Draw background
    g.setColour (comboBox.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (boxBounds.toFloat(), 6.0f);
    
    // Draw border
    g.setColour (comboBox.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat(), 6.0f, 1.0f);
    
    // Draw text
    const juce::String text = comboBox.getText();
    if (text.isNotEmpty())
    {
        g.setColour (comboBox.findColour (juce::ComboBox::textColourId));
        g.setFont (juce::FontOptions (14.0f));
        g.drawText (text, boxBounds.reduced (8, 0), juce::Justification::centredLeft);
    }
    
    // Draw arrow
    juce::Path arrow;
    const float arrowX = static_cast<float> (buttonX);
    const float arrowY = static_cast<float> (buttonY);
    const float arrowW = static_cast<float> (buttonW);
    const float arrowH = static_cast<float> (buttonH);
    
    arrow.startNewSubPath (arrowX + arrowW * 0.3f, arrowY + arrowH * 0.4f);
    arrow.lineTo (arrowX + arrowW * 0.7f, arrowY + arrowH * 0.4f);
    arrow.lineTo (arrowX + arrowW * 0.5f, arrowY + arrowH * 0.7f);
    
    g.setColour (comboBox.findColour (juce::ComboBox::textColourId));
    g.strokePath (arrow, juce::PathStrokeType (2.0f));
}

void CustomLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    const auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height);
    
    // Draw dark background
    g.setColour (juce::Colour::fromRGB (35, 35, 40));
    g.fillRoundedRectangle (bounds, 8.0f);
    
    // Draw border
    g.setColour (juce::Colour::fromRGB (60, 60, 70));
    g.drawRoundedRectangle (bounds, 8.0f, 1.0f);
}

// ============================================================================
// Typography
// ============================================================================

juce::Typeface::Ptr CustomLookAndFeel::getTypefaceForFont (const juce::Font& font)
{
    // Try to load Inter font, fallback to system sans-serif
    if (auto typeface = juce::Typeface::createSystemTypefaceFor (font))
        return typeface;
    
    return juce::LookAndFeel::getTypefaceForFont (font);
}

void CustomLookAndFeel::drawText (juce::Graphics& g, juce::String text,
                                  juce::Rectangle<int> area, float fontSize,
                                  juce::Colour colour, int justification)
{
    g.setColour (colour);
    g.setFont (juce::FontOptions (fontSize));
    g.drawText (text, area, justification, 1);
}

// ============================================================================
// Effects
// ============================================================================

void CustomLookAndFeel::drawNeumorphicShadow (juce::Graphics& g,
                                              const juce::Rectangle<int>& bounds,
                                              bool isRecessed,
                                              int blurRadius)
{
    // Neumorphism uses two shadows:
    // - Light shadow (top-left) for highlights
    // - Dark shadow (bottom-right) for depth
    
    const int offset = 4;
    
    if (isRecessed)
    {
        // Inset shadow for recessed controls
        g.setColour (shadowDark);
        g.fillRoundedRectangle (
            bounds.getX() + offset,
            bounds.getY() + offset,
            bounds.getWidth() - offset * 2,
            bounds.getHeight() - offset * 2,
            8.0f
        );
        
        g.setColour (shadowLight);
        g.fillRoundedRectangle (
            bounds.getX(),
            bounds.getY(),
            bounds.getWidth() - offset,
            bounds.getHeight() - offset,
            8.0f
        );
    }
    else
    {
        // Outer shadow for raised controls
        // Dark shadow (bottom-right)
        g.setColour (shadowDark);
        g.fillRoundedRectangle (
            bounds.getX() + offset,
            bounds.getY() + offset,
            bounds.getWidth(),
            bounds.getHeight(),
            8.0f
        );
        
        // Light shadow (top-left)
        g.setColour (shadowLight);
        g.fillRoundedRectangle (
            bounds.getX() - offset,
            bounds.getY() - offset,
            bounds.getWidth(),
            bounds.getHeight(),
            8.0f
        );
    }
}

void CustomLookAndFeel::drawMetallicGradient (juce::Graphics& g,
                                              const juce::Rectangle<int>& bounds,
                                              float rotation)
{
    auto gradient = metallicGradient.transformed (
        juce::AffineTransform::rotation (rotation,
                                        bounds.getCentreX(),
                                        bounds.getCentreY())
    );
    
    g.setGradientFill (gradient);
    g.fillRect (bounds);
}

void CustomLookAndFeel::drawLedGlow (juce::Graphics& g,
                                     float centreX, float centreY,
                                     float radius,
                                     const juce::Colour& colour,
                                     float intensity)
{
    // Create radial gradient for LED glow
    const float glowRadius = radius * (2.0f + intensity * 3.0f);
    
    juce::ColourGradient gradient (
        colour.withMultipliedAlpha (intensity),
        centreX, centreY,
        colour.withMultipliedAlpha (0.0f),
        centreX, centreY,
        glowRadius,
        true
    );
    
    g.setGradientFill (gradient);
    g.fillEllipse (
        centreX - glowRadius,
        centreY - glowRadius,
        glowRadius * 2.0f,
        glowRadius * 2.0f
    );
}

// ============================================================================
// Assets
// ============================================================================

void CustomLookAndFeel::loadKnobFilmstrip (const juce::String& assetPath)
{
    // Load filmstrip image from assets
    knobFilmstrip = juce::ImageCache::getFromMemory (
        BinaryData::knob_metal_01_png,
        BinaryData::knob_metal_01_pngSize
    );
}

void CustomLookAndFeel::setKnobFilmstrip (int filmstripIndex)
{
    filmstripFrames = juce::jlimit (1, 128, filmstripIndex);
}

} // namespace fleen
