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
    template<typename ProcessContext>
    void WarmInflator<FloatType>::process(const ProcessContext &context) {
        if (isON.load()) {
            juce::ScopedLock lock(paraUpdateLock);
            auto inputBlock = context.getInputBlock();
            auto outputBlock = context.getOutputBlock();
            juce::dsp::AudioBlock<FloatType>::process(inputBlock, outputBlock, [this](FloatType x){return shape(x);});
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
