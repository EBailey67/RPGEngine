#pragma once

#include <string>
#include "UIElement.h"


class UILabel : public UIElement
{
public:
    UILabel(int x, int y, std::string text);
    ~UILabel();

    void SetText(std::string text);

    SDL_Color color{ 0, 0, 0, SDL_ALPHA_OPAQUE };
    void OnRender() override;

    UIFont font{ UIFont::Default };
private:
    std::string m_text;
};

