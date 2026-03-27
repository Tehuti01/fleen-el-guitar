#pragma once

#include <JuceHeader.h>

namespace fleen
{

/**
 * @brief Distortion DSP processor
 * 
 * Implements soft-clipping distortion with smooth drive control.
 */
class Distortion
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    Distortion() = default;
    
    // ========================================================================
    // JUCE DSP Processor Interface
    // ========================================================================
    
    /** @brief Prepare processor for playback */
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        driveLinear.store (0.5f);
        targetDriveLinear.store (0.5f);
    }
    
    /** @brief Reset processor state */
    void reset()
    {
        driveLinear.store (0.5f);
        targetDriveLinear.store (0.5f);
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
        
        // Smooth drive transition
        const float driveIncrement = (targetDriveLinear.load() - driveLinear.load()) / static_cast<float> (numSamples);
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* input = inputBlock.getChannelPointer (channel);
            auto* output = outputBlock.getChannelPointer (channel);
            
            float currentDrive = driveLinear.load();
            const float gain = 1.0f + currentDrive * 20.0f; // Drive range: 1x - 21x
            
            for (size_t i = 0; i < numSamples; ++i)
            {
                currentDrive += driveIncrement;
                const float currentGain = 1.0f + currentDrive * 20.0f;
                
                // Apply gain
                float sample = input[i] * currentGain;
                
                // Soft clipping using tanh
                sample = std::tanh (sample);
                
                // Add subtle harmonic enhancement
                const float harmonic = sample * sample * sample * 0.1f;
                sample += harmonic;
                
                output[i] = sample;
            }
            
            driveLinear.store (currentDrive);
        }
    }

    // ========================================================================
    // Parameters
    // ========================================================================
    
    /** @brief Set drive (0.0 - 1.0) */
    void setDrive (float newDrive)
    {
        targetDriveLinear.store (newDrive);
    }
    
    /** @brief Get current drive */
    float getDrive() const { return driveLinear.load(); }

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    double sampleRate { 44100.0 };
    std::atomic<float> driveLinear { 0.5f };
    std::atomic<float> targetDriveLinear { 0.5f };
};

/**
 * @brief Compressor DSP processor
 * 
 * Implements RMS-based compression with smooth attack and release.
 */
class Compressor
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    Compressor() = default;
    
    // ========================================================================
    // JUCE DSP Processor Interface
    // ========================================================================
    
    /** @brief Prepare processor for playback */
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        attackTime = 0.003f; // 3ms
        releaseTime = 0.1f;  // 100ms
        ratio = 4.0f;
        threshold = -20.0f;
        
        envelopeLevel.store (0.0f);
        gainReduction.store (0.0f);
    }
    
    /** @brief Reset processor state */
    void reset()
    {
        envelopeLevel.store (0.0f);
        gainReduction.store (0.0f);
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
        
        const float attackCoeff = std::exp (-1.0f / (attackTime * sampleRate));
        const float releaseCoeff = std::exp (-1.0f / (releaseTime * sampleRate));
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* input = inputBlock.getChannelPointer (channel);
            auto* output = outputBlock.getChannelPointer (channel);
            
            float envelope = envelopeLevel.load();
            
            for (size_t i = 0; i < numSamples; ++i)
            {
                const float inputSample = input[i];
                const float absInput = std::abs (inputSample);
                
                // Envelope follower
                if (absInput > envelope)
                    envelope = attackCoeff * envelope + (1.0f - attackCoeff) * absInput;
                else
                    envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * absInput;
                
                // Convert to dB
                const float envelopeDB = envelope > 1e-6f ? 20.0f * std::log10 (envelope) : -60.0f;
                
                // Calculate gain reduction
                float gainReductionDB = 0.0f;
                if (envelopeDB > threshold)
                {
                    const float overThreshold = envelopeDB - threshold;
                    gainReductionDB = overThreshold * (1.0f - 1.0f / ratio);
                }
                
                const float gain = juce::Decibels::decibelsToGain (-gainReductionDB);
                
                output[i] = inputSample * gain;
            }
            
            envelopeLevel.store (envelope);
            gainReduction.store (juce::Decibels::gainToDecibels (envelope, -60.0f));
        }
    }

    // ========================================================================
    // Parameters
    // ========================================================================
    
    /** @brief Set compression amount (0.0 - 1.0) */
    void setAmount (float amount)
    {
        ratio = juce::jmap (amount, 1.0f, 20.0f);
        threshold = juce::jmap (amount, -10.0f, -30.0f);
    }

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    double sampleRate { 44100.0 };
    float attackTime { 0.003f };
    float releaseTime { 0.1f };
    float ratio { 4.0f };
    float threshold { -20.0f };
    
    std::atomic<float> envelopeLevel { 0.0f };
    std::atomic<float> gainReduction { 0.0f };
};

/**
 * @brief Reverb DSP processor
 * 
 * Implements high-quality reverb using JUCE's Reverb class.
 */
class Reverb
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    Reverb() = default;
    
    // ========================================================================
    // JUCE DSP Processor Interface
    // ========================================================================
    
    /** @brief Prepare processor for playback */
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        reverb.prepare (*spec);
        
        // Set default reverb parameters
        juce::Reverb::Parameters params;
        params.roomSize = 0.75f;
        params.damping = 0.5f;
        params.wetLevel = 0.3f;
        params.dryLevel = 0.7f;
        params.width = 1.0f;
        params.freezeMode = 0.0f;
        
        reverb.setParameters (params);
    }
    
    /** @brief Reset processor state */
    void reset()
    {
        reverb.reset();
    }
    
    /**
     * @brief Process audio block
     * @param context Processing context
     */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        reverb.process (context);
    }

    // ========================================================================
    // Parameters
    // ========================================================================
    
    /** @brief Set reverb mix (0.0 - 1.0) */
    void setMix (float mix)
    {
        auto params = reverb.getParameters();
        params.wetLevel = mix * 0.5f; // Cap at 0.5 to avoid overwhelming dry signal
        params.dryLevel = 1.0f - mix * 0.3f;
        reverb.setParameters (params);
    }

private:
    // ========================================================================
    // Member Variables
    // ========================================================================
    
    double sampleRate { 44100.0 };
    juce::dsp::Reverb<float> reverb;
};

} // namespace fleen
