/*
==============================================================================
Copyright (C) 2024 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLWarm_CONTROLLER_HPP
#define ZLWarm_CONTROLLER_HPP

#include "waveshaper/waveshaper.hpp"
#include "meter/meter.hpp"
#include "splitter/splitter.hpp"

namespace zlDSP {
    template<typename FloatType>
    class Controller {
    public:
        Controller() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);

        void process(juce::AudioBuffer<FloatType> &buffer);

        zlMeter::SingleMeter<FloatType> &getInMeter() { return inMeter; }

        zlMeter::SingleMeter<FloatType> &getOutMeter() { return outMeter; }

        zlSplitter::IIRSplitter<FloatType> &getSplitter() { return splitter; }

        zlWaveShaper::WarmInflator<FloatType> &getShaper() { return shaper; }

        inline void setWet(const FloatType x) {
            juce::ScopedLock lock(mixerLock);
            mixer.setWetMixProportion(x);
        }

        inline void setInGain(const FloatType x) {
            juce::ScopedLock lock(inGainLock);
            inGain.setGainDecibels(x);
        }

        inline void setOutGain(const FloatType x) {
            juce::ScopedLock lock(outGainLock);
            outGain.setGainDecibels(x);
        }
        void setOverSampleID(size_t idx);

    private:
        zlMeter::SingleMeter<FloatType> inMeter, outMeter;
        zlSplitter::IIRSplitter<FloatType> splitter;
        zlWaveShaper::WarmInflator<FloatType> shaper;

        juce::dsp::DryWetMixer<FloatType> mixer;
        juce::CriticalSection mixerLock;

        juce::dsp::Gain<FloatType> inGain, outGain;
        juce::CriticalSection inGainLock, outGainLock;

        std::array<std::unique_ptr<juce::dsp::Oversampling<FloatType>>, 5> overSamplers{};
        std::array<int, 5> overSampleRate = {1, 2, 4, 8, 16};
        size_t oversampleID {0};
        juce::CriticalSection oversampleLock;
        juce::dsp::ProcessSpec spec;
    };
} // zlDSP

#endif //ZLWarm_CONTROLLER_HPP
