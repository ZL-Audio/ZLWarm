/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_TOPPANEL_H
#define ZLINFLATOR_TOPPANEL_H

#include <BinaryData.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "../gui/gui.hpp"
#include "logo_panel.h"
#include "panel_definitions.h"

class TopPanel final : public juce::Component {
public:
    explicit TopPanel(PluginProcessor &p, zlInterface::UIBase &base);

    ~TopPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    zlInterface::UIBase &uiBase;

    zlInterface::CompactLinearSlider wetS;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;

    zlInterface::CompactButton bypassC, bandSplitC;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

    zlInterface::CompactCombobox oversampleC;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttachments;
    juce::Label oversampleL;
    zlInterface::NameLookAndFeel nameLAF;

    const std::unique_ptr<juce::Drawable> bypassDrawable, splitDrawable;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};

#endif //ZLINFLATOR_TOPPANEL_H
