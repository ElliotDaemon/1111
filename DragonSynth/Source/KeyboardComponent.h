#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

class MarbleKeyboardComponent : public juce::Component,
                                 public juce::MidiKeyboardState::Listener
{
public:
    MarbleKeyboardComponent(juce::MidiKeyboardState& state);
    ~MarbleKeyboardComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    bool keyPressed(const juce::KeyPress& key) override;
    bool keyStateChanged(bool isKeyDown) override;
    
    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void setBaseOctave(int octave);
    int getBaseOctave() const { return baseOctave; }
    
private:
    juce::MidiKeyboardState& keyboardState;
    int baseOctave = 4;
    int numOctaves = 2;
    int currentMouseNote = -1;
    
    std::set<int> activeNotes;
    std::map<int, int> keyToNote;  // Maps key codes to MIDI note offsets
    std::set<int> heldKeys;
    
    void setupKeyMapping();
    int getKeyForPosition(juce::Point<float> pos) const;
    juce::Rectangle<float> getKeyBounds(int keyIndex) const;
    bool isBlackKey(int keyIndex) const;
    void drawWhiteKey(juce::Graphics& g, juce::Rectangle<float> bounds, bool isPressed, int keyIndex);
    void drawBlackKey(juce::Graphics& g, juce::Rectangle<float> bounds, bool isPressed, int keyIndex);
    void drawMarbleTexture(juce::Graphics& g, juce::Rectangle<float> bounds, bool isWhite, bool isPressed);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MarbleKeyboardComponent)
};
