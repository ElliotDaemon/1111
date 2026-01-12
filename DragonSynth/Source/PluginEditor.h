#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "KeyboardComponent.h"
#include "WaveformDisplay.h"

class DragonSynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                         public juce::Timer
{
public:
    DragonSynthAudioProcessorEditor(DragonSynthAudioProcessor&);
    ~DragonSynthAudioProcessorEditor() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
private:
    DragonSynthAudioProcessor& audioProcessor;
    DragonLookAndFeel dragonLookAndFeel;
    
    // MIDI keyboard - reference to processor's keyboard state
    juce::MidiKeyboardState& keyboardState;
    std::unique_ptr<MarbleKeyboardComponent> keyboardComponent;
    
    // Waveform display
    WaveformDisplay waveformDisplay;
    
    // Oscillator 1 controls
    juce::ComboBox osc1WaveSelector;
    juce::Slider osc1GainSlider;
    juce::Label osc1Label;
    juce::Label osc1GainLabel;
    
    // Oscillator 2 controls
    juce::ComboBox osc2WaveSelector;
    juce::Slider osc2GainSlider;
    juce::Slider osc2DetuneSlider;
    juce::Slider osc2OctaveSlider;
    juce::Label osc2Label;
    juce::Label osc2GainLabel;
    juce::Label osc2DetuneLabel;
    juce::Label osc2OctaveLabel;
    
    // Filter controls
    juce::ComboBox filterTypeSelector;
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider filterEnvAmountSlider;
    juce::Label filterLabel;
    juce::Label filterCutoffLabel;
    juce::Label filterResonanceLabel;
    juce::Label filterEnvAmountLabel;
    
    // Amp envelope controls
    juce::Slider ampAttackSlider;
    juce::Slider ampDecaySlider;
    juce::Slider ampSustainSlider;
    juce::Slider ampReleaseSlider;
    juce::Label ampEnvLabel;
    juce::Label ampAttackLabel;
    juce::Label ampDecayLabel;
    juce::Label ampSustainLabel;
    juce::Label ampReleaseLabel;
    
    // Filter envelope controls
    juce::Slider filterAttackSlider;
    juce::Slider filterDecaySlider;
    juce::Slider filterSustainSlider;
    juce::Slider filterReleaseSlider;
    juce::Label filterEnvLabel;
    juce::Label filterAttackLabel;
    juce::Label filterDecayLabel;
    juce::Label filterSustainLabel;
    juce::Label filterReleaseLabel;
    
    // Master controls
    juce::Slider masterGainSlider;
    juce::Label masterGainLabel;
    juce::Label titleLabel;
    
    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2DetuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2OctaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterGainAttachment;
    
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText);
    void setupComboBox(juce::ComboBox& box, const juce::StringArray& items);
    void drawSectionPanel(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& title);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonSynthAudioProcessorEditor)
};
