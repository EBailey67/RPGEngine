#include "camerasystem.hpp"
#include "../core.hpp"

void CameraCreate()
{
	const auto id = registry.create();
    auto &camera = registry.emplace<Camera>(id);
    camera.position = {500, 500};
    camera.viewRadius = {24 * 16, 24 * 9};
    camera.UpdateWindowSize(Graphics::Window());
}

void CameraViewUpdate(const float xradius, const float yradius)
{
    const auto id = registry.create();
    auto& camera = registry.emplace<Camera>(id);
	
    camera.viewRadius = { xradius, yradius };
}

void CameraFollow()
{
    if (CameraData::isFollowing)
    {
	    const auto playerView = registry.view<Player, Position, Sprite>();
        auto &&[position, sprite] = playerView.get<Position, Sprite>(*playerView.begin());
	    const auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());

        activeCamera.position.Set(position.position.x + static_cast<float>(sprite.rect.w) * sprite.scale.x / 2,
                                  position.position.y + static_cast<float>(sprite.rect.h) * sprite.scale.y / 2);
    }
}
