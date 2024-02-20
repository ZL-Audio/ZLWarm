/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_METERPANEL_H
#define ZLINFLATOR_METERPANEL_H

#include "../dsp/dsp.hpp"
#include "../gui/gui.hpp"
#include "meter_panel/single_meter_panel.hpp"

class MeterPanel final : public juce::Component, private juce::Timer {
public:
    explicit MeterPanel(zlDSP::Controller<float> &controller,
                        zlInterface::UIBase &base);

    ~MeterPanel() override;

    void resized() override;

private:
    zlInterface::UIBase &uiBase;
    zlPanel::SingleMeterPanel inPanel, outPanel;
    juce::Label inLabel, outLabel;
    zlInterface::NameLookAndFeel labelLAF;

    void timerCallback() override;
};

#endif //ZLINFLATOR_METERPANEL_H
