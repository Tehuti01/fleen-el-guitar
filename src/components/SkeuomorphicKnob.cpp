#include "SkeuomorphicKnob.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

SkeuomorphicKnob::SkeuomorphicKnob (const juce::String& name)
    : labelText (name)
{
    // Setup slider
    slider.setSliderStyle (juce::Slider::Rotary);
    slider.setRange (0.0, 1.0, 0.01);
    slider.setValue (0.5);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    
    // Add slider as child component (for APVTS attachment)
    addAndMakeVisible (slider);
    slider.setAlwaysOnTop (true);
    slider.setInterceptsMouseClicks (false, false); // Pass through mouse events
    
    // Make component opaque for better performance
    setOpaque (true);
    
    // Start timer for animations
    startTimerHz (60);
}

SkeuomorphicKnob::~SkeuomorphicKnob()
{
    stopTimer();
}

// ============================================================================
// Component Interface
// ============================================================================

void SkeuomorphicKnob::paint (juce::Graphics& g)
{
    g.setRenderQuality (juce::Graphics::highRenderingQuality);
    g.setRenderQuality (juce::Graphics::antiAliasedRendering);
    
    const auto bounds = getLocalBounds().toFloat();
    
    // Draw outer glow when hovered
    if (isHovered)
    {
        juce::ColourGradient glow (
            ringColour.withMultipliedAlpha (0.3f * highlightAlpha),
            bounds.getCentreX(), bounds.getCentreY(),
            ringColour.withMultipliedAlpha (0.0f),
            bounds.getCentreX(), bounds.getCentreY(),
            bounds.getWidth() * 0.7f,
            true
        );
        g.setGradientFill (glow);
        g.fillRect (bounds);
    }
    
    // Draw shadow ring
    {
        juce::Path shadowRing;
        const float shadowRadius = bounds.getWidth() * 0.45f;
        shadowRing.addArc (
            bounds.getCentreX() - shadowRadius,
            bounds.getCentreY() - shadowRadius,
            shadowRadius * 2.0f,
            shadowRadius * 2.0f,
            0.0f,
            juce::MathConstants<float>::twoPi
        );
        
        // Dark shadow (bottom-right)
        g.setColour (juce::Colour::fromRGBA (0, 0, 0, 80));
        g.fillPath (shadowRing, juce::AffineTransform::translation (4.0f, 4.0f));
        
        // Light shadow (top-left)
        g.setColour (juce::Colour::fromRGBA (255, 255, 255, 30));
        g.fillPath (shadowRing, juce::AffineTransform::translation (-4.0f, -4.0f));
    }
    
    // Draw knob body
    drawKnobBody (g, bounds);
    
    // Draw indicator
    const float angle = juce::MathConstants<float>::pi * 0.75f + 
                       value * juce::MathConstants<float>::pi * 1.5f;
    drawIndicator (g, bounds, angle);
    
    // Draw label
    drawLabel (g, bounds);
    
    // Draw highlight ring
    if (isDragging || isHovered)
        drawHighlightRing (g, bounds);
}

void SkeuomorphicKnob::resized()
{
    // Slider covers entire component (invisible, just for APVTS)
    slider.setBounds (getLocalBounds());
}

void SkeuomorphicKnob::mouseDown (const juce::MouseEvent& event)
{
    isDragging = true;
    lastMouseY = static_cast<float> (event.y);
    highlightAlpha = 1.0f;
    repaint();
}

void SkeuomorphicKnob::mouseDrag (const juce::MouseEvent& event)
{
    if (!isDragging)
        return;
    
    const float deltaY = lastMouseY - static_cast<float> (event.y);
    const float sensitivity = 0.005f;
    
    float newValue = value + deltaY * sensitivity * (maxValue - minValue);
    newValue = juce::jlimit (minValue, maxValue, newValue);
    
    setValue (newValue, juce::NotificationType::sendNotificationSync);
    
    lastMouseY = static_cast<float> (event.y);
    repaint();
}

void SkeuomorphicKnob::mouseUp (const juce::MouseEvent&)
{
    isDragging = false;
    highlightAlpha = 0.0f;
    repaint();
}

void SkeuomorphicKnob::mouseEnter (const juce::MouseEvent&)
{
    isHovered = true;
    setMouseCursor (juce::MouseCursor::LeftRightResizeCursor);
    repaint();
}

void SkeuomorphicKnob::mouseExit (const juce::MouseEvent&)
{
    isHovered = false;
    highlightAlpha = 0.0f;
    repaint();
}

// ============================================================================
// Value Interface
// ============================================================================

