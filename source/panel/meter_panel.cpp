/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "meter_panel.h"

MeterPanel::MeterPanel(zlDSP::Controller<float> &controller, zlInterface::UIBase &base)
    : uiBase(base),
      inPanel(controller.getInMeter(), base),
      outPanel(controller.getOutMeter(), base),
      inLabel("", "IN"),
      outLabel("", "OUT"),
      labelLAF(uiBase) {
    addAndMakeVisible(inPanel);
    addAndMakeVisible(outPanel);

    labelLAF.setJustification(juce::Justification::centred);
    labelLAF.setFontScale(1.25f);
    inLabel.setLookAndFeel(&labelLAF);
    outLabel.setLookAndFeel(&labelLAF);
    addAndMakeVisible(inLabel);
    addAndMakeVisible(outLabel);
    startTimerHz(30);
}

MeterPanel::~MeterPanel() {
    stopTimer();
    inLabel.setLookAndFeel(nullptr);
    outLabel.setLookAndFeel(nullptr);
}

void MeterPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    bound.removeFromRight(uiBase.getFontSize() * .5f);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(11)), Track(Fr(150))};
    grid.templateColumns = {Track(Fr(6)), Track(Fr(6))};

    grid.items = {
        juce::GridItem(inLabel).withArea(1, 1, 2, 2),
        juce::GridItem(outLabel).withArea(1, 2, 2, 3),
        juce::GridItem(inPanel).withArea(2, 1, 3, 2),
        juce::GridItem(outPanel).withArea(2, 2, 3, 3),
    };
    grid.setGap(juce::Grid::Px(uiBase.getFontSize() * .25f));
    grid.performLayout(bound.toNearestInt());
}

void MeterPanel::timerCallback() {
    inPanel.repaint();
    outPanel.repaint();
}
