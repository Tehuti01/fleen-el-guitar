#pragma once

#include <JuceHeader.h>

namespace fleen
{

/**
 * @brief Gain stage DSP processor
 * 
 * Provides clean gain amplification with smooth parameter transitions.
 */
class GainStage
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    GainStage() = default;
    
    // ========================================================================
    // JUCE DSP Processor Interface
    // ========================================================================
    
    /** @brief Prepare processor for playback */
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        gainLinear.store (0.5f);
        targetGainLinear.store (0.5f);
    }
    
    /** @brief Reset processor state */
    void reset()
    {
        gainLinear.store (0.5f);
        targetGainLinear.store (0.5f);
    }
    
    /**
     * @brief Process audio block
     * @param context Processing context
     */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto inputBlock = context.getInputBlock();
        const auto outputBlock = context.getOutputBlock();
        const auto numChannels = inputBlock.getNumChannels();
        const auto numSamples = inputBlock.getNumSamples();
        
        // Smooth gain transition
        const float gainIncrement = (targetGainLinear.load() - gainLinear.load()) / static_cast<float> (numSamples);
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* input = inputBlock.getChannelPointer (channel);
            auto* output = outputBlock.getChannelPointer (channel);
            
            float currentGain = gainLinear.load();
            
            for (size_t i = 0; i < numSamples; ++i)
            {
                currentGain += gainIncrement;
                output[i] = input[i] * currentGain;
            }
            
            gainLinear.store (currentGain);
        }
    }

    // ========================================================================
    // Parameters
    // ========================================================================
    
    /** @brief Set gain (0.0 - 1.0) */
    void setGain (float newGain)
    {
        targetGainLinear.store (newGain);
    }
    
    /** @brief Get current gain */
    float getGain() const { return gainLinear.load(); }

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    double sampleRate { 44100.0 };
    std::atomic<float> gainLinear { 0.5f };
    std::atomic<float> targetGainLinear { 0.5f };
};

/**
 * @brief Tone stack DSP processor
 * 
 * Implements Baxandall-style tone control with Bass, Mid, Treble, and Presence.
 */
class ToneStack
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    ToneStack() = default;
    
    // ========================================================================
    // JUCE DSP Processor Interface
    // ========================================================================
    
    /** @brief Prepare processor for playback */
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // Initialize filters
        bassLeft.prepare (*spec);
        bassRight.prepare (*spec);
        midLeft.prepare (*spec);
        midRight.prepare (*spec);
        trebleLeft.prepare (*spec);
        trebleRight.prepare (*spec);
        presenceLeft.prepare (*spec);
        presenceRight.prepare (*spec);
        
        updateCoefficients();
    }
    
    /** @brief Reset processor state */
    void reset()
    {
        bassLeft.reset();
        bassRight.reset();
        midLeft.reset();
        midRight.reset();
        trebleLeft.reset();
        trebleRight.reset();
        presenceLeft.reset();
        presenceRight.reset();
    }
    
    /**
     * @brief Process audio block
     * @param context Processing context
     */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto inputBlock = context.getInputBlock();
        const auto outputBlock = context.getOutputBlock();
        const auto numChannels = inputBlock.getNumChannels();
        
        if (numChannels >= 2)
        {
            // Process left channel
            auto* leftIn = inputBlock.getChannelPointer (0);
            auto* leftOut = outputBlock.getChannelPointer (0);
            juce::dsp::AudioBlock<float> leftBlock (leftIn, 1, inputBlock.getNumSamples());
            juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
            
            bassLeft.process (leftContext);
            midLeft.process (leftContext);
            trebleLeft.process (leftContext);
            presenceLeft.process (leftContext);
            
            // Process right channel
            auto* rightIn = inputBlock.getChannelPointer (1);
            auto* rightOut = outputBlock.getChannelPointer (1);
            juce::dsp::AudioBlock<float> rightBlock (rightIn, 1, inputBlock.getNumSamples());
            juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
            
            bassRight.process (rightContext);
            midRight.process (rightContext);
            trebleRight.process (rightContext);
            presenceRight.process (rightContext);
        }
        else if (numChannels == 1)
        {
            auto* monoIn = inputBlock.getChannelPointer (0);
            auto* monoOut = outputBlock.getChannelPointer (0);
            juce::dsp::AudioBlock<float> monoBlock (monoIn, 1, inputBlock.getNumSamples());
            juce::dsp::ProcessContextReplacing<float> monoContext (monoBlock);
            
            bassLeft.process (monoContext);
            midLeft.process (monoContext);
            trebleLeft.process (monoContext);
            presenceLeft.process (monoContext);
        }
    }

    // ========================================================================
    // Parameters
    // ========================================================================
    
    void setBass (float newBass) { targetBass.store (newBass); updateCoefficients(); }
    void setMid (float newMid) { targetMid.store (newMid); updateCoefficients(); }
    void setTreble (float newTreble) { targetTreble.store (newTreble); updateCoefficients(); }
    void setPresence (float newPresence) { targetPresence.store (newPresence); updateCoefficients(); }

private:
    // ========================================================================
    // Internal Methods
    // ========================================================================
    
    void updateCoefficients()
    {
        // Update filter coefficients based on tone controls
        // This is a simplified implementation - a full tone stack would use
        // more sophisticated filter designs
        
        const float bassFreq = 200.0f;
        const float midFreq = 1000.0f;
        const float trebleFreq = 3000.0f;
        const float presenceFreq = 5000.0f;
        
        const float bassGain = juce::Decibels::decibelsToGain (juce::jmap (targetBass.load(), -12.0f, 12.0f));
        const float midGain = juce::Decibels::decibelsToGain (juce::jmap (targetMid.load(), -12.0f, 12.0f));
        const float trebleGain = juce::Decibels::decibelsToGain (juce::jmap (targetTreble.load(), -12.0f, 12.0f));
        const float presenceGain = juce::Decibels::decibelsToGain (juce::jmap (targetPresence.load(), -6.0f, 6.0f));
        
        bassLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, bassFreq, 1.0f, bassGain);
        bassRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, bassFreq, 1.0f, bassGain);
        
        midLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makePeaking (sampleRate, midFreq, 1.0f, midGain);
        midRight.coefficients = juce::dsp::IIR::Coefficients<float>::makePeaking (sampleRate, midFreq, 1.0f, midGain);
        
        trebleLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, trebleFreq, 1.0f, trebleGain);
        trebleRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, trebleFreq, 1.0f, trebleGain);
        
        presenceLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, presenceFreq, 0.5f, presenceGain);
        presenceRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, presenceFreq, 0.5f, presenceGain);
    }

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    double sampleRate { 44100.0 };
    
    // Tone controls
    std::atomic<float> targetBass { 0.5f };
    std::atomic<float> targetMid { 0.5f };
    std::atomic<float> targetTreble { 0.5f };
    std::atomic<float> targetPresence { 0.5f };
    
    // Filters (stereo)
    juce::dsp::IIR::Filter<float> bassLeft, bassRight;
    juce::dsp::IIR::Filter<float> midLeft, midRight;
    juce::dsp::IIR::Filter<float> trebleLeft, trebleRight;
    juce::dsp::IIR::Filter<float> presenceLeft, presenceRight;
};

} // namespace fleen
