#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "state/StateManager.h"
#include "state/PresetManager.h"

namespace fleen
{

// ============================================================================
// Construction
// ============================================================================

PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , parameters (*this, nullptr, juce::Identifier ("FleenElGuitar"), createParameterLayout())
    , presetManager (*this)
{
    // Attach parameter listeners
    parameters.addParameterListener ("gain", this);
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("bass", this);
    parameters.addParameterListener ("mid", this);
    parameters.addParameterListener ("treble", this);
    parameters.addParameterListener ("presence", this);
    parameters.addParameterListener ("reverb", this);
    parameters.addParameterListener ("compression", this);
    parameters.addParameterListener ("delay", this);
    parameters.addParameterListener ("chorus", this);
    parameters.addParameterListener ("bypass", this);
    
    // Load factory presets
    presetManager.loadFactoryPresets();
}

PluginProcessor::~PluginProcessor()
{
    releaseResources();
}

// ============================================================================
// AudioProcessor Interface
// ============================================================================

void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    prepareDSP();
    resetDSP();
    isPrepared = true;
}

void PluginProcessor::releaseResources()
{
    isPrepared = false;
    dspChain.reset();
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (bypassParam.load())
    {
        inputLevel = 0.0f;
        outputLevel = 0.0f;
        return;
    }
    
    // Calculate input level
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
    
    // Process audio
    if (isPrepared)
    {
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        
        // Update DSP parameters
        auto& inputGain = dspChain.get<0>();
        inputGain.setGainDecibels (juce::Decibels::gainToDecibels (gainParam.load()));
        
        auto& distortion = dspChain.get<1>();
        auto& compressor = dspChain.get<2>();
        auto& reverb = dspChain.get<3>();
        
        dspChain.process (context);
    }
    
    // Calculate output level
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
    if (xml != nullptr)
    {
        auto xmlString = xml->toString();
        destData.setSize (xmlString.getNumBytesAsUTF8());
        destData.copyFrom (xmlString.toUTF8(), 0, destData.getSize());
    }
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
// Parameter Handling
// ============================================================================

void PluginProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "gain") gainParam = newValue;
    else if (parameterID == "drive") driveParam = newValue;
    else if (parameterID == "bass") bassParam = newValue;
    else if (parameterID == "mid") midParam = newValue;
    else if (parameterID == "treble") trebleParam = newValue;
    else if (parameterID == "presence") presenceParam = newValue;
    else if (parameterID == "reverb") reverbParam = newValue;
    else if (parameterID == "compression") compressionParam = newValue;
    else if (parameterID == "delay") delayParam = newValue;
    else if (parameterID == "chorus") chorusParam = newValue;
    else if (parameterID == "bypass") bypassParam = newValue > 0.5f;
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    auto makeParam = [] (const juce::String& id, const juce::String& name, 
                         float defaultVal, float min = 0.0f, float max = 1.0f)
    {
        return std::make_unique<juce::AudioParameterFloat> (
            id, name,
            juce::NormalisableRange<float> (min, max, 0.01f),
            defaultVal,
            name,
            juce::AudioProcessorParameter::genericParameter,
            [] (float value, int maxLength) { return juce::String (value * 100.0f, 1).substring (0, maxLength); },
            [] (const juce::String& text) { return text.getFloatValue() / 100.0f; }
        );
    };
    
    // Core tone controls
    layout.add (makeParam ("gain", "Gain", 0.5f));
    layout.add (makeParam ("drive", "Drive", 0.3f));
    layout.add (makeParam ("bass", "Bass", 0.5f));
    layout.add (makeParam ("mid", "Mid", 0.5f));
    layout.add (makeParam ("treble", "Treble", 0.5f));
    layout.add (makeParam ("presence", "Presence", 0.5f));
    
    // Effects
    layout.add (makeParam ("reverb", "Reverb", 0.3f));
    layout.add (makeParam ("compression", "Compression", 0.4f));
    layout.add (makeParam ("delay", "Delay", 0.2f));
    layout.add (makeParam ("chorus", "Chorus", 0.2f));
    
    // Utility
    layout.add (std::make_unique<juce::AudioParameterBool> ("bypass", "Bypass", false));
    
    return layout;
}

// ============================================================================
// DSP
// ============================================================================

void PluginProcessor::prepareDSP()
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = currentSampleRate;
    spec.maximumBlockSize = 1024;
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    
    dspChain.prepare (spec);
}

void PluginProcessor::resetDSP()
{
    dspChain.reset();
}

} // namespace fleen
