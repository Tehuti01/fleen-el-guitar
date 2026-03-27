#include "OpenGLVisualizer.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

OpenGLVisualizer::OpenGLVisualizer()
{
    setOpaque (true);
    
    // Initialize buffers
    waveformBuffer.fill (0.0f);
    spectrumBuffer.fill (0.0f);
    
    // Start timer for animations (60 FPS)
    startTimerHz (60);
}

OpenGLVisualizer::~OpenGLVisualizer()
{
    stopTimer();
}

// ============================================================================
// Component Interface
// ============================================================================

void OpenGLVisualizer::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    
    // Draw dark background with subtle gradient
    juce::ColourGradient background (
        juce::Colour::fromRGB (15, 15, 20),
        0.0f, 0.0f,
        juce::Colour::fromRGB (25, 25, 30),
        0.0f, bounds.getHeight(),
        false
    );
    g.setGradientFill (background);
    g.fillRect (bounds);
    
    // Draw grid
    drawGrid (g, bounds);
    
    // Draw visualization based on mode
    switch (visualizerMode)
    {
        case Mode::waveform:
            drawWaveform (g, bounds);
            break;
        case Mode::spectrum:
            drawSpectrum (g, bounds);
            break;
        case Mode::spectrogram:
            drawWaveform (g, bounds); // Fallback to waveform for now
            break;
    }
    
    // Draw label
    drawLabel (g, bounds);
}

void OpenGLVisualizer::resized()
{
    repaint();
}

// ============================================================================
// Setup
// ============================================================================

void OpenGLVisualizer::setProcessor (PluginProcessor* newProcessor)
{
    processor = newProcessor;
}

