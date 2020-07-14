#pragma once

#include <entt/entt.hpp>
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Managers/TextureManager.h"

struct PF_MapTile
{
public:
	void Create(entt::registry& registry, int srcX, int srcY, int xPos, int yPos, const char* filePath);
};



inline void PF_MapTile::Create(entt::registry& registry, int srcX, int srcY, int xPos, int yPos, const char* filePath)
{
	SDL_Texture* texture = TextureManager::LoadTexture(filePath);
	if (texture == nullptr)
	{
		std::cout << "Error loading texture :" << filePath << std::endl;
		return;
	}

	auto mapTileEntity = registry.create();

	SDL_Rect srcRect{ srcX, srcY, 32, 32 };
	Position2D pos;
	pos.x = xPos;
	pos.y = yPos;

	registry.emplace<TextureComponent>(mapTileEntity, texture, srcRect);
	registry.emplace<TransformComponent>(mapTileEntity,
		pos,
		Vector2D(0.0f, 0.0f),
		Vector2D(2.0f, 2.0f), 0.0f);
}