#include "SynthVoice.h"

SynthVoice::SynthVoice()
{
    osc1.setWaveform(WaveformType::Saw);
    osc2.setWaveform(WaveformType::Saw);
    osc2.setDetune(7.0f);  // Slight detune for thickness
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::prepareToPlay(double sampleRate, int /*samplesPerBlock*/, int /*outputChannels*/)
{
    currentSampleRate = sampleRate;
    ampEnvelope.setSampleRate(static_cast<float>(sampleRate));
    filterEnvelope.setSampleRate(static_cast<float>(sampleRate));
    filter.setSampleRate(static_cast<float>(sampleRate));
    isPrepared = true;
}

void SynthVoice::startNote(int midiNoteNumber, float vel, juce::SynthesiserSound* /*sound*/, int currentPitchWheelPosition)
{
    currentNote = midiNoteNumber;
    velocity = vel;
    
    // Calculate frequency from MIDI note
    float frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    
    // Apply pitch bend
    pitchBend = (currentPitchWheelPosition - 8192) / 8192.0f * 2.0f;  // +/- 2 semitones
    float bendMultiplier = std::pow(2.0f, pitchBend / 12.0f);
    frequency *= bendMultiplier;
    
    osc1.setFrequency(frequency, static_cast<float>(currentSampleRate));
    osc1.reset();
    
    // Osc2 with octave offset
    float osc2Freq = frequency * std::pow(2.0f, static_cast<float>(osc2Octave));
    osc2.setFrequency(osc2Freq, static_cast<float>(currentSampleRate));
    osc2.reset();
    
    ampEnvelope.noteOn();
    filterEnvelope.noteOn();
    filter.reset();
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        ampEnvelope.noteOff();
        filterEnvelope.noteOff();
    }
    else
    {
        ampEnvelope.reset();
        filterEnvelope.reset();
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    pitchBend = (newPitchWheelValue - 8192) / 8192.0f * 2.0f;
    
    if (currentNote >= 0)
    {
        float frequency = juce::MidiMessage::getMidiNoteInHertz(currentNote);
        float bendMultiplier = std::pow(2.0f, pitchBend / 12.0f);
        frequency *= bendMultiplier;
        
        osc1.setFrequency(frequency, static_cast<float>(currentSampleRate));
        
        float osc2Freq = frequency * std::pow(2.0f, static_cast<float>(osc2Octave));
        osc2.setFrequency(osc2Freq, static_cast<float>(currentSampleRate));
    }
}

void SynthVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
    // Can be extended for modulation wheel, etc.
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isPrepared)
        return;
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float ampEnv = ampEnvelope.getNextSample();
        float filterEnv = filterEnvelope.getNextSample();
        
        if (!ampEnvelope.isActive())
        {
            clearCurrentNote();
            break;
        }
        
        // Mix oscillators
        float osc1Sample = osc1.getNextSample() * osc1Gain;
        float osc2Sample = osc2.getNextSample() * osc2Gain;
        float mixedSample = osc1Sample + osc2Sample;
        
        // Apply filter with envelope modulation
        float cutoffMod = baseCutoff + (filterEnv * filterEnvAmount * 8000.0f);
        filter.setCutoff(cutoffMod);
        float filteredSample = filter.process(mixedSample);
        
        // Apply amplitude envelope and velocity
        float outputSample = filteredSample * ampEnv * velocity * masterGain;
        
        // Write to all channels
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + sample, outputSample);
        }
    }
}

// Parameter setters
void SynthVoice::setOsc1Waveform(WaveformType type)
{
    osc1.setWaveform(type);
}

void SynthVoice::setOsc2Waveform(WaveformType type)
{
    osc2.setWaveform(type);
}

void SynthVoice::setOsc1Gain(float gain)
{
    osc1Gain = gain;
}

void SynthVoice::setOsc2Gain(float gain)
{
    osc2Gain = gain;
}

void SynthVoice::setOsc2Detune(float cents)
{
    osc2.setDetune(cents);
}

void SynthVoice::setOsc2Octave(int octave)
{
    osc2Octave = octave;
}

void SynthVoice::setFilterType(FilterType type)
{
    filter.setType(type);
}

void SynthVoice::setFilterCutoff(float frequency)
{
    baseCutoff = frequency;
    filter.setCutoff(frequency);
}

void SynthVoice::setFilterResonance(float q)
{
    filter.setResonance(q);
}

void SynthVoice::setFilterEnvAmount(float amount)
{
    filterEnvAmount = amount;
}

void SynthVoice::setAmpAttack(float ms)
{
    ampEnvelope.setAttack(ms);
}

void SynthVoice::setAmpDecay(float ms)
{
    ampEnvelope.setDecay(ms);
}

void SynthVoice::setAmpSustain(float level)
{
    ampEnvelope.setSustain(level);
}

void SynthVoice::setAmpRelease(float ms)
{
    ampEnvelope.setRelease(ms);
}

void SynthVoice::setFilterAttack(float ms)
{
    filterEnvelope.setAttack(ms);
}

void SynthVoice::setFilterDecay(float ms)
{
    filterEnvelope.setDecay(ms);
}

void SynthVoice::setFilterSustain(float level)
{
    filterEnvelope.setSustain(level);
}

void SynthVoice::setFilterRelease(float ms)
{
    filterEnvelope.setRelease(ms);
}

void SynthVoice::setMasterGain(float gain)
{
    masterGain = gain;
}
