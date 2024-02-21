/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "controller_attach.hpp"

namespace zlDSP {
    template<typename FloatType>
    ControllerAttach<FloatType>::ControllerAttach(juce::AudioProcessor &processor,
                                                  juce::AudioProcessorValueTreeState &parameters,
                                                  Controller<FloatType> &controller)
        : processorRef(processor),
          parametersRef(parameters),
          controllerRef(controller) {
        for (size_t i = 0; i < defaultVs.size(); ++i) {
            handleParameterChange(IDs[i], defaultVs[i]);
        }
        for (auto &ID: IDs) {
            parametersRef.addParameterListener(ID, this);
        }
    }

    template<typename FloatType>
    ControllerAttach<FloatType>::~ControllerAttach() {
        for (auto &ID: IDs) {
            parametersRef.removeParameterListener(ID, this);
        }
    }


    template<typename FloatType>
    void ControllerAttach<FloatType>::parameterChanged(const juce::String &parameterID, float newValue) {
        handleParameterChange(parameterID, newValue);
    }

    template<typename FloatType>
    void ControllerAttach<FloatType>::handleParameterChange(const juce::String &parameterID, float newValue) {
        auto value = static_cast<FloatType>(newValue);
        if (parameterID == inputGain::ID) {
            controllerRef.setInGain(value);
        } else if (parameterID == outputGain::ID) {
            controllerRef.setOutGain(value);
        } else if (parameterID == wet::ID) {
            controllerRef.getShaper().setWet(wet::formatV(value));
        } else if (parameterID == warm::ID) {
            controllerRef.getShaper().setWarm(warm::formatV(value));
        } else if (parameterID == curve::ID) {
            controllerRef.getShaper().setCurve(curve::formatV(value));
        } else if (parameterID == lowSplit::ID) {
            controllerRef.getSplitter().setLowFreq(value);
        } else if (parameterID == highSplit::ID) {
            controllerRef.getSplitter().setHighFreq(value);
        } else if (parameterID == effectOff::ID) {
            controllerRef.enableShaper(!static_cast<bool>(value));
        } else if (parameterID == bandSplit::ID) {
            controllerRef.enableSplitter(static_cast<bool>(value));
        } else if (parameterID == overSample::ID) {
            controllerRef.setOverSampleID(static_cast<size_t>(value));
            triggerAsyncUpdate();
        }
    }

    template<typename FloatType>
    void ControllerAttach<FloatType>::handleAsyncUpdate() {
        processorRef.setLatencySamples(controllerRef.getLatency());
    }

    template
    class ControllerAttach<float>;

    template
    class ControllerAttach<double>;
} // zlDSP
