#pragma once

#include <SDL.h>

#include "Console.h"
#include "terminate.hpp"

namespace Term
{
	namespace SDL
	{
		class Context
		{
		public:
			Context(int width, int height);
			~Context();

			void Tilemap(const std::string& path);
			[[nodiscard]] SDL_Texture* Tilemap() const;
			[[nodiscard]] int TileWidth() const;
			[[nodiscard]] int TileHeight() const;
			void Print(Char ch, size_t x, size_t y) const;
			void Print();
			void Render(int x, int y) const;

			Console& GetConsole();
//			ConsoleBuffer& Framebuffer();
		private:
			int twidth, theight;
			SDL_Texture* tilemap;
			SDL_Surface* tilemap_surface;
			SDL_Texture* buffer_texture;
			Console console;
		};
	}
}
