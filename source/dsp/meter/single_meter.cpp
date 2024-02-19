/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "single_meter.hpp"

namespace zlMeter {
    template<typename FloatType>
    void SingleMeter<FloatType>::prepare(const juce::dsp::ProcessSpec &spec) {
        sampleRate.store(static_cast<FloatType>(spec.sampleRate));
        maxPeak.resize(static_cast<size_t>(spec.numChannels));
        bufferPeak.resize(static_cast<size_t>(spec.numChannels));
        tempPeak.resize(static_cast<size_t>(spec.numChannels));
        for (size_t idx = 0; idx < maxPeak.size(); ++idx) {
            maxPeak[idx].store(-120.f);
            bufferPeak[idx].store(-120.f);
        }
    }

    template<typename FloatType>
    template<typename ProcessContext>
    void SingleMeter<FloatType>::process(const ProcessContext &context) {
        juce::dsp::AudioBlock<FloatType> outputBlock = context.getOutputBlock();
        if (!isON.load()) return;
        const auto decay = decayRate.load() * static_cast<FloatType>(outputBlock.getNumSamples()) / sampleRate.load();

        std::fill(tempPeak.begin(), tempPeak.end(), FloatType(0));
        for (size_t channel = 0; channel < maxPeak.size(); ++channel) {
            for (size_t idx = 0; idx < outputBlock.getNumSamples(); ++idx) {
                tempPeak[channel] = std::max(tempPeak[channel],
                                             std::abs(outputBlock.getSample(
                                                 static_cast<int>(channel), static_cast<int>(idx))));
            }
        }

        for (size_t channel = 0; channel < maxPeak.size(); ++channel) {
            tempPeak[channel] = juce::Decibels::gainToDecibels(tempPeak[channel]);
            bufferPeak[channel].store(std::max(bufferPeak[channel].load() * decay, tempPeak[channel]));
            maxPeak[channel].store(std::max(maxPeak[channel].load(), tempPeak[channel]));
        }
    }

    template
    class SingleMeter<float>;

    template
    class SingleMeter<double>;
} // zlMeter
