#pragma once

#include <array>
#include <utility>

#include <SDL_render.h>

#include "config.hpp"
#include "fwd.hpp"
#include "../Utility/Vector2D.h"

enum class Layer
{
    None = -1,
    Floor = 0,
    Walls = 1,
    Objects = 2,
    Mobs = 3,
    Debug = 5,
    UI = 6,
};

class Graphics
{
    friend Game;

    static constexpr const auto reset_layer = Layer::None;

private:
    Graphics() = default;

    ~Graphics() = default;

    static void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        DestroyData();
        m_window = window;

        m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
        if (!m_renderer)
        {
            DebugError("Graphics", SDL_GetError());
            SDL_THROW();
        }

        if (SDL_RenderTargetSupported(m_renderer) == SDL_FALSE)
        {
            SDL_THROW();
        }

    	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        CreateLayers();
    }

    static void CreateLayers()
    {
        auto [w, h] = WindowSize();
        for (auto &layer : m_layers)
        {
            layer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
            if (!layer)
            {
                SDL_THROW();
            }
            SDL_SetTextureBlendMode(layer, SDL_BLENDMODE_BLEND);
        }
    }

    static void DestroyLayers()
    {
        for (auto &layer : m_layers)
        {
            if (layer)
            {
                SDL_DestroyTexture(layer);
            }
        }
    }

    static void OnRenderStart()
    {
        // clear all layers textures.
        for (auto &layer : m_layers)
        {
            if (SDL_SetRenderTarget(m_renderer, layer))
            {
                SDL_THROW();
            }
            TargetClear();
        }
        // reset renderer
        ResetRenderer();

        m_currentLayer = reset_layer;
    }

    static void RenderPresent()
    {
        SDL_RenderPresent(m_renderer);
    }

    static void DrawLayers()
    {
        ResetRenderer();
        TargetClear();
        for (auto* layer : m_layers)
        {
            if (SDL_RenderCopy(m_renderer, layer, nullptr, nullptr))
            {
                SDL_THROW();
            }
        }
    }

    static void DestroyData()
    {
        if (m_window)
        {
            DestroyLayers();
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
        }
    }

