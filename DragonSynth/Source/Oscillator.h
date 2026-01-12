#pragma once

#include <JuceHeader.h>
#include <cmath>

enum class WaveformType
{
    Sine = 0,
    Saw,
    Square,
    Triangle,
    Noise
};

class Oscillator
{
public:
    Oscillator();
    ~Oscillator() = default;
    
    void setFrequency(float frequency, float sampleRate);
    void setWaveform(WaveformType type);
    void setDetune(float cents);
    void setPulseWidth(float width);
    void reset();
    
    float getNextSample();
    
    WaveformType getWaveform() const { return waveform; }
    
private:
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float currentFrequency = 440.0f;
    float sampleRate = 44100.0f;
    float detuneAmount = 0.0f;
    float pulseWidth = 0.5f;
    WaveformType waveform = WaveformType::Saw;
    
    juce::Random random;
    
    float polyBLEP(float t, float dt);
};
