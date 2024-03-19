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
#include "gain/gain.hpp"

namespace zlDSP {
    template<typename FloatType>
    class Controller {
    public:
        Controller() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);

        void reset();

        void process(juce::AudioBuffer<FloatType> &buffer);

        zlMeter::SingleMeter<FloatType> &getInMeter() { return inMeter; }

        zlMeter::SingleMeter<FloatType> &getOutMeter() { return outMeter; }

        zlSplitter::IIRSplitter<FloatType> &getSplitter() { return splitter; }

        zlWaveShaper::WarmInflator<FloatType> &getShaper() { return shaper; }

        inline void setInGain(const FloatType x) {
            inGain.setGainDecibels(x);
        }

        inline void setOutGain(const FloatType x) {
            outGain.setGainDecibels(x);
        }

        void setOverSampleID(size_t idx);

        inline void enableSplitter(const bool x) { isSplitterON.store(x); }

        inline void enableShaper(const bool x) { isShaperON.store(x); }

        inline int getLatency() const { return latency.load(); }

    private:
        zlMeter::SingleMeter<FloatType> inMeter, outMeter;
        zlSplitter::IIRSplitter<FloatType> splitter;
        std::atomic<bool> isSplitterON;
        zlWaveShaper::WarmInflator<FloatType> shaper;
        std::atomic<bool> isShaperON;

        zlGain::Gain<FloatType> inGain{}, outGain{};

        std::array<std::unique_ptr<juce::dsp::Oversampling<FloatType> >, 5> overSamplers{};
        std::array<int, 5> overSampleRate = {1, 2, 4, 8, 16};
        size_t oversampleID{0};
        std::atomic<int> latency{0};
        juce::CriticalSection oversampleLock;
        juce::dsp::ProcessSpec mainSpec{44100, 512, 2};
    };
} // zlDSP

#endif //ZLWarm_CONTROLLER_HPP
