#pragma once

#include "terminate.hpp"
#include <SDL.h>

namespace Term
{
	namespace SDL
	{
		inline SDL_Color toSDLColor(const Color& c)
		{
			SDL_Color sdl;
			sdl.r = c.r;
			sdl.g = c.g;
			sdl.b = c.b;
			sdl.a = SDL_ALPHA_OPAQUE;
			
			return sdl;
		}

		class Context
		{
		public:
			Context(size_t width, size_t height);
			~Context();

			void Tilemap(const std::string& path);
			[[nodiscard]] SDL_Texture* Tilemap() const;
			[[nodiscard]] int TileWidth() const;
			[[nodiscard]] int TileHeight() const;
			void Print(Char ch, size_t x, size_t y) const;
			void Print();
			void Render(int x, int y) const;

			ConsoleBuffer& Framebuffer();

		private:
			int twidth, theight;
			SDL_Texture* tilemap;
			SDL_Surface* tilemap_surface;
			SDL_Texture* buffer_texture;
			ConsoleBuffer buffer;
		};
	}
}
