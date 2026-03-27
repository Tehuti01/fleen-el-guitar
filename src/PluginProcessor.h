#pragma once

#include <JuceHeader.h>
#include "dsp/GainStage.h"
#include "dsp/ToneStack.h"
#include "dsp/Distortion.h"
#include "dsp/Reverb.h"
#include "dsp/Compressor.h"
#include "state/StateManager.h"
#include "state/PresetManager.h"

namespace fleen
{

/**
 * @brief Fleen El-Guitar Audio Processor
 * 
 * Professional guitar processing plugin with premium skeuomorphic interface.
 * Implements lock-free, real-time safe DSP processing chain.
 */
class PluginProcessor : public juce::AudioProcessor,
                        private juce::AudioProcessorValueTreeState::Listener
{
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================
    
    PluginProcessor();
    ~PluginProcessor() override;

    // ========================================================================
    // AudioProcessor Interface
    // ========================================================================
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // ========================================================================
    // State Management
    // ========================================================================
    
    /** @brief Get the APVTS instance for parameter binding */
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    const juce::AudioProcessorValueTreeState& getParameters() const { return parameters; }
    
    /** @brief Get the state manager for thread-safe updates */
    StateManager& getStateManager() { return stateManager; }
    
    /** @brief Get the preset manager */
    PresetManager& getPresetManager() { return presetManager; }
    const PresetManager& getPresetManager() const { return presetManager; }
    
    /** @brief Get current gain parameter (thread-safe) */
    float getGain() const { return gain.load(); }
    
    /** @brief Get current drive parameter (thread-safe) */
    float getDrive() const { return drive.load(); }

private:
    // ========================================================================
    // APVTS Listener
    // ========================================================================
    
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    // ========================================================================
    // Parameter Layout
    // ========================================================================
    
    /** @brief Create the parameter layout for APVTS */
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // ========================================================================
    // DSP Chain
    // ========================================================================
    
    /** @brief Prepare DSP chain for playback */
    void prepareDSP();
    
    /** @brief Reset DSP chain */
    void resetDSP();

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    // State Management
    juce::AudioProcessorValueTreeState parameters;
    StateManager stateManager;
    PresetManager presetManager;
    
    // Atomic parameters for real-time access
    std::atomic<float> gain { 0.5f };
    std::atomic<float> drive { 0.5f };
    std::atomic<float> bass { 0.5f };
    std::atomic<float> mid { 0.5f };
    std::atomic<float> treble { 0.5f };
    std::atomic<float> presence { 0.5f };
    std::atomic<float> reverbMix { 0.3f };
    std::atomic<float> compression { 0.5f };
    std::atomic<bool> bypass { false };
    
    // DSP Chain
    juce::dsp::ProcessorChain<
        juce::dsp::GainChain<juce::dsp::Gain<float>>,
        GainStage,
        ToneStack,
        Distortion,
        Compressor,
        Reverb,
        juce::dsp::DryWetMix<float>
    > dspChain;
    
    // Processing state
    bool isPrepared { false };
    double currentSampleRate { 44100.0 };
    int currentBlockSize { 512 };
    
    // Level meters (atomic for thread-safe access)
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

} // namespace fleen
