#pragma once

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include "graphics.hpp"

struct ResourceLoader
{
    static SDL_Texture *Sprite(std::string_view path)
    {
	    const auto texture = IMG_LoadTexture(Graphics::Renderer(), path.data());
        if (!texture)
        {
            SDL_THROW();
        }

        return texture;
    }

    static TTF_Font *Font(const std::string_view path, const int size)
    {
	    auto* font = TTF_OpenFont(path.data(), size);
        if (!font)
        {
            SDL_THROW();
        }

        return font;
    }

	static SDL_Texture* Glyph(TTF_Font *font, uint16_t c, const SDL_Color fgColor, const SDL_Color bgColor)
    {
	    auto* const surface = TTF_RenderGlyph_Shaded(font, c, fgColor, bgColor);
        if (!surface)
        {
            SDL_THROW();
        }

	    auto* const texture = SDL_CreateTextureFromSurface(Graphics::Renderer(), surface);
        if (!texture)
        {
            SDL_THROW();
        }
        SDL_FreeSurface(surface);
        return texture;
    }
	
    static SDL_Texture *Text(TTF_Font *font, const std::string_view text, const SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE}, const int width = -1)
    {
        SDL_Surface* surface;

        if (width != -1)
            surface = TTF_RenderUTF8_Blended_Wrapped(font, text.data(), color, width);
        else
            surface = TTF_RenderUTF8_Blended(font, text.data(), color);
    	
        if (!surface)
        {
            SDL_THROW();
        }

	    auto* const texture = SDL_CreateTextureFromSurface(Graphics::Renderer(), surface);
        if (!texture)
        {
            SDL_THROW();
        }
        SDL_FreeSurface(surface);
        return texture;
    }

    static Mix_Music *Music(const std::string_view path)
    {
	    auto* music = Mix_LoadMUS(path.data());
        if (!music)
        {
            SDL_THROW();
        }

        return music;
    }
};

