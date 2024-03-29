/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor &p) :
        AudioProcessorEditor(&p), processorRef(p),
        property(p.states),
        mainPanel(p) {

    for (auto &ID: IDs) {
        processorRef.states.addParameterListener(ID, this);
    }
    // set font
    auto sourceCodePro = juce::Typeface::createSystemTypefaceFor(BinaryData::MiSansLatinMedium_ttf,
                                                                 BinaryData::MiSansLatinMedium_ttfSize);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(sourceCodePro);

    // add main panel
    addAndMakeVisible(mainPanel);

    // set size & size listener
    setResizeLimits(zlstate::windowW::minV, zlstate::windowH::minV, zlstate::windowW::maxV, zlstate::windowH::maxV);
    getConstrainer()->setFixedAspectRatio(
            static_cast<float>(zlstate::windowW::defaultV) / static_cast<float>(zlstate::windowH::defaultV));
    setResizable(true, p.wrapperType != PluginProcessor::wrapperType_AudioUnitv3);
    lastUIWidth.referTo(p.states.getParameterAsValue(zlstate::windowW::ID));
    lastUIHeight.referTo(p.states.getParameterAsValue(zlstate::windowH::ID));
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
    lastUIWidth.addListener(this);
    lastUIHeight.addListener(this);
}

PluginEditor::~PluginEditor() {
    for (auto &ID: IDs) {
        processorRef.states.removeParameterListener(ID, this);
    }
}

//==============================================================================
void PluginEditor::paint(juce::Graphics &g) {
    juce::ignoreUnused(g);
}

void PluginEditor::resized() {
    mainPanel.setBounds(getLocalBounds());
    lastUIWidth = getWidth();
    lastUIHeight = getHeight();
}

void PluginEditor::valueChanged(juce::Value &) {
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
}

void PluginEditor::parameterChanged(const juce::String &parameterID, float newValue) {
    juce::ignoreUnused(parameterID, newValue);
    triggerAsyncUpdate();
}

void PluginEditor::handleAsyncUpdate() {
    property.saveAPVTS(processorRef.states);
    sendLookAndFeelChange();
}