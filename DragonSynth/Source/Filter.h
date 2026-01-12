#pragma once

#include <JuceHeader.h>

enum class FilterType
{
    LowPass = 0,
    HighPass,
    BandPass
};

class Filter
{
public:
    Filter();
    ~Filter() = default;
    
    void setType(FilterType type);
    void setCutoff(float frequency);
    void setResonance(float q);
    void setSampleRate(float sampleRate);
    void reset();
    
    float process(float input);
    
    FilterType getType() const { return filterType; }
    float getCutoff() const { return cutoffFrequency; }
    float getResonance() const { return resonance; }
    
private:
    FilterType filterType = FilterType::LowPass;
    float cutoffFrequency = 1000.0f;
    float resonance = 0.707f;
    float sampleRate = 44100.0f;
    
    // State variables for SVF (State Variable Filter)
    float ic1eq = 0.0f;
    float ic2eq = 0.0f;
    
    // Coefficients
    float g = 0.0f;
    float k = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float a3 = 0.0f;
    
    void updateCoefficients();
};
