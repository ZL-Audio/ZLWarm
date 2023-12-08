/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_SHAPERFUNCTIONS_H
#define ZLINFLATOR_SHAPERFUNCTIONS_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include "dsp_defines.h"

namespace shaper {
    enum ShaperType {
        cubic,
        quartic,
        ShaperNUM
    };

    template<typename FloatType>
    class Shaper {
    public:
        Shaper() = default;

        virtual ~Shaper() = default;

        FloatType operator()(FloatType x) const {
            return shape(x);
        }

        virtual void setParameters(FloatType curve) = 0;

    private:
        virtual FloatType basic(FloatType x) const = 0;

        virtual FloatType shape(FloatType x) const = 0;
    };

    template<typename FloatType>
    class CubicShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType curve) override {
            a = FloatType(1.35);
            b = FloatType(-3.35) + FloatType(0.75) * curve;
            c = FloatType(1.95) - FloatType(1.75) * curve;
            d = curve + 1;
            auto_compensate = 1 / (FloatType(0.5625) * curve + FloatType(1.125));
        }

    private:
        FloatType a, b, c, d, auto_compensate;

        FloatType basic(FloatType x) const override { return auto_compensate * x * (d + x * (c + x * (b + a * x))); }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

    template<typename FloatType>
    class QuarticShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType curve) override {
            a = FloatType(0.25) * (curve - 1);
            b = FloatType(0.5) * (curve - 1);
            c = FloatType(0.75) - FloatType(1.75) * curve;
            d = curve + 1;
            auto_compensate = 1 / (FloatType(0.5625) * curve + FloatType(1.125));
        }

    private:
        FloatType a, b, c, d, auto_compensate;

        FloatType basic(FloatType x) const override { return auto_compensate * x * (d + x * (c + x * (b + a * x))); }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

    template<typename FloatType>
    static std::unique_ptr<Shaper<FloatType>> makeShaper(ShaperType type) {
        if (type == ShaperType::cubic) {
            return std::make_unique<CubicShaper<FloatType>>();
        } else {
            return std::make_unique<QuarticShaper<FloatType>>();
        }
    }

    template<typename FloatType>
    class ShaperMixer {
    public:
        ShaperMixer() {
            pShaper = makeShaper<FloatType>(ShaperType::quartic);
            nShaper = makeShaper<FloatType>(ShaperType::cubic);
            setShapes(zldsp::curve::formatV(zldsp::curve::defaultV));
        }

        ~ShaperMixer() = default;

        FloatType operator()(FloatType x) const {
            return shape(x);
        }

        void setShapes(FloatType curve) {
            pShaper->setParameters(curve);
            nShaper->setParameters(curve);
        }

        void setWarm(FloatType value) {
            warm = value;
        }

    private:
        std::unique_ptr<Shaper<FloatType>> pShaper, nShaper;
        float warm = zldsp::warm::formatV(zldsp::warm::defaultV);

        FloatType shape(FloatType x) const {
            return (x > 0 ? (*pShaper)(std::abs(x)) : warm * (*nShaper)(std::abs(x)) +
                                                      (1 - warm) * (*pShaper)(std::abs(x)));
        }
    };
} // namespace shaper

#endif // ZLINFLATOR_SHAPERFUNCTIONS_H
