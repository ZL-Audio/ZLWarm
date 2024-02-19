/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "main_panel.h"

MainPanel::MainPanel(PluginProcessor &p,
                     MeterSource<float> *input,
                     MeterSource<float> *output) :
        uiBase(),
        controlPanel(p.parameters, uiBase),
        topPanel(p, uiBase),
        // meterPanel(input, output, uiBase),
        logoPanel(p, uiBase) {
    addAndMakeVisible(controlPanel);
    addAndMakeVisible(topPanel);
    // addAndMakeVisible(meterPanel);
    logoPanel.setJustification(juce::Justification::centredLeft);
    addAndMakeVisible(logoPanel);
}

MainPanel::~MainPanel() = default;

void MainPanel::paint(juce::Graphics &g) {
    g.fillAll(uiBase.getBackgroundColor());
    auto bound = getLocalBounds().toFloat();
    float fontSize = bound.getHeight() * 0.048f;
    bound = uiBase.fillRoundedShadowRectangle(g, bound, fontSize * 0.5f, {});
    uiBase.fillRoundedInnerShadowRectangle(g, bound, fontSize * 0.5f, {.blurRadius=0.45f, .flip=true});
}

void MainPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    auto fontSize = bound.getWidth() * 0.033838297872340425f;
    bound = zlInterface::UIBase::getRoundedShadowRectangleArea(bound, fontSize * 0.5f, {});
    bound = zlInterface::UIBase::getRoundedShadowRectangleArea(bound, fontSize * 0.5f, {});

    uiBase.setFontSize(fontSize);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(10)), Track(Fr(48))};
    grid.templateColumns = {Track(Fr(6)), Track(Fr(15))};

    grid.items = {
            juce::GridItem(logoPanel).withArea(1, 1, 2, 2),
            // juce::GridItem(meterPanel).withArea(2, 1, 3, 2),
            juce::GridItem(topPanel).withArea(1, 2, 2, 3),
            juce::GridItem(controlPanel).withArea(2, 2, 3, 3),
    };

    grid.performLayout(bound.toNearestInt());
}