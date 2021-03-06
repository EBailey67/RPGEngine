#pragma once

#include <algorithm>
#include <cmath>

#include <SDL_rect.h>
#include <SDL_video.h>

#include "../Utility/Vector2D.h"

constexpr int ZOOM_LEVELS = 5;
const float zoom_level[ZOOM_LEVELS] = {0.25f, 0.5f, 1.0f, 2.0f, 4.0f};
constexpr int DEFAULT_ZOOM_LEVEL = 2;

class Camera
{

public:
    Camera() = default;
    
    Camera(const Vector2Df &position, const Vector2Df &viewRadius) : 
        position(position), viewRadius(viewRadius)
    {
    }

    Camera(const float x, const float y, const float w, const float h) :
		position{x, y}, viewRadius{w, h}
    {
    }

    [[nodiscard]] Vector2Df FromScreenViewToWorld(const Vector2Df& point) const
    {
        Vector2Df world{(point.x * (2 * viewRadius.x) / static_cast<float>(m_windowWidth) + position.x - viewRadius.x),
                       -(point.y * (2 * viewRadius.y) / static_cast<float>(m_windowHeight) + position.y + viewRadius.y)};

    	return world;
    }

    [[nodiscard]] Vector2Di FromWorldToScreenView(const Vector2Df& point) const
    {
        Vector2Di screen{
            std::round(((point.x - (position.x - viewRadius.x)) * static_cast<float>(m_windowWidth)) / (2.0f * viewRadius.x)),
            -std::round(((point.y - (position.y + viewRadius.y)) * static_cast<float>(m_windowHeight)) / (2.0f * viewRadius.y))};

    	return screen;
    }

    [[nodiscard]] SDL_Rect FromWorldToScreenRect(const SDL_FRect& rect) const
    {
	    const auto left_up = FromWorldToScreenView({rect.x, rect.y + rect.h});
        return {static_cast<int>(std::round(left_up.x)), static_cast<int>(std::round(left_up.y)),
                static_cast<int>(std::round(rect.w * static_cast<float>(m_windowWidth) / (2.0f * viewRadius.x))),
                static_cast<int>(std::round(rect.h * static_cast<float>(m_windowHeight) / (2.0f * viewRadius.y)))};
    }

    void UpdateWindowSize(SDL_Window *window) noexcept
    {
        SDL_GetWindowSize(window, &m_windowWidth, &m_windowHeight);

    	UpdateViewRadius();
        viewRadius.y = viewRadius.x / (static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight));
    }

	[[nodiscard]] float GetZoom() const noexcept
    {
	    return zoom_level[current_zoom_level];
    }

    float ZoomIn()
    {
	    current_zoom_level = std::max(0, current_zoom_level - 1);
    	UpdateViewRadius();
    	
	    return zoom_level[current_zoom_level];
    }

    float ZoomOut()
    {
	    current_zoom_level = std::min(ZOOM_LEVELS - 1, current_zoom_level + 1);
    	UpdateViewRadius();
    	
	    return zoom_level[current_zoom_level];
    }

    [[nodiscard]] bool Contains(const Vector2Df &point) const noexcept
    {
        return (point.x >= (position.x - viewRadius.x) && (point.x <= (position.x + viewRadius.x))) &&
               (point.y >= (position.y - viewRadius.y) && (point.y <= (position.y + viewRadius.y)));
    }

    [[nodiscard]] bool Contains(const SDL_FRect &rect) const noexcept
    {
        return !(rect.x + rect.w < position.x - viewRadius.x) || (rect.x > position.x + viewRadius.x) ||
                 (rect.y > position.y + viewRadius.y) || (rect.y + rect.h < position.y - viewRadius.y);
    }

    [[nodiscard]] bool Contains(const SDL_Rect &rect) const noexcept
    {
        return !((rect.x + rect.w < 0) || (rect.x > m_windowWidth) || (rect.y + rect.h < 0) ||
                 (rect.y > m_windowHeight));
    }

	void UpdateViewRadius()
    {
	    viewRadius.x = 24 * 16 * zoom_level[current_zoom_level];
    	viewRadius.y = 24 * 9 * zoom_level[current_zoom_level];
        viewRadius.y = viewRadius.x / (static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight));
    }

    Vector2Df position{};
    Vector2Df viewRadius{};

private:
	
    int m_windowWidth{};
    int m_windowHeight{};
	int current_zoom_level = DEFAULT_ZOOM_LEVEL;
};

struct CameraData
{
    inline static bool isFollowing = true;
};
