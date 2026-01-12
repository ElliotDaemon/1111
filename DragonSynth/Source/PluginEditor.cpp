#include "PluginProcessor.h"
#include "PluginEditor.h"

DragonSynthAudioProcessorEditor::DragonSynthAudioProcessorEditor(DragonSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), keyboardState(p.getKeyboardState())
{
    setLookAndFeel(&dragonLookAndFeel);
    
    // Setup keyboard - use processor's keyboard state
    keyboardComponent = std::make_unique<MarbleKeyboardComponent>(keyboardState);
    addAndMakeVisible(*keyboardComponent);
    
    // Waveform display
    addAndMakeVisible(waveformDisplay);
    
    // Title
    titleLabel.setText("DRAGON SYNTH", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(28.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonIce());
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // OSC 1 Controls
    osc1Label.setText("OSC 1", juce::dontSendNotification);
    osc1Label.setFont(juce::Font(14.0f, juce::Font::bold));
    osc1Label.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonFire());
    osc1Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(osc1Label);
    
    setupComboBox(osc1WaveSelector, {"Sine", "Saw", "Square", "Triangle", "Noise"});
    osc1WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "osc1Wave", osc1WaveSelector);
    
    setupSlider(osc1GainSlider, osc1GainLabel, "Gain");
    osc1GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "osc1Gain", osc1GainSlider);
    
    // OSC 2 Controls
    osc2Label.setText("OSC 2", juce::dontSendNotification);
    osc2Label.setFont(juce::Font(14.0f, juce::Font::bold));
    osc2Label.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonFire());
    osc2Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(osc2Label);
    
    setupComboBox(osc2WaveSelector, {"Sine", "Saw", "Square", "Triangle", "Noise"});
    osc2WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "osc2Wave", osc2WaveSelector);
    
    setupSlider(osc2GainSlider, osc2GainLabel, "Gain");
    osc2GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "osc2Gain", osc2GainSlider);
    
    setupSlider(osc2DetuneSlider, osc2DetuneLabel, "Detune");
    osc2DetuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "osc2Detune", osc2DetuneSlider);
    
    setupSlider(osc2OctaveSlider, osc2OctaveLabel, "Octave");
    osc2OctaveSlider.setRange(-2, 2, 1);
    osc2OctaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "osc2Octave", osc2OctaveSlider);
    
    // Filter Controls
    filterLabel.setText("FILTER", juce::dontSendNotification);
    filterLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    filterLabel.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonIce());
    filterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterLabel);
    
    setupComboBox(filterTypeSelector, {"Low Pass", "High Pass", "Band Pass"});
    filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "filterType", filterTypeSelector);
    
    setupSlider(filterCutoffSlider, filterCutoffLabel, "Cutoff");
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterCutoff", filterCutoffSlider);
    
    setupSlider(filterResonanceSlider, filterResonanceLabel, "Res");
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterResonance", filterResonanceSlider);
    
    setupSlider(filterEnvAmountSlider, filterEnvAmountLabel, "Env");
    filterEnvAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterEnvAmount", filterEnvAmountSlider);
    
    // Amp Envelope Controls
    ampEnvLabel.setText("AMP ENV", juce::dontSendNotification);
    ampEnvLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    ampEnvLabel.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonFireGlow());
    ampEnvLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ampEnvLabel);
    
    setupSlider(ampAttackSlider, ampAttackLabel, "A");
    ampAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampAttack", ampAttackSlider);
    
    setupSlider(ampDecaySlider, ampDecayLabel, "D");
    ampDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampDecay", ampDecaySlider);
    
    setupSlider(ampSustainSlider, ampSustainLabel, "S");
    ampSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampSustain", ampSustainSlider);
    
    setupSlider(ampReleaseSlider, ampReleaseLabel, "R");
    ampReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampRelease", ampReleaseSlider);
    
    // Filter Envelope Controls
    filterEnvLabel.setText("FILTER ENV", juce::dontSendNotification);
    filterEnvLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    filterEnvLabel.setColour(juce::Label::textColourId, DragonLookAndFeel::getDragonIceGlow());
    filterEnvLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterEnvLabel);
    
    setupSlider(filterAttackSlider, filterAttackLabel, "A");
    filterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterAttack", filterAttackSlider);
    
    setupSlider(filterDecaySlider, filterDecayLabel, "D");
    filterDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterDecay", filterDecaySlider);
    
    setupSlider(filterSustainSlider, filterSustainLabel, "S");
    filterSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterSustain", filterSustainSlider);
    
    setupSlider(filterReleaseSlider, filterReleaseLabel, "R");
    filterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterRelease", filterReleaseSlider);
    
    // Master Controls
    masterGainLabel.setText("MASTER", juce::dontSendNotification);
    masterGainLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    masterGainLabel.setColour(juce::Label::textColourId, DragonLookAndFeel::getTextLight());
    masterGainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(masterGainLabel);
    
    setupSlider(masterGainSlider, masterGainLabel, "");
    masterGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    masterGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "masterGain", masterGainSlider);
    
    // Update waveform display based on oscillator selection
    osc1WaveSelector.onChange = [this]() {
        waveformDisplay.setWaveformType(osc1WaveSelector.getSelectedItemIndex());
    };
    
    setSize(900, 700);
    startTimerHz(30);
}

