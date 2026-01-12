#include "PluginProcessor.h"
#include "PluginEditor.h"

DragonSynthAudioProcessor::DragonSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Add voices to synthesizer
    for (int i = 0; i < 16; ++i)
    {
        synth.addVoice(new SynthVoice());
    }
    
    // Add sound
    synth.addSound(new SynthSound());
    
    // Set up parameter listeners
    auto paramIDs = {"osc1Wave", "osc2Wave", "osc1Gain", "osc2Gain", "osc2Detune", "osc2Octave",
                     "filterType", "filterCutoff", "filterResonance", "filterEnvAmount",
                     "ampAttack", "ampDecay", "ampSustain", "ampRelease",
                     "filterAttack", "filterDecay", "filterSustain", "filterRelease",
                     "masterGain"};
    
    for (auto& paramID : paramIDs)
    {
        apvts.addParameterListener(paramID, this);
    }
    
    waveformData.fill(0.0f);
}

DragonSynthAudioProcessor::~DragonSynthAudioProcessor()
{
    auto paramIDs = {"osc1Wave", "osc2Wave", "osc1Gain", "osc2Gain", "osc2Detune", "osc2Octave",
                     "filterType", "filterCutoff", "filterResonance", "filterEnvAmount",
                     "ampAttack", "ampDecay", "ampSustain", "ampRelease",
                     "filterAttack", "filterDecay", "filterSustain", "filterRelease",
                     "masterGain"};
    
    for (auto& paramID : paramIDs)
    {
        apvts.removeParameterListener(paramID, this);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout DragonSynthAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Oscillator 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "osc1Wave", "OSC1 Wave",
        juce::StringArray{"Sine", "Saw", "Square", "Triangle", "Noise"}, 1));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc1Gain", "OSC1 Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    // Oscillator 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "osc2Wave", "OSC2 Wave",
        juce::StringArray{"Sine", "Saw", "Square", "Triangle", "Noise"}, 1));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc2Gain", "OSC2 Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc2Detune", "OSC2 Detune",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 7.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "osc2Octave", "OSC2 Octave", -2, 2, 0));
    
    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "filterType", "Filter Type",
        juce::StringArray{"Low Pass", "High Pass", "Band Pass"}, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterCutoff", "Filter Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 8000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterResonance", "Filter Resonance",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.5f), 0.707f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterEnvAmount", "Filter Env Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f));
    
    // Amp Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampAttack", "Amp Attack",
        juce::NormalisableRange<float>(0.1f, 5000.0f, 0.1f, 0.3f), 10.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampDecay", "Amp Decay",
        juce::NormalisableRange<float>(0.1f, 5000.0f, 0.1f, 0.3f), 100.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampSustain", "Amp Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.7f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampRelease", "Amp Release",
        juce::NormalisableRange<float>(0.1f, 10000.0f, 0.1f, 0.3f), 200.0f));
    
    // Filter Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterAttack", "Filter Attack",
        juce::NormalisableRange<float>(0.1f, 5000.0f, 0.1f, 0.3f), 10.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterDecay", "Filter Decay",
        juce::NormalisableRange<float>(0.1f, 5000.0f, 0.1f, 0.3f), 300.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterSustain", "Filter Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterRelease", "Filter Release",
        juce::NormalisableRange<float>(0.1f, 10000.0f, 0.1f, 0.3f), 500.0f));
    
    // Master
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "masterGain", "Master Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.8f));
    
    return { params.begin(), params.end() };
}

void DragonSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
    updateAllVoices();
}

void DragonSynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DragonSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void DragonSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Get keyboard MIDI events
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    // Update waveform data for visualization
    if (buffer.getNumSamples() > 0)
    {
        auto* channelData = buffer.getReadPointer(0);
        int step = juce::jmax(1, buffer.getNumSamples() / 512);
        for (int i = 0; i < 512 && i * step < buffer.getNumSamples(); ++i)
        {
            waveformData[i] = channelData[i * step];
        }
        waveformReady.store(true);
    }
}

juce::AudioProcessorEditor* DragonSynthAudioProcessor::createEditor()
{
    return new DragonSynthAudioProcessorEditor(*this);
}

bool DragonSynthAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String DragonSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DragonSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DragonSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DragonSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DragonSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DragonSynthAudioProcessor::getNumPrograms()
{
    return 1;
}

int DragonSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DragonSynthAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String DragonSynthAudioProcessor::getProgramName(int /*index*/)
{
    return {};
}

void DragonSynthAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
}

void DragonSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DragonSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
        updateAllVoices();
    }
}

void DragonSynthAudioProcessor::parameterChanged(const juce::String& /*parameterID*/, float /*newValue*/)
{
    updateAllVoices();
}

void DragonSynthAudioProcessor::updateAllVoices()
{
    auto osc1Wave = static_cast<WaveformType>(apvts.getRawParameterValue("osc1Wave")->load());
    auto osc2Wave = static_cast<WaveformType>(apvts.getRawParameterValue("osc2Wave")->load());
    auto osc1Gain = apvts.getRawParameterValue("osc1Gain")->load();
    auto osc2Gain = apvts.getRawParameterValue("osc2Gain")->load();
    auto osc2Detune = apvts.getRawParameterValue("osc2Detune")->load();
    auto osc2Octave = static_cast<int>(apvts.getRawParameterValue("osc2Octave")->load());
    
    auto filterType = static_cast<FilterType>(apvts.getRawParameterValue("filterType")->load());
    auto filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
    auto filterResonance = apvts.getRawParameterValue("filterResonance")->load();
    auto filterEnvAmount = apvts.getRawParameterValue("filterEnvAmount")->load();
    
    auto ampAttack = apvts.getRawParameterValue("ampAttack")->load();
    auto ampDecay = apvts.getRawParameterValue("ampDecay")->load();
    auto ampSustain = apvts.getRawParameterValue("ampSustain")->load();
    auto ampRelease = apvts.getRawParameterValue("ampRelease")->load();
    
    auto filterAttack = apvts.getRawParameterValue("filterAttack")->load();
    auto filterDecay = apvts.getRawParameterValue("filterDecay")->load();
    auto filterSustain = apvts.getRawParameterValue("filterSustain")->load();
    auto filterRelease = apvts.getRawParameterValue("filterRelease")->load();
    
    auto masterGain = apvts.getRawParameterValue("masterGain")->load();
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setOsc1Waveform(osc1Wave);
            voice->setOsc2Waveform(osc2Wave);
            voice->setOsc1Gain(osc1Gain);
            voice->setOsc2Gain(osc2Gain);
            voice->setOsc2Detune(osc2Detune);
            voice->setOsc2Octave(osc2Octave);
            
            voice->setFilterType(filterType);
            voice->setFilterCutoff(filterCutoff);
            voice->setFilterResonance(filterResonance);
            voice->setFilterEnvAmount(filterEnvAmount);
            
            voice->setAmpAttack(ampAttack);
            voice->setAmpDecay(ampDecay);
            voice->setAmpSustain(ampSustain);
            voice->setAmpRelease(ampRelease);
            
            voice->setFilterAttack(filterAttack);
            voice->setFilterDecay(filterDecay);
            voice->setFilterSustain(filterSustain);
            voice->setFilterRelease(filterRelease);
            
            voice->setMasterGain(masterGain);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DragonSynthAudioProcessor();
}
