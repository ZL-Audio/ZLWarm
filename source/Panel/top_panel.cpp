/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "top_panel.h"

TopPanel::TopPanel(PluginProcessor &p,
                   zlinterface::UIBase &base) :
        logoPanel(p, base) {
    uiBase = &base;
    // init buttons
    std::array<std::string, 2> buttonID{zldsp::effectIn::ID, zldsp::bandSplit::ID};
    zlpanel::attachButtons(*this, buttonList, buttonAttachments, buttonID, p.parameters, base);
    // init combobox
    std::array<std::string, 1> comboboxID{"over_sample"};
    zlpanel::attachBoxes(*this, comboBoxList, comboboxAttachments, comboboxID, p.parameters, base);
    addAndMakeVisible(logoPanel);
}

TopPanel::~TopPanel() = default;

void TopPanel::paint(juce::Graphics &g) { juce::ignoreUnused(g); }

void TopPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(*effectButton);
    items.add(*splitButton);
    items.add(*sampleRateCombobox);

    grid.items = items;
    grid.performLayout(getLocalBounds());
}