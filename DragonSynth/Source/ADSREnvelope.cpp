#include "ADSREnvelope.h"

ADSREnvelope::ADSREnvelope()
{
    updateCoefficients();
}

void ADSREnvelope::setAttack(float attackTimeMs)
{
    attackTime = juce::jmax(0.1f, attackTimeMs);
    updateCoefficients();
}

void ADSREnvelope::setDecay(float decayTimeMs)
{
    decayTime = juce::jmax(0.1f, decayTimeMs);
    updateCoefficients();
}

void ADSREnvelope::setSustain(float level)
{
    sustainLevel = juce::jlimit(0.0f, 1.0f, level);
}

void ADSREnvelope::setRelease(float releaseTimeMs)
{
    releaseTime = juce::jmax(0.1f, releaseTimeMs);
    updateCoefficients();
}

void ADSREnvelope::setSampleRate(float sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void ADSREnvelope::updateCoefficients()
{
    float attackSamples = (attackTime / 1000.0f) * sampleRate;
    float decaySamples = (decayTime / 1000.0f) * sampleRate;
    float releaseSamples = (releaseTime / 1000.0f) * sampleRate;
    
    attackIncrement = 1.0f / juce::jmax(1.0f, attackSamples);
    decayDecrement = (1.0f - sustainLevel) / juce::jmax(1.0f, decaySamples);
    releaseDecrement = 1.0f / juce::jmax(1.0f, releaseSamples);
}

void ADSREnvelope::noteOn()
{
    currentState = State::Attack;
}

void ADSREnvelope::noteOff()
{
    if (currentState != State::Idle)
        currentState = State::Release;
}

void ADSREnvelope::reset()
{
    currentState = State::Idle;
    currentLevel = 0.0f;
}

bool ADSREnvelope::isActive() const
{
    return currentState != State::Idle;
}

float ADSREnvelope::getNextSample()
{
    switch (currentState)
    {
        case State::Idle:
            currentLevel = 0.0f;
            break;
            
        case State::Attack:
            currentLevel += attackIncrement;
            if (currentLevel >= 1.0f)
            {
                currentLevel = 1.0f;
                currentState = State::Decay;
            }
            break;
            
        case State::Decay:
            currentLevel -= decayDecrement;
            if (currentLevel <= sustainLevel)
            {
                currentLevel = sustainLevel;
                currentState = State::Sustain;
            }
            break;
            
        case State::Sustain:
            currentLevel = sustainLevel;
            break;
            
        case State::Release:
            currentLevel -= releaseDecrement * currentLevel + 0.0001f;
            if (currentLevel <= 0.001f)
            {
                currentLevel = 0.0f;
                currentState = State::Idle;
            }
            break;
    }
    
    return currentLevel;
}
