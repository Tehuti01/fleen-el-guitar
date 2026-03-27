#include "PluginEditor.h"

namespace fleen
{

// ============================================================================
// Construction / Destruction
// ============================================================================

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (p)
    , processor (p)
    , gainKnob ("GAIN")
    , driveKnob ("DRIVE")
    , bassKnob ("BASS")
    , midKnob ("MID")
    , trebleKnob ("TREBLE")
    , presenceKnob ("PRESENCE")
    , reverbKnob ("REVERB")
    , compressionKnob ("COMP")
    , powerLed (LedIndicator::Color::red)
    , bypassLed (LedIndicator::Color::blue)
    , bypassButton ("BYPASS")
{
    // Set custom look and feel
    setLookAndFeel (&customLookAndFeel);
    
    // Setup size based on Golden Ratio
    constexpr int baseWidth = 800;
    constexpr int baseHeight = 500;
    setSize (baseWidth, baseHeight);
    setResizable (true, true);
    setResizeLimits (600, 400, 1600, 1000);
    
    // Setup all components
    setupComponents();
    attachParameters();
    setupOpenGL();
    
    // Start timer for meter updates
    startTimer (meterUpdateInterval);
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

// ============================================================================
// Component Interface
// ============================================================================

void PluginEditor::paint (juce::Graphics& g)
{
    // Draw premium dark background with subtle gradient
    juce::ColourGradient gradient (
        juce::Colour::fromRGB (30, 30, 35),
        0.0f, 0.0f,
        juce::Colour::fromRGB (20, 20, 25),
        0.0f, static_cast<float> (getHeight()),
        false
    );
    g.setGradientFill (gradient);
    g.fillRect (getLocalBounds());
    
    // Draw subtle brushed metal texture overlay
    g.setColour (juce::Colour::fromRGBA (255, 255, 255, 3));
    g.fillRect (getLocalBounds());
}

void PluginEditor::resized()
{
    const auto bounds = getLocalBounds();
    const auto width = bounds.getWidth();
    const auto height = bounds.getHeight();

    // Golden Ratio layout: 61.8% main panel, 38.2% sidebar
    const float mainPanelRatio = 0.618f;
    const int mainPanelWidth = static_cast<int> (width * mainPanelRatio);
    const int sidebarWidth = width - mainPanelWidth;

    // Main Panel (left side - controls)
    auto mainBounds = bounds;
    mainPanel.setBounds (mainBounds.removeFromLeft (mainPanelWidth));

    // Sidebar (right side - visualizer and meters)
    sidebar.setBounds (mainBounds);

    // Layout main panel with Golden Ratio spacing
    const int spacing = getSpacing (2); // Base spacing tier
    const int topMargin = getSpacing (3);
    const int bottomMargin = getSpacing (3);
    const int leftMargin = spacing;
    const int rightMargin = spacing;

    auto mainContent = mainPanel.getLocalBounds()
        .reduced (leftMargin, topMargin)
        .translated (0, 0);
    mainContent.setHeight (mainContent.getHeight() - bottomMargin);
    mainContent.setWidth (mainContent.getWidth() - rightMargin);
    
    // Top row: Gain and Drive (primary controls)
    const int knobSize = juce::jmin (120, mainContent.getWidth() / 4);
    const int knobRowHeight = knobSize + getSpacing (4);
    auto topRow = mainContent.removeFromTop (knobRowHeight);
    
    gainKnob.setBounds (topRow.removeFromLeft (knobSize).withSizeKeepingCentre (knobSize, knobSize));
    topRow.removeFromLeft (spacing);
    driveKnob.setBounds (topRow.removeFromLeft (knobSize).withSizeKeepingCentre (knobSize, knobSize));
    
    // Middle section: Tone stack (Bass, Mid, Treble, Presence)
    const int toneKnobSize = juce::jmin (90, mainContent.getWidth() / 5);
    const int toneRowHeight = toneKnobSize + getSpacing (4);
    auto toneRow = mainContent.removeFromTop (toneRowHeight);
    
    bassKnob.setBounds (toneRow.removeFromLeft (toneKnobSize).withSizeKeepingCentre (toneKnobSize, toneKnobSize));
    toneRow.removeFromLeft (spacing / 2);
    midKnob.setBounds (toneRow.removeFromLeft (toneKnobSize).withSizeKeepingCentre (toneKnobSize, toneKnobSize));
    toneRow.removeFromLeft (spacing / 2);
    trebleKnob.setBounds (toneRow.removeFromLeft (toneKnobSize).withSizeKeepingCentre (toneKnobSize, toneKnobSize));
    toneRow.removeFromLeft (spacing / 2);
    presenceKnob.setBounds (toneRow.removeFromLeft (toneKnobSize).withSizeKeepingCentre (toneKnobSize, toneKnobSize));
    
    // Bottom row: Reverb, Compression, and Bypass
    const int smallKnobSize = juce::jmin (80, mainContent.getWidth() / 4);
    auto bottomRow = mainContent.removeFromTop (smallKnobSize + getSpacing (3));
    
    reverbKnob.setBounds (bottomRow.removeFromLeft (smallKnobSize).withSizeKeepingCentre (smallKnobSize, smallKnobSize));
    bottomRow.removeFromLeft (spacing);
    compressionKnob.setBounds (bottomRow.removeFromLeft (smallKnobSize).withSizeKeepingCentre (smallKnobSize, smallKnobSize));
    
    // Bypass button
    const int buttonWidth = 100;
    const int buttonHeight = 40;
    auto buttonArea = bottomRow.removeFromRight (buttonWidth).withSizeKeepingCentre (buttonWidth, buttonHeight);
    bypassButton.setBounds (buttonArea);
    
    // Power LED indicator
    powerLed.setBounds (mainContent.removeFromBottom (30).removeFromLeft (30));
    
    // Layout sidebar
    auto sidebarContent = sidebar.getLocalBounds().reduced (spacing);
    
    // Preset selector at top
    const int presetHeight = 40;
    presetBox.setBounds (sidebarContent.removeFromTop (presetHeight));
    
    sidebarContent.removeFromTop (spacing);
    
    // Visualizer (main area)
    const int visualizerHeight = static_cast<int> (sidebarContent.getHeight() * 0.6f);
    visualizer.setBounds (sidebarContent.removeFromTop (visualizerHeight));
    
    sidebarContent.removeFromTop (spacing);
    
    // Meters at bottom
    auto meterArea = sidebarContent;
    const int meterWidth = meterArea.getWidth() / 3;
    inputMeter.setBounds (meterArea.removeFromLeft (meterWidth));
    meterArea.removeFromLeft (spacing / 2);
    outputMeter.setBounds (meterArea.removeFromLeft (meterWidth));
    meterArea.removeFromLeft (spacing / 2);
    bypassLed.setBounds (meterArea.removeFromLeft (30).withSizeKeepingCentre (30, 30));
}

void PluginEditor::visibilityChanged()
{
    // Refresh preset list when editor becomes visible
    if (isVisible())
    {
        presetBox.clear();
        const auto& presets = processor.getPresetManager().getPresetList();
        for (int i = 0; i < presets.size(); ++i)
        {
            presetBox.addItem (presets[i].name, i + 1);
        }
        presetBox.setSelectedId (processor.getPresetManager().getCurrentPresetIndex() + 1);
    }
}

// ============================================================================
// Timer Interface
// ============================================================================

void PluginEditor::timerCallback()
{
    // Update meters at 60 FPS
    inputMeter.setLevel (processor.getInputLevel());
    outputMeter.setLevel (processor.getOutputLevel());
    
    // Update visualizer
    visualizer.updateDisplay();
    
    // Trigger repaint for smooth animations
    repaint();
}

// ============================================================================
// Layout Helpers
// ============================================================================

void PluginEditor::setupComponents()
{
    // Add main components
    addAndMakeVisible (mainPanel);
    addAndMakeVisible (sidebar);
    
    // Add knobs to main panel
    mainPanel.addAndMakeVisible (gainKnob);
    mainPanel.addAndMakeVisible (driveKnob);
    mainPanel.addAndMakeVisible (bassKnob);
    mainPanel.addAndMakeVisible (midKnob);
    mainPanel.addAndMakeVisible (trebleKnob);
    mainPanel.addAndMakeVisible (presenceKnob);
    mainPanel.addAndMakeVisible (reverbKnob);
    mainPanel.addAndMakeVisible (compressionKnob);
    
    // Add LEDs
    mainPanel.addAndMakeVisible (powerLed);
    sidebar.addAndMakeVisible (bypassLed);
    
    // Add meters
    sidebar.addAndMakeVisible (inputMeter);
    sidebar.addAndMakeVisible (outputMeter);
    
    // Add visualizer
    sidebar.addAndMakeVisible (visualizer);
    
    // Add bypass button
    bypassButton.setColour (juce::TextButton::buttonColourId, juce::Colour::fromRGB (60, 60, 70));
    bypassButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour::fromRGB (0, 150, 200));
    bypassButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    bypassButton.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
    bypassButton.setClickingTogglesState (true);
    mainPanel.addAndMakeVisible (bypassButton);
    
    // Add preset combo box
    presetBox.setColour (juce::ComboBox::backgroundColourId, juce::Colour::fromRGB (40, 40, 45));
    presetBox.setColour (juce::ComboBox::textColourId, juce::Colours::white);
    presetBox.setColour (juce::ComboBox::outlineColourId, juce::Colour::fromRGB (80, 80, 90));
    sidebar.addAndMakeVisible (presetBox);
    
    // Bypass button listener
    bypassButton.onClick = [this]
    {
        processor.getParameters().getParameter ("bypass")->setValueNotifyingHost (bypassButton.getToggleState() ? 1.0f : 0.0f);
    };
    
    // Preset box listener
    presetBox.onChange = [this]
    {
        const int index = presetBox.getSelectedId() - 1;
        if (index >= 0 && index < processor.getPresetManager().getPresetList().size())
        {
            processor.getPresetManager().loadPreset (index);
        }
    };
}

void PluginEditor::attachParameters()
{
    // Attach knobs to parameters
    gainAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "gain", gainKnob.getSlider());
    driveAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "drive", driveKnob.getSlider());
    bassAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "bass", bassKnob.getSlider());
    midAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "mid", midKnob.getSlider());
    trebleAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "treble", trebleKnob.getSlider());
    presenceAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "presence", presenceKnob.getSlider());
    reverbAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "reverb", reverbKnob.getSlider());
    compressionAttachment = std::make_unique<Attachment> (
        processor.getParameters(), "compression", compressionKnob.getSlider());
}

void PluginEditor::setupOpenGL()
{
    // Initialize OpenGL context for hardware-accelerated rendering
    visualizer.setProcessor (&processor);
}

} // namespace fleen
