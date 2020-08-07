#include "camerasystem.hpp"
#include "../core.hpp"

void CameraCreate()
{
	const auto id = registry.create();
    auto &camera = registry.emplace<Camera>(id);
    camera.position = {500, 500};
    camera.viewRadius = {600, 600};
    camera.UpdateWindowSize(Graphics::Window());
}

void CameraFollow()
{
    if (CameraData::isFollowing)
    {
	    const auto playerView = registry.view<Player, Position, Sprite>();
        auto &&[position, sprite] = playerView.get<Position, Sprite>(*playerView.begin());
	    const auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());

        activeCamera.position.Set(position.position.x + sprite.rect.w * sprite.scale.x / 2,
                                  position.position.y + sprite.rect.h * sprite.scale.y / 2);
    }
}
