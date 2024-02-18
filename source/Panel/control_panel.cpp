/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "control_panel.h"

ControlPanel::ControlPanel(juce::AudioProcessorValueTreeState &apvts, zlInterface::UIBase &base)
    : parametersRef(apvts), uiBase(base),
      inputGainSlider("IN\n(dB)", base),
      outputGainSlider("OUT\n(dB)", base),
      lowSplitSlider("Low\n(Hz)", base),
      highSplitSlider("High\n(Hz)", base),
      warmSlider("Warm", base),
      curveSlider("Curve", base) {
    // init sliders
    std::array<std::string, 6> rotarySliderID{
        zldsp::inputGain::ID, zldsp::outputGain::ID,
        zldsp::lowSplit::ID, zldsp::highSplit::ID,
        zldsp::warm::ID, zldsp::curve::ID
    };

    zlPanel::attach({
                        &inputGainSlider.getSlider1(), &outputGainSlider.getSlider1(),
                        &lowSplitSlider.getSlider1(), &highSplitSlider.getSlider1(),
                        &warmSlider.getSlider1(), &curveSlider.getSlider1()
                    },
                    {
                        zldsp::inputGain::ID, zldsp::outputGain::ID,
                        zldsp::lowSplit::ID, zldsp::highSplit::ID,
                        zldsp::warm::ID, zldsp::curve::ID
                    }, parametersRef, sliderAttachments);

    for (const juce::String &visibleChangeID: visibleChangeIDs) {
        handleParameterChanges(visibleChangeID, parametersRef.getRawParameterValue(visibleChangeID)->load());
        parametersRef.addParameterListener(visibleChangeID, this);
    }

    inputGainSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(inputGainSlider);
    outputGainSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(outputGainSlider);
    lowSplitSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(lowSplitSlider);
    highSplitSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(highSplitSlider);
    warmSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(warmSlider);
    curveSlider.getLabelLAF().setFontScale(1.5f);
    addAndMakeVisible(curveSlider);
}

ControlPanel::~ControlPanel() {
    for (const juce::String &visibleChangeID: visibleChangeIDs) {
        parametersRef.removeParameterListener(visibleChangeID, this);
    }
}

//==============================================================================
void ControlPanel::paint(juce::Graphics &g) {
    juce::ignoreUnused(g);
}

void ControlPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1)), Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;

    items.add(inputGainSlider);
    items.add(lowSplitSlider);
    items.add(warmSlider);
    items.add(outputGainSlider);
    items.add(highSplitSlider);
    items.add(curveSlider);

    grid.items = items;
    grid.setGap(juce::Grid::Px(uiBase.getFontSize() * .5f));
    grid.performLayout(getLocalBounds());
}

void ControlPanel::parameterChanged(const juce::String &parameterID, float newValue) {
    handleParameterChanges(parameterID, newValue);
    triggerAsyncUpdate();
}

void ControlPanel::handleParameterChanges(const juce::String &parameterID, float newValue) {
    if (parameterID == zldsp::bandSplit::ID) {
        const auto f = static_cast<bool>(newValue);
        lowSplitSlider.setEditable(f);
        highSplitSlider.setEditable(f);
    }
}

void ControlPanel::handleAsyncUpdate() {
    repaint();
}
