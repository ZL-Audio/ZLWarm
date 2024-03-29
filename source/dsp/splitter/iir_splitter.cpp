/*
==============================================================================
Copyright (C) 2024 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "iir_splitter.hpp"

namespace zlSplitter {
    template<typename FloatType>
    void IIRSplitter<FloatType>::reset() {
        for (auto &f: {&low1, &low2, &high1, &high2, &all1, &all2}) {
            f->reset();
        }
    }

    template<typename FloatType>
    void IIRSplitter<FloatType>::prepare(const juce::dsp::ProcessSpec &spec) {
        lBuffer.setSize(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize));
        mBuffer.setSize(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize));
        hBuffer.setSize(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize));
        for (auto &f: {&low1, &low2, &high1, &high2, &all1, &all2}) {
            f->prepare(spec);
        }
        low1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        low2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        high1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        high2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        all1.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
        all2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    }

    template<typename FloatType>
    void IIRSplitter<FloatType>::split(juce::dsp::AudioBlock<FloatType> block) {
        auto inputBlock = block;
        lBuffer.setSize(static_cast<int>(block.getNumChannels()), static_cast<int>(block.getNumSamples()),
            true, false, true);
        mBuffer.setSize(static_cast<int>(block.getNumChannels()), static_cast<int>(block.getNumSamples()),
            true, false, true);
        hBuffer.setSize(static_cast<int>(block.getNumChannels()), static_cast<int>(block.getNumSamples()),
            true, false, true);
        auto lBlock = juce::dsp::AudioBlock<FloatType>(lBuffer);
        auto mBlock = juce::dsp::AudioBlock<FloatType>(mBuffer);
        auto hBlock = juce::dsp::AudioBlock<FloatType>(hBuffer);

        lBlock.copyFrom(inputBlock);
        mBlock.copyFrom(inputBlock);

        auto lContext = juce::dsp::ProcessContextReplacing<FloatType>(lBlock);
        auto mContext = juce::dsp::ProcessContextReplacing<FloatType>(mBlock);
        auto hContext = juce::dsp::ProcessContextReplacing<FloatType>(hBlock);

        juce::ScopedLock lock(paraLock);
        low1.process(lContext);
        all2.process(lContext);

        high1.process(mContext);
        hBlock.copyFrom(mBlock);

        low2.process(mContext);
        high2.process(hContext);
    }

    template<typename FloatType>
    void IIRSplitter<FloatType>::combine(juce::dsp::AudioBlock<FloatType> block) {
        auto outputBlock = block;

        auto lBlock = juce::dsp::AudioBlock<FloatType>(lBuffer);
        auto mBlock = juce::dsp::AudioBlock<FloatType>(mBuffer);
        auto hBlock = juce::dsp::AudioBlock<FloatType>(hBuffer);

        outputBlock.copyFrom(lBlock);
        outputBlock.add(mBlock);
        outputBlock.add(hBlock);
    }

    template<typename FloatType>
    void IIRSplitter<FloatType>::setLowFreq(FloatType freq) {
        juce::ScopedLock lock(paraLock);
        for (auto &f: {&low1, &high1, &all1}) {
            f->setCutoffFrequency(freq);
        }
    }

    template<typename FloatType>
    void IIRSplitter<FloatType>::setHighFreq(FloatType freq) {
        juce::ScopedLock lock(paraLock);
        for (auto &f: {&low2, &high2, &all2}) {
            f->setCutoffFrequency(freq);
        }
    }

    template
    class IIRSplitter<float>;

    template
    class IIRSplitter<double>;
} // zlSplitter
