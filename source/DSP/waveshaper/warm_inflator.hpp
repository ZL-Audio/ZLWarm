//
// Created by Zishu Liu on 2/18/24.
//

#ifndef ZLWarm_WARM_INFLATOR_HPP
#define ZLWarm_WARM_INFLATOR_HPP

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

namespace zlWaveShaper {
    template<typename FloatType>
    class WarmInflator {
    private:
        class PositiveShaper {
        public:
            inline FloatType shape(FloatType x) const { return basic(x); }

            void setParameters(FloatType curve) {
                a = FloatType(0.25) * (curve - 1);
                b = FloatType(0.5) * (curve - 1);
                c = FloatType(0.75) - FloatType(1.75) * curve;
                d = curve + 1;
                auto_compensate = 1 / (FloatType(0.5625) * curve + FloatType(1.125));
            }

        private:
            FloatType a, b, c, d, auto_compensate;

            inline FloatType basic(FloatType x) const { return auto_compensate * x * (d + x * (c + x * (b + a * x))); }
        };

        class NegativeShaper {
        public:
            FloatType shape(FloatType x) const { return basic(x); }

            void setParameters(FloatType curve) {
                a = FloatType(1.35);
                b = FloatType(-3.35) + FloatType(0.75) * curve;
                c = FloatType(1.95) - FloatType(1.75) * curve;
                d = curve + 1;
                auto_compensate = 1 / (FloatType(0.5625) * curve + FloatType(1.125));
            }

        private:
            FloatType a, b, c, d, auto_compensate;

            FloatType basic(FloatType x) const { return auto_compensate * x * (d + x * (c + x * (b + a * x))); }
        };

    public:
        WarmInflator() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);

        void process(juce::AudioBuffer<FloatType> &buffer);

        void setCurve(FloatType x);

        void setWarm(FloatType x);

        void enable(const bool x) { isON.store(x); }

    private:
        PositiveShaper pShaper;
        NegativeShaper nShaper;
        FloatType warm;
        std::atomic<bool> isON;

        juce::CriticalSection paraUpdateLock;

        inline FloatType shape(FloatType x) {
            return (x > 0
                        ? pShaper.shape(std::abs(x))
                        : warm * nShaper.shape(std::abs(x)) + (1 - warm) * pShaper.shape(std::abs(x)));
        }
    };
} // zlWaveShaper

#endif //ZLWarm_WARM_INFLATOR_HPP
