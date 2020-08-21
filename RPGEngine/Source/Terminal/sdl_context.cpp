#include "sdl_context.hpp"
#include <iostream>
#include <SDL_image.h>
#include <stdexcept>


#include "../core.hpp"
#include "../SDL/graphics.hpp"
#include "../SDL/resource_loader.hpp"


namespace Term
{
	constexpr auto console_fontid = "console_font";
	namespace SDL
	{
		Context::Context(const int width, const int height) :
			twidth(0), theight(0),
			console(width, height)
		{
			std::cout << "Constructing Term::SDL::Context()\n";
			buffer_texture = SDL_CreateTexture(Graphics::Renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * 8, height * 8);
			TTF_Font *font = fontCache.load(console_fontid, ResourceLoader::Font("resources/fonts/consola.ttf", 14));
			if (!TTF_FontFaceIsFixedWidth(font))
			{
				throw std::runtime_error("Font for console must be a fixed-width font.");
			}
			
			// Get the size of the fixed-spaced font
			const SDL_Color defColor{ 255,255,255,0 };
			auto* const glyphTexture = ResourceLoader::Glyph(font, 'X', defColor);
			SDL_QueryTexture(glyphTexture, nullptr, nullptr, &twidth, &theight);
			SDL_DestroyTexture(glyphTexture);
		}

		Context::~Context()
		{
			if (buffer_texture != nullptr)
				SDL_DestroyTexture(buffer_texture);
		}

		int Context::TileWidth() const
		{
			return twidth;
		}

		int Context::TileHeight() const
		{
			return theight;
		}


		void Context::Print(const Char ch, const int x, const int y) const
		{
			auto tw = TileWidth() ;
			auto th = TileHeight();
			SDL_Rect bgRect = {x * tw, y * th, tw, th};

			if (ch.Ascii() == 0)
			{
				Graphics::SetDrawColor(ch.BgColor());
				SDL_RenderFillRect(Graphics::Renderer(), &bgRect);
				return;
			}

			TTF_Font *font = fontCache.resource(console_fontid);
			auto* const labelTex = ResourceLoader::Glyph(font, ch.Ascii(), ch.FgColor());

			int gw, gh;
			SDL_QueryTexture(labelTex, nullptr, nullptr, &gw, &gh);
			SDL_Rect dst = {x * tw, y *th, gw , gh};

			auto fg = ch.FgColor();

			Graphics::SetDrawColor(ch.BgColor());
			SDL_RenderFillRect(Graphics::Renderer(), &bgRect);
			SDL_SetTextureBlendMode(labelTex, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(Graphics::Renderer(), labelTex, nullptr, &dst);
			SDL_DestroyTexture(labelTex);
		
		}

		void Context::Print()
		{
			if (!console.IsDirty())
				return;

			PROFILE_FUNCTION();		// Only profile if we're actually doing something to reduce noise.

			const auto l = Graphics::GetCurrentLayer();
			SDL_SetTextureBlendMode(buffer_texture, SDL_BLENDMODE_BLEND);
			SDL_SetRenderTarget(Graphics::Renderer(), buffer_texture);
			SDL_Texture *rt = SDL_GetRenderTarget(Graphics::Renderer());
			Graphics::TargetClear();
			
			for (auto y = 0; y < console.Height(); ++y)
				for (auto x = 0; x < console.Width(); ++x)
					Print(console.GetCh(x, y), x, y);

			Graphics::RenderTarget(l);
			console.Clean();
		}

		void Context::Render(const int x, const int y) const
		{
			int gw, gh;
			SDL_QueryTexture(buffer_texture, nullptr, nullptr, &gw, &gh);

			SDL_Rect dstRect = {x, y, gw, gh};
			Graphics::RenderToLayer(Layer::UI, buffer_texture, nullptr, &dstRect, SDL_FLIP_NONE);
		}

		Console& Context::GetConsole()
		{
			return console;
		}
	}
}
