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

ControlPanel::ControlPanel(juce::AudioProcessorValueTreeState &apvts, zlinterface::UIBase &base) {
    // init sliders
    std::array<std::string, 6> rotarySliderID{zldsp::inputGain::ID, zldsp::outputGain::ID,
                                              zldsp::lowSplit::ID, zldsp::highSplit::ID,
                                              zldsp::warm::ID, zldsp::curve::ID};
    zlpanel::attachSliders<zlinterface::RotarySliderComponent, 6>(*this, rotarySliderList, sliderAttachments,
                                                                  rotarySliderID,
                                                                  apvts, base);
    parameters = &apvts;

    for (const juce::String &visibleChangeID: visibleChangeIDs) {
        handleParameterChanges(visibleChangeID, parameters->getRawParameterValue(visibleChangeID)->load());
        parameters->addParameterListener(visibleChangeID, this);
    }
}

ControlPanel::~ControlPanel() {
    for (const juce::String &visibleChangeID: visibleChangeIDs) {
        parameters->removeParameterListener(visibleChangeID, this);
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

    items.add(*inputGainSlider);
    items.add(*lowSplitSlider);
    items.add(*warmSlider);
    items.add(*outputGainSlider);
    items.add(*highSplitSlider);
    items.add(*curveSlider);

    grid.items = items;
    grid.performLayout(getLocalBounds());
}

void ControlPanel::parameterChanged(const juce::String &parameterID, float newValue) {
    handleParameterChanges(parameterID, newValue);
    triggerAsyncUpdate();
}

void ControlPanel::handleParameterChanges(const juce::String &parameterID, float newValue) {
    if (parameterID == zldsp::bandSplit::ID) {
        auto f = static_cast<bool>(newValue);
        lowSplitSlider->setEditable(f);
        highSplitSlider->setEditable(f);
    }
}

void ControlPanel::handleAsyncUpdate() {
    repaint();
}