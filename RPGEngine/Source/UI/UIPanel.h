#pragma once
#include "UIElement.h"

class UIPanel : public UIElement
{
public:
    UIPanel(int x, int y, int w, int h);
    ~UIPanel();

    SDL_Color color{ 0, 0, 0, SDL_ALPHA_OPAQUE };
    void OnRender() override;
};

