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

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

namespace zlMeter {
    template<typename FloatType>
    class SingleMeter {
    public:
        SingleMeter() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);

        void process(juce::AudioBuffer<FloatType> &buffer);

        std::vector<std::atomic<FloatType>> &getmaxPeak() { return maxPeak; }

        std::vector<std::atomic<FloatType>> &getBufferPeak() { return bufferPeak; }

    private:
        std::vector<std::atomic<FloatType>> maxPeak, bufferPeak;
        std::vector<FloatType> tempPeak;
        std::atomic<FloatType> decayRate, sampleRate;
        std::atomic<bool> isON = false;
    };
} // zlMeter

#endif //ZL_SINGLE_METER_HPP
