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
      gainLabel("", "Gain (dB)"),
      splitLabel("", "Split Freq (Hz)"),
      saturateLabel("", "Saturate (%)"),
      nameLAF(uiBase),
      inputGainSlider("IN", base),
      outputGainSlider("OUT", base),
      lowSplitSlider("Low", base),
      highSplitSlider("High", base),
      warmSlider("Warm", base),
      curveSlider("Curve", base) {
    // init sliders
    std::array<std::string, 6> rotarySliderID{
        zlDSP::inputGain::ID, zlDSP::outputGain::ID,
        zlDSP::lowSplit::ID, zlDSP::highSplit::ID,
        zlDSP::warm::ID, zlDSP::curve::ID
    };

    zlPanel::attach({
                        &inputGainSlider.getSlider1(), &outputGainSlider.getSlider1(),
                        &lowSplitSlider.getSlider1(), &highSplitSlider.getSlider1(),
                        &warmSlider.getSlider1(), &curveSlider.getSlider1()
                    },
                    {
                        zlDSP::inputGain::ID, zlDSP::outputGain::ID,
                        zlDSP::lowSplit::ID, zlDSP::highSplit::ID,
                        zlDSP::warm::ID, zlDSP::curve::ID
                    }, parametersRef, sliderAttachments);

    for (const juce::String &visibleChangeID: visibleChangeIDs) {
        handleParameterChanges(visibleChangeID, parametersRef.getRawParameterValue(visibleChangeID)->load());
        parametersRef.addParameterListener(visibleChangeID, this);
    }

    nameLAF.setFontScale(1.2f);
    for (auto &l: {&gainLabel, &splitLabel, &saturateLabel}) {
        l->setLookAndFeel(&nameLAF);
        addAndMakeVisible(l);
    }

    for (auto &s: {
             &inputGainSlider, &outputGainSlider,
             &lowSplitSlider, &highSplitSlider,
             &warmSlider, &curveSlider
         }) {
        s->getLabelLAF().setFontScale(1.5f);
        addAndMakeVisible(s);
    }
}

ControlPanel::~ControlPanel() {
    for (auto &l: {&gainLabel, &splitLabel, &saturateLabel}) {
        l->setLookAndFeel(nullptr);
    }
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

    grid.templateRows = {Track(Fr(35)), Track(Fr(100)), Track(Fr(100))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;

    items.add(gainLabel);
    items.add(splitLabel);
    items.add(saturateLabel);
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
    if (parameterID == zlDSP::bandSplit::ID) {
        const auto f = static_cast<bool>(newValue);
        lowSplitSlider.setEditable(f);
        highSplitSlider.setEditable(f);
    }
}

void ControlPanel::handleAsyncUpdate() {
    repaint();
}
