#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

class WaveformDisplay : public juce::Component,
                         public juce::Timer
{
public:
    WaveformDisplay();
    ~WaveformDisplay() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    void setWaveformData(const std::array<float, 512>& data);
    void setWaveformType(int type);
    
private:
    std::array<float, 512> waveformData;
    std::array<float, 512> displayData;
    int currentWaveformType = 1;  // Saw by default
    
    float smoothingFactor = 0.3f;
    
    void generateStaticWaveform();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