void SkeuomorphicKnob::setValue (float newValue, juce::NotificationType notification)
{
    value = juce::jlimit (minValue, maxValue, newValue);
    slider.setValue (value, notification);
    repaint();
}

void SkeuomorphicKnob::setRange (float newMin, float newMax)
{
    minValue = newMin;
    maxValue = newMax;
    slider.setRange (newMin, newMax);
}

void SkeuomorphicKnob::setLabelText (const juce::String& newText)
{
    labelText = newText;
    repaint();
}

// ============================================================================
// Rendering
// ============================================================================

void SkeuomorphicKnob::drawKnobBody (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const float radius = bounds.getWidth() * 0.4f;
    const auto centre = bounds.getCentre();
    
    // Create metallic gradient
    juce::ColourGradient gradient (
        knobHighlight,
        centre.getX() - radius * 0.3f,
        centre.getY() - radius * 0.3f,
        knobBase,
        centre.getX() + radius * 0.5f,
        centre.getY() + radius * 0.5f,
        false
    );
    
    // Add highlight stop
    gradient.addColour (0.5, juce::Colour (80, 80, 90));
    
    g.setGradientFill (gradient);
    
    // Draw knob circle
    juce::Path knobPath;
    knobPath.addEllipse (
        centre.getX() - radius,
        centre.getY() - radius,
        radius * 2.0f,
        radius * 2.0f
    );
    g.fillPath (knobPath);
    
    // Draw outer bevel
    g.setColour (juce::Colour::fromRGB (40, 40, 50));
    g.strokePath (knobPath, juce::PathStrokeType (2.0f));
    
    // Draw inner ring
    juce::Path innerRing;
    const float innerRadius = radius * 0.85f;
    innerRing.addEllipse (
        centre.getX() - innerRadius,
        centre.getY() - innerRadius,
        innerRadius * 2.0f,
        innerRadius * 2.0f
    );
    g.setColour (juce::Colour::fromRGBA (0, 0, 0, 30));
    g.strokePath (innerRing, juce::PathStrokeType (1.0f));
}

void SkeuomorphicKnob::drawIndicator (juce::Graphics& g, const juce::Rectangle<float>& bounds, float angle)
{
    const auto centre = bounds.getCentre();
    const float outerRadius = bounds.getWidth() * 0.35f;
    const float innerRadius = bounds.getWidth() * 0.15f;
    
    const float x1 = centre.getX() + std::cos (angle) * innerRadius;
    const float y1 = centre.getY() + std::sin (angle) * innerRadius;
    const float x2 = centre.getX() + std::cos (angle) * outerRadius;
    const float y2 = centre.getY() + std::sin (angle) * outerRadius;
    
    // Draw indicator line with gradient
    juce::ColourGradient gradient (
        indicatorColour,
        x1, y1,
        indicatorColour.withMultipliedBrightness (0.7f),
        x2, y2,
        false
    );
    
    g.setGradientFill (gradient);
    g.drawLine (x1, y1, x2, y2, 4.0f);
    
    // Draw indicator dot
    g.setColour (indicatorColour);
    g.fillEllipse (x2 - 4.0f, y2 - 4.0f, 8.0f, 8.0f);
    
    // Draw dot highlight
    g.setColour (juce::Colours::white.withMultipliedAlpha (0.5f));
    g.fillEllipse (x2 - 2.0f, y2 - 2.0f, 4.0f, 4.0f);
}

void SkeuomorphicKnob::drawLabel (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const auto labelBounds = bounds.reduced (bounds.getWidth() * 0.25f)
                                  .removeFromBottom (bounds.getHeight() * 0.2f);
    
    g.setColour (juce::Colours::white.withMultipliedAlpha (0.8f));
    g.setFont (juce::Font ("Inter", 12.0f, juce::Font::bold));
    g.drawText (labelText, labelBounds, juce::Justification::centred);
}

void SkeuomorphicKnob::drawHighlightRing (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const float radius = bounds.getWidth() * 0.42f;
    const auto centre = bounds.getCentre();
    
    juce::Path ring;
    ring.addArc (
        centre.getX() - radius,
        centre.getY() - radius,
        radius * 2.0f,
        radius * 2.0f,
        juce::MathConstants<float>::pi * 0.75f,
        juce::MathConstants<float>::pi * 2.25f
    );
    
    g.setColour (ringColour.withMultipliedAlpha (highlightAlpha * 0.5f));
    g.strokePath (ring, juce::PathStrokeType (2.0f, juce::PathStrokeType::rounded));
}

} // namespace fleen
