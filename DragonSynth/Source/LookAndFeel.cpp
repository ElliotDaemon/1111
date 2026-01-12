#include "LookAndFeel.h"

DragonLookAndFeel::DragonLookAndFeel()
{
    // Set default colors
    setColour(juce::ResizableWindow::backgroundColourId, getDarkBackground());
    setColour(juce::Slider::rotarySliderFillColourId, getDragonIce());
    setColour(juce::Slider::thumbColourId, getMetallicShine());
    setColour(juce::ComboBox::backgroundColourId, getDragonGlass());
    setColour(juce::ComboBox::textColourId, getTextLight());
    setColour(juce::ComboBox::outlineColourId, getMetallicHighlight());
    setColour(juce::PopupMenu::backgroundColourId, getDragonGlass());
    setColour(juce::PopupMenu::textColourId, getTextLight());
    setColour(juce::PopupMenu::highlightedBackgroundColourId, getDragonGlassLight());
    setColour(juce::Label::textColourId, getTextLight());
    setColour(juce::TextButton::buttonColourId, getDragonGlass());
    setColour(juce::TextButton::textColourOnId, getTextLight());
    setColour(juce::TextButton::textColourOffId, getTextLight());
}

juce::ColourGradient DragonLookAndFeel::createDragonGlassGradient(juce::Rectangle<float> bounds)
{
    juce::ColourGradient gradient(getDragonGlassLight(), bounds.getX(), bounds.getY(),
                                   getDarkBackground(), bounds.getX(), bounds.getBottom(), false);
    gradient.addColour(0.3, getDragonGlass());
    gradient.addColour(0.7, getDragonGlass().darker(0.2f));
    return gradient;
}

void DragonLookAndFeel::drawDragonGlassPanel(juce::Graphics& g, juce::Rectangle<float> bounds, float cornerSize)
{
    // Main glass panel with gradient
    auto gradient = createDragonGlassGradient(bounds);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    // Inner glow/reflection
    juce::Path innerGlow;
    innerGlow.addRoundedRectangle(bounds.reduced(2), cornerSize - 2);
    
    juce::ColourGradient innerGradient(getMetallicHighlight().withAlpha(0.3f), 
                                        bounds.getCentreX(), bounds.getY(),
                                        juce::Colours::transparentBlack, 
                                        bounds.getCentreX(), bounds.getCentreY(), false);
    g.setGradientFill(innerGradient);
    g.fillPath(innerGlow);
    
    // Metallic border
    g.setColour(getMetallicHighlight());
    g.drawRoundedRectangle(bounds, cornerSize, 1.5f);
    
    // Outer glow
    g.setColour(getDragonIce().withAlpha(0.1f));
    g.drawRoundedRectangle(bounds.expanded(1), cornerSize + 1, 1.0f);
}

void DragonLookAndFeel::drawMetallicKnob(juce::Graphics& g, juce::Rectangle<float> bounds, float angle)
{
    auto centre = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Outer ring shadow
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillEllipse(bounds.translated(2, 2));
    
    // Base metallic layer
    juce::ColourGradient baseGradient(getMetallicShine(), centre.x, bounds.getY(),
                                       getDragonGlass(), centre.x, bounds.getBottom(), false);
    g.setGradientFill(baseGradient);
    g.fillEllipse(bounds);
    
    // Inner darker circle
    auto innerBounds = bounds.reduced(radius * 0.15f);
    juce::ColourGradient innerGradient(getDragonGlassLight(), centre.x, innerBounds.getY(),
                                        getDarkBackground(), centre.x, innerBounds.getBottom(), false);
    g.setGradientFill(innerGradient);
    g.fillEllipse(innerBounds);
    
    // Glowing ring for value indication
    juce::Path valuePath;
    float startAngle = juce::MathConstants<float>::pi * 1.25f;
    float endAngle = startAngle + angle * juce::MathConstants<float>::pi * 1.5f;
    
    valuePath.addCentredArc(centre.x, centre.y, radius * 0.85f, radius * 0.85f, 
                            0, startAngle, endAngle, true);
    
    g.setColour(getDragonIce());
    g.strokePath(valuePath, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, 
                                                   juce::PathStrokeType::rounded));
    
    // Glow effect
    g.setColour(getDragonIceGlow().withAlpha(0.3f));
    g.strokePath(valuePath, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved, 
                                                   juce::PathStrokeType::rounded));
    
    // Pointer line
    float pointerAngle = startAngle + angle * juce::MathConstants<float>::pi * 1.5f - juce::MathConstants<float>::pi / 2.0f;
    float pointerLength = radius * 0.5f;
    
    juce::Point<float> pointerEnd(centre.x + std::cos(pointerAngle) * pointerLength,
                                   centre.y + std::sin(pointerAngle) * pointerLength);
    
    g.setColour(getTextLight());
    g.drawLine(centre.x, centre.y, pointerEnd.x, pointerEnd.y, 2.0f);
    
    // Center cap
    auto capBounds = bounds.reduced(radius * 0.6f);
    juce::ColourGradient capGradient(getMetallicShine(), centre.x, capBounds.getY(),
                                      getMetallicHighlight(), centre.x, capBounds.getBottom(), false);
    g.setGradientFill(capGradient);
    g.fillEllipse(capBounds);
    
    // Metallic rim
    g.setColour(getMetallicHighlight());
    g.drawEllipse(bounds, 2.0f);
}

void DragonLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPosProportional, float /*rotaryStartAngle*/,
                                          float /*rotaryEndAngle*/, juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y), 
                                          static_cast<float>(width), static_cast<float>(height));
    auto knobBounds = bounds.reduced(4);
    
    // Make it square and centered
    auto size = juce::jmin(knobBounds.getWidth(), knobBounds.getHeight());
    knobBounds = knobBounds.withSizeKeepingCentre(size, size);
    
    drawMetallicKnob(g, knobBounds, sliderPosProportional);
}

void DragonLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                          const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                          static_cast<float>(width), static_cast<float>(height));
    
    bool isVertical = style == juce::Slider::LinearVertical || style == juce::Slider::LinearBarVertical;
    
    // Track background
    auto trackBounds = isVertical 
        ? bounds.withWidth(6).withX(bounds.getCentreX() - 3)
        : bounds.withHeight(6).withY(bounds.getCentreY() - 3);
    
    g.setColour(getDarkBackground());
    g.fillRoundedRectangle(trackBounds, 3);
    
    g.setColour(getMetallicHighlight());
    g.drawRoundedRectangle(trackBounds, 3, 1);
    
    // Filled portion
    auto filledBounds = trackBounds;
    if (isVertical)
    {
        filledBounds = filledBounds.withTop(sliderPos);
    }
    else
    {
        filledBounds = filledBounds.withRight(sliderPos);
    }
    
    juce::ColourGradient fillGradient(getDragonIce(), filledBounds.getX(), filledBounds.getY(),
                                       getDragonIceGlow(), filledBounds.getRight(), filledBounds.getBottom(), false);
    g.setGradientFill(fillGradient);
    g.fillRoundedRectangle(filledBounds, 3);
    
    // Thumb
    if (!slider.isBar())
    {
        auto thumbSize = isVertical ? width * 0.8f : height * 0.8f;
        auto thumbBounds = isVertical
            ? juce::Rectangle<float>(bounds.getCentreX() - thumbSize / 2, sliderPos - thumbSize / 2, thumbSize, thumbSize)
            : juce::Rectangle<float>(sliderPos - thumbSize / 2, bounds.getCentreY() - thumbSize / 2, thumbSize, thumbSize);
        
        // Thumb shadow
        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.fillEllipse(thumbBounds.translated(1, 1));
        
        // Thumb gradient
        juce::ColourGradient thumbGradient(getMetallicShine(), thumbBounds.getCentreX(), thumbBounds.getY(),
                                            getMetallicHighlight(), thumbBounds.getCentreX(), thumbBounds.getBottom(), false);
        g.setGradientFill(thumbGradient);
        g.fillEllipse(thumbBounds);
        
        g.setColour(getMetallicHighlight());
        g.drawEllipse(thumbBounds, 1.5f);
    }
}

void DragonLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                                      int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                                      juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float>(0, 0, static_cast<float>(width), static_cast<float>(height));
    
    // Background
    auto gradient = createDragonGlassGradient(bounds);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, 6);
    
    // Border
    g.setColour(getMetallicHighlight());
    g.drawRoundedRectangle(bounds.reduced(0.5f), 6, 1.5f);
    
    // Arrow
    juce::Path arrow;
    auto arrowBounds = bounds.removeFromRight(30).reduced(10);
    arrow.addTriangle(arrowBounds.getX(), arrowBounds.getY(),
                      arrowBounds.getRight(), arrowBounds.getY(),
                      arrowBounds.getCentreX(), arrowBounds.getBottom());
    
    g.setColour(box.isEnabled() ? getTextLight() : getTextDim());
    g.fillPath(arrow);
}

void DragonLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
    auto bounds = juce::Rectangle<float>(0, 0, static_cast<float>(width), static_cast<float>(height));
    
    g.fillAll(getDragonGlass());
    
    // Inner gradient
    auto gradient = createDragonGlassGradient(bounds);
    g.setGradientFill(gradient);
    g.fillRect(bounds);
    
    // Border
    g.setColour(getMetallicHighlight());
    g.drawRect(bounds, 1.5f);
}

void DragonLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                                           bool /*isSeparator*/, bool isActive, bool isHighlighted,
                                           bool isTicked, bool /*hasSubMenu*/,
                                           const juce::String& text, const juce::String& /*shortcutKeyText*/,
                                           const juce::Drawable* /*icon*/, const juce::Colour* /*textColour*/)
{
    auto bounds = area.toFloat().reduced(2);
    
    if (isHighlighted && isActive)
    {
        g.setColour(getDragonGlassLight());
        g.fillRoundedRectangle(bounds, 4);
        
        g.setColour(getDragonIce().withAlpha(0.3f));
        g.drawRoundedRectangle(bounds, 4, 1);
    }
    
    g.setColour(isActive ? getTextLight() : getTextDim());
    
    auto textBounds = bounds.reduced(10, 0);
    
    if (isTicked)
    {
        g.setColour(getDragonIce());
        g.fillEllipse(bounds.getX() + 8, bounds.getCentreY() - 4, 8, 8);
        textBounds = textBounds.withLeft(textBounds.getX() + 15);
    }
    
    g.setFont(14.0f);
    g.drawText(text, textBounds, juce::Justification::centredLeft);
}

void DragonLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));
    
    if (!label.isBeingEdited())
    {
        auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
        
        g.setColour(label.findColour(juce::Label::textColourId).withAlpha(label.isEnabled() ? 1.0f : 0.5f));
        g.setFont(label.getFont());
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                         juce::jmax(1, static_cast<int>(textArea.getHeight() / label.getFont().getHeight())),
                         label.getMinimumHorizontalScale());
    }
}

void DragonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                              const juce::Colour& /*backgroundColour*/,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1);
    
    // Base color
    juce::Colour baseColour = getDragonGlass();
    
    if (shouldDrawButtonAsDown)
        baseColour = getDragonGlassLight();
    else if (shouldDrawButtonAsHighlighted)
        baseColour = getDragonGlass().brighter(0.1f);
    
    if (button.getToggleState())
        baseColour = getDragonIce().withAlpha(0.3f);
    
    // Gradient fill
    juce::ColourGradient gradient(baseColour.brighter(0.1f), bounds.getCentreX(), bounds.getY(),
                                   baseColour.darker(0.1f), bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, 6);
    
    // Border
    g.setColour(button.getToggleState() ? getDragonIce() : getMetallicHighlight());
    g.drawRoundedRectangle(bounds, 6, 1.5f);
    
    // Glow if toggled
    if (button.getToggleState())
    {
        g.setColour(getDragonIceGlow().withAlpha(0.2f));
        g.drawRoundedRectangle(bounds.expanded(1), 7, 2.0f);
    }
}

void DragonLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                        bool /*shouldDrawButtonAsHighlighted*/,
                                        bool /*shouldDrawButtonAsDown*/)
{
    auto bounds = button.getLocalBounds();
    
    g.setColour(button.getToggleState() ? getDragonIce() : getTextLight());
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
}
