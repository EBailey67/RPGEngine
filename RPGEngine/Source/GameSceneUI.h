#pragma once

#include <iostream>

#include "sdlgui/screen.h"
#include "sdlgui/window.h"
#include "sdlgui/layout.h"
#include "sdlgui/label.h"
#include "sdlgui/checkbox.h"
#include "sdlgui/button.h"
#include "sdlgui/toolbutton.h"
#include "sdlgui/popupbutton.h"
#include "sdlgui/combobox.h"
#include "sdlgui/dropdownbox.h"
#include "sdlgui/progressbar.h"
#include "sdlgui/entypo.h"
#include "sdlgui/messagedialog.h"
#include "sdlgui/textbox.h"
#include "sdlgui/slider.h"
#include "sdlgui/imagepanel.h"
#include "sdlgui/imageview.h"
#include "sdlgui/vscrollpanel.h"
#include "sdlgui/colorwheel.h"
#include "sdlgui/graph.h"
#include "sdlgui/tabwidget.h"
#include "sdlgui/switchbox.h"
#include "sdlgui/formhelper.h"

#include <SDL.h>
#include "SDL/graphics.hpp"


namespace GUI
{
    class GameSceneUI : public Screen
    {
    public:
        GameSceneUI(SDL_Window* pwindow, int rwidth, int rheight);
        ~GameSceneUI() = default;

        bool keyboardEvent(const int key, const int scancode, const int action, const int modifiers) override
        {
            if (Screen::keyboardEvent(key, scancode, action, modifiers))
                return true;

            //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            // {
            //    setVisible(false);
            //    return true;
            //}
            return false;
        }

        void draw(SDL_Renderer* renderer) override;
        void drawContents() override  {}
    private:
        std::vector<SDL_Texture*> m_images_data;
        int m_current_image;
    };
}
