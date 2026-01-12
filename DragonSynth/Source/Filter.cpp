#include "Filter.h"

Filter::Filter()
{
    updateCoefficients();
}

void Filter::setType(FilterType type)
{
    filterType = type;
}

void Filter::setCutoff(float frequency)
{
    cutoffFrequency = juce::jlimit(20.0f, 20000.0f, frequency);
    updateCoefficients();
}

void Filter::setResonance(float q)
{
    resonance = juce::jlimit(0.1f, 10.0f, q);
    updateCoefficients();
}

void Filter::setSampleRate(float sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void Filter::reset()
{
    ic1eq = 0.0f;
    ic2eq = 0.0f;
}

void Filter::updateCoefficients()
{
    // SVF filter coefficients
    g = std::tan(juce::MathConstants<float>::pi * cutoffFrequency / sampleRate);
    k = 1.0f / resonance;
    a1 = 1.0f / (1.0f + g * (g + k));
    a2 = g * a1;
    a3 = g * a2;
}

float Filter::process(float input)
{
    // State Variable Filter (Chamberlin implementation)
    float v3 = input - ic2eq;
    float v1 = a1 * ic1eq + a2 * v3;
    float v2 = ic2eq + a2 * ic1eq + a3 * v3;
    
    ic1eq = 2.0f * v1 - ic1eq;
    ic2eq = 2.0f * v2 - ic2eq;
    
    // Output based on filter type
    switch (filterType)
    {
        case FilterType::LowPass:
            return v2;
        case FilterType::HighPass:
            return input - k * v1 - v2;
        case FilterType::BandPass:
            return v1;
        default:
            return v2;
    }
}
