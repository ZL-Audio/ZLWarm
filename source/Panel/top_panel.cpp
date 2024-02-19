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

#include "BinaryData.h"

TopPanel::TopPanel(PluginProcessor &p, zlInterface::UIBase &base)
    : uiBase(base),
      wetS("Wet", base),
      bypassC("", base),
      bandSplitC("", base),
      oversampleC("", zlDSP::overSample::choices, base),
      oversampleL("", "OS:"),
      nameLAF(base),
      bypassDrawable(
          juce::Drawable::createFromImageData(BinaryData::shutdownline_svg, BinaryData::shutdownline_svgSize)),
      splitDrawable(juce::Drawable::createFromImageData(BinaryData::splitcellshorizontal_svg,
                                                        BinaryData::splitcellshorizontal_svgSize)) {
    juce::ignoreUnused(p);
    zlPanel::attach({&wetS.getSlider()}, {zlDSP::wet::ID}, p.parameters, sliderAttachments);

    bypassC.setDrawable(bypassDrawable.get());
    bandSplitC.setDrawable(splitDrawable.get());
    zlPanel::attach({&bypassC.getButton(), &bandSplitC.getButton()},
                    {zlDSP::effectIn::ID, zlDSP::bandSplit::ID},
                    p.parameters, buttonAttachments);

    nameLAF.setJustification(juce::Justification::centredRight);
    nameLAF.setFontScale(1.5f);

    oversampleL.setLookAndFeel(&nameLAF);
    zlPanel::attach({&oversampleC.getBox()}, {zlDSP::overSample::ID}, p.parameters, comboboxAttachments);

    addAndMakeVisible(wetS);
    addAndMakeVisible(bypassC);
    addAndMakeVisible(bandSplitC);
    addAndMakeVisible(oversampleL);
    addAndMakeVisible(oversampleC);
}

TopPanel::~TopPanel() {
    oversampleL.setLookAndFeel(nullptr);
}

void TopPanel::paint(juce::Graphics &g) { juce::ignoreUnused(g); }

void TopPanel::resized() {
    // nameLAF.setPadding(0.f, 0.f, uiBase.getFontSize() * .2f, 0.f);
    wetS.setPadding(uiBase.getFontSize(), 0.f);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(20)), Track(Fr(10)), Track(Fr(10)),
        Track(Fr(9)), Track(Fr(11))};

    juce::Array<juce::GridItem> items;
    items.add(wetS);
    items.add(bypassC);
    items.add(bandSplitC);
    items.add(oversampleL);
    items.add(oversampleC);

    grid.items = items;
    grid.performLayout(getLocalBounds());
}
