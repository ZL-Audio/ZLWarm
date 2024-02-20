/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "single_meter_panel.hpp"

namespace zlPanel {
    SingleMeterPanel::SingleMeterPanel(zlMeter::SingleMeter<float> &meter, zlInterface::UIBase &base)
        : m(meter), uiBase(base),
          scalePanel(uiBase) {
        m.enable(true);
        setInterceptsMouseClicks(true, false);
        addAndMakeVisible(scalePanel);
    }

    SingleMeterPanel::~SingleMeterPanel() {
        m.enable(false);
    }

    void SingleMeterPanel::paint(juce::Graphics &g) {
        juce::ScopedLock lock(m.getLock());
        const auto &maxPeak = m.getmaxPeak();
        const auto &bufferPeak = m.getBufferPeak();
        auto rectBound = getLocalBounds().toFloat();
        auto labelBound = rectBound.removeFromTop(rectBound.getHeight() * labelPortion);
        const auto width = rectBound.getWidth() / static_cast<float>(maxPeak.size());
        // draw max peak labels
        g.setFont(uiBase.getFontSize() * 1.125f);
        auto mmaxPeak = -160.f;
        for (auto &v: maxPeak) {
            mmaxPeak = std::max(mmaxPeak, v.load());
        }
        if (mmaxPeak > minDB) {
            if (mmaxPeak >= 0.f) {
                g.setColour(uiBase.getTextColor());
            } else {
                g.setColour(uiBase.getTextInactiveColor());
            }
            g.drawText(juce::String(mmaxPeak).substring(0, 5),
                       labelBound, juce::Justification::centredTop);
        } else {
            g.drawText("-inf", labelBound, juce::Justification::centred);
        }
        // draw rectangles
        g.setColour(uiBase.getTextInactiveColor());
        for (size_t i = 0; i < bufferPeak.size(); ++i) {
            auto currentBound = rectBound.removeFromLeft(width);
            currentBound = currentBound.withSizeKeepingCentre(
                currentBound.getWidth() - paddingPortion * uiBase.getFontSize(),
                currentBound.getHeight());
            const auto peakPortion = juce::jlimit(
                -1.f, 1.f, (bufferPeak[i].load() - minDB) / (maxDB - minDB));
            if (peakPortion > 0.f) {
                juce::Rectangle<float> boundToFill{
                    currentBound.getX(), currentBound.getY() + (1 - peakPortion) * currentBound.getHeight(),
                    currentBound.getWidth(), peakPortion * currentBound.getHeight()
                };
                path.clear();
                if (i == 0) {
                    path.addRoundedRectangle(
                        boundToFill.getX(), boundToFill.getY(),
                        boundToFill.getWidth(), boundToFill.getHeight(),
                        uiBase.getFontSize() * .5f, uiBase.getFontSize() * .5f,
                        false, false, true, false);
                } else if (i == bufferPeak.size() - 1) {
                    path.addRoundedRectangle(
                        boundToFill.getX(), boundToFill.getY(),
                        boundToFill.getWidth(), boundToFill.getHeight(),
                        uiBase.getFontSize() * .5f, uiBase.getFontSize() * .5f,
                        false, false, false, true);
                } else {
                    path.addRectangle(boundToFill);
                }
                g.fillPath(path);
            }
        }
    }

    void SingleMeterPanel::mouseDown(const juce::MouseEvent &event) {
        juce::ignoreUnused(event);
        m.reset();
    }

    void SingleMeterPanel::resized() {
        scalePanel.setBounds(getLocalBounds());
    }
} // zlPanel
