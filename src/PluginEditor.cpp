#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "lookandfeel/CustomLookAndFeel.h"
#include "components/SkeuomorphicKnob.h"
#include "components/LedIndicator.h"
#include "components/MeterDisplay.h"
#include "components/OpenGLVisualizer.h"
#include "components/PresetSelector.h"
#include "components/TestKeyboard.h"

namespace fleen
{

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (p)
    , processor (p)
    , customLookAndFeel (new CustomLookAndFeel())
    , gainKnob (new SkeuomorphicKnob ("GAIN"))
    , driveKnob (new SkeuomorphicKnob ("DRIVE"))
    , bassKnob (new SkeuomorphicKnob ("BASS"))
    , midKnob (new SkeuomorphicKnob ("MID"))
    , trebleKnob (new SkeuomorphicKnob ("TREBLE"))
    , presenceKnob (new SkeuomorphicKnob ("PRESENCE"))
    , reverbKnob (new SkeuomorphicKnob ("REVERB"))
    , compressionKnob (new SkeuomorphicKnob ("COMP"))
    , delayKnob (new SkeuomorphicKnob ("DELAY"))
    , chorusKnob (new SkeuomorphicKnob ("CHORUS"))
    , depthKnob (new SkeuomorphicKnob ("DEPTH"))
    , speedKnob (new SkeuomorphicKnob ("SPEED"))
    , powerLed (new LedIndicator (LedIndicator::Color::red))
    , bypassLed (new LedIndicator (LedIndicator::Color::blue))
    , inputMeter (new MeterDisplay())
    , outputMeter (new MeterDisplay())
    , visualizer (new OpenGLVisualizer())
    , presetSelector (new PresetSelector (p))
    , testKeyboard (new TestKeyboard (p))
    , bypassButton ("BYPASS")
{
    setLookAndFeel (customLookAndFeel);
    setSize (1000, 600);
    setResizable (true, true);
    
    setupComponents();
    attachParameters();
    
    startTimer (30);
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
    delete customLookAndFeel;
    delete gainKnob;
    delete driveKnob;
    delete bassKnob;
    delete midKnob;
    delete trebleKnob;
    delete presenceKnob;
    delete reverbKnob;
    delete compressionKnob;
    delete delayKnob;
    delete chorusKnob;
    delete depthKnob;
    delete speedKnob;
    delete powerLed;
    delete bypassLed;
    delete inputMeter;
    delete outputMeter;
    delete visualizer;
    delete presetSelector;
    delete testKeyboard;
}

void PluginEditor::paint (juce::Graphics& g)
{
    g.setRenderQuality (juce::Graphics::highRenderingQuality);
    
    // Premium dark gradient background
    juce::ColourGradient gradient (
        juce::Colour::fromRGB (35, 35, 40),
        0.0f, 0.0f,
        juce::Colour::fromRGB (20, 20, 25),
        0.0f, static_cast<float> (getHeight()),
        false
    );
    g.setGradientFill (gradient);
    g.fillRect (getLocalBounds());
}

void PluginEditor::resized()
{
    const auto bounds = getLocalBounds();
    const int spacing = 16;
    
    // Top bar: Preset selector
    presetSelector->setBounds (bounds.removeFromTop (50).reduced (spacing, 0));
    
    // Main area: 70% controls, 30% visualizer
    const int mainHeight = static_cast<int> (bounds.getHeight() * 0.65);
    auto mainArea = bounds.removeFromTop (mainHeight);
    
    // Left: Controls (61.8%)
    const int controlsWidth = static_cast<int> (mainArea.getWidth() * 0.618);
    mainPanel.setBounds (mainArea.removeFromLeft (controlsWidth).reduced (spacing));
    
    // Right: Visualizer
    sidebar.setBounds (mainArea.reduced (spacing));
    
    // Bottom: Test keyboard
    bottomPanel.setBounds (bounds.reduced (spacing));
    testKeyboard->setBounds (bottomPanel.getLocalBounds());
    
    // Layout controls in grid
    auto controls = mainPanel.getLocalBounds();
    const int knobSize = 90;
    const int rowHeight = knobSize + 40;
    
    // Row 1: Gain, Drive
    auto row1 = controls.removeFromTop (rowHeight);
    gainKnob->setBounds (row1.removeFromLeft (knobSize + 40).withSizeKeepingCentre (knobSize, knobSize));
    driveKnob->setBounds (row1.removeFromLeft (knobSize + 40).withSizeKeepingCentre (knobSize, knobSize));
    
    // Row 2: Bass, Mid, Treble, Presence
    auto row2 = controls.removeFromTop (rowHeight);
    bassKnob->setBounds (row2.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    midKnob->setBounds (row2.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    trebleKnob->setBounds (row2.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    presenceKnob->setBounds (row2.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    
    // Row 3: Reverb, Compression, Delay, Chorus
    auto row3 = controls.removeFromTop (rowHeight);
    reverbKnob->setBounds (row3.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    compressionKnob->setBounds (row3.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    delayKnob->setBounds (row3.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    chorusKnob->setBounds (row3.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    
    // Row 4: Depth, Speed, Bypass
    auto row4 = controls.removeFromTop (rowHeight);
    depthKnob->setBounds (row4.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    speedKnob->setBounds (row4.removeFromLeft (knobSize + 20).withSizeKeepingCentre (knobSize, knobSize));
    
    auto bypassArea = row4.removeFromRight (120);
    bypassButton.setBounds (bypassArea.withSizeKeepingCentre (100, 40));
    
    // Visualizer area
    visualizer->setBounds (sidebar.getLocalBounds().removeFromTop (static_cast<int> (sidebar.getHeight() * 0.6)));
    
    // Meters and LEDs
    auto bottomSidebar = sidebar.getLocalBounds().removeFromBottom (100);
    inputMeter->setBounds (bottomSidebar.removeFromLeft (60));
    bottomSidebar.removeFromLeft (spacing);
    outputMeter->setBounds (bottomSidebar.removeFromLeft (60));
    bottomSidebar.removeFromLeft (spacing);
    powerLed->setBounds (bottomSidebar.removeFromLeft (40).withSizeKeepingCentre (30, 30));
    bottomSidebar.removeFromLeft (spacing);
    bypassLed->setBounds (bottomSidebar.removeFromLeft (40).withSizeKeepingCentre (30, 30));
}

void PluginEditor::setupComponents()
{
    // Add all components
    addAndMakeVisible (mainPanel);
    addAndMakeVisible (sidebar);
    addAndMakeVisible (bottomPanel);
    addAndMakeVisible (presetSelector);
    addAndMakeVisible (testKeyboard);
    addAndMakeVisible (visualizer);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (powerLed);
    addAndMakeVisible (bypassLed);
    addAndMakeVisible (bypassButton);
    
    addAndMakeVisible (gainKnob);
    addAndMakeVisible (driveKnob);
    addAndMakeVisible (bassKnob);
    addAndMakeVisible (midKnob);
    addAndMakeVisible (trebleKnob);
    addAndMakeVisible (presenceKnob);
    addAndMakeVisible (reverbKnob);
    addAndMakeVisible (compressionKnob);
    addAndMakeVisible (delayKnob);
    addAndMakeVisible (chorusKnob);
    addAndMakeVisible (depthKnob);
    addAndMakeVisible (speedKnob);
    
    // Style bypass button
    bypassButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    bypassButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour::fromRGB (0, 150, 200));
    bypassButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    bypassButton.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
    bypassButton.setClickingTogglesState (true);
    
    // Preset selector callbacks
    presetSelector->onPresetChanged = [this] (int index)
    {
        processor.getPresetManager().loadPreset (index);
    };
    
    presetSelector->onPresetSaved = [this] (const juce::String& name)
    {
        processor.getPresetManager().savePreset (name, "User");
        presetSelector->loadPresets();
    };
    
    // Load presets
    presetSelector->loadPresets();
}

void PluginEditor::attachParameters()
{
    gainAttachment = std::make_unique<Attachment> (processor.getParameters(), "gain", gainKnob->getSlider());
    driveAttachment = std::make_unique<Attachment> (processor.getParameters(), "drive", driveKnob->getSlider());
    bassAttachment = std::make_unique<Attachment> (processor.getParameters(), "bass", bassKnob->getSlider());
    midAttachment = std::make_unique<Attachment> (processor.getParameters(), "mid", midKnob->getSlider());
    trebleAttachment = std::make_unique<Attachment> (processor.getParameters(), "treble", trebleKnob->getSlider());
    presenceAttachment = std::make_unique<Attachment> (processor.getParameters(), "presence", presenceKnob->getSlider());
    reverbAttachment = std::make_unique<Attachment> (processor.getParameters(), "reverb", reverbKnob->getSlider());
    compressionAttachment = std::make_unique<Attachment> (processor.getParameters(), "compression", compressionKnob->getSlider());
    delayAttachment = std::make_unique<Attachment> (processor.getParameters(), "delay", delayKnob->getSlider());
    chorusAttachment = std::make_unique<Attachment> (processor.getParameters(), "chorus", chorusKnob->getSlider());
    depthAttachment = std::make_unique<Attachment> (processor.getParameters(), "depth", depthKnob->getSlider());
    speedAttachment = std::make_unique<Attachment> (processor.getParameters(), "speed", speedKnob->getSlider());
    bypassAttachment = std::make_unique<ButtonAttachment> (processor.getParameters(), "bypass", bypassButton);
}

void PluginEditor::timerCallback()
{
    inputMeter->setLevel (processor.getInputLevel());
    outputMeter->setLevel (processor.getOutputLevel());
    visualizer->updateDisplay();
    repaint();
}

} // namespace fleen