DragonSynthAudioProcessorEditor::~DragonSynthAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void DragonSynthAudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    addAndMakeVisible(slider);
    
    label.setText(labelText, juce::dontSendNotification);
    label.setFont(juce::Font(11.0f));
    label.setColour(juce::Label::textColourId, DragonLookAndFeel::getTextDim());
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void DragonSynthAudioProcessorEditor::setupComboBox(juce::ComboBox& box, const juce::StringArray& items)
{
    for (int i = 0; i < items.size(); ++i)
        box.addItem(items[i], i + 1);
    box.setSelectedItemIndex(0);
    addAndMakeVisible(box);
}

void DragonSynthAudioProcessorEditor::drawSectionPanel(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& /*title*/)
{
    DragonLookAndFeel::drawDragonGlassPanel(g, bounds.toFloat(), 10);
}

void DragonSynthAudioProcessorEditor::timerCallback()
{
    // Update waveform display with audio data
    if (audioProcessor.waveformReady.exchange(false))
    {
        waveformDisplay.setWaveformData(audioProcessor.waveformData);
    }
}

void DragonSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Main background gradient
    juce::ColourGradient bgGradient(DragonLookAndFeel::getDarkBackground(), 0, 0,
                                     DragonLookAndFeel::getDarkBackground().darker(0.3f), 0, static_cast<float>(getHeight()), false);
    bgGradient.addColour(0.5, DragonLookAndFeel::getDragonGlass().darker(0.5f));
    g.setGradientFill(bgGradient);
    g.fillAll();
    
    // Draw subtle dragon scale pattern
    g.setColour(DragonLookAndFeel::getDragonGlassLight().withAlpha(0.03f));
    for (int y = 0; y < getHeight(); y += 30)
    {
        for (int x = (y / 30 % 2) * 15; x < getWidth(); x += 30)
        {
            juce::Path hexagon;
            float cx = static_cast<float>(x);
            float cy = static_cast<float>(y);
            float size = 15.0f;
            
            hexagon.startNewSubPath(cx + size, cy);
            for (int i = 1; i < 6; ++i)
            {
                float angle = i * juce::MathConstants<float>::pi / 3.0f;
                hexagon.lineTo(cx + size * std::cos(angle), cy + size * std::sin(angle));
            }
            hexagon.closeSubPath();
            g.strokePath(hexagon, juce::PathStrokeType(0.5f));
        }
    }
    
    // Section panels
    auto bounds = getLocalBounds().reduced(10);
    
    // Top section (title and waveform)
    auto topSection = bounds.removeFromTop(100);
    
    // Control sections
    auto controlSection = bounds.removeFromTop(280);
    auto oscSection = controlSection.removeFromLeft(300);
    auto filterSection = controlSection.removeFromLeft(200);
    auto envSection = controlSection;
    
    // Draw panels
    drawSectionPanel(g, oscSection.reduced(5), "OSCILLATORS");
    drawSectionPanel(g, filterSection.reduced(5), "FILTER");
    drawSectionPanel(g, envSection.reduced(5), "ENVELOPES");
    
    // Master panel
    auto masterBounds = bounds.removeFromRight(80);
    bounds.removeFromTop(10);
    drawSectionPanel(g, masterBounds.removeFromTop(200).reduced(5), "MASTER");
    
    // Keyboard section
    bounds.removeFromTop(20);
    drawSectionPanel(g, bounds.reduced(5), "KEYBOARD");
    
    // Draw decorative dragon elements
    g.setColour(DragonLookAndFeel::getDragonFire().withAlpha(0.1f));
    
    // Fire accent line under title
    auto titleBounds = getLocalBounds().reduced(10).removeFromTop(50);
    juce::ColourGradient fireGradient(DragonLookAndFeel::getDragonFire().withAlpha(0.0f), 
                                       titleBounds.getX(), titleBounds.getBottom(),
                                       DragonLookAndFeel::getDragonFire().withAlpha(0.5f), 
                                       titleBounds.getCentreX(), titleBounds.getBottom(), false);
    fireGradient.addColour(0.5, DragonLookAndFeel::getDragonFireGlow().withAlpha(0.8f));
    g.setGradientFill(fireGradient);
    g.fillRect(titleBounds.getX(), titleBounds.getBottom() - 2, titleBounds.getWidth() / 2, 2);
    
    juce::ColourGradient fireGradient2(DragonLookAndFeel::getDragonFire().withAlpha(0.5f), 
                                        titleBounds.getCentreX(), titleBounds.getBottom(),
                                        DragonLookAndFeel::getDragonFire().withAlpha(0.0f), 
                                        titleBounds.getRight(), titleBounds.getBottom(), false);
    fireGradient2.addColour(0.5, DragonLookAndFeel::getDragonFireGlow().withAlpha(0.8f));
    g.setGradientFill(fireGradient2);
    g.fillRect(titleBounds.getCentreX(), titleBounds.getBottom() - 2, titleBounds.getWidth() / 2, 2);
}

void DragonSynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Title
    auto titleBounds = bounds.removeFromTop(50);
    titleLabel.setBounds(titleBounds);
    
    // Waveform display
    auto waveformBounds = bounds.removeFromTop(80);
    waveformDisplay.setBounds(waveformBounds.reduced(5));
    
    bounds.removeFromTop(10);
    
    // Control sections
    auto controlSection = bounds.removeFromTop(260);
    
    // OSC 1 Section
    auto osc1Section = controlSection.removeFromLeft(150).reduced(10);
    osc1Label.setBounds(osc1Section.removeFromTop(25));
    osc1WaveSelector.setBounds(osc1Section.removeFromTop(30).reduced(5, 2));
    osc1Section.removeFromTop(10);
    auto osc1GainBounds = osc1Section.removeFromTop(150);
    osc1GainSlider.setBounds(osc1GainBounds.withSizeKeepingCentre(80, 80));
    osc1GainLabel.setBounds(osc1GainBounds.removeFromBottom(20));
    
    // OSC 2 Section
    auto osc2Section = controlSection.removeFromLeft(150).reduced(10);
    osc2Label.setBounds(osc2Section.removeFromTop(25));
    osc2WaveSelector.setBounds(osc2Section.removeFromTop(30).reduced(5, 2));
    osc2Section.removeFromTop(5);
    
    auto osc2KnobRow1 = osc2Section.removeFromTop(70);
    auto osc2KnobRow2 = osc2Section.removeFromTop(70);
    
    auto osc2GainBounds = osc2KnobRow1.removeFromLeft(70);
    osc2GainSlider.setBounds(osc2GainBounds.withSizeKeepingCentre(55, 55));
    osc2GainLabel.setBounds(osc2GainBounds.removeFromBottom(15));
    
    auto osc2DetuneBounds = osc2KnobRow1;
    osc2DetuneSlider.setBounds(osc2DetuneBounds.withSizeKeepingCentre(55, 55));
    osc2DetuneLabel.setBounds(osc2DetuneBounds.removeFromBottom(15));
    
    auto osc2OctaveBounds = osc2KnobRow2.withSizeKeepingCentre(70, 70);
    osc2OctaveSlider.setBounds(osc2OctaveBounds.withSizeKeepingCentre(55, 55));
    osc2OctaveLabel.setBounds(osc2KnobRow2.removeFromBottom(15));
    
    // Filter Section
    auto filterSectionBounds = controlSection.removeFromLeft(200).reduced(10);
    filterLabel.setBounds(filterSectionBounds.removeFromTop(25));
    filterTypeSelector.setBounds(filterSectionBounds.removeFromTop(30).reduced(5, 2));
    filterSectionBounds.removeFromTop(5);
    
    auto filterKnobRow = filterSectionBounds.removeFromTop(80);
    int knobWidth = filterKnobRow.getWidth() / 3;
    
    auto cutoffBounds = filterKnobRow.removeFromLeft(knobWidth);
    filterCutoffSlider.setBounds(cutoffBounds.withSizeKeepingCentre(55, 55));
    filterCutoffLabel.setBounds(cutoffBounds.removeFromBottom(15));
    
    auto resBounds = filterKnobRow.removeFromLeft(knobWidth);
    filterResonanceSlider.setBounds(resBounds.withSizeKeepingCentre(55, 55));
    filterResonanceLabel.setBounds(resBounds.removeFromBottom(15));
    
    auto envAmtBounds = filterKnobRow;
    filterEnvAmountSlider.setBounds(envAmtBounds.withSizeKeepingCentre(55, 55));
    filterEnvAmountLabel.setBounds(envAmtBounds.removeFromBottom(15));
    
    // Amp Envelope Section
    auto ampEnvSection = controlSection.removeFromLeft(150).reduced(10);
    ampEnvLabel.setBounds(ampEnvSection.removeFromTop(25));
    ampEnvSection.removeFromTop(10);
    
    auto ampKnobRow1 = ampEnvSection.removeFromTop(80);
    auto ampKnobRow2 = ampEnvSection.removeFromTop(80);
    
    auto aKnobWidth = ampKnobRow1.getWidth() / 2;
    
    auto attackBounds = ampKnobRow1.removeFromLeft(aKnobWidth);
    ampAttackSlider.setBounds(attackBounds.withSizeKeepingCentre(50, 50));
    ampAttackLabel.setBounds(attackBounds.removeFromBottom(15));
    
    auto decayBounds = ampKnobRow1;
    ampDecaySlider.setBounds(decayBounds.withSizeKeepingCentre(50, 50));
    ampDecayLabel.setBounds(decayBounds.removeFromBottom(15));
    
    auto sustainBounds = ampKnobRow2.removeFromLeft(aKnobWidth);
    ampSustainSlider.setBounds(sustainBounds.withSizeKeepingCentre(50, 50));
    ampSustainLabel.setBounds(sustainBounds.removeFromBottom(15));
    
    auto releaseBounds = ampKnobRow2;
    ampReleaseSlider.setBounds(releaseBounds.withSizeKeepingCentre(50, 50));
    ampReleaseLabel.setBounds(releaseBounds.removeFromBottom(15));
    
    // Filter Envelope Section
    auto filterEnvSection = controlSection.removeFromLeft(150).reduced(10);
    filterEnvLabel.setBounds(filterEnvSection.removeFromTop(25));
    filterEnvSection.removeFromTop(10);
    
    auto fEnvKnobRow1 = filterEnvSection.removeFromTop(80);
    auto fEnvKnobRow2 = filterEnvSection.removeFromTop(80);
    
    auto fAttackBounds = fEnvKnobRow1.removeFromLeft(aKnobWidth);
    filterAttackSlider.setBounds(fAttackBounds.withSizeKeepingCentre(50, 50));
    filterAttackLabel.setBounds(fAttackBounds.removeFromBottom(15));
    
    auto fDecayBounds = fEnvKnobRow1;
    filterDecaySlider.setBounds(fDecayBounds.withSizeKeepingCentre(50, 50));
    filterDecayLabel.setBounds(fDecayBounds.removeFromBottom(15));
    
    auto fSustainBounds = fEnvKnobRow2.removeFromLeft(aKnobWidth);
    filterSustainSlider.setBounds(fSustainBounds.withSizeKeepingCentre(50, 50));
    filterSustainLabel.setBounds(fSustainBounds.removeFromBottom(15));
    
    auto fReleaseBounds = fEnvKnobRow2;
    filterReleaseSlider.setBounds(fReleaseBounds.withSizeKeepingCentre(50, 50));
    filterReleaseLabel.setBounds(fReleaseBounds.removeFromBottom(15));
    
    // Master Section
    auto masterSection = controlSection.reduced(10);
    masterSection = masterSection.withWidth(60);
    masterGainLabel.setBounds(masterSection.removeFromTop(20));
    masterGainSlider.setBounds(masterSection.reduced(5));
    
    // Keyboard
    bounds.removeFromTop(20);
    auto keyboardBounds = bounds.reduced(10);
    keyboardComponent->setBounds(keyboardBounds);
}
