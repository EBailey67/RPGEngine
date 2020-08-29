#include "debugsystem.hpp"
#include "../core.hpp"

void PositionDebug()
{
    if (Position::hasDebugDraw)
    {
        auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        auto view = registry.view<Position, Active>();

        for (auto& entity : view)
        {
            const auto& pos = view.get<Position>(entity);
            Graphics::SetDrawColor(255, 0, 0, SDL_ALPHA_OPAQUE);

            auto position = activeCamera.FromWorldToScreenView(pos.position.ToFloat());
            Graphics::DrawLineToLayer(Layer::Debug, position.x, position.y, position.x + 20, position.y);

            Graphics::SetDrawColor(0, 255, 0, SDL_ALPHA_OPAQUE);
            Graphics::DrawLineToLayer(Layer::Debug, 
                static_cast<int>(position.x), static_cast<int>(position.y), 
                static_cast<int>(position.x), static_cast<int>(position.y) - 20);
            Graphics::ResetDrawColor();
        }
    }
}

void CameraUpdateDebug()
{
    if (!CameraData::isFollowing)
    {
	    const auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        const auto& event = Events::Event();

        if (event.key.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                activeCamera.position.Set(activeCamera.position.x, activeCamera.position.y + activeCamera.viewRadius.y / 10);
                break;
            case SDLK_DOWN:
                activeCamera.position.Set(activeCamera.position.x, activeCamera.position.y - activeCamera.viewRadius.y / 10);
                break;
            case SDLK_LEFT:
                activeCamera.position.Set(activeCamera.position.x - activeCamera.viewRadius.x / 10, activeCamera.position.y);
                break;
            case SDLK_RIGHT:
                activeCamera.position.Set(activeCamera.position.x + activeCamera.viewRadius.x / 10, activeCamera.position.y);
                break;
            case SDLK_PAGEUP:
                activeCamera.ZoomOut();
                break;
            case SDLK_PAGEDOWN:
            	activeCamera.ZoomIn();
                break;
            default:
            	break;
            }
        }
    }
}

void RectDebug()
{
    if (RectCollider::hasDebugDraw)
    {
	    const auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        auto view = registry.view<Position, RectCollider, Active>();

        for (const auto& entity : view)
        {
            const auto& pos = view.get<Position>(entity);
            const auto& rect = view.get<RectCollider>(entity);

            Graphics::SetDrawColor(255, 0, 0, SDL_ALPHA_OPAQUE);

            SDL_FRect frect = {pos.position.x + rect.rect.x, pos.position.y + rect.rect.y, rect.rect.w, rect.rect.h};
            auto position = activeCamera.FromWorldToScreenRect(frect);
            Graphics::SetDrawColor(255, 255, 0, 120);
            Graphics::DrawFillRectToLayer(Layer::Debug, &position);
            Graphics::SetDrawColor(0, 255, 255, 255);
            Graphics::DrawRectToLayer(Layer::Debug, &position);
            Graphics::ResetDrawColor();
        }
    }
}

void DebugMode()
{
	const auto* const state = Events::KeyboardState();
    if (state[SDL_SCANCODE_F5])
    {
        RectCollider::hasDebugDraw = !RectCollider::hasDebugDraw;
        Position::hasDebugDraw = !Position::hasDebugDraw;
        TileGrid::hasDebugDraw = !TileGrid::hasDebugDraw;
        CameraData::isFollowing = !CameraData::isFollowing;
    }
}
