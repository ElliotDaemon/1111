#pragma once

#include <JuceHeader.h>

class ADSREnvelope
{
public:
    ADSREnvelope();
    ~ADSREnvelope() = default;
    
    void setAttack(float attackTimeMs);
    void setDecay(float decayTimeMs);
    void setSustain(float sustainLevel);
    void setRelease(float releaseTimeMs);
    void setSampleRate(float sampleRate);
    
    void noteOn();
    void noteOff();
    void reset();
    
    float getNextSample();
    bool isActive() const;
    
    // Get current parameters
    float getAttack() const { return attackTime; }
    float getDecay() const { return decayTime; }
    float getSustain() const { return sustainLevel; }
    float getRelease() const { return releaseTime; }
    
private:
    enum class State
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };
    
    State currentState = State::Idle;
    float currentLevel = 0.0f;
    float attackTime = 10.0f;  // ms
    float decayTime = 100.0f;  // ms
    float sustainLevel = 0.7f;
    float releaseTime = 200.0f;  // ms
    float sampleRate = 44100.0f;
    
    float attackIncrement = 0.0f;
    float decayDecrement = 0.0f;
    float releaseDecrement = 0.0f;
    
    void updateCoefficients();
};
