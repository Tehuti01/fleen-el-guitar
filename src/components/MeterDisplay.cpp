#include "MeterDisplay.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

MeterDisplay::MeterDisplay()
{
    setOpaque (true);
    startTimerHz (60);
}

MeterDisplay::~MeterDisplay()
{
    stopTimer();
}

// ============================================================================
// Component Interface
// ============================================================================

void MeterDisplay::timerCallback()
{
    // Apply ballistic response
    if (targetLevel > currentLevel)
    {
        // Attack
        currentLevel += (targetLevel - currentLevel) * attackTime;
    }
    else
    {
        // Release
        currentLevel += (targetLevel - currentLevel) * releaseTime;
    }
    
    // Clamp
    currentLevel = juce::jlimit (0.0f, 1.0f, currentLevel);
    
    repaint();
}

void MeterDisplay::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    
    // Draw background
    drawBackground (g, bounds);
    
    // Draw level bar
    drawLevelBar (g, bounds, currentLevel);
    
    // Draw scale
    drawScale (g, bounds);
}

void MeterDisplay::resized()
{
    repaint();
}

// ============================================================================
// Level Control
// ============================================================================

void MeterDisplay::setLevel (float newLevel)
{
    targetLevel = juce::jlimit (0.0f, 1.0f, newLevel * sensitivity);
}

void MeterDisplay::setSensitivity (float newSensitivity)
{
    sensitivity = juce::jmax (0.1f, newSensitivity);
}

// ============================================================================
// Rendering
// ============================================================================

void MeterDisplay::drawBackground (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    // Dark background
    g.setColour (juce::Colour::fromRGB (20, 20, 25));
    g.fillRect (bounds);
    
    // Inner bevel
    g.setColour (juce::Colour::fromRGB (40, 40, 45));
    g.drawRect (bounds, 2.0f);
    
    // Scale markings background
    const float barWidth = bounds.getWidth() * 0.6f;
    const float barX = bounds.getX() + (bounds.getWidth() - barWidth) / 2.0f;
    juce::Rectangle<float> barArea (barX, bounds.getY() + 2.0f, barWidth, bounds.getHeight() - 4.0f);
    
    g.setColour (juce::Colour::fromRGB (30, 30, 35));
    g.fillRect (barArea);
}

void MeterDisplay::drawLevelBar (juce::Graphics& g, const juce::Rectangle<float>& bounds, float level)
{
    const float barWidth = bounds.getWidth() * 0.6f;
    const float barX = bounds.getX() + (bounds.getWidth() - barWidth) / 2.0f;
    const float barHeight = bounds.getHeight() - 8.0f;
    const float barY = bounds.getY() + 4.0f;
    
    // Calculate filled height
    const float filledHeight = barHeight * level;
    const float emptyHeight = barHeight - filledHeight;
    
    // Draw gradient fill
    juce::ColourGradient gradient (
        getLevelColour (1.0f), // Top color (red)
        barX, barY,
        getLevelColour (0.0f), // Bottom color (green)
        barX, barY + barHeight,
        false
    );
    
    // Draw filled portion
    juce::Rectangle<float> filledArea (
        barX,
        barY + emptyHeight,
        barWidth,
        filledHeight
    );
    
    g.setGradientFill (gradient);
    g.fillRect (filledArea);
    
    // Draw peak indicator
    if (level > 0.01f)
    {
        const float peakY = barY + emptyHeight - 2.0f;
        g.setColour (juce::Colours::white.withMultipliedAlpha (0.8f));
        g.fillRect (barX, peakY, barWidth, 2.0f);
    }
}

void MeterDisplay::drawScale (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const float barWidth = bounds.getWidth() * 0.6f;
    const float barX = bounds.getX() + (bounds.getWidth() - barWidth) / 2.0f;
    const float barHeight = bounds.getHeight() - 8.0f;
    const float barY = bounds.getY() + 4.0f;
    
    // Draw scale markings
    g.setColour (juce::Colour::fromRGB (100, 100, 110));
    
    const int numMarks = 10;
    for (int i = 0; i <= numMarks; ++i)
    {
        const float y = barY + barHeight * (1.0f - static_cast<float> (i) / numMarks);
        const float lineWidth = (i % 5 == 0) ? barWidth * 0.15f : barWidth * 0.08f;
        const float lineX = barX - lineWidth;
        
        g.fillRect (lineX, y, lineWidth, 1.0f);
        g.fillRect (barX + barWidth, y, lineWidth, 1.0f);
    }
}

juce::Colour MeterDisplay::getLevelColour (float level) const
{
    if (level < 0.7f)
    {
        // Green to yellow
        return juce::Colour::fromRGB (50, 200, 50).interpolatedWith (
            juce::Colour::fromRGB (200, 200, 50), level / 0.7f);
    }
    else if (level < 0.9f)
    {
        // Yellow to orange
        return juce::Colour::fromRGB (200, 200, 50).interpolatedWith (
            juce::Colour::fromRGB (255, 150, 50), (level - 0.7f) / 0.2f);
    }
    else
    {
        // Orange to red
        return juce::Colour::fromRGB (255, 150, 50).interpolatedWith (
            juce::Colour::fromRGB (255, 50, 50), (level - 0.9f) / 0.1f);
    }
}

} // namespace fleen
