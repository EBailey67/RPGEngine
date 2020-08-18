#include "sdl_context.hpp"
#include <iostream>
#include <SDL_image.h>
#include <stdexcept>

#include "../SDL/graphics.hpp"


namespace Term
{
	namespace SDL
	{
		Context::Context(size_t width, size_t height) :
			twidth(0), theight(0),
			tilemap(nullptr),
			tilemapSurface(nullptr),
			buffer(width, height)
		{
			std::cout << "Constructing Term::SDL::Context()\n";
		}

		Context::~Context()
		{
			if (tilemap != nullptr)
				SDL_DestroyTexture(tilemap);
		}

		void Context::Tilemap(const std::string& path)
		{
			if (tilemapSurface != nullptr)
				SDL_FreeSurface(tilemapSurface);
			
			tilemapSurface = IMG_Load(path.c_str());
			if (tilemapSurface == nullptr)
				throw std::runtime_error("Error opening file: " + path);

			auto* const texture = SDL_CreateTextureFromSurface(Graphics::Renderer(), tilemapSurface);
			if (tilemap != nullptr)
				SDL_DestroyTexture(tilemap);
			tilemap = texture;
			SDL_QueryTexture(texture, nullptr, nullptr, &twidth, &theight);
			twidth /= 16;
			theight /= 16;
		}


		SDL_Texture* Context::Tilemap() const
		{
			return tilemap;
		}

		PixDim Context::TileWidth() const
		{
			return twidth;
		}

		PixDim Context::TileHeight() const
		{
			return theight;
		}

		void Context::Print(const Char ch, const size_t x, const size_t y) const
		{
			//if (ch.ASCII() == 0)
			//	return;
			
			// Convert the ASCII value to tilemap coordinates.
			Sint16 tilex = (ch.ASCII() % 16) * TileWidth();
			Sint16 tiley = (ch.ASCII() / 16) * TileHeight();
			SDL_Rect tile = {
				tilex, tiley,
				static_cast<Uint16>(TileWidth()),
				static_cast<Uint16>(TileHeight()) };
			SDL_Rect dst = {
				static_cast<Sint16>(x * TileWidth()),
				static_cast<Sint16>(y * TileHeight()),
				TileWidth(),TileHeight() };

			auto bg = toSDLColor(ch.PriColor());
			auto fg = toSDLColor(ch.SecColor());

			Graphics::SetDrawColor(bg.r, bg.g, bg.b, bg.a);
			Graphics::DrawFillRectToLayer(Layer::UI, &dst);
			SDL_SetTextureColorMod(tilemap, fg.r, fg.g, fg.b);
			Graphics::RenderToLayer(Layer::UI, tilemap, &tile, &dst, SDL_FLIP_NONE);
		}

		void Context::Print() const
		{
			PROFILE_FUNCTION();

			//Graphics::SetDrawColor(0, 0, 0, 255);
			//SDL_Rect r{0, 0, buffer.Width() * TileWidth(), buffer.Height() * TileHeight()};
			//Graphics::DrawFillRectToLayer(Layer::UI, &r);
			
			for (size_t y = 0; y < buffer.Height(); ++y)
				for (size_t x = 0; x < buffer.Width(); ++x)
					Print(buffer.Get(x, y), x, y);
		}
		
		Buffer& Context::Framebuffer()
		{
			return buffer;
		}
	}
}
