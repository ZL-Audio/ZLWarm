/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_CONTROLPANEL_H
#define ZLINFLATOR_CONTROLPANEL_H

#include "../GUI/button_component.h"
#include "../GUI/rotary_slider_component.h"
#include "../GUI/combobox_component.h"
#include "../DSP/dsp_defines.h"
#include "panel_definitions.h"
#include <juce_audio_processors/juce_audio_processors.h>

class ControlPanel : public juce::Component, public juce::AudioProcessorValueTreeState::Listener,
                     private juce::AsyncUpdater {
public:
    explicit ControlPanel(juce::AudioProcessorValueTreeState &apvts, zlinterface::UIBase &base);

    ~ControlPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    void parameterChanged(const juce::String &parameterID, float newValue) override;

private:
    std::unique_ptr<zlinterface::RotarySliderComponent> inputGainSlider, outputGainSlider;
    std::unique_ptr<zlinterface::RotarySliderComponent> lowSplitSlider, highSplitSlider, warmSlider, curveSlider;
    std::array<std::unique_ptr<zlinterface::RotarySliderComponent> *, 6> rotarySliderList{&inputGainSlider,
                                                                                          &outputGainSlider,
                                                                                          &lowSplitSlider,
                                                                                          &highSplitSlider,
                                                                                          &warmSlider,
                                                                                          &curveSlider};
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;

    juce::AudioProcessorValueTreeState *parameters;
    std::array<juce::String, 1> visibleChangeIDs = {zldsp::bandSplit::ID};

    void handleAsyncUpdate() override;

    void handleParameterChanges(const juce::String &parameterID, float newValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
};

#endif //ZLINFLATOR_CONTROLPANEL_H
