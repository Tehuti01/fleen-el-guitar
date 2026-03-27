#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , parameters (*this, nullptr, juce::Identifier ("FleenElGuitar"), createParameterLayout())
    , presetManager (*this)
{
    // Attach listeners to parameters
    parameters.addParameterListener ("gain", this);
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("bass", this);
    parameters.addParameterListener ("mid", this);
    parameters.addParameterListener ("treble", this);
    parameters.addParameterListener ("presence", this);
    parameters.addParameterListener ("reverb", this);
    parameters.addParameterListener ("compression", this);
    parameters.addParameterListener ("bypass", this);
    
    // Initialize preset manager with factory presets
    presetManager.loadFactoryPresets();
}

PluginProcessor::~PluginProcessor()
{
    // Ensure DSP is released
    releaseResources();
}

// ============================================================================
// AudioProcessor Interface
// ============================================================================

void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    prepareDSP();
    resetDSP();
    
    isPrepared = true;
}

void PluginProcessor::releaseResources()
{
    isPrepared = false;
    
    // Reset DSP chain
    dspChain.reset();
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear output channels if needed
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Bypass processing
    if (bypass.load())
    {
        inputLevel = 0.0f;
        outputLevel = 0.0f;
        return;
    }
    
    // Calculate input level (RMS)
    {
        float sum = 0.0f;
        const auto numSamples = buffer.getNumSamples();
        for (int channel = 0; channel < juce::jmin (2, totalNumInputChannels); ++channel)
        {
            auto* channelData = buffer.getReadPointer (channel);
            for (int i = 0; i < numSamples; ++i)
            {
                const float sample = channelData[i];
                sum += sample * sample;
            }
        }
        const float rms = juce::Decibels::gainToDecibels (std::sqrt (sum / (buffer.getNumChannels() * numSamples)), -100.0f);
        inputLevel = juce::jmap (rms, -60.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // Process through DSP chain
    if (isPrepared)
    {
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        
        // Update DSP parameters from atomics
        auto& gainStage = dspChain.get<0>();
        gainStage.setGain (gain.load());
        
        auto& toneStack = dspChain.get<1>();
        toneStack.setBass (bass.load());
        toneStack.setMid (mid.load());
        toneStack.setTreble (treble.load());
        toneStack.setPresence (presence.load());
        
        auto& distortion = dspChain.get<2>();
        distortion.setDrive (drive.load());
        
        auto& compressor = dspChain.get<3>();
        compressor.setAmount (compression.load());
        
        auto& reverb = dspChain.get<4>();
        reverb.setMix (reverbMix.load());
        
        // Process
        dspChain.process (context);
    }
    
    // Calculate output level (RMS)
    {
        float sum = 0.0f;
        const auto numSamples = buffer.getNumSamples();
        for (int channel = 0; channel < juce::jmin (2, totalNumOutputChannels); ++channel)
        {
            auto* channelData = buffer.getReadPointer (channel);
            for (int i = 0; i < numSamples; ++i)
            {
                const float sample = channelData[i];
                sum += sample * sample;
            }
        }
        const float rms = juce::Decibels::gainToDecibels (std::sqrt (sum / (buffer.getNumChannels() * numSamples)), -100.0f);
        outputLevel = juce::jmap (rms, -60.0f, 0.0f, 0.0f, 1.0f);
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    destData.reset();
    destData.appendAll (xml->createDocument().toRawXML());
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::String xmlString = juce::String::fromUTF8 (static_cast<const char*> (data), sizeInBytes);
    
    if (auto xml = juce::XmlDocument::parse (xmlString))
    {
        auto state = juce::ValueTree::fromXml (*xml);
        parameters.replaceState (state);
    }
}

// ============================================================================
// APVTS Listener
// ============================================================================

void PluginProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    // Update atomic parameters for real-time access
    if (parameterID == "gain")
        gain = newValue;
    else if (parameterID == "drive")
        drive = newValue;
    else if (parameterID == "bass")
        bass = newValue;
    else if (parameterID == "mid")
        mid = newValue;
    else if (parameterID == "treble")
        treble = newValue;
    else if (parameterID == "presence")
        presence = newValue;
    else if (parameterID == "reverb")
        reverbMix = newValue;
    else if (parameterID == "compression")
        compression = newValue;
    else if (parameterID == "bypass")
        bypass = newValue > 0.5f;
}

// ============================================================================
// Parameter Layout
// ============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Gain Stage
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "gain", "Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Gain",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Drive / Distortion
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "drive", "Drive",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Drive",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Tone Stack - Bass
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "bass", "Bass",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Bass",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Tone Stack - Mid
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "mid", "Mid",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Mid",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Tone Stack - Treble
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "treble", "Treble",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Treble",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Presence
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "presence", "Presence",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Presence",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Reverb Mix
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "reverb", "Reverb",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.3f,
        "Reverb",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Compression
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "compression", "Compression",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        "Compression",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
        [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
    ));
    
    // Bypass
    layout.add (std::make_unique<juce::AudioParameterBool> (
        "bypass", "Bypass",
        false
    ));
    
    return layout;
}

// ============================================================================
// DSP Chain
// ============================================================================

void PluginProcessor::prepareDSP()
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = currentSampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (currentBlockSize);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    
    dspChain.prepare (spec);
}

void PluginProcessor::resetDSP()
{
    dspChain.reset();
}

} // namespace fleen
