#include "RenderSystem.h"

void RenderSystem::Update(entt::registry& Registry)
{
    auto View = Registry.view<TextureComponent, TransformComponent>();

    for (auto& Entity : View)
    {
        auto& Transform = Registry.get<TransformComponent>(Entity);
        auto& MapTile = Registry.get<TextureComponent>(Entity);

        // Do any processing here
    }
}

void RenderSystem::Draw(entt::registry& Registry, const SDL_Renderer* renderer)
{
    auto View = Registry.view<TextureComponent, TransformComponent>();

    for (auto& Entity : View)
    {
        auto& Transform = Registry.get<TransformComponent>(Entity);
        auto& MapTile = Registry.get<TextureComponent>(Entity);

        SDL_Rect destRect;
        destRect.x = Transform.Position.x;
        destRect.y = Transform.Position.y;
        destRect.w = static_cast<int>(MapTile.srcRect.w * Transform.Size.x());
        destRect.h = static_cast<int>(MapTile.srcRect.h * Transform.Size.y());

        TextureManager::Draw(MapTile.texture, MapTile.srcRect, destRect, SDL_FLIP_NONE);
    }
}
