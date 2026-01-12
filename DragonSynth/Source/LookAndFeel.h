#pragma once

#include <JuceHeader.h>

class DragonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DragonLookAndFeel();
    ~DragonLookAndFeel() override = default;
    
    // Colors for the dragon glass metallic theme
    static juce::Colour getDarkBackground() { return juce::Colour(0xff0a0a12); }
    static juce::Colour getDragonGlass() { return juce::Colour(0xff1a1a2e); }
    static juce::Colour getDragonGlassLight() { return juce::Colour(0xff2a2a4e); }
    static juce::Colour getMetallicHighlight() { return juce::Colour(0xff4a4a6a); }
    static juce::Colour getMetallicShine() { return juce::Colour(0xff7a7a9a); }
    static juce::Colour getDragonFire() { return juce::Colour(0xffff4444); }
    static juce::Colour getDragonFireGlow() { return juce::Colour(0xffff6644); }
    static juce::Colour getDragonIce() { return juce::Colour(0xff44aaff); }
    static juce::Colour getDragonIceGlow() { return juce::Colour(0xff66ccff); }
    static juce::Colour getMarbleWhite() { return juce::Colour(0xffe8e8f0); }
    static juce::Colour getMarbleGrey() { return juce::Colour(0xffb8b8c8); }
    static juce::Colour getMarbleVein() { return juce::Colour(0xff505070); }
    static juce::Colour getMarbleBlack() { return juce::Colour(0xff202030); }
    static juce::Colour getTextLight() { return juce::Colour(0xffc0c0d0); }
    static juce::Colour getTextDim() { return juce::Colour(0xff707090); }
    
    // Rotary slider styling
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
    
    // Linear slider styling
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    
    // ComboBox styling
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;
    
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;
    
    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted,
                           bool isTicked, bool hasSubMenu,
                           const juce::String& text, const juce::String& shortcutKeyText,
                           const juce::Drawable* icon, const juce::Colour* textColour) override;
    
    // Label styling
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    
    // Button styling
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;
    
    // Helper methods for drawing effects
    static void drawDragonGlassPanel(juce::Graphics& g, juce::Rectangle<float> bounds, float cornerSize = 10.0f);
    static void drawMetallicKnob(juce::Graphics& g, juce::Rectangle<float> bounds, float angle);
    static juce::ColourGradient createDragonGlassGradient(juce::Rectangle<float> bounds);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonLookAndFeel)
};
