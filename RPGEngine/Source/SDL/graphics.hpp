#pragma once

#include <array>
#include <utility>

#include <SDL_render.h>

#include "config.hpp"
#include "fwd.hpp"

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

    ~Graphics()
    {
    }

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
        SDL_RenderClear(m_renderer);
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

    static void RenderToLayerF(Layer layer, SDL_Texture *texture, const SDL_Rect *src_rect = nullptr,
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

private:
    static inline SDL_Window *m_window = nullptr;
    static inline SDL_Renderer *m_renderer = nullptr;
    static inline std::array<SDL_Texture *, SDL_RENDER_LAYERS> m_layers{};
    static inline Layer m_currentLayer = reset_layer;
};

