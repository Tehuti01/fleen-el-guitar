#include "LedIndicator.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

LedIndicator::LedIndicator (Color color)
    : ledColor (color)
{
    setOpaque (false);
}

LedIndicator::~LedIndicator()
{
}

// ============================================================================
// Component Interface
// ============================================================================

void LedIndicator::paint (juce::Graphics& g)
{
    g.setRenderQuality (juce::Graphics::highRenderingQuality);
    
    const auto bounds = getLocalBounds().toFloat();
    
    // Draw glow effect when on
    if (isOn)
        drawGlow (g, bounds);
    
    // Draw LED body
    drawLedBody (g, bounds);
}

void LedIndicator::resized()
{
    // Repaint when resized to update gradients
    repaint();
}

// ============================================================================
// State
// ============================================================================

void LedIndicator::setState (bool newState)
{
    if (isOn != newState)
    {
        isOn = newState;
        repaint();
    }
}

void LedIndicator::toggle()
{
    setState (!isOn);
}

void LedIndicator::setBrightness (float newBrightness)
{
    brightness = juce::jlimit (0.0f, 1.0f, newBrightness);
    repaint();
}

// ============================================================================
// Rendering
// ============================================================================

void LedIndicator::drawLedBody (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const auto centre = bounds.getCentre();
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.4f;
    
    // Draw LED circle
    juce::Path ledPath;
    ledPath.addEllipse (
        centre.getX() - radius,
        centre.getY() - radius,
        radius * 2.0f,
        radius * 2.0f
    );
    
    // Base color (off state)
    juce::Colour baseColour = getLedColour();
    juce::Colour offColour = baseColour.withMultipliedAlpha (0.2f).withMultipliedBrightness (0.3f);
    
    if (isOn)
    {
        // On state - bright with gradient
        juce::ColourGradient gradient (
            baseColour.withMultipliedAlpha (brightness),
            centre.getX() - radius * 0.3f,
            centre.getY() - radius * 0.3f,
            baseColour.withMultipliedBrightness (0.5f).withMultipliedAlpha (brightness),
            centre.getX() + radius * 0.5f,
            centre.getY() + radius * 0.5f,
            false
        );
        
        g.setGradientFill (gradient);
        g.fillPath (ledPath);
        
        // Add bright center highlight
        g.setColour (juce::Colours::white.withMultipliedAlpha (brightness * 0.8f));
        g.fillEllipse (
            centre.getX() - radius * 0.3f,
            centre.getY() - radius * 0.3f,
            radius * 0.6f,
            radius * 0.6f
        );
    }
    else
    {
        // Off state - dim
        g.setColour (offColour);
        g.fillPath (ledPath);
    }
    
    // Draw bezel
    g.setColour (juce::Colour::fromRGB (40, 40, 45));
    g.strokePath (ledPath, juce::PathStrokeType (2.0f));
    
    // Draw inner ring
    juce::Path innerRing;
    const float innerRadius = radius * 0.7f;
    innerRing.addEllipse (
        centre.getX() - innerRadius,
        centre.getY() - innerRadius,
        innerRadius * 2.0f,
        innerRadius * 2.0f
    );
    g.setColour (juce::Colour::fromRGBA (0, 0, 0, 40));
    g.strokePath (innerRing, juce::PathStrokeType (1.0f));
}

void LedIndicator::drawGlow (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const auto centre = bounds.getCentre();
    const float glowRadius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.8f;
    
    juce::Colour glowColour = getLedColour();
    
    // Create radial gradient for glow
    juce::ColourGradient gradient (
        glowColour.withMultipliedAlpha (brightness * 0.4f),
        centre.getX(), centre.getY(),
        glowColour.withMultipliedAlpha (0.0f),
        centre.getX(), centre.getY(),
        glowRadius,
        true
    );
    
    g.setGradientFill (gradient);
    g.fillRect (
        centre.getX() - glowRadius,
        centre.getY() - glowRadius,
        glowRadius * 2.0f,
        glowRadius * 2.0f
    );
}

juce::Colour LedIndicator::getLedColour() const
{
    switch (ledColor)
    {
        case Color::red:
            return juce::Colour::fromRGB (255, 50, 50);
        case Color::blue:
            return juce::Colour::fromRGB (50, 150, 255);
        case Color::green:
            return juce::Colour::fromRGB (50, 255, 100);
        case Color::amber:
            return juce::Colour::fromRGB (255, 180, 50);
        default:
            return juce::Colours::red;
    }
}

} // namespace fleen
