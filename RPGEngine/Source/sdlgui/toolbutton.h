/*
    sdlgui/toolbutton.h -- Simple radio+toggle button with an icon

    Based on NanoGUI by Wenzel Jakob <wenzel@inf.ethz.ch>.
    Adaptation for SDL by Dalerank <dalerankn8@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include "button.h"

namespace GUI
{

    class ToolButton : public Button
    {
    public:
        ToolButton(Widget* parent, const int icon, const std::string& caption = "", const int w = 25, const int h = 25, const int flags = Flags::RadioButton | Flags::ToggleButton)
            : Button(parent, caption, icon)
        {
            setFlags(flags);
            setFixedSize(Vector2i(w, h));
        }
    };

}
