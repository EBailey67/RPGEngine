#pragma once

#include <map>
#include <SDL.h>
#include <unordered_map>


#include "Console.h"
#include "CharCell.hpp"

namespace Term
{
	namespace SDL
	{
		// This is an SDL specific implementation to handle the
		// rendering of a Console class.
		class Context
		{
		public:
			Context(int width, int height);
			~Context();

			[[nodiscard]] int TileWidth() const;
			[[nodiscard]] int TileHeight() const;
			void Print(const CharCell ch, const int x, const int y);
			void Print();
			void Render(int x, int y) const;

			Console& GetConsole();
		private:
			int twidth, theight;
			SDL_Texture* buffer_texture;
			Console console;
			std::unordered_map<CharCell, SDL_Texture*, CharCellHashFunction> cache;
		};
	}
}
