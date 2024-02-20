/*
==============================================================================
Copyright (C) 2024 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLWarm_IIR_SPLITTER_HPP
#define ZLWarm_IIR_SPLITTER_HPP

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace zlSplitter {
    /**
     * a three-band Linkwitz-Riley filter
     * @tparam FloatType
     */
    template<typename FloatType>
    class IIRSplitter {
    public:
        IIRSplitter() = default;

        void reset();

        void prepare(const juce::dsp::ProcessSpec &spec);

        /**
         * split the audio buffer into internal low, mid and high buffers
         * @param block
         */
        void split(juce::dsp::AudioBlock<FloatType> block);

        /**
         * combine the internal low, mid and high buffers into the audio buffer
         * @param block
         */
        void combine(juce::dsp::AudioBlock<FloatType> block);

        void setLowFreq(FloatType freq);

        void setHighFreq(FloatType freq);

        inline juce::AudioBuffer<FloatType> &getLBuffer() { return lBuffer; }

        inline juce::AudioBuffer<FloatType> &getMBuffer() { return mBuffer; }

        inline juce::AudioBuffer<FloatType> &getHBuffer() { return hBuffer; }

    private:
        juce::AudioBuffer<FloatType> lBuffer, mBuffer, hBuffer;
        juce::dsp::LinkwitzRileyFilter<FloatType> low1, high1, all1;
        juce::dsp::LinkwitzRileyFilter<FloatType> low2, high2, all2;

        juce::CriticalSection paraLock;
    };
} // zlSplitter

#endif //ZLWarm_IIR_SPLITTER_HPP
