/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLWarm_CONTROLLER_ATTACH_HPP
#define ZLWarm_CONTROLLER_ATTACH_HPP

#include "controller.hpp"
#include "dsp_definitions.hpp"

namespace zlDSP {
    template<typename FloatType>
    class ControllerAttach : private juce::AudioProcessorValueTreeState::Listener,
    private juce::AsyncUpdater {
    public:
        explicit ControllerAttach(juce::AudioProcessor &processor,
                                  juce::AudioProcessorValueTreeState &parameters,
                                  Controller<FloatType> &controller);

        ~ControllerAttach() override;

    private:
        juce::AudioProcessor &processorRef;
        juce::AudioProcessorValueTreeState &parametersRef;
        Controller<FloatType> &controllerRef;

        constexpr static std::array IDs{
            inputGain::ID, outputGain::ID, wet::ID,
            warm::ID, curve::ID,
            lowSplit::ID, highSplit::ID,
            effectOff::ID, bandSplit::ID, overSample::ID
        };

        inline static std::array defaultVs {
            static_cast<float>(inputGain::defaultV), static_cast<float>(outputGain::defaultV),
            static_cast<float>(wet::defaultV),
            static_cast<float>(warm::defaultV), static_cast<float>(curve::defaultV),
            static_cast<float>(lowSplit::defaultV), static_cast<float>(highSplit::defaultV),
            static_cast<float>(effectOff::defaultV), static_cast<float>(bandSplit::defaultV),
            static_cast<float>(overSample::defaultI)
        };

        void parameterChanged(const juce::String &parameterID, float newValue) override;

        void handleParameterChange(const juce::String &parameterID, float newValue);

        void handleAsyncUpdate() override;
    };
} // zlDSP

#endif //ZLWarm_CONTROLLER_ATTACH_HPP
