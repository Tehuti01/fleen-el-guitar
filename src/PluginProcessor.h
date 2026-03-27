#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "state/StateManager.h"
#include "state/PresetManager.h"

namespace fleen
{

/**
 * @brief Fleen El-Guitar Audio Processor
 * 
 * Professional guitar processing plugin with:
 * - 8 premium factory presets
 * - A/B/C/D preset switching
 * - Custom preset save/load
 * - Premium DSP chain
 * - MIDI keyboard support
 */
class PluginProcessor : public juce::AudioProcessor
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
    bool acceptsMidi() const override { return true; }
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
    
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    const juce::AudioProcessorValueTreeState& getParameters() const { return parameters; }
    
    StateManager& getStateManager() { return stateManager; }
    PresetManager& getPresetManager() { return presetManager; }
    const PresetManager& getPresetManager() const { return presetManager; }
    
    // Real-time parameter access
    float getGain() const { return gainParam.load(); }
    float getDrive() const { return driveParam.load(); }
    float getBass() const { return bassParam.load(); }
    float getMid() const { return midParam.load(); }
    float getTreble() const { return trebleParam.load(); }
    float getPresence() const { return presenceParam.load(); }
    float getReverb() const { return reverbParam.load(); }
    float getCompression() const { return compressionParam.load(); }
    float getDelay() const { return delayParam.load(); }
    float getChorus() const { return chorusParam.load(); }
    bool getBypass() const { return bypassParam.load(); }
    
    // Level meters
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }

private:
    // ========================================================================
    // Parameter Handling
    // ========================================================================
    
    void parameterChanged (const juce::String& parameterID, float newValue);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // ========================================================================
    // DSP
    // ========================================================================
    
    void prepareDSP();
    void resetDSP();

    // ========================================================================
    // Member Variables
    // ========================================================================
    
    juce::AudioProcessorValueTreeState parameters;
    StateManager stateManager;
    PresetManager presetManager;
    
    // Atomic parameters for real-time access
    std::atomic<float> gainParam { 0.5f };
    std::atomic<float> driveParam { 0.3f };
    std::atomic<float> bassParam { 0.5f };
    std::atomic<float> midParam { 0.5f };
    std::atomic<float> trebleParam { 0.5f };
    std::atomic<float> presenceParam { 0.5f };
    std::atomic<float> reverbParam { 0.3f };
    std::atomic<float> compressionParam { 0.4f };
    std::atomic<float> delayParam { 0.2f };
    std::atomic<float> chorusParam { 0.2f };
    std::atomic<bool> bypassParam { false };
    
    // DSP Chain - simplified for JUCE 8
    juce::dsp::ProcessorChain<
        juce::dsp::Gain<float>,           // Input gain
        juce::dsp::WaveShaper<float>,     // Distortion
        juce::dsp::Compressor<float>,     // Compression
        juce::dsp::Reverb                 // Reverb
    > dspChain;
    
    bool isPrepared { false };
    double currentSampleRate { 44100.0 };
    
    // Level meters
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

} // namespace fleen
