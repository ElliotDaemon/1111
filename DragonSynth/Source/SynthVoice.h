#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Oscillator.h"
#include "ADSREnvelope.h"
#include "Filter.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice();
    ~SynthVoice() override = default;
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    
    // Parameter setters
    void setOsc1Waveform(WaveformType type);
    void setOsc2Waveform(WaveformType type);
    void setOsc1Gain(float gain);
    void setOsc2Gain(float gain);
    void setOsc2Detune(float cents);
    void setOsc2Octave(int octave);
    
    void setFilterType(FilterType type);
    void setFilterCutoff(float frequency);
    void setFilterResonance(float q);
    void setFilterEnvAmount(float amount);
    
    void setAmpAttack(float ms);
    void setAmpDecay(float ms);
    void setAmpSustain(float level);
    void setAmpRelease(float ms);
    
    void setFilterAttack(float ms);
    void setFilterDecay(float ms);
    void setFilterSustain(float level);
    void setFilterRelease(float ms);
    
    void setMasterGain(float gain);
    
private:
    Oscillator osc1;
    Oscillator osc2;
    ADSREnvelope ampEnvelope;
    ADSREnvelope filterEnvelope;
    Filter filter;
    
    float osc1Gain = 0.5f;
    float osc2Gain = 0.5f;
    int osc2Octave = 0;
    float filterEnvAmount = 0.0f;
    float baseCutoff = 1000.0f;
    float masterGain = 0.8f;
    float velocity = 0.0f;
    float pitchBend = 0.0f;
    int currentNote = -1;
    
    bool isPrepared = false;
    double currentSampleRate = 44100.0;
};