void OpenGLVisualizer::updateDisplay()
{
    // Update waveform buffer from processor
    if (processor != nullptr)
    {
        // Generate animated waveform based on input/output levels
        const float inputLevel = processor->getInputLevel();
        const float outputLevel = processor->getOutputLevel();
        
        // Create smooth animated waveform
        for (size_t i = 0; i < waveformBuffer.size(); ++i)
        {
            const float t = static_cast<float> (i) / waveformBuffer.size();
            const float phase1 = t * juce::MathConstants<float>::twoPi + phase;
            const float phase2 = t * juce::MathConstants<float>::twoPi * 2.0f + phase * 1.5f;
            
            // Combine multiple sine waves for organic look
            float sample = std::sin (phase1) * 0.5f;
            sample += std::sin (phase2) * 0.3f;
            sample += std::sin (phase1 * 3.0f) * 0.2f;
            
            // Apply envelope from audio levels
            const float envelope = inputLevel * 0.5f + outputLevel * 0.5f;
            sample *= envelope;
            
            // Add some noise for texture
            sample += juce::Random::getSystemRandom().nextFloat() * 0.05f * envelope;
            
            waveformBuffer[i] = sample;
        }
        
        // Generate spectrum buffer (simplified FFT simulation)
        for (size_t i = 0; i < spectrumBuffer.size(); ++i)
        {
            const float t = static_cast<float> (i) / spectrumBuffer.size();
            spectrumBuffer[i] = (inputLevel + outputLevel) * 0.5f * 
                               (1.0f - t * 0.5f) * 
                               (0.5f + 0.5f * std::sin (phase * 2.0f + t * 10.0f));
        }
        
        // Update animation phase
        phase += 0.05f;
        if (phase > juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
        
        // Update glow intensity based on level
        glowIntensity = juce::jlimit (0.0f, 1.0f, 
            glowIntensity + (inputLevel - glowIntensity) * 0.1f);
    }
    
    repaint();
}

void OpenGLVisualizer::setMode (Mode newMode)
{
    visualizerMode = newMode;
    repaint();
}

// ============================================================================
// Timer Interface
// ============================================================================

void OpenGLVisualizer::timerCallback()
{
    // Continuous animation even without audio
    if (processor == nullptr || (processor->getInputLevel() < 0.01f && 
                                  processor->getOutputLevel() < 0.01f))
    {
        phase += 0.02f;
        if (phase > juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
        
        repaint();
    }
}

// ============================================================================
// Rendering
// ============================================================================

void OpenGLVisualizer::drawWaveform (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const float centerY = bounds.getCentreY();
    const float amplitude = bounds.getHeight() * 0.35f;
    
    // Create waveform path
    juce::Path waveformPath;
    
    for (size_t i = 0; i < waveformBuffer.size(); ++i)
    {
        const float x = bounds.getX() + (static_cast<float> (i) / waveformBuffer.size()) * bounds.getWidth();
        const float y = centerY + waveformBuffer[i] * amplitude;
        
        if (i == 0)
            waveformPath.startNewSubPath (x, y);
        else
            waveformPath.lineTo (x, y);
    }
    
    // Draw glow effect
    if (glowIntensity > 0.01f)
    {
        juce::PathStrokeType glowStroke (3.0f + glowIntensity * 4.0f);
        g.setColour (primaryColour.withMultipliedAlpha (glowIntensity * 0.3f));
        g.strokePath (waveformPath, glowStroke);
    }
    
    // Draw main waveform with gradient
    juce::ColourGradient gradient (
        primaryColour,
        bounds.getX(), centerY - amplitude,
        secondaryColour,
        bounds.getX() + bounds.getWidth(), centerY + amplitude,
        false
    );
    
    g.setGradientFill (gradient);
    g.strokePath (waveformPath, juce::PathStrokeType (2.0f));

    // Draw mirrored waveform (for stereo effect)
    juce::Path mirroredPath;
    for (size_t i = 0; i < waveformBuffer.size(); ++i)
    {
        const float x = bounds.getX() + (static_cast<float> (i) / waveformBuffer.size()) * bounds.getWidth();
        const float y = centerY - waveformBuffer[i] * amplitude;

        if (i == 0)
            mirroredPath.startNewSubPath (x, y);
        else
            mirroredPath.lineTo (x, y);
    }

    g.setColour (primaryColour.withMultipliedAlpha (0.5f));
    g.strokePath (mirroredPath, juce::PathStrokeType (1.0f));
}

void OpenGLVisualizer::drawSpectrum (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const int numBars = static_cast<int> (spectrumBuffer.size());
    const float barWidth = bounds.getWidth() / numBars;
    const float maxHeight = bounds.getHeight() * 0.8f;
    
    for (int i = 0; i < numBars; ++i)
    {
        const float x = bounds.getX() + i * barWidth;
        const float barHeight = spectrumBuffer[i] * maxHeight;
        const float y = bounds.getBottom() - barHeight;
        
        // Get color based on frequency (position)
        const float t = static_cast<float> (i) / numBars;
        juce::Colour barColour = primaryColour.interpolatedWith (secondaryColour, t);
        
        // Draw bar with gradient
        juce::ColourGradient gradient (
            barColour,
            x, y,
            barColour.withMultipliedAlpha (0.3f),
            x, bounds.getBottom(),
            false
        );
        
        g.setGradientFill (gradient);
        g.fillRect (x + 1.0f, y, barWidth - 2.0f, barHeight);
    }
}

void OpenGLVisualizer::drawGrid (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour (gridColour);
    
    // Vertical lines
    const int numVerticalLines = 5;
    for (int i = 0; i <= numVerticalLines; ++i)
    {
        const float x = bounds.getX() + (static_cast<float> (i) / numVerticalLines) * bounds.getWidth();
        g.drawLine (x, bounds.getY(), x, bounds.getY() + bounds.getHeight(), 1.0f);
    }
    
    // Horizontal lines
    const int numHorizontalLines = 4;
    for (int i = 0; i <= numHorizontalLines; ++i)
    {
        const float y = bounds.getY() + (static_cast<float> (i) / numHorizontalLines) * bounds.getHeight();
        g.drawLine (bounds.getX(), y, bounds.getX() + bounds.getWidth(), y, 1.0f);
    }
}

void OpenGLVisualizer::drawLabel (juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    const juce::String modeText = [this]
    {
        switch (visualizerMode)
        {
            case Mode::waveform: return "WAVEFORM";
            case Mode::spectrum: return "SPECTRUM";
            case Mode::spectrogram: return "SPECTROGRAM";
            default: return "";
        }
    }();
    
    g.setColour (juce::Colours::white.withMultipliedAlpha (0.5f));
    g.setFont (10.0f);
    g.drawText (modeText, bounds.reduced (8), juce::Justification::topRight);
}

} // namespace fleen
