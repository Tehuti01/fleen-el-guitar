#pragma once

#include <JuceHeader.h>

namespace fleen
{

/**
 * @brief Golden Ratio utility constants and functions
 * 
 * Provides mathematical constants and helper functions for
 * Golden Ratio-based design decisions.
 */
namespace GoldenRatio
{
    // ========================================================================
    // Constants
    // ========================================================================
    
    /** @brief The Golden Ratio (φ) */
    constexpr float phi = 1.61803398875f;
    
    /** @brief Inverse Golden Ratio (1/φ) */
    constexpr float invPhi = 0.61803398875f;
    
    /** @brief Golden Ratio squared (φ²) */
    constexpr float phiSquared = 2.61803398875f;
    
    // ========================================================================
    // Spacing Scale (Fibonacci-derived)
    // ========================================================================
    
    /** @brief Base unit for spacing calculations */
    constexpr int baseUnit = 10;
    
    /** @brief Fibonacci spacing sequence */
    constexpr int spacingScale[] = { 6, 10, 16, 26, 42, 68, 110, 178 };
    
    /**
     * @brief Get spacing value for a given tier
     * @param tier Spacing tier (0-7)
     * @return Spacing value in pixels
     */
    inline int getSpacing (int tier)
    {
        return spacingScale[juce::jlimit (0, 7, tier)];
    }
    
    // ========================================================================
    // Typography Scale
    // ========================================================================
    
    /** @brief Base font size */
    constexpr float baseFontSize = 14.0f;
    
    /**
     * @brief Calculate font size using Golden Ratio
     * @param steps Number of steps from base (negative for smaller, positive for larger)
     * @return Font size in points
     */
    inline float getFontSize (int steps)
    {
        return baseFontSize * std::pow (phi, static_cast<float> (steps));
    }
    
    // ========================================================================
    // Layout Helpers
    // ========================================================================
    
    /**
     * @brief Calculate primary dimension using 61.8% rule
     * @param total Total dimension
     * @return Primary dimension (61.8% of total)
     */
    inline int getPrimaryDimension (int total)
    {
        return static_cast<int> (total * invPhi);
    }
    
    /**
     * @brief Calculate secondary dimension using 61.8% rule
     * @param total Total dimension
     * @return Secondary dimension (38.2% of total)
     */
    inline int getSecondaryDimension (int total)
    {
        return total - getPrimaryDimension (total);
    }
    
    /**
     * @brief Center a component within a bounds using Golden Ratio margins
     * @param bounds Parent bounds
     * @param componentWidth Desired component width
     * @param componentHeight Desired component height
     * @return Centered rectangle
     */
    inline juce::Rectangle<int> centerInBounds (const juce::Rectangle<int>& bounds,
                                                int componentWidth,
                                                int componentHeight)
    {
        const int x = bounds.getX() + (bounds.getWidth() - componentWidth) / 2;
        const int y = bounds.getY() + (bounds.getHeight() - componentHeight) / 2;
        return juce::Rectangle<int> (x, y, componentWidth, componentHeight);
    }
    
    // ========================================================================
    // Color Helpers
    // ========================================================================
    
    /**
     * @brief Interpolate between two colors using Golden Ratio
     * @param colour1 First colour
     * @param colour2 Second colour
     * @param t Interpolation factor (0.0 - 1.0)
     * @return Interpolated colour
     */
    inline juce::Colour interpolateColour (const juce::Colour& colour1,
                                           const juce::Colour& colour2,
                                           float t)
    {
        return colour1.interpolatedWith (colour2, t);
    }
    
} // namespace GoldenRatio

} // namespace fleen
