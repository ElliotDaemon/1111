#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay()
{
    waveformData.fill(0.0f);
    displayData.fill(0.0f);
    generateStaticWaveform();
    startTimerHz(30);
}

void WaveformDisplay::setWaveformData(const std::array<float, 512>& data)
{
    waveformData = data;
}

void WaveformDisplay::setWaveformType(int type)
{
    if (currentWaveformType != type)
    {
        currentWaveformType = type;
        generateStaticWaveform();
    }
}

void WaveformDisplay::generateStaticWaveform()
{
    for (int i = 0; i < 512; ++i)
    {
        float phase = static_cast<float>(i) / 512.0f;
        float sample = 0.0f;
        
        switch (currentWaveformType)
        {
            case 0: // Sine
                sample = std::sin(2.0f * juce::MathConstants<float>::pi * phase * 2);
                break;
            case 1: // Saw
                sample = 2.0f * std::fmod(phase * 2, 1.0f) - 1.0f;
                break;
            case 2: // Square
                sample = phase * 2 < 1.0f ? (std::fmod(phase * 2, 1.0f) < 0.5f ? 1.0f : -1.0f) : 
                         (std::fmod(phase * 2, 1.0f) < 0.5f ? 1.0f : -1.0f);
                break;
            case 3: // Triangle
                sample = 2.0f * std::abs(2.0f * std::fmod(phase * 2, 1.0f) - 1.0f) - 1.0f;
                break;
            case 4: // Noise
                sample = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
                break;
        }
        
        waveformData[i] = sample * 0.8f;
    }
}

void WaveformDisplay::timerCallback()
{
    // Smooth interpolation for display
    for (int i = 0; i < 512; ++i)
    {
        displayData[i] += (waveformData[i] - displayData[i]) * smoothingFactor;
    }
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Background with dragon glass effect
    DragonLookAndFeel::drawDragonGlassPanel(g, bounds, 8);
    
    auto displayBounds = bounds.reduced(10);
    
    // Grid lines
    g.setColour(DragonLookAndFeel::getMetallicHighlight().withAlpha(0.2f));
    
    // Horizontal center line
    float centerY = displayBounds.getCentreY();
    g.drawHorizontalLine(static_cast<int>(centerY), displayBounds.getX(), displayBounds.getRight());
    
    // Vertical grid lines
    for (int i = 1; i < 4; ++i)
    {
        float x = displayBounds.getX() + (displayBounds.getWidth() / 4.0f) * i;
        g.drawVerticalLine(static_cast<int>(x), displayBounds.getY(), displayBounds.getBottom());
    }
    
    // Draw waveform
    juce::Path waveformPath;
    bool started = false;
    
    for (int i = 0; i < 512; ++i)
    {
        float x = displayBounds.getX() + (static_cast<float>(i) / 511.0f) * displayBounds.getWidth();
        float y = centerY - displayData[i] * (displayBounds.getHeight() * 0.4f);
        
        if (!started)
        {
            waveformPath.startNewSubPath(x, y);
            started = true;
        }
        else
        {
            waveformPath.lineTo(x, y);
        }
    }
    
    // Glow effect
    g.setColour(DragonLookAndFeel::getDragonIceGlow().withAlpha(0.3f));
    g.strokePath(waveformPath, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, 
                                                     juce::PathStrokeType::rounded));
    
    // Main waveform line
    g.setColour(DragonLookAndFeel::getDragonIce());
    g.strokePath(waveformPath, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved, 
                                                     juce::PathStrokeType::rounded));
    
    // Bright core
    g.setColour(DragonLookAndFeel::getDragonIceGlow().brighter(0.5f));
    g.strokePath(waveformPath, juce::PathStrokeType(0.5f, juce::PathStrokeType::curved, 
                                                     juce::PathStrokeType::rounded));
    
    // Title
    g.setColour(DragonLookAndFeel::getTextLight());
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("WAVEFORM", bounds.removeFromTop(25).reduced(10, 5), juce::Justification::centredLeft);
}

void WaveformDisplay::resized()
{
}
