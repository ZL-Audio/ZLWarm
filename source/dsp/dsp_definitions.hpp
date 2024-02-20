/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_DSP_DEFINES_H
#define ZLINFLATOR_DSP_DEFINES_H

#include <juce_audio_processors/juce_audio_processors.h>

namespace zlDSP {
    inline auto static constexpr versionHint = 1;
    // floats
    template<class T>
    class FloatParameters {
    public:
        static std::unique_ptr<juce::AudioParameterFloat> get(bool automate = true) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID, versionHint), T::name,
                                                               T::range, T::defaultV, attributes);
        }

        inline static float convertTo01(float x) {
            return T::range.convertTo0to1(x);
        }
    };

    class inputGain : public FloatParameters<inputGain> {
    public:
        auto static constexpr ID = "input_gain";
        auto static constexpr name = "IN Gain (dB)";
        inline auto static const range = juce::NormalisableRange<float>(-16.f, 16.f, .01f, 0.5, true);
        auto static constexpr defaultV = 0.0f;
    };

    class outputGain : public FloatParameters<outputGain> {
    public:
        auto static constexpr ID = "output_gain";
        auto static constexpr name = "OUT Gain (dB)";
        inline auto static const range = juce::NormalisableRange<float>(-16.f, 16.f, .01f, 0.5, true);
        auto static constexpr defaultV = 0.0f;
    };

    class wet : public FloatParameters<wet> {
    public:
        auto static constexpr ID = "wet";
        auto static constexpr name = "Wet (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 100.0f;
        inline static float formatV(const float v) {
            return v / 100.f;
        }
        inline static double formatV(const double v) {
            return v / 100.0;
        }
    };

    class curve : public FloatParameters<curve> {
    public:
        auto static constexpr ID = "curve";
        auto static constexpr name = "Curve (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 50.0f;
        inline static float formatV(const float v) {
            return v / 100.f;
        }
        inline static double formatV(const double v) {
            return v / 100.0;
        }
    };

    class warm : public FloatParameters<warm> {
    public:
        auto static constexpr ID = "warm";
        auto static constexpr name = "Warm (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 0.0f;
        inline static float formatV(const float v) {
            return v / 100.f;
        }
        inline static double formatV(const double v) {
            return v / 100.0;
        }
    };

    class lowSplit : public FloatParameters<lowSplit> {
    public:
        auto static constexpr ID = "low_split";
        auto static constexpr name = "Low Split (Hz)";
        inline auto static const range = juce::NormalisableRange<float>(20.0f, 20000.0f, .1f, 0.2f);
        auto static constexpr defaultV = 240.0f;
    };

    class highSplit : public FloatParameters<highSplit> {
    public:
        auto static constexpr ID = "high_split";
        auto static constexpr name = "High Split (Hz)";
        inline auto static const range = juce::NormalisableRange<float>(20.0f, 20000.0f, .1f, 0.2f);
        auto static constexpr defaultV = 2400.0f;
    };

    // bools
    template<class T>
    class BoolParameters {
    public:
        static std::unique_ptr<juce::AudioParameterBool> get(const bool automate = true) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID, versionHint), T::name,
                                                              T::defaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterBool> get(const juce::String &label, const bool automate = true) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(label);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID, versionHint), T::name,
                                                              T::defaultV, attributes);
        }
    };

    class effectIn : public BoolParameters<effectIn> {
    public:
        auto static constexpr ID = "effect_in";
        auto static constexpr name = "Effect In";
        auto static constexpr defaultV = true;
    };

    class bandSplit : public BoolParameters<bandSplit> {
    public:
        auto static constexpr ID = "band_split";
        auto static constexpr name = "Band Split";
        auto static constexpr defaultV = false;
    };

    // choices
    template<class T>
    class ChoiceParameters {
    public:
        static std::unique_ptr<juce::AudioParameterChoice> get(const bool automate = true) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterChoice>(
                    juce::ParameterID(T::ID, versionHint), T::name, T::choices, T::defaultI, attributes);
        }

        static std::unique_ptr<juce::AudioParameterChoice> get(const juce::String &label, const bool automate = true) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(label);
            return std::make_unique<juce::AudioParameterChoice>(
                    juce::ParameterID(T::ID, versionHint), T::name, T::choices, T::defaultI, attributes);
        }
    };

    class overSample : public ChoiceParameters<overSample> {
    public:
        auto static constexpr ID = "over_sample";
        auto static constexpr name = "Over Sampling";
        inline auto static const choices = juce::StringArray{"OFF", "2x", "4x", "8x", "16x"};
        int static constexpr defaultI = 0;
    };

    inline juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout() {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add(inputGain::get(), outputGain::get(), wet::get(),
                   warm::get(), curve::get(),
                   lowSplit::get(), highSplit::get(),
                   effectIn::get(), bandSplit::get(), overSample::get());
        return layout;
    }
}

#endif //ZLINFLATOR_DSP_DEFINES_H
