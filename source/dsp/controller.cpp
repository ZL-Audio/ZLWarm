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
        mainSpec = spec;
        setOverSampleID(oversampleID);
    }

    template<typename FloatType>
    void Controller<FloatType>::reset() {
        // inGain.reset();
        // outGain.reset();
        // mixer.reset();
        // splitter.reset();
        // for (size_t i = 0; i < overSamplers.size(); ++i) {
        //     overSamplers[i].reset();
        // }
    }

    template<typename FloatType>
    void Controller<FloatType>::process(juce::AudioBuffer<FloatType> &buffer) {
        juce::dsp::AudioBlock<FloatType> block(buffer);
        juce::dsp::ProcessContextReplacing<FloatType> context(block);
        {
            inGain.process(context);
        }
        inMeter.process(block);
        {
            juce::ScopedLock lock1(oversampleLock);
            auto oversampled_block =
                   overSamplers[oversampleID]->processSamplesUp(context.getInputBlock());

            if (isSplitterON.load()) {
                splitter.split(oversampled_block);

                auto lBlock = juce::dsp::AudioBlock<FloatType>(splitter.getLBuffer());
                auto mBlock = juce::dsp::AudioBlock<FloatType>(splitter.getMBuffer());
                auto hBlock = juce::dsp::AudioBlock<FloatType>(splitter.getHBuffer());

                if (isShaperON.load()) {
                    shaper.process(lBlock);
                    shaper.process(mBlock);
                    shaper.process(hBlock);
                }

                splitter.combine(oversampled_block);
            } else {
                if (isShaperON.load()) {
                    shaper.process(oversampled_block);
                }
            }
            overSamplers[oversampleID]->processSamplesDown(block);
        }
        {
            juce::ScopedLock lock(outGainLock);
            outGain.process(context);
        }
        outMeter.process(block);
    }

    template<typename FloatType>
    void Controller<FloatType>::setOverSampleID(const size_t idx) {
        juce::ScopedLock lock(oversampleLock);
        oversampleID = idx;
        juce::dsp::ProcessSpec overSampleSpec{
            mainSpec.sampleRate * static_cast<double>(overSampleRate[idx]),
            mainSpec.maximumBlockSize * static_cast<juce::uint32>(overSampleRate[idx]),
            mainSpec.numChannels
        };
        shaper.prepare(overSampleSpec);
        splitter.prepare(overSampleSpec);
        if (overSamplers[oversampleID] != nullptr) {
            latency.store(static_cast<int>(overSamplers[oversampleID]->getLatencyInSamples()));
        }
    }

    template
    class Controller<float>;

    template
    class Controller<double>;
} // zlDSP
