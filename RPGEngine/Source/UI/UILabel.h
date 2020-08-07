#pragma once

#include <string>
#include "UIElement.h"


class UILabel final : public UIElement
{
public:
    UILabel(int x, int y, const std::string& text);
    ~UILabel();

    void SetText(const std::string& text);

    SDL_Color color{ 0, 0, 0, SDL_ALPHA_OPAQUE };
    void OnRender() override;

    UIFont font{ UIFont::Default };

private:
    std::string m_text;
};

