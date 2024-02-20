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
    void WarmInflator<FloatType>::process(juce::dsp::AudioBlock<FloatType> block) {
        juce::ScopedLock lock(paraUpdateLock);
        for (int i = 0; i < static_cast<int>(block.getNumChannels()); ++i) {
            for (int j = 0; j < static_cast<int>(block.getNumSamples()); ++j) {
                const auto x = block.getSample(i, j);
                block.setSample(i, j, wet * shape(x) + (1 - wet) * x);
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

    template<typename FloatType>
    void WarmInflator<FloatType>::setWet(FloatType x) {
        juce::ScopedLock lock(paraUpdateLock);
        wet = x;
    }

    template
    class WarmInflator<float>;

    template
    class WarmInflator<double>;
} // zlWaveShaper
