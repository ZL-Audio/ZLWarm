/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor(
        BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
),
#endif
          dummyProcessor(),
          parameters(*this, nullptr, juce::Identifier("ZLWarmParameters"), zldsp::getParameterLayout()),
          states(dummyProcessor, nullptr, juce::Identifier("ZLWarmStates"), zlstate::getParameterLayout()),
          waveShaper(*this),
          waveShaperAttach(waveShaper, parameters) {
    inGain.setGainDecibels(zldsp::inputGain::defaultV);
    outGain.setGainDecibels(zldsp::outputGain::defaultV);
    parameters.addParameterListener(zldsp::inputGain::ID, this);
    parameters.addParameterListener(zldsp::outputGain::ID, this);
    waveShaperAttach.addListeners();
}

PluginProcessor::~PluginProcessor() = default;

//==============================================================================
const juce::String PluginProcessor::getName() const {
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const { return 0.0; }

int PluginProcessor::getNumPrograms() {
    return 1;
}

int PluginProcessor::getCurrentProgram() { return 0; }

void PluginProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void PluginProcessor::changeProgramName(int index,
                                        const juce::String &newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate,
                                    int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need...
    reset();
    auto channels = static_cast<juce::uint32> (juce::jmin(getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32> (samplesPerBlock), channels};

    inGain.prepare(spec);
    outGain.prepare(spec);
    meterIn.prepare(spec);
    meterOut.prepare(spec);
    waveShaper.prepare(spec);
}

void PluginProcessor::reset() {
    inGain.reset();
    outGain.reset();
    meterIn.reset();
    meterOut.reset();
    waveShaper.reset();
}

void PluginProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations

bool PluginProcessor::isBusesLayoutSupported(
        const BusesLayout &layouts) const {
    if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet())
        return false;
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

#endif

void PluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                   juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    inGain.process(juce::dsp::ProcessContextReplacing<float>(block));
    meterIn.process(juce::dsp::ProcessContextReplacing<float>(block));
    waveShaper.process(juce::dsp::ProcessContextReplacing<float>(block));
    outGain.process(juce::dsp::ProcessContextReplacing<float>(block));
    meterOut.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool PluginProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation(
        juce::MemoryBlock &destData) {
    auto tempTree = juce::ValueTree("ZLWarmParaState");
    tempTree.appendChild(parameters.copyState(), nullptr);
    std::unique_ptr<juce::XmlElement> xml(tempTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void *data,
                                          int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName("ZLWarmParaState")) {
        auto tempTree = juce::ValueTree::fromXml(*xmlState);
        parameters.replaceState(tempTree.getChildWithName(parameters.state.getType()));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

MeterSource<float> *PluginProcessor::getInputMeterSource() {
    return &meterIn;
}

MeterSource<float> *PluginProcessor::getOutputMeterSource() {
    return &meterOut;
}

shaper::ShaperMixer<float> *PluginProcessor::getShaperMixer() {
    return waveShaper.getShaper();
}

void PluginProcessor::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID.equalsIgnoreCase(zldsp::inputGain::ID)) {
        inGain.setGainDecibels(newValue);
    } else if (parameterID.equalsIgnoreCase(zldsp::outputGain::ID)) {
        outGain.setGainDecibels(newValue);
    }
}