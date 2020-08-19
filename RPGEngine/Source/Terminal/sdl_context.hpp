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

		class Context : public Term::Context
		{
		public:
			Context(size_t width, size_t height);
			~Context() override;

			void Tilemap(const std::string& path);
			[[nodiscard]] SDL_Texture* Tilemap() const;
			[[nodiscard]] PixDim TileWidth() const;
			[[nodiscard]] PixDim TileHeight() const;
			void Print(Char ch, size_t x, size_t y) const override;
			void Print() override;
			void Render(int x, int y) const;

			Buffer& Framebuffer() override;

		private:
			PixDim twidth, theight;
			SDL_Texture* tilemap;
			SDL_Surface* tilemap_surface;
			SDL_Texture* buffer_texture;
			StaticBuffer buffer;
		};
	}
}
