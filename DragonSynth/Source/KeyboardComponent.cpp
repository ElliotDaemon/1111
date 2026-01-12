#include "KeyboardComponent.h"

MarbleKeyboardComponent::MarbleKeyboardComponent(juce::MidiKeyboardState& state)
    : keyboardState(state)
{
    setupKeyMapping();
    keyboardState.addListener(this);
    setWantsKeyboardFocus(true);
}

MarbleKeyboardComponent::~MarbleKeyboardComponent()
{
    keyboardState.removeListener(this);
}

void MarbleKeyboardComponent::setupKeyMapping()
{
    // QWERTY keyboard mapping (2 octaves)
    // Lower row: Z to M = C3 to B3
    keyToNote['Z'] = 0;   // C
    keyToNote['S'] = 1;   // C#
    keyToNote['X'] = 2;   // D
    keyToNote['D'] = 3;   // D#
    keyToNote['C'] = 4;   // E
    keyToNote['V'] = 5;   // F
    keyToNote['G'] = 6;   // F#
    keyToNote['B'] = 7;   // G
    keyToNote['H'] = 8;   // G#
    keyToNote['N'] = 9;   // A
    keyToNote['J'] = 10;  // A#
    keyToNote['M'] = 11;  // B
    
    // Upper row: Q to P = C4 to B4
    keyToNote['Q'] = 12;  // C
    keyToNote['2'] = 13;  // C#
    keyToNote['W'] = 14;  // D
    keyToNote['3'] = 15;  // D#
    keyToNote['E'] = 16;  // E
    keyToNote['R'] = 17;  // F
    keyToNote['5'] = 18;  // F#
    keyToNote['T'] = 19;  // G
    keyToNote['6'] = 20;  // G#
    keyToNote['Y'] = 21;  // A
    keyToNote['7'] = 22;  // A#
    keyToNote['U'] = 23;  // B
    keyToNote['I'] = 24;  // C (next octave)
}

void MarbleKeyboardComponent::setBaseOctave(int octave)
{
    baseOctave = juce::jlimit(0, 8, octave);
    repaint();
}

bool MarbleKeyboardComponent::keyPressed(const juce::KeyPress& key)
{
    int keyCode = key.getKeyCode();
    
    // Octave change with [ and ]
    if (keyCode == '[')
    {
        setBaseOctave(baseOctave - 1);
        return true;
    }
    else if (keyCode == ']')
    {
        setBaseOctave(baseOctave + 1);
        return true;
    }
    
    return false;
}

bool MarbleKeyboardComponent::keyStateChanged(bool /*isKeyDown*/)
{
    // Check all mapped keys
    for (const auto& [keyCode, noteOffset] : keyToNote)
    {
        bool isDown = juce::KeyPress::isKeyCurrentlyDown(keyCode);
        int midiNote = (baseOctave * 12) + noteOffset;
        
        if (isDown && heldKeys.find(keyCode) == heldKeys.end())
        {
            // Key just pressed
            heldKeys.insert(keyCode);
            keyboardState.noteOn(1, midiNote, 0.8f);
        }
        else if (!isDown && heldKeys.find(keyCode) != heldKeys.end())
        {
            // Key just released
            heldKeys.erase(keyCode);
            keyboardState.noteOff(1, midiNote, 0.0f);
        }
    }
    
    repaint();
    return true;
}

void MarbleKeyboardComponent::handleNoteOn(juce::MidiKeyboardState* /*source*/, int /*midiChannel*/, int midiNoteNumber, float /*velocity*/)
{
    activeNotes.insert(midiNoteNumber);
    repaint();
}

void MarbleKeyboardComponent::handleNoteOff(juce::MidiKeyboardState* /*source*/, int /*midiChannel*/, int midiNoteNumber, float /*velocity*/)
{
    activeNotes.erase(midiNoteNumber);
    repaint();
}

bool MarbleKeyboardComponent::isBlackKey(int keyIndex) const
{
    int noteInOctave = keyIndex % 12;
    return noteInOctave == 1 || noteInOctave == 3 || noteInOctave == 6 || 
           noteInOctave == 8 || noteInOctave == 10;
}

