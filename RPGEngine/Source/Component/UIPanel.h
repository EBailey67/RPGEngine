#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "../config.hpp"
#include "../sdl/graphics.hpp"
#include "tileset.hpp"
#include "vector2d.hpp"


struct UIPanel
{
    SDL_Texture* texture = nullptr;
    SDL_Rect rect{};
    Vector2D scale{ 1, 1 };
    std::size_t layer = 0;

    bool isHidden = false;
    SDL_Color color{ 0, 0, 0, SDL_ALPHA_OPAQUE };

    SDL_Texture* texture;
    SDL_Rect src_rect{ 0, 0, 0, 0 };
    SDL_Rect dst_rect;

    void AssignTexture(SDL_Texture* texture)
    {
        this->texture = texture;
        SDL_QueryTexture(this->texture, nullptr, nullptr, &src_rect.w, &src_rect.h);
        SDL_QueryTexture(this->texture, nullptr, nullptr, &dst_rect.w, &dst_rect.h);
    }


};
