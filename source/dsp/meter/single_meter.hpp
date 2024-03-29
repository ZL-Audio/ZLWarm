/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZL_SINGLE_METER_HPP
#define ZL_SINGLE_METER_HPP

// #include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

namespace zlMeter {
    template<typename FloatType>
    class SingleMeter {
    public:
        SingleMeter() = default;

        void reset();

        void prepare(const juce::dsp::ProcessSpec &spec);

        void process(juce::dsp::AudioBlock<FloatType> block);

        std::deque<std::atomic<FloatType> > &getmaxPeak() { return maxPeak; }

        std::deque<std::atomic<FloatType> > &getBufferPeak() { return bufferPeak; }

        void enable(const bool x) { isON.store(x); }

        juce::CriticalSection &getLock() { return resizeLock; }

    private:
        std::deque<std::atomic<FloatType> > maxPeak, bufferPeak;
        std::vector<FloatType> tempPeak;
        std::atomic<FloatType> decayRate{2}, sampleRate;
        std::vector<FloatType> currentDecay;
        std::atomic<bool> isON = false;

        juce::CriticalSection resizeLock;
    };
} // zlMeter

#endif //ZL_SINGLE_METER_HPP