juce::Rectangle<float> MarbleKeyboardComponent::getKeyBounds(int keyIndex) const
{
    int totalWhiteKeys = numOctaves * 7 + 1;  // +1 for the last C
    float whiteKeyWidth = getWidth() / static_cast<float>(totalWhiteKeys);
    float blackKeyWidth = whiteKeyWidth * 0.65f;
    float whiteKeyHeight = static_cast<float>(getHeight());
    float blackKeyHeight = whiteKeyHeight * 0.6f;
    
    int octave = keyIndex / 12;
    int noteInOctave = keyIndex % 12;
    
    // Map note to white key position
    static const int whiteKeyPositions[] = {0, -1, 1, -1, 2, 3, -1, 4, -1, 5, -1, 6};
    static const int blackKeyOffsets[] = {-1, 0, -1, 1, -1, -1, 2, -1, 3, -1, 4, -1};
    
    if (!isBlackKey(keyIndex))
    {
        int whitePos = octave * 7 + whiteKeyPositions[noteInOctave];
        return juce::Rectangle<float>(whitePos * whiteKeyWidth, 0, whiteKeyWidth, whiteKeyHeight);
    }
    else
    {
        int whitePos = octave * 7 + blackKeyOffsets[noteInOctave];
        float xPos = (whitePos + 1) * whiteKeyWidth - blackKeyWidth / 2;
        return juce::Rectangle<float>(xPos, 0, blackKeyWidth, blackKeyHeight);
    }
}

int MarbleKeyboardComponent::getKeyForPosition(juce::Point<float> pos) const
{
    // Check black keys first (they're on top)
    int totalKeys = numOctaves * 12 + 1;
    
    for (int i = 0; i < totalKeys; ++i)
    {
        if (isBlackKey(i))
        {
            auto bounds = getKeyBounds(i);
            if (bounds.contains(pos))
                return i;
        }
    }
    
    // Then check white keys
    for (int i = 0; i < totalKeys; ++i)
    {
        if (!isBlackKey(i))
        {
            auto bounds = getKeyBounds(i);
            if (bounds.contains(pos))
                return i;
        }
    }
    
    return -1;
}

void MarbleKeyboardComponent::mouseDown(const juce::MouseEvent& event)
{
    int keyIndex = getKeyForPosition(event.position);
    if (keyIndex >= 0)
    {
        int midiNote = (baseOctave * 12) + keyIndex;
        currentMouseNote = midiNote;
        keyboardState.noteOn(1, midiNote, 0.8f);
    }
}

void MarbleKeyboardComponent::mouseUp(const juce::MouseEvent& /*event*/)
{
    if (currentMouseNote >= 0)
    {
        keyboardState.noteOff(1, currentMouseNote, 0.0f);
        currentMouseNote = -1;
    }
}

void MarbleKeyboardComponent::mouseDrag(const juce::MouseEvent& event)
{
    int keyIndex = getKeyForPosition(event.position);
    int newNote = keyIndex >= 0 ? (baseOctave * 12) + keyIndex : -1;
    
    if (newNote != currentMouseNote)
    {
        if (currentMouseNote >= 0)
            keyboardState.noteOff(1, currentMouseNote, 0.0f);
        
        if (newNote >= 0)
            keyboardState.noteOn(1, newNote, 0.8f);
        
        currentMouseNote = newNote;
    }
}

