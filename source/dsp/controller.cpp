/*
==============================================================================
Copyright (C) 2024 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "controller.hpp"

namespace zlDSP {
    template<typename FloatType>
    void Controller<FloatType>::prepare(const juce::dsp::ProcessSpec &spec) {
        inMeter.prepare(spec);
        outMeter.prepare(spec);
        {
            juce::ScopedLock lock(inGainLock);
            inGain.prepare(spec);
        }
        {
            juce::ScopedLock lock(outGainLock);
            outGain.prepare(spec);
        }
        {
            juce::ScopedLock lock(oversampleLock);
            for (size_t i = 0; i < overSamplers.size(); ++i) {
                overSamplers[i] = std::make_unique<juce::dsp::Oversampling<FloatType >>(
                        spec.numChannels, i,
                        juce::dsp::Oversampling<FloatType>::filterHalfBandFIREquiripple, true,
                        true);
                overSamplers[i]->initProcessing(spec.maximumBlockSize);
            }
        }
        setOverSampleID(0);
    }

    template<typename FloatType>
    void Controller<FloatType>::process(juce::AudioBuffer<FloatType> &buffer) {

    }

    template<typename FloatType>
    void Controller<FloatType>::setOverSampleID(const size_t idx) {
        juce::ScopedLock lock(oversampleLock);
        oversampleID = idx;
        juce::dsp::ProcessSpec overSampleSpec{
            spec.sampleRate * static_cast<double>(overSampleRate[idx]),
            spec.maximumBlockSize * static_cast<juce::uint32>(overSampleRate[idx]),
            spec.numChannels
        };
        shaper.prepare(overSampleSpec);
        splitter.prepare(overSampleSpec);
        mixer.prepare(overSampleSpec);
    }
} // zlDSP
