#include "Oscillator.h"

Oscillator::Oscillator()
{
    random.setSeed(juce::Time::currentTimeMillis());
}

void Oscillator::setFrequency(float frequency, float sr)
{
    sampleRate = sr;
    currentFrequency = frequency;
    
    // Apply detune in cents
    float detuneMultiplier = std::pow(2.0f, detuneAmount / 1200.0f);
    float detunedFreq = currentFrequency * detuneMultiplier;
    
    phaseIncrement = detunedFreq / sampleRate;
}

void Oscillator::setWaveform(WaveformType type)
{
    waveform = type;
}

void Oscillator::setDetune(float cents)
{
    detuneAmount = cents;
}

void Oscillator::setPulseWidth(float width)
{
    pulseWidth = juce::jlimit(0.01f, 0.99f, width);
}

void Oscillator::reset()
{
    phase = 0.0f;
}

float Oscillator::polyBLEP(float t, float dt)
{
    // PolyBLEP for anti-aliasing
    if (t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

float Oscillator::getNextSample()
{
    float sample = 0.0f;
    
    switch (waveform)
    {
        case WaveformType::Sine:
            sample = std::sin(2.0f * juce::MathConstants<float>::pi * phase);
            break;
            
        case WaveformType::Saw:
        {
            sample = 2.0f * phase - 1.0f;
            sample -= polyBLEP(phase, phaseIncrement);
            break;
        }
            
        case WaveformType::Square:
        {
            sample = phase < pulseWidth ? 1.0f : -1.0f;
            sample += polyBLEP(phase, phaseIncrement);
            sample -= polyBLEP(std::fmod(phase + (1.0f - pulseWidth), 1.0f), phaseIncrement);
            break;
        }
            
        case WaveformType::Triangle:
        {
            sample = 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
            break;
        }
            
        case WaveformType::Noise:
            sample = random.nextFloat() * 2.0f - 1.0f;
            break;
    }
    
    // Advance phase
    phase += phaseIncrement;
    while (phase >= 1.0f)
        phase -= 1.0f;
    
    return sample;
}