void MarbleKeyboardComponent::drawMarbleTexture(juce::Graphics& g, juce::Rectangle<float> bounds, bool isWhite, bool isPressed)
{
    juce::Random rand(42);  // Fixed seed for consistent marble pattern
    
    if (isWhite)
    {
        // White marble base
        juce::Colour baseColour = isPressed 
            ? DragonLookAndFeel::getMarbleGrey() 
            : DragonLookAndFeel::getMarbleWhite();
        
        juce::ColourGradient gradient(baseColour, bounds.getX(), bounds.getY(),
                                       baseColour.darker(0.15f), bounds.getX(), bounds.getBottom(), false);
        gradient.addColour(0.5, baseColour.darker(0.05f));
        g.setGradientFill(gradient);
        g.fillRect(bounds);
        
        // Marble veins
        g.setColour(DragonLookAndFeel::getMarbleVein().withAlpha(0.15f));
        
        for (int i = 0; i < 8; ++i)
        {
            juce::Path vein;
            float startX = bounds.getX() + rand.nextFloat() * bounds.getWidth();
            float startY = bounds.getY() + rand.nextFloat() * bounds.getHeight();
            
            vein.startNewSubPath(startX, startY);
            
            for (int j = 0; j < 4; ++j)
            {
                float ctrlX = startX + (rand.nextFloat() - 0.5f) * 30;
                float ctrlY = startY + rand.nextFloat() * 40;
                float endX = startX + (rand.nextFloat() - 0.5f) * 20;
                float endY = startY + rand.nextFloat() * 50;
                
                vein.quadraticTo(ctrlX, ctrlY, endX, endY);
                startX = endX;
                startY = endY;
            }
            
            g.strokePath(vein, juce::PathStrokeType(0.5f + rand.nextFloat() * 1.5f));
        }
        
        // Subtle highlights
        if (isPressed)
        {
            g.setColour(juce::Colours::black.withAlpha(0.1f));
            g.fillRect(bounds);
        }
        else
        {
            juce::ColourGradient highlight(juce::Colours::white.withAlpha(0.3f), 
                                            bounds.getCentreX(), bounds.getY(),
                                            juce::Colours::transparentWhite, 
                                            bounds.getCentreX(), bounds.getY() + 30, false);
            g.setGradientFill(highlight);
            g.fillRect(bounds.withHeight(30));
        }
    }
    else
    {
        // Black marble base
        juce::Colour baseColour = isPressed 
            ? DragonLookAndFeel::getMarbleBlack().brighter(0.2f)
            : DragonLookAndFeel::getMarbleBlack();
        
        juce::ColourGradient gradient(baseColour.brighter(0.1f), bounds.getX(), bounds.getY(),
                                       baseColour.darker(0.1f), bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill(gradient);
        g.fillRect(bounds);
        
        // Dark marble veins (subtle)
        g.setColour(DragonLookAndFeel::getMarbleVein().darker(0.3f).withAlpha(0.3f));
        
        for (int i = 0; i < 4; ++i)
        {
            juce::Path vein;
            float startX = bounds.getX() + rand.nextFloat() * bounds.getWidth();
            float startY = bounds.getY() + rand.nextFloat() * bounds.getHeight() * 0.6f;
            
            vein.startNewSubPath(startX, startY);
            
            for (int j = 0; j < 3; ++j)
            {
                float ctrlX = startX + (rand.nextFloat() - 0.5f) * 15;
                float ctrlY = startY + rand.nextFloat() * 20;
                float endX = startX + (rand.nextFloat() - 0.5f) * 10;
                float endY = startY + rand.nextFloat() * 25;
                
                vein.quadraticTo(ctrlX, ctrlY, endX, endY);
                startX = endX;
                startY = endY;
            }
            
            g.strokePath(vein, juce::PathStrokeType(0.5f + rand.nextFloat() * 0.5f));
        }
        
        // Glossy top highlight
        if (!isPressed)
        {
            juce::ColourGradient highlight(juce::Colours::white.withAlpha(0.15f), 
                                            bounds.getCentreX(), bounds.getY(),
                                            juce::Colours::transparentWhite, 
                                            bounds.getCentreX(), bounds.getY() + 15, false);
            g.setGradientFill(highlight);
            g.fillRect(bounds.withHeight(15));
        }
    }
}

void MarbleKeyboardComponent::drawWhiteKey(juce::Graphics& g, juce::Rectangle<float> bounds, bool isPressed, int /*keyIndex*/)
{
    auto keyBounds = bounds.reduced(1, 0);
    
    // Shadow
    if (!isPressed)
    {
        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.fillRoundedRectangle(keyBounds.translated(2, 2), 4);
    }
    
    // Main key body with marble texture
    drawMarbleTexture(g, keyBounds, true, isPressed);
    
    // 3D effect - side shadows
    g.setColour(DragonLookAndFeel::getMarbleGrey().withAlpha(0.5f));
    g.fillRect(keyBounds.getX(), keyBounds.getY(), 2.0f, keyBounds.getHeight());
    g.fillRect(keyBounds.getRight() - 2, keyBounds.getY(), 2.0f, keyBounds.getHeight());
    
    // Bottom edge
    g.setColour(DragonLookAndFeel::getMarbleGrey());
    g.fillRoundedRectangle(keyBounds.getX(), keyBounds.getBottom() - 8, keyBounds.getWidth(), 8, 4);
    
    // Border
    g.setColour(DragonLookAndFeel::getMetallicHighlight().withAlpha(0.5f));
    g.drawRoundedRectangle(keyBounds, 2, 1);
    
    // Glow when pressed
    if (isPressed)
    {
        g.setColour(DragonLookAndFeel::getDragonIce().withAlpha(0.3f));
        g.drawRoundedRectangle(keyBounds, 2, 3);
    }
}

void MarbleKeyboardComponent::drawBlackKey(juce::Graphics& g, juce::Rectangle<float> bounds, bool isPressed, int /*keyIndex*/)
{
    auto keyBounds = bounds.reduced(0, 0);
    
    // Shadow
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.fillRoundedRectangle(keyBounds.translated(3, 3), 3);
    
    // Main key body with black marble
    juce::Path keyPath;
    keyPath.addRoundedRectangle(keyBounds.getX(), keyBounds.getY(), 
                                 keyBounds.getWidth(), keyBounds.getHeight(), 
                                 3, 3, false, false, true, true);
    
    g.saveState();
    g.reduceClipRegion(keyPath);
    drawMarbleTexture(g, keyBounds, false, isPressed);
    g.restoreState();
    
    // 3D beveled edge
    if (!isPressed)
    {
        // Left highlight
        juce::ColourGradient leftGrad(juce::Colours::white.withAlpha(0.1f), keyBounds.getX(), keyBounds.getY(),
                                       juce::Colours::transparentWhite, keyBounds.getX() + 5, keyBounds.getY(), false);
        g.setGradientFill(leftGrad);
        g.fillRect(keyBounds.getX(), keyBounds.getY(), 5.0f, keyBounds.getHeight());
    }
    
    // Border
    g.setColour(DragonLookAndFeel::getMetallicHighlight().withAlpha(0.4f));
    g.drawRoundedRectangle(keyBounds, 3, 1);
    
    // Glow when pressed
    if (isPressed)
    {
        g.setColour(DragonLookAndFeel::getDragonIce().withAlpha(0.5f));
        g.drawRoundedRectangle(keyBounds.expanded(1), 4, 2);
    }
}

void MarbleKeyboardComponent::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(DragonLookAndFeel::getDarkBackground());
    
    // Draw metallic frame
    auto frameBounds = getLocalBounds().toFloat().reduced(2);
    DragonLookAndFeel::drawDragonGlassPanel(g, frameBounds, 8);
    
    // Keyboard area
    auto keyboardBounds = frameBounds.reduced(8);
    
    int totalKeys = numOctaves * 12 + 1;
    
    // Draw white keys first
    for (int i = 0; i < totalKeys; ++i)
    {
        if (!isBlackKey(i))
        {
            int midiNote = (baseOctave * 12) + i;
            bool isActive = activeNotes.find(midiNote) != activeNotes.end();
            auto bounds = getKeyBounds(i);
            bounds = bounds.withX(bounds.getX() + 8).withWidth(bounds.getWidth());
            bounds = bounds.withY(bounds.getY() + 8).withHeight(bounds.getHeight() - 16);
            drawWhiteKey(g, bounds, isActive, i);
        }
    }
    
    // Draw black keys on top
    for (int i = 0; i < totalKeys; ++i)
    {
        if (isBlackKey(i))
        {
            int midiNote = (baseOctave * 12) + i;
            bool isActive = activeNotes.find(midiNote) != activeNotes.end();
            auto bounds = getKeyBounds(i);
            bounds = bounds.withX(bounds.getX() + 8);
            bounds = bounds.withY(bounds.getY() + 8).withHeight(bounds.getHeight() - 8);
            drawBlackKey(g, bounds, isActive, i);
        }
    }
    
    // Draw octave indicator
    g.setColour(DragonLookAndFeel::getTextLight());
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("Oct: " + juce::String(baseOctave) + " [/] to change", 
               getLocalBounds().removeFromBottom(20).reduced(10, 0), 
               juce::Justification::centredRight);
    
    // Draw PC keyboard hint
    g.setColour(DragonLookAndFeel::getTextDim());
    g.setFont(juce::Font(10.0f));
    g.drawText("PC Keys: Z-M (lower), Q-I (upper)", 
               getLocalBounds().removeFromBottom(20).reduced(10, 0), 
               juce::Justification::centredLeft);
}

void MarbleKeyboardComponent::resized()
{
}
