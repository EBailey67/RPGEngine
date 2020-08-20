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

			[[nodiscard]] int TileWidth() const;
			[[nodiscard]] int TileHeight() const;
			void Print(const Char ch, const int x, const int y) const;
			void Print();
			void Render(int x, int y) const;

			Console& GetConsole();
		private:
			int twidth, theight;
			SDL_Texture* buffer_texture;
			Console console;
		};
	}
}
