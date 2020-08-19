#include "sdl_context.hpp"
#include <iostream>
#include <SDL_image.h>
#include <stdexcept>

#include "../SDL/graphics.hpp"


namespace Term
{
	namespace SDL
	{
		Context::Context(const int width, const int height) :
			twidth(0), theight(0),
			tilemap(nullptr),
			tilemap_surface(nullptr),
			buffer(width, height)
		{
			std::cout << "Constructing Term::SDL::Context()\n";
			buffer_texture = SDL_CreateTexture(Graphics::Renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * 8, height * 8);
		}

		Context::~Context()
		{
			if (tilemap != nullptr)
				SDL_DestroyTexture(tilemap);
		}

		void Context::Tilemap(const std::string& path)
		{
			if (tilemap_surface != nullptr)
				SDL_FreeSurface(tilemap_surface);
			
			tilemap_surface = IMG_Load(path.c_str());
			if (tilemap_surface == nullptr)
				throw std::runtime_error("Error opening file: " + path);

			auto* const texture = SDL_CreateTextureFromSurface(Graphics::Renderer(), tilemap_surface);
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

		int Context::TileWidth() const
		{
			return twidth;
		}

		int Context::TileHeight() const
		{
			return theight;
		}

		void Context::Print(const Char ch, const size_t x, const size_t y) const
		{
			// Convert the ASCII value to tilemap coordinates.
			Sint16 tilex = (ch.Ascii() % 16) * TileWidth();
			Sint16 tiley = (ch.Ascii() / 16) * TileHeight();
			SDL_Rect tile = {
				tilex, tiley,
				static_cast<Uint16>(TileWidth()),
				static_cast<Uint16>(TileHeight()) };
			SDL_Rect dst = {
				static_cast<Sint16>(x * TileWidth()),
				static_cast<Sint16>(y * TileHeight()),
				TileWidth(),TileHeight() };

			auto bg = toSDLColor(ch.BgColor());
			auto fg = toSDLColor(ch.FgColor());

			SDL_RenderFillRect(Graphics::Renderer(), &dst);
			Graphics::SetDrawColor(bg.r, bg.g, bg.b, bg.a);
			SDL_RenderFillRect(Graphics::Renderer(), &dst);
			SDL_SetTextureColorMod(tilemap, fg.r, fg.g, fg.b);
			SDL_RenderCopyEx(Graphics::Renderer(), tilemap, &tile, &dst, 0, nullptr, SDL_FLIP_NONE);
		}

		void Context::Print()
		{
			PROFILE_FUNCTION();

			if (!buffer.IsDirty())
				return;

			const auto l = Graphics::GetCurrentLayer();
			SDL_SetRenderTarget(Graphics::Renderer(), buffer_texture);
			SDL_Texture *rt = SDL_GetRenderTarget(Graphics::Renderer());
			Graphics::TargetClear();
			
			for (auto y = 0; y < buffer.Height(); ++y)
				for (auto x = 0; x < buffer.Width(); ++x)
					Print(buffer.Get(x, y), x, y);

			Graphics::RenderTarget(l);
			buffer.Clean();
		}

		void Context::Render(const int x, const int y) const
		{
			SDL_Rect srcRect = {0, 0, buffer.Width() * TileWidth(), buffer.Height() * TileHeight()};
			SDL_Rect dstRect = {x, y, buffer.Width() * TileWidth(), buffer.Height() * TileHeight()};

			Graphics::RenderToLayer(Layer::UI, buffer_texture, &srcRect, &dstRect, SDL_FLIP_NONE);
		}
		
		ConsoleBuffer& Context::Framebuffer()
		{
			return buffer;
		}
	}
}
