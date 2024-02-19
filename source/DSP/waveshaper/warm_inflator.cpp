//
// Created by Zishu Liu on 2/18/24.
//

#include "warm_inflator.hpp"

namespace zlWaveShaper {
    template<typename FloatType>
    void WarmInflator<FloatType>::prepare(const juce::dsp::ProcessSpec &spec) {
        juce::ignoreUnused(spec);
    }

    template<typename FloatType>
    void WarmInflator<FloatType>::process(juce::AudioBuffer<FloatType> &buffer) {
        juce::ScopedLock lock(paraUpdateLock);
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            auto *pointer = buffer.getWritePointer(channel);

            for (size_t i = 0; i < static_cast<size_t>(buffer.getNumSamples()); ++i) {
                pointer[i] = shape(pointer[i]);
            }
        }
    }

    template<typename FloatType>
    void WarmInflator<FloatType>::setCurve(FloatType x) {
        juce::ScopedLock lock(paraUpdateLock);
        pShaper.setParameters(x);
        nShaper.setParameters(x);
    }

    template<typename FloatType>
    void WarmInflator<FloatType>::setWarm(FloatType x) {
        juce::ScopedLock lock(paraUpdateLock);
        warm = x;
    }

    template
    class WarmInflator<float>;

    template
    class WarmInflator<double>;
} // zlWaveShaper