public:
    static void RenderTarget(Layer layer)
    {
        if (SDL_SetRenderTarget(m_renderer, m_layers.at(static_cast<int>(layer))))
        {
            SDL_THROW();
        }
        m_currentLayer = layer;
    }

    static void ResetRenderer()
    {
        if (SDL_SetRenderTarget(m_renderer, nullptr))
        {
            SDL_THROW();
        }
    }

    static void TargetClear()
    {
    	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        SDL_RenderClear(m_renderer);
    }

	static std::pair<int, int> WindowSize() noexcept
    {
        SSECS_ASSERT(m_window);
        int w, h;
        SDL_GetWindowSize(m_window, &w, &h);
        return std::make_pair(w, h);
    }

    static SDL_Window *Window() noexcept
    {
        return m_window;
    }

    static SDL_Renderer *Renderer() noexcept
    {
        return m_renderer;
    }

    static void RenderToLayer(const Layer layer, SDL_Texture *texture, const SDL_Rect *src_rect = nullptr,
                              const SDL_Rect *dst_rect = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderCopyEx(m_renderer, texture, src_rect, dst_rect, 0, nullptr, flip))
        {
            SDL_THROW();
        }
    }

    static void RenderToLayerF(const Layer layer, SDL_Texture *texture, const SDL_Rect *src_rect = nullptr,
                               const SDL_FRect *dst_rect = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
        if (SDL_RenderCopyExF(m_renderer, texture, src_rect, dst_rect, 0, nullptr, flip))
        {
            SDL_THROW();
        }
    }

    static void DrawRectToLayer(Layer layer, const SDL_Rect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawRect(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawRectToLayerF(Layer layer, const SDL_FRect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawRectF(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawFillRectToLayer(Layer layer, const SDL_Rect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
        if (SDL_RenderFillRect(m_renderer, rect))
        {
            SDL_THROW();
        }
    }
    static void DrawFillRectToLayerF(Layer layer, const SDL_FRect *rect)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderFillRectF(m_renderer, rect))
        {
            SDL_THROW();
        }
    }

    static void DrawLineToLayer(Layer layer, int x1, int y1, int x2, int y2)
    {
        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

        if (SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2))
        {
            SDL_THROW();
        }
    }

    static void SetDrawColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = SDL_ALPHA_OPAQUE) noexcept
    {
        SDL_SetRenderDrawColor(Graphics::Renderer(), r, g, b, a);
    }

    static void ResetDrawColor() noexcept
    {
        SDL_SetRenderDrawColor(Graphics::Renderer(), 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    }

	static int DrawCircleToLayer(const Layer layer, const int x, const int y, const int radius)
	{
	    int offsetx = 0;
    	int offsety = radius;
    	int d = radius - 1;
	    int status = 0;

        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }
    	
	    while (offsety >= offsetx) 
        {
	        status += SDL_RenderDrawPoint(m_renderer, x + offsetx, y + offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsety, y + offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsetx, y + offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsety, y + offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsetx, y - offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x + offsety, y - offsetx);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsetx, y - offsety);
	        status += SDL_RenderDrawPoint(m_renderer, x - offsety, y - offsetx);

	        if (status < 0) 
            {
	            status = -1;
	            break;
	        }

	        if (d >= 2*offsetx) 
            {
	            d -= 2*offsetx + 1;
	            offsetx +=1;
	        }
	        else if (d < 2 * (radius - offsety)) 
            {
	            d += 2 * offsety - 1;
	            offsety -= 1;
	        }
	        else 
            {
	            d += 2 * (offsety - offsetx - 1);
	            offsety -= 1;
	            offsetx += 1;
	        }
	    }

	    return status;
	}


	static int DrawFillCircleToLayer(const Layer layer, const int x, const int y, const int radius)
	{
	    int offsetx = 0;
    	int offsety = radius;
    	int d = radius - 1;
	    int status = 0;

        if (layer != m_currentLayer)
        {
            RenderTarget(layer);
        }

	    while (offsety >= offsetx) 
        {

	        status += SDL_RenderDrawLine(m_renderer, x - offsety, y + offsetx,
	                                     x + offsety, y + offsetx);
	        status += SDL_RenderDrawLine(m_renderer, x - offsetx, y + offsety,
	                                     x + offsetx, y + offsety);
	        status += SDL_RenderDrawLine(m_renderer, x - offsetx, y - offsety,
	                                     x + offsetx, y - offsety);
	        status += SDL_RenderDrawLine(m_renderer, x - offsety, y - offsetx,
	                                     x + offsety, y - offsetx);

	        if (status < 0) 
            {
	            status = -1;
	            break;
	        }

	        if (d >= 2*offsetx) 
            {
	            d -= 2*offsetx + 1;
	            offsetx +=1;
	        }
	        else if (d < 2 * (radius - offsety)) 
            {
	            d += 2 * offsety - 1;
	            offsety -= 1;
	        }
	        else 
            {
	            d += 2 * (offsety - offsetx - 1);
	            offsety -= 1;
	            offsetx += 1;
	        }
	    }

	    return status;
	}

	static void DrawTriangleToLayer(const Layer layer, Vector2D& pos1, Vector2D& pos2, Vector2D& pos3)
	{
		DrawTriangleToLayer(layer, 
            static_cast<int>(pos1.x), static_cast<int>(pos1.y), 
            static_cast<int>(pos2.x), static_cast<int>(pos2.y), 
            static_cast<int>(pos3.x), static_cast<int>(pos3.y));
	}

	static void DrawTriangleToLayer(const Layer layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
	{
		DrawLineToLayer(layer, x1, y1, x2, y2);
		DrawLineToLayer(layer, x2, y2, x3, y3);
		DrawLineToLayer(layer, x3, y3, x1, y1);
	}

	static void DrawFillTriangleToLayer(const Layer layer, Vector2D& pos1, Vector2D& pos2, Vector2D& pos3)
	{
		DrawFillTriangleToLayer(layer, 
            static_cast<int>(pos1.x), static_cast<int>(pos1.y), 
            static_cast<int>(pos2.x), static_cast<int>(pos2.y), 
            static_cast<int>(pos3.x), static_cast<int>(pos3.y));
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	static void DrawFillTriangleToLayer(const Layer layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
	{
		auto drawline = [&](const int sx, const int ex, const int ny) { DrawLineToLayer(layer, sx, ny, ex ,ny); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
    	
		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1);
		if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1);
		if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i < dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}

	
private:
    static inline SDL_Window *m_window = nullptr;
    static inline SDL_Renderer *m_renderer = nullptr;
    static inline std::array<SDL_Texture *, SDL_RENDER_LAYERS> m_layers{};
    static inline Layer m_currentLayer = reset_layer;
};

